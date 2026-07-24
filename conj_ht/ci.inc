#include <cmath>
#include <vector>

#include "pointInterpolation.hpp"

static int ciMode = 0;
static double numCVODERHS = 0.0;

constexpr dfloat solutionTolerance = 1.0e-4;
constexpr dfloat interpolationTolerance = 1.0e-4;

void ciSetup(MPI_Comm comm, setupAide& options)
{
  options.setArgs("POLYNOMIAL DEGREE", "7");
  options.setArgs("RESTART FILE NAME", "restart.fld+time=0");
  options.setArgs("SOLUTION OUTPUT INTERVAL", "0");
  options.setArgs("END TIME", "1");
  options.setArgs("DT", "1.0e-2");
  options.setArgs("SUBCYCLING STEPS", "0");

  if (ciMode == 2) {
    options.setArgs("SUBCYCLING STEPS", "1");
  }

  options.setArgs("BDF ORDER", "2");
  options.setArgs("ADVECTION TYPE", "CONVECTIVE+CUBATURE");
  options.setArgs("FLUID VELOCITY SOLVER TOLERANCE", "1e-12");
  options.setArgs("FLUID PRESSURE SOLVER TOLERANCE", "1e-10");
  options.setArgs("SCALAR00 SOLVER TOLERANCE", "1e-12");
  options.setArgs("VARIABLEPROPERTIES", "TRUE");

  if (ciMode == 3) {
    options.setArgs("CVODE", "TRUE");
    options.setArgs("CVODE INTEGRATOR", "BDF");
    options.setArgs("CVODE MAX TIMESTEPPER ORDER", "3");
    options.setArgs("CVODE EPS LIN", "0.05");
    options.setArgs("CVODE STOP TIME", "TRUE");
    options.setArgs("CVODE HMAX RATIO", "1");
    options.setArgs("CVODE MAX STEPS", "10000");
    options.setArgs("CVODE RELATIVE TOLERANCE", "1e-06");
    options.setArgs("CVODE ABSOLUTE TOLERANCE", "1e-08");
    options.setArgs("CVODE GMR VECTORS", "20");
    options.setArgs("SCALAR00 SOLVER", "CVODE");
  }
}

void ciTestPointInterpolation(nrs_t* nrs, double time, int tstep)
{
  if (ciMode != 1) {
    return;
  }

  auto& scalar = nrs->scalar;
  auto mesh = scalar->mesh("temperature");

  pointInterpolation_t interp(mesh, platform->comm.mpiComm());

  std::vector<dfloat> x = {4.0, 4.0, 4.0};
  std::vector<dfloat> y = {-0.25, 0.25, 1.0};
  std::vector<dfloat> z = {0.5, 0.5, 0.5};

  const dlong nPoints = x.size();

  interp.setPoints(x, y, z);
  interp.find();

  auto o_Tint = platform->device.malloc<dfloat>(nPoints);

  interp.eval(1,
              scalar->fieldOffset(),
              scalar->o_solution("temperature"),
              nPoints,
              o_Tint);

  std::vector<dfloat> Tint(nPoints);
  o_Tint.copyTo(Tint.data(), nPoints);

  const std::vector<dfloat> Texpected = {
      134.80357142857144,
      14.10044642857143,
      125.42857142857143};

  bool pass = true;

  for (dlong i = 0; i < nPoints; ++i) {
    pass &= std::abs(Tint[i] - Texpected[i]) < interpolationTolerance;
  }

  if (platform->comm.mpiRank() == 0) {
    std::cout << "Point interpolation values: expected = ";

    for (dlong i = 0; i < nPoints; ++i) {
      std::cout << Texpected[i] << " ";
    }

    std::cout << "actual = ";

    for (dlong i = 0; i < nPoints; ++i) {
      std::cout << Tint[i] << " ";
    }

    std::cout << "\n";
  }

  CiEvalTest("temperature point interpolation", pass);
}

void ciTestErrors(nrs_t* nrs, double time, int tstep)
{
  if (nrs->scalar->cvode) {
    numCVODERHS += nrs->scalar->cvode->numRHSEvals();
    numCVODERHS += nrs->scalar->cvode->numLinIters();
  }

  if (!nrs->lastStep) {
    return;
  }

  ciTestPointInterpolation(nrs, time, tstep);

  const int rank = platform->comm.mpiRank();

  nrs->copyToNek(time, tstep);
  nek::userchk();

  auto err = nek::ptr<double>("errors");

  const double vxErr = err[0];
  const double temperatureErr = err[1];

  if (rank == 0) {
    printf("solution errors: vx=%g T=%g\n", vxErr, temperatureErr);
  }

  CiEvalTest("velocity L2 error = " + to_string_f(vxErr),
             vxErr < solutionTolerance);

  CiEvalTest("temperature L2 error = " + to_string_f(temperatureErr),
             temperatureErr < solutionTolerance);

  if (nrs->scalar->cvode) {
    const double averageCVODERHS = tstep > 0 ? numCVODERHS / tstep : numCVODERHS;
    const dfloat expectedCVODERHS = 13.11;

    if (rank == 0) {
      printf("CVODE RHS evaluations per timestep: expected = %g, actual = %g\n",
             expectedCVODERHS,
             averageCVODERHS);
    }

    CiEvalTest("CVODE RHS evaluations per timestep = " + to_string_f(averageCVODERHS),
               averageCVODERHS <= 1.1 * expectedCVODERHS);
  }
}
