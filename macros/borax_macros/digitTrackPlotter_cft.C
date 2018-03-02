#include "THStack.h"
#include <climits>
#include <vector>
#include "TROOT.h"
#include "TRint.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TView.h"
#include "TF1.h"
#include "TCut.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TPolyLine3D.h"
#include "TAxis.h"
#include "TChain.h"
#include "TTree.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TLine.h"
#include "TPolyLine.h"
#include "TMarker.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TText.h"
#include "TVector3.h"

#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>


void digitTrackPlotter_cft(const int preamp, const char* actinide, const char* otherTreeCuts="&&1&&fitType==5",const char* pidType="zCut",const char* driftVel="") {

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
c1->SetGrid();
TCanvas *dig_canvas = new TCanvas("dig_canvas","dig_canvas",1400,1400);
char c1PlotName[234];
gStyle->SetPalette(84);
gStyle->SetOptStat(0);
gStyle->SetNumberContours(99);
char buffChar[345];

int zCut=0;
double MeV_per_ADC=0;
double ADC_per_MeV=0;
double dEnergy = 0.1;
double fragEnergyThreshold=0;
double deg2rad = TMath::Pi()/(double)180;

char noMaskCut[234];
char cftCut[234];
char noMaskRunCut[234];
char cftRunCut[234];
char azCut[234];
char noMaskDigitFileName[234];
char cftDigitFileName[234];
char noMaskTrackFileName[234];
char cftTrackFileName[234];
char pidFile_char[345];

if (preamp==1)
{
	zCut=70;
	fragEnergyThreshold=2.5;
	ADC_per_MeV=1180/6.118;
	MeV_per_ADC=6.118/1180;
	if (!(strcmp(actinide,"cf252"))){}
	sprintf(noMaskRunCut,"run>100001856&&run!=100001870");
	sprintf(noMaskCut," %s&&%s",otherTreeCuts,noMaskRunCut);
	sprintf(cftCut,"%s",otherTreeCuts);
}
else if (preamp==2)
{
	zCut=30;
	fragEnergyThreshold=2.0;
	ADC_per_MeV=1450/6.118;
	MeV_per_ADC=6.118/1450;
	if (!(strcmp(actinide,"cf252"))){
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
	}
	if ( !(strcmp(actinide,"cm244"))){
		sprintf(azCut,"direction.Phi()<2.2");
	}
	sprintf(noMaskCut,"%s&&%s",otherTreeCuts,azCut);
	sprintf(cftCut,"%s&&%s",otherTreeCuts,azCut);
}
/////////  create TCutG chars ///////////
char fragCut_adc_char[345];
char fragCut_unlikely_adc_char[345];
char alphaCut_adc_char[345];
char alpha_short_high_adc_char[345];
char alpha_short_low_adc_char[345];
char above_zCut_adc_char[345];
sprintf(fragCut_unlikely_adc_char,"fragCut_unlikely_adc");
sprintf(fragCut_adc_char,"fragCut_adc");
sprintf(alphaCut_adc_char,"alphaCut_adc");
sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
sprintf(above_zCut_adc_char,"alphaCut_zCut%03d_adc",zCut);
char fragCut_energy_char[345];
char fragCut_unlikely_energy_char[345];
char alphaCut_energy_char[345];
char alpha_short_high_energy_char[345];
char alpha_short_low_energy_char[345];
char above_zCut_energy_char[345];
sprintf(fragCut_unlikely_energy_char,"fragCut_unlikely_energy");
sprintf(fragCut_energy_char,"fragCut_energy");
sprintf(alphaCut_energy_char,"alphaCut_energy");
sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
sprintf(above_zCut_energy_char,"alphaCut_zCut%03d_energy",zCut);

////////////////////// creating length-energy TCutGs ///////////////////////////////////
printf("////////////////////// creating length-energy TCutGs ///////////////////////////////////\n");
sprintf(pidFile_char,"$cft/pid/pidCut_preamp%d_zCut%d_fragThresh%0.1f.driftVel%s.root",preamp,zCut,fragEnergyThreshold,driftVel);
TFile* pidFile = new TFile(pidFile_char);
TCutG* fragCut_adc = (TCutG*)pidFile->Get(fragCut_adc_char);
fragCut_adc->SetName("fragCut_adc");
fragCut_adc->SetLineColor(kViolet);
fragCut_adc->SetLineWidth(1);
fragCut_adc->SetFillColorAlpha(kViolet,0.2);
TCutG* fragCut_unlikely_adc = (TCutG*)pidFile->Get(fragCut_unlikely_adc_char);
fragCut_unlikely_adc->SetName("fragCut_unlikely_adc");
fragCut_unlikely_adc->SetLineColor(kViolet-9);
fragCut_unlikely_adc->SetLineWidth(1);
fragCut_unlikely_adc->SetFillColorAlpha(kViolet-9,0.2);
TCutG* alphaCut_above_zCut_adc = (TCutG*)pidFile->Get(above_zCut_adc_char);
alphaCut_above_zCut_adc->SetName("alphaCut_above_zCut_adc");
alphaCut_above_zCut_adc->SetLineColor(kSpring);
alphaCut_above_zCut_adc->SetLineWidth(2);
alphaCut_above_zCut_adc->SetFillColorAlpha(kSpring,0.2);
TCutG* alphaCut_long_low_adc = (TCutG*)pidFile->Get("alphaCut_long_low_adc");
alphaCut_long_low_adc->SetName("alphaCut_long_low_adc");
alphaCut_long_low_adc->SetLineColor(kCyan);
alphaCut_long_low_adc->SetLineWidth(1);
alphaCut_long_low_adc->SetFillColorAlpha(kYellow,0.2);
TCutG* alphaCut_long_adc = (TCutG*)pidFile->Get("alphaCut_long_adc");
alphaCut_long_adc->SetName("alphaCut_long_adc");
alphaCut_long_adc->SetLineColor(kCyan);
alphaCut_long_adc->SetLineWidth(1);
alphaCut_long_adc->SetFillColorAlpha(kCyan,0.2);
TCutG* long_high_adc = (TCutG*)pidFile->Get("long_high_adc");
long_high_adc->SetName("long_high_adc");
long_high_adc->SetLineColor(kBlue);
long_high_adc->SetLineWidth(1);
long_high_adc->SetFillColorAlpha(kBlue,0.2);
TCutG* alphaCut_short_higher_adc = (TCutG*)pidFile->Get(alpha_short_high_adc_char);
alphaCut_short_higher_adc->SetName("alphaCut_short_higher_adc");
alphaCut_short_higher_adc->SetLineColor(kOrange+7);
alphaCut_short_higher_adc->SetLineWidth(1);
alphaCut_short_higher_adc->SetFillColorAlpha(kOrange+7,0.2);
TCutG* alphaCut_short_low_adc = (TCutG*)pidFile->Get(alpha_short_low_adc_char);
alphaCut_short_low_adc->SetName("alphaCut_short_low_adc");
alphaCut_short_low_adc->SetLineColor(kRed);
alphaCut_short_low_adc->SetLineWidth(1);
alphaCut_short_low_adc->SetFillColorAlpha(kRed,0.2);
TCutG* fragCut_energy = (TCutG*)pidFile->Get(fragCut_energy_char);
fragCut_energy->SetName("fragCut_energy");
fragCut_energy->SetLineColor(kViolet);
fragCut_energy->SetLineWidth(1);
fragCut_energy->SetFillColorAlpha(kViolet,0.2);
TCutG* fragCut_unlikely_energy = (TCutG*)pidFile->Get(fragCut_unlikely_energy_char);
fragCut_unlikely_energy->SetName("fragCut_unlikely_energy");
fragCut_unlikely_energy->SetLineColor(kViolet-9);
fragCut_unlikely_energy->SetLineWidth(1);
fragCut_unlikely_energy->SetFillColorAlpha(kViolet-9,0.2);
TCutG* alphaCut_above_zCut_energy = (TCutG*)pidFile->Get(above_zCut_energy_char);
alphaCut_above_zCut_energy->SetName("above_zCut_energy");
alphaCut_above_zCut_energy->SetLineColor(kSpring);
alphaCut_above_zCut_energy->SetLineWidth(2);
alphaCut_above_zCut_energy->SetFillColorAlpha(kSpring,0.2);
TCutG* alphaCut_long_low_energy = (TCutG*)pidFile->Get("alphaCut_long_low_energy");
alphaCut_long_low_energy->SetName("alphaCut_long_low_energy");
alphaCut_long_low_energy->SetLineColor(kCyan);
alphaCut_long_low_energy->SetLineWidth(1);
alphaCut_long_low_energy->SetFillColorAlpha(kYellow,0.2);
TCutG* alphaCut_long_energy = (TCutG*)pidFile->Get("alphaCut_long_energy");
alphaCut_long_energy->SetName("alphaCut_long_energy");
alphaCut_long_energy->SetLineColor(kCyan);
alphaCut_long_energy->SetLineWidth(1);
alphaCut_long_energy->SetFillColorAlpha(kCyan,0.2);
TCutG* long_high_energy = (TCutG*)pidFile->Get("long_high_energy");
long_high_energy->SetName("long_high_energy");
long_high_energy->SetLineColor(kBlue);
long_high_energy->SetLineWidth(1);
long_high_energy->SetFillColorAlpha(kBlue,0.2);
TCutG* alphaCut_short_higher_energy = (TCutG*)pidFile->Get(alpha_short_high_energy_char);
alphaCut_short_higher_energy->SetName("alphaCut_short_higher_energy");
alphaCut_short_higher_energy->SetLineColor(kOrange+7);
alphaCut_short_higher_energy->SetLineWidth(1);
alphaCut_short_higher_energy->SetFillColorAlpha(kOrange+7,0.2);
TCutG* alphaCut_short_low_energy = (TCutG*)pidFile->Get(alpha_short_low_energy_char);
alphaCut_short_low_energy->SetName("alphaCut_short_low_energy");
alphaCut_short_low_energy->SetLineColor(kRed);
alphaCut_short_low_energy->SetLineWidth(1);
alphaCut_short_low_energy->SetFillColorAlpha(kRed,0.2);
char noMaskFragCut[234];sprintf(noMaskFragCut,"%s&&%s", noMaskCut,"fragCut_adc");
char noMaskFragUnlikelyCut_char[234];sprintf(noMaskFragUnlikelyCut_char,"%s&&%s", noMaskCut,"fragCut_unlikely_adc");
char noMaskAlphaCut_above_zCut_adc_char[234];sprintf(noMaskAlphaCut_above_zCut_adc_char,"%s&&%s", noMaskCut,"alphaCut_above_zCut_adc");
char noMaskAlphaCut_long_char[234];sprintf(noMaskAlphaCut_long_char,"%s&&%s", noMaskCut,"alphaCut_long_adc");
char noMaskAlphaCut_long_low_char[234];sprintf(noMaskAlphaCut_long_low_char,"%s&&%s", noMaskCut,"alphaCut_long_low_adc");
char noMaskAlphaCut_long_high_adc_char[234];sprintf(noMaskAlphaCut_long_high_adc_char,"%s&&%s", noMaskCut,"long_high_adc");
char noMaskAlphaCut_short_higher_adc_char[234];sprintf(noMaskAlphaCut_short_higher_adc_char,"%s&&%s", noMaskCut,"alphaCut_short_higher_adc");
char noMaskAlphaCut_alphaCut_short_low_adc_char[234];sprintf(noMaskAlphaCut_alphaCut_short_low_adc_char,"%s&&%s", noMaskCut,"alphaCut_short_low_adc");
char cftFragCut[234];sprintf(cftFragCut,"%s&&%s", cftCut,"fragCut_adc");
char cftFragUnlikelyCut[234];sprintf(cftFragUnlikelyCut,"%s&&%s", cftCut,"fragCut_unlikely_adc");
char cftAlphaCut_above_zCut_adc_char[234];sprintf(cftAlphaCut_above_zCut_adc_char,"%s&&%s", cftCut,"alphaCut_above_zCut_adc");
char cftAlphaCut_long_char[234];sprintf(cftAlphaCut_long_char,"%s&&%s", cftCut,"alphaCut_long_adc");
char cftAlphaCut_long_low_char[234];sprintf(cftAlphaCut_long_low_char,"%s&&%s", cftCut,"alphaCut_long_low_adc");
char cftAlphaCut_long_high_adc_char[234];sprintf(cftAlphaCut_long_high_adc_char,"%s&&%s", cftCut,"long_high_adc");
char cftAlphaCut_short_higher_adc_char[234];sprintf(cftAlphaCut_short_higher_adc_char,"%s&&%s", cftCut,"alphaCut_short_higher_adc");
char cftAlphaCut_alphaCut_short_low_adc_char[234];sprintf(cftAlphaCut_alphaCut_short_low_adc_char,"%s&&%s", cftCut,"alphaCut_short_low_adc");
printf("noMaskCut = %s\n",noMaskCut);
printf("cftCut = %s\n",cftCut);
printf("otherTreeCuts = %s\n",otherTreeCuts);

///////// digits plus track /////////
printf("///////// digits plus track /////////\n");
///////// initialize track tree /////////
printf("///////// initialize track tree /////////\n");
sprintf(noMaskTrackFileName,"/p/lscratche/jerbundg/data/thesisData/trees/%s.driftVel%s_noMask%d.nEv2k.trees.root",actinide,driftVel,preamp);
printf("noMaskTrackFileName: %s\n\n", noMaskTrackFileName);
TFile* noMaskTrackFile = new TFile(noMaskTrackFileName);
TTree *tracks; noMaskTrackFile->GetObject("tracks", tracks);
///////// initialize digit tree /////////
printf("///////// initialize digit tree /////////\n");
sprintf(noMaskDigitFileName,"/p/lscratchd/tpcuser/cft_data/thesisData/trees/digTrkTreeDir/noMask%d.driftVel%s_trk_dig.2k.trees.root",preamp,driftVel);
printf("noMaskDigitFileName: %s\n\n", noMaskDigitFileName);
TFile* noMaskDigitFile = new TFile(noMaskDigitFileName);
TDirectory *cwd = gDirectory; // we create histograms "here"
TH3F *track3d = new TH3F("track3d", "track3d", 12, -7, 7, 12, -7, 7, 12, -7, 7);
track3d->SetStats(kFALSE);
TTree *digits; noMaskDigitFile->GetObject("tracks", digits);
digits->SetMarkerStyle(20);
cwd->cd(); // go to the directory where histograms are

TH2D* yx_digProjection = new TH2D("yx_digProjection","",100,-7,7,100,-7,7);
TH2D* zy_digProjection = new TH2D("zy_digProjection","",100,-7,7,100,-7,7);
TH2D* zx_digProjection = new TH2D("zx_digProjection","",100,-7,7,100,-7,7);

TH2D* LvsADC = new TH2D("LvsADC","LvsADC",1000,100,40000,100,0,10);
TH2D* ADCvsCosTheta = new TH2D("ADCvsCosTheta","ADCvsCosTheta",200,-1,1,1000,100,40000);
double cosTheta[10];
TH1F* trackCount_hist = new TH1F("trackCount_hist","trackCount_hist",10,0,10);
TH1F* trackADC = new TH1F("trackADC","trackADC",40000,0,40000);
TH1F* trackCosTheta = new TH1F("trackCosTheta","trackCosTheta",1000,-1,1);
double trk_adc[10];
TH1F* trackLength = new TH1F("trackLength","trackLength",1000,0,10);
double trk_length[10];
TH1F* start_hist = new TH1F("start_hist","start_hist",1000,-20,20);
double start_x[10];
double start_y[10];
double start_z[10];
TH1F* end_hist = new TH1F("end_hist","end_hist",1000,-20,20);
double end_x[10];
double end_y[10];
double end_z[10];
TPolyLine3D *pl_track = new TPolyLine3D(2);
pl_track->SetLineColor(kRed);
pl_track->SetLineWidth(3);

std::vector<std::string> LVEn_cut_char;

LVEn_cut_char.push_back(noMaskAlphaCut_above_zCut_adc_char);
LVEn_cut_char.push_back(noMaskFragCut);
LVEn_cut_char.push_back(noMaskFragUnlikelyCut_char);
LVEn_cut_char.push_back(noMaskAlphaCut_long_char);
LVEn_cut_char.push_back(noMaskAlphaCut_long_low_char);
LVEn_cut_char.push_back(noMaskAlphaCut_long_high_adc_char);
LVEn_cut_char.push_back(noMaskAlphaCut_short_higher_adc_char);
LVEn_cut_char.push_back(noMaskAlphaCut_alphaCut_short_low_adc_char);

const int first_run = 100002055;
const int last_run = 100002055;
const int first_event = 0;
const int last_event = 2000;

for (int run = first_run; run <= last_run; run++)
{
	int num_plots=0;
	for (int event_id = first_event; event_id <= last_event; event_id++)
	{
		char individ_eventCut[234];
		// sprintf(individ_eventCut,"fitType==5&&run==%d&&eventID==%d", run, event_id);
		sprintf(individ_eventCut,"fitType==5&&run==%d&&eventID==%d&&trackCount==2", run, event_id);
		tracks->Draw("trackCount>>trackCount_hist",individ_eventCut);
		if (trackCount_hist->GetEntries()==0) continue;
		int trk_count=trackCount_hist->FindLastBinAbove(0)-1;
		printf("\n\n**********************************************\n");
		printf("**********************************************\n");
		printf("run: %d \t	eventID: %d  \n", run, event_id);
		printf("**********************************************\n");
		printf("**********************************************\n");
		printf("trk_count:%d\n",trk_count);
		bool skip_event=1;
		for (int trk_n = 0; trk_n < trk_count; trk_n++)
		{
			sprintf(buffChar,"%s&&trackNo==%d",individ_eventCut,trk_n);
			tracks->Draw("adc>>trackADC",buffChar);
			trk_adc[trk_n] = trackADC->GetMean();
			tracks->Draw("length>>trackLength",buffChar);
			trk_length[trk_n] = trackLength->GetMean();
			printf("\t trk_adc[%d]: %f\n",trk_n, trk_adc[trk_n]);
			printf("\t trk_length[%d]: %f\n",trk_n, trk_length[trk_n]);
			LvsADC->Fill(trk_adc[trk_n],trk_length[trk_n]);
			tracks->Draw("direction.CosTheta()>>trackCosTheta",buffChar);
			cosTheta[trk_n] = trackCosTheta->GetMean();
			ADCvsCosTheta->Fill(cosTheta[trk_n],trk_adc[trk_n]);
			tracks->Draw("start.x()>>start_hist",buffChar);
			start_x[trk_n] = start_hist->GetMean();
			tracks->Draw("start.y()>>start_hist",buffChar);
			start_y[trk_n] = start_hist->GetMean();
			tracks->Draw("start.z()>>start_hist",buffChar);
			start_z[trk_n] = start_hist->GetMean();
			tracks->Draw("end.x()>>end_hist",buffChar);
			end_x[trk_n] = end_hist->GetMean();
			tracks->Draw("end.y()>>end_hist",buffChar);
			end_y[trk_n] = end_hist->GetMean();
			tracks->Draw("end.z()>>end_hist",buffChar);
			end_z[trk_n] = end_hist->GetMean();
			printf("fragCut_adc->IsInside(%f,%f): %d\n",trk_adc[trk_n],trk_length[trk_n],fragCut_adc->IsInside(trk_adc[trk_n],trk_length[trk_n]));
			if (fragCut_adc->IsInside(trk_adc[trk_n],trk_length[trk_n])) skip_event=0;
		}

		printf("skip_event: %d\n", skip_event);
		if(skip_event) continue;
		// if (!(trk_adc[0]>2000&&trk_adc[1]<800&&trk_length[1]<1)) continue;

		track3d->SetTitle(individ_eventCut);
		track3d->Reset("M");
		if (!(track3d->TestBit(TH1::kNoStats)))
		{
			digits->Project("track3d", "dig_x:dig_y:dig_z", individ_eventCut, "");
			((TArrayF *)track3d)->Reset();
		}
		dig_canvas->cd();

		digits->Draw("dig_z:dig_y:dig_x:dig_adc>>+track3d", individ_eventCut, "col");
		track3d->GetXaxis()->SetTitle("X [cm]");
		track3d->GetXaxis()->SetTitleOffset(1.6);
		track3d->GetYaxis()->SetTitle("Y [cm]");
		track3d->GetYaxis()->SetTitleOffset(1.6);
		track3d->GetZaxis()->SetTitle("Z [cm]");
		TPolyLine3D *pl_hex_plus = new TPolyLine3D(5);
		pl_hex_plus->SetLineColor(kSpring);
		pl_hex_plus->SetLineWidth(2);
		for (int n = 0; n <= 6; ++n)
			pl_hex_plus->SetPoint(n,
				5*TMath::Cos(n*60*TMath::Pi()/180),
				5*TMath::Sin(n*60*TMath::Pi()/180),
				5);
		pl_hex_plus->Draw("same");
		TPolyLine3D *pl_hex_minus = new TPolyLine3D(5);
		pl_hex_minus->SetLineColor(kSpring);
		pl_hex_minus->SetLineWidth(2);
		for (int n = 0; n <= 6; ++n)
			pl_hex_minus->SetPoint(n,
				5*TMath::Cos(n*60*TMath::Pi()/180),
				5*TMath::Sin(n*60*TMath::Pi()/180),
				-5);
		pl_hex_minus->Draw("same");
		TPolyLine3D *pl_circle = new TPolyLine3D(5);
		pl_circle->SetLineColor(kRed);
		pl_circle->SetLineWidth(2);
		for (int n = 0; n <= 60; ++n)
			pl_circle->SetPoint(n,
				1*TMath::Cos(n*6*TMath::Pi()/180),
				1*TMath::Sin(n*6*TMath::Pi()/180),
				0);
		pl_circle->Draw("same");

		for (int trk_n = 0; trk_n < trackCount_hist->GetMaximum(); trk_n++)
		{
			// if (trk_n > 9) continue; // just a precaution (0 ... 9 are allowed)
			TPolyLine3D *pl_track = new TPolyLine3D(2);
			pl_track->SetLineColor(kYellow);
			pl_track->SetLineWidth(5);
			printf("trk_n:%d, start_x:%f, start_y:%f, start_z:%f\n",
				trk_n,
				start_x[trk_n],
				start_y[trk_n],
				start_z[trk_n]);
			printf("trk_n:%d, end_x:%f, end_y:%f, end_z:%f\n",
				trk_n,
				end_x[trk_n],
				end_y[trk_n],
				end_z[trk_n]);
			pl_track->SetPoint(0, start_x[trk_n], start_y[trk_n], start_z[trk_n]);
			pl_track->SetPoint(1, end_x[trk_n], end_y[trk_n], end_z[trk_n]);
			pl_track->Draw();
		}
		// for (int phi = 0; phi <= 360; phi+=10)
		// {
		// 	c1->SetPhi(phi+3);
		// 	c1->Modified(); c1->Update();
		// 	sprintf(buffChar,"run%d_eventID%d_phi%03d.png", run, event_id, phi);
		// 	c1->SaveAs(buffChar);
		// }

		dig_canvas->Modified(); dig_canvas->Update();
		sprintf(buffChar,"run%d_eventID%04d.png", run, event_id);
		dig_canvas->SaveAs(buffChar);

		c1->cd();
		tracks->Draw("adc:direction.CosTheta()>>ADCvsCosTheta",individ_eventCut,"colz");
		c1->SetLogx(0);	c1->SetLogy(1);
		c1->Modified(); c1->Update();

		sprintf(buffChar,"run%d_eventID%04d_EnVsCosTheta.png", run, event_id);
		c1->SaveAs(buffChar);
		c1->SetLogy(0);

		tracks->Draw("length:adc>>LvsADC",individ_eventCut,"colz");
		c1->SetLogx(1);
		c1->Modified(); c1->Update();
		alphaCut_above_zCut_adc->Draw("same f");
		alphaCut_long_low_adc->Draw("same f");
		alphaCut_long_adc->Draw("same f");
		long_high_adc->Draw("same f");
		alphaCut_short_higher_adc->Draw("same f");
		alphaCut_short_low_adc->Draw("same f");
		fragCut_adc->Draw("same f");
		fragCut_unlikely_adc->Draw("same f");
		sprintf(buffChar,"run%d_eventID%04d_LVEn.png", run, event_id);
		c1->SaveAs(buffChar);
		c1->SetLogx(0);

		digits->Draw("dig_y:dig_x:dig_adc>>yx_digProjection", individ_eventCut, "colz");
		digits->Draw("dig_z:dig_y:dig_adc>>zy_digProjection", individ_eventCut, "colz");
		digits->Draw("dig_z:dig_x:dig_adc>>zx_digProjection", individ_eventCut, "colz");

		TCanvas *digProjection_canvas = new TCanvas("digProjection_canvas","digProjection_canvas",600,1400);
		digProjection_canvas->cd();
		digProjection_canvas->Clear();
		digProjection_canvas->Modified();
		digProjection_canvas->Update();
		digProjection_canvas->Divide(1,3,0,0);
		digProjection_canvas->cd(3);
		gPad->SetGrid();
		zx_digProjection->Draw("colz");
		for (int trk_n = 0; trk_n < trk_count; trk_n++)
		{
			TLine *line = new TLine(start_x[trk_n], start_z[trk_n],end_x[trk_n], end_z[trk_n]);
		  line->SetLineColor(kYellow);
			line->SetLineWidth(5);
		  line->Draw("same");
		}
		TText *t = new TText();
	  t->SetTextAngle(0);
		t->DrawTextNDC(0.85,0.13,"X [cm]");
		t->SetTextAngle(90);
		t->DrawTextNDC(0.15,0.80,"Z [cm]");
		digProjection_canvas->cd(2);
		gPad->SetGrid();
		zy_digProjection->Draw("colz");
		for (int trk_n = 0; trk_n < trk_count; trk_n++)
		{
			TLine *line = new TLine(start_y[trk_n], start_z[trk_n],end_y[trk_n], end_z[trk_n]);
		  line->SetLineColor(kYellow);
			line->SetLineWidth(5);
		  line->Draw("same");
		}
		t->SetTextAngle(0);
		t->DrawTextNDC(0.85,0.03,"Y [cm]");
		t->SetTextAngle(90);
		t->DrawTextNDC(0.15,0.80,"Z [cm]");
		digProjection_canvas->cd(1);
		gPad->SetGrid();
		yx_digProjection->Draw("colz");
		for (int trk_n = 0; trk_n < trk_count; trk_n++)
		{
			TLine *line = new TLine(start_x[trk_n], start_y[trk_n],end_x[trk_n], end_y[trk_n]);
		  line->SetLineColor(kYellow);
			line->SetLineWidth(5);
		  line->Draw("same");
		}
		t->SetTextAngle(0);
		t->DrawTextNDC(0.85,0.03,"X [cm]");
		t->SetTextAngle(90);
		t->DrawTextNDC(0.15,0.80,"Y [cm]");
		sprintf(buffChar,"2d_projections %s",individ_eventCut);
		digProjection_canvas->SetTitle(buffChar);
		digProjection_canvas->Modified();
		digProjection_canvas->Update();
		sprintf(buffChar,"run%d_eventID%04d_2d_digProjs.png",run,event_id);
		digProjection_canvas->SaveAs(buffChar);

		sprintf(buffChar,"gm montage -mode concatenate -tile 2x1 run%d_eventID%04d.png run%d_eventID%04d_2d_digProjs.png run%d_eventID%04d_digitTrack.png",run,event_id,run,event_id,run,event_id);
		system(buffChar);
		sprintf(buffChar,"rm run%d_eventID%04d_2d_digProjs.png run%d_eventID%04d.png",run,event_id,run,event_id);
		system(buffChar);
		sprintf(buffChar,"gm montage -mode concatenate -tile 1x2 run%d_eventID%04d_EnVsCosTheta.png run%d_eventID%04d_LVEn.png EnVsCosTheta_LVEn_tmp.png",run,event_id,run,event_id);
		system(buffChar);
		sprintf(buffChar,"gm montage -mode concatenate -tile 2x1 EnVsCosTheta_LVEn_tmp.png run%d_eventID%04d_digitTrack.png run%d_eventID%04d_montage.png",run,event_id,run,event_id);
		system(buffChar);
		sprintf(buffChar,"rm run%d_eventID%04d_EnVsCosTheta.png EnVsCosTheta_LVEn_tmp.png run%d_eventID%04d_LVEn.png run%d_eventID%04d_digitTrack.png",run,event_id,run,event_id,run,event_id);
		system(buffChar);

	}
}

LvsADC->Draw("colz");
LvsADC->GetXaxis()->SetTitle("energy [ADC]");
LvsADC->GetYaxis()->SetTitle("length [cm]");
alphaCut_above_zCut_adc->Draw("same f");
alphaCut_long_low_adc->Draw("same f");
alphaCut_long_adc->Draw("same f");
long_high_adc->Draw("same f");
alphaCut_short_higher_adc->Draw("same f");
alphaCut_short_low_adc->Draw("same f");
fragCut_adc->Draw("same f");
fragCut_unlikely_adc->Draw("same f");
c1->SetLogx(1);
c1->Modified(); c1->Update();
c1->SaveAs("LVEn.png");
c1->SetLogx(0);

ADCvsCosTheta->Draw("colz");
ADCvsCosTheta->GetYaxis()->SetTitle("energy [ADC]");
ADCvsCosTheta->GetXaxis()->SetTitle("cos #theta");
c1->Modified(); c1->Update();
c1->SaveAs("ADCvsCosTheta.png");
c1->SetLogx(0);

// TPolyLine3D *pl_hex = new TPolyLine3D(5);
// pl_hex->SetLineColor(kSpring);
// pl_hex->SetLineWidth(2);
// for (int n = 0; n <= 6; ++n)
// 	pl_hex->SetPoint(n,
// 		5*TMath::Cos(n*60*TMath::Pi()/180),
// 		5*TMath::Sin(n*60*TMath::Pi()/180),
// 		5);
// TPolyLine3D *pl_circle = new TPolyLine3D(5);
// pl_circle->SetLineColor(kRed);
// pl_circle->SetLineWidth(2);
// for (int n = 0; n <= 60; ++n)
// 	pl_circle->SetPoint(n,
// 		5*TMath::Cos(n*6*TMath::Pi()/180),
// 		5*TMath::Sin(n*6*TMath::Pi()/180),
// 		0);

// printf("drawing digits in noMaskLvADCAlphas cut \n");
// track3d->SetTitle(noMaskAlphaCut_above_zCut_adc_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskAlphaCut_above_zCut_adc_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d",noMaskAlphaCut_above_zCut_adc_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskLvADCAlphas.png");

// printf("drawing digits in noMaskLvADCFrags cut \n");
// track3d->SetTitle(noMaskFragCut);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskFragCut, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d",noMaskFragCut);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskFragCut.png");

// printf("drawing digits in noMaskFragUnlikelyCut_char cut \n");
// track3d->SetTitle(noMaskFragUnlikelyCut_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskFragUnlikelyCut_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d",noMaskFragUnlikelyCut_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskFragUnlikelyCut.png");

// printf("drawing digits in noMaskAlphaCut_long cut \n");
// track3d->SetTitle(noMaskAlphaCut_long_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskAlphaCut_long_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d", noMaskAlphaCut_long_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskAlphaCut_long.png");


// printf("drawing digits in noMaskAlphaCut_long_low cut \n");
// track3d->SetTitle(noMaskAlphaCut_long_low_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskAlphaCut_long_low_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d", noMaskAlphaCut_long_low_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskAlphaCut_long_low.png");

// printf("drawing digits in noMaskAlphaCut_long_high_adc cut \n");
// track3d->SetTitle(noMaskAlphaCut_long_high_adc_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskAlphaCut_long_high_adc_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d", noMaskAlphaCut_long_high_adc_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskAlphaCut_long_high_adc.png");

// printf("drawing digits in noMaskAlphaCut_short_higher_adc cut \n");
// track3d->SetTitle(noMaskAlphaCut_short_higher_adc_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskAlphaCut_short_higher_adc_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d", noMaskAlphaCut_short_higher_adc_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskAlphaCut_short_higher_adc.png");

// printf("drawing digits in noMaskAlphaCut_alphaCut_short_low_adc cut \n");
// track3d->SetTitle(noMaskAlphaCut_alphaCut_short_low_adc_char);
// track3d->Reset("M");
// if (!(track3d->TestBit(TH1::kNoStats)))
// {
// 	digits->Project("track3d", "dig_x:dig_y:dig_z", noMaskAlphaCut_alphaCut_short_low_adc_char, "");
// 	((TArrayF *)track3d)->Reset();
// }
// digits->Draw("dig_x:dig_y:dig_z:dig_adc>>+track3d", noMaskAlphaCut_alphaCut_short_low_adc_char);
// pl_hex->Draw("same");
// pl_circle->Draw("same");
// c1->SaveAs("noMaskAlphaCut_alphaCut_short_low_adc.png");

}
