#ifndef SortCode_h
#define SortCode_h

#include <iostream>
#include <iomanip>
#include "TCutG.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TChain.h"
#include "TH2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TRandom.h"
#include "TS3.h"
#include "TS3Hit.h"
#include "TReaction.h"
#include "TSRIM.h"
#include "TTigress.h"
#include "TSpectrum.h"
#include "TChannel.h"
#include "TEmma.h"
#include "TParserLibrary.h"
#include "TEnv.h"
using namespace std;

TList *tigList, *s3List, *tigtigList, *tigs3List, *emmaList, *icList, *tigemmaList, *tgList, *bgList, *PIDgatedList, *PIDgatedListS3,*emmas3List; 

TH1F *ssb_anode_dt1,*ssb_anode_dt2;

//Raw TIGRESS
TH1F *tigE, *addE, *addDoppRaw, *tigRate;
TH2F *tigE_ANum,*addE_ANum, *addDoppRaw_ANum, *num_addr, *tigE_theta, *tigRate_ANum; 

//Raw S3
TH1F *s3_E, *s3_rings_sectors_singlesT, *s3_ringNum, *s3_secNum, *s3Rate;
TH2F *rings, *sectors, *ringsT, *sectorsT, *s3_E_theta, *hitmap, *s3_rings_sectors, *s3_rings_sectors_singles, *s3_rings_sectors_singlesTvT, *s3_rings_sectors_singlesTvSec, *s3_rings_sectors_singlesTvRing, *s3_rings_sectors_singlesTvE, *s3_ringNum_secNum;
TH2F *s3_E_ringNum;

//TIGRESS-TIGRESS
TH1F *addT_addT; 
TH2F *addE_addE, *addE_addE_tg;

//TIGRESS-S3 
TH1F *addT_s3T, *excE, *addDopp, *s3_E_gated, *s3_E_theta_gated_PIDG_gamG, *s3_E_gated_PIDG_gamG;
TH2F *excE_theta, *addDopp_ANum_s3, *addDopp_exc, *s3_E_theta_gated, *addE_s3_E, *addDopp_s3_E, *s3_E_theta_coinc; 
TH2F *s3_E_ringNumb_1808_gated;

//Raw EMMA
TH1F *xPos, *yPos, *siE, *icSum, *icE[5], *ssbE[2];
TH2F *emma_dE_E,*emma_tig_dt_E;
TH2F *pgac, *icN, *iC0V1, *iC0V2, *iC0V3, *iC1V2, *iC1V3, *iC2V3, *tDE, *siET, *ssbET[2],*icSumVSi,*icSumVicPlusSi, *ic0VSi, *ic1VSi, *ic2VSi, *ic3VSi, *anodessbT[2];
TH2F *pgac_ne22, *pgac_na22;
TH1F *xPos_26Al_gated, *xPos_26Mg_gated, *xPos_24Na_gated;


TH2F *ICvsSiICSum_1274;
TH2F *iC0VSum_1274;
TH2F *iC0VSum_ne22;
TH2F *iC0VSum_na22; 
TH2F* iC0VSum; 
// EMMA-S3
TH1F *emma_s3_exc, *emma_s3_E, *s3_gatedon_Em_and_Tig;
TH2F *emma_s3_exc_ring,*emma_s3_exc_addback, *emma_s3_E_ring, *emma_s3_E_ring_tig;
TH2F *icSumVicPlusSi_s3gate;

TH1F *s3_sector0_mg_pid_gated, *s3_sector1_mg_pid_gated, *s3_sector2_mg_pid_gated, *s3_sector3_mg_pid_gated;
TH1F *s3_ring0_mg_pid_gated, *s3_ring1_mg_pid_gated, *s3_ring20_mg_pid_gated, *s3_ring21_mg_pid_gated; 
TH2F *s3_E_ringNum_mg_pid_gated;
TH2F *s3_E_ringNum_na_pid_gated;

TH2F *s3_E_theta_Mg_PID_Gated, *s3_E_theta_Mg_PID_1808_gamma_gated, *excE_thetaCm_Mg_PID_Gated, *excE_thetaCm_Mg_PID_1808_gamma_gated;

//ToF Spectra
TH1F *excE_tg, *addDopp_tg, *excE_bg, *addDopp_bg, *addemmatof, *s3emmatof, *ssbICtof, *ssbSItof, *tigICtof, *tigAnodetof, *ssbEMTtof;
TH2F *addE_tof, *excE_theta_tg, *addDopp_ANum_tg, *addE_addE_tofg, *addDopp_exc_tg, *addDopp_addDopp_tg, *excE_theta_bg, *addDopp_ANum_bg, *addDopp_exc_bg, *hitmap_time_gated, *icSumVSi_gated_350, *addemmatofdet; 

// TIGRESS EMMA
TH1F *emmaTigressCoincNoAddback, *addDopp_emma, *addDopp_emma_corona, *addDopp_emma_lampshade, *addDopp_na22, *addDopp_ne22, *addDopp_na22_ic, *addDopp_ne22_ic, *addDopp_mg22, *addDopp_mg22_corona, *addDopp_mg22_lampshade, *addDopp_mg22_corona_unsupp, *addDopp_mg22_lampshade_unsupp;
TH2F *icSumVicPlusSi1274Gate,*add_na22vt,*add_ne22vt;
TH2F *addDopp_ne22_gg,*addDopp_na22_gg,*addDopp_mg22_gg;
TH1F *emTrigE; 
//pid gated
TH1F *doppE_19F_pidg, *doppE_24Na_pidg, *doppE_26Mg_pidg, *doppE_26Al_pidg, *doppE_26Mg_pidg_xPosG, *doppE_26Mg_pidg_delayT;
TH1F *doppE_Mg_pid_s3_gated;
TH2F *excE_doppE_Mg_PID_Gated;

//Mass Spectra
TH1F *excE_PIDG, *addDopp_PIDG, *excE_PIDG_bg, *addDopp_PIDG_bg, *aE_icSumVSi_gated;
TH2F *excE_theta_PIDG, *addDopp_ANum_PIDG, *addDopp_exc_PIDG, *excE_theta_PIDG_bg, *addDopp_ANum_PIDG_bg, *addDopp_exc_PIDG_bg, *rings_PIDG, *s3_E_theta_gated_PIDG; 

//TCuts
TCutG *massGate, *siicGate;

TCutG *na22_cut,*ne22_cut, *mg22_cut;
TCutG *na22_cut_ic,*ne22_cut_ic;

class SortCode {

	public :

		SortCode(){;} 
		void SortData(const char*, const char*, const char*, const char*);
		void Initialise();
};
#endif

void SortCode::Initialise() {

  printf("Start initializations\n");
  printf("Creating list\n");

  tigList = new TList;
  s3List = new TList;
  tigtigList = new TList;
  tigs3List = new TList;
  emmaList = new TList;
  icList = new TList;
  tigemmaList = new TList;
  tgList = new TList;
  bgList = new TList;
  PIDgatedList = new TList;
  PIDgatedListS3 = new TList;
	emmas3List = new TList;

  printf("Creating histograms\n");

  /*

	ne22_cut_ic = new TCutG("ic_ne22",14);
	ne22_cut_ic->SetPoint(0,456.135,2188.9);
	ne22_cut_ic->SetPoint(1,491.949,2262.18);
	ne22_cut_ic->SetPoint(2,537.315,2307.27);
	ne22_cut_ic->SetPoint(3,573.129,2273.45);
	ne22_cut_ic->SetPoint(4,573.129,2155.08);
	ne22_cut_ic->SetPoint(5,554.028,2014.16);
	ne22_cut_ic->SetPoint(6,511.051,1867.61);
	ne22_cut_ic->SetPoint(7,460.91,1788.7);
	ne22_cut_ic->SetPoint(8,427.483,1777.43);
	ne22_cut_ic->SetPoint(9,394.056,1783.06);
	ne22_cut_ic->SetPoint(10,389.281,1856.34);
	ne22_cut_ic->SetPoint(11,394.056,1991.62);
	ne22_cut_ic->SetPoint(12,425.096,2115.62);
	ne22_cut_ic->SetPoint(13,456.135,2188.9);
	emmaList->Add(ne22_cut_ic);


	na22_cut_ic = new TCutG("IC_na22",11);
	na22_cut_ic->SetPoint(0,522.989,2532.74);
	na22_cut_ic->SetPoint(1,594.618,2775.11);
	na22_cut_ic->SetPoint(2,656.697,2842.75);
	na22_cut_ic->SetPoint(3,690.124,2792.02);
	na22_cut_ic->SetPoint(4,697.287,2690.56);
	na22_cut_ic->SetPoint(5,663.86,2555.28);
	na22_cut_ic->SetPoint(6,616.107,2420);
	na22_cut_ic->SetPoint(7,568.354,2335.45);
	na22_cut_ic->SetPoint(8,530.152,2352.36);
	na22_cut_ic->SetPoint(9,515.826,2431.28);
	na22_cut_ic->SetPoint(10,522.989,2532.74);
	emmaList->Add(na22_cut_ic);


  ne22_cut = new TCutG("CUTG_ne",6);
  ne22_cut->SetPoint(0,3372.2,2282.41);
  ne22_cut->SetPoint(1,3287.48,2194.44);
  ne22_cut->SetPoint(2,3261.7,2074.07);
  ne22_cut->SetPoint(3,3254.33,1930.56);
  ne22_cut->SetPoint(4,3305.9,1824.07);
  ne22_cut->SetPoint(5,3464.29,1777.78);
  ne22_cut->SetPoint(6,3641.09,1814.81);
  ne22_cut->SetPoint(7,3714.76,2013.89);
  ne22_cut->SetPoint(8,3714.76,2148.15);
  ne22_cut->SetPoint(9,3655.83,2203.7);
  ne22_cut->SetPoint(10,3512.17,2277.78);
  ne22_cut->SetPoint(11,3390.62,2291.67);
  ne22_cut->SetPoint(12,3372.2,2282.41);
  emmaList->Add(ne22_cut);

  na22_cut = new TCutG("CUTG_na",11);
  na22_cut->SetPoint(0,3324.32,2731.48);
  na22_cut->SetPoint(1,3302.22,2532.41);
  na22_cut->SetPoint(2,3335.37,2402.78);
  na22_cut->SetPoint(3,3467.97,2310.19);
  na22_cut->SetPoint(4,3685.29,2305.56);
  na22_cut->SetPoint(5,3744.23,2458.33);
  na22_cut->SetPoint(6,3733.18,2638.89);
  na22_cut->SetPoint(7,3670.56,2726.85);
  na22_cut->SetPoint(8,3482.71,2805.56);
  na22_cut->SetPoint(9,3364.83,2777.78);
	na22_cut->SetPoint(10,3324.32,2731.48);

  emmaList->Add(na22_cut);

  mg22_cut = new TCutG("CUTG_mg22",11);
  mg22_cut->SetPoint(0,3420.32,3224.81);
  mg22_cut->SetPoint(1,3467.45,3042.12);
  mg22_cut->SetPoint(2,3582.32,2992.83);
  mg22_cut->SetPoint(3,3682.47,3059.52);
  mg22_cut->SetPoint(4,3729.59,3204.51);
  mg22_cut->SetPoint(5,3738.43,3384.3);
  mg22_cut->SetPoint(6,3688.36,3491.59);
  mg22_cut->SetPoint(7,3573.49,3520.59);
  mg22_cut->SetPoint(8,3455.67,3471.29);
  mg22_cut->SetPoint(9,3426.22,3297.31);
  mg22_cut->SetPoint(10,3420.32,3224.81);
 
  emmaList->Add(mg22_cut);

  */


  //Raw TIGRESS Spectra
  tigE = new TH1F("tigE", "Tigress_Energy;Energy (keV);Counts", 8192, 0, 16384);
  tigList->Add(tigE);
  tigE_ANum = new TH2F("tigE_ANum", "Tigress_Energy_V_ArrayNumber;Array Number;Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  tigList->Add(tigE_ANum);
  addE = new TH1F("addE", "Addback_Energy;Energy (keV);Counts", 8192, 0, 16384);
  tigList->Add(addE);
  addDoppRaw = new TH1F("addDoppRaw", "Addback_Doppler_Raw;Energy (keV);Counts", 8192, 0, 16384);
  tigList->Add(addDoppRaw);
  addE_ANum = new TH2F("addE_ANum", "Addback_Energy_V_ArrayNumber;Array Number;Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  tigList->Add(addE_ANum);
  addDoppRaw_ANum = new TH2F("addDoppRaw_ANum", "Addback_Doppler_Energy_V_ArrayNumber;Array Number;Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  tigList->Add(addDoppRaw_ANum);
  num_addr = new TH2F("num_addr", "TIGRESS_position_vs_address;Array Number;Electronic Address", 64, 0, 64, 8192, 0, 16384);
  tigList->Add(num_addr);
  tigE_theta = new TH2F("tigE_theta","TIGRESS energy vs lab angle;Lab Energy (keV);#theta_{lab} (#circ)",8192,0,16384,180,0,180);
  tigList->Add(tigE_theta);
  tigRate = new TH1F("tigRate", "TIGRESS Rate", 8192, 0, 16384);
  tigRate->GetXaxis()->SetTitle("Run Time (s)");
  tigRate->GetYaxis()->SetTitle("Count/s");
  tigList->Add(tigRate);
  tigRate_ANum = new TH2F("tigRate_ANum", "TIGRESS Rate", 8192, 0, 16384, 64, 0, 64);
  tigRate_ANum->GetXaxis()->SetTitle("Run Time (s)");
  tigRate_ANum->GetYaxis()->SetTitle("Array Number");
  tigList->Add(tigRate_ANum);

  //Raw S3 Spectra
  s3_E_ringNum = new TH2F("s3_E_ringNum","S3 Energy vs Ring Number;Ring Number;Energy (keV)",25,0,25,8192,0,8192);
  s3List->Add(s3_E_ringNum);

  s3_ringNum = new TH1F("S3_ringNum","S3 Ring Number;Ring Number;Counts",25,0,25);
  s3List->Add(s3_ringNum);
  s3_secNum = new TH1F("S3_secNum","S3 Sector Number;Sector Number;Counts",33,0,33);
  s3List->Add(s3_secNum);
  s3_ringNum_secNum = new TH2F("s3_ringNum_secNum","S3 Ring and Sector Number",25,0,25,33,0,33);
  s3_ringNum_secNum->GetXaxis()->SetTitle("Ring Number");
  s3_ringNum_secNum->GetYaxis()->SetTitle("Sector Number");
  s3List->Add(s3_ringNum_secNum);
  rings = new TH2F("rings","rings",25,0,25,4096,0,8192);
  rings->GetXaxis()->SetTitle("Ring Number");
  rings->GetYaxis()->SetTitle("Energy (uncalibrated)");
  s3List->Add(rings);
  ringsT = new TH2F("ringsT","rings_TimeCut_Mult1",25,0,25,4096,0,8192);
  ringsT->GetXaxis()->SetTitle("Ring Number");
  ringsT->GetYaxis()->SetTitle("Ring Energy (uncalibrated)");
  s3List->Add(ringsT);
  sectors = new TH2F("sectors","sectors",33,0,33,4096,0,8192);
  sectors->GetXaxis()->SetTitle("Sector Number");
  sectors->GetYaxis()->SetTitle("Energy (uncalibrated)");
  s3List->Add(sectors);
  sectorsT = new TH2F("sectorsT","sectors_TimeCut_Mult1",33,0,33,4096,0,8192);
  sectorsT->GetXaxis()->SetTitle("Sector Number");
  sectorsT->GetYaxis()->SetTitle("Sector Energy (uncalibrated)");
  s3List->Add(sectorsT);
  s3_E = new TH1F("S3_E","S3_E;Energy (keV);Counts",8192,0,8192);
  s3List->Add(s3_E); 
  s3_E_theta = new TH2F("s3_E_theta","s3_E_theta;#theta_{lab} (#circ);Energy (keV)",180,0,180, 8192, 0, 16384); 
  s3List->Add(s3_E_theta);
  hitmap = new TH2F("hitmap","Hitmap",200,-50,50,200,-50,50); 
  s3List->Add(hitmap);
  s3_rings_sectors = new TH2F("s3_rings_sectors","S3_sectorE_vs_ringE;Ring Energy;Sector Energy",2048,0,8192,2048,0,8192);
  s3List->Add(s3_rings_sectors);
  s3_rings_sectors_singles = new TH2F("s3_rings_sectors_singles","S3_sectorE_vs_ringE_Multiplicity_1;Ring Energy;Sector Energy",2048,0,8192,2048,0,8192);
  s3List->Add(s3_rings_sectors_singles);
  s3_rings_sectors_singlesT = new TH1F("s3_rings_sectors_singles_timing","S3_sectorT_vs_ringT_Multiplicity_1;Sector Time - Ring Time",2048,-1024,1024);
  s3List->Add(s3_rings_sectors_singlesT);
  s3_rings_sectors_singlesTvT = new TH2F("s3_rings_sectors_singles_timing_v_time","S3_sectorT_vs_ringT_Multiplicity_1_vs_Time;Time (min);Sector Time - Ring Time",1600,0,1600,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvT);
  s3_rings_sectors_singlesTvSec = new TH2F("s3_rings_sectors_singles_timing_v_sector","S3_sectorT_vs_ringT_Multiplicity_1_vs_Sector;Sector;Sector Time - Ring Time",32,0,32,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvSec);
  s3_rings_sectors_singlesTvRing = new TH2F("s3_rings_sectors_singles_timing_v_ring","S3_sectorT_vs_ringT_Multiplicity_1_vs_Ring;Ring;Sector Time - Ring Time",24,0,24,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvRing);
  s3_rings_sectors_singlesTvE = new TH2F("s3_rings_sectors_singles_timing_v_energy","S3_sectorT_vs_ringT_Multiplicity_1_vs_Energy;Energy (keV);Sector Time - Ring Time",4096,4096,8192,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvE);
  s3Rate = new TH1F("s3Rate", "S3 Rate", 8192, 0, 8192);
  s3Rate->GetXaxis()->SetTitle("Run Time (s)");
  s3Rate->GetYaxis()->SetTitle("Count/s");
  s3List->Add(s3Rate);

  //TIGRESS-TIGRESS
  addT_addT = new TH1F("addT_addT","Tigress-Tigress_time;Time Difference (ns);Counts",4096,-2048,2048); 
  tigtigList->Add(addT_addT);
  addE_addE = new TH2F("addE_addE", "Addback_Gamma_Gamma;Energy (keV);Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tigtigList->Add(addE_addE);
  addE_addE_tg = new TH2F("addE_addE_tg", "Addback_Gamma_Gamma_Time_Gated;Energy (keV);Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tigtigList->Add(addE_addE_tg);

  //TIGRESS-S3
  s3_E_ringNumb_1808_gated = new TH2F("s3_E_ringNumb_1808_gated","S3 E vs ring number, gated on 1808 gamma;Ring Number;Energy (keV)",25,0,25,8192,0,8192);
  tigs3List->Add(s3_E_ringNumb_1808_gated);
  addT_s3T = new TH1F("addT_s3T","Tigress-S3_time;Time Difference (ns)",4096,-2048,2048); 
  tigs3List->Add(addT_s3T);
  excE = new TH1F("excE","Excitation_Energy_MeV;Exc Energy (MeV);Counts",280,-2.,12.); 
  tigs3List->Add(excE);
  excE_theta = new TH2F("excE_theta","Excitation_Energy_Theta;#theta_{lab} (#circ);Exc Energy (MeV)",180,0,180,280,-2.,12.); 
  tigs3List->Add(excE_theta);
  addDopp = new TH1F("addDopp", "Addback_Doppler_Energy;Doppler Energy (keV);Counts", 8192, 0, 16384);
  tigs3List->Add(addDopp);
  addDopp_ANum_s3 = new TH2F("addDopp_ANum_s3", "Addback_Doppler_Energy_V_ArrayNumber;Array Number;Doppler Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  tigs3List->Add(addDopp_ANum_s3);
  addDopp_exc = new TH2F("addDopp_Exc", "Addback_Doppler_Energy_V_Excitation_Energy;Doppler Energy (keV);Exc Energy (MeV)", 8192, 0, 16384, 280, -2., 12.);
  tigs3List->Add(addDopp_exc);
  addE_s3_E  = new TH2F("addE_s3E", "Addback_Energy_V_S3_Energy;Addback Energy (keV);S3 Energy (keV)", 8192, 0, 16384, 2048, 0, 32768);
  tigs3List->Add(addE_s3_E);
  addDopp_s3_E  = new TH2F("addDopp_s3E", "Addback_Doppler_Energy_V_S3_Energy;Doppler Energy (keV);S3 Energy (keV)", 8192, 0, 16384, 2048, 0, 32768);
  tigs3List->Add(addDopp_s3_E);
  s3_E_theta_coinc = new TH2F("s3_E_theta_coinc","s3_E_theta_coin;#theta_{lab} (#circ);S3 Energy (keV)",180,0,180, 8192, 0, 16384); 
  tigs3List->Add(s3_E_theta_coinc);

	// EMMA - S3
	emma_s3_exc = new TH1F("emma_s3_exc","Emma gated S3 excitation energy;Exc Energy (MeV);Counts",300,-2,13);  	 
	emmas3List->Add(emma_s3_exc);
	emma_s3_exc_ring = new TH2F("emma_s3_exc_ring","Emma gated S3 excitation energy vs ring;S3 Ring Number;Exc Energy (MeV)",24,0,24,300,-2,13);  	 
	emmas3List->Add(emma_s3_exc_ring);
	emma_s3_exc_addback = new TH2F("emma_s3_exc_addback","Emma gated S3 excitation energy vs addback Doppler corrected;Doppler Energy (keV);Exc Energy (MeV)",2048,0,8192,300,-2,13);  	 
	emmas3List->Add(emma_s3_exc_addback);
	emma_s3_E = new TH1F("emma_s3_E","Emma gated S3 energy;S3 Energy (keV);Counts",8192,0,8192);
        emmas3List->Add(emma_s3_E);
	emma_s3_E_ring = new TH2F("emma_s3_E_ring","Emma gated S3 energy vs ring;Ring Number;S3 Energy (keV)",24,0,24,8192,0,8192);
        emmas3List->Add(emma_s3_E_ring);
        emma_s3_E_ring_tig = new TH2F("emma_s3_E_ring_tig","Emma & tigress gated S3 energy vs ring;Ring Number;S3 Energy (keV)",24,0,24,8192,0,8192);
        emmas3List->Add(emma_s3_E_ring_tig);

	//pid gated
	
	s3_sector0_mg_pid_gated = new TH1F("s3_sector0_mg_pid_gated","S3 sector 0 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_sector0_mg_pid_gated);
	s3_sector1_mg_pid_gated = new TH1F("s3_sector1_mg_pid_gated","S3 sector 1 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_sector1_mg_pid_gated);
	s3_sector2_mg_pid_gated = new TH1F("s3_sector2_mg_pid_gated","S3 sector 2 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_sector2_mg_pid_gated);
	s3_sector3_mg_pid_gated = new TH1F("s3_sector3_mg_pid_gated","S3 sector 3 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_sector3_mg_pid_gated);
	
	s3_ring0_mg_pid_gated = new TH1F("s3_ring0_mg_pid_gated","S3 ring 0 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_ring0_mg_pid_gated);
	s3_ring1_mg_pid_gated = new TH1F("s3_ring1_mg_pid_gated","S3 ring 1 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_ring1_mg_pid_gated);
	s3_ring20_mg_pid_gated = new TH1F("s3_ring20_mg_pid_gated","S3 ring 20 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_ring20_mg_pid_gated);
	s3_ring21_mg_pid_gated = new TH1F("s3_ring21_mg_pid_gated","S3 ring 21 energy gated on Mg PID;Energy (keV);Counts",8192,0,8192);
	emmas3List->Add(s3_ring21_mg_pid_gated);
	s3_E_ringNum_mg_pid_gated = new TH2F("s3_E_ringNum_mg_pid_gated","S3 energy vs ring number gated on Mg PID;Ring Number;Energy (keV)",25,0,25,8192,0,8192);
	emmas3List->Add(s3_E_ringNum_mg_pid_gated);
	s3_E_ringNum_na_pid_gated = new TH2F("s3_E_ringNum_na_pid_gated","S3 energy vs ring number gated on Na PID;Ring Number;Energy (keV)",25,0,25,8192,0,8192);
	emmas3List->Add(s3_E_ringNum_na_pid_gated);
	
	

	//TIGRESS - EMMA - S3 triples
	s3_gatedon_Em_and_Tig = new TH1F("s3_gatedon_Em_and_Tig","S3_E;Energy (keV) gated on EMMA PID and TIGRESS 1806 keV;Counts",8192,0,8192); //here i am!
	PIDgatedListS3->Add(s3_gatedon_Em_and_Tig); 

  icSumVicPlusSi_s3gate = new TH2F("icSumVicPlusSi_s3gate ","Si_+_IC_SUM_Energy_VS_IC_SUM_S3_Gated",256,2048,4096,256,1500,3500);
  icSumVicPlusSi_s3gate ->GetXaxis()->SetTitle("Silicon + IC Sum");
  icSumVicPlusSi_s3gate ->GetYaxis()->SetTitle("IC Sum");
  emmas3List->Add(icSumVicPlusSi_s3gate);

  // Raw EMMA
  xPos = new TH1F("xPos", "PGAC_X_Position;X Position (mm?);Counts", 160, -80, 80);
  emmaList->Add(xPos);
  yPos = new TH1F("yPos", "PGAC_Y_Position;Y Position (mm?);Counts", 60, -30, 30);
  emmaList->Add(yPos);
  pgac = new TH2F("pgac", "PGAC_Hit_Pattern", 160, -80, 80, 60, -30, 30);
  emmaList->Add(pgac);
  
  xPos_26Al_gated = new TH1F("xPos_26Al_gated","PGAC X Pos gated on 26Al PID;X Position (mm);Counts",160,-80,80);
  emmaList->Add(xPos_26Al_gated);
  xPos_26Mg_gated = new TH1F("xPos_26Mg_gated","PGAC X Pos gated on 26Mg PID;X Position (mm);Counts",160,-80,80);
  emmaList->Add(xPos_26Mg_gated);
  xPos_24Na_gated = new TH1F("xPos_24Na_gated","PGAC X Pos gated on 24Na PID;X Position (mm);Counts",160,-80,80);
  emmaList->Add(xPos_24Na_gated);


/*
  pgac_ne22 = new TH2F("pgac_ne22", "PGAC_Hit_Pattern, 22Ne", 160, -80, 80, 60, -30, 30);
  emmaList->Add(pgac_ne22);
  pgac_na22 = new TH2F("pgac_na22", "PGAC_Hit_Pattern, 22Na", 160, -80, 80, 60, -30, 30);
  emmaList->Add(pgac_na22);
*/
  siE = new TH1F("siE", "EMMA_Focal_Plane_Silicon_Energy;Energy (keV);Counts", 16384, 0, 16384);
  emmaList->Add(siE);
  siET = new TH2F("siET", "EMMA_Focal_Plane_Silicon_Energy_V_Time;Time (s);Energy (keV)", 4096,0,4096, 16384, 0, 16384);
  emmaList->Add(siET);

  emma_dE_E = new TH2F("emma_dE_E","Si_Energy_VS_IC_SUM, EMMA;Silicon Energy (arb.);IC Sum (arb.)",4096,0,4096,16384,0,16384);
  emmaList->Add(emma_dE_E);
  icSumVSi = new TH2F("icSumVSi","Si_Energy_VS_IC_SUM;Silicon Energy (arb.);IC Sum (arb.)",4096,0,4096,16384,0,16384);
  emmaList->Add(icSumVSi);
  icSumVicPlusSi = new TH2F("icSumVicPlusSi","Si_+_IC_SUM_Energy_VS_IC_SUM",256,2048,4096,256,1500,3500);
  icSumVicPlusSi->GetXaxis()->SetTitle("Silicon + IC Sum");
  icSumVicPlusSi->GetYaxis()->SetTitle("IC Sum");
  emmaList->Add(icSumVicPlusSi);
  ic0VSi = new TH2F("ic0VSi","Si_Energy_VS_IC0",4096,0,4096,16384,0,16384);
  emmaList->Add(ic0VSi);
  ic1VSi = new TH2F("ic1VSi","Si_Energy_VS_IC1",4096,0,4096,16384,0,16384);
  emmaList->Add(ic1VSi);
  ic2VSi = new TH2F("ic2VSi","Si_Energy_VS_IC2",4096,0,4096,16384,0,16384);
  emmaList->Add(ic2VSi);
  ic3VSi = new TH2F("ic3VSi","Si_Energy_VS_IC3",4096,0,4096,16384,0,16384);
  emmaList->Add(ic3VSi);
  for (int i = 0; i < 5; i++) {
    char hname[64];
    sprintf(hname, "EMMA_IC-Segment_%i", i);
    icE[i] = new TH1F(hname, hname, 4096, 0, 4096);
    icList->Add(icE[i]);
  }
  icN = new TH2F("icN", "IC_Energy_V_Segment_Number", 5, 0, 5, 4096, 0, 4096);
  icList->Add(icN);
  icSum = new TH1F("icSum", "IC_Summed_Energy", 16384, 0, 16384);
  icList->Add(icSum);
  iC0V1= new TH2F("iC0V1", "IC0_Energy_V_IC1_Energy", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC0V1);
  iC0V2= new TH2F("iC0V2", "IC0_Energy_V_IC2_Energy", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC0V2);
  iC0V3= new TH2F("iC0V3", "IC0_Energy_V_IC3_Energy", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC0V3);
  iC1V2= new TH2F("iC1V2", "IC1_Energy_V_IC2_Energy", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC1V2);
  iC1V3= new TH2F("iC1V3", "IC1_Energy_V_IC3_Energy", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC1V3);
  iC2V3= new TH2F("iC2V3", "IC2_Energy_V_IC3_Energy", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC2V3);

  

	ICvsSiICSum_1274 = new TH2F("ICvsSiICSum_1274","IC vs IC + Si, 1274 keV gamma gated",1024,0, 4096,1024,0,4096);
	emmaList->Add(ICvsSiICSum_1274);
	iC0VSum_1274 = new TH2F("iC0VSum_1274","IC0 vs IC sum, 1274 keV gamma gated",1024,0, 4096,1024,0,4096);
	emmaList->Add(iC0VSum_1274);
	iC0VSum = new TH2F("iC0VSum","IC0 vs IC sum",1024,0, 4096,1024,0,4096);
	emmaList->Add(iC0VSum);
	
	/*
	iC0VSum_ne22 = new TH2F("iC0VSum_ne22","IC0 vs IC sum, 22Ne gated",1024,0, 4096,1024,0,4096);
	emmaList->Add(iC0VSum_ne22);
	iC0VSum_na22 = new TH2F("iC0VSum_na22","IC0 vs IC sum, 22Na gated",1024,0, 4096,1024,0,4096);
	emmaList->Add(iC0VSum_na22);
	iC0VSum = new TH2F("iC0VSum","IC0 vs IC sum",1024,0, 4096,1024,0,4096);
	emmaList->Add(iC0VSum);

	*/

  for (int i = 0; i < 2; i++) {
    char hname[64];
    char hname2[64];
    sprintf(hname, "SSB_%i", i + 1);
    sprintf(hname2, "SSB_%i_Versus_Time", i + 1);
    ssbE[i] = new TH1F(hname, hname, 16384, 0, 98304);
    ssbET[i] = new TH2F(hname2, hname2, 4000, 0, 4000, 16384, 0, 98304);
    emmaList->Add(ssbE[i]);
    emmaList->Add(ssbET[i]);
    sprintf(hname2, "SSB_%i_Vs_Anode_Time", i + 1);
    anodessbT[i] = new TH2F(hname2, hname2, 4096, -4096, 4096, 4, 0, 4);
    anodessbT[i]->GetXaxis()->SetTitle("SSB Time -- EMMA anode Time ns");
    anodessbT[i]->GetYaxis()->SetTitle("EMMA anode segment");
    emmaList->Add(anodessbT[i]);
  }

  //ToF Spectra
  addemmatofdet = new TH2F("addemmatofdet","Addback_Time - EMMA Time vs TIGRESS det num;Array Number;TIG-EMMA Time Difference (ns)",64,0,64,2000,-5000,5000);
  tigemmaList->Add(addemmatofdet);
  addemmatof = new TH1F("addemmatof", "Addback_Time - EMMA Time;TIG-EMMA Time Difference (ns);Counts", 20000, -10000, 10000);
  tigemmaList->Add(addemmatof);
  
  emTrigE = new TH1F("emTrigE", "EMMA Trigger Energy; Energy [keV]; Counts/5keV", 4000, 0, 20000); 
  tigemmaList->Add(emTrigE); 
  
  
  s3emmatof = new TH1F("s3emmatof", "S3_Time - EMMA Time;S3-EMMA Time Difference (ns)", 20000, -10000, 10000);
  tigemmaList->Add(s3emmatof);
  addE_tof = new TH2F("addE_tof", "Addback_Energy_vs_(Addback_Time - EMMA Time);TIG-EMMA Time Difference (ns);Energy (keV)", 2000, -10000, 10000, 8192, 0, 16384);
  tigemmaList->Add(addE_tof);
  ssbICtof = new TH1F("ssbICtof", "SSB_Time - EMMA IC Time;Time Difference (ns);Counts", 20000, -10000, 10000);
  tigemmaList->Add(ssbICtof);
  ssbSItof = new TH1F("ssbSItof", "SSB_Time - EMMA Si Time;Time Difference (ns);Counts", 20000, -10000, 10000);
  tigemmaList->Add(ssbSItof);
  ssbEMTtof = new TH1F("ssbEMTtof", "SSB_Time - EMMA Trigger Time;Time Difference (ns);Counts", 20000, -10000, 10000);
  tigemmaList->Add(ssbEMTtof);
  tigICtof = new TH1F("tigICtof", "Addback_Time - EMMA IC Time;Time Difference (ns);Counts", 20000, -10000, 10000);
  tigemmaList->Add(tigICtof);
  tigAnodetof = new TH1F("tigAnodetof", "Addback_Time - EMMA Anode Time;Time Difference (ns);Counts", 20000, -10000, 10000);
  tigemmaList->Add(tigAnodetof);
  tDE = new TH2F("tDE","tDE",4000,0,4000,400,-2000,2000);
  tigemmaList->Add(tDE);
	emmaTigressCoincNoAddback = new TH1F("EMMA-TIGRESS_Coincidence", "EMMA-TIGRESS Coinc.;TIGRESS Energy (keV);Counts",8192,0,16384);
	tigemmaList->Add(emmaTigressCoincNoAddback); 
	addDopp_emma = new TH1F("addDopp_emma","Addback Doppler, EMMA coinc.;Doppler Energy (keV);Counts",8192,0,16384);
	tigemmaList->Add(addDopp_emma);
  addDopp_emma_corona = new TH1F("addDopp_emma_corona","Addback Doppler (90 deg detectors), EMMA coinc.;Doppler Energy (keV);Counts",8192,0,16384);
	tigemmaList->Add(addDopp_emma_corona);
  addDopp_emma_lampshade = new TH1F("addDopp_emma_lampshade","Addback Doppler (135 deg detectors), EMMA coinc.;Doppler Energy (keV);Counts",8192,0,16384);
	tigemmaList->Add(addDopp_emma_lampshade);
	/*
	addDopp_na22_ic = new TH1F("addDopp_na22_ic","Addback Doppler, EMMA coinc. - Na22, IC",8192,0,16384);
	tigemmaList->Add(addDopp_na22_ic);
	addDopp_ne22_ic = new TH1F("addDopp_ne22_ic","Addback Doppler, EMMA coinc. - Ne22, IC",8192,0,16384);
	tigemmaList->Add(addDopp_ne22_ic);
	addDopp_na22 = new TH1F("addDopp_na22","Addback Doppler, EMMA coinc. - Na22",8192,0,16384);
	tigemmaList->Add(addDopp_na22);
	addDopp_ne22 = new TH1F("addDopp_ne22","Addback Doppler, EMMA coinc. - Ne22",8192,0,16384);
	tigemmaList->Add(addDopp_ne22);
	add_na22vt = new TH2F("add_na22vt","Addback, EMMA coinc. - Na22 vs angle",8192,0,16384,180,0,180);
	tigemmaList->Add(add_na22vt);
	add_ne22vt = new TH2F("add_ne22vt","Addback, EMMA coinc. - Ne22 vs angle",8192,0,16384,180,0,180);
	tigemmaList->Add(add_ne22vt);
  addDopp_mg22 = new TH1F("addDopp_mg22","Addback Doppler, EMMA coinc. - Mg22",8192,0,16384);
	tigemmaList->Add(addDopp_mg22);
  addDopp_mg22_corona = new TH1F("addDopp_mg22_corona","Addback Doppler (90 deg detecotrs), EMMA coinc. - Mg22",8192,0,16384);
	tigemmaList->Add(addDopp_mg22_corona);
  addDopp_mg22_lampshade = new TH1F("addDopp_mg22_lampshade","Addback Doppler (135 deg detecotrs), EMMA coinc. - Mg22",8192,0,16384);
	tigemmaList->Add(addDopp_mg22_lampshade);
  addDopp_mg22_corona_unsupp = new TH1F("addDopp_mg22_corona_unsupp","Addback Doppler (90 deg detecotrs, unsuppressed), EMMA coinc. - Mg22",8192,0,16384);
	tigemmaList->Add(addDopp_mg22_corona_unsupp);
  addDopp_mg22_lampshade_unsupp = new TH1F("addDopp_mg22_lampshade_unsupp","Addback Doppler (135 deg detecotrs, unsuppressed), EMMA coinc. - Mg22",8192,0,16384);
	tigemmaList->Add(addDopp_mg22_lampshade_unsupp);
	


	addDopp_na22_gg = new TH2F("addDopp_na22_gg","Gamma-gamma, Addback Doppler, EMMA coinc. - Na22",8192,0,16384,8192,0,16384);
	tigemmaList->Add(addDopp_na22_gg);
	addDopp_ne22_gg = new TH2F("addDopp_ne22_gg","Gamma-gamma, Addback Doppler, EMMA coinc. - Ne22",8192,0,163848192,8192,0,16384);
	tigemmaList->Add(addDopp_ne22_gg);
  addDopp_mg22_gg = new TH2F("addDopp_mg22_gg","Gamma-gamma, Addback Doppler, EMMA coinc. - Mg22",8192,0,16384,8192,0,16384);
	tigemmaList->Add(addDopp_mg22_gg);

	*/

  emma_tig_dt_E = new TH2F("emma_tig_dt_E","emma_tig_dt_E;TIG-EMMA Time Difference (ns);Doppler Energy (keV)",1000,-2000,2000,1024,0,4196);
  tigemmaList->Add(emma_tig_dt_E);
  icSumVicPlusSi1274Gate = new TH2F("icSumVicPlusSi1274Gate","Si_+_IC_SUM_Energy_VS_IC_SUM_1274_keV_gated",256,2048,4096,256,1500,3500);
  icSumVicPlusSi1274Gate->GetXaxis()->SetTitle("Silicon + IC Sum");
  icSumVicPlusSi1274Gate->GetYaxis()->SetTitle("IC Sum");
  tigemmaList->Add(icSumVicPlusSi1274Gate);
	ssb_anode_dt1 = new TH1F("SSB_EMMA_dT1","SSB_EMMA_dT1;SSB-EMMA Time Difference (ns);Counts",1000,-5000,5000);
	emmas3List->Add(ssb_anode_dt1);  
	ssb_anode_dt2 = new TH1F("SSB_EMMA_dT2","SSB_EMMA_dT2;SSB-EMMA Time Difference (ns);Counts",1000,-5000,5000);
	emmas3List->Add(ssb_anode_dt2);  
//Time Gated Spectra
  excE_tg = new TH1F("excE_tg","Excitation_Energy_MeV_Time_Gated;Exc Energy (MeV);Counts",280,-2.,12.); 
  tgList->Add(excE_tg);
  excE_theta_tg = new TH2F("excE_theta_tg","Excitation_Energy_Theta_Time_Gated#theta_{lab} (#circ);Exc Energy (MeV)",180,0,180,280,-2.,12.); 
  tgList->Add(excE_theta_tg);
  addDopp_tg = new TH1F("addDopp_tg", "Addback_Doppler_Energy_Time_Gated;Doppler Energy (keV);Counts", 8192, 0, 8192);
  tgList->Add(addDopp_tg);
  addDopp_ANum_tg = new TH2F("addDopp_ANum_tg", "Addback_Doppler_Energy_V_ArrayNumber_Time_Gated;Array Number;Doppler Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  tgList->Add(addDopp_ANum_tg);
  addDopp_exc_tg = new TH2F("addDopp_Exc_tg", "Addback_Doppler_Energy_V_Excitation_Energy_Time_Gated;Doppler Energy (keV);Exc Energy (MeV)", 8192, 0, 16384, 280, -2., 12.);
  tgList->Add(addDopp_exc_tg);
  addE_addE_tofg = new TH2F("addE_addE_tofg", "Addback_Gamma_Gamma_Time_Gated;Energy (keV);Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tgList->Add(addE_addE_tofg);
  addDopp_addDopp_tg = new TH2F("addDopp_addDopp_tg", "Addback_Gamma_Gamma_Time_Gated_Doppler_Corrected;Energy (keV);Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tgList->Add(addDopp_addDopp_tg);
  s3_E_theta_gated = new TH2F("s3_E_theta_gated","s3_E_theta_gated;#theta_{lab} (#circ);Energy (keV)",180,0,180, 820, 0, 16384); 
  tgList->Add(s3_E_theta_gated);
  s3_E_gated = new TH1F("S3_E_gated","S3_E_gated;Energy (keV);Counts",8192,0,16384);
  tgList->Add(s3_E_gated); 
  hitmap_time_gated = new TH2F("hitmap_time_gated","Hitmap_gated_on_emma_tof",200,-50,50,200,-50,50);
  s3List->Add(hitmap_time_gated);
  icSumVSi_gated_350 = new TH2F("icSumVSi_gated_350","Si_Energy_VS_IC_SUM_gated_on_350_gamma",4096,0,4096,16384,0,16384);
  tgList->Add(icSumVSi_gated_350);



  //Time Random Gated Spectra
  excE_bg = new TH1F("excE_bg","Excitation_Energy_MeV_Random_Time_Gated;Exc Energy (MeV);Counts",2800,-2.,12.); 
  bgList->Add(excE_bg);
  excE_theta_bg = new TH2F("excE_theta_bg","Excitation_Energy_Theta_Random_Time_Gated;#theta_{lab} (#circ);Exc Energy (MeV)",180,0,180,2800,-2.,12.); 
  bgList->Add(excE_theta_bg);
  addDopp_bg = new TH1F("addDopp_bg", "Addback_Doppler_Energy_Random_Time_Gated;Doppler Energy (keV);Counts", 8192, 0, 16384);
  bgList->Add(addDopp_bg);
  addDopp_ANum_bg = new TH2F("addDopp_ANum_bg", "Addback_Doppler_Energy_V_ArrayNumber_Random_Time_Gated;Array Number;Doppler Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  bgList->Add(addDopp_ANum_bg);
  addDopp_exc_bg = new TH2F("addDopp_Exc_bg", "Addback_Doppler_Energy_V_Excitation_Energy_Random_Time_Gated;Doppler Energy (keV);Exc Energy (MeV)", 8192, 0, 16384, 2800, -2., 12.);
  bgList->Add(addDopp_exc_bg);

  //PID Gated Spectra
  doppE_19F_pidg = new TH1F("doppE_19F_pidg","Doppler energy gated on 19F PID and TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(doppE_19F_pidg);
  doppE_24Na_pidg = new TH1F("doppE_24Na_pidg","Doppler energy gated on 22Ne PID and TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(doppE_24Na_pidg);
  doppE_26Mg_pidg = new TH1F("doppE_26Mg_pidg","Doppler energy gated on 26Mg PID and TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(doppE_26Mg_pidg);
  //doppE_26Mg_pidg_delayT = new TH1F("doppE_26Mg_pidg_delayT","Doppler energy gated on 26Mg PID and delayed TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  //PIDgatedList->Add(doppE_26Mg_pidg_delayT);
  doppE_26Mg_pidg_xPosG = new TH1F("doppE_26Mg_pidg_xPosG","Doppler energy gated on 26Mg PID & PGAC xPos<0 and TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(doppE_26Mg_pidg_xPosG);
  doppE_26Al_pidg = new TH1F("doppE_26Al_pidg","Doppler energy gated on 26Al PID and TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(doppE_26Al_pidg);


  doppE_Mg_pid_s3_gated = new TH1F("doppE_Mg_pid_s3_gated","Doppler energy gated on Mg PID and TIG-EMMA Timing and hit in S3;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(doppE_Mg_pid_s3_gated);


  s3_E_theta_Mg_PID_Gated = new TH2F("s3_E_theta_Mg_PID_Gated","S3 E vs #theta gated on Mg PID;#theta_{lab} (#circ);Energy (keV)",180,0,180,8192,0,16384);
  PIDgatedListS3->Add(s3_E_theta_Mg_PID_Gated);
  s3_E_theta_Mg_PID_1808_gamma_gated = new TH2F("s3_E_theta_Mg_PID_1808_gamma_gated","S3 E vs #theta gated on Mg PID, TIG-EMMA TOF, 1808 keV #gamma;#theta_{cm} (#circ);Energy (keV)",180,0,180,8192,0,16384);
  PIDgatedListS3->Add(s3_E_theta_Mg_PID_1808_gamma_gated);

  excE_thetaCm_Mg_PID_Gated = new TH2F("excE_thetaCm_Mg_PID_Gated","Exc Energy vs #theta gated on Mg PID;#theta_{lab} (#circ);Exc Energy (keV)",180,0,180,8192,0,16384);
  PIDgatedListS3->Add(excE_thetaCm_Mg_PID_Gated);
  excE_thetaCm_Mg_PID_1808_gamma_gated = new TH2F("excE_thetaCm_Mg_PID_1808_gamma_gated","Exc Energy vs #theta gated on Mg PID, TIG-EMMA TOF, 1808 keV #gamma;#theta_{cm} (#circ);Exc Energy (keV)",180,0,180,8192,0,16384);
  PIDgatedListS3->Add(excE_thetaCm_Mg_PID_1808_gamma_gated);

  excE_doppE_Mg_PID_Gated = new TH2F("excE_doppE_Mg_PID_Gated","Exc Energy vs Doppler Energy, Mg PID and EMMA-TIG TOF Gated;Doppler Energy (keV);Exc Energy (keV)",2048,0,16384,1024,0,16384);
  PIDgatedListS3->Add(excE_doppE_Mg_PID_Gated);


  excE_PIDG = new TH1F("excE_PIDG","Excitation_Energy_MeV_PID_Gated;Exc Energy (MeV);Counts",2800,-2.,12.); 
  PIDgatedList->Add(excE_PIDG);
  excE_theta_PIDG = new TH2F("excE_theta_PIDG","Excitation_Energy_Theta_PID_Gated;#theta_{lab} (#circ);Exc Energy (MeV)",180,0,180,2800,-2.,12.); 
  PIDgatedList->Add(excE_theta_PIDG);
  addDopp_PIDG = new TH1F("addDopp_PIDG", "Addback_Doppler_Energy_PID_Gated;Doppler Energy (keV);Counts", 8192, 0, 16384);
  PIDgatedList->Add(addDopp_PIDG);
  addDopp_ANum_PIDG = new TH2F("addDopp_ANum_PIDG", "Addback_Doppler_Energy_V_ArrayNumber_PID_Gated;Array Number;Doppler Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  PIDgatedList->Add(addDopp_ANum_PIDG);
  addDopp_exc_PIDG = new TH2F("addDopp_exc_PIDG", "Addback_Doppler_Energy_V_Excitation_Energy_PID_GatedDoppler Energy (keV);Exc Energy (MeV)", 8192, 0, 16384, 2800, -2., 12.);
  PIDgatedList->Add(addDopp_exc_PIDG);

  excE_PIDG_bg = new TH1F("excE_PIDG_bg","Excitation_Energy_MeV_PID_Gated_Random_Time_Gated;Exc Energy (MeV)",2800,-2.,12.); 
  PIDgatedList->Add(excE_PIDG_bg);
  excE_theta_PIDG_bg = new TH2F("excE_theta_PIDG_bg","Excitation_Energy_Theta_PID_Gated_Random_Time_Gated;#theta_{lab} (#circ);Exc Energy (MeV)",180,0,180,2800,-2.,12.); 
  PIDgatedList->Add(excE_theta_PIDG_bg);
  addDopp_PIDG_bg = new TH1F("addDopp_PIDG_bg", "Addback_Doppler_Energy_PID_Gated_Random_Time_Gated;Doppler Energy (keV);Counts", 8192, 0, 16384);
  PIDgatedList->Add(addDopp_PIDG_bg);
  addDopp_ANum_PIDG_bg = new TH2F("addDopp_ANum_PIDG_bg", "Addback_Doppler_Energy_V_ArrayNumber_PID_Gated_Random_Time_Gated;Array Number;Doppler Energy (keV)", 64, 0, 64, 8192, 0, 16384);
  PIDgatedList->Add(addDopp_ANum_PIDG_bg);
  addDopp_exc_PIDG_bg = new TH2F("addDopp_exc_PIDG_bg", "Addback_Doppler_Energy_V_Excitation_Energy_PID_Gated_Random_Time_Gated;Doppler Energy (keV);Exc Energy (MeV)", 8192, 0, 16384, 2800, -2., 12.);
  PIDgatedList->Add(addDopp_exc_PIDG_bg);
  aE_icSumVSi_gated = new TH1F("aE_icSumVSi_gated","Addback_gamma_energy_gated_on_IC_Si_cut;Energy (keV);Counts",8192,0,16384);
  PIDgatedList->Add(aE_icSumVSi_gated); 

  rings_PIDG = new TH2F("rings_PID_Gated","rings_PID_Gated",24,0,24,2048,0,8192); 
  PIDgatedListS3->Add(rings_PIDG);
  s3_E_theta_gated_PIDG = new TH2F("s3_E_theta_gated_PID_Gated","s3_E_theta_gated_PID_Gated;Energy (keV);#theta_{lab} (#circ)",180,0,180, 820, 0, 16384); 
  PIDgatedListS3->Add(s3_E_theta_gated_PIDG);
  s3_E_gated_PIDG_gamG = new TH1F("s3_E_gated_PIDG_gamG","s3_E_theta_gated_PID_Gated_1806keV_Gated;Energy (keV);",8192, 0, 16.384); 
  PIDgatedListS3->Add(s3_E_gated_PIDG_gamG);

}
