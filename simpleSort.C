// MakeKinCut_TKinematics.C
#include "TKinematics.h"
#include "TCutG.h"
#include "TFile.h"
#include "TMath.h"
#include <vector>

// Build a TCutG band around the TKinematics E(θ) curve for the ejectile.
// Units: pass beam energy in MeV. Output cut matches y-axis in keV.
TCutG* MakeKinCut_TKinematics(const char* name,
                              double Ebeam_MeV,           // mid-target beam energy
                              const char* beam="Ne22",
                              const char* target="Li7",
                              const char* ejectile="t",
                              const char* recoil="Mg26",
                              double theta_min=130.,       // deg (S3 coverage)
                              double theta_max=160.,       // deg
                              int    nTheta=241,           // sampling
                              double bandHalf_keV=250.)    // half-thickness of band
{
  // 1) Create the kinematics helper and get E vs θ for the ejectile (index 2)
  TKinematics kin(Ebeam_MeV, beam, target, ejectile, recoil);
  double dθ = (theta_max - theta_min)/(nTheta - 1);
  TSpline3* s = kin.Evslab(theta_min, theta_max, dθ, 2); // 2 = ejectile (right-to-left indexing)

  // 2) Sample the spline and build an upper+lower envelope
  std::vector<double> th(nTheta), e_keV(nTheta);
  for (int i = 0; i < nTheta; ++i) {
    double θ = theta_min + i*dθ;
    double E_MeV = s->Eval(θ);       // same units as beam_energy (MeV)
    th[i]    = θ;
    e_keV[i] = 1000.0*E_MeV;         // convert to keV to match your histograms
  }

  // 3) Polygon: go along upper edge, then back along lower edge
  TCutG* cut = new TCutG(name, 2*nTheta + 1);
  int k = 0;
  for (int i = 0; i < nTheta; ++i) cut->SetPoint(k++, th[i], e_keV[i] + bandHalf_keV);
  for (int i = nTheta - 1; i >= 0; --i) cut->SetPoint(k++, th[i], e_keV[i] - bandHalf_keV);
  cut->SetPoint(k, th[0], e_keV[0] + bandHalf_keV); // close

  cut->SetVarX("#theta_{lab} (deg)");
  cut->SetVarY("S3 E (keV)");
  return cut;
}

// Helper to build & save
void BuildAndSave_KinCut() {
  // Use your actual mid-target beam energy (MeV)
  double Ebeam_MeV = 64.14166; // example from your setup
  TCutG* g = MakeKinCut_TKinematics("S3Kin_t_gs", Ebeam_MeV,
                                    "Ne22","Li7","t","Mg26",
                                    130.,160.,241, /*bandHalf_keV=*/250.);
  TFile f("KinematicCuts.root","UPDATE");
  g->Write();
  f.Close();
  printf("Wrote %s to KinematicCuts.root\n", g->GetName());
}
