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

TList *tigList, *emmaList, *s3List, *PIDList, *tofList;

// TIGRESS
TH1F *tigE, *tigAddE, *tigAddDoppE;

// EMMA
TH1F *emmaXpos, *emmaYpos, *emmaICSum, *emmaICE[5], *emmaSSBE[2], *emmaSiE;
TH2F *emmadEE, *emmaTigdt, *emmaICN, *emmaIC0V1, *emmaIC0V2, *emmaIC0V3, *emmaIC1V2, *emmaIC1V3, *emmaIC2V3;
TH2F *emmadtE, *emmaICSumVSi, *emmaICSumVSiPlusIC, *emmaIC0VSi, *emmaIC1VSi, *emmaIC2VSi, *emmaIC3VSi;
TH2F *emmaPgac, *emmaS3TimeGatedPgac, *emmaPIDGatedPgac;

// IC
TH1F *emmaICSegmentEnergy[5];
TH2F *emmaICSegment;  

// S3
TH1F *s3E, *s3RingsSectorsSinglesT, *s3RingCounts, *s3SecCounts, *s3Rate;
TH2F *s3Rings, *s3Sectors, *s3RingsT, *s3SectorsT, *siETheta, *s3Hitmap, *s3RingsSectors;

// TOF
TH1F *s3EmmaTof, *ssbICTof, *ssbSiTof, *tigICTof, *tigAnodeTof;

// PID



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
	printf("Creating histograms\n");

	// TIGRESS
	tigE = new TH1F("tigE", "TigressEnergy;Energy (keV);Counts", 8192, 0, 16384);
	tigList->Add(tigE);
	tigAddE = new TH1F("tigAddE", "TigressAddbackEnergy;Energy (keV);Counts", 8192, 0, 16384);
	tigList->Add(tigAddE);
	tigAddDoppE = new TH1F("tigAddDoppE", "TigressAddbackDopplerCorrectedEnergy;Energy (keV);Counts", 8192, 0, 16384);
	tigList->Add(tigAddDoppE);

	// EMMA
	emmaXpos = new TH1F("EMMAPgacXPosition", "PGAC X Position;X Position (mm);Counts", 160, -80, 80);
	emmaList->Add(emmaXpos);
	emmaYpos = new TH1F("EMMAPgacYPosition", "PGAC Y Position;Y Position (mm);Counts", 60, -30, 30);
	emmaList->Add(emmaYpos);
	emmaPgac = new TH2F("EMMAPgac", "PGAC Hit Pattern", 160, -80, 80, 60, -30, 30);
	emmaList->Add(emmaPgac);
	emmaS3TimeGatedPgac = new TH2F("emmaS3TimeGatedPgac", "PGAC Time Gated Hit Pattern", 160, -80, 80, 60, -30, 30);
	emmaList->Add(emmaS3TimeGatedPgac);
	emmaPIDGatedPgac = new TH2F("emmaPIDGatedPgac", "PGAC PID Gated Hit Pattern", 160, -80, 80, 60, -30, 30);
	emmaList->Add(emmaPIDGatedPgac);

	emmaSiE = new TH1F("EMMASiE", "EMMA Focal Plane Silicon Energy;Energy (keV);Counts", 16384, 0, 16384);
	emmaList->Add(emmaSiE);
	emmadEE = new TH2F("emmadEE", "Si Energy VS IC SUM, EMMA;Silicon Energy (arb.);IC Sum (arb.)", 4096, 0, 4096, 16384, 0, 16384);
	emmaList->Add(emmadEE);
	emmaICSumVSi = new TH2F("emmaICSumVSi", "Si Energy VS IC SUM;Silicon Energy (arb.);IC Sum (arb.)", 4096, 0, 4096, 16384, 0, 16384);
	emmaList->Add(emmaICSumVSi);
	emmaICSumVSiPlusIC = new TH2F("emmaICSumVSiPlusIC", "Si + IC SUM Energy VS IC SUM", 256, 2048, 8192, 256, 1500, 4500);
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
}
