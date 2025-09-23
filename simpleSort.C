
{

  double ekin;
  double thetalab;
  double exc;
  TVector3 s3pos;
  TS3Hit *s3hit;
  TEmmaHit *em_hit;
  TTigressHit *add_hit; 
  double s3_emma_T[2] = {350, 620}; // S3-EMMA Timing
  double particle_beta = 0.0681;
  TReaction *reac =
      new TReaction("22Ne", "7Li", "3H", "26Mg", 64.14166, 0, true);
  TChain *oak = new TChain("AnalysisTree");
  oak->Add("~/S2223/AnalysisTrees/UsingCalibrationFilePostExperiment/"
           "analysis61000_000.root"); // change me!!
  TS3 *s3 = 0;
  oak->SetBranchAddress("TS3", &s3);
  TTigress *tigress = 0; 
  oak->SetBranchAddress("TTigress", &tigress); 
  TEmma *emma = 0;
  oak->SetBranchAddress("TEmma", &emma);
  TChannel::ReadCalFile("CalibrationFilePostExperiment.cal"); // change me!!
  long analentries = oak->GetEntries();

  TH1F *S3Energy = new TH1F("S3 Energy", "S3 Energy;Energy (keV);Counts", 200, 0, 16384);
  TH1F *S3Charge = new TH1F("S3 Charge", "S3 Charge;Charge; Counts", 200, 0, 16384);
  TH1F *mg26ExcEmmaS3 = new TH1F(
      "mg26Exc", "EMMA-S3 Gated 26Mg Excitation energy; Energy(MeV); Counts",
       230, -10, 16);
  TH1F *mg26Exc1808 = new TH1F("Exc1808", "Exc gated on 1808; Counts", 150, 0, 15);
  TH1F *tigEng = new TH1F("tigEng", "tigress energy;Counts", 200, 0, 16384); 

  for (int i = 0; o < analentries; o++) {
    oak->GetEntry(o);
    if (emma) {
      if (s3) {
        for (int i = 0; i < emma->GetSiMultiplicity(); i++) {
          em_hit = emma->GetSiHit(i);
            for (int k = 0; k < tigress->GetAddbackMultiplicity(); k++) {
              add_hit = tigress->GetAddbackHit(k); 
              tigEng->Fill(add_hit->GetDoppler(particle_beta)); 
              if (add_hit->GetDoppler(particle_beta) > 1785 && add_hit->GetDoppler(particle_beta) < 1850) {
                for (int j = 0; j < s3->GetPixelMultiplicity(); j++) {
                  s3hit = s3->GetPixelHit(j); 
                  s3pos = s3hit->GetPosition(-101.25 * TMath::Pi() / 180., true);
                  thetalab = s3pos.Theta();
                  ekin = s3hit->GetEnergy();
                  exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2);
                  mg26Exc1808->Fill(exc); 
                }
              }
            }
        }
      }
    }
  }


  TFile *outFile = new TFile("test.root", "recreate"); // change me!!
  S3Energy->Write();
  S3Charge->Write();
  mg26ExcEmmaS3->Write();
  outFile->Close();
}