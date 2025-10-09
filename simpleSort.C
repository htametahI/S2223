
// PlotTwoKinematicCurves.C
#include "TReaction.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include <vector>
#include <utility>
#include <iostream>

// Solve for triton energy (keV) at a given theta (deg) that yields target Ex (MeV).
static double SolveEforEx(TReaction& R, double theta_deg, int ejectileIndex,
                          double Ex_target_MeV,
                          double Emin_keV=100., double Emax_keV=15000., int iters=50)
{
  const double th_rad = theta_deg * TMath::DegToRad();
  auto f = [&](double E_keV){
    return R.GetExcEnergy(E_keV*1e-3, th_rad, ejectileIndex) - Ex_target_MeV;
  };
  double lo = Emin_keV, hi = Emax_keV;
  double flo = f(lo),   fhi = f(hi);
  if (flo*fhi > 0) {
    // Fallback: try to widen bracket once
    lo = std::max(10.0, Emin_keV*0.5);
    hi = Emax_keV*1.5;
    flo = f(lo); fhi = f(hi);
    if (flo*fhi > 0) return 0.5*(Emin_keV+Emax_keV); // give something sane
  }
  for (int i=0;i<iters;i++){
    double mid = 0.5*(lo+hi);
    double fmid = f(mid);
    if (flo*fmid <= 0) { hi = mid; fhi = fmid; }
    else               { lo = mid; flo = fmid; }
  }
  return 0.5*(lo+hi);
}

void PlotTwoKinematicCurves()
{
  // --- configure your setup ---
  const double Ebeam_MeV = 64.14166;          // beam energy at (mid) target; update if needed
  const double theta_min  = 130.0;            // S3 coverage
  const double theta_max  = 160.0;
  const int    nTheta     = 241;
  const int    ejectile   = 2;                // 0=beam,1=target,2=ejectile,3=recoil in TReaction

  // Build reaction (inverse kinematics = true)
  TReaction R("22Ne","7Li","3H","26Mg", Ebeam_MeV, 0.0, true);

  // Prepare arrays
  std::vector<double> x(nTheta), y_ex0(nTheta), y_ex12(nTheta);
  for (int i=0;i<nTheta;i++){
    const double th = theta_min + i*(theta_max-theta_min)/(nTheta-1);
    x[i]      = th;
    y_ex0[i]  = SolveEforEx(R, th, ejectile,  0.0);   // keV
    y_ex12[i] = SolveEforEx(R, th, ejectile, 12.0);   // keV
  }

  // Make graphs
  auto g0   = new TGraph(nTheta, x.data(), y_ex0.data());
  auto g12  = new TGraph(nTheta, x.data(), y_ex12.data());
  g0->SetLineColor(kBlue+1);  g0->SetLineWidth(3);
  g12->SetLineColor(kRed+1);  g12->SetLineWidth(3);

  // Draw
  auto c = new TCanvas("cKin","S3 kinematics",900,650);
  g0->SetTitle("^{22}Ne(^{7}Li,t)^{26}Mg kinematics;#theta_{lab} (deg);S3 Energy (keV)");
  g0->Draw("AL");
  g12->Draw("L SAME");

  auto leg = new TLegend(0.55,0.75,0.88,0.88);
  leg->AddEntry(g0,  "E_{x} = 0 MeV",   "l");
  leg->AddEntry(g12, "E_{x} = 12 MeV",  "l");
  leg->Draw();
}
