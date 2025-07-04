// g++ SortCode.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lTTigress -lTEmma -lMinuit -lTS3 -lX11 -o SortData
// S2223
// SortCode.cxx
// Y. Qiu
// 06/25/2025

#define Sortcode_cxx
#include "SortCodeQYZ.h"

using namespace std;

Double_t r2d = TMath::RadToDeg();
Double_t d2r = TMath::DegToRad();

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
    // massGate = (TCutG * ) cuts->Get("mass");
    // siicGate = (TCutG *)cuts->Get("siIC");
    return true;
}

bool goodIC(double tempIC[])
{
    double gatemin[4] = {550, 600, 620, 600}; // Gate for 26Mg events for each IC segment
    double gatemax[4] = {670, 720, 750, 710};
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
double xp[2] = {-16.0, 16.0};          // PGAC X-position 1D gate, minimum and maximum . Change !!!
double yp[2] = {-20.0, +22.0};         // PGAC T-position 1D gate, minimum and maximum. Change !!!
double tigs3T[2] = {350, 550};         // TIGRESS - S3 Timing. Change !!!
double tigtigT[2] = {-100, 100};       // TIGRESS - TIGRESS Timing. Change !!!
double tig_emma_T[2] = {800, 1000};    // TIGRESS-EMMA Timing
double tig_emmasi_T[2] = {800, 1000};  // TIGRESS-EMMA Si Timing
double tig_emmaic_T[2] = {800, 1000};  // TIGRESS-EMMA IC Timing
double s3_emma_T[2] = {350, 620};      // S3-EMMA Timing
double gamma_350_E[2] = {340, 350};    // 350keV Gammas
double gamma_1129_E[2] = {1125, 1145}; // 1129keV gmma 26Mg
double gamma_1274_E[2] = {1250, 1300}; // 1274keV gamma 26Mg
double gamma_1808_E[2] = {1805, 1820}; // 1808=keV gamma 26Mg

double tempIC;
double tempICArray[4];
double tempIC2D[5][4];
double s3_x_offset = -0.0; // S3 x offset (mm) will need to be recalculated with beam
double s3_y_offset = -0.0; // S3 y offset (mm) will need to be recalculated with beam
double s3_z_offset = 0.0;  // S3 z offset (mm) will need to be calculated.
double particle_beta = 0.0681;
double thetalab;
double exc;
double ekin;
double recoiltheta;
double thetacm;
double rekin;

bool suppTig = false;
bool suppAdd = false;
bool s3EnergyDiff = false; // this is for comparing s3 ring vs sector energy differences
bool siicCut = false;      // checking if Si/ic is within the cut we loaded for it

void SortCode::SortData(char const *afile, char const *calfile, char const *outfile, char const *target = "NULL")
{
    Initialise();
    TFile *analysisfile = new TFile(afile, "READ"); // Opens Analysis Trees

    if (!analysisfile->IsOpen())
    {
        printf("Opening file %s failed, aborting\n", afile);
        return;
    }

    printf("File %s opened\n", afile);
    TChain *AnalysisTree = new TChain("AnalysisTree");
    AnalysisTree->Add(afile);
    long analentries = AnalysisTree->GetEntries();
    const char *testval = "NULL";
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
    double EBeam = 64.14166; // Beam energy half way into the target

    TReaction *reac = new TReaction("22Ne", "7Li", "3H", "26Mg", 64.14166, 0, true);

    // Defining Pointers
    TEmmaHit *em_hit, *ic_hit, *si_hit, *ssb_hit, *trigger_hit, *anode_hit;
    TTigressHit *tig_hit, *add_hit, *add_hit2;
    TS3Hit *ring_hit, *sector_hit, *s3hit;
    TVector3 s3pos, recoil_vec;

    if (s3)
    {
        s3->SetFrontBackTime(140); // Needed to build S3 pixels properly
        // s3->SetFrontBackTime(1000);
        s3->SetFrontBackEnergy(0.9);
    }

    // S3 detector rotation
    double s3_phi_offset = -90.0 * TMath::Pi() / 180.; // detector rotated 90deg in chamber
    s3_phi_offset -= TMath::Pi() / 16.;                // GRSISort seems to rotate incorrectly by 1 sector width

    printf("Reading calibration file: %s\n", calfile);
    TChannel::ReadCalFile(calfile);

    int tig_emma_counter = 0;
    int tig_counter = 0;
    int emma_counter = 0;
    int counter_ssb = 0;
    int counter_na22 = 0;

    double_t particle_betaDoppler = 0.0681; // approx 26Mg at 10.949 MeV exc recoil after carbon backing.
    cout << endl
         << "particle_beta: " << particle_betaDoppler << endl;

    std::cout << "\nLoading cuts now";
    // TFile *cutFile = new TFile("26MgCut.root");
    // TFile *cutFile = new TFile("PID_Cuts.root");

    // TCutG *F19_cut = (TCutG *)cutFile->Get("F19_cut");
    // TCutG *Na24_cut = (TCutG *)cutFile->Get("Ne22_cut"); // the cut itself is called "Ne22" but it is actually 24Na.
    // TCutG *Mg26_cut = (TCutG *)cutFile->Get("Mg26_cut");
    // TCutG *Al26_cut = (TCutG *)cutFile->Get("Al26_cut");

    TFile *cutFile = new TFile("Mg26_Cut_July3.root");
    TCutG *Mg26_cut = (TCutG *)cutFile->Get("CUTG");

    printf("\nSorting analysis events...\n");
    for (int jentry = 0; jentry < analentries; jentry++)
    {

        if (jentry % 10000 == 0)
            cout << setiosflags(ios::fixed) << "Entry " << jentry << " of " << analentries << ", " << 100 * jentry / analentries << "% complete" << "\r" << flush;

        AnalysisTree->GetEntry(jentry);

        // reset the exc energy
        exc = -1; // that way we don't accidentally fill things with the previous exc energy
        reac->SetExcEnergy(0);

        // ========================================== BASIC S3 HISTOGRAMS ======================================
        if (s3)
        {
            for (int i = 0; i < s3->GetPixelMultiplicity(); i++)
            {
                s3hit = s3->GetPixelHit(i);
                s3E->Fill(s3hit->GetEnergy());
            }
        }

        // ========================================== BASIC TIGRESS HISTOGRAMS ======================================
        // RAW ENERGY: 
        if (tigress)
        {
            for (int i = 0; i < tigress->GetMultiplicity(); i++)
            {
                tig_hit = tigress->GetTigressHit(i);
                if (tig_hit != NULL)    // check if it exsits first 
                {
                    suppTig = tig_hit->BGOFired();  // check if BGO fired, veto the event if so 
                    if (!suppTig && tig_hit->GetEnergy() > 15)  // if BGO not fired and event has more than 15keV of energy, fill the energy hist. 
                    {
                        tigE->Fill(tig_hit->GetEnergy()); 
                    }
                }
            } // end of tigress->GetMultiplicity. 
        }

        // TIGRESS ADDBACK: 
            for (int i = 0; i < tigress->GetAddbackMultiplicity(); i++)
            {
                add_hit = tigress->GetAddbackHit(i);
                suppAdd = add_hit->BGOFired(); // check if BGO fired for this addback event, veto if so 
                if (!suppAdd && add_hit->GetEnergy() > 15)
                {
                    tigAddDoppE->Fill(add_hit->GetDoppler(particle_betaDoppler));
                }
            } // End of tigress->GetAddbackMultiplicity()
        
        //  ========================================== EMMA ======================================
        if (emma)
        {
            tempIC = 0;                 // initialize IC energy accumulator, this is the total energy loss of particle in all the ICs
            for (int i = 0; i < 4; i++) // initilize IC arry for each segment
            {
                tempICArray[i] = 0;
            }

            // ========================================= EMMA-S3 =========================================
            for (int i = 0; i < emma->GetMultiplicity(); i++)
            {
                em_hit = emma->GetEmmaHit(i);
                emmaPgac->Fill(em_hit->GetPosition().X(), em_hit->GetPosition().Y()); // Raw 2D pgac
                emmaXpos->Fill(em_hit->GetPosition().X());                            // Pgac x position
                emmaYpos->Fill(em_hit->GetPosition().Y());                            // pgac y position
                for (int j = 0; j < s3->GetPixelMultiplicity(); j++)
                {
                    s3hit = s3->GetPixelHit(j);
                    s3EmmaTof->Fill(s3hit->GetTime() - em_hit->GetTime());
                    if (s3hit->GetTime() - em_hit->GetTime() > s3_emma_T[0] && s3hit->GetTime() - em_hit->GetTime() < s3_emma_T[1])
                    {
                        emmaS3TimeGatedPgac->Fill(em_hit->GetPosition().X(), em_hit->GetPosition().Y()); // time gated pgac
                    }
                }  
            } // end of EMMA - S3

            // ========================================= IC ============================================
            for (int i = 0; i < emma->GetICMultiplicity(); i++)
            {
                ic_hit = emma->GetICHit(i);
                emmaICSegmentEnergy[ic_hit->GetSegment()]->Fill(ic_hit->GetEnergy());
                emmaICSegment->Fill(ic_hit->GetSegment(), ic_hit->GetEnergy());
                tempIC += ic_hit->GetEnergy();                               // this is the total energy deposited in 4 IC segments
                tempICArray[ic_hit->GetSegment() - 1] = ic_hit->GetEnergy(); // this is the energy deposited in each segment. Index 0 is IC seg 1 (GetSegment() returns 1,2,3...)
            }

            // IC and Si:
            for (int j = 0; j < emma->GetSiMultiplicity(); j++)
            {
                si_hit = emma->GetSiHit(j);
                emmaICSumVSi->Fill(si_hit->GetEnergy(), tempIC);
                emmaICSumVSiPlusIC->Fill(si_hit->GetEnergy() + tempIC, tempIC);
            }

            // IC data gating on 1808 keV gamma ray from 26Mg: 
            

            // PID gated PGAC:
            for (int k = 0; k < emma->GetMultiplicity(); k++)
            {
                em_hit = emma->GetEmmaHit(k);
                for (int m = 0; m < emma->GetSiMultiplicity(); m++)
                {
                    si_hit = emma->GetSiHit(m);
                    if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC))
                    {
                        pgac26MgPID->Fill(em_hit->GetPosition().X(), em_hit->GetPosition().Y());
                        pgacXPos26MgPID->Fill(em_hit->GetPosition().X());
                        // tigress coincidence:
                        if (tigress) // why is negative emma x position inserted here in the previous code?
                        {
                            for (int j = 0; j < tigress->GetAddbackMultiplicity(); j++)
                            {
                                add_hit = tigress->GetAddbackHit(j);
                                addDopp26MgPID->Fill(add_hit->GetDoppler(particle_betaDoppler));
                            }
                        }

                        tigress->ResetAddback();
                    }
                }
            }



            // EMMA - S3 - PID (UNFINISHED)
            tigress->ResetAddback();
            for (int i = 0; i < emma->GetSiMultiplicity(); i++)
            {
                si_hit = emma->GetSiHit(i);
                if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC))
                {
                    for (int j; j < s3->GetPixelMultiplicity(); j++)
                    {
                        cout << "here?" << endl;
                        s3hit = s3->GetPixelHit(j);
                        // if (s3hit->GetTime() - si_hit->GetTime() > s3_emma_T[0] && s3hit->GetTime() - si_hit->GetTime() < s3_emma_T[1] && tigress)
                        for (int k; k < tigress->GetAddbackMultiplicity(); k++)
                        {
                            cout << "here" << endl;
                            add_hit = tigress->GetAddbackHit(k);
                            addDopp26MgPIDS3T->Fill(add_hit->GetDoppler(particle_betaDoppler));
                        }
                        // tigress->ResetAddback();
                    }
                }
            }
        }
    } // end of jentries loop
    printf("\nEnd of main event loops");
    cout << "Entry " << analentries << " of " << analentries << " , 100% complete" << endl;
    fflush(stdout);
    cout << "Event sorting complete, WOHOO!" << endl;
    std::cout << analentries << " events, " << tig_emma_counter << " containing TIGRESS + EMMA, " << tig_counter << " containing TIGRESS, " << emma_counter << " containing EMMA"
              << std::endl;

    cout << "Writing histograms to " << outfile << endl;
    fflush(stdout);

    // Organize Histograms
    TFile *myfile = new TFile(outfile, "RECREATE");
    myfile->cd();

    // S3
    TDirectory *s3Dir = myfile->mkdir("S3");
    s3Dir->cd();
    s3List->Write();
    myfile->cd();

    // TIGRESS
    TDirectory *tigDir = myfile->mkdir("TIGRESS");
    tigDir->cd();
    tigList->Write();
    myfile->cd();

    // EMMA
    TDirectory *emmaDir = myfile->mkdir("EMMA");
    emmaDir->cd();
    emmaList->Write();
    myfile->cd();

    // PID
    TDirectory *pidDir = myfile->mkdir("PID");
    pidDir->cd();
    PIDList->Write();
    myfile->cd();

    // Write out the Histogram file
    myfile->Write();
    myfile->Close();
} // end of SortCode

// main function here
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
    if (argc == 1)
    {
        cout << "Insufficient arguments, provide analysis tree" << endl;
        return 0;
    }
    else if (argc == 2)
    {
        afile = argv[1];
        calfile = "CalibrationFile.cal";
        outfile = "Histograms.root";
        printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
        mysort->SortData(afile, calfile, outfile);
    }
    else if (argc == 3)
    {
        afile = argv[1];
        calfile = argv[2];
        outfile = "Histograms.root";
        printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
        mysort->SortData(afile, calfile, outfile);
    }
    else if (argc == 4)
    {
        afile = argv[1];
        calfile = argv[2];
        outfile = argv[3];
        printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
        mysort->SortData(afile, calfile, outfile);
    }
    else if (argc == 5)
    {
        afile = argv[1];
        calfile = argv[2];
        outfile = argv[3];
        target = argv[4];
        printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\nTarget: %s\n", afile, calfile, outfile, target);
        mysort->SortData(afile, calfile, outfile, target);
    }
    else
    {
        printf("Incorrect arguments\n");
        printf("SortData analysis_tree cal_file(optional) out_file(optional)\n");
        return 0;
    }

    return 0;
} // end of main()