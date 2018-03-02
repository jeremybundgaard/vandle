#include "TDirectory.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include <climits>
#include <vector>
#include "TROOT.h"
#include "TRint.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCut.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TAxis.h"
#include "TString.h"
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

#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>


void cftAnaPlots(const int preamp, const char* actinide, const char* pidFileName="", const char* noMaskFileName="", const char* cftFileName="", const char* otherTreeCuts="1",const char* driftVel="",const char* addGuide="") {

// gStyle->SetOptTitle(0);
// gStyle->SetOptStat(0);
// gStyle->SetPalette(57);
// gStyle->SetNumberContours(99);
// gStyle->SetTitleY(.98);
// gStyle->SetTitleFontSize(1.4*gStyle->GetTitleFontSize());
// gStyle->SetTitleXSize(1.3*gStyle->GetTitleXSize());
// gStyle->SetTitleYOffset(0.7);
// gStyle->SetTitleYSize(1.3*gStyle->GetTitleYSize());
// TGaxis::SetMaxDigits(3);

TCanvas* c1 = new TCanvas("c1","c1",1000,800);
c1->SetGrid(1);
c1->SetRightMargin(0.1);
c1->SetLeftMargin(0.08);
c1->SetBorderMode(-1);
c1->SetBorderSize(100);
c1->SetHighLightColor(kRed);

double MeV_per_ADC=0;
double ADC_per_MeV=0;
double dEnergy = 0.1;
double isoTopicCalib=0.8982;
double pi=4*atan(1);
double deg2rad = pi/(double)180;
char c1PlotName[234];
char buffChar[345];
char noMaskCut[234];
char cftCut[234];
char noMaskRunCut[234];
char cftRunCut[234];
char azCut[234];
char noMaskDigitFileName[234];
char cftDigitFileName[234];
char pidFile_char[345];

double alphaScaler_alphaCut_long_adc_region = 1.077;
double alphaScaler_alphaCut_long_low_adc_region = 0.213;
double alphaScaler_alphaCut_short_higher_adc_region = 0.888;
double alphaScaler_alphaCut_short_low_adc_region = 0.886;
double alphaScaler_alphaCut_zCut_adc_region = 1.000;
double alphaScaler_fragCut_adc_region = 0.000;
double alphaScaler_fragCut_unlikely_adc_region = 0.013;
double alphaScaler_long_high_adc_region = 0.708;

double fragScaler_alphaCut_long_adc_region = 0.00;
double fragScaler_alphaCut_long_low_adc_region = 0.78;
double fragScaler_alphaCut_short_higher_adc_region = 0.00;
double fragScaler_alphaCut_short_low_adc_region = 0.00;
double fragScaler_alphaCut_zCut_adc_region = 0.00;
double fragScaler_fragCut_adc_region = 1.00;
double fragScaler_fragCut_unlikely_adc_region = 0.00;
double fragScaler_long_high_adc_region = 0.33;

if (preamp==1){
	if (!(strcmp(actinide,"cf252"))){
		MeV_per_ADC=6.118/1180;
		sprintf(noMaskRunCut,"run>100001856");
	}
	if (!(strcmp(actinide,"cf244"))){
		ADC_per_MeV=1214/(double)5.800;
		sprintf(noMaskRunCut,"run=100001995");
	}
	sprintf(noMaskCut," %s",noMaskRunCut);
}
else if (preamp==2){
	if (!(strcmp(actinide,"cf252"))){
		MeV_per_ADC=6.118/1450;
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
		sprintf(noMaskCut,"%s",azCut);
		sprintf(cftCut,"%s &&!(600000>eventID&&eventID>450000)",azCut);
	}
	if ( !(strcmp(actinide,"cm244"))){
		sprintf(azCut,"direction.Phi()<2.2");
	}
}

////////////////////// initialize cuts from pid file ///////////////////////////////////
printf("////////////////////// initialize cuts from pid file:%s ///////////////////////////////////\n",pidFileName);
/////////  create TCutG chars ///////////
char fragCut_adc_char[345];
char junkCut_adc_char[345];
char alphaCut_adc_char[345];
char alpha_short_high_adc_char[345];
char alpha_short_low_adc_char[345];
char above_zCut_adc_char[345];
// sprintf(junkCut_adc_char,"low_adc");
sprintf(junkCut_adc_char,"junkCut_adc");
sprintf(fragCut_adc_char,"fragCut_adc");
sprintf(alphaCut_adc_char,"alphaCut_adc");
sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
sprintf(above_zCut_adc_char,"alphaCut_zCut_adc");
char fragCut_energy_char[345];
char junkCut_energy_char[345];
char alphaCut_energy_char[345];
char alpha_short_high_energy_char[345];
char alpha_short_low_energy_char[345];
char above_zCut_energy_char[345];
sprintf(junkCut_energy_char,"low_energy");
// sprintf(junkCut_energy_char,"junkCut_energy");
sprintf(fragCut_energy_char,"fragCut_energy");
sprintf(alphaCut_energy_char,"alphaCut_energy");
sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
sprintf(above_zCut_energy_char,"alphaCut_zCut_energy");
// fragCut_adc is crucial for invoking this cut in ->Draw()
TFile* pidFile = new TFile(pidFileName);
TCutG* fragCut_adc = (TCutG*)pidFile->Get(fragCut_adc_char);
fragCut_adc->SetName("fragCut_adc");
fragCut_adc->SetLineColor(kViolet);
fragCut_adc->SetLineWidth(3);
fragCut_adc->SetFillColorAlpha(kViolet,0.3);
TCutG* junkCut_adc = (TCutG*)pidFile->Get(junkCut_adc_char);
junkCut_adc->SetName("junkCut_adc");
junkCut_adc->SetLineColor(28);
junkCut_adc->SetLineWidth(1);
junkCut_adc->SetFillColorAlpha(28,0.3);
TCutG* alphaCut_above_zCut_adc = (TCutG*)pidFile->Get(above_zCut_adc_char);
alphaCut_above_zCut_adc->SetName("alphaCut_above_zCut_adc");
alphaCut_above_zCut_adc->SetLineColor(kSpring);
alphaCut_above_zCut_adc->SetLineWidth(3);
alphaCut_above_zCut_adc->SetFillColorAlpha(kSpring,0.3);
TCutG* alphaCut_long_low_adc = (TCutG*)pidFile->Get("alphaCut_long_low_adc");
alphaCut_long_low_adc->SetName("alphaCut_long_low_adc");
alphaCut_long_low_adc->SetLineColor(kYellow);
alphaCut_long_low_adc->SetLineWidth(1);
alphaCut_long_low_adc->SetFillColorAlpha(kYellow,0.3);
TCutG* alphaCut_long_adc = (TCutG*)pidFile->Get("alphaCut_long_adc");
alphaCut_long_adc->SetName("alphaCut_long_adc");
alphaCut_long_adc->SetLineColor(kCyan);
alphaCut_long_adc->SetLineWidth(1);
alphaCut_long_adc->SetFillColorAlpha(kCyan,0.3);
TCutG* long_high_adc = (TCutG*)pidFile->Get("long_high_adc");
long_high_adc->SetName("long_high_adc");
long_high_adc->SetLineColor(kBlue);
long_high_adc->SetLineWidth(1);
long_high_adc->SetFillColorAlpha(kBlue,0.3);
TCutG* alphaCut_short_higher_adc = (TCutG*)pidFile->Get(alpha_short_high_adc_char);
alphaCut_short_higher_adc->SetName("alphaCut_short_higher_adc");
alphaCut_short_higher_adc->SetLineColor(kOrange+7);
alphaCut_short_higher_adc->SetLineWidth(1);
alphaCut_short_higher_adc->SetFillColorAlpha(kOrange+7,0.3);
TCutG* alphaCut_short_low_adc = (TCutG*)pidFile->Get(alpha_short_low_adc_char);
alphaCut_short_low_adc->SetName("alphaCut_short_low_adc");
alphaCut_short_low_adc->SetLineColor(kRed);
alphaCut_short_low_adc->SetLineWidth(1);
alphaCut_short_low_adc->SetFillColorAlpha(kRed,0.3);
// fragCut_energy is crucial for invoking this cut in ->Draw()
TCutG* fragCut_energy = (TCutG*)pidFile->Get(fragCut_energy_char);
fragCut_energy->SetName("fragCut_energy");
fragCut_energy->SetLineColor(kViolet);
fragCut_energy->SetLineWidth(3);
fragCut_energy->SetFillColorAlpha(kViolet,0.3);
TCutG* junkCut_energy = (TCutG*)pidFile->Get(junkCut_energy_char);
junkCut_energy->SetName("junkCut_energy");
// junkCut_energy->SetName("low_energy");
junkCut_energy->SetLineColor(28);
junkCut_energy->SetLineWidth(1);
junkCut_energy->SetFillColorAlpha(28,0.3);
TCutG* alphaCut_above_zCut_energy = (TCutG*)pidFile->Get(above_zCut_energy_char);
alphaCut_above_zCut_energy->SetName("above_zCut_energy");
alphaCut_above_zCut_energy->SetLineColor(kSpring);
alphaCut_above_zCut_energy->SetLineWidth(3);
alphaCut_above_zCut_energy->SetFillColorAlpha(kSpring,0.3);
TCutG* alphaCut_long_low_energy = (TCutG*)pidFile->Get("alphaCut_long_low_energy");
alphaCut_long_low_energy->SetName("alphaCut_long_low_energy");
alphaCut_long_low_energy->SetLineColor(kYellow);
alphaCut_long_low_energy->SetLineWidth(1);
alphaCut_long_low_energy->SetFillColorAlpha(kYellow,0.3);
TCutG* alphaCut_long_energy = (TCutG*)pidFile->Get("alphaCut_long_energy");
alphaCut_long_energy->SetName("alphaCut_long_energy");
alphaCut_long_energy->SetLineColor(kCyan);
alphaCut_long_energy->SetLineWidth(1);
alphaCut_long_energy->SetFillColorAlpha(kCyan,0.3);
TCutG* long_high_energy = (TCutG*)pidFile->Get("long_high_energy");
long_high_energy->SetName("long_high_energy");
long_high_energy->SetLineColor(kBlue);
long_high_energy->SetLineWidth(1);
long_high_energy->SetFillColorAlpha(kBlue,0.3);
TCutG* alphaCut_short_higher_energy = (TCutG*)pidFile->Get(alpha_short_high_energy_char);
alphaCut_short_higher_energy->SetName("alphaCut_short_higher_energy");
alphaCut_short_higher_energy->SetLineColor(kOrange+7);
alphaCut_short_higher_energy->SetLineWidth(1);
alphaCut_short_higher_energy->SetFillColorAlpha(kOrange+7,0.3);
TCutG* alphaCut_short_low_energy = (TCutG*)pidFile->Get(alpha_short_low_energy_char);
alphaCut_short_low_energy->SetName("alphaCut_short_low_energy");
alphaCut_short_low_energy->SetLineColor(kRed);
alphaCut_short_low_energy->SetLineWidth(1);
alphaCut_short_low_energy->SetFillColorAlpha(kRed,0.3);
char noMaskFragCut[234];sprintf(noMaskFragCut,"fragCut_adc");
printf("noMaskFragCut: %s\n",noMaskFragCut);
char noMaskFragUnlikelyCut_char[234];sprintf(noMaskFragUnlikelyCut_char,"junkCut_adc");
printf("noMaskFragUnlikelyCut_char: %s\n",noMaskFragUnlikelyCut_char);
char noMaskAlphaCut_above_zCut_adc_char[234];sprintf(noMaskAlphaCut_above_zCut_adc_char,"alphaCut_above_zCut_adc");
char noMaskAlphaCut_long_char[234];sprintf(noMaskAlphaCut_long_char,"alphaCut_long_adc");
char noMaskAlphaCut_long_low_char[234];sprintf(noMaskAlphaCut_long_low_char,"alphaCut_long_low_adc");
char noMaskAlphaCut_long_high_adc_char[234];sprintf(noMaskAlphaCut_long_high_adc_char,"long_high_adc");
char noMaskAlphaCut_short_higher_adc_char[234];sprintf(noMaskAlphaCut_short_higher_adc_char,"alphaCut_short_higher_adc");
char noMaskAlphaCut_alphaCut_short_low_adc_char[234];sprintf(noMaskAlphaCut_alphaCut_short_low_adc_char,"alphaCut_short_low_adc");
char cftFragCut[234];sprintf(cftFragCut,"fragCut_adc");
char cftFragUnlikelyCut_char[234];sprintf(cftFragUnlikelyCut_char,"junkCut_adc");
char cftAlphaCut_above_zCut_adc_char[234];sprintf(cftAlphaCut_above_zCut_adc_char,"alphaCut_above_zCut_adc");
char cftAlphaCut_long_char[234];sprintf(cftAlphaCut_long_char,"alphaCut_long_adc");
char cftAlphaCut_long_low_char[234];sprintf(cftAlphaCut_long_low_char,"alphaCut_long_low_adc");
char cftAlphaCut_long_high_adc_char[234];sprintf(cftAlphaCut_long_high_adc_char,"long_high_adc");
char cftAlphaCut_short_higher_adc_char[234];sprintf(cftAlphaCut_short_higher_adc_char,"alphaCut_short_higher_adc");
char cftAlphaCut_alphaCut_short_low_adc_char[234];sprintf(cftAlphaCut_alphaCut_short_low_adc_char,"alphaCut_short_low_adc");

// ////////////////////// draw noMask digit/tracks trees ///////////////////////////////////
// printf("////////////////////// draw noMask digit/tracks trees ///////////////////////////////////\n");
// sprintf(noMaskDigitFileName,"/p/lscratche/jerbundg/Data/thesisData/trees/digTrkTreeDir/noMask%d.driftVel%s_trk_dig.2k.trees.root",preamp,driftVel);
// TFile* noMaskDigitFile = new TFile(noMaskDigitFileName);
// TTree* noMaskDigits = (TTree*)noMaskDigitFile->Get("tracks");
// printf("drawing digits in noMaskLvADC cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskCut);
// c1->SaveAs("noMaskLvADC.png");
// c1->SaveAs("noMaskLvADC.C");
// printf("drawing digits in noMaskLvADCAlphas cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskAlphaCut_above_zCut_adc_char);
// c1->SaveAs("noMaskLvADCAlphas.png");
// c1->SaveAs("noMaskLvADCAlphas.C");
// printf("drawing digits in noMaskLvADCFrags cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskFragCut);
// c1->SaveAs("noMaskLvADCFrags.png");
// c1->SaveAs("noMaskLvADCFrags.C");
// printf("drawing digits in noMaskLvADCFragsUnlikely cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskFragUnlikelyCut_char);
// c1->SaveAs("noMaskLvADCFragsUnlikely.png");
// c1->SaveAs("noMaskLvADCFragsUnlikely.C");
// printf("drawing digits in noMaskLvADCAlphas_long cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskAlphaCut_long_char);
// c1->SaveAs("noMaskLvADCAlphas_long.png");
// c1->SaveAs("noMaskLvADCAlphas_long.C");
// printf("drawing digits in noMaskLvADCAlphas_long_low cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskAlphaCut_long_low_char);
// c1->SaveAs("noMaskLvADCAlphas_long_low.png");
// c1->SaveAs("noMaskLvADCAlphas_long_low.C");
// printf("drawing digits in noMaskLvADCAlphas_long_high cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskAlphaCut_long_high_adc_char);
// c1->SaveAs("noMaskLvADCAlphas_long_high.png");
// c1->SaveAs("noMaskLvADCAlphas_long_high.C");
// printf("drawing digits in noMaskLvADCAlphas_short_higher cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskAlphaCut_short_higher_adc_char);
// c1->SaveAs("noMaskLvADCAlphas_short_higher.png");
// c1->SaveAs("noMaskLvADCAlphas_short_higher.C");
// printf("drawing digits in noMaskLvADCAlphas_alphaCut_short_low cut \n");
// noMaskDigits->Draw("dig_x:dig_y:dig_z:dig_adc",noMaskAlphaCut_alphaCut_short_low_adc_char);
// c1->SaveAs("noMaskLvADCAlphas_alphaCut_short_low.png");
// c1->SaveAs("noMaskLvADCAlphas_alphaCut_short_low.C");

printf("noMaskCut = %s\n",noMaskCut);
printf("cftCut = %s\n",cftCut);
////////////////////// get track trees ///////////////////////////////////
printf("////////////////////// get track trees ///////////////////////////////////\n");
TFile* cftFile = new TFile(cftFileName);
TTree* cftTracks_orig = (TTree*)cftFile->Get("tracks");
TFile* noMaskFile = new TFile(noMaskFileName);
TTree* noMaskTracks_orig = (TTree*)noMaskFile->Get("tracks");

printf("\n/////////////////////// unCut plots ////////////////////\n\n");
/////////////////////// unCut plots ////////////////////
/////////////////////// length vs energy ////////////////////
TH2D* noMaskLvEn_noCuts = new TH2D("noMaskLvEn_noCuts","noMaskLvEn_noCuts",4000,0,40000*MeV_per_ADC,500,0,10);
noMaskLvEn_noCuts->SetTitle("noMask Data with dead EtherDAQ");
noMaskLvEn_noCuts->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_noCuts->GetYaxis()->CenterTitle();
noMaskLvEn_noCuts->GetYaxis()->SetRangeUser(0,6);
noMaskLvEn_noCuts->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_noCuts->GetXaxis()->CenterTitle();
noMaskLvEn_noCuts->GetXaxis()->SetRangeUser(0.4,125);
sprintf(buffChar,"length:adc*%f>>noMaskLvEn_noCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar);
c1->SetLogx(1);c1->SetLogy(0);c1->SetLogz(1);
noMaskLvEn_noCuts->Draw("colz");
c1->SaveAs("noMaskLvEn_noCuts.png");
/////////////////////// length vs cosTheta ////////////////////
TH2D* Length_vs_cosTheta_noCuts = new TH2D("Length_vs_cosTheta_noCuts","Length_vs_cosTheta_noCuts",500,-1,1,500,0,10);
Length_vs_cosTheta_noCuts->SetTitle("noMask Data with dead EtherDAQ");
Length_vs_cosTheta_noCuts->GetYaxis()->SetRangeUser(0,6);
Length_vs_cosTheta_noCuts->GetYaxis()->SetTitle("Track Length [cm]");
Length_vs_cosTheta_noCuts->GetYaxis()->CenterTitle();
Length_vs_cosTheta_noCuts->SetTitle("");
Length_vs_cosTheta_noCuts->GetXaxis()->SetTitle("Track cos(#theta)");
Length_vs_cosTheta_noCuts->GetXaxis()->CenterTitle();
sprintf(buffChar,"length:direction.CosTheta()>>Length_vs_cosTheta_noCuts");
noMaskTracks_orig->Draw(buffChar);
c1->SetLogx(0);c1->SetLogy(0);
Length_vs_cosTheta_noCuts->Draw("colz");
c1->SaveAs("Length_vs_cosTheta_noCuts.png");
/////////////////////// energy vs cos theta ////////////////////
TH2D* Energy_vs_cosTheta_noCuts = new TH2D("Energy_vs_cosTheta_noCuts","Energy_vs_cosTheta_noCuts",500,-1,1,4000,0,40000*MeV_per_ADC);
Energy_vs_cosTheta_noCuts->SetTitle("");
Energy_vs_cosTheta_noCuts->GetXaxis()->SetTitle("Track cos(#theta)");
Energy_vs_cosTheta_noCuts->GetXaxis()->CenterTitle();
Energy_vs_cosTheta_noCuts->GetYaxis()->SetRangeUser(0.4,125);
Energy_vs_cosTheta_noCuts->GetYaxis()->SetTitle("Track Energy [MeV]");
Energy_vs_cosTheta_noCuts->GetYaxis()->CenterTitle();
sprintf(buffChar,"adc*%f:direction.CosTheta()>>Energy_vs_cosTheta_noCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar);
c1->SetLogx(0);c1->SetLogy(1);
Energy_vs_cosTheta_noCuts->Draw("colz");
c1->SaveAs("Energy_vs_cosTheta_noCuts.png");
/////////////////////// energy vs azimuth ////////////////////
TH2D* Energy_vs_Azimuth_noCuts = new TH2D("Energy_vs_Azimuth_noCuts","Energy_vs_Azimuth_noCuts",500,-3.15,3.15,4000,0,40000*MeV_per_ADC);
Energy_vs_Azimuth_noCuts->SetTitle("");
Energy_vs_Azimuth_noCuts->GetXaxis()->SetTitle("Track Azimuth [radians]");
Energy_vs_Azimuth_noCuts->GetXaxis()->CenterTitle();
Energy_vs_Azimuth_noCuts->GetYaxis()->SetTitle("Track Energy [MeV]");
Energy_vs_Azimuth_noCuts->GetYaxis()->CenterTitle();
Energy_vs_Azimuth_noCuts->GetYaxis()->SetRangeUser(0.4,125);
sprintf(buffChar,"adc*%f:direction.Phi()>>Energy_vs_Azimuth_noCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar);
c1->SetLogx(0);c1->SetLogy(1);
Energy_vs_Azimuth_noCuts->Draw("colz");
c1->SaveAs("Energy_vs_Azimuth_noCuts.png");
c1->SetLogx(0);c1->SetLogy(0);

printf("\n/////////////////////// Cut plots ////////////////////\n\n");
/////////////////////// Cut plots ////////////////////
////////////////////// energy vs length ////////////////////
TH2D* noMaskLvEn_wCuts = new TH2D("noMaskLvEn_wCuts","noMaskLvEn_wCuts",4000,0,40000*MeV_per_ADC,500,0,10);
noMaskLvEn_wCuts->SetTitle("aziumth cut removes dead card effects");
noMaskLvEn_wCuts->GetYaxis()->SetRangeUser(0,6);
noMaskLvEn_wCuts->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_wCuts->GetYaxis()->CenterTitle();
noMaskLvEn_wCuts->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_wCuts->GetXaxis()->CenterTitle();
noMaskLvEn_wCuts->GetXaxis()->SetRangeUser(0.4,125);
sprintf(buffChar,"length:adc*%f>>noMaskLvEn_wCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar,noMaskCut);
c1->SetLogx(1);c1->SetLogy(0);
noMaskLvEn_wCuts->Draw("colz");
c1->SaveAs("noMaskLvEn_wCuts.png");
/////////////////////// length vs cosTheta ////////////////////
TH2D* Length_vs_cosTheta_wCuts = new TH2D("Length_vs_cosTheta_wCuts","Length_vs_cosTheta_wCuts",500,-1,1,500,0,10);
Length_vs_cosTheta_wCuts->SetTitle("noMask Data with dead EtherDAQ");
Length_vs_cosTheta_wCuts->GetYaxis()->SetRangeUser(0,6);
Length_vs_cosTheta_wCuts->GetYaxis()->SetTitle("Track Length [cm]");
Length_vs_cosTheta_wCuts->GetYaxis()->CenterTitle();
Length_vs_cosTheta_wCuts->SetTitle("");
Length_vs_cosTheta_wCuts->GetXaxis()->SetTitle("Track cos(#theta)");
Length_vs_cosTheta_wCuts->GetXaxis()->CenterTitle();
sprintf(buffChar,"length:direction.CosTheta()>>Length_vs_cosTheta_wCuts");
noMaskTracks_orig->Draw(buffChar,noMaskCut);
c1->SetLogx(0);c1->SetLogy(0);
Length_vs_cosTheta_wCuts->Draw("colz");
c1->SaveAs("Length_vs_cosTheta_wCuts.png");
/////////////////////// energy vs cos theta ////////////////////
TH2D* Energy_vs_cosTheta_wCuts = new TH2D("Energy_vs_cosTheta_wCuts","Energy_vs_cosTheta_wCuts",500,-1,1,4000,0,40000*MeV_per_ADC);
Energy_vs_cosTheta_wCuts->SetTitle("");
Energy_vs_cosTheta_wCuts->GetXaxis()->SetTitle("Track cos(#theta)");
Energy_vs_cosTheta_wCuts->GetXaxis()->CenterTitle();
Energy_vs_cosTheta_wCuts->GetYaxis()->SetRangeUser(0.4,125);
Energy_vs_cosTheta_wCuts->GetYaxis()->SetTitle("Track Energy [MeV]");
Energy_vs_cosTheta_wCuts->GetYaxis()->CenterTitle();
sprintf(buffChar,"adc*%f:direction.CosTheta()>>Energy_vs_cosTheta_wCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar,noMaskCut);
c1->SetLogx(0);c1->SetLogy(1);
Energy_vs_cosTheta_wCuts->Draw("colz");
c1->SaveAs("Energy_vs_cosTheta_wCuts.png");
/////////////////////// energy vs azimuth ////////////////////
TH2D* Energy_vs_Azimuth_wCuts = new TH2D("Energy_vs_Azimuth_wCuts","Energy_vs_Azimuth_wCuts",250,-3.15,3.15,4000,0,40000*MeV_per_ADC);
Energy_vs_Azimuth_wCuts->SetTitle("");
Energy_vs_Azimuth_wCuts->GetXaxis()->SetTitle("Track Azimuth [radians]");
Energy_vs_Azimuth_wCuts->GetXaxis()->CenterTitle();
Energy_vs_Azimuth_wCuts->GetYaxis()->SetTitle("Track Energy [MeV]");
Energy_vs_Azimuth_wCuts->GetYaxis()->CenterTitle();
Energy_vs_Azimuth_wCuts->GetYaxis()->SetRangeUser(0.4,125);
sprintf(buffChar,"adc*%f:direction.Phi()>>Energy_vs_Azimuth_wCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar,noMaskCut);
c1->SetLogx(0);c1->SetLogy(1);
Energy_vs_Azimuth_wCuts->Draw("colz");
c1->SaveAs("Energy_vs_Azimuth_wCuts.png");
c1->SetLogx(0);c1->SetLogy(0);
// return;

////////////////////// create new TTree's with noMask and CFT cuts ///////////////////////////////////
printf("////////////////////// create new TTree's with noMask and CFT cuts ///////////////////////////////////\n");
TFile* dummy = new TFile("deleteme.root","create");
dummy->cd();
TTree* cftTracks = (TTree*)cftTracks_orig->CopyTree(cftCut);
TTree* noMaskTracks = (TTree*)noMaskTracks_orig->CopyTree(noMaskCut);

////////////////////// create PID bin histograms ///////////////////////////////////
printf("////////////////////// create PID bin histograms ///////////////////////////////////\n");
TH2D* noMaskLvADC = new TH2D("noMaskLvADC","noMaskLvADC",4000,0,40000,1000,0,10);
noMaskTracks->Draw("length:adc>>noMaskLvADC",noMaskCut);

TH2D* noMaskLvADCFrags = (TH2D*)noMaskLvADC->Clone("noMaskLvADCFrags");
TH2D* noMaskLvADCFragsUnlikely = (TH2D*)noMaskLvADC->Clone("noMaskLvADCFragsUnlikely");
TH2D* noMaskLvADCAlphas = (TH2D*)noMaskLvADC->Clone("noMaskLvADCAlphas");
TH2D* noMaskLvADCAlphas_long = (TH2D*)noMaskLvADC->Clone("noMaskLvADCAlphas_long");
TH2D* noMaskLvADCAlphas_long_low = (TH2D*)noMaskLvADC->Clone("noMaskLvADCAlphas_long_low");
TH2D* noMaskLvADCAlphas_long_high = (TH2D*)noMaskLvADC->Clone("noMaskLvADCAlphas_long_high");
TH2D* noMaskLvADCAlphas_short_higher = (TH2D*)noMaskLvADC->Clone("noMaskLvADCAlphas_short_higher");
TH2D* noMaskLvADCAlphas_alphaCut_short_low = (TH2D*)noMaskLvADC->Clone("noMaskLvADCAlphas_alphaCut_short_low");
TH2D* noMaskLvADC_checkMutualExclusivity = (TH2D*)noMaskLvADC->Clone("noMaskLvADC_checkMutualExclusivity");


TH2D* cftLvADC = new TH2D("cftLvADC","cftLvADC",4000,0,40000,1000,0,10);
TH2D* cftLvADCFrags = (TH2D*)cftLvADC->Clone("cftLvADCFrags");
TH2D* cftLvADCFragsUnlikely = (TH2D*)cftLvADC->Clone("cftLvADCFragsUnlikely");
TH2D* cftLvADCAlphas = (TH2D*)cftLvADC->Clone("cftLvADCAlphas");
TH2D* cftLvADCAlphas_long = (TH2D*)cftLvADC->Clone("cftLvADCAlphas_long");
TH2D* cftLvADCAlphas_long_low = (TH2D*)cftLvADC->Clone("cftLvADCAlphas_long_low");
TH2D* cftLvADCAlphas_long_high = (TH2D*)cftLvADC->Clone("cftLvADCAlphas_long_high");
TH2D* cftLvADCAlphas_short_higher = (TH2D*)cftLvADC->Clone("cftLvADCAlphas_short_higher");
TH2D* cftLvADCAlphas_alphaCut_short_low = (TH2D*)cftLvADC->Clone("cftLvADCAlphas_alphaCut_short_low");

////////////////////// fill noMask PID histograms ///////////////////////////////////
printf("////////////////////// fill noMask PID histograms ///////////////////////////////////\n");
char noMask_mutualExclusivityCheckChar[567];
sprintf(noMask_mutualExclusivityCheckChar,"%s||%s||%s||%s||%s||%s||%s||%s",
noMaskAlphaCut_above_zCut_adc_char,
noMaskFragCut,
noMaskFragUnlikelyCut_char,
noMaskAlphaCut_long_char,
noMaskAlphaCut_long_low_char,
noMaskAlphaCut_long_high_adc_char,
noMaskAlphaCut_short_higher_adc_char,
noMaskAlphaCut_alphaCut_short_low_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADC_checkMutualExclusivity",noMask_mutualExclusivityCheckChar);
// noMaskTracks->Draw("length:adc>>noMaskLvADC",noMaskCut);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas",noMaskAlphaCut_above_zCut_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCFrags",noMaskFragCut);
noMaskTracks->Draw("length:adc>>noMaskLvADCFragsUnlikely",noMaskFragUnlikelyCut_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_long",noMaskAlphaCut_long_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_long_low",noMaskAlphaCut_long_low_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_long_high",noMaskAlphaCut_long_high_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_short_higher",noMaskAlphaCut_short_higher_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_alphaCut_short_low",noMaskAlphaCut_alphaCut_short_low_adc_char);

double noMaskLvADC_checkMutualExclusivity_integral = noMaskLvADC_checkMutualExclusivity->Integral();
double noMaskLvADC_integral = noMaskLvADC->Integral();
double noMaskLvADCAlphas_integral = noMaskLvADCAlphas->Integral();
double noMaskLvADCFrags_integral = noMaskLvADCFrags->Integral();
double noMaskLvADCFragsUnlikely_integral = noMaskLvADCFragsUnlikely->Integral();
double noMaskLvADCAlphas_long_integral = noMaskLvADCAlphas_long->Integral();
double noMaskLvADCAlphas_long_low_integral = noMaskLvADCAlphas_long_low->Integral();
double noMaskLvADCAlphas_long_high_integral = noMaskLvADCAlphas_long_high->Integral();
double noMaskLvADCAlphas_short_higher_integral = noMaskLvADCAlphas_short_higher->Integral();
double noMaskLvADCAlphas_alphaCut_short_low_integral = noMaskLvADCAlphas_alphaCut_short_low->Integral();
printf("noMask_mutualExclusivityCheckChar:%s\n",noMask_mutualExclusivityCheckChar);
printf("filled %f noMaskLvADC_checkMutualExclusivity from track tree\n",noMaskLvADC_checkMutualExclusivity_integral);
printf("filled %f noMaskLvADC from track tree\n",noMaskLvADC_integral);
printf("filled %f noMaskLvADCAlphas from track tree\n",noMaskLvADCAlphas_integral);
printf("filled %f noMaskLvADCFrags from track tree\n",noMaskLvADCFrags_integral);
printf("filled %f noMaskLvADCFragsUnlikely from track tree\n",noMaskLvADCFragsUnlikely_integral);
printf("filled %f noMaskLvADCAlphas_long from track tree\n",noMaskLvADCAlphas_long_integral);
printf("filled %f noMaskLvADCAlphas_long_low from track tree\n",noMaskLvADCAlphas_long_low_integral);
printf("filled %f noMaskLvADCAlphas_long_high from track tree\n",noMaskLvADCAlphas_long_high_integral);
printf("filled %f noMaskLvADCAlphas_short_higher from track tree\n",noMaskLvADCAlphas_short_higher_integral);////////////////////// noMask create energy projection of color coded length-energy containers ///////////////
printf("////////////////////// noMask create energy projection of color coded length-energy containers ///////////////\n");
TH1D* noMaskADC = noMaskLvADC->ProjectionX("noMaskADC");
noMaskADC->SetTitle("");
TH1D* noMaskFragsADC = noMaskLvADCFrags->ProjectionX("noMaskFragsADC");
noMaskFragsADC->SetTitle("");
TH1D* noMaskFragsUnlikeyADC = noMaskLvADCFrags->ProjectionX("noMaskFragsUnlikeyADC");
noMaskFragsUnlikeyADC->SetTitle("");
TH1D* noMaskAlphasADC = noMaskLvADCAlphas->ProjectionX("noMaskAlphasADC");
noMaskAlphasADC->SetTitle("");
TH1D* noMaskAlphasADC_long = noMaskLvADCAlphas_long->ProjectionX("noMaskAlphasADC_long");
noMaskAlphasADC_long->SetTitle("");
TH1D* noMaskAlphasADC_long_low = noMaskLvADCAlphas_long_low->ProjectionX("noMaskAlphasADC_long_low");
noMaskAlphasADC_long_low->SetTitle("");
TH1D* noMaskAlphasADC_long_high = noMaskLvADCAlphas_long_high->ProjectionX("noMaskAlphasADC_long_high");
noMaskAlphasADC_long_high->SetTitle("");
TH1D* noMaskAlphasADC_short_higher = noMaskLvADCAlphas_short_higher->ProjectionX("noMaskAlphasADC_short_higher");
noMaskAlphasADC_short_higher->SetTitle("");
TH1D* noMaskAlphasADC_alphaCut_short_low = noMaskLvADCAlphas_alphaCut_short_low->ProjectionX("noMaskAlphasADC_alphaCut_short_low");
noMaskAlphasADC_alphaCut_short_low->SetTitle("");
////////////////////// verify PID bins are mutually exclusive ///////////////////////////////////
printf("////////////////////// verify PID bins are mutually exclusive ///////////////////////////////////\n");
printf("filled %f noMaskLvADCAlphas_alphaCut_short_low from track tree\n",noMaskLvADCAlphas_alphaCut_short_low_integral);
printf("noMaskLvADC_integral - all TCutG integrals :%f\n",
noMaskLvADC_integral
-noMaskLvADCAlphas_integral
-noMaskLvADCFrags_integral
-noMaskLvADCFragsUnlikely_integral
-noMaskLvADCAlphas_long_integral
-noMaskLvADCAlphas_long_low_integral
-noMaskLvADCAlphas_long_high_integral
-noMaskLvADCAlphas_short_higher_integral
-noMaskLvADCAlphas_alphaCut_short_low_integral
);
printf("noMaskLvADC_integral - noMaskLvADC_checkMutualExclusivity_integral :%f\n",
noMaskLvADC_integral - noMaskLvADC_checkMutualExclusivity_integral);

////////////////////// fill cft PID histograms ///////////////////////////////////
printf("////////////////////// fill cft PID histograms ///////////////////////////////////\n");
cftTracks->Draw("length:adc>>cftLvADC",cftCut);
cftTracks->Draw("length:adc>>cftLvADCAlphas",cftAlphaCut_above_zCut_adc_char);
cftTracks->Draw("length:adc>>cftLvADCFrags",cftFragCut);
cftTracks->Draw("length:adc>>cftLvADCFragsUnlikely",cftFragUnlikelyCut_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_long",cftAlphaCut_long_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);
printf("////////////////////// cft create ADC projection of color coded length-ADC containers ///////////////\n");
TH1D* cftADC = cftLvADC->ProjectionX("cftADC");
cftADC->SetTitle("");
TH1D* cftFragsADC = cftLvADCFrags->ProjectionX("cftFragsADC");
cftFragsADC->SetTitle("");
TH1D* cftFragsUnlikeyADC = cftLvADCFrags->ProjectionX("cftFragsUnlikeyADC");
cftFragsUnlikeyADC->SetTitle("");
TH1D* cftAlphasADC = cftLvADCAlphas->ProjectionX("cftAlphasADC");
cftAlphasADC->SetTitle("");
TH1D* cftAlphasADC_long = cftLvADCAlphas_long->ProjectionX("cftAlphasADC_long");
cftAlphasADC_long->SetTitle("");
TH1D* cftAlphasADC_long_low = cftLvADCAlphas_long_low->ProjectionX("cftAlphasADC_long_low");
cftAlphasADC_long_low->SetTitle("");
TH1D* cftAlphasADC_long_high = cftLvADCAlphas_long_high->ProjectionX("cftAlphasADC_long_high");
cftAlphasADC_long_high->SetTitle("");
TH1D* cftAlphasADC_short_higher = cftLvADCAlphas_short_higher->ProjectionX("cftAlphasADC_short_higher");
cftAlphasADC_short_higher->SetTitle("");
TH1D* cftAlphasADC_alphaCut_short_low = cftLvADCAlphas_alphaCut_short_low->ProjectionX("cftAlphasADC_alphaCut_short_low");
cftAlphasADC_alphaCut_short_low->SetTitle("");
int cftLvADC_integral = cftLvADC->Integral();
int cftLvADCAlphas_integral = cftLvADCAlphas->Integral();
int cftLvADCFrags_integral = cftLvADCFrags->Integral();
int cftLvADCFragsUnlikely_integral = cftLvADCFragsUnlikely->Integral();
int cftLvADCAlphas_long_integral = cftLvADCAlphas_long->Integral();
int cftLvADCAlphas_long_low_integral = cftLvADCAlphas_long_low->Integral();
int cftLvADCAlphas_long_high_integral = cftLvADCAlphas_long_high->Integral();
int cftLvADCAlphas_short_higher_integral = cftLvADCAlphas_short_higher->Integral();
int cftLvADCAlphas_alphaCut_short_low_integral = cftLvADCAlphas_alphaCut_short_low->Integral();
printf("cftLvADC_integral: %d \n",cftLvADC_integral);
printf("cftLvADCAlphas_integral: %d \n",cftLvADCAlphas_integral);
printf("cftLvADCFrags_integral: %d \n",cftLvADCFrags_integral);
printf("cftLvADCFragsUnlikely_integral: %d \n",cftLvADCFragsUnlikely_integral);
printf("cftLvADCAlphas_long_integral: %d \n",cftLvADCAlphas_long_integral);
printf("cftLvADCAlphas_long_low_integral: %d \n",cftLvADCAlphas_long_low_integral);
printf("cftLvADCAlphas_long_high_integral: %d \n",cftLvADCAlphas_long_high_integral);
printf("cftLvADCAlphas_short_higher_integral: %d \n",cftLvADCAlphas_short_higher_integral);
printf("cftLvADCAlphas_alphaCut_short_low_integral: %d \n",cftLvADCAlphas_alphaCut_short_low_integral);

////////////////////// create noMaskLvEn  ///////////////////////////////////
printf("////////////////////// create noMaskLvEn  ///////////////////////////////////\n");
TH2D* noMaskLvEn = new TH2D("noMaskLvEn","noMaskLvEn",4000,0,40000*MeV_per_ADC,1000,0,10);
noMaskLvEn->SetTitle("");
noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn->GetXaxis()->CenterTitle();
noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn->GetYaxis()->CenterTitle();
TH2D* noMaskLvEnFrags = (TH2D*)noMaskLvEn->Clone("noMaskLvEnFrags");
noMaskLvEnFrags->SetTitle("");
noMaskLvEnFrags->SetTitle("noMaskLvEnFrags");
TH2D* noMaskLvEnFragsUnlikely = (TH2D*)noMaskLvEn->Clone("noMaskLvEnFragsUnlikely");
noMaskLvEnFragsUnlikely->SetTitle("");
noMaskLvEnFragsUnlikely->SetTitle("noMaskLvEnFragsUnlikely");
TH2D* noMaskLvEnAlphas = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas");
noMaskLvEnAlphas->SetTitle("");
noMaskLvEnAlphas->SetTitle("noMaskLvEnAlphas");
TH2D* noMaskLvEnAlphas_long = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_long");
noMaskLvEnAlphas_long->SetTitle("");
noMaskLvEnAlphas_long->SetTitle("noMaskLvEnAlphas_long");
TH2D* noMaskLvEnAlphas_long_low = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_long_low");
noMaskLvEnAlphas_long_low->SetTitle("");
noMaskLvEnAlphas_long_low->SetTitle("noMaskLvEnAlphas_long_low");
TH2D* noMaskLvEnAlphas_long_high = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_long_high");
noMaskLvEnAlphas_long_high->SetTitle("");
noMaskLvEnAlphas_long_high->SetTitle("noMaskLvEnAlphas_long_high");
TH2D* noMaskLvEnAlphas_short_higher = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_short_higher");
noMaskLvEnAlphas_short_higher->SetTitle("");
noMaskLvEnAlphas_short_higher->SetTitle("noMaskLvEnAlphas_short_higher");
TH2D* noMaskLvEnAlphas_alphaCut_short_low = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_alphaCut_short_low");
noMaskLvEnAlphas_alphaCut_short_low->SetTitle("");
noMaskLvEnAlphas_alphaCut_short_low->SetTitle("noMaskLvEnAlphas_alphaCut_short_low");
////////////////////// create cftLvEn  ///////////////////////////////////
printf("////////////////////// create cftLvEn  ///////////////////////////////////\n");
TH2D* cftLvEn = new TH2D("cftLvEn","cftLvEn",4000,0,40000*MeV_per_ADC,1000,0,10);
cftLvEn->SetTitle("");
cftLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftLvEn->GetXaxis()->CenterTitle();
cftLvEn->GetYaxis()->SetTitle("Track Length [cm]");
cftLvEn->GetYaxis()->CenterTitle();
TH2D* cftLvEnFrags = (TH2D*)cftLvEn->Clone("cftLvEnFrags");
cftLvEnFrags->SetTitle("");
cftLvEnFrags->SetTitle("cftLvEnFrags");
TH2D* cftLvEnFragsUnlikely = (TH2D*)cftLvEn->Clone("cftLvEnFragsUnlikely");
cftLvEnFragsUnlikely->SetTitle("");
cftLvEnFragsUnlikely->SetTitle("cftLvEnFragsUnlikely");
TH2D* cftLvEnAlphas = (TH2D*)cftLvEn->Clone("cftLvEnAlphas");
cftLvEnAlphas->SetTitle("");
cftLvEnAlphas->SetTitle("cftLvEnAlphas");
TH2D* cftLvEnAlphas_long = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_long");
cftLvEnAlphas_long->SetTitle("");
cftLvEnAlphas_long->SetTitle("cftLvEnAlphas_long");
TH2D* cftLvEnAlphas_long_low = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_long_low");
cftLvEnAlphas_long_low->SetTitle("");
cftLvEnAlphas_long_low->SetTitle("cftLvEnAlphas_long_low");
TH2D* cftLvEnAlphas_long_high = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_long_high");
cftLvEnAlphas_long_high->SetTitle("");
cftLvEnAlphas_long_high->SetTitle("cftLvEnAlphas_long_high");
TH2D* cftLvEnAlphas_short_higher = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_short_higher");
cftLvEnAlphas_short_higher->SetTitle("");
cftLvEnAlphas_short_higher->SetTitle("cftLvEnAlphas_short_higher");
TH2D* cftLvEnAlphas_alphaCut_short_low = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_alphaCut_short_low");
cftLvEnAlphas_alphaCut_short_low->SetTitle("");
cftLvEnAlphas_alphaCut_short_low->SetTitle("cftLvEnAlphas_alphaCut_short_low");

////////////////////// create normalization histograms  ///////////////////////////////////
printf("////////////////////// create normalization histograms  ///////////////////////////////////\n");
TH2D* cftLvEn_NOT_norm = (TH2D*)cftLvEn->Clone("cftLvEn_NOT_norm");
cftLvEn_NOT_norm->SetTitle("");
TH2D* noMaskLvEn_NOT_norm = (TH2D*)noMaskLvEn->Clone("noMaskLvEn_NOT_norm");
noMaskLvEn_NOT_norm->SetTitle("");
TH1D* noMaskFragsEn_NOT_norm = (TH1D*)noMaskLvEnFrags->ProjectionX("noMaskFragsEn_NOT_norm");
noMaskFragsEn_NOT_norm->SetTitle("");
TH1D* cftFragsEn_NOT_norm = (TH1D*)cftLvEnFrags->ProjectionX("cftFragsEn_NOT_norm");
cftFragsEn_NOT_norm->SetTitle("");

//////////////  fragment normalization ////////////////
printf("\n//////////////  fragment normalization ////////////////\n\n");
int minNormEnergy=12;
int minNormADC_bin=noMaskFragsEn_NOT_norm->FindBin(minNormEnergy);
double noMaskFrags_above_norm_thresh=noMaskFragsADC->Integral(minNormADC_bin,4000);
double cftFrags_above_norm_thresh=cftFragsADC->Integral(minNormADC_bin,4000);
double fragNormalization = noMaskFrags_above_norm_thresh/cftFrags_above_norm_thresh;
printf("noMaskFrags_above_norm_thresh: %d \n",(int)noMaskFrags_above_norm_thresh);
printf("cftFrags_above_norm_thresh: %d \n",(int)cftFrags_above_norm_thresh);
printf("fragNormalization: %0.4f \n",fragNormalization);

//////////////  create normalizded energy histograms ////////////////
printf("\n//////////////  create energy histograms ////////////////\n\n");
for (int adcBin = 0; adcBin < 4000; ++adcBin)
{
	if (adcBin%1000==0)printf("adcBin:%d\n",adcBin);
	for (int lengthBin = 0; lengthBin < 1000; ++lengthBin)
	{
		noMaskLvEn->SetBinContent(adcBin,lengthBin,noMaskLvADC->GetBinContent(adcBin,lengthBin));
		noMaskLvEnFrags->SetBinContent(adcBin,lengthBin,noMaskLvADCFrags->GetBinContent(adcBin,lengthBin));
		noMaskLvEnFragsUnlikely->SetBinContent(adcBin,lengthBin,noMaskLvADCFragsUnlikely->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas_long->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas_long->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas_long_low->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas_long_low->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas_long_high->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas_long_high->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas_short_higher->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas_short_higher->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas_alphaCut_short_low->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas_alphaCut_short_low->GetBinContent(adcBin,lengthBin));
		cftLvEn->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADC->GetBinContent(adcBin,lengthBin));
		cftLvEnFrags->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCFrags->GetBinContent(adcBin,lengthBin));
		cftLvEnFragsUnlikely->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCFragsUnlikely->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCAlphas->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_long->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCAlphas_long->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_long_low->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCAlphas_long_low->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_long_high->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCAlphas_long_high->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_short_higher->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCAlphas_short_higher->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_alphaCut_short_low->SetBinContent(adcBin,lengthBin,fragNormalization*cftLvADCAlphas_alphaCut_short_low->GetBinContent(adcBin,lengthBin));
		//////////////  create unmormalzed energy histogram for explanation plots ////////////////
		cftLvEn_NOT_norm->SetBinContent(adcBin,lengthBin,cftLvADC->GetBinContent(adcBin,lengthBin));
		noMaskLvEn_NOT_norm->SetBinContent(adcBin,lengthBin,noMaskLvADC->GetBinContent(adcBin,lengthBin));
		cftFragsEn_NOT_norm->SetBinContent(adcBin,cftFragsADC->GetBinContent(adcBin));
		noMaskFragsEn_NOT_norm->SetBinContent(adcBin,noMaskFragsADC->GetBinContent(adcBin));
	}
}

int rebin=2;
noMaskFragsEn_NOT_norm->Rebin(rebin);
cftFragsEn_NOT_norm->Rebin(rebin);
TCanvas* c2_wide = new TCanvas("c2_wide","c2_wide",1000,800);
c2_wide->UseCurrentStyle();
c2_wide->SetGrid(1);
c2_wide->SetRightMargin(0.1);
c2_wide->SetLeftMargin(0.08);
c2_wide->SetBorderMode(-1);
c2_wide->SetBorderSize(100);
c2_wide->SetHighLightColor(kRed);
noMaskLvEn_NOT_norm->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_NOT_norm->GetXaxis()->CenterTitle();
noMaskLvEn_NOT_norm->GetXaxis()->SetRangeUser(0,125);
noMaskLvEn_NOT_norm->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_NOT_norm->GetYaxis()->CenterTitle();
noMaskLvEn_NOT_norm->GetYaxis()->SetRangeUser(0,5);
noMaskLvEn_NOT_norm->SetTitle("noMask Data");
noMaskLvEn_NOT_norm->Draw("colz");
fragCut_energy->SetLineColor(kRed);
fragCut_energy->SetFillColorAlpha(kRed,0.2);
fragCut_energy->SetLineWidth(2);
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
c2_wide->SaveAs("normalization_noMask_LvsADC_fragCut_energy.png");
double maxY_energy_norm=0;
if( noMaskFragsEn_NOT_norm->GetMaximum() > cftFragsEn_NOT_norm->GetMaximum()){
	maxY_energy_norm = 1.3*noMaskFragsEn_NOT_norm->GetMaximum();
}
else{
	maxY_energy_norm = 1.3*cftFragsEn_NOT_norm->GetMaximum();
}

noMaskFragsEn_NOT_norm->SetLineColor(kRed);
noMaskFragsEn_NOT_norm->SetFillColorAlpha(kRed,0.2);
noMaskFragsEn_NOT_norm->GetXaxis()->SetRangeUser(0,125);
noMaskFragsEn_NOT_norm->GetYaxis()->SetRangeUser(0,maxY_energy_norm);
// noMaskFragsEn_NOT_norm->SetTitle("noMaskLvEn_NOT_norm - energy projection");
noMaskFragsEn_NOT_norm->SetTitle("");
noMaskFragsEn_NOT_norm->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskFragsEn_NOT_norm->Draw();
TLegend* noMaskFrag_legend = new TLegend(0.4,0.78,0.89,0.89);
// noMaskFrag_legend->SetTextFont(42);
noMaskFrag_legend->SetTextSize(0.054);
sprintf(buffChar,"  %d fragments",(int)noMaskLvADCFrags_integral);
printf("buffChar:%s\n",buffChar);
noMaskFrag_legend->AddEntry((TObject*)0,buffChar, "");
noMaskFrag_legend->Draw();
c2_wide->SaveAs("normalization_noMaskFragsenergy_fragCut_energy.png");
cftLvEn_NOT_norm->GetXaxis()->SetTitle("Track Energy [MeV]");
cftLvEn_NOT_norm->GetXaxis()->CenterTitle();
cftLvEn_NOT_norm->GetXaxis()->SetRangeUser(0,125);
cftLvEn_NOT_norm->GetYaxis()->SetTitle("Track Length [cm]");
cftLvEn_NOT_norm->GetYaxis()->CenterTitle();
cftLvEn_NOT_norm->GetYaxis()->SetRangeUser(0,5);
cftLvEn_NOT_norm->SetTitle("Fission Trigger Data");
cftLvEn_NOT_norm->Draw("colz");
fragCut_energy->SetLineColor(kBlue);
fragCut_energy->SetFillColorAlpha(kBlue,0.2);
fragCut_energy->SetLineWidth(2);
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
c2_wide->SaveAs("normalization_cft_Lvsenergy_fragCut_energy.png");
cftFragsEn_NOT_norm->SetLineColor(kBlue);
cftFragsEn_NOT_norm->SetFillColorAlpha(kBlue,0.2);
cftFragsEn_NOT_norm->GetXaxis()->SetRangeUser(0,125);
cftFragsEn_NOT_norm->GetYaxis()->SetRangeUser(0,maxY_energy_norm);
// cftFragsEn_NOT_norm->SetTitle("cftLvEn_NOT_norm - energy projection");
cftFragsEn_NOT_norm->SetTitle("");
cftFragsEn_NOT_norm->GetXaxis()->SetTitle("Track Energy [MeV]");
cftFragsEn_NOT_norm->Draw();
TLegend* cftFrag_legend = new TLegend(0.4,0.78,0.89,0.89);
// cftFrag_legend->SetTextFont(42);
cftFrag_legend->SetTextSize(0.054);
sprintf(buffChar,"  %d fragments",(int)cftLvADCFrags_integral);
printf("buffChar:%s\n",buffChar);
cftFrag_legend->AddEntry((TObject*)0,buffChar, "");
cftFrag_legend->Draw();
c2_wide->SaveAs("normalization_cftFragsADC_fragCut_energy.png");
TH1D* cftFragsEn_normalized = (TH1D*)cftFragsEn_NOT_norm->Clone("cftFragsEn_normalized");
cftFragsEn_normalized->SetTitle("");
TH1D* cftFragsEn_ScaleRegion = (TH1D*)cftFragsEn_NOT_norm->Clone("cftFragsEn_ScaleRegion");
cftFragsEn_ScaleRegion->SetTitle("");
minNormADC_bin=cftFragsEn_normalized->FindBin(minNormEnergy);
for (int i = 0; i < cftFragsEn_normalized->GetNbinsX(); ++i) {
	cftFragsEn_ScaleRegion->SetBinContent(i,fragNormalization*cftFragsEn_NOT_norm->GetBinContent(i));
	cftFragsEn_normalized->SetBinContent(i,fragNormalization*cftFragsEn_NOT_norm->GetBinContent(i));
	if(i < minNormADC_bin) cftFragsEn_ScaleRegion->SetBinContent(i,0);
}
TCanvas* c3_wide = new TCanvas("c3_wide","c3_wide",2000,800);
c3_wide->UseCurrentStyle();
c3_wide->SetGrid(1);
c3_wide->SetRightMargin(0.05);
c3_wide->SetLeftMargin(0.04);
c3_wide->SetBorderMode(-1);
c3_wide->SetBorderSize(100);
c3_wide->SetHighLightColor(kRed);
noMaskFragsEn_NOT_norm->SetFillColorAlpha(kRed,0);
cftFragsEn_normalized->SetFillColorAlpha(kBlue,0);
cftFragsEn_ScaleRegion->SetFillColorAlpha(kViolet,0.2);
cftFragsEn_normalized->SetTitle("Fission Trigger Data Normalized to noMask Tracks");
printf("maxY_energy_norm: %f\n", maxY_energy_norm);
maxY_energy_norm = 1.1*cftFragsEn_normalized->GetMaximum();
printf("maxY_energy_norm: %f\n", maxY_energy_norm);
printf("cftFragsEn_normalized->Integral(): %f \n",cftFragsEn_normalized->Integral());
printf("noMaskFragsEn_NOT_norm->Integral(): %f \n",noMaskFragsEn_NOT_norm->Integral());
printf("cftFragsEn_ScaleRegion->Integral(): %f \n",cftFragsEn_ScaleRegion->Integral());
TLegend* normFrag_legend = new TLegend(0.05,0.74,0.94,0.89);
// normFrag_legend->SetTextFont(42);
normFrag_legend->SetNColumns(2);
normFrag_legend->SetTextSize(0.054);
normFrag_legend->AddEntry(cftFragsEn_ScaleRegion,"normalization region","f");
sprintf(buffChar,"%d noMask fragments > %d MeV",(int)noMaskFrags_above_norm_thresh,minNormEnergy);
printf("buffChar:%s\n",buffChar);
normFrag_legend->AddEntry((TObject*)0,buffChar, "");
sprintf(buffChar," %.6f = normalization factor",fragNormalization);
printf("buffChar:%s\n",buffChar);
normFrag_legend->AddEntry((TObject*)0,buffChar, "");
sprintf(buffChar,"%d CFT fragments > %d MeV",(int)cftFrags_above_norm_thresh,minNormEnergy);
printf("buffChar:%s\n",buffChar);
normFrag_legend->AddEntry((TObject*)0,buffChar, "");
cftFragsEn_normalized->SetMaximum(maxY_energy_norm);
cftFragsEn_normalized->SetMinimum(0);
noMaskFragsEn_NOT_norm->SetMaximum(maxY_energy_norm);
noMaskFragsEn_NOT_norm->SetMinimum(0);
cftFragsEn_ScaleRegion->SetMaximum(maxY_energy_norm);
cftFragsEn_ScaleRegion->SetMinimum(0);
cftFragsEn_normalized->Draw();
c3_wide->ForceUpdate();
noMaskFragsEn_NOT_norm->Draw("same");
cftFragsEn_ScaleRegion->Draw("same");
cftFragsEn_normalized->SetMaximum(maxY_energy_norm);
cftFragsEn_normalized->SetMinimum(0);
noMaskFragsEn_NOT_norm->SetMaximum(maxY_energy_norm);
noMaskFragsEn_NOT_norm->SetMinimum(0);
cftFragsEn_ScaleRegion->SetMaximum(maxY_energy_norm);
cftFragsEn_ScaleRegion->SetMinimum(0);
c3_wide->ForceUpdate();
normFrag_legend->Draw();
c3_wide->SaveAs("normalization_fragEn.png");

// return;
fragCut_energy->SetName("fragCut_energy");
fragCut_energy->SetLineColor(kViolet);
fragCut_energy->SetLineWidth((Width_t)0.01);
fragCut_energy->SetFillColorAlpha(kViolet,0.3);
c1->cd();

// ////////////////////// noMask fill color coded length-ADC containers ///////////////////////////////////
// printf("////////////////////// noMask fill color coded length-ADC containers ///////////////////////////////////\n");
//
// c1->SetLogy(0);
// noMaskLvEnAlphas_long->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnAlphas_long->Draw("colz");
// alphaCut_long_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnAlphas_long.png");
// noMaskLvEnAlphas_long_low->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnAlphas_long_low->Draw("colz");
// alphaCut_long_low_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnAlphas_long_low.png");
// noMaskLvEnAlphas_long_high->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnAlphas_long_high->Draw("colz");
// long_high_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnAlphas_long_high.png");
// noMaskLvEnAlphas_short_higher->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnAlphas_short_higher->Draw("colz");
// alphaCut_short_higher_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnAlphas_short_higher.png");
// noMaskLvEnAlphas_alphaCut_short_low->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnAlphas_alphaCut_short_low->Draw("colz");
// alphaCut_short_low_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnAlphas_alphaCut_short_low.png");
// noMaskLvEnAlphas->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnAlphas->Draw("colz");
// alphaCut_above_zCut_energy->Draw("same f");
// alphaCut_above_zCut_energy->Draw("same");
// c1->SaveAs("noMaskLvEnAlphas.png");
// noMaskLvEnFrags->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnFrags->Draw("colz");
// fragCut_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnFrags.png");
// noMaskLvEnFragsUnlikely->GetXaxis()->SetRangeUser(0.4,10);
// noMaskLvEnFragsUnlikely->Draw("colz");
// junkCut_energy->Draw("same f");
// c1->SaveAs("noMaskLvEnFragsUnlikely.png");
// //////////////////// cft fill color coded length-ADC containers ///////////////////////////////////
// // printf("////////////////////// cft fill color coded length-ADC containers ///////////////////////////////////\n");
// cftLvEnAlphas_long->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnAlphas_long->Draw("colz");
// alphaCut_long_energy->Draw("same f");
// c1->SaveAs("cftLvEnAlphas_long.png");
// cftLvEnAlphas_long_low->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnAlphas_long_low->Draw("colz");
// alphaCut_long_low_energy->Draw("same f");
// c1->SaveAs("cftLvEnAlphas_long_low.png");
// cftLvEnAlphas_long_high->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnAlphas_long_high->Draw("colz");
// long_high_energy->Draw("same f");
// c1->SaveAs("cftLvEnAlphas_long_high.png");
// cftLvEnAlphas_short_higher->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnAlphas_short_higher->Draw("colz");
// alphaCut_short_higher_energy->Draw("same f");
// c1->SaveAs("cftLvEnAlphas_short_higher.png");
// cftLvEnAlphas_alphaCut_short_low->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnAlphas_alphaCut_short_low->Draw("colz");
// alphaCut_short_low_energy->Draw("same f");
// c1->SaveAs("cftLvEnAlphas_alphaCut_short_low.png");
// cftLvEnAlphas->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnAlphas->Draw("colz");
// alphaCut_above_zCut_energy->Draw("same f");
// alphaCut_above_zCut_energy->Draw("same");
// c1->SaveAs("cftLvEnAlphas.png");
// cftLvEnFrags->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnFrags->Draw("colz");
// fragCut_energy->Draw("same f");
// c1->SaveAs("cftLvEnFrags.png");
// cftLvEnFragsUnlikely->GetXaxis()->SetRangeUser(0.4,10);
// cftLvEnFragsUnlikely->Draw("colz");
// junkCut_energy->Draw("same f");
// c1->SaveAs("cftLvEnFragsUnlikely.png");

c1->SetLogx(0);c1->SetLogy(1);
TH1D* noMaskEn = new TH1D("noMaskEn","noMaskEn",4000,0,40000*MeV_per_ADC);
noMaskEn->SetTitle("");
noMaskEn = noMaskLvEn->ProjectionX("noMaskEn");
noMaskEn->Rebin(rebin);
double maxY_energy = 1.5*noMaskEn->GetMaximum();
noMaskEn->SetLineColor(kRed);
// noMaskEn->SetLineWidth((Width_t)0.01);
noMaskEn->SetFillColorAlpha(kRed,0.2);
noMaskEn->GetXaxis()->SetRangeUser(0.4,140);
noMaskEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskEn->SetTitle("noMaskLvEn energy projection");
noMaskEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskEn->Draw();
c1->SaveAs("noMaskEn.png");
TH1D* noMaskFragsEn = new TH1D("noMaskFragsEn","noMaskFragsEn",4000,0,40000*MeV_per_ADC);
noMaskFragsEn->SetTitle("");
noMaskFragsEn = noMaskLvEnFrags->ProjectionX("noMaskFragsEn");
noMaskFragsEn->Rebin(rebin);
noMaskFragsEn->GetXaxis()->SetRangeUser(0.4,140);
noMaskFragsEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskFragsEn->SetTitle("noMaskFragsEn");
noMaskFragsEn->SetLineColor(kViolet);
noMaskFragsEn->SetLineWidth((Width_t)0.01);
noMaskFragsEn->SetFillColorAlpha(kViolet,0.7);
noMaskFragsEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskFragsEn->Draw();
c1->SaveAs("noMaskFragsEn.png");
TH1D* noMaskFragsUnlikelyEn = new TH1D("noMaskFragsUnlikelyEn","noMaskFragsUnlikelyEn",4000,0,40000*MeV_per_ADC);
noMaskFragsUnlikelyEn->SetTitle("");
noMaskFragsUnlikelyEn = noMaskLvEnFragsUnlikely->ProjectionX("noMaskFragsUnlikelyEn");
noMaskFragsUnlikelyEn->Rebin(rebin);
noMaskFragsUnlikelyEn->GetXaxis()->SetRangeUser(0.4,140);
noMaskFragsUnlikelyEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskFragsUnlikelyEn->SetTitle("noMaskFragsUnlikelyEn");
noMaskFragsUnlikelyEn->SetLineColor(28);
noMaskFragsUnlikelyEn->SetLineWidth((Width_t)0.01);
noMaskFragsUnlikelyEn->SetFillColorAlpha(28,0.7);
noMaskFragsUnlikelyEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskFragsUnlikelyEn->Draw();
c1->SaveAs("noMaskFragsUnlikelyEn.png");
TH1D* noMaskAlphasEn = new TH1D("noMaskAlphasEn","noMaskAlphasEn",4000,0,40000*MeV_per_ADC);
noMaskAlphasEn->SetTitle("");
noMaskAlphasEn = noMaskLvEnAlphas->ProjectionX("noMaskAlphasEn");
noMaskAlphasEn->Rebin(rebin);
noMaskAlphasEn->GetXaxis()->SetRangeUser(0.4,140);
noMaskAlphasEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskAlphasEn->SetTitle("noMaskAlphasEn");
noMaskAlphasEn->SetLineColor(kSpring);
noMaskAlphasEn->SetLineWidth((Width_t)0.01);
noMaskAlphasEn->SetFillColorAlpha(kSpring,0.7);
noMaskAlphasEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskAlphasEn->Draw();
c1->SaveAs("noMaskAlphasEn.png");
TH1D* noMaskAlphasEn_long = new TH1D("noMaskAlphasEn_long","noMaskAlphasEn_long",4000,0,40000*MeV_per_ADC);
noMaskAlphasEn_long->SetTitle("");
noMaskAlphasEn_long = noMaskLvEnAlphas_long->ProjectionX("noMaskAlphasEn_long");
noMaskAlphasEn_long->Rebin(rebin);
noMaskAlphasEn_long->GetXaxis()->SetRangeUser(0.4,140);
noMaskAlphasEn_long->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskAlphasEn_long->SetTitle("noMaskAlphasEn_long");
noMaskAlphasEn_long->SetLineColor(kCyan);
noMaskAlphasEn_long->SetLineWidth((Width_t)0.01);
noMaskAlphasEn_long->SetFillColorAlpha(kCyan,0.7);
noMaskAlphasEn_long->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskAlphasEn_long->Draw();
c1->SaveAs("noMaskAlphasEn_long.png");
TH1D* noMaskAlphasEn_long_low = new TH1D("noMaskAlphasEn_long_low","noMaskAlphasEn_long_low",4000,0,40000*MeV_per_ADC);
noMaskAlphasEn_long_low->SetTitle("");
noMaskAlphasEn_long_low = noMaskLvEnAlphas_long_low->ProjectionX("noMaskAlphasEn_long_low");
noMaskAlphasEn_long_low->Rebin(rebin);
noMaskAlphasEn_long_low->GetXaxis()->SetRangeUser(0.4,140);
noMaskAlphasEn_long_low->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskAlphasEn_long_low->SetTitle("noMaskAlphasEn_long_low");
noMaskAlphasEn_long_low->SetLineColor(kYellow);
noMaskAlphasEn_long_low->SetLineWidth((Width_t)0.01);
noMaskAlphasEn_long_low->SetFillColorAlpha(kYellow,0.7);
noMaskAlphasEn_long_low->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskAlphasEn_long_low->Draw();
c1->SaveAs("noMaskAlphasEn_long_low.png");
TH1D* noMaskAlphasEn_long_high = new TH1D("noMaskAlphasEn_long_high","noMaskAlphasEn_long_high",4000,0,40000*MeV_per_ADC);
noMaskAlphasEn_long_high->SetTitle("");
noMaskAlphasEn_long_high = noMaskLvEnAlphas_long_high->ProjectionX("noMaskAlphasEn_long_high");
noMaskAlphasEn_long_high->Rebin(rebin);
noMaskAlphasEn_long_high->GetXaxis()->SetRangeUser(0.4,140);
noMaskAlphasEn_long_high->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskAlphasEn_long_high->SetTitle("noMaskAlphasEn_long_high");
noMaskAlphasEn_long_high->SetLineColor(kBlue);
noMaskAlphasEn_long_high->SetLineWidth((Width_t)0.01);
noMaskAlphasEn_long_high->SetFillColorAlpha(kBlue,0.7);
noMaskAlphasEn_long_high->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskAlphasEn_long_high->Draw();
c1->SaveAs("noMaskAlphasEn_long_high.png");
TH1D* noMaskAlphasEn_short_higher = new TH1D("noMaskAlphasEn_short_higher","noMaskAlphasEn_short_higher",4000,0,40000*MeV_per_ADC);
noMaskAlphasEn_short_higher->SetTitle("");
noMaskAlphasEn_short_higher = noMaskLvEnAlphas_short_higher->ProjectionX("noMaskAlphasEn_short_higher");
noMaskAlphasEn_short_higher->Rebin(rebin);
noMaskAlphasEn_short_higher->GetXaxis()->SetRangeUser(0.4,140);
noMaskAlphasEn_short_higher->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskAlphasEn_short_higher->SetTitle("noMaskAlphasEn_short_higher");
noMaskAlphasEn_short_higher->SetLineColor(kOrange+7);
noMaskAlphasEn_short_higher->SetLineWidth((Width_t)0.01);
noMaskAlphasEn_short_higher->SetFillColorAlpha(kOrange+7,0.7);
noMaskAlphasEn_short_higher->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskAlphasEn_short_higher->Draw();
c1->SaveAs("noMaskAlphasEn_short_higher.png");
TH1D* noMaskAlphasEn_alphaCut_short_low = new TH1D("noMaskAlphasEn_alphaCut_short_low","noMaskAlphasEn_alphaCut_short_low",4000,0,40000*MeV_per_ADC);
noMaskAlphasEn_alphaCut_short_low->SetTitle("");
noMaskAlphasEn_alphaCut_short_low = noMaskLvEnAlphas_alphaCut_short_low->ProjectionX("noMaskAlphasEn_alphaCut_short_low");
noMaskAlphasEn_alphaCut_short_low->Rebin(rebin);
noMaskAlphasEn_alphaCut_short_low->GetXaxis()->SetRangeUser(0.4,140);
noMaskAlphasEn_alphaCut_short_low->GetYaxis()->SetRangeUser(0.1,maxY_energy);
noMaskAlphasEn_alphaCut_short_low->SetTitle("noMaskAlphasEn_alphaCut_short_low");
noMaskAlphasEn_alphaCut_short_low->SetLineColor(kRed);
noMaskAlphasEn_alphaCut_short_low->SetLineWidth((Width_t)0.1);
noMaskAlphasEn_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
noMaskAlphasEn_alphaCut_short_low->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskAlphasEn_alphaCut_short_low->Draw();
c1->SaveAs("noMaskAlphasEn_alphaCut_short_low.png");
////////////////////// cft create ADC projection of color coded length-ADC containers ///////////////
TH1D* cftEn = new TH1D("cftEn","cftEn",4000,0,40000*MeV_per_ADC);
cftEn->SetTitle("");
cftEn = cftLvEn->ProjectionX("cftEn");
cftEn->Rebin(rebin);
cftEn->SetLineColor(kBlue);
// cftEn->SetLineWidth((Width_t)0.01);
cftEn->SetFillColorAlpha(kBlue,0.2);
cftEn->GetXaxis()->SetRangeUser(0.4,140);
cftEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftEn->SetTitle("cftLvEn energy projection");
cftEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftEn->Draw();
c1->SaveAs("cftEn.png");
TH1D* cftFragsEn = new TH1D("cftFragsEn","cftFragsEn",4000,0,40000*MeV_per_ADC);
cftFragsEn->SetTitle("");
cftFragsEn = cftLvEnFrags->ProjectionX("cftFragsEn");
cftFragsEn->Rebin(rebin);
cftFragsEn->GetXaxis()->SetRangeUser(0.4,140);
cftFragsEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftFragsEn->SetTitle("cftFragsEn");
cftFragsEn->SetLineColor(kViolet);
cftFragsEn->SetLineWidth((Width_t)0.01);
cftFragsEn->SetFillColorAlpha(kViolet,0.7);
cftFragsEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftFragsEn->Draw();
c1->SaveAs("cftFragsEn.png");
TH1D* cftFragsUnlikelyEn = new TH1D("cftFragsUnlikelyEn","cftFragsUnlikelyEn",4000,0,40000*MeV_per_ADC);
cftFragsUnlikelyEn->SetTitle("");
cftFragsUnlikelyEn = cftLvEnFragsUnlikely->ProjectionX("cftFragsUnlikelyEn");
cftFragsUnlikelyEn->Rebin(rebin);
cftFragsUnlikelyEn->GetXaxis()->SetRangeUser(0.4,140);
cftFragsUnlikelyEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftFragsUnlikelyEn->SetTitle("cftFragsUnlikelyEn");
cftFragsUnlikelyEn->SetLineColor(28);
cftFragsUnlikelyEn->SetLineWidth((Width_t)0.01);
cftFragsUnlikelyEn->SetFillColorAlpha(28,0.7);
cftFragsUnlikelyEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftFragsUnlikelyEn->Draw();
c1->SaveAs("cftFragsUnlikelyEn.png");
TH1D* cftAlphasEn = new TH1D("cftAlphasEn","cftAlphasEn",4000,0,40000*MeV_per_ADC);
cftAlphasEn->SetTitle("");
cftAlphasEn = cftLvEnAlphas->ProjectionX("cftAlphasEn");
cftAlphasEn->Rebin(rebin);
cftAlphasEn->GetXaxis()->SetRangeUser(0.4,140);
cftAlphasEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftAlphasEn->SetTitle("cftAlphasEn");
cftAlphasEn->SetLineColor(kSpring);
cftAlphasEn->SetLineWidth((Width_t)0.01);
cftAlphasEn->SetFillColorAlpha(kSpring,0.7);
cftAlphasEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftAlphasEn->SetStats(1);
cftAlphasEn->Draw();
c1->SaveAs("cftAlphasEn.png");
TH1D* cftAlphasEn_long = new TH1D("cftAlphasEn_long","cftAlphasEn_long",4000,0,40000*MeV_per_ADC);
cftAlphasEn_long->SetTitle("");
cftAlphasEn_long = cftLvEnAlphas_long->ProjectionX("cftAlphasEn_long");
cftAlphasEn_long->Rebin(rebin);
cftAlphasEn_long->GetXaxis()->SetRangeUser(0.4,140);
cftAlphasEn_long->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftAlphasEn_long->SetTitle("cftAlphasEn_long");
cftAlphasEn_long->SetLineColor(kCyan);
cftAlphasEn_long->SetLineWidth((Width_t)0.01);
cftAlphasEn_long->SetFillColorAlpha(kCyan,0.7);
cftAlphasEn_long->GetXaxis()->SetTitle("Track Energy [MeV]");
cftAlphasEn_long->Draw();
c1->SaveAs("cftAlphasEn_long.png");
TH1D* cftAlphasEn_long_low = new TH1D("cftAlphasEn_long_low","cftAlphasEn_long_low",4000,0,40000*MeV_per_ADC);
cftAlphasEn_long_low->SetTitle("");
cftAlphasEn_long_low = cftLvEnAlphas_long_low->ProjectionX("cftAlphasEn_long_low");
cftAlphasEn_long_low->Rebin(rebin);
cftAlphasEn_long_low->GetXaxis()->SetRangeUser(0.4,140);
cftAlphasEn_long_low->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftAlphasEn_long_low->SetTitle("cftAlphasEn_long_low");
cftAlphasEn_long_low->SetLineColor(kYellow);
cftAlphasEn_long_low->SetLineWidth((Width_t)0.01);
cftAlphasEn_long_low->SetFillColorAlpha(kYellow,0.7);
cftAlphasEn_long_low->GetXaxis()->SetTitle("Track Energy [MeV]");
cftAlphasEn_long_low->Draw();
c1->SaveAs("cftAlphasEn_long_low.png");
TH1D* cftAlphasEn_long_high = new TH1D("cftAlphasEn_long_high","cftAlphasEn_long_high",4000,0,40000*MeV_per_ADC);
cftAlphasEn_long_high->SetTitle("");
cftAlphasEn_long_high = cftLvEnAlphas_long_high->ProjectionX("cftAlphasEn_long_high");
cftAlphasEn_long_high->Rebin(rebin);
cftAlphasEn_long_high->GetXaxis()->SetRangeUser(0.4,140);
cftAlphasEn_long_high->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftAlphasEn_long_high->SetTitle("cftAlphasEn_long_high");
cftAlphasEn_long_high->SetLineColor(kBlue);
cftAlphasEn_long_high->SetLineWidth((Width_t)0.01);
cftAlphasEn_long_high->SetFillColorAlpha(kBlue,0.7);
cftAlphasEn_long_high->GetXaxis()->SetTitle("Track Energy [MeV]");
cftAlphasEn_long_high->Draw();
c1->SaveAs("cftAlphasEn_long_high.png");
TH1D* cftAlphasEn_short_higher = new TH1D("cftAlphasEn_short_higher","cftAlphasEn_short_higher",4000,0,40000*MeV_per_ADC);
cftAlphasEn_short_higher->SetTitle("");
cftAlphasEn_short_higher = cftLvEnAlphas_short_higher->ProjectionX("cftAlphasEn_short_higher");
cftAlphasEn_short_higher->Rebin(rebin);
cftAlphasEn_short_higher->GetXaxis()->SetRangeUser(0.4,140);
cftAlphasEn_short_higher->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftAlphasEn_short_higher->SetTitle("cftAlphasEn_short_higher");
cftAlphasEn_short_higher->SetLineColor(kOrange+7);
cftAlphasEn_short_higher->SetLineWidth((Width_t)0.01);
cftAlphasEn_short_higher->SetFillColorAlpha(kOrange+7,0.7);
cftAlphasEn_short_higher->GetXaxis()->SetTitle("Track Energy [MeV]");
cftAlphasEn_short_higher->Draw();
c1->SaveAs("cftAlphasEn_short_higher.png");
TH1D* cftAlphasEn_alphaCut_short_low = new TH1D("cftAlphasEn_alphaCut_short_low","cftAlphasEn_alphaCut_short_low",4000,0,40000*MeV_per_ADC);
cftAlphasEn_alphaCut_short_low->SetTitle("");
cftAlphasEn_alphaCut_short_low = cftLvEnAlphas_alphaCut_short_low->ProjectionX("cftAlphasEn_alphaCut_short_low");
cftAlphasEn_alphaCut_short_low->Rebin(rebin);
cftAlphasEn_alphaCut_short_low->GetXaxis()->SetRangeUser(0.4,140);
cftAlphasEn_alphaCut_short_low->GetYaxis()->SetRangeUser(0.1,maxY_energy);
cftAlphasEn_alphaCut_short_low->SetTitle("cftAlphasEn_alphaCut_short_low");
cftAlphasEn_alphaCut_short_low->SetLineColor(kRed);
cftAlphasEn_alphaCut_short_low->SetLineWidth((Width_t)0.1);
cftAlphasEn_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
cftAlphasEn_alphaCut_short_low->GetXaxis()->SetTitle("Track Energy [MeV]");
cftAlphasEn_alphaCut_short_low->Draw();
c1->SaveAs("cftAlphasEn_alphaCut_short_low.png");

double noMaskIntegral=noMaskADC->Integral();
printf("noMaskIntegral: %f\n",noMaskIntegral);
double noMaskFragIntegral=noMaskFragsADC->Integral();
printf("noMaskFragIntegral: %f\n",noMaskFragIntegral);
double noMaskAlphaIntegral=noMaskAlphasADC->Integral();
printf("noMaskAlphaIntegral: %f\n",noMaskAlphaIntegral);
double noMaskFragsEn_percent_integral=noMaskFragsEn->Integral()/(double)noMaskIntegral*100;
printf("noMaskFragsEn_percent_integral: %.4f\n",noMaskFragsEn_percent_integral);
double noMaskFragsUnlikelyEn_percent_integral=noMaskFragsUnlikelyEn->Integral()/(double)noMaskIntegral*100;
printf("noMaskFragsUnlikelyEn_percent_integral: %.4f\n",noMaskFragsUnlikelyEn_percent_integral);
double noMaskAlphasEn_percent_integral=noMaskAlphasEn->Integral()/(double)noMaskIntegral*100;
printf("noMaskAlphasEn_percent_integral: %.4f\n",noMaskAlphasEn_percent_integral);
double noMaskAlphasEn_long_percent_integral=noMaskAlphasEn_long->Integral()/(double)noMaskIntegral*100;
printf("noMaskAlphasEn_long_percent_integral: %.4f\n",noMaskAlphasEn_long_percent_integral);
double noMaskAlphasEn_long_low_percent_integral=noMaskAlphasEn_long_low->Integral()/(double)noMaskIntegral*100;
printf("noMaskAlphasEn_long_low_percent_integral: %.4f\n",noMaskAlphasEn_long_low_percent_integral);
double noMaskAlphasEn_long_high_percent_integral=noMaskAlphasEn_long_high->Integral()/(double)noMaskIntegral*100;
printf("noMaskAlphasEn_long_high_percent_integral: %.4f\n",noMaskAlphasEn_long_high_percent_integral);
double noMaskAlphasEn_short_higher_percent_integral=noMaskAlphasEn_short_higher->Integral()/(double)noMaskIntegral*100;
printf("noMaskAlphasEn_short_higher_percent_integral: %.4f\n",noMaskAlphasEn_short_higher_percent_integral);
double noMaskAlphasEn_alphaCut_short_low_percent_integral=noMaskAlphasEn_alphaCut_short_low->Integral()/(double)noMaskIntegral*100;
printf("noMaskAlphasEn_alphaCut_short_low_percent_integral: %.4f\n",noMaskAlphasEn_alphaCut_short_low_percent_integral);

//////////////  count frags ////////////////
printf("\n//////////////  count frags ////////////////\n\n");
int noMaskNotFragIntegral=noMaskIntegral-noMaskFragIntegral;
printf("noMaskNotFragIntegral: %d\n",noMaskNotFragIntegral);
double percent_FragAccepted = 100*cftFragsEn->Integral()/(double)noMaskFragsEn->Integral();
printf("percent_FragAccepted: %.4f\n",percent_FragAccepted);
double percent_AlphasRejected = 100*TMath::Abs(1-cftAlphasEn->Integral()/(double)noMaskAlphasEn->Integral());
printf("percent_AlphasRejected: %.4f\n",percent_AlphasRejected);

printf("\n\n");
double alpha_COUNT_noMaskLvADCAlphas_integral = alphaScaler_alphaCut_zCut_adc_region*noMaskLvADCAlphas_integral;
printf("alpha_COUNT_noMaskLvADCAlphas_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCAlphas_integral,alphaScaler_alphaCut_zCut_adc_region,noMaskLvADCAlphas_integral);
double alpha_COUNT_noMaskLvADCFrags_integral = alphaScaler_fragCut_adc_region*noMaskLvADCFrags_integral;
printf("alpha_COUNT_noMaskLvADCFrags_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCFrags_integral,alphaScaler_fragCut_adc_region,noMaskLvADCFrags_integral);
double alpha_COUNT_noMaskLvADCFragsUnlikely_integral = alphaScaler_fragCut_unlikely_adc_region*noMaskLvADCFragsUnlikely_integral;
printf("alpha_COUNT_noMaskLvADCFragsUnlikely_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCFragsUnlikely_integral,alphaScaler_fragCut_unlikely_adc_region,noMaskLvADCFragsUnlikely_integral);
double alpha_COUNT_noMaskLvADCAlphas_long_integral = alphaScaler_alphaCut_long_adc_region*noMaskLvADCAlphas_long_integral;
printf("alpha_COUNT_noMaskLvADCAlphas_long_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCAlphas_long_integral,alphaScaler_alphaCut_long_adc_region,noMaskLvADCAlphas_long_integral);
double alpha_COUNT_noMaskLvADCAlphas_long_low_integral = alphaScaler_alphaCut_long_low_adc_region*noMaskLvADCAlphas_long_low_integral;
printf("alpha_COUNT_noMaskLvADCAlphas_long_low_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCAlphas_long_low_integral,alphaScaler_alphaCut_long_low_adc_region,noMaskLvADCAlphas_long_low_integral);
double alpha_COUNT_noMaskLvADCAlphas_long_high_integral = alphaScaler_long_high_adc_region*noMaskLvADCAlphas_long_high_integral;
printf("alpha_COUNT_noMaskLvADCAlphas_long_high_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCAlphas_long_high_integral,alphaScaler_long_high_adc_region,noMaskLvADCAlphas_long_high_integral);
double alpha_COUNT_noMaskLvADCAlphas_short_higher_integral = alphaScaler_alphaCut_short_higher_adc_region*noMaskLvADCAlphas_short_higher_integral;
printf("alpha_COUNT_noMaskLvADCAlphas_short_higher_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCAlphas_short_higher_integral,alphaScaler_alphaCut_short_higher_adc_region,noMaskLvADCAlphas_short_higher_integral);
double alpha_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral = alphaScaler_alphaCut_short_low_adc_region*noMaskLvADCAlphas_alphaCut_short_low_integral;
printf("alpha_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral:%f = %f x %f \n",alpha_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral,alphaScaler_alphaCut_short_low_adc_region,noMaskLvADCAlphas_alphaCut_short_low_integral);

double frag_COUNT_noMaskLvADCAlphas_integral = fragScaler_alphaCut_zCut_adc_region*noMaskLvADCAlphas_integral;
printf("frag_COUNT_noMaskLvADCAlphas_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCAlphas_integral,fragScaler_alphaCut_zCut_adc_region,noMaskLvADCAlphas_integral);
double frag_COUNT_noMaskLvADCFrags_integral = fragScaler_fragCut_adc_region*noMaskLvADCFrags_integral;
printf("frag_COUNT_noMaskLvADCFrags_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCFrags_integral,fragScaler_fragCut_adc_region,noMaskLvADCFrags_integral);
double frag_COUNT_noMaskLvADCFragsUnlikely_integral = fragScaler_fragCut_unlikely_adc_region*noMaskLvADCFragsUnlikely_integral;
printf("frag_COUNT_noMaskLvADCFragsUnlikely_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCFragsUnlikely_integral,fragScaler_fragCut_unlikely_adc_region,noMaskLvADCFragsUnlikely_integral);
double frag_COUNT_noMaskLvADCAlphas_long_integral = fragScaler_alphaCut_long_adc_region*noMaskLvADCAlphas_long_integral;
printf("frag_COUNT_noMaskLvADCAlphas_long_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCAlphas_long_integral,fragScaler_alphaCut_long_adc_region,noMaskLvADCAlphas_long_integral);
double frag_COUNT_noMaskLvADCAlphas_long_low_integral = fragScaler_alphaCut_long_low_adc_region*noMaskLvADCAlphas_long_low_integral;
printf("frag_COUNT_noMaskLvADCAlphas_long_low_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCAlphas_long_low_integral,fragScaler_alphaCut_long_low_adc_region,noMaskLvADCAlphas_long_low_integral);
double frag_COUNT_noMaskLvADCAlphas_long_high_integral = fragScaler_long_high_adc_region*noMaskLvADCAlphas_long_high_integral;
printf("frag_COUNT_noMaskLvADCAlphas_long_high_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCAlphas_long_high_integral,fragScaler_long_high_adc_region,noMaskLvADCAlphas_long_high_integral);
double frag_COUNT_noMaskLvADCAlphas_short_higher_integral = fragScaler_alphaCut_short_higher_adc_region*noMaskLvADCAlphas_short_higher_integral;
printf("frag_COUNT_noMaskLvADCAlphas_short_higher_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCAlphas_short_higher_integral,fragScaler_alphaCut_short_higher_adc_region,noMaskLvADCAlphas_short_higher_integral);
double frag_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral = fragScaler_alphaCut_short_low_adc_region*noMaskLvADCAlphas_alphaCut_short_low_integral;
printf("frag_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral:%f = %f x %f \n",frag_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral,fragScaler_alphaCut_short_low_adc_region,noMaskLvADCAlphas_alphaCut_short_low_integral);

double corrected_alphaCount =
			alpha_COUNT_noMaskLvADCAlphas_integral+
			alpha_COUNT_noMaskLvADCFrags_integral+
			alpha_COUNT_noMaskLvADCFragsUnlikely_integral+
			alpha_COUNT_noMaskLvADCAlphas_long_integral+
			alpha_COUNT_noMaskLvADCAlphas_long_low_integral+
			alpha_COUNT_noMaskLvADCAlphas_long_high_integral+
			alpha_COUNT_noMaskLvADCAlphas_short_higher_integral+
			alpha_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral;
printf("corrected_alphaCount:%f \n",corrected_alphaCount);

// RooFitAlphasAndFrags(TH1* alphaHistAll, TH1* fragHistAll, TH1* low_adcHistAll);
double corrected_fragCount =
			frag_COUNT_noMaskLvADCAlphas_integral+
			frag_COUNT_noMaskLvADCFrags_integral+
			frag_COUNT_noMaskLvADCFragsUnlikely_integral+
			frag_COUNT_noMaskLvADCAlphas_long_integral+
			frag_COUNT_noMaskLvADCAlphas_long_low_integral+
			frag_COUNT_noMaskLvADCAlphas_long_high_integral+
			frag_COUNT_noMaskLvADCAlphas_short_higher_integral+
			frag_COUNT_noMaskLvADCAlphas_alphaCut_short_low_integral;
printf("corrected_fragCount:%f \n",corrected_fragCount);

double corrected_asf_ratio = isoTopicCalib*2*corrected_alphaCount/(double)corrected_fragCount;
printf("corrected_asf_ratio:%f \n\n\n",corrected_asf_ratio);

int cftIntegral=cftEn->Integral();
printf("cftIntegral: %d\n",cftIntegral);
int cftFragIntegral=cftFragsEn->Integral();
printf("cftFragIntegral: %d\n",cftFragIntegral);
int cftAlphaIntegral=cftAlphasEn->Integral();
printf("cftAlphaIntegral: %d\n",cftAlphaIntegral);
int cftNotFragIntegral=cftIntegral-cftFragIntegral;
printf("cftNotFragIntegral: %d\n",cftNotFragIntegral);
double cftFragsEn_percent_integral=cftFragsEn->Integral()/(double)cftIntegral*100;
printf("cftFragsEn_percent_integral: %.4f\n",cftFragsEn_percent_integral);
double cftFragsUnlikelyEn_percent_integral=cftFragsUnlikelyEn->Integral()/(double)cftIntegral*100;
printf("cftFragsUnlikelyEn_percent_integral: %.4f\n",cftFragsUnlikelyEn_percent_integral);
double cftAlphasEn_percent_integral=cftAlphasEn->Integral()/(double)cftIntegral*100;
printf("cftAlphasEn_percent_integral: %.4f\n",cftAlphasEn_percent_integral);
double cftAlphasEn_long_percent_integral=cftAlphasEn_long->Integral()/(double)cftIntegral*100;
printf("cftAlphasEn_long_percent_integral: %.4f\n",cftAlphasEn_long_percent_integral);
double cftAlphasEn_long_low_percent_integral=cftAlphasEn_long_low->Integral()/(double)cftIntegral*100;
printf("cftAlphasEn_long_low_percent_integral: %.4f\n",cftAlphasEn_long_low_percent_integral);
double cftAlphasEn_long_high_percent_integral=cftAlphasEn_long_high->Integral()/(double)cftIntegral*100;
printf("cftAlphasEn_long_high_percent_integral: %.4f\n",cftAlphasEn_long_high_percent_integral);
double cftAlphasEn_short_higher_percent_integral=cftAlphasEn_short_higher->Integral()/(double)cftIntegral*100;
printf("cftAlphasEn_short_higher_percent_integral: %.4f\n",cftAlphasEn_short_higher_percent_integral);
double cftAlphasEn_alphaCut_short_low_percent_integral=cftAlphasEn_alphaCut_short_low->Integral()/(double)cftIntegral*100;
printf("cftAlphasEn_alphaCut_short_low_percent_integral: %.4f\n",cftAlphasEn_alphaCut_short_low_percent_integral);
printf("cftFragIntegral: %d\n",cftFragIntegral);
printf("cftAlphaIntegral: %d\n",cftAlphaIntegral);
printf("noMaskFragIntegral: %f\n",noMaskFragIntegral);
printf("noMaskAlphaIntegral: %f\n",noMaskAlphaIntegral);
double noMask_aSF_ratio=2*noMaskAlphaIntegral/(double)noMaskFragIntegral*isoTopicCalib;
double cft_aSF_ratio=2*cftAlphaIntegral/(double)cftFragIntegral*isoTopicCalib;
char noMask_aSF_ratioChar[234],cft_aSF_ratioChar[234],
	percent_FragAcceptedChar[234],percent_AlphasRejectedChar[234],
	noMaskFragsEn_percent_char[234],
	noMaskFragsUnlikelyEn_percent_char[234],
	noMaskAlphasEn_percent_char[234],
	noMaskAlphasEn_long_percent_char[234],
	noMaskAlphasEn_long_low_percent_char[234],
	noMaskAlphasEn_long_high_percent_char[234],
	noMaskAlphasEn_short_higher_percent_char[234],
	noMaskAlphasEn_alphaCut_short_low_percent_char[234],
	cftFragsEn_percent_char[234],
	cftFragsUnlikelyEn_percent_char[234],
	cftAlphasEn_percent_char[234],
	cftAlphasEn_long_percent_char[234],
	cftAlphasEn_long_low_percent_char[234],
	cftAlphasEn_long_high_percent_char[234],
	cftAlphasEn_short_higher_percent_char[234],
	cftAlphasEn_alphaCut_short_low_percent_char[234];

TString cftAlphaIntegral_string;
cftAlphaIntegral_string = Form("%5.3g",(double)cftAlphaIntegral);
cftAlphaIntegral_string.ReplaceAll("e+0"," #scale[1.7]{#upoint} 10^{");cftAlphaIntegral_string.Append("} CFT alphas");
TLatex* cftAlphaIntegral_latex = new TLatex(.5,.5, cftAlphaIntegral_string.Data());

TString noMaskAlphaIntegral_string;
noMaskAlphaIntegral_string = Form("%5.3g",(double)noMaskAlphaIntegral);
noMaskAlphaIntegral_string.ReplaceAll("e+0"," #scale[1.7]{#upoint} 10^{");noMaskAlphaIntegral_string.Append("}  noMask alphas");
TLatex* noMaskAlphaIntegral_latex = new TLatex(.5,.5, noMaskAlphaIntegral_string.Data());

TString cftFragIntegral_string;
cftFragIntegral_string = Form("%5.3g",(double)cftFragIntegral);
cftFragIntegral_string.ReplaceAll("e+0"," #scale[1.7]{#upoint} 10^{");cftFragIntegral_string.Append("}  CFT fragments");
TLatex* cftFragIntegral_latex = new TLatex(.5,.5, cftFragIntegral_string.Data());

TString noMaskFragIntegral_string;
noMaskFragIntegral_string = Form("%5.3g",(double)noMaskFragIntegral);
noMaskFragIntegral_string.ReplaceAll("e+0"," #scale[1.7]{#upoint} 10^{");noMaskFragIntegral_string.Append("} noMask Fragments");
TLatex* noMaskFragIntegral_latex = new TLatex(.5,.5, noMaskFragIntegral_string.Data());

sprintf(cft_aSF_ratioChar,"cft_aSF_ratio = %.2f",cft_aSF_ratio);
sprintf(noMask_aSF_ratioChar,"%.3f = alpha/SF ratio",corrected_asf_ratio);
sprintf(percent_FragAcceptedChar,"%.2f%% Fragments Accepted",percent_FragAccepted);
sprintf(percent_AlphasRejectedChar,"%.2f%% Alphas Rejected",percent_AlphasRejected);
sprintf(noMaskFragsEn_percent_char,"%.3f%%",noMaskFragsEn_percent_integral);
sprintf(noMaskFragsUnlikelyEn_percent_char,"%.3f%%",noMaskFragsUnlikelyEn_percent_integral);
sprintf(noMaskAlphasEn_percent_char,"%.3f%%",noMaskAlphasEn_percent_integral);
sprintf(noMaskAlphasEn_long_percent_char,"%.3f%%",noMaskAlphasEn_long_percent_integral);
sprintf(noMaskAlphasEn_long_low_percent_char,"%.3f%%",noMaskAlphasEn_long_low_percent_integral);
sprintf(noMaskAlphasEn_long_high_percent_char,"%.3f%%",noMaskAlphasEn_long_high_percent_integral);
sprintf(noMaskAlphasEn_short_higher_percent_char,"%.3f%%",noMaskAlphasEn_short_higher_percent_integral);
sprintf(noMaskAlphasEn_alphaCut_short_low_percent_char,"%.3f%%",noMaskAlphasEn_alphaCut_short_low_percent_integral);
sprintf(cftFragsEn_percent_char,"%.3f%%",cftFragsEn_percent_integral);
sprintf(cftFragsUnlikelyEn_percent_char,"%.3f%%",cftFragsUnlikelyEn_percent_integral);
sprintf(cftAlphasEn_percent_char,"%.3f%%",cftAlphasEn_percent_integral);
sprintf(cftAlphasEn_long_percent_char,"%.3f%%",cftAlphasEn_long_percent_integral);
sprintf(cftAlphasEn_long_low_percent_char,"%.3f%%",cftAlphasEn_long_low_percent_integral);
sprintf(cftAlphasEn_long_high_percent_char,"%.3f%%",cftAlphasEn_long_high_percent_integral);
sprintf(cftAlphasEn_short_higher_percent_char,"%.3f%%",cftAlphasEn_short_higher_percent_integral);
sprintf(cftAlphasEn_alphaCut_short_low_percent_char,"%.3f%%",cftAlphasEn_alphaCut_short_low_percent_integral);
printf("fragNormalization: %f\n",fragNormalization);
printf("\n\n");
printf("percent_FragAcceptedChar: %s\n",percent_FragAcceptedChar);
printf("percent_AlphasRejectedChar: %s\n",percent_AlphasRejectedChar);
printf("cft_aSF_ratioChar: %s\n",cft_aSF_ratioChar);
printf("noMask_aSF_ratioChar: %s\n",noMask_aSF_ratioChar);
cout<<"cftAlphaIntegral_latex: "<<cftAlphaIntegral_latex<<endl;
cout<<"noMaskAlphaIntegral_latex: "<<noMaskAlphaIntegral_latex<<endl;
cout<<"cftFragIntegral_latex: "<<cftFragIntegral_latex<<endl;
cout<<"noMaskFragIntegral_latex: "<<noMaskFragIntegral_latex<<endl;
double stack_leg_x_start;
if (preamp==1) {stack_leg_x_start = 0;}
if (preamp==2) {stack_leg_x_start=0.71;}
double stack_leg_x_stop = stack_leg_x_start+0.17;

noMaskLvEn->GetYaxis()->SetRangeUser(0,7.5);
cftLvEn->GetYaxis()->SetRangeUser(0,7.5);
////////////////////// fill length-energy & plots color cuts ///////////////////////////////////
printf("////////////////////// noMask fill length-energy & plots color cuts ///////////////////////////////////\n");
TH2D* noFrags_noAlphas_noMaskLvEn = new TH2D("noFrags_noAlphas_noMaskLvEn","noFrags_noAlphas_noMaskLvEn",4000,0,40000*MeV_per_ADC,500,0,10);
noFrags_noAlphas_noMaskLvEn->SetTitle("");
c1->SetLogx(0);c1->SetLogy(0);c1->SetLogz(0);
noFrags_noAlphas_noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
noFrags_noAlphas_noMaskLvEn->GetYaxis()->CenterTitle();
noFrags_noAlphas_noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noFrags_noAlphas_noMaskLvEn->GetXaxis()->CenterTitle();
noFrags_noAlphas_noMaskLvEn->GetXaxis()->SetRangeUser(0,20);
char noAlphas_noFrags_char[567];
sprintf(noAlphas_noFrags_char,"!(%s||%s)&&%s",noMaskAlphaCut_above_zCut_adc_char,noMaskFragCut,noMaskCut);
sprintf(buffChar,"length:adc*%f>>noFrags_noAlphas_noMaskLvEn",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar,noAlphas_noFrags_char);
noFrags_noAlphas_noMaskLvEn->Draw("colz");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
junkCut_energy->Draw("same f");
c1->SaveAs("noAlphas_noFrags_noMaskLvEn.cuts.fullEn.logx.png");
noMaskEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(1);c1->SetLogz(1);
noMaskEn->SetMinimum(0.5);
noMaskEn->Draw();
c1->SaveAs("noMaskEn.fullEn.logx.png");
noMaskEn->GetXaxis()->SetRangeUser(0.4,10);
c1->SetLogx(0);c1->SetLogy(1);
noMaskEn->Draw();
c1->SaveAs("noMaskEn.lowEn.png");
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(0);
noMaskLvEn->SetTitle("noMask Data");
noMaskLvEn->Draw("colz");
c1->SaveAs("noMaskLvEn.fullEn.logx.png");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->SetLineWidth(3);
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->SetLineWidth(3);
fragCut_energy->Draw("same f");
fragCut_energy->Draw("same");
noMaskLvEn->SetTitle("PID bins [noMask Data]");
c1->SaveAs("noMaskLvEn.cuts.fullEn.logx.png");
c1->SetLogx(0);c1->SetLogy(0);
noMaskLvEn->GetYaxis()->SetRangeUser(0,7.5);
noMaskLvEn->SetTitle("noMask Data");
noMaskLvEn->Draw("colz");
c1->SaveAs("noMaskLvEn.fullEn.linx.png");
noMaskLvEn->GetYaxis()->SetRangeUser(0,7.5);
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,10);
noMaskLvEn->Draw("colz");
c1->SaveAs("noMaskLvEn.lowEn.png");
noMaskLvEn->SetTitle("PID bins [noMask Data]");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->Draw("same f");
fragCut_energy->Draw("same");
c1->SaveAs("noMaskLvEn.cuts.lowEn.png");
printf("////////////////////// cft fill length-energy & plots color cuts ///////////////////////////////////\n");
cftEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(1);
cftEn->SetMinimum(0.5);
cftEn->Draw();
c1->SaveAs("cftEn.fullEn.logx.png");
cftEn->GetXaxis()->SetRangeUser(0.4,10);
c1->SetLogx(0);c1->SetLogy(1);
cftEn->Draw();
c1->SaveAs("cftEn.lowEn.png");
cftLvEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(0);
cftLvEn->SetTitle("Fission Trigger Data");
cftLvEn->Draw("colz");
c1->SaveAs("cftLvEn.fullEn.logx.png");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->Draw("same f");
fragCut_energy->Draw("same");
cftLvEn->SetTitle("PID bins [Fission Trigger Data]");
c1->SaveAs("cftLvEn.cuts.fullEn.logx.png");
c1->SetLogx(0);c1->SetLogy(0);
cftLvEn->GetYaxis()->SetRangeUser(0,7.5);
cftLvEn->SetTitle("Fission Trigger Data");
cftLvEn->Draw("colz");
c1->SaveAs("cftLvEn.fullEn.linx.png");
cftLvEn->GetYaxis()->SetRangeUser(0,7.5);
cftLvEn->GetXaxis()->SetRangeUser(0.4,10);
cftLvEn->Draw("colz");
c1->SaveAs("cftLvEn.lowEn.png");
cftLvEn->SetTitle("PID bins [Fission Trigger Data]");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->Draw("same f");
fragCut_energy->Draw("same");
c1->SaveAs("cftLvEn.cuts.lowEn.png");

/////////////////////////////// noMask create colorcoded ADC projection stack ///////////////////////////////////
printf("////////////////////// noMask create colorcoded ADC projection stack ///////////////////////////////////\n");
TLegend* noMask_pidCuts_stack_legend = new TLegend(stack_leg_x_start,0.52,stack_leg_x_stop,0.89);
// noMask_pidCuts_stack_legend->SetTextFont(42);
noMask_pidCuts_stack_legend->SetTextSize(0.04);
noMask_pidCuts_stack_legend->AddEntry(noMaskAlphasEn,noMaskAlphasEn_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskFragsEn,noMaskFragsEn_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskFragsUnlikelyEn,noMaskFragsUnlikelyEn_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskAlphasEn_alphaCut_short_low,noMaskAlphasEn_alphaCut_short_low_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskAlphasEn_short_higher,noMaskAlphasEn_short_higher_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskAlphasEn_long,noMaskAlphasEn_long_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskAlphasEn_long_low,noMaskAlphasEn_long_low_percent_char,"f");
noMask_pidCuts_stack_legend->AddEntry(noMaskAlphasEn_long_high,noMaskAlphasEn_long_high_percent_char,"f");
THStack* noMask_pidCuts_En_Stack = new THStack("noMask_pidCuts_En_Stack","");
noMaskFragsEn->SetFillColorAlpha(kViolet,0.7);
noMaskFragsUnlikelyEn->SetFillColorAlpha(28,0.7);
noMaskAlphasEn->SetFillColorAlpha(kSpring,0.7);
noMaskAlphasEn_long->SetFillColorAlpha(kCyan,0.7);
noMaskAlphasEn_long_low->SetFillColorAlpha(kYellow,0.7);
noMaskAlphasEn_long_high->SetFillColorAlpha(kBlue,0.7);
noMaskAlphasEn_short_higher->SetFillColorAlpha(kOrange+7,0.7);
noMaskAlphasEn_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
noMask_pidCuts_En_Stack->Add(noMaskFragsEn);
noMask_pidCuts_En_Stack->Add(noMaskFragsUnlikelyEn);
noMask_pidCuts_En_Stack->Add(noMaskAlphasEn);
noMask_pidCuts_En_Stack->Add(noMaskAlphasEn_long);
noMask_pidCuts_En_Stack->Add(noMaskAlphasEn_long_low);
noMask_pidCuts_En_Stack->Add(noMaskAlphasEn_long_high);
noMask_pidCuts_En_Stack->Add(noMaskAlphasEn_short_higher);
noMask_pidCuts_En_Stack->Add(noMaskAlphasEn_alphaCut_short_low);
noMask_pidCuts_En_Stack->SetMinimum(0.5);
noMask_pidCuts_En_Stack->SetMaximum(maxY_energy);
noMask_pidCuts_En_Stack->Draw();
c1->SetGrid(1);
c1->SetLogx(0);c1->SetLogy(1);
noMask_pidCuts_En_Stack->GetXaxis()->SetTitle("Track Energy [MeV]");
noMask_pidCuts_En_Stack->GetXaxis()->CenterTitle();
noMask_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,10);
noMask_pidCuts_En_Stack->SetTitle("Stacked Projections");
noMask_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("noMask_pidCuts_En_Stack_logy_lowEn.png");
c1->SetLogx(1);
noMask_pidCuts_En_Stack->Draw();
c1->SetGrid(1);
noMask_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,140);
noMask_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("noMask_pidCuts_En_Stack_logxy_fullEn.png");
/////////////////////////////// cft create colorcoded ADC projection stack ///////////////////////////////////
printf("////////////////////// cft create colorcoded ADC projection stack ///////////////////////////////////\n");
TLegend* cft_pidCuts_stack_legend = new TLegend(stack_leg_x_start,0.52,stack_leg_x_stop,0.89);
// cft_pidCuts_stack_legend->SetTextFont(42);
cft_pidCuts_stack_legend->SetTextSize(0.04);
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn,cftAlphasEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftFragsEn,cftFragsEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftFragsUnlikelyEn,cftFragsUnlikelyEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_alphaCut_short_low,cftAlphasEn_alphaCut_short_low_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_short_higher,cftAlphasEn_short_higher_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_long,cftAlphasEn_long_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_long_low,cftAlphasEn_long_low_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_long_high,cftAlphasEn_long_high_percent_char,"f");
THStack* cft_pidCuts_En_Stack = new THStack("cft_pidCuts_En_Stack","");
cftFragsEn->SetFillColorAlpha(kViolet,0.7);
cftFragsUnlikelyEn->SetFillColorAlpha(28,0.7);
cftAlphasEn->SetFillColorAlpha(kSpring,0.7);
cftAlphasEn_long->SetFillColorAlpha(kCyan,0.7);
cftAlphasEn_long_low->SetFillColorAlpha(kYellow,0.7);
cftAlphasEn_long_high->SetFillColorAlpha(kBlue,0.7);
cftAlphasEn_short_higher->SetFillColorAlpha(kOrange+7,0.7);
cftAlphasEn_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
cft_pidCuts_En_Stack->Add(cftFragsEn);
cft_pidCuts_En_Stack->Add(cftFragsUnlikelyEn);
cft_pidCuts_En_Stack->Add(cftAlphasEn);
cft_pidCuts_En_Stack->Add(cftAlphasEn_long);
cft_pidCuts_En_Stack->Add(cftAlphasEn_long_low);
cft_pidCuts_En_Stack->Add(cftAlphasEn_long_high);
cft_pidCuts_En_Stack->Add(cftAlphasEn_short_higher);
cft_pidCuts_En_Stack->Add(cftAlphasEn_alphaCut_short_low);
cft_pidCuts_En_Stack->SetMaximum(maxY_energy);
cft_pidCuts_En_Stack->SetMinimum(0.5);
cft_pidCuts_En_Stack->Draw();
c1->SetGrid(1);
c1->SetLogx(0);c1->SetLogy(1);
cft_pidCuts_En_Stack->GetXaxis()->SetTitle("Track Energy [MeV]");
cft_pidCuts_En_Stack->GetXaxis()->CenterTitle();
cft_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,10);
cft_pidCuts_En_Stack->SetTitle("Stacked Projections");
cft_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("cft_pidCuts_En_Stack_logy_lowEn.png");
c1->SetLogx(1);
cft_pidCuts_En_Stack->Draw();
c1->SetGrid(1);
cft_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,140);
cft_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("cft_pidCuts_En_Stack_logxy_fullEn.png");

//////////////////// alpha/frag stacks comparison /////////////
printf("\n//////////////////// noMask stacks comparison /////////////\n\n");
TLegend* noMaskEnStack_legend = new TLegend(0.3,0.70,0.86,0.89);
// noMaskEnStack_legend->SetTextFont(42);
noMaskEnStack_legend->SetTextSize(0.04);
noMaskEnStack_legend->AddEntry((TObject*)0,noMask_aSF_ratioChar,"");
noMaskEnStack_legend->AddEntry((TObject*)0,noMaskAlphaIntegral_string.Data(),"");
noMaskEnStack_legend->AddEntry((TObject*)0,noMaskFragIntegral_string.Data(),"");
THStack* noMaskEnStack = new THStack("noMaskEnStack","");
noMaskFragsEn->SetFillColorAlpha(kViolet,0.3);
noMaskFragsEn->SetLineWidth((Width_t)1);
noMaskFragsEn->SetLineColor(kViolet);
noMaskAlphasEn->SetFillColorAlpha(kSpring,0.3);
noMaskAlphasEn->SetLineWidth((Width_t)1);
noMaskAlphasEn->SetLineColor(kSpring);
noMaskEnStack->Add(noMaskFragsEn);
noMaskEnStack->Add(noMaskAlphasEn);
noMaskEnStack->SetMinimum(0.5);
noMaskEnStack->SetMaximum(maxY_energy);
noMaskEnStack->SetTitle("Stacked Projections");
noMaskEnStack->Draw();
c1->SetGrid(1);
noMaskEnStack->GetXaxis()->SetRangeUser(0.4,10);
c1->SetLogx(0);c1->SetLogy(1);
noMaskEnStack->SetTitle("Stacked Energy Projection [noMask Data]");
noMaskEnStack->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskEnStack->GetXaxis()->CenterTitle();
c1->Update();
sprintf(c1PlotName,"%s.preamp%d.noMaskEn.lowEn.logy.png",actinide,preamp);
c1->SaveAs(c1PlotName,"recreate");
noMaskEnStack->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(1);
c1->Update();
sprintf(c1PlotName,"%s.preamp%d.noMaskEn.fullEn.logxy.png",actinide,preamp);
c1->SaveAs(c1PlotName,"recreate");
printf("\n//////////////////// cft stacks comparison /////////////\n\n");
TLegend* cftEnStack_legend = new TLegend(0.3,0.75,0.86,0.89);
// cftEnStack_legend->SetTextFont(42);
cftEnStack_legend->SetTextSize(0.04);
cftEnStack_legend->AddEntry((TObject*)0,cftFragIntegral_string.Data(),"");
cftEnStack_legend->AddEntry((TObject*)0,cftAlphaIntegral_string.Data(),"");
THStack* cftEnStack = new THStack("cftEnStack","");
cftFragsEn->SetFillColorAlpha(kViolet,0.3);
cftFragsEn->SetLineWidth((Width_t)1);
cftFragsEn->SetLineColor(kViolet);
cftAlphasEn->SetFillColorAlpha(kSpring,0.3);
cftAlphasEn->SetLineWidth((Width_t)1);
cftAlphasEn->SetLineColor(kSpring);
cftEnStack->Add(cftFragsEn);
cftEnStack->Add(cftAlphasEn);
cftEnStack->SetMinimum(0.5);
cftEnStack->SetMaximum(maxY_energy);
cftEnStack->SetTitle("Stacked Projections");
cftEnStack->Draw();
c1->SetGrid(1);
c1->SetLogx(0);c1->SetLogy(1);
cftEnStack->GetXaxis()->SetRangeUser(0.4,10);
cftEnStack->SetTitle("Stacked Energy Projection [Fission Trigger]");
cftEnStack->GetXaxis()->SetTitle("Track Energy [MeV]");
cftEnStack->GetXaxis()->CenterTitle();
c1->Update();
sprintf(c1PlotName,"%s.preamp%d.cftEn.lowEn.logy.png",actinide,preamp);
c1->SaveAs(c1PlotName,"recreate");
cftEnStack->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(1);
c1->Update();
sprintf(c1PlotName,"%s.preamp%d.cftEn.fullEn.logxy.png",actinide,preamp);
c1->SaveAs(c1PlotName,"recreate");

///////////////////////  length energy plots  ////////////////////
printf("\n/////////////////////// noMask Lenght VS Energy ////////////////////\n\n");
fragCut_energy->SetFillColorAlpha(kViolet,0.2);
alphaCut_above_zCut_energy->SetFillColorAlpha(kSpring,0.2);
c1->SetLogx(0);c1->SetLogy(0);
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,10);
noMaskLvEn->SetTitle("noMask Data");
noMaskLvEn->Draw("colz");
alphaCut_above_zCut_energy->SetLineWidth(3);
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->SetLineWidth(3);
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
c1->SetLogx(0);
noMaskEnStack_legend->Draw();
c1->SaveAs("noMaskCuts.asf.lowEn.png");
c1->SetLogx(1);
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,140);
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
c1->SaveAs("noMaskCuts.asf.fullEn.logxy.png");
printf("\n/////////////////////// cft Lenght VS Energy ////////////////////\n\n");
fragCut_energy->SetFillColorAlpha(kViolet,0.2);
alphaCut_above_zCut_energy->SetFillColorAlpha(kSpring,0.2);
c1->SetLogx(0);c1->SetLogy(0);
cftLvEn->GetXaxis()->SetRangeUser(0.4,10);
cftLvEn->SetTitle("Fission Trigger Data");
cftLvEn->Draw("colz");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
c1->SetLogx(0);
cftEnStack_legend->Draw();
c1->SaveAs("cftCuts.asf.lowEn.png");
c1->SetLogx(1);
cftLvEn->GetXaxis()->SetRangeUser(0.4,140);
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
c1->SaveAs("cftCuts.asf.fullEn.logxy.png");

/////////////////////// efficiency ////////////////////
printf("\n/////////////////////// efficiency ////////////////////\n\n");
TH1D* cftEfficiency = (TH1D*)cftFragsEn->Clone("cftEfficiency");
cftEfficiency->SetTitle("");
cftEfficiency->SetLineColor(kMagenta);
cftEfficiency->SetLineWidth(1);
cftEfficiency->SetFillColorAlpha(kMagenta,0);
cftEfficiency->SetTitle("cftEfficiency = cft / noMask");
cftEfficiency->Divide(noMaskFragsEn);
noMaskFragsEn->SetLineColor(kRed);
noMaskFragsEn->SetLineWidth(1);
noMaskFragsEn->SetFillColorAlpha(kViolet,0);
cftFragsEn->SetLineColor(kBlue);
cftFragsEn->SetLineWidth(1);
cftFragsEn->SetFillColorAlpha(kViolet,0);
// for plotting the scaled region
TH1D* cftFragsEnScaleRegion = (TH1D*)cftFragsEn->Clone();
cftFragsEnScaleRegion->SetTitle("");
minNormADC_bin=cftFragsEn->FindBin(minNormEnergy);
for (int i = 0; i < minNormADC_bin; ++i) cftFragsEnScaleRegion->SetBinContent(i,0);
cftFragsEnScaleRegion->SetFillColorAlpha(kBlue,0.2);
cftFragsEnScaleRegion->SetLineColor(kBlue);
cftFragsEnScaleRegion->SetLineWidth((Width_t)0.01);
//////////////  find y max for plots
int start_bin=0;
double maxY_frags_low_energy=0, maxY_frags_full_energy=0, dummyDub=0;
start_bin=noMaskFragsEn->GetBinContent(noMaskFragsEn->FindFirstBinAbove(0));
for (size_t i = start_bin; i < start_bin+200; i++) {
	dummyDub=noMaskFragsEn->GetBinContent(i);
	if ( dummyDub > maxY_frags_low_energy) maxY_frags_low_energy = dummyDub;
}
maxY_frags_low_energy=1.25*maxY_frags_low_energy;
for (size_t i = 500; i < 1500; i+=50) {
	dummyDub=noMaskFragsEn->GetBinContent(i);
	if ( dummyDub > maxY_frags_low_energy) maxY_frags_full_energy = dummyDub;
}
maxY_frags_full_energy=100*maxY_frags_full_energy;
printf("maxY_frags_full_energy:%.0f\n", maxY_frags_full_energy);
TLegend* legend_normalization=new TLegend(0.08,0.68,0.89,0.89);
// legend_normalization->SetTextFont(42);
legend_normalization->SetTextSize(0.037);
legend_normalization->SetNColumns(2);
legend_normalization->AddEntry(cftFragsEnScaleRegion,"normalization","f");
legend_normalization->AddEntry((TObject*)0,percent_AlphasRejectedChar, "");
legend_normalization->AddEntry(noMaskFragsEn,"noMask Fragments");
legend_normalization->AddEntry((TObject*)0,cftFragIntegral_string.Data(),"");
legend_normalization->AddEntry(cftFragsEn,"CFT Fragments");
legend_normalization->AddEntry((TObject*)0,noMaskFragIntegral_string.Data(),"");
legend_normalization->AddEntry(cftEfficiency,"CFT/noMask");
legend_normalization->AddEntry((TObject*)0,percent_FragAcceptedChar, "");
c1->SetLogx(0);c1->SetLogy(1);
noMaskFragsEn->SetMaximum(maxY_frags_full_energy);
noMaskFragsEn->SetMinimum(0.1);
noMaskFragsEn->GetXaxis()->SetRangeUser(0,130);
cftFragsEn->SetMaximum(maxY_frags_full_energy);
cftFragsEn->SetMinimum(0.1);
cftFragsEn->GetXaxis()->SetRangeUser(0,130);
cftEfficiency->SetMaximum(maxY_frags_full_energy);
cftEfficiency->SetMinimum(0.1);
cftEfficiency->GetXaxis()->SetRangeUser(0,130);
cftFragsEnScaleRegion->SetMaximum(maxY_frags_full_energy);
cftFragsEnScaleRegion->SetMinimum(0.1);
cftFragsEnScaleRegion->GetXaxis()->SetRangeUser(0,130);
c1->Clear();
noMaskFragsEn->SetTitle("Fission Trigger Efficiency");
noMaskFragsEn->Draw();
cftFragsEn->Draw("same");
cftEfficiency->Draw("same");
cftFragsEnScaleRegion->Draw("same");
legend_normalization->Draw();
sprintf(c1PlotName,"%s.preamp%d.bothFragsCut.fullEn.logy.png",actinide,preamp);
c1->SaveAs(c1PlotName,"recreate");
cftFragsEnScaleRegion->GetXaxis()->SetRangeUser(1,25);
cftFragsEnScaleRegion->GetYaxis()->SetRangeUser(0,maxY_frags_low_energy);
noMaskFragsEn->GetXaxis()->SetRangeUser(1,25);
noMaskFragsEn->GetYaxis()->SetRangeUser(0,maxY_frags_low_energy);
cftFragsEn->GetXaxis()->SetRangeUser(1,25);
cftFragsEn->GetYaxis()->SetRangeUser(0,maxY_frags_low_energy);
cftEfficiency->GetXaxis()->SetRangeUser(1,25);
cftEfficiency->GetYaxis()->SetRangeUser(0,maxY_frags_low_energy);
c1->SetLogx(0);c1->SetLogy(0);c1->Update();
noMaskFragsEn->SetTitle("");
noMaskFragsEn->Draw();
cftFragsEn->Draw("same");
cftFragsEnScaleRegion->Draw("same");
// cftEfficiency->Draw("same");
sprintf(c1PlotName,"%s.preamp%d.bothFragsCut.lowEn.logy.png",actinide,preamp);
c1->SaveAs(c1PlotName,"recreate");

////////////////////// create polar histograms ///////////////////////////////////
printf("////////////////////// create polar histograms ///////////////////////////////////\n");
noMaskFragsEn->SetLineColor(kViolet);
noMaskFragsUnlikelyEn->SetLineColor(kViolet);
noMaskAlphasEn->SetLineColor(kSpring);
noMaskAlphasEn_long->SetLineColor(kCyan);
noMaskAlphasEn_long_low->SetLineColor(kYellow);
noMaskAlphasEn_long_high->SetLineColor(kBlue);
noMaskAlphasEn_short_higher->SetLineColor(kOrange+7);
noMaskAlphasEn_alphaCut_short_low->SetLineColor(kRed);
cftFragsEn->SetLineColor(kViolet);
cftFragsUnlikelyEn->SetLineColor(kViolet);
cftAlphasEn->SetLineColor(kSpring);
cftAlphasEn_long->SetLineColor(kCyan);
cftAlphasEn_long_low->SetLineColor(kYellow);
cftAlphasEn_long_high->SetLineColor(kBlue);
cftAlphasEn_short_higher->SetLineColor(kOrange+7);
cftAlphasEn_alphaCut_short_low->SetLineColor(kRed);
TH1D* noMaskCosTheta = new TH1D("noMaskCosTheta","noMaskCosTheta",500,-1,1);
noMaskCosTheta->SetTitle("");
TH1D* noMaskCosThetaFrags = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaFrags");
noMaskCosThetaFrags->SetTitle("");
noMaskCosThetaFrags->SetLineColor(kViolet);
noMaskCosThetaFrags->SetLineWidth((Width_t)0.01);
noMaskCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
TH1D* noMaskCosThetaFragsUnlikely = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaFragsUnlikely");
noMaskCosThetaFragsUnlikely->SetTitle("");
noMaskCosThetaFragsUnlikely->SetLineColor(28);
noMaskCosThetaFragsUnlikely->SetLineWidth((Width_t)0.01);
noMaskCosThetaFragsUnlikely->SetFillColorAlpha(28,0.7);
TH1D* noMaskCosThetaAlphas = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas");
noMaskCosThetaAlphas->SetTitle("");
noMaskCosThetaAlphas->SetLineColor(kSpring);
noMaskCosThetaAlphas->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas->SetFillColorAlpha(kSpring,0.7);
TH1D* noMaskCosThetaAlphas_long = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_long");
noMaskCosThetaAlphas_long->SetTitle("");
noMaskCosThetaAlphas_long->SetLineColor(kCyan);
noMaskCosThetaAlphas_long->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_long->SetFillColorAlpha(kCyan,0.7);
TH1D* noMaskCosThetaAlphas_long_low = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_long_low");
noMaskCosThetaAlphas_long_low->SetTitle("");
noMaskCosThetaAlphas_long_low->SetLineColor(kYellow);
noMaskCosThetaAlphas_long_low->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_long_low->SetFillColorAlpha(kYellow,0.7);
TH1D* noMaskCosThetaAlphas_long_high = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_long_high");
noMaskCosThetaAlphas_long_high->SetTitle("");
noMaskCosThetaAlphas_long_high->SetLineColor(kBlue);
noMaskCosThetaAlphas_long_high->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_long_high->SetFillColorAlpha(kBlue,0.7);
TH1D* noMaskCosThetaAlphas_short_higher = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_short_higher");
noMaskCosThetaAlphas_short_higher->SetTitle("");
noMaskCosThetaAlphas_short_higher->SetLineColor(kOrange+7);
noMaskCosThetaAlphas_short_higher->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_short_higher->SetFillColorAlpha(kOrange+7,0.7);
TH1D* noMaskCosThetaAlphas_alphaCut_short_low = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_alphaCut_short_low");
noMaskCosThetaAlphas_alphaCut_short_low->SetTitle("");
noMaskCosThetaAlphas_alphaCut_short_low->SetLineColor(kRed);
noMaskCosThetaAlphas_alphaCut_short_low->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
TH1D* cftCosTheta = new TH1D("cftCosTheta","cftCosTheta",500,-1,1);
cftCosTheta->SetTitle("");
TH1D* cftCosThetaFrags = (TH1D*)cftCosTheta->Clone("cftCosThetaFrags");
cftCosThetaFrags->SetTitle("");
cftCosThetaFrags->SetLineColor(kViolet);
cftCosThetaFrags->SetLineWidth((Width_t)0.01);
cftCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
TH1D* cftCosThetaFragsUnlikely = (TH1D*)cftCosTheta->Clone("cftCosThetaFragsUnlikely");
cftCosThetaFragsUnlikely->SetTitle("");
cftCosThetaFragsUnlikely->SetLineColor(28);
cftCosThetaFragsUnlikely->SetLineWidth((Width_t)0.01);
cftCosThetaFragsUnlikely->SetFillColorAlpha(28,0.7);
TH1D* cftCosThetaAlphas = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas");
cftCosThetaAlphas->SetTitle("");
cftCosThetaAlphas->SetLineColor(kSpring);
cftCosThetaAlphas->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas->SetFillColorAlpha(kSpring,0.7);
TH1D* cftCosThetaAlphas_long = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_long");
cftCosThetaAlphas_long->SetTitle("");
cftCosThetaAlphas_long->SetLineColor(kCyan);
cftCosThetaAlphas_long->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_long->SetFillColorAlpha(kCyan,0.7);
TH1D* cftCosThetaAlphas_long_low = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_long_low");
cftCosThetaAlphas_long_low->SetTitle("");
cftCosThetaAlphas_long_low->SetLineColor(kYellow);
cftCosThetaAlphas_long_low->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_long_low->SetFillColorAlpha(kYellow,0.7);
TH1D* cftCosThetaAlphas_long_high = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_long_high");
cftCosThetaAlphas_long_high->SetTitle("");
cftCosThetaAlphas_long_high->SetLineColor(kBlue);
cftCosThetaAlphas_long_high->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_long_high->SetFillColorAlpha(kBlue,0.7);
TH1D* cftCosThetaAlphas_short_higher = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_short_higher");
cftCosThetaAlphas_short_higher->SetTitle("");
cftCosThetaAlphas_short_higher->SetLineColor(kOrange+7);
cftCosThetaAlphas_short_higher->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_short_higher->SetFillColorAlpha(kOrange+7,0.7);
TH1D* cftCosThetaAlphas_alphaCut_short_low = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_alphaCut_short_low");
cftCosThetaAlphas_alphaCut_short_low->SetTitle("");
cftCosThetaAlphas_alphaCut_short_low->SetLineColor(kRed);
cftCosThetaAlphas_alphaCut_short_low->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);

////////////////////// fill polar histograms ///////////////////////////////////
printf("////////////////////// fill polar histograms ///////////////////////////////////\n");
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosTheta",noMaskCut);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaAlphas",noMaskAlphaCut_above_zCut_adc_char);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaFrags",noMaskFragCut);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaFragsUnlikely",noMaskFragUnlikelyCut_char);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaAlphas_long",noMaskAlphaCut_long_char);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaAlphas_long_low",noMaskAlphaCut_long_low_char);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaAlphas_long_high",noMaskAlphaCut_long_high_adc_char);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaAlphas_short_higher",noMaskAlphaCut_short_higher_adc_char);
noMaskTracks->Draw("direction.CosTheta()>>noMaskCosThetaAlphas_alphaCut_short_low",noMaskAlphaCut_alphaCut_short_low_adc_char);
cftTracks->Draw("direction.CosTheta()>>cftCosTheta",cftCut);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas",cftAlphaCut_above_zCut_adc_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaFrags",cftFragCut);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaFragsUnlikely",cftFragUnlikelyCut_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_long",cftAlphaCut_long_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);
for (size_t i = 0; i < cftCosTheta->GetNbinsX(); i++) {
	cftCosTheta->SetBinContent(i,fragNormalization*cftCosTheta->GetBinContent(i));
	cftCosThetaAlphas->SetBinContent(i,fragNormalization*cftCosThetaAlphas->GetBinContent(i));
	cftCosThetaFrags->SetBinContent(i,fragNormalization*cftCosThetaFrags->GetBinContent(i));
	cftCosThetaFragsUnlikely->SetBinContent(i,fragNormalization*cftCosThetaFragsUnlikely->GetBinContent(i));
	cftCosThetaAlphas_long->SetBinContent(i,fragNormalization*cftCosThetaAlphas_long->GetBinContent(i));
	cftCosThetaAlphas_long_low->SetBinContent(i,fragNormalization*cftCosThetaAlphas_long_low->GetBinContent(i));
	cftCosThetaAlphas_long_high->SetBinContent(i,fragNormalization*cftCosThetaAlphas_long_high->GetBinContent(i));
	cftCosThetaAlphas_short_higher->SetBinContent(i,fragNormalization*cftCosThetaAlphas_short_higher->GetBinContent(i));
	cftCosThetaAlphas_alphaCut_short_low->SetBinContent(i,fragNormalization*cftCosThetaAlphas_alphaCut_short_low->GetBinContent(i));
}
noMaskCosThetaFrags->Draw();c1->SaveAs("noMaskCosThetaFrags.png");
noMaskCosThetaFragsUnlikely->Draw();c1->SaveAs("noMaskCosThetaFragsUnlikely.png");
noMaskCosThetaAlphas->Draw();c1->SaveAs("noMaskCosThetaAlphas.png");
noMaskCosThetaAlphas_long->Draw();c1->SaveAs("noMaskCosThetaAlphas_long.png");
noMaskCosThetaAlphas_long_low->Draw();c1->SaveAs("noMaskCosThetaAlphas_long_low.png");
noMaskCosThetaAlphas_long_high->Draw();c1->SaveAs("noMaskCosThetaAlphas_long_high.png");
noMaskCosThetaAlphas_short_higher->Draw();c1->SaveAs("noMaskCosThetaAlphas_short_higher.png");
noMaskCosThetaAlphas_alphaCut_short_low->Draw();c1->SaveAs("noMaskCosThetaAlphas_alphaCut_short_low.png");
cftCosTheta->Draw();c1->SaveAs("cftCosTheta.png");
cftCosThetaFrags->Draw();c1->SaveAs("cftCosThetaFrags.png");
cftCosThetaFragsUnlikely->Draw();c1->SaveAs("cftCosThetaFragsUnlikely.png");
cftCosThetaAlphas->Draw();c1->SaveAs("cftCosThetaAlphas.png");
cftCosThetaAlphas_long->Draw();c1->SaveAs("cftCosThetaAlphas_long.png");
cftCosThetaAlphas_long_low->Draw();c1->SaveAs("cftCosThetaAlphas_long_low.png");
cftCosThetaAlphas_long_high->Draw();c1->SaveAs("cftCosThetaAlphas_long_high.png");
cftCosThetaAlphas_short_higher->Draw();c1->SaveAs("cftCosThetaAlphas_short_higher.png");
cftCosThetaAlphas_alphaCut_short_low->Draw();c1->SaveAs("cftCosThetaAlphas_alphaCut_short_low.png");
/////////////////////////////// colorcoded polar projection stack ///////////////////////////////////
printf("////////////////////// colorcoded polar projection stack ///////////////////////////////////\n");
//////////////  find y max for plots
double maxY_cos=0;
for (size_t i = 0; i < 50; i++) {
	dummyDub=noMaskCosTheta->GetBinContent(i);
	if ( dummyDub > maxY_cos) maxY_cos = dummyDub;
}
maxY_cos=10*maxY_cos;
c1->SetLogx(0);c1->SetLogy(0);
TLegend* noMask_cosTheta_stack_legend = new TLegend(stack_leg_x_start,0.52,stack_leg_x_stop,0.89);
// noMask_cosTheta_stack_legend->SetTextFont(42);
noMask_cosTheta_stack_legend->SetTextSize(0.04);
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaAlphas_alphaCut_short_low,noMaskAlphasEn_alphaCut_short_low_percent_char,"f");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaAlphas_short_higher,noMaskAlphasEn_short_higher_percent_char,"f");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaAlphas_long,noMaskAlphasEn_long_percent_char,"f");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaAlphas_long_low,noMaskAlphasEn_long_low_percent_char,"f");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaAlphas_long_high,noMaskAlphasEn_long_high_percent_char,"f");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaFragsUnlikely,noMaskFragsUnlikelyEn_percent_char,"f");
THStack* noMask_cosTheta_Stack = new THStack("noMask_cosTheta_Stack","");
noMaskCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
noMaskCosThetaFragsUnlikely->SetFillColorAlpha(28,0.7);
noMaskCosThetaAlphas->SetFillColorAlpha(kSpring,0.7);
noMaskCosThetaAlphas_long->SetFillColorAlpha(kCyan,0.7);
noMaskCosThetaAlphas_long_low->SetFillColorAlpha(kYellow,0.7);
noMaskCosThetaAlphas_long_high->SetFillColorAlpha(kBlue,0.7);
noMaskCosThetaAlphas_short_higher->SetFillColorAlpha(kOrange+7,0.7);
noMaskCosThetaAlphas_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
noMask_cosTheta_Stack->Add(noMaskCosThetaAlphas_long);
noMask_cosTheta_Stack->Add(noMaskCosThetaAlphas_long_low);
noMask_cosTheta_Stack->Add(noMaskCosThetaAlphas_long_high);
noMask_cosTheta_Stack->Add(noMaskCosThetaAlphas_short_higher);
noMask_cosTheta_Stack->Add(noMaskCosThetaAlphas_alphaCut_short_low);
noMask_cosTheta_Stack->Add(noMaskCosThetaFragsUnlikely);
noMask_cosTheta_Stack->Draw();
c1->SetGrid(1);
noMask_cosTheta_Stack->GetXaxis()->SetTitle("cos #theta");
c1->Update();
noMask_cosTheta_Stack->SetTitle("Off Band Stacked cos#theta PID Bin Projections [noMask Data]");
noMask_cosTheta_stack_legend->Draw();
c1->Update();
c1->SaveAs("noMask_NOalpha_NOfrag_cosTheta_Stack.png");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaAlphas,noMaskAlphasEn_percent_char,"f");
noMask_cosTheta_stack_legend->AddEntry(noMaskCosThetaFrags,noMaskFragsEn_percent_char,"f");
noMask_cosTheta_Stack->Add(noMaskCosThetaFrags);
noMask_cosTheta_Stack->Add(noMaskCosThetaAlphas);
noMask_cosTheta_Stack->SetMinimum(0.5);
noMask_cosTheta_Stack->SetMaximum(maxY_cos);
noMask_cosTheta_Stack->Draw();
c1->SetGrid(1);
c1->SetLogy(1);
noMask_cosTheta_stack_legend->Draw();
c1->Update();
c1->SaveAs("noMask_cosTheta_Stack.png");
c1->SetLogx(0);c1->SetLogy(0);
TLegend* cft_cosTheta_stack_legend = new TLegend(stack_leg_x_start,0.52,stack_leg_x_stop,0.89);
// cft_cosTheta_stack_legend->SetTextFont(42);
cft_cosTheta_stack_legend->SetTextSize(0.04);
cftCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
cftCosThetaFragsUnlikely->SetFillColorAlpha(28,0.7);
cftCosThetaAlphas->SetFillColorAlpha(kSpring,0.7);
cftCosThetaAlphas_long->SetFillColorAlpha(kCyan,0.7);
cftCosThetaAlphas_long_low->SetFillColorAlpha(kYellow,0.7);
cftCosThetaAlphas_long_high->SetFillColorAlpha(kBlue,0.7);
cftCosThetaAlphas_short_higher->SetFillColorAlpha(kOrange+7,0.7);
cftCosThetaAlphas_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas_alphaCut_short_low,cftAlphasEn_alphaCut_short_low_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas_short_higher,cftAlphasEn_short_higher_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas_long,cftAlphasEn_long_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas_long_low,cftAlphasEn_long_low_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas_long_high,cftAlphasEn_long_high_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaFragsUnlikely,cftFragsUnlikelyEn_percent_char,"f");
THStack* cft_cosTheta_Stack = new THStack("cft_cosTheta_Stack","");
cft_cosTheta_Stack->Add(cftCosThetaAlphas_long);
cft_cosTheta_Stack->Add(cftCosThetaAlphas_long_low);
cft_cosTheta_Stack->Add(cftCosThetaAlphas_long_high);
cft_cosTheta_Stack->Add(cftCosThetaAlphas_short_higher);
cft_cosTheta_Stack->Add(cftCosThetaAlphas_alphaCut_short_low);
cft_cosTheta_Stack->Add(cftCosThetaFragsUnlikely);
cft_cosTheta_Stack->Draw();
c1->SetGrid(1);
cft_cosTheta_Stack->SetTitle("Off Band Stacked cos#theta PID Bin Projections [Fission Trigger Data]");
cft_cosTheta_stack_legend->Draw();
cft_cosTheta_Stack->GetXaxis()->SetTitle("cos #theta");
c1->Update();
c1->SaveAs("cft_NOalpha_NOfrag_cosTheta_Stack.png");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas,cftAlphasEn_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaFrags,cftFragsEn_percent_char,"f");
cft_cosTheta_Stack->Add(cftCosThetaFrags);
cft_cosTheta_Stack->Add(cftCosThetaAlphas);
cft_cosTheta_Stack->SetMinimum(0.5);
cft_cosTheta_Stack->SetMaximum(maxY_cos);
cft_cosTheta_Stack->Draw();
c1->SetGrid(1);
c1->SetLogy(1);
cft_cosTheta_stack_legend->Draw();
cft_cosTheta_Stack->SetTitle("cft_cosTheta_Stack");
c1->Update();
c1->SaveAs("cft_cosTheta_Stack.png");

///////////////////////  write histograms ////////////////////
printf("\n/////////////////////// write histograms ////////////////////\n\n");
TFile* theseHists = new TFile("cftAnaPlots.root","create");

///////////////////////  write TCutG ///////////////////
fragCut_adc->Write();
alphaCut_above_zCut_adc->Write();
alphaCut_long_low_adc->Write();
alphaCut_long_adc->Write();
long_high_adc->Write();
alphaCut_short_higher_adc->Write();
alphaCut_short_low_adc->Write();
fragCut_energy->Write();
alphaCut_above_zCut_energy->Write();
alphaCut_long_low_energy->Write();
alphaCut_long_energy->Write();
long_high_energy->Write();
alphaCut_short_higher_energy->Write();
alphaCut_short_low_energy->Write();

///////////////////////  write TH2D ////////////////////
noMaskLvEn_noCuts->Write();
Length_vs_cosTheta_noCuts->Write();
Energy_vs_cosTheta_noCuts->Write();
Energy_vs_Azimuth_noCuts->Write();
noMaskLvEn_wCuts->Write();
Length_vs_cosTheta_wCuts->Write();
Energy_vs_cosTheta_wCuts->Write();
Energy_vs_Azimuth_wCuts->Write();
noMaskLvADC->Write();
noMaskLvADCFrags->Write();
noMaskLvADCFragsUnlikely->Write();
noMaskLvADCAlphas->Write();
noMaskLvADCAlphas_long->Write();
noMaskLvADCAlphas_long_low->Write();
noMaskLvADCAlphas_long_high->Write();
noMaskLvADCAlphas_short_higher->Write();
noMaskLvADCAlphas_alphaCut_short_low->Write();
noMaskLvADC_checkMutualExclusivity->Write();
cftLvADC->Write();
cftLvADCFrags->Write();
cftLvADCFragsUnlikely->Write();
cftLvADCAlphas->Write();
cftLvADCAlphas_long->Write();
cftLvADCAlphas_long_low->Write();
cftLvADCAlphas_long_high->Write();
cftLvADCAlphas_short_higher->Write();
cftLvADCAlphas_alphaCut_short_low->Write();
noMaskLvEn->Write();
noMaskLvEnFrags->Write();
noMaskLvEnFragsUnlikely->Write();
noMaskLvEnAlphas->Write();
noMaskLvEnAlphas_long->Write();
noMaskLvEnAlphas_long_low->Write();
noMaskLvEnAlphas_long_high->Write();
noMaskLvEnAlphas_short_higher->Write();
noMaskLvEnAlphas_alphaCut_short_low->Write();
cftLvEn->Write();
cftLvEnFrags->Write();
cftLvEnFragsUnlikely->Write();
cftLvEnAlphas->Write();
cftLvEnAlphas_long->Write();
cftLvEnAlphas_long_low->Write();
cftLvEnAlphas_long_high->Write();
cftLvEnAlphas_short_higher->Write();
cftLvEnAlphas_alphaCut_short_low->Write();
cftLvEn_NOT_norm->Write();
noMaskLvEn_NOT_norm->Write();
noFrags_noAlphas_noMaskLvEn->Write();

///////////////////////  write TH1D ////////////////////
noMaskADC->Write();
noMaskFragsADC->Write();
noMaskFragsUnlikeyADC->Write();
noMaskAlphasADC->Write();
noMaskAlphasADC_long->Write();
noMaskAlphasADC_long_low->Write();
noMaskAlphasADC_long_high->Write();
noMaskAlphasADC_short_higher->Write();
noMaskAlphasADC_alphaCut_short_low->Write();
cftADC->Write();
cftFragsADC->Write();
cftFragsUnlikeyADC->Write();
cftAlphasADC->Write();
cftAlphasADC_long->Write();
cftAlphasADC_long_low->Write();
cftAlphasADC_long_high->Write();
cftAlphasADC_short_higher->Write();
cftAlphasADC_alphaCut_short_low->Write();
noMaskFragsEn_NOT_norm->Write();
cftFragsEn_NOT_norm->Write();
cftFragsEn_normalized->Write();
cftFragsEn_ScaleRegion->Write();
noMaskEn->Write();
noMaskFragsEn->Write();
noMaskFragsUnlikelyEn->Write();
noMaskAlphasEn->Write();
noMaskAlphasEn_long->Write();
noMaskAlphasEn_long_low->Write();
noMaskAlphasEn_long_high->Write();
noMaskAlphasEn_short_higher->Write();
noMaskAlphasEn_alphaCut_short_low->Write();
cftEn->Write();
cftFragsEn->Write();
cftFragsUnlikelyEn->Write();
cftAlphasEn->Write();
cftAlphasEn_long->Write();
cftAlphasEn_long_low->Write();
cftAlphasEn_long_high->Write();
cftAlphasEn_short_higher->Write();
cftAlphasEn_alphaCut_short_low->Write();
cftEfficiency->Write();
cftFragsEnScaleRegion->Write();
noMaskCosTheta->Write();
noMaskCosThetaFrags->Write();
noMaskCosThetaFragsUnlikely->Write();
noMaskCosThetaAlphas->Write();
noMaskCosThetaAlphas_long->Write();
noMaskCosThetaAlphas_long_low->Write();
noMaskCosThetaAlphas_long_high->Write();
noMaskCosThetaAlphas_short_higher->Write();
noMaskCosThetaAlphas_alphaCut_short_low->Write();
cftCosTheta->Write();
cftCosThetaFrags->Write();
cftCosThetaFragsUnlikely->Write();
cftCosThetaAlphas->Write();
cftCosThetaAlphas_long->Write();
cftCosThetaAlphas_long_low->Write();
cftCosThetaAlphas_long_high->Write();
cftCosThetaAlphas_short_higher->Write();
cftCosThetaAlphas_alphaCut_short_low->Write();

///////////////////////  write THStack ///////////////////
noMask_pidCuts_En_Stack->Write();
cft_pidCuts_En_Stack->Write();
noMaskEnStack->Write();
cftEnStack->Write();
noMask_cosTheta_Stack->Write();
cft_cosTheta_Stack->Write();

return;
}
