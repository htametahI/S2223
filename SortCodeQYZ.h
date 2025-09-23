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

TList *tigList, *emmaList, *s3List, *PIDList, *tofList, *emmaS3List;

// TIGRESS
TH1F *tigE, *tigAddE, *tigAddDoppE, *gammaExc, *gammaExc10p9, *gammaExc11p09; 

// EMMA
TH1F *emmaXpos, *emmaYpos, *emmaICSum, *emmaICE[5], *emmaSSBE[2], *emmaSiE; 
TH2F *emmadEE, *emmaTigdt, *emmaICN, *emmaIC0V1, *emmaIC0V2, *emmaIC0V3, *emmaIC1V2, *emmaIC1V3, *emmaIC2V3;
TH2F *emmadtE, *emmaICSumVSi, *emmaICSumVSiPlusIC, *emmaIC0VSi, *emmaIC1VSi, *emmaIC2VSi, *emmaIC3VSi;
TH2F *emmaPgac,  *emmaPIDGatedPgac;

// EMMA - S3 
TH2F *emmaS3TimeGatedPgac; 
TH1F *mg26ExcEmmaS3, *mg26ExcPIDGated, *mg26ExcTimeGated, *mg26Exc1808keV;

// IC
TH1F *emmaICSegmentEnergy[5];
TH2F *emmaICSegment;  

// S3
TH1F *s3E, *s3RingsSectorsSinglesT, *s3RingCounts, *s3SecCounts, *s3Rate;
TH2F *s3Rings, *s3Sectors, *s3RingsT, *s3SectorsT, *siETheta, *s3Hitmap, *s3RingsSectors;

// TOF
TH1F *s3EmmaTof, *ssbICTof, *ssbSiTof, *tigICTof, *tigAnodeTof;

// PID
TH1F *pgacXPos26MgPID, *addDopp26MgPID, *addDopp26MgPIDS3T; 
TH2F *pgac26MgPID, *pgacS3TgMg26PID; 


// Cuts
TCutG *Ne24Cut, *Mg26Cut, *Al26Cut, *Na22Cut;

class SortCode
{
public:
	SortCode() { ; }
	void SortData(const char *, const char *, const char *, const char *);
	void Initialise();
};
#endif

void SortCode::Initialise()
{

	printf("Start initializations\n");
	printf("Creating list\n");
	tigList = new TList;
	s3List = new TList;
	emmaList = new TList;
	PIDList = new TList;
	tofList = new TList;
	emmaS3List = new TList; 
	printf("Creating histograms\n");

	// FORMAT: 
	// hist = new TH1F("filename/label", "Title;Xlabel;Ylabel", #of bins, low, high)

	// TIGRESS
	tigE = new TH1F("tigE", "Tigress Raw Energy;Energy (keV);Counts", 2000, 0, 4500);
	tigList->Add(tigE);
	tigAddDoppE = new TH1F("tigAddDoppE", "Tigress Addback Doppler Corrected Energy;Energy (keV);Counts", 2000, 0, 4500);
	tigList->Add(tigAddDoppE);
	gammaExc = new TH1F("tigExc", "Tigress Addback Doppler Corrected Spectrum; Counts", 500, 0, 4500);
	tigList->Add(gammaExc); 
	gammaExc10p9 = new TH1F("tigExc10p9", "Tigress Addback Doppler Gated on 10.9MeV Excitation Energy; Counts", 500, 0, 4500);
	tigList->Add(gammaExc10p9); 
	gammaExc11p09 = new TH1F("tigExc11p09", "Tigress Addback Doppler Gated on 11.09MeV Excitation Energy; Counts", 500, 0, 4500); 
	tigList->Add(gammaExc11p09); 


	// EMMA
	emmaXpos = new TH1F("EMMAPgacXPosition", "PGAC X Position;X Position (mm);Counts", 160, -80, 80);
	emmaList->Add(emmaXpos);
	emmaYpos = new TH1F("EMMAPgacYPosition", "PGAC Y Position;Y Position (mm);Counts", 60, -30, 30);
	emmaList->Add(emmaYpos);
	emmaPgac = new TH2F("EMMAPgac", "PGAC Hit Pattern", 160, -80, 80, 60, -30, 30);
	emmaList->Add(emmaPgac);

	emmaPIDGatedPgac = new TH2F("emmaPIDGatedPgac", "PGAC PID Gated Hit Pattern", 160, -80, 80, 60, -30, 30);
	emmaList->Add(emmaPIDGatedPgac);

	emmaSiE = new TH1F("EMMASiE", "EMMA Focal Plane Silicon Energy;Energy (keV);Counts", 16384, 0, 16384);
	emmaList->Add(emmaSiE);
	emmaICSumVSi = new TH2F("emmaICSumVSi", "Si Energy VS IC SUM;Silicon Energy (keV);IC Sum (keV)", 1500, 0, 1500, 6000, 0, 6000);
	emmaList->Add(emmaICSumVSi);
	emmaICSumVSiPlusIC = new TH2F("emmaICSumVSiPlusIC", "Si + IC SUM Energy VS IC SUM", 800, 2048, 4600, 800, 1500, 4500);
	emmaICSumVSiPlusIC->GetXaxis()->SetTitle("Silicon + IC Sum");
	emmaICSumVSiPlusIC->GetYaxis()->SetTitle("IC Sum");
	emmaList->Add(emmaICSumVSiPlusIC);
	emmaIC0VSi = new TH2F("emmaIC0VSi", "Si Energy VS IC0", 4096, 0, 4096, 16384, 0, 16384);
	emmaList->Add(emmaIC0VSi);
	emmaIC1VSi = new TH2F("emmaIC1VSi", "Si Energy VS IC1", 4096, 0, 4096, 16384, 0, 16384);
	emmaList->Add(emmaIC1VSi);
	emmaIC2VSi = new TH2F("emmaIC2VSi", "Si Energy VS IC2", 4096, 0, 4096, 16384, 0, 16384);
	emmaList->Add(emmaIC2VSi);
	emmaIC3VSi = new TH2F("emmaIC3VSi", "Si Energy VS IC3", 4096, 0, 4096, 16384, 0, 16384);
	emmaList->Add(emmaIC3VSi);

	// S3
	s3E = new TH1F("S3 Energy", "S3 Energy;Energy (keV);Counts", 8192, 0, 8192);
	s3List->Add(s3E);
	s3RingsSectors = new TH2F("s3RingsSectors", "S3 sectorE vs ringE Multiplicity;Ring Energy;Sector Energy", 2048, 0, 8192, 2048, 0, 8192);
	s3List->Add(s3RingsSectors);
	s3RingsSectorsSinglesT = new TH1F("s3RingsSectorsSinglesT", "S3 sectorT vs ringT Multiplicity 1;Sector Time - Ring Time", 2048, -1024, 1024);
	s3RingsSectorsSinglesT->Add(s3RingsSectorsSinglesT);

	// EMMA - S3 
	emmaS3TimeGatedPgac = new TH2F("emmaS3TimeGatedPgac", "PGAC Time Gated Hit Pattern", 160, -80, 80, 120, -60, 60);
	emmaS3List->Add(emmaS3TimeGatedPgac);
	mg26ExcEmmaS3 = new TH1F("mg26Exc", "EMMA-S3 Gated 26Mg Excitation energy; Energy(MeV); Counts", 100, 0, 16);
	emmaS3List->Add(mg26ExcEmmaS3);
	mg26ExcPIDGated = new TH1F("mg26ExcPID", "Mg-26 Excitation Energy PID Gated; Counts", 100, 0, 16);
	emmaS3List->Add(mg26ExcPIDGated);
	mg26ExcTimeGated = new TH1F("mg26ExcTime", "Mg-26 Excitation Energy Time Gated; Counts", 100, 0, 16);
	emmaS3List->Add(mg26ExcTimeGated);
	mg26Exc1808keV = new TH1F("mg26Exc1808keV", "Mg-26 Excitation Energy 1808keV Gated; Counts", 100, 0, 16); 
	emmaS3List->Add(mg26Exc1808keV); 

	// TOF
	s3EmmaTof = new TH1F("s3EmmaTof", "S3 EMMA TOF; S3 EMMA Time Difference(ns);", 20000, -10000, 10000);
	tofList->Add(s3EmmaTof);
	ssbICTof = new TH1F("ssbICTof", "SSB IC TOF; SSB IC Time Difference(ns);", 20000, -10000, 10000);
	tofList->Add(ssbICTof);
	ssbSiTof = new TH1F("ssbSiTof", "SSB Si TOF; SSB Si Time Difference(ns);", 20000, -10000, 10000);
	tofList->Add(ssbSiTof);
	tigICTof = new TH1F("tigICTof", "TIGRESS IC TOF; TIGRESS IC Time Difference(ns);", 20000, -10000, 10000);
	tofList->Add(tigICTof);
	tigAnodeTof = new TH1F("tigAnodeTof", "TIGRESS EMMA Anode TOF; TIGRESS EMMA Anode Time Difference(ns);", 20000, -10000, 10000);
	tofList->Add(tigAnodeTof);

	// IC
	for (int i = 0; i < 5; i++)
	{
		char hname[64];
		sprintf(hname, "EMMA IC Segment_%i", i);
		emmaICSegmentEnergy[i] = new TH1F(hname, hname, 2000, 0, 2000);
		emmaICSegmentEnergy[i]->GetXaxis()->SetTitle("IC Energy [keV]");
		emmaICSegmentEnergy[i]->GetYaxis()->SetTitle("Counts");
		emmaList->Add(emmaICSegmentEnergy[i]);
	}

	emmaICSegment = new TH2F("EMMA IC Segments", "IC Energy vs Counts; IC Segment; Counts", 5, 0, 5, 4096, 0, 4096); 
	emmaList->Add(emmaICSegment); 

	// PID 
	pgac26MgPID = new TH2F("Mg-26 PID Gated Pgac", "Mg-26 PID Gated Pgac; X Position (mm);Y Position(mm)", 160, -80, 80, 120, -60, 60); 
	PIDList->Add(pgac26MgPID);
	pgacXPos26MgPID = new TH1F("Mg-26 PID Gated Pgac X Position", "Mg-26 PID Gated Pgac X Position; X Postion(mm); Counts", 100, -40, 40); 
	PIDList->Add(pgacXPos26MgPID);
	addDopp26MgPID = new TH1F("Mg-26 PID Gated Doppler Corrected TIGRESS Addback Spectrum", "addDopp;Energy (keV);Counts", 1000, 0, 8192);
	PIDList->Add(addDopp26MgPID); 
	addDopp26MgPIDS3T = new TH1F("Mg-26 PID EMMA-S3 Time Gated Doppler Corrected TIGRESS Addback Spectrum", "addDopppS3TPID;Energy (keV);Counts", 1000, 0, 8192);
	PIDList->Add(addDopp26MgPIDS3T); 
	pgacS3TgMg26PID = new TH2F("Mg-26 PID EMMA-S3 Time Gated Pgac", "Mg-26 PID EMMA-S3 Time Gated Pgac; X Position (mm);Y Position(mm)", 160, -80, 80, 120, -60, 60); 
	PIDList->Add(pgacS3TgMg26PID);


}
