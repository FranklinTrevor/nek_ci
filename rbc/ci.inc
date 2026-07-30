static int ciMode = 0;

#define EPS 9.8e-2

void ciSetup(MPI_Comm comm, setupAide &options)
{
  options.setArgs("POLYNOMIAL DEGREE", std::string("7"));
  options.setArgs("CUBATURE POLYNOMIAL DEGREE", std::string("10"));
  options.setArgs("RESTART FILE NAME", std::string("rbc.fld+time=0"));
  // options.setArgs("SOLUTION OUTPUT INTERVAL", "-1");
  // options.setArgs("NUMBER TIMESTEPS", std::string("-1"));
  options.setArgs("END TIME", std::string("4"));
  options.setArgs("ADVECTION TYPE", "CONVECTIVE+CUBATURE");
  options.setArgs("DT", std::string("5e-3"));
}

void ciTestErrors(double time, int tstep, const occa::memory &o_wT)
{
  if(!nrs->lastStep) 
    return;

  auto mesh = nrs->meshV;

  nrs->scalar->o_solution("wtavg").copyFrom(o_wT, mesh->Nlocal);

  const dfloat weighted_wT = platform->linAlg->innerProd(mesh->Nlocal,
                                                         o_wT,
                                                         mesh->o_Jw,
                                                         platform->comm.mpiComm());

  const dfloat int_wT = weighted_wT / mesh->volume;

  const dfloat Nu = 1.0 + sqrt(Ra * Pr) * int_wT;

  //Togni R, Cimarelli A, De Angelis E. Journal of Fluid Mechanics. 2015;782:380-404. doi:10.1017/jfm.2015.547
  const auto NuRef = 5.0;
  const auto NuErr = std::abs(Nu - NuRef);

  CiEvalTest("Nu_v (Err:" + to_string_f(NuErr) + ")", fabs(NuErr) < EPS);
}
