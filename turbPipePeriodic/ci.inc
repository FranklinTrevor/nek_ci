#include <math.h>
#include <limits>

static int ciMode = 0;

#define EPS 2e-2

void ciSetup(MPI_Comm comm, setupAide &options)
{
  options.setArgs("POLYNOMIAL DEGREE", std::string("5"));
  options.setArgs("CUBATURE POLYNOMIAL DEGREE", std::string("7"));
  options.setArgs("CONSTANT FLOW RATE", "TRUE");
  options.setArgs("CONSTANT FLOW RATE TYPE", "BULK");
  options.setArgs("CONSTANT FLOW DIRECTION", "Z");
  options.setArgs("SOLUTION OUTPUT INTERVAL", std::string("100"));
  options.setArgs("FLUID VISCOSITY", std::to_string(1.0 / 19000.0));
  options.setArgs("FLUID DENSITY", std::string("1.0"));
  options.setArgs("END TIME", std::string("0.5"));
  options.setArgs("BDF ORDER", std::string("2"));
  options.setArgs("ADVECTION TYPE", "CONVECTIVE+CUBATURE");
  options.setArgs("DT", std::string("1e-3"));
  options.setArgs("TARGET CFL", std::string("4"));
  options.setArgs("SUBCYCLING STEPS", std::string("2"));
  options.setArgs("RESTART FILE NAME", std::string("pipe.fld+time=0"));

  options.setArgs("FLUID PRESSURE SOLVER", "PGMRES+FLEXIBLE");
  options.setArgs("FLUID PRESSURE SOLVER TOLERANCE", std::string("1e-4"));
  options.setArgs("FLUID PRESSURE PRECONDITIONER", "MULTIGRID");
  options.setArgs("FLUID PRESSURE MULTIGRID COARSE SOLVE", "TRUE");
  options.setArgs("FLUID PRESSURE MULTIGRID SMOOTHER", "FOURTHOPTCHEBYSHEV+ASM");
  options.setArgs("FLUID PRESSURE MULTIGRID CHEBYSHEV DEGREE", std::string("3"));
  options.setArgs("FLUID PRESSURE MULTIGRID CHEBYSHEV MAX EIGENVALUE BOUND FACTOR", std::string("1.1"));
  options.setArgs("FLUID PRESSURE INITIAL GUESS", "PROJECTION-ACONJ");
  options.setArgs("FLUID PRESSURE RESIDUAL PROJECTION VECTORS", std::string("8"));

  options.setArgs("FLUID VELOCITY SOLVER TOLERANCE", std::string("1e-6"));
  options.setArgs("FLUID VELOCITY SOLVER", "CG+BLOCK");
  options.setArgs("FLUID VELOCITY INITIAL GUESS", "EXTRAPOLATION");

  options.setArgs("FLUID VELOCITY REGULARIZATION METHOD", "HPFRT");
  options.setArgs("FLUID VELOCITY HPFRT MODES", std::string("1"));
  options.setArgs("FLUID VELOCITY HPFRT STRENGTH", std::string("10"));

  Re_tau = 550;
  zLength = 6;
}

void ciTestErrors(nrs_t *nrs, double time, int tstep)
{
  if (!nrs->lastStep) {
    return;
  }

  auto mesh = nrs->meshV;

  std::vector<int> bidWall = {1};
  auto o_bidWall = platform->device.malloc<int>(bidWall.size(), bidWall.data());

  auto o_Sij = nrs->strainRate();

  auto forces = nrs->aeroForces(o_bidWall, o_Sij);
  o_Sij.free();

  auto o_tmp = platform->deviceMemoryPool.reserve<dfloat>(mesh->Nlocal);
  platform->linAlg->fill(mesh->Nlocal, 1.0, o_tmp);
  const auto areaWall = mesh->surfaceAreaMultiplyIntegrate(o_bidWall, o_tmp);

  const auto utau = sqrt(std::abs(forces->viscousFriction()[0]) / areaWall);
  // https://www.lstm.tf.fau.de/database/simulation-database/
  const auto utauRef = 5.7901316173625254E-02;
  const auto utauErr = std::abs((utau - utauRef) / utauRef);

  if (platform->comm.mpiRank() == 0) {
    printf("tau: %g; relative error = %g\n", utau, utauErr);
  }

  (fabs(utauErr) < EPS) ? (CiPassTest()) : (CiFailTest());
}
