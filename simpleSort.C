
{

  double ekin;
  double thetalab;
  double exc;
  TVector3 s3pos;
  TS3Hit *s3hit;
  TEmmaHit *em_hit;
  double s3_emma_T[2] = {350, 620}; // S3-EMMA Timing
  double particle_beta = 0.0681;
  TReaction *reac =
      new TReaction("22Ne", "7Li", "3H", "26Mg", 64.14166, 0, true);
  TChain *oak = new TChain("AnalysisTree");
  oak->Add("~/S2223/AnalysisTrees/UsingCalibrationFilePostExperiment/"
           "analysis60831_000.root"); // change me!!
  TS3 *s3 = 0;
  oak->SetBranchAddress("TS3", &s3);
  TEmma *emma = 0;
  oak->SetBranchAddress("TEmma", &emma);
  TChannel::ReadCalFile("CalibrationFilePostExperiment.cal"); // change me!!

  TH1F *hist = new TH1F("hist", "hist;Energy (keV);Counts", 16384, 0, 16384);
  TH1F *mg26ExcEmmaS3 = new TH1F(
      "mg26Exc", "EMMA-S3 Gated 26Mg Excitation energy; Energy(MeV); Counts",
       230, -10, 13);

  for (int i = 0; i < oak->GetEntries(); i++) {
    oak->GetEntry(i);
    // if (s3 && emma) {
    //   for (int j = 0; j < s3->GetPixelMultiplicity(); j++) {
    //     auto *s3_hit = s3->GetPixelHit(j);
    //     hist->Fill(s3_hit->GetEnergy());
    //   }
    // }

    if (s3) {
      s3->SetMultiHit();
      for (int i = 0; i < emma->GetMultiplicity(); i++) {
        auto em_hit = emma->GetEmmaHit(i);
        for (int j = 0; j < s3->GetPixelMultiplicity(); j++) {
          s3hit = s3->GetPixelHit(j);
          hist->Fill(s3hit->GetEnergy());
          s3pos = s3hit->GetPosition(-101.25 * TMath::Pi() / 180.,
                                     true); // rotation, s3 offset
          thetalab = s3pos.Theta();         // lab angle
          ekin = s3hit->GetEnergy();        // triton energy
          exc = reac->GetExcEnergy(
              ekin * 1e-3, thetalab,
              2); // 26Mg Excitation energy, Energy conversion from keV to MeV
                  // (1e-3), two-body reaction
          mg26ExcEmmaS3->Fill(exc);
        }
      }
    }
  }


  TFile *outFile = new TFile("test.root", "recreate"); // change me!!
  hist->Write();
  mg26ExcEmmaS3->Write();
  outFile->Close();
}