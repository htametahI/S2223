{

	TChain *oak = new TChain("AnalysisTree");
	oak->Add("~/S2223/AnalysisTrees/UsingCalibrationFilePostExperiment/analysis61000_000.root"); //change me!!
	TS3 *s3 = 0;
	oak->SetBranchAddress("TS3",&s3);
	TChannel::ReadCalFile("CalibrationFilePostExperiment.cal"); //change me!!


	TH1F *hist = new TH1F("hist","hist;Energy (keV);Counts",16384,0,16384);

	for (int i = 0; i < oak->GetEntries(); i++){
		oak->GetEntry(i);

		for (int j = 0; j < s3->GetPixelMultiplicity(); j++){
			auto *s3_hit = s3->GetPixelHit(j);
			hist->Fill(s3_hit->GetEnergy());
		}


	}


	TFile *outFile = new TFile("test.root","recreate"); //change me!!
	hist->Write();
	outFile->Close();


}