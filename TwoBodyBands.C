// MakeTwoBodyBand.C
#include <TFile.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCutG.h>
#include <TCanvas.h>
#include <TReaction.h>
#include <vector>

// Solve for triton energy at given theta (deg) so that Ex(E,theta)=Ex_target
static double Et_from_Ex(TReaction* R, double theta_deg, double Ex_target){
  double lo=0.0, hi=20.0;                 // MeV triton search bracket
  for(int it=0; it<50; ++it){
    double mid = 0.5*(lo+hi);
    double Ex_mid = R->GetExcEnergy(mid, theta_deg, /*ejectile index*/ 2);
    if(Ex_mid > Ex_target) lo = mid; else hi = mid;   // note monotonicity
  }
  return 0.5*(lo+hi);
}

void MakeTwoBodyBand(const char* infile="Etheta.root",
                     const char* hname ="s3ETheta",     // X=theta(deg), Y=E_t(MeV)
                     double EbeamMeV=64.14166,
                     double Ex_lo=10.62, double Ex_hi=11.40,
                     double pad_dE=0.30,                // MeV padding for res/straggling
                     double th_min=132, double th_max=162,
                     double th_step=1.0,
                     const char* outCutG="TwoBodyBand_E62to140.TCutG")
{
  // reaction: 22Ne(7Li,t)26Mg; adjust if S3 sees recoil (change ejectile index & axis)
  auto R = new TReaction("22Ne","7Li","3H","26Mg", EbeamMeV, 0, true);

  // open your 2D E vs theta
  TFile* f = TFile::Open(infile);
  TH2* h = (TH2*)f->Get(hname);

  // build the two edges
  std::vector<double> th;
  for(double t=th_min; t<=th_max; t+=th_step) th.push_back(t);

  auto gTop = new TGraph(th.size()); // Ex=Ex_lo + pad (upper edge)
  auto gBot = new TGraph(th.size()); // Ex=Ex_hi - pad (lower edge)

  for(size_t i=0;i<th.size();++i){
    double t = th[i];
    double Et_top = Et_from_Ex(R, t, Ex_lo) + pad_dE;
    double Et_bot = Et_from_Ex(R, t, Ex_hi) - pad_dE;
    gTop->SetPoint(i, t, Et_top);
    gBot->SetPoint(i, t, Et_bot);
  }

  // draw
  TCanvas* c = new TCanvas("c","Two-body band",900,700);
  h->SetTitle("S3 Energy vs Lab Angle;#theta_{lab} (deg);E_{t} (MeV)");
  h->Draw("colz");
  gTop->SetLineColor(kRed); gTop->SetLineWidth(2);
  gBot->SetLineColor(kRed); gBot->SetLineWidth(2);
  gTop->Draw("L same"); gBot->Draw("L same");

  // optional: create & save a TCutG polygon tracing the band
  auto cut = new TCutG("TwoBodyBand", 2*th.size());
  // go along top (increasing theta)
  for(int i=0;i<(int)th.size();++i){
    double x,y; gTop->GetPoint(i,x,y); cut->SetPoint(i, x, y);
  }
  // and back along bottom (decreasing theta)
  for(int i=(int)th.size()-1, j=(int)th.size(); i>=0; --i, ++j){
    double x,y; gBot->GetPoint(i,x,y); cut->SetPoint(j, x, y);
  }
  cut->SetLineColor(kRed); cut->SetLineWidth(2);
  cut->Draw("same");

  // write the cut to the current directory so you can .L and GetObject later
  TFile fout("TwoBodyBand.root","RECREATE");
  cut->Write(outCutG);
  gTop->Write("Edge_Ex10p62");
  gBot->Write("Edge_Ex11p40");
  fout.Close();
}
