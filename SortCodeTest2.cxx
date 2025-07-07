// g++ SortCode.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT -o SortData
//  S1873
//  SortCode.cxx
//  S. A. Gillespie
//  12/12/2019
//  E. J. Williams
//  6/9/2020
//  D. Yates
//  26/11/2024

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
  // massGate = (TCutG * ) cuts->Get("mass");
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
double xp[2] = {-16.0, 16.0};          // PGAC X-position 1D gate, minimum and maximum . Change !!!
double yp[2] = {-20.0, +22.0};         // PGAC T-position 1D gate, minimum and maximum. Change !!!
double tigs3T[2] = {350, 550};         // TIGRESS - S3 Timing. Change !!!
double tigtigT[2] = {-100, 100};       // TIGRESS - TIGRESS Timing. Change !!!
double tig_emma_T[2] = {800, 1000};    // TIGRESS-EMMA Timing
double tig_emmasi_T[2] = {800, 1000};  // TIGRESS-EMMA Si Timing
double tig_emmaic_T[2] = {800, 1000};  // TIGRESS-EMMA IC Timing
double s3_emma_T[2] = {350, 620};      // S4-EMMA Timing
double gamma_350_E[2] = {340, 350};    // 350keV Gammas
double gamma_1274_E[2] = {1250, 1300}; // 1274keV gamma 26Mg
double gamma_1808_E[2] = {1789, 1840}; // 1808=keV gamma 26Mg

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
  f

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
  // TSRIM *srim_22Ne_in_LiF = new TSRIM;
  // srim_22Ne_in_LiF->ReadEnergyLossFile("22Ne_in_LiF.txt");

  double EBeam = 64.14166; // Beam energy half way into the target
  // printf("Beam energy: %f MeV\n", EBeam);

  // adjust beam energy for middle of the target based on 22Ne through LiF.
  // double targetThicknessugcm2 = 500.;//approximately 500 ug/cm2 LiF targets
  // double targetThicknessum = 6.078; //500 ug/cm2 LiF are approx 6 um thick.

  // EBeam = srim_22Ne_in_LiF->GetAdjustedEnergy(EBeam*1000,targetThicknessum/2.0,0.001)/1000.;
  //*1000 and /1000 convert from MeV to keV and back. we use /2.0 to get the beam energy in the middle of the target. 0.001 is the integration step size

  // printf("Adjusted Beam energy: %f MeV\n", EBeam);

  TReaction *reac = new TReaction("22Ne", "7Li", "3H", "26Mg", 64.14166, 0, true);

  // Defining Pointers
  TEmmaHit *em_hit, *ic_hit, *si_hit, *ssb_hit, *trigger_hit, *anode_hit;
  TTigressHit *tig_hit, *add_hit, *add_hit2;
  TS3Hit *ring_hit, *sector_hit, *s3hit;
  TVector3 s3pos, recoil_vec;

  if (s3)
  {
    // s3->SetFrontBackTime(140); // Needed to build S3 pixels properly
    s3->SetFrontBackTime(1000);
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
  TFile *cutFile = new TFile("PID_Cuts.root");

  TCutG *F19_cut = (TCutG *)cutFile->Get("F19_cut");
  TCutG *Na24_cut = (TCutG *)cutFile->Get("Ne22_cut"); // the cut itself is called "Ne22" but it is actually 24Na.
  TCutG *Mg26_cut = (TCutG *)cutFile->Get("Mg26_cut");
  TCutG *Al26_cut = (TCutG *)cutFile->Get("Al26_cut");

  // TFile *cutFile = new TFile("26MgCut.root");
  // TCutG *Mg26_cut = (TCutG*)cutFile->Get("CUTG");

  printf("\nSorting analysis events...\n");
  for (int jentry = 0; jentry < analentries; jentry++)
  {

    if (jentry % 10000 == 0)
      cout << setiosflags(ios::fixed) << "Entry " << jentry << " of " << analentries << ", " << 100 * jentry / analentries << "% complete" << "\r" << flush;

    AnalysisTree->GetEntry(jentry);

    // reset the exc energy
    exc = -1; // that way we don't accidentally fill things with the previous exc energy
    reac->SetExcEnergy(0);

    if (tigress)
      if (tigress->GetMultiplicity() > 0)
        tig_counter++;
    if (emma)
      if (emma->GetICMultiplicity() > 0)
        emma_counter++;

    if (tigress && emma)
    {
      if (tigress->GetMultiplicity() > 0 && emma->GetICMultiplicity() > 0)
      {
        tig_emma_counter++;
      }
    }

    // ======== just plotting s3 energy here before everything else happens:
    for (int i = 0; i < s3->GetPixelMultiplicity(); i++)
    {
      s3hit = s3->GetPixelHit(i);
      s3ETest->Fill(s3hit->GetEnergy());
      s3Charge->Fill(s3hit->GetCharge());
    }
    // ===================================================

    // ---------------- TIGRESS energy fill -s----------------

    if (tigress)
    {
      for (int t = 0; t < tigress->GetMultiplicity(); t++)
      {
        tig_hit = tigress->GetTigressHit(t);
        if (tig_hit != NULL)
        {
          suppTig = tig_hit->BGOFired();
          if (!suppTig && tig_hit->GetEnergy() > 15)
          {
            tigE->Fill(tig_hit->GetEnergy());
            tigE_ANum->Fill(tig_hit->GetArrayNumber(), tig_hit->GetEnergy());
            tigE_theta->Fill(tig_hit->GetEnergy(), TMath::RadToDeg() * (tig_hit->GetPosition().Theta()));
            tigRate->Fill(tig_hit->GetTime() / pow(10, 9));
            tigRate_ANum->Fill(tig_hit->GetTime() / pow(10, 9), tig_hit->GetArrayNumber());
          }
        }
      }
    }

    {                          // -------------- S3 singles ----------------
                               // reset some variables for this new event
      siicCut = false;         // to keep track if the si/ic event is within the cut we made
      s3EnergyDiff = false;    // this is to compare if the s3 ring vs sector energies are close to each other
      double ringEnergy = 0.0; // keeping track of total energy in case of split hits
      double sectorEnergy = 0.0;
      s3->SetMultiHit();

      for (int i = 0; i < s3->GetRingMultiplicity(); i++)
      {
        ring_hit = s3->GetRingHit(i);
        rings->Fill(ring_hit->GetRing(), ring_hit->GetEnergy());
        ringEnergy += ring_hit->GetEnergy();
        s3_ringNum->Fill(ring_hit->GetRing());
        s3_E_ringNum->Fill(ring_hit->GetRing(), ring_hit->GetEnergy());
        for (int j = 0; j < s3->GetSectorMultiplicity(); j++)
        {
          // get the ring and sector energies, plot in 2DHisto
          if (s3->GetRingMultiplicity() != 1 || s3->GetSectorMultiplicity() != 1)
            continue; // only look at multiplicity 1 rings and sector hits
          s3_ringNum_secNum->Fill(ring_hit->GetRing(), s3->GetSectorHit(j)->GetSector());
          s3_rings_sectors_singles->Fill(ring_hit->GetEnergy(), s3->GetSectorHit(j)->GetEnergy());
          s3_rings_sectors_singlesT->Fill(s3->GetSectorHit(j)->GetTime() - ring_hit->GetTime());
          s3_rings_sectors_singlesTvT->Fill(ring_hit->GetTime() / (1E9 * 60.), s3->GetSectorHit(j)->GetTime() - ring_hit->GetTime());
          s3_rings_sectors_singlesTvSec->Fill(s3->GetSectorHit(j)->GetSector(), s3->GetSectorHit(j)->GetTime() - ring_hit->GetTime());
          s3_rings_sectors_singlesTvRing->Fill(ring_hit->GetRing(), s3->GetSectorHit(j)->GetTime() - ring_hit->GetTime());
          s3_rings_sectors_singlesTvE->Fill(s3->GetSectorHit(j)->GetEnergy(), s3->GetSectorHit(j)->GetTime() - ring_hit->GetTime());

          if (std::abs(s3->GetSectorHit(j)->GetTime() - ring_hit->GetTime()) > 100.)
            continue;
          ringsT->Fill(ring_hit->GetRing(), ring_hit->GetEnergy());
          sectorsT->Fill(s3->GetSectorHit(j)->GetSector(), s3->GetSectorHit(j)->GetEnergy());
        }
      }

      for (int j = 0; j < s3->GetSectorMultiplicity(); j++)
      {
        sector_hit = s3->GetSectorHit(j);
        sectors->Fill(sector_hit->GetSector(), sector_hit->GetEnergy());
        sectorEnergy += s3->GetSectorHit(j)->GetEnergy();
        s3_secNum->Fill(sector_hit->GetSector());
      }

      // fill this with the sum of rings and sector energies
      s3_rings_sectors->Fill(ringEnergy, sectorEnergy);

      // check if difference in ring and sector energies is less than 100 keV for this event
      if (std::abs(ringEnergy - sectorEnergy) < 100.0)
      {
        s3EnergyDiff = true;
        // continue;
      }

      for (int i = 0; i < s3->GetPixelMultiplicity(); i++)
      {
        s3hit = s3->GetPixelHit(i);
        s3_E->Fill(s3hit->GetEnergy());
        // s3_E->Fill(s3hit->GetCharge());
        // s3pos = s3hit->GetPosition(-101.25 * TMath::Pi() / 180.,true);
        s3pos = s3hit->GetPosition(s3_phi_offset, true);
        s3pos.SetX(s3pos.X() + s3_x_offset);
        s3pos.SetY(s3pos.Y() + s3_y_offset);
        s3pos.SetZ(s3pos.Z() + s3_z_offset);
        s3_E_theta->Fill(s3pos.Theta() * r2d, s3hit->GetEnergy());
        hitmap->Fill(s3pos.X(), s3pos.Y());
        s3Rate->Fill(s3hit->GetTime() / pow(10, 9));
        thetalab = s3pos.Theta();

        ekin = s3hit->GetEnergy();
        // cout << "\ntriton energy is" << ekin;
        exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2);
        reac->SetExcEnergy(exc);
        excE->Fill(exc);
        excE_theta->Fill(s3hit->GetTheta() * r2d, exc);

        //  -------------------------------- S3 ADDBACK BLOCK ---------------------------------------

        if (tigress)
        {
          for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
          {
            add_hit = tigress->GetAddbackHit(t);
            suppAdd = add_hit->BGOFired();
            if (!suppAdd && add_hit->GetEnergy() > 15)
            {

              if (s3hit->GetEnergy() > 600. && s3hit->GetEnergy() < 2500.)
                addT_s3T->Fill(add_hit->GetTime() - s3hit->GetTime());

              if (gate1D((add_hit->GetTime() - s3hit->GetTime()), tigs3T[0], tigs3T[1]))
              {
                thetacm = reac->ConvertThetaLabToCm(thetalab, 2);
                rekin = reac->GetTLabFromThetaCm(TMath::Pi() - thetacm, 3) * 1e3;
                particle_beta = reac->AnalysisBeta(rekin * 1e-3, 3);
                recoiltheta = reac->ConvertThetaCmToLab(thetacm, 3);
                recoil_vec.SetMagThetaPhi(1., recoiltheta, s3pos.Phi() - TMath::Pi());
                addDopp->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec));
                addDopp_ANum_s3->Fill(add_hit->GetArrayNumber(), add_hit->GetDoppler(particle_beta, &recoil_vec));
                addDopp_exc->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec), exc);
                addE_s3_E->Fill(add_hit->GetEnergy(), s3hit->GetEnergy());
                addDopp_s3_E->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec), s3hit->GetEnergy());
                s3_E_theta_coinc->Fill(s3pos.Theta() * r2d, s3hit->GetEnergy());

                if (add_hit->GetDoppler(particle_beta, &recoil_vec) > 1790 && add_hit->GetDoppler(particle_beta, &recoil_vec) < 1840)
                {
                  s3_E_ringNumb_1808_gated->Fill(s3hit->GetRing(), s3hit->GetEnergy());
                }
              }
            }
          }
        }
        // tigres->ResetAddback();
        tigress->ResetAddback();
        // --------------------------------------S3 ADDBACK BLOCK ENDS------------------------------------------------
      }
    }
    // ---------------------------------------EMMA DATA BEGINS--------------------------------------------------
    if (emma)
    { // EMMA singles

      for (int l = 0; l < emma->GetSSBMultiplicity(); l++)
      { // Get SSB hits
        ssb_hit = emma->GetSSBHit(l);
        ssbE[ssb_hit->GetDetector()]->Fill(ssb_hit->GetEnergy());
        ssbET[ssb_hit->GetDetector()]->Fill(ssb_hit->GetTime() / 1e9, ssb_hit->GetEnergy());
        for (int j = 0; j < emma->GetICMultiplicity(); j++)
        {
          ic_hit = emma->GetICHit(j);
          ssbICtof->Fill(ssb_hit->GetTime() - ic_hit->GetTime());
        }

        for (int k = 0; k < emma->GetSiMultiplicity(); k++)
        {
          si_hit = emma->GetSiHit(k);
          ssbSItof->Fill(ssb_hit->GetTime() - si_hit->GetTime());
        }
        for (int k = 0; k < emma->GetTriggerMultiplicity(); k++)
        {
          trigger_hit = emma->GetTriggerHit(k); // EMMA trigger (sent to TIGRESS DAQ)
          ssbEMTtof->Fill(ssb_hit->GetTime() - trigger_hit->GetTime());
          emTrigE->Fill(trigger_hit->GetEnergy());
        }
      }
      // ================================= TIGRESS - EMMA =============================

      if (tigress)
      {
        for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
        {
          add_hit = tigress->GetAddbackHit(t);
          for (int j = 0; j < emma->GetICMultiplicity(); j++)
          {
            ic_hit = emma->GetICHit(j);
            tigICtof->Fill(add_hit->GetTime() - ic_hit->GetTime());
          }
          for (int m = 0; m < emma->GetTdcMultiplicity(); m++)
          {
            anode_hit = emma->GetTdcHit(m);
            tigAnodetof->Fill(add_hit->GetTime() - anode_hit->GetTime());
          }
        }
      }
      tigress->ResetAddback();

      tempIC = 0; // intialise IC arrays
      for (int k = 0; k < 4; k++)
      {
        tempICArray[k] = 0;
        for (int kk = 0; kk < 5; kk++)
          tempIC2D[kk][k] = 0;
      }

      for (int j = 0; j < emma->GetICMultiplicity(); j++)
      { // Get IC Edep
        ic_hit = emma->GetICHit(j);
        tempICArray[ic_hit->GetSegment() - 1] = ic_hit->GetEnergy();
      }

      for (int e = 0; e < emma->GetMultiplicity(); e++)
      {
        em_hit = emma->GetEmmaHit(e);
        for (int one = 0; one < emma->GetTriggerMultiplicity(); one++)
        {
          trigger_hit = emma->GetTriggerHit(one);
          // cout << "hit " << one << ", energy: " << trigger_hit->GetEnergy() << endl;
          if (trigger_hit->GetEnergy() > 200)
            continue;
          tDE->Fill(trigger_hit->GetTime() / pow(10, 9), em_hit->GetTime() - trigger_hit->GetTime()); // get time between EMMA and TIGRESS events
        }
      }

      for (int m = 0; m < emma->GetTdcMultiplicity(); m++)
      { // was GetICMultiplicity
        anode_hit = emma->GetTdcHit(m);
        for (int j = 0; j < emma->GetSSBMultiplicity(); j++)
        {
          ssb_hit = emma->GetSSBHit(j);
          if (ssb_hit->GetDetector() == 0)
            ssb_anode_dt1->Fill(ssb_hit->GetTime() - anode_hit->GetTime());
          if (ssb_hit->GetDetector() == 1)
            ssb_anode_dt2->Fill(ssb_hit->GetTime() - anode_hit->GetTime());
        }
      }

      for (int k = 0; k < emma->GetSiMultiplicity(); k++)
      { // get FP silicon energy
        si_hit = emma->GetSiHit(k);
        siE->Fill(si_hit->GetEnergy());
        siET->Fill(si_hit->GetTime() / 1e9, si_hit->GetEnergy());
      }

      double ICtime = -1;
      for (int j = 0; j < emma->GetICMultiplicity(); j++)
      {
        ic_hit = emma->GetICHit(j);
        icE[ic_hit->GetSegment()]->Fill(ic_hit->GetEnergy());
        icN->Fill(ic_hit->GetSegment(), ic_hit->GetEnergy());
        tempIC += ic_hit->GetEnergy();
        if (ic_hit->GetSegment() == 0)
          ICtime = ic_hit->GetTime();
        if (ic_hit->GetEnergy() < 4000)
          tempICArray[ic_hit->GetSegment() - 1] = ic_hit->GetEnergy(); // IC total energy?
      }

      for (int e = 0; e < emma->GetMultiplicity(); e++)
      {
        em_hit = emma->GetEmmaHit(e);
        xPos->Fill(em_hit->GetPosition().X());
        yPos->Fill(em_hit->GetPosition().Y());
        pgac->Fill(em_hit->GetPosition().X(), em_hit->GetPosition().Y());
        for (int k = 0; k < emma->GetSiMultiplicity(); k++)
        {
          si_hit = emma->GetSiHit(k);
          // pid gated pgac
          if (Na24_cut->IsInside(si_hit->GetEnergy(), tempIC))
          {
            xPos_24Na_gated->Fill(em_hit->GetPosition().X());
            // yPos_24Na_gated->Fill(em_hit->GetPosition().Y());

            // s3 ring and sector info
            for (int ringIndex = 0; ringIndex < s3->GetRingMultiplicity(); ringIndex++)
            {
              ring_hit = s3->GetRingHit(ringIndex);
              s3_E_ringNum_na_pid_gated->Fill(ring_hit->GetRing(), ring_hit->GetEnergy());
            }
          }
          // Mg26 cut begins
          if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC))
          {
            xPos_26Mg_gated->Fill(em_hit->GetPosition().X());
            //============= doppE_26Mg_pidG_xPosG ===============//
            if (tigress && em_hit->GetPosition().X() < 0)
            {
              for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
              {
                add_hit = tigress->GetAddbackHit(t);
                doppE_26Mg_pidg_xPosG->Fill(add_hit->GetDoppler(particle_betaDoppler));
              }
            }

            // s3 loop within Mg pid gate
            for (int s3index = 0; s3index < s3->GetPixelMultiplicity(); s3index++)
            {
              s3hit = s3->GetPixelHit(s3index);
              s3pos = s3hit->GetPosition(-101.25 * TMath::Pi() / 180., true);
              s3pos.SetX(s3pos.X() + s3_x_offset);
              s3pos.SetY(s3pos.Y() + s3_y_offset);
              s3pos.SetZ(s3pos.Z() + s3_z_offset);
              s3_E_theta_Mg_PID_Gated->Fill(s3pos.Theta() * r2d, s3hit->GetEnergy());
              thetalab = s3pos.Theta();
              ekin = s3hit->GetEnergy();
              exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2);
              reac->SetExcEnergy(exc);
              double thetaCm = reac->ConvertThetaLabToCm(thetalab, 2);    // 2 is the ejectile
              excE_thetaCm_Mg_PID_Gated->Fill(thetaCm * r2d, exc * 1000); // in keV

              // tigress loop here
              // ALSO NOT CAUSINHG ADDBACK ISSUES
              for (int tigIndex = 0; tigIndex < tigress->GetAddbackMultiplicity(); tigIndex++)
              {
                add_hit = tigress->GetAddbackHit(tigIndex);
                double timeCut = add_hit->GetTime() - si_hit->GetTime();
                if (800. > timeCut || 1000. < timeCut)
                  continue;
                excE_doppE_Mg_PID_Gated->Fill(add_hit->GetDoppler(particle_betaDoppler), exc * 1000);
              }
            }
          } // end of 26Mg cut

          if (Al26_cut->IsInside(si_hit->GetEnergy(), tempIC))
          {
            xPos_26Al_gated->Fill(em_hit->GetPosition().X());
          }
        }
      }

      if (tempICArray[0] != 0 && tempICArray[1] != 0 && tempICArray[2] != 0 && tempICArray[3] != 0)
      {
        // build 2D IC related histograms
        icSum->Fill(tempIC);
        iC0V1->Fill(tempICArray[0], tempICArray[1]);
        iC0V2->Fill(tempICArray[0], tempICArray[2]);
        iC0V3->Fill(tempICArray[0], tempICArray[3]);
        iC1V2->Fill(tempICArray[1], tempICArray[2]);
        iC1V3->Fill(tempICArray[1], tempICArray[3]);
        iC2V3->Fill(tempICArray[2], tempICArray[3]);
        iC0VSum->Fill(tempICArray[0], tempIC);

        // THIS IS NOT CAUSING ADDBACK PROBLEMS? MAYBE BC IT'S NOT LOOPING?
        if (tigress)
        {
          for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
          {
            if (add_hit->GetEnergy() < 50)
              continue;
            add_hit = tigress->GetAddbackHit(t);
            if (add_hit->GetTime() - ICtime < 800 || add_hit->GetTime() - ICtime > 1050)
              continue;
            if (add_hit->GetDoppler(particle_betaDoppler) > 1250 && add_hit->GetDoppler(particle_betaDoppler) < 1300)
              iC0VSum_1274->Fill(tempICArray[0], tempIC);
          }
        }
        // tigress->ResetAddback();

        for (int k = 0; k < emma->GetSiMultiplicity(); k++)
        { // build hists for IC vs Silicon
          si_hit = emma->GetSiHit(k);
          icSumVSi->Fill(si_hit->GetEnergy(), tempIC);
          icSumVicPlusSi->Fill(tempIC + si_hit->GetEnergy(), tempIC);
          ic0VSi->Fill(si_hit->GetEnergy(), tempICArray[0]);
          ic1VSi->Fill(si_hit->GetEnergy(), tempICArray[1]);
          ic2VSi->Fill(si_hit->GetEnergy(), tempICArray[2]);
          ic3VSi->Fill(si_hit->GetEnergy(), tempICArray[3]);
          if (s3)
          {
            if (s3->GetPixelMultiplicity() > 0)
            {
              icSumVicPlusSi_s3gate->Fill(tempIC + si_hit->GetEnergy(), tempIC);
            }
          }
          // s3 rings in coincidence with emma PID
          for (int ringIndex = 0; ringIndex < s3->GetRingMultiplicity(); ringIndex++)
          {
            if (Na24_cut->IsInside(si_hit->GetEnergy(), tempIC))
            {
              ring_hit = s3->GetRingHit(ringIndex);
              s3_E_ringNum_na_pid_gated->Fill(ring_hit->GetRing(), ring_hit->GetEnergy());
            }
            if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC))
            {
              ring_hit = s3->GetRingHit(ringIndex);
              s3_E_ringNum_mg_pid_gated->Fill(ring_hit->GetRing(), ring_hit->GetEnergy());
            }
          } // end of s3 rings in coinc with emma PID

          // std::cout << "\nTig loop in emma loop";fflush(stdout);

          // =================================== Start of TIGRESS-S3 Block===================================
          if (tigress)
          {
            for (int tigIndex = 0; tigIndex < tigress->GetAddbackMultiplicity(); tigIndex++)
            {
              // std::cout << "\nTig hit number " << tigIndex;fflush(stdout);
              add_hit = tigress->GetAddbackHit(tigIndex);
              // std::cout << "\nEnergy " << add_hit->GetEnergy();fflush(stdout);

              // timing cut on tigress-emma
              double timeCut = add_hit->GetTime() - si_hit->GetTime();

              if (800. > timeCut || 1000. < timeCut)
                continue;

              if (F19_cut->IsInside(si_hit->GetEnergy(), tempIC))
              {
                doppE_19F_pidg->Fill(add_hit->GetDoppler(particle_betaDoppler));
              }
              if (Na24_cut->IsInside(si_hit->GetEnergy(), tempIC))
              {
                doppE_24Na_pidg->Fill(add_hit->GetDoppler(particle_betaDoppler));
              }
              if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC))
              {
                doppE_26Mg_pidg->Fill(add_hit->GetDoppler(particle_betaDoppler));
                for (int s3index = 0; s3index < s3->GetPixelMultiplicity(); s3index++)
                {                                                                         // requires S3 hit now
                  doppE_Mg_pid_s3_gated->Fill(add_hit->GetDoppler(particle_betaDoppler)); // this fills no matter what. Only requirement is that there is *some* S3 hit

                  if (add_hit->GetEnergy() > 1789 && add_hit->GetEnergy() < 1830)
                  {
                    s3hit = s3->GetPixelHit(s3index);
                    s3pos = s3hit->GetPosition(-101.25 * TMath::Pi() / 180., true);
                    s3pos.SetX(s3pos.X() + s3_x_offset);
                    s3pos.SetY(s3pos.Y() + s3_y_offset);
                    s3pos.SetZ(s3pos.Z() + s3_z_offset);

                    s3_E_theta_Mg_PID_1808_gamma_gated->Fill(s3pos.Theta() * r2d, s3hit->GetEnergy());
                    thetalab = s3pos.Theta();
                    ekin = s3hit->GetEnergy();
                    exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2);
                    reac->SetExcEnergy(exc);
                    double thetaCm = reac->ConvertThetaLabToCm(thetalab, 2); // 2 is the ejectile

                    excE_thetaCm_Mg_PID_1808_gamma_gated->Fill(thetaCm * r2d, exc * 1000); // in keV
                  }
                }
              }
              if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC) && add_hit->GetEnergy() > 1789 && add_hit->GetEnergy() < 1830)
              {
                s3_E_gated_PIDG_gamG->Fill(exc);
              }
              if (Al26_cut->IsInside(si_hit->GetEnergy(), tempIC))
              {
                doppE_26Al_pidg->Fill(add_hit->GetDoppler(particle_betaDoppler));
              }
              // after here

              // std::cout << "\ntig multiplicity is " << tigress->GetAddbackMultiplicity();fflush(stdout);
              if (add_hit->GetDoppler(particle_betaDoppler) > 1250 && add_hit->GetDoppler(particle_betaDoppler) < 1300)
                ICvsSiICSum_1274->Fill(tempIC + si_hit->GetEnergy(), tempIC);
              suppAdd = add_hit->BGOFired();
              if (!suppAdd && add_hit->GetEnergy() > 15)
              {
              }
            }
          } // End of if(tigress)
          // =======================================END OF BIG TIGRESS BLOCK======================================
          tigress->ResetAddback();
        }
      } // before here
      // std::cout << "\nMade it hereeee!";fflush(stdout);

      for (int s = 0; s < emma->GetSSBMultiplicity(); s++)
      {
        counter_ssb++;
      }
      for (int e = 0; e < emma->GetSiMultiplicity(); e++)
      {
        si_hit = emma->GetSiHit(e);
        tempIC = 0;
        for (int j = 0; j < emma->GetICMultiplicity(); j++)
        {
          ic_hit = emma->GetICHit(j);
          tempIC += ic_hit->GetEnergy();
        }
        emma_dE_E->Fill(si_hit->GetEnergy(), tempIC);
        if (tigress)
        {
          for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
          {
            add_hit = tigress->GetAddbackHit(t);
            suppAdd = add_hit->BGOFired();

            addemmatof->Fill(add_hit->GetTime() - si_hit->GetTime());
            addE_tof->Fill((add_hit->GetTime() - si_hit->GetTime()), add_hit->GetEnergy());
            addemmatofdet->Fill(add_hit->GetArrayNumber(), add_hit->GetTime() - si_hit->GetTime());

            emma_tig_dt_E->Fill(si_hit->GetTime() - add_hit->GetTime(), add_hit->GetDoppler(particle_betaDoppler));

            if (((add_hit->GetTime() - si_hit->GetTime()) > 820) && ((add_hit->GetTime() - si_hit->GetTime()) < 1000))
            {
              double particle_beta = particle_betaDoppler;
              if (!suppAdd && add_hit->GetEnergy() > 50)
              {
                addDopp_emma->Fill(add_hit->GetDoppler(particle_beta));
                if (add_hit->GetArrayNumber() > 48)
                {
                  addDopp_emma_lampshade->Fill(add_hit->GetDoppler(particle_beta));
                }
                else
                {
                  addDopp_emma_corona->Fill(add_hit->GetDoppler(particle_beta));
                }
              }
            }
          }
        } // end of this tigress loop
        tigress->ResetAddback();
      }

      //==========================================EXCITATION ENERGY BEGINS ======================================

      if (s3 && emma->GetICMultiplicity() > 0)
      {

        double ICtime = -1;
        exc = -1;
        for (int j = 0; j < emma->GetMultiplicity(); j++)
        {
          em_hit = emma->GetEmmaHit(j);
          for (int i = 0; i < s3->GetPixelMultiplicity(); i++)
          {
            s3hit = s3->GetPixelHit(i);
            s3pos = s3hit->GetPosition(s3_phi_offset, true); // addded rotation of S3, apparently GRSISort rotated it
            // s3pos.SetX(s3pos.X() + s3_x_offset);				// these are all 0s
            // s3pos.SetY(s3pos.Y() + s3_y_offset);
            // s3pos.SetZ(s3pos.Z() + s3_z_offset);
            s3emmatof->Fill(s3hit->GetTime() - em_hit->GetTime());
            // tigress->ResetAddback();

            if (s3hit->GetTime() - em_hit->GetTime() > 400 && s3hit->GetTime() - em_hit->GetTime() < 550)
            {
              thetalab = s3pos.Theta();

              // cout << "\ntheta lab is" << thetalab;
              ekin = s3hit->GetEnergy();
              // exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2); //Energy conversion from keV to MeV
              exc = reac->GetExcEnergy(ekin * 1e-3, thetalab, 2);
              emma_s3_exc->Fill(exc); // Filling excitation energy gated on EMMA
              emma_s3_exc_ring->Fill(s3hit->GetRing(), exc);

              emma_s3_E->Fill(ekin);
              emma_s3_E_ring->Fill(s3hit->GetRing(), ekin);

              if (tigress)
              {
                for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
                {
                  add_hit = tigress->GetAddbackHit(t);
                  suppAdd = add_hit->BGOFired();

                  if (!suppAdd && add_hit->GetEnergy() > 50 && add_hit->GetTime() - em_hit->GetTime() > 850 && add_hit->GetTime() - em_hit->GetTime() < 1020)
                  {
                    emma_s3_exc_addback->Fill(add_hit->GetDoppler(particle_beta), exc);
                    emma_s3_E_ring_tig->Fill(s3hit->GetRing(), ekin);
                    // --------------------THIS PART IS CAUSING SEG FAULTS???-------------
                    // if (Mg26_cut->IsInside(si_hit->GetEnergy(), tempIC) && add_hit->GetEnergy() > 1789 && add_hit->GetEnergy() > 1830){
                    // s3_E_theta_gated_PIDG_gamG->Fill(exc);
                    //}
                  }
                }
              }
              tigress->ResetAddback();
            }
          }
        }
        // ==================================== Excitation Energy ENDS ================================

        // ====================================EMMA + S3 + TIGRESS coincidences=======================================
        if (s3 && tigress)
        {
          for (int i = 0; i < s3->GetPixelMultiplicity(); i++)
          {
            s3hit = s3->GetPixelHit(i);
            for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++)
            {
              add_hit = tigress->GetAddbackHit(t);
              suppAdd = add_hit->BGOFired();
              if (!suppAdd && add_hit->GetEnergy() > 15)
              {
                if (gate1D((add_hit->GetTime() - s3hit->GetTime()), tigs3T[0], tigs3T[1]))
                {
                  s3pos = s3hit->GetPosition(true);
                  s3pos.SetX(s3pos.X() + s3_x_offset);
                  s3pos.SetY(s3pos.Y() + s3_y_offset);
                  s3pos.SetZ(s3pos.Z() + s3_z_offset);

                  if (emma)
                  {
                    for (int j = 0; j < emma->GetMultiplicity(); j++)
                    {
                      em_hit = emma->GetEmmaHit(j);
                      s3emmatof->Fill(s3hit->GetTime() - em_hit->GetTime());
                      if (tofGate((s3hit->GetTime() - em_hit->GetTime())))
                      { // TOF gate and
                        excE_tg->Fill(exc);
                        excE_theta_tg->Fill(s3hit->GetTheta() * r2d, exc);
                        addDopp_tg->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec));
                        addDopp_ANum_tg->Fill(add_hit->GetArrayNumber(), add_hit->GetEnergy());
                        addDopp_exc_tg->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec), exc);
                        s3_E_gated->Fill(s3hit->GetEnergy());
                        s3_E_theta_gated->Fill(s3pos.Theta() * r2d, s3hit->GetEnergy());
                        hitmap_time_gated->Fill(s3pos.X(), s3pos.Y());

                        // TIGRESS-TIGRESS-S3 time gated
                        for (int t2 = t + 1; t2 < tigress->GetAddbackMultiplicity(); t2++)
                        {
                          add_hit2 = tigress->GetAddbackHit(t2);
                          suppAdd = add_hit2->BGOFired();
                          if (gate1D((add_hit->GetTime() - add_hit2->GetTime()), tigtigT[0], tigtigT[1]))
                          {
                            addE_addE_tofg->Fill(add_hit->GetEnergy(), add_hit2->GetEnergy());
                            addE_addE_tofg->Fill(add_hit2->GetEnergy(), add_hit->GetEnergy()); // symmetrized
                            addDopp_addDopp_tg->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec), add_hit2->GetDoppler(particle_beta, &recoil_vec));
                            addDopp_addDopp_tg->Fill(add_hit2->GetDoppler(particle_beta, &recoil_vec), add_hit->GetDoppler(particle_beta, &recoil_vec)); // symmetrized
                          }
                        }
                        if (emma->GetICMultiplicity() == 4 && emma->GetSiMultiplicity() > 0 && emma->GetSiHit(0)->GetEnergy() > 1000 && add_hit->GetDoppler(particle_beta, &recoil_vec) > 340 && add_hit->GetDoppler(particle_beta, &recoil_vec) < 350)
                        { // add in the Si vs IC with the 350 keV gamma gate
                          for (int k = 0; k < emma->GetSiMultiplicity(); k++)
                          {
                            icSumVSi_gated_350->Fill(emma->GetSiHit(k)->GetEnergy(), tempIC);
                          }
                        }

                        if (gate1D(em_hit->GetPosition().X(), xp[0], xp[1]) && gate1D(em_hit->GetPosition().Y(), yp[0], yp[1]))
                        { // PGAC position gated
                          excE_PIDG->Fill(exc);
                          excE_theta_PIDG->Fill(s3hit->GetTheta() * r2d, exc);
                          addDopp_PIDG->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec));
                          addDopp_ANum_PIDG->Fill(add_hit->GetArrayNumber(), add_hit->GetEnergy());
                          addDopp_exc_PIDG->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec), exc);
                        }
                      }
                      else if (BGtofGate((s3hit->GetTime() - em_hit->GetTime())))
                      { // TOF gate
                        excE_bg->Fill(exc);
                        excE_theta_bg->Fill(s3hit->GetTheta() * r2d, exc);
                        addDopp_bg->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec));
                        addDopp_ANum_bg->Fill(add_hit->GetArrayNumber(), add_hit->GetEnergy());
                        addDopp_exc_bg->Fill(add_hit->GetDoppler(particle_beta, &recoil_vec), exc);
                      }
                    }
                  }
                }
              }
            } // if S3 & tigress
          } // emma
        } // si
        tigress->ResetAddback();
      }
    } // if emma

  } // this is the end bracket for jentries.

  printf("\nend of main loops for events");
  cout << "Entry " << analentries << " of " << analentries << ", 100% complete" << endl;
  fflush(stdout);
  cout << "Event sorting complete" << endl;

  std::cout << analentries << " events, " << tig_emma_counter << " containing TIGRESS + EMMA, " << tig_counter << " containing TIGRESS, " << emma_counter << " containing EMMA"
            << std::endl;
  // std::cout	<< "Na22: " << counter_na22 << "\t SSB: " << counter_ssb << std::endl;

  cout << "Writing histograms to " << outfile << endl;
  fflush(stdout);

  TFile *myfile = new TFile(outfile, "RECREATE");
  myfile->cd();
  TDirectory *tigdir = myfile->mkdir("TIGRESS");
  tigdir->cd();
  tigList->Write();
  myfile->cd();

  TDirectory *s3dir = myfile->mkdir("S3");
  s3dir->cd();
  s3List->Write();
  myfile->cd();

  TDirectory *tigtigdir = myfile->mkdir("TIGRESS-TIGRESS");
  tigtigdir->cd();
  tigtigList->Write();
  myfile->cd();

  TDirectory *tigs3dir = myfile->mkdir("TIGRESS-S3");
  tigs3dir->cd();
  tigs3List->Write();
  myfile->cd();

  TDirectory *emmadir = myfile->mkdir("EMMA");
  emmadir->cd();
  emmaList->Write();
  TDirectory *emmaicdir = emmadir->mkdir("IC");
  emmaicdir->cd();
  icList->Write();
  emmadir->cd();
  myfile->cd();

  TDirectory *s3emmadir = myfile->mkdir("EMMA-S3");
  s3emmadir->cd();
  emmas3List->Write();
  myfile->cd();

  TDirectory *tigemmadir = myfile->mkdir("TIGRESS-EMMA");
  tigemmadir->cd();
  tigemmaList->Write();
  myfile->cd();

  TDirectory *tgatedir = myfile->mkdir("Time_Gated_TIGRESS");
  tgatedir->cd();
  tgList->Write();
  myfile->cd();

  TDirectory *rtgatedir = myfile->mkdir("Random_Time_Gated_TIGRESS");
  rtgatedir->cd();
  bgList->Write();
  myfile->cd();

  TDirectory *pidgatedir = myfile->mkdir("PID_Gated_TIGRESS");
  pidgatedir->cd();
  PIDgatedList->Write();
  myfile->cd();

  TDirectory *s3pidgatedir = myfile->mkdir("PID_Gated_S3");
  s3pidgatedir->cd();
  PIDgatedListS3->Write();
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
}
