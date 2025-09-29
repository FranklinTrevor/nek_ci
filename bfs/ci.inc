#include <math.h>
#include <vector>

#include "postProcessing.hpp"

static int ciMode = 0;

#define EPS 7.2e-3

void ciSetup(MPI_Comm comm, setupAide &options)
{
  options.setArgs("POLYNOMIAL DEGREE", std::string("5"));
  options.setArgs("RESTART FILE NAME", std::string("bfs.fld+time=0"));  
  options.setArgs("END TIME", std::string("0.005"));
  options.setArgs("DT", std::string("1e-04"));
}

void extractLine(nrs_t *nrs, double time, int tstep, occa::memory& o_tauw)
{
  const auto np = (platform->comm.mpiRank == 0) ? 200 : 0;
  const auto offset = np;

  static pointInterpolation_t *interpolator = nullptr;
  static std::vector<dfloat> xp, yp, zp;
  static occa::memory o_tauwp;

  if (!interpolator) {
    auto mesh = nrs->meshV;

    if (np) {
      const auto x0 = 0.0;
      const auto y0 = -0.25;
      const auto z0 = 0.0;

      xp.push_back(x0);
      yp.push_back(y0);
      zp.push_back(z0);

      const auto dx = 10.0 / (np - 1);
      for (int i = 1; i < np - 1; i++) {
        xp.push_back(x0 + i * dx);
        yp.push_back(y0);
        zp.push_back(z0);
      }

      xp.push_back(10.0);
      yp.push_back(y0);
      zp.push_back(z0);
      o_tauwp = platform->device.malloc<dfloat>(offset);
    }

    interpolator = new pointInterpolation_t(nrs);
    interpolator->setPoints(np, xp.data(), yp.data(), zp.data());
    interpolator->find();
  }

  interpolator->eval(1, nrs->fieldOffset, o_tauw, offset, o_tauwp);

  if (platform->comm.mpiRank == 0) {
    std::vector<dfloat> tauwp(np);
    o_tauwp.copyTo(tauwp.data());

    std::ofstream f("profile.dat");
    for (int i = 0; i < np; i++) {
      f << std::scientific << " " << 4.0 * xp[i] << " " << yp[i] << " " << zp[i] << " "
        << 2.0 * tauwp[i + 0 * offset] << std::endl;
    }
    f.close();
  }
}

occa::memory skinFriction(nrs_t *nrs, int nbID, const occa::memory& o_bID, occa::memory& o_Sij)
{
  mesh_t *mesh = nrs->meshV;

  auto o_tau = platform->device.malloc<dfloat>(3 * nrs->fieldOffset);

  viscousShearStress(mesh->Nelements,
                     nrs->fieldOffset,
                     nbID,
                     o_bID,
                     mesh->o_sgeo,
                     mesh->o_vmapM,
                     mesh->o_EToB,
                     nrs->o_mue,
                     o_Sij,
                     o_tau);

  return o_tau;
}

void getCfErr(nrs_t *nrs)
{
  mesh_t *mesh = nrs->meshV;

  std::vector<int> bidWall = {4};
  occa::memory o_bidWall = platform->device.malloc<int>(bidWall.size(), bidWall.data());

  auto o_Sij = platform->o_memPool.reserve<dfloat>(2 * nrs->NVfields * nrs->fieldOffset);
  postProcessing::strainRate(nrs, true, nrs->o_U, o_Sij);
  auto o_tau = skinFriction(nrs, bidWall.size(), o_bidWall, o_Sij);

  // extractLine(nrs, time, tstep, o_tau);

  auto o_err = nrs->cds->o_S.slice(2 * nrs->fieldOffset);

  cfErrKernel(mesh->Nelements,
              bidWall.size(),
              o_bidWall,
              mesh->o_vmapM,
              mesh->o_EToB,
              mesh->o_x,
              o_tau,
              o_err);
}

void ciTestErrors(nrs_t *nrs, double time, int tstep)
{
  if (!nrs->lastStep) {
    return;
  }

  mesh_t *mesh = nrs->meshV;

  auto o_err = nrs->cds->o_S.slice(2 * nrs->fieldOffset);

  auto enorm = platform->linAlg->norm2(mesh->Nlocal, o_err, platform->comm.mpiComm);

  if (platform->comm.mpiRank == 0) {
    printf("Error in Cf: %g\n", enorm);
  }

  (enorm < EPS) ? (CIPASS) : (CIFAIL);
}
