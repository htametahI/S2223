//g++ SortCode.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT -o SortData
// S1873
// SortCode.cxx
// S. A. Gillespie
// 12/12/2019
// E. J. Williams
// 6/9/2020
// D. Yates
// 26/11/2024



#define Sortcode_cxx
#include "SortCodeTest.h"

using namespace std;

Double_t r2d = TMath::RadToDeg();
Double_t d2r = TMath::DegToRad();

bool tofGate(Double_t Time)
{
  if (Time < 750 && Time > 350)
    return true; // ToF gate. Change !!!
  else
    return false;
}

bool BGtofGate(Double_t Time)
{
  if ((Time < 0 && Time > -3000) || (Time < 4000 && Time > 1000))
    return true; // Random ToF gate. Change !!!
  else
    return false;
}


bool gate1D(Double_t value, Double_t min, Double_t max)
{
  if (min < value && value < max)
    return true;
  else
    return false;
}

bool mgate2D(TEmmaHit *hit_one, TCutG *cut)
{
  return cut->IsInside(hit_one->GetPosition().X(), hit_one->GetPosition().Y());
}

bool loadCutG(char const *cutfile)
{ // 2D Gate Loader. Code only uses mass gate if cut file given
  TFile *cuts = new TFile(cutfile, "READ");
  //massGate = (TCutG * ) cuts->Get("mass");
  siicGate = (TCutG *)cuts->Get("siIC");
  return true;
}

bool goodIC(double tempIC[])
{
  //  double gatemin[4] = {2400, 2400, 2400, 200}; // IC Segment Minimum energy gates. Change !!!
  //  double gatemax[4] = {3200, 3200, 3200, 3200}; // IC Segment Maximum energy gates. Change !!!
  double gatemin[4] = {400, 440, 440, 420}; // gh
  double gatemax[4] = {550, 580, 600, 580}; // gh ... for 21Ne recoils in S1873
  bool good = true;
  for (int i = 0; i < 4; i++)
  {
    if (good == false)
      break;
    if (tempIC[i] < gatemax[i] && gatemin[i] < tempIC[i])
      good = true;
    else
      good = false;
  }
  return good;
}



double pi = TMath::Pi();
double xp[2] = {-16.0, 16.0};     // PGAC X-position 1D gate, minimum and maximum . Change !!!
double yp[2] = {-20.0, +22.0};  // PGAC T-position 1D gate, minimum and maximum. Change !!!
double tigs3T[2] = {350, 550}; // TIGRESS - S3 Timing. Change !!!
double tigtigT[2] = {-100, 100}; // TIGRESS - TIGRESS Timing. Change !!!
double tig_emma_T[2] = {800,1000}; 		// TIGRESS-EMMA Timing
double tig_emmasi_T[2] = {800,1000};    // TIGRESS-EMMA Si Timing 
double tig_emmaic_T[2] = {800,1000};    // TIGRESS-EMMA IC Timing 
double s3_emma_T[2] = {350, 620};		// S4-EMMA Timing 
double gamma_350_E[2] = {340, 350}; 	// 350keV Gammas 
double gamma_1274_E[2] = {1250, 1300}; 	// 1274keV gamma 26Mg
double gamma_1808_E[2] = {1789, 1840};	// 1808=keV gamma 26Mg 


double tempIC;
double tempICArray[4];
double tempIC2D[5][4];
double s3_x_offset = -0.0; // S3 x offset (mm) will need to be recalculated with beam
double s3_y_offset = -0.0; // S3 y offset (mm) will need to be recalculated with beam
double s3_z_offset = 0.0; // S3 z offset (mm) will need to be calculated.
double particle_beta = 0.0681;
double thetalab;
double exc;
double ekin;
double recoiltheta;
double thetacm;
double rekin;

bool suppTig = false;
bool suppAdd = false;
bool s3EnergyDiff = false; //this is for comparing s3 ring vs sector energy differences
bool siicCut = false;      //checking if Si/ic is within the cut we loaded for it

void SortCode::SortData(char const *afile, char const *calfile, char const *outfile, char const *target = "NULL"){

  Initialise();

  TFile *analysisfile = new TFile(afile, "READ"); //Opens Analysis Trees
  if (!analysisfile->IsOpen())
  {
    printf("Opening file %s failed, aborting\n", afile);
    return;
  }

  printf("File %s opened\n", afile);
//  TChain *AnalysisTree = (TChain *)analysisfile->Get("AnalysisTree");
  TChain *AnalysisTree = new TChain("AnalysisTree");
  AnalysisTree->Add(afile); 
  long analentries = AnalysisTree->GetEntries();
  const char *testval = "NULL";
  printf("Trees Loaded (line 117)"); 
  // Checks for branches and sets pointers
  TEmma *emma = 0;
  if (AnalysisTree->FindBranch("TEmma"))
  {
    AnalysisTree->SetBranchAddress("TEmma", &emma);
  }
  else
  {
    cout << "Branch 'TEmma' not found! TEmma variable is NULL pointer" << endl;
  }

  TTigress *tigress = 0;
  if (AnalysisTree->FindBranch("TTigress"))
  {
    AnalysisTree->SetBranchAddress("TTigress", &tigress);
  }
  else
  {
    cout << "Branch 'TTigress' not found! TTigress variable is NULL pointer" << endl;
  }

  TS3 *s3 = 0;
  if (AnalysisTree->FindBranch("TS3"))
  {
    AnalysisTree->SetBranchAddress("TS3", &s3);
    AnalysisTree->SetBranchAddress("TS3", &s3);
  }
  else
  {
    cout << "Branch 'TS3' not found! TS3 variable is NULL pointer" << endl;
  }

  // ====== Energy lost already calculated in LISE++ ==================
  //TSRIM *srim_22Ne_in_LiF = new TSRIM;
  //srim_22Ne_in_LiF->ReadEnergyLossFile("22Ne_in_LiF.txt");
  
  double EBeam = 64.14166; // Beam energy half way into the target
  //double EBeam = 1000; 
  //printf("Beam energy: %f MeV\n", EBeam);


  //adjust beam energy for middle of the target based on 22Ne through LiF.
  //double targetThicknessugcm2 = 500.;//approximately 500 ug/cm2 LiF targets
  //double targetThicknessum = 6.078; //500 ug/cm2 LiF are approx 6 um thick.

  //EBeam = srim_22Ne_in_LiF->GetAdjustedEnergy(EBeam*1000,targetThicknessum/2.0,0.001)/1000.;
  //*1000 and /1000 convert from MeV to keV and back. we use /2.0 to get the beam energy in the middle of the target. 0.001 is the integration step size


  //printf("Adjusted Beam energy: %f MeV\n", EBeam);

  TReaction *reac = new TReaction("22Ne", "7Li", "3H", "26Mg", EBeam, 0, true);
  //TReaction *reac = new TReaction("6Li", "2H", "4He", "4He", EBeam, 0, true);

  //Defining Pointers
  TEmmaHit *em_hit, *ic_hit, *si_hit, *ssb_hit, *trigger_hit, *anode_hit;
  TTigressHit *tig_hit, *add_hit, *add_hit2;
  TS3Hit *ring_hit, *sector_hit, *s3hit;
  TVector3 s3pos, recoil_vec;

  if(s3){
    //s3->SetFrontBackTime(140); // Needed to build S3 pixels properly
    s3->SetFrontBackTime(1000);
    s3->SetFrontBackEnergy(0.9);
  }


  //S3 detector rotation
  double s3_phi_offset = -90.0*TMath::Pi()/180.; //detector rotated 90deg in chamber
  double ekin_chan;
  double exc_chan; 
  s3_phi_offset -= TMath::Pi()/16.; //GRSISort seems to rotate incorrectly by 1 sector width

  printf("Reading calibration file: %s\n", calfile);
  TChannel::ReadCalFile(calfile);
  
  
	int	tig_emma_counter = 0; 
	int	tig_counter = 0;
	int emma_counter = 0;
	int	counter_ssb = 0;
	int	counter_na22 = 0;
	
  double_t particle_betaDoppler = 0.0681; //approx 26Mg at 10.949 MeV exc recoil after carbon backing.
  cout << endl << "particle_beta: " << particle_betaDoppler << endl;

  std::cout << "\nLoading cuts now";
  TFile *cutFile = new TFile("PID_Cuts.root");
  
  TCutG *F19_cut = (TCutG*)cutFile->Get("F19_cut");
  TCutG *Na24_cut = (TCutG*)cutFile->Get("Ne22_cut");//the cut itself is called "Ne22" but it is actually 24Na.
  TCutG *Mg26_cut = (TCutG*)cutFile->Get("Mg26_cut");
  TCutG *Al26_cut = (TCutG*)cutFile->Get("Al26_cut");
  
  
  
  //TFile *cutFile = new TFile("26MgCut.root");
  //TCutG *Mg26_cut = (TCutG*)cutFile->Get("CUTG");
  TH1F *s3emmaTOF, *excitation, *tritonEnergy, *excEnergyNoCalibration, *s3ETest; 
  s3emmaTOF = new TH1F("s3emmaTOF", "S3_Time - EMMA Time;S3-EMMA Time Difference (ns)", 20000, -10000, 10000);
  excitation = new TH1F("Excitation", "Excitation Energy;Excitation Energy (MeV)", 1000, -2, 100);
  tritonEnergy = new TH1F("TritonEnergy", "Triton Energy;Triton Energy (keV)", 1000, -1000, 20000);
  excEnergyNoCalibration = new TH1F("Exc Energy in Channels", "Exc Energy; Channel", 1000, -1000, 10000);
  s3ETest = new TH1F("S3 Raw Energy (before loop)", "S3 Raw Energy; S3 Raw Energy (keV)", 1000, -1000, 20000); 
  

  printf("\nSorting analysis events...\n");
  for (int jentry = 0; jentry < analentries; jentry++) {
  
	if (jentry % 10000 == 0) 
		cout << setiosflags(ios::fixed) << "Entry " << jentry << " of " << analentries << ", " << 100 * jentry / analentries << "% complete" << "\r" << flush;
   
    AnalysisTree->GetEntry(jentry);
    
    //reset the exc energy
	exc = -1; //that way we don't accidentally fill things with the previous exc energy
	reac->SetExcEnergy(0); 
		
	// ======== just plotting s3 energy here before everything else happens: 
	/*
	for (int i = 0; i < s3->GetPixelMultiplicity(); i++){
        s3hit = s3->GetPixelHit(i);
        s3ETest->Fill(s3hit->GetEnergy());
        s3Charge->Fill(s3hit->GetCharge()); 
    }
    */
    if (emma) {
		if (s3 && emma->GetICMultiplicity() > 0) {
			exc = -1; 
			for (int j = 0; j < emma->GetMultiplicity(); j++) {
				em_hit = emma->GetEmmaHit(j); 
				for (int i = 0; i < s3->GetPixelMultiplicity(); i++){
					s3hit = s3->GetPixelHit(i); 
					s3pos = s3hit->GetPosition(s3_phi_offset, true);
					s3emmaTOF->Fill(s3hit->GetTime() - em_hit->GetTime());
					s3ETest->Fill(s3hit->GetEnergy()); 
					if(s3hit->GetTime() - em_hit->GetTime() > 400 && s3hit->GetTime() - em_hit->GetTime() < 550){
						thetalab = s3pos.Theta();
						// cout << "\ntheta lab is" << thetalab; 
						ekin = s3hit->GetEnergy();
						ekin_chan = s3hit->GetCharge(); 
						//exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2); //Energy conversion from keV to MeV
						exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2);
						exc_chan = reac->GetExcEnergy(ekin_chan, thetalab,2); 
						excEnergyNoCalibration -> Fill(exc_chan); 
                        excitation->Fill(exc);   							// Filling excitation energy gated on EMMA
						tritonEnergy->Fill(ekin);
					}
				}
			}
		} 
	} 
} // this is the end bracket for jentries. 
	

  
  printf("\nend of main loops for events");
  cout << "Entry " << analentries << " of " << analentries << ", 100% complete" << endl;fflush(stdout);
  cout << "Event sorting complete" << endl;

	std::cout	<< analentries << " events, " << tig_emma_counter << " containing TIGRESS + EMMA, " << tig_counter << " containing TIGRESS, " << emma_counter << " containing EMMA"
						<< std::endl;
	//std::cout	<< "Na22: " << counter_na22 << "\t SSB: " << counter_ssb << std::endl;

  cout << "Writing histograms to " << outfile << endl;fflush(stdout);

  TFile *myfile = new TFile(outfile, "RECREATE");
  myfile->cd();

  TDirectory *s3dir = myfile->mkdir("S3");
  s3dir->cd();
  s3List->Write();
  s3ETest->Write(); 
  s3emmaTOF->Write();
  excitation->Write(); 
  tritonEnergy->Write();  
  excEnergyNoCalibration->Write(); 
  myfile->cd();
  

  /* 
        TDirectory * ggatedir = myfile->mkdir("Gamma_Gated_Focal_Plane");
        ggatedir->cd();
        ggatedList->Write();
        myfile->cd();*/

  myfile->Write();
  myfile->Close();
} 
int main(int argc, char **argv)
{

  SortCode *mysort = new SortCode();

  char const *afile;
  char const *outfile;
  char const *calfile;
  char const *target;
  char const *cutfile;
  printf("Starting sortcode\n");

  std::string grsi_path = getenv("GRSISYS"); // Finds the GRSISYS path to be used by other parts of the grsisort code
  if (grsi_path.length() > 0)
  {
    grsi_path += "/";
  }
  // Read in grsirc in the GRSISYS directory to set user defined options on grsisort startup
  grsi_path += ".grsirc";
  gEnv->ReadFile(grsi_path.c_str(), kEnvChange);
  TParserLibrary::Get()->Load();

  // Input-chain-file, output-histogram-file
  if (argc == 1){
    cout << "Insufficient arguments, provide analysis tree" << endl;
    return 0;
  }else if (argc == 2){
    afile = argv[1];
    calfile = "CalibrationFile.cal";
    outfile = "Histograms.root";
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->SortData(afile, calfile, outfile);
  }else if (argc == 3){
    afile = argv[1];
    calfile = argv[2];
    outfile = "Histograms.root";
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->SortData(afile, calfile, outfile);
  }else if (argc == 4){
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->SortData(afile, calfile, outfile);
  }else if (argc == 5){
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    target = argv[4];
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\nTarget: %s\n", afile, calfile, outfile, target);
    mysort->SortData(afile, calfile, outfile, target);
  }else{
    printf("Incorrect arguments\n");
    printf("SortData analysis_tree cal_file(optional) out_file(optional)\n");
    return 0;
  }

  return 0;
}

