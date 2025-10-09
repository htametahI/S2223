// PlotTwoKinematicCurves_Safe.C
#include "TReaction.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include <vector>
#include <iostream>
#include <cmath>

// Try to find a bracket [lo,hi] in which f changes sign by scanning
static bool FindBracket(std::function<double(double)> f,
                        double &lo, double &hi,
                        double lo_start, double hi_start,
                        int steps = 100)
{
  lo = lo_start; hi = hi_start;
  double flo = f(lo), fhi = f(hi);
  if (std::isfinite(flo) && std::isfinite(fhi) && flo*fhi <= 0) return true;

  // scan logarithmically-ish over a wide range
  double Emin = std::max(1.0, lo_start*0.1);
  double Emax = hi_start*5.0;
  double dE   = (Emax - Emin)/steps;
  double prevE = Emin, prevF = f(prevE);
  for (int i=1;i<=steps;i++){
    double Ei = Emin + i*dE;
    double Fi = f(Ei);
    if (std::isfinite(prevF) && std::isfinite(Fi) && prevF*Fi <= 0){
      lo = prevE; hi = Ei; return true;
    }
    prevE = Ei; prevF = Fi;
  }
  return false;
}

// Solve E_keV at given theta (deg) such that GetExcEnergy(E,theta,ejectile) = Ex_target (MeV).
static bool SolveEforEx(TReaction& R, double theta_deg, int ejectileIndex,
                        double Ex_target_MeV, double &E_keV_out)
{
  const double th = theta_deg * TMath::DegToRad();
  auto f = [&](double E_keV){
    return R.GetExcEnergy(E_keV*1e-3, th, ejectileIndex) - Ex_target_MeV;
  };

  double lo, hi;
  if (!FindBracket(f, lo, hi, /*lo_start*/100., /*hi_start*/15000., /*steps*/200))
    return false;

  // bisection
  double flo = f(lo), fhi = f(hi);
  for (int it=0; it<60; ++it){
    double mid = 0.5*(lo+hi);
    double fmid = f(mid);
    if (!std::isfinite(fmid)) return false;
    if (flo*fmid <= 0) { hi = mid; fhi = fmid; }
    else               { lo = mid; flo = fmid; }
  }
  E_keV_out = 0.5*(lo+hi);
  return true;
}

void PlotTwoKinematicCurves_Safe()
{
  // --- your setup ---
  const double Ebeam_MeV = 64.14166; // mid-target beam energy (adjust to your SRIM-corrected value)
  const double tmin = 0.0, tmax = 180.0;
  const int    nT   = 241;
  const int    ej   = 2;            // ejectile index (triton) for TReaction

  TReaction R("22Ne","7Li","3H","26Mg", Ebeam_MeV, 0.0, true);

  std::vector<double> x0, y0, x12, y12;

  int nfail0 = 0, nfail12 = 0;
  for (int i=0;i<nT;i++){
    double th = tmin + i*(tmax - tmin)/(nT - 1);
    double EkeV;

    // Ex = 0
    if (SolveEforEx(R, th, ej, 0.0, EkeV)) { x0.push_back(th);  y0.push_back(EkeV); }
    else                                    { nfail0++; }

    // Ex = 12 MeV
    if (SolveEforEx(R, th, ej, 12.0, EkeV)) { x12.push_back(th); y12.push_back(EkeV); }
    else                                     { nfail12++; }
  }

  std::cout << "Ex=0:  points="  << x0.size()  << ", failures=" << nfail0  << "\n";
  std::cout << "Ex=12: points="  << x12.size() << ", failures=" << nfail12 << "\n";

  auto g0  = new TGraph((int)x0.size(),  x0.data(),  y0.data());
  auto g12 = new TGraph((int)x12.size(), x12.data(), y12.data());
  g0->SetLineColor(kBlue+1); g0->SetLineWidth(3);
  g12->SetLineColor(kRed+1); g12->SetLineWidth(3);

  auto mg = new TMultiGraph();
  mg->Add(g0,  "L");
  mg->Add(g12, "L");
  mg->SetTitle("^{22}Ne(^{7}Li,t)^{26}Mg;#theta_{lab} (deg);S3 (t) Energy (keV)");

  auto c = new TCanvas("cKin","S3 kinematics",900,650);
  mg->Draw("AL");

  auto leg = new TLegend(0.55,0.75,0.88,0.88);
  leg->AddEntry(g0,  "E_{x} = 0 MeV",  "l");
  leg->AddEntry(g12, "E_{x} = 12 MeV", "l");
  leg->Draw();

  c->Update();
}
