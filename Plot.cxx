void PlotPGAC(const char* filename = "analysis.root") {
  // Open the ROOT file
  TFile* f = new TFile(filename, "READ");
  if (!f || f->IsZombie()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  // Get the tree
  TTree* tree = (TTree*)f->Get("AnalysisTree");
  if (!tree) {
    std::cerr << "Could not find AnalysisTree in file!" << std::endl;
    return;
  }

  // Set up TEmma pointer
  TEmma* emma = nullptr;
  tree->SetBranchAddress("TEmma", &emma);

  // Create histograms
  TH1F* xPos = new TH1F("xPos", "PGAC X Position;X [mm];Counts", 200, -100, 100);
  TH1F* yPos = new TH1F("yPos", "PGAC Y Position;Y [mm];Counts", 200, -100, 100);
  TH2F* pgac = new TH2F("pgac", "PGAC X vs Y;X [mm];Y [mm]", 200, -100, 100, 200, -100, 100);

  Long64_t nentries = tree->GetEntries();
  std::cout << "Processing " << nentries << " entries..." << std::endl;

  // Loop through entries
  for (Long64_t i = 0; i < nentries; ++i) {
    tree->GetEntry(i);

    if (!emma) continue;

    for (int j = 0; j < emma->GetMultiplicity(); ++j) {
      TEmmaHit* hit = emma->GetEmmaHit(j);
      if (!hit) continue;

      TVector3 pos = hit->GetPosition();
      xPos->Fill(pos.X());
      yPos->Fill(pos.Y());
      pgac->Fill(pos.X(), pos.Y());
    }
  }

  // Draw histograms
  TCanvas* c1 = new TCanvas("c1", "PGAC Histograms", 1200, 400);
  c1->Divide(3,1);
  c1->cd(1); xPos->Draw();
  c1->cd(2); yPos->Draw();
  c1->cd(3); pgac->Draw("COLZ");

  std::cout << "Done. Histograms plotted." << std::endl;
}
