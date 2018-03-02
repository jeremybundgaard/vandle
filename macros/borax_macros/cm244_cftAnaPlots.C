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

void cm244_cftAnaPlots(const int preamp, const char* actinide, const char* pidFileName="", const char* noMaskFileName="", const char* cftFileName="", const char* otherTreeCuts="1",const char* driftVel="",const char* addGuide="") {

// gStyle->SetOptTitle(0);
gStyle->SetOptStat(0);
gStyle->SetPalette(57);
gStyle->SetNumberContours(99);
gStyle->SetTitleY(.98);
gStyle->SetTitleFontSize(1.4*gStyle->GetTitleFontSize());
gStyle->SetTitleXSize(1.3*gStyle->GetTitleXSize());
gStyle->SetTitleYOffset(0.7);
gStyle->SetTitleYSize(1.3*gStyle->GetTitleYSize());
TGaxis::SetMaxDigits(3);

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
double isoTopicCalib=0.9186;
double pi=4*atan(1);
double deg2rad = pi/(double)180;
char c1PlotName[234]="";
char buffChar[345]="";
char noMaskCut[234]="";
char cftCut[234]="";
char noMaskRunCut[234]="";
char cftRunCut[234]="";
char azCut[234]="";
char noMaskDigitFileName[234]="";
char cftDigitFileName[234]="";
char pidFile_char[345]="";
double stack_leg_x_start=0,stack_leg_x_stop=0;

if ( !(strcmp(actinide,"cm244"))){
	if (preamp==1){
		ADC_per_MeV=1214/(double)5.800;
		MeV_per_ADC=5.800/(double)1214;
		stack_leg_x_start=0.71;
		sprintf(noMaskRunCut,"run==100001995");
		sprintf(noMaskCut," %s",noMaskRunCut);
		sprintf(cftCut,"1");
	}
}
stack_leg_x_stop = stack_leg_x_start+0.14;


////////////////////// initialize cuts from pid file ///////////////////////////////////
printf("////////////////////// initialize cuts from pid file:%s ///////////////////////////////////\n",pidFileName);
/////////  create TCutG chars ///////////
char fragCut_adc_char[345];
char highEn_fragCut_adc_char[345];
char junkCut_adc_char[345];
char alphaCut_adc_char[345];
char alpha_short_high_adc_char[345];
char alpha_short_low_adc_char[345];
char above_zCut_adc_char[345];
sprintf(junkCut_adc_char,"junkCut_adc");
sprintf(fragCut_adc_char,"fragCut_adc");
sprintf(highEn_fragCut_adc_char,"highEn_fragCut_adc");
sprintf(alphaCut_adc_char,"alphaCut_adc");
sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
sprintf(above_zCut_adc_char,"alphaCut_zCut_adc");
char fragCut_energy_char[345];
char highEn_fragCut_energy_char[345];
char junkCut_energy_char[345];
char alphaCut_energy_char[345];
char alpha_short_high_energy_char[345];
char alpha_short_low_energy_char[345];
char above_zCut_energy_char[345];
sprintf(junkCut_energy_char,"junkCut_energy");
sprintf(fragCut_energy_char,"fragCut_energy");
sprintf(highEn_fragCut_energy_char,"highEn_fragCut_energy");
sprintf(alphaCut_energy_char,"alphaCut_energy");
sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
sprintf(above_zCut_energy_char,"alphaCut_zCut_energy");
// fragCut_adc is crucial for invoking this cut in ->Draw()
TFile* pidFile = new TFile(pidFileName);
TCutG* fragCut_adc = (TCutG*)pidFile->Get(fragCut_adc_char);
fragCut_adc->SetName("fragCut_adc");
fragCut_adc->SetLineColor(kViolet);
fragCut_adc->SetLineWidth(1);
fragCut_adc->SetFillColorAlpha(kViolet,0.3);
TCutG* highEn_fragCut_adc = (TCutG*)pidFile->Get(highEn_fragCut_adc_char);
highEn_fragCut_adc->SetName("highEn_fragCut_adc");
highEn_fragCut_adc->SetLineColor(kViolet);
highEn_fragCut_adc->SetLineWidth(1);
highEn_fragCut_adc->SetFillColorAlpha(kViolet,0.3);
TCutG* junkCut_adc = (TCutG*)pidFile->Get(junkCut_adc_char);
junkCut_adc->SetName("junkCut_adc");
junkCut_adc->SetLineColor(28);
junkCut_adc->SetLineWidth(1);
junkCut_adc->SetFillColorAlpha(28,0.3);
TCutG* alphaCut_above_zCut_adc = (TCutG*)pidFile->Get(above_zCut_adc_char);
alphaCut_above_zCut_adc->SetName("alphaCut_above_zCut_adc");
alphaCut_above_zCut_adc->SetLineColor(kSpring);
alphaCut_above_zCut_adc->SetLineWidth(2);
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
fragCut_energy->SetLineWidth(1);
fragCut_energy->SetFillColorAlpha(kViolet,0.3);
TCutG* highEn_fragCut_energy = (TCutG*)pidFile->Get(highEn_fragCut_energy_char);
highEn_fragCut_energy->SetName("fragCut_energy");
highEn_fragCut_energy->SetLineColor(kViolet);
highEn_fragCut_energy->SetLineWidth(1);
highEn_fragCut_energy->SetFillColorAlpha(kViolet,0.3);
TCutG* junkCut_energy = (TCutG*)pidFile->Get(junkCut_energy_char);
junkCut_energy->SetName("junkCut_energy");
junkCut_energy->SetLineColor(28);
junkCut_energy->SetLineWidth(1);
junkCut_energy->SetFillColorAlpha(28,0.3);
TCutG* alphaCut_above_zCut_energy = (TCutG*)pidFile->Get(above_zCut_energy_char);
alphaCut_above_zCut_energy->SetName("above_zCut_energy");
alphaCut_above_zCut_energy->SetLineColor(kSpring);
alphaCut_above_zCut_energy->SetLineWidth(2);
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

char noMaskFragCut[234];
sprintf(noMaskFragCut,"fragCut_adc");
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
char highEn_cftFragCut[234];sprintf(highEn_cftFragCut,"highEn_fragCut_adc");
char cftFragUnlikelyCut_char[234];sprintf(cftFragUnlikelyCut_char,"junkCut_adc");
char cftAlphaCut_above_zCut_adc_char[234];sprintf(cftAlphaCut_above_zCut_adc_char,"alphaCut_above_zCut_adc");
char cftAlphaCut_long_char[234];sprintf(cftAlphaCut_long_char,"alphaCut_long_adc");
char cftAlphaCut_long_low_char[234];sprintf(cftAlphaCut_long_low_char,"alphaCut_long_low_adc");
char cftAlphaCut_long_high_adc_char[234];sprintf(cftAlphaCut_long_high_adc_char,"long_high_adc");
char cftAlphaCut_short_higher_adc_char[234];sprintf(cftAlphaCut_short_higher_adc_char,"alphaCut_short_higher_adc");
char cftAlphaCut_alphaCut_short_low_adc_char[234];sprintf(cftAlphaCut_alphaCut_short_low_adc_char,"alphaCut_short_low_adc");

printf("noMaskCut = %s\n",noMaskCut);
printf("cftCut = %s\n",cftCut);
////////////////////// get track trees ///////////////////////////////////
printf("////////////////////// get track trees ///////////////////////////////////\n");
TFile* cftFile = new TFile(cftFileName);
TTree* cftTracks_orig = (TTree*)cftFile->Get("tracks");
TFile* noMaskFile = new TFile(noMaskFileName);
TTree* noMaskTracks_orig = (TTree*)noMaskFile->Get("tracks");

printf("\n/////////////////////// ratio ////////////////////\n\n");
/////////////////////// ratio ////////////////////
sprintf(buffChar,"%s && %s",highEn_fragCut_adc->GetName(),cftCut);

TH2D* ratio_frags = new TH2D("ratio_frags","ratio_frags",4000,100,40000,1000,0,10);
cftTracks_orig->Draw("length:adc>>ratio_frags",buffChar);
ratio_frags->GetXaxis()->SetRangeUser(100,40000);
ratio_frags->GetYaxis()->SetRangeUser(0,6);
sprintf(buffChar,"%s && %s",alphaCut_above_zCut_adc->GetName(),noMaskRunCut);

TH2D* ratio_alphas = new TH2D("ratio_alphas","ratio_alphas",4000,100,40000,1000,0,10);
noMaskTracks_orig->Draw("length:adc>>ratio_alphas",buffChar);
ratio_alphas->GetXaxis()->SetRangeUser(100,40000);
ratio_alphas->GetYaxis()->SetRangeUser(0,6);

TH2D* ratio_both = new TH2D("ratio_both","244Cm",4000,100,40000,1000,0,10);
ratio_both->Add(ratio_alphas);
ratio_both->Add(ratio_frags);
ratio_both->GetYaxis()->SetTitle("Track Length [cm]");
ratio_both->GetYaxis()->CenterTitle();
ratio_both->GetXaxis()->SetTitle("Track Energy [MeV]");
ratio_both->GetXaxis()->CenterTitle();
ratio_both->GetXaxis()->SetRangeUser(100,40000);
ratio_both->GetYaxis()->SetRangeUser(0,6);

char alphaCount[456],fragCount[456],alphaTime[456],fragTime[456];
sprintf(alphaCount,"%.0f alphas",ratio_alphas->Integral());
sprintf(fragCount,"%.0f frags",ratio_frags->Integral());
TLegend* ratio_legend = new TLegend(0.65,0.7,0.89,0.89);
ratio_legend->SetTextFont(42);
ratio_legend->SetTextSize(0.03);
ratio_legend->AddEntry((TObject*)0,alphaCount,"");
ratio_legend->AddEntry((TObject*)0,fragCount,"");

c1->SetLogx(1);c1->SetLogz(1);
ratio_both->Draw("colz");
c1->SetLogx(1);c1->SetLogz(1);
ratio_legend->Draw();
alphaCut_above_zCut_adc->Draw("same f");
alphaCut_above_zCut_adc->Draw("same");
highEn_fragCut_adc->Draw("same f");
highEn_fragCut_adc->Draw("same");
c1->SaveAs("1__cm244_ratioestimate.png");

TH2D* raw_lvadc = new TH2D("raw_lvadc","244Cm",4000,100,40000,1000,0,10);
cftTracks_orig->Draw("length:adc>>raw_lvadc");
raw_lvadc->GetYaxis()->SetTitle("Track Length [cm]");
raw_lvadc->GetYaxis()->CenterTitle();
raw_lvadc->GetXaxis()->SetTitle("Track Energy [MeV]");
raw_lvadc->GetXaxis()->CenterTitle();
raw_lvadc->GetXaxis()->SetRangeUser(100,40000);
raw_lvadc->GetYaxis()->SetRangeUser(0,6);
ratio_legend->Clear();
sprintf(fragCount,"%.0f frags",highEn_fragCut_adc->IntegralHist(raw_lvadc));
ratio_legend->AddEntry((TObject*)0,fragCount,"");
raw_lvadc->Draw("colz");
ratio_legend->Draw();
// alphaCut_above_zCut_adc->Draw("same f");
// alphaCut_above_zCut_adc->Draw("same");
highEn_fragCut_adc->Draw("same f");
highEn_fragCut_adc->Draw("same");
c1->SaveAs("1__raw.png");
c1->SetLogx(0);c1->SetLogz(1);
raw_lvadc->GetXaxis()->SetRangeUser(100,4000);
raw_lvadc->GetYaxis()->SetRangeUser(0,6);
raw_lvadc->Draw("colz");
// alphaCut_above_zCut_adc->Draw("same f");
// alphaCut_above_zCut_adc->Draw("same");
highEn_fragCut_adc->Draw("same f");
highEn_fragCut_adc->Draw("same");
c1->SaveAs("1__raw_zoom.png");


printf("\n/////////////////////// unCut plots ////////////////////\n\n");
/////////////////////// azimuth vs energy ////////////////////
TH2D* noMaskLvEn_noCuts = new TH2D("noMaskLvEn_noCuts","noMaskLvEn_noCuts",4000,0,40000*MeV_per_ADC,500,0,10);
noMaskLvEn_noCuts->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_noCuts->GetYaxis()->CenterTitle();
noMaskLvEn_noCuts->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_noCuts->GetXaxis()->CenterTitle();
noMaskLvEn_noCuts->GetXaxis()->SetRangeUser(0.4,125);
sprintf(buffChar,"length:adc*%f>>noMaskLvEn_noCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar);
c1->SetLogx(1);c1->SetLogy(0);c1->SetLogz(1);
noMaskLvEn_noCuts->Draw("colz");
c1->SaveAs("noMaskLvEn_noCuts.png");
/////////////////////// energy vs cos theta ////////////////////
TH2D* Energy_vs_cosTheta_noCuts = new TH2D("Energy_vs_cosTheta_noCuts","Energy_vs_cosTheta_noCuts",500,-1,1,4000,0,40000*MeV_per_ADC);
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

printf("\n/////////////////////// unCut plots ////////////////////\n\n");
////////////////////// energy vs length ////////////////////
TH2D* noMaskLvEn_wCuts = new TH2D("noMaskLvEn_wCuts","noMaskLvEn_wCuts",4000,0,40000*MeV_per_ADC,500,0,10);
noMaskLvEn_wCuts->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_wCuts->GetYaxis()->CenterTitle();
noMaskLvEn_wCuts->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_wCuts->GetXaxis()->CenterTitle();
noMaskLvEn_wCuts->GetXaxis()->SetRangeUser(0.4,125);
sprintf(buffChar,"length:adc*%f>>noMaskLvEn_wCuts",MeV_per_ADC);
noMaskTracks_orig->Draw(buffChar,noMaskCut);
c1->SetLogx(1);c1->SetLogy(0);c1->SetLogz(1);
noMaskLvEn_wCuts->Draw("colz");
c1->SaveAs("noMaskLvEn_wCuts.png");
/////////////////////// energy vs cos theta ////////////////////
TH2D* Energy_vs_cosTheta_wCuts = new TH2D("Energy_vs_cosTheta_wCuts","Energy_vs_cosTheta_wCuts",500,-1,1,4000,0,40000*MeV_per_ADC);
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


////////////////////// create new TTree's with noMask and CFT cuts ///////////////////////////////////
printf("////////////////////// create new TTree's with noMask and CFT cuts ///////////////////////////////////\n");
TFile* dummy = new TFile("deleteme.root","create");
dummy->cd();
TTree* cftTracks = (TTree*)cftTracks_orig->CopyTree(cftCut);
TTree* noMaskTracks = (TTree*)noMaskTracks_orig->CopyTree(noMaskCut);

////////////////////// create PID bin histograms ///////////////////////////////////
printf("////////////////////// create PID bin histograms ///////////////////////////////////\n");
TH2D* noMaskLvADC = new TH2D("noMaskLvADC","noMaskLvADC",4000,0,40000,1000,0,10);
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
TH2D* highEn_cftLvADCFrags = (TH2D*)cftLvADC->Clone("highEn_cftLvADCFrags");
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
noMaskTracks->Draw("length:adc>>noMaskLvADC",noMaskCut);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas",noMaskAlphaCut_above_zCut_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCFrags",noMaskFragCut);
noMaskTracks->Draw("length:adc>>noMaskLvADCFragsUnlikely",noMaskFragUnlikelyCut_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_long",noMaskAlphaCut_long_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_long_low",noMaskAlphaCut_long_low_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_long_high",noMaskAlphaCut_long_high_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_short_higher",noMaskAlphaCut_short_higher_adc_char);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas_alphaCut_short_low",noMaskAlphaCut_alphaCut_short_low_adc_char);
int noMaskLvADC_checkMutualExclusivity_integral = noMaskLvADC_checkMutualExclusivity->Integral();
int noMaskLvADC_integral = noMaskLvADC->Integral();
int noMaskLvADCAlphas_integral = noMaskLvADCAlphas->Integral();
int noMaskLvADCFrags_integral = noMaskLvADCFrags->Integral();
int noMaskLvADCFragsUnlikely_integral = noMaskLvADCFragsUnlikely->Integral();
int noMaskLvADCAlphas_long_integral = noMaskLvADCAlphas_long->Integral();
int noMaskLvADCAlphas_long_low_integral = noMaskLvADCAlphas_long_low->Integral();
int noMaskLvADCAlphas_long_high_integral = noMaskLvADCAlphas_long_high->Integral();
int noMaskLvADCAlphas_short_higher_integral = noMaskLvADCAlphas_short_higher->Integral();
int noMaskLvADCAlphas_alphaCut_short_low_integral = noMaskLvADCAlphas_alphaCut_short_low->Integral();
printf("noMask_mutualExclusivityCheckChar:%s\n",noMask_mutualExclusivityCheckChar);
printf("filled %d noMaskLvADC_checkMutualExclusivity from track tree\n",noMaskLvADC_checkMutualExclusivity_integral);
printf("filled %d noMaskLvADC from track tree\n",noMaskLvADC_integral);
printf("filled %d noMaskLvADCAlphas from track tree\n",noMaskLvADCAlphas_integral);
printf("filled %d noMaskLvADCFrags from track tree\n",noMaskLvADCFrags_integral);
printf("filled %d noMaskLvADCFragsUnlikely from track tree\n",noMaskLvADCFragsUnlikely_integral);
printf("filled %d noMaskLvADCAlphas_long from track tree\n",noMaskLvADCAlphas_long_integral);
printf("filled %d noMaskLvADCAlphas_long_low from track tree\n",noMaskLvADCAlphas_long_low_integral);
printf("filled %d noMaskLvADCAlphas_long_high from track tree\n",noMaskLvADCAlphas_long_high_integral);
printf("filled %d noMaskLvADCAlphas_short_higher from track tree\n",noMaskLvADCAlphas_short_higher_integral);////////////////////// noMask create energy projection of color coded length-energy containers ///////////////
printf("////////////////////// noMask create energy projection of color coded length-energy containers ///////////////\n");
TH1D* noMaskADC = noMaskLvADC->ProjectionX("noMaskADC");
TH1D* noMaskFragsADC = noMaskLvADCFrags->ProjectionX("noMaskFragsADC");
TH1D* noMaskFragsUnlikeyADC = noMaskLvADCFrags->ProjectionX("noMaskFragsUnlikeyADC");
TH1D* noMaskAlphasADC = noMaskLvADCAlphas->ProjectionX("noMaskAlphasADC");
TH1D* noMaskAlphasADC_long = noMaskLvADCAlphas_long->ProjectionX("noMaskAlphasADC_long");
TH1D* noMaskAlphasADC_long_low = noMaskLvADCAlphas_long_low->ProjectionX("noMaskAlphasADC_long_low");
TH1D* noMaskAlphasADC_long_high = noMaskLvADCAlphas_long_high->ProjectionX("noMaskAlphasADC_long_high");
TH1D* noMaskAlphasADC_short_higher = noMaskLvADCAlphas_short_higher->ProjectionX("noMaskAlphasADC_short_higher");
TH1D* noMaskAlphasADC_alphaCut_short_low = noMaskLvADCAlphas_alphaCut_short_low->ProjectionX("noMaskAlphasADC_alphaCut_short_low");
////////////////////// verify PID bins are mutually exclusive ///////////////////////////////////
printf("////////////////////// verify PID bins are mutually exclusive ///////////////////////////////////\n");
printf("filled %d noMaskLvADCAlphas_alphaCut_short_low from track tree\n",noMaskLvADCAlphas_alphaCut_short_low_integral);
printf("noMaskLvADC_integral - all TCutG integrals :%d\n",
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
printf("noMaskLvADC_integral - noMaskLvADC_checkMutualExclusivity_integral :%d\n",
noMaskLvADC_integral - noMaskLvADC_checkMutualExclusivity_integral);

////////////////////// fill cft PID histograms ///////////////////////////////////
printf("////////////////////// fill cft PID histograms ///////////////////////////////////\n");
cftTracks->Draw("length:adc>>cftLvADC",cftCut);
cftTracks->Draw("length:adc>>cftLvADCAlphas",cftAlphaCut_above_zCut_adc_char);
cftTracks->Draw("length:adc>>cftLvADCFrags",cftFragCut);
cftTracks->Draw("length:adc>>highEn_cftLvADCFrags",highEn_fragCut_adc_char);
cftTracks->Draw("length:adc>>cftLvADCFragsUnlikely",cftFragUnlikelyCut_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_long",cftAlphaCut_long_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("length:adc>>cftLvADCAlphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);
printf("////////////////////// cft create ADC projection of color coded length-ADC containers ///////////////\n");
TH1D* cftADC = cftLvADC->ProjectionX("cftADC");
TH1D* cftFragsADC = cftLvADCFrags->ProjectionX("cftFragsADC");
TH1D* highEn_cftFragsADC = highEn_cftLvADCFrags->ProjectionX("highEn_cftFragsADC");
TH1D* cftFragsUnlikeyADC = cftLvADCFrags->ProjectionX("cftFragsUnlikeyADC");
TH1D* cftAlphasADC = cftLvADCAlphas->ProjectionX("cftAlphasADC");
TH1D* cftAlphasADC_long = cftLvADCAlphas_long->ProjectionX("cftAlphasADC_long");
TH1D* cftAlphasADC_long_low = cftLvADCAlphas_long_low->ProjectionX("cftAlphasADC_long_low");
TH1D* cftAlphasADC_long_high = cftLvADCAlphas_long_high->ProjectionX("cftAlphasADC_long_high");
TH1D* cftAlphasADC_short_higher = cftLvADCAlphas_short_higher->ProjectionX("cftAlphasADC_short_higher");
TH1D* cftAlphasADC_alphaCut_short_low = cftLvADCAlphas_alphaCut_short_low->ProjectionX("cftAlphasADC_alphaCut_short_low");
int cftLvADC_integral = cftLvADC->Integral();
int cftLvADCAlphas_integral = cftLvADCAlphas->Integral();
int cftLvADCFrags_integral = cftLvADCFrags->Integral();
int highEn_cftLvADCFrags_integral = highEn_cftLvADCFrags->Integral();
int cftLvADCFragsUnlikely_integral = cftLvADCFragsUnlikely->Integral();
int cftLvADCAlphas_long_integral = cftLvADCAlphas_long->Integral();
int cftLvADCAlphas_long_low_integral = cftLvADCAlphas_long_low->Integral();
int cftLvADCAlphas_long_high_integral = cftLvADCAlphas_long_high->Integral();
int cftLvADCAlphas_short_higher_integral = cftLvADCAlphas_short_higher->Integral();
int cftLvADCAlphas_alphaCut_short_low_integral = cftLvADCAlphas_alphaCut_short_low->Integral();
printf("cftLvADC_integral: %d \n",cftLvADC_integral);
printf("cftLvADCAlphas_integral: %d \n",cftLvADCAlphas_integral);
printf("cftLvADCFrags_integral: %d \n",cftLvADCFrags_integral);
printf("highEn_cftLvADCFrags_integral: %d \n",highEn_cftLvADCFrags_integral);
printf("cftLvADCFragsUnlikely_integral: %d \n",cftLvADCFragsUnlikely_integral);
printf("cftLvADCAlphas_long_integral: %d \n",cftLvADCAlphas_long_integral);
printf("cftLvADCAlphas_long_low_integral: %d \n",cftLvADCAlphas_long_low_integral);
printf("cftLvADCAlphas_long_high_integral: %d \n",cftLvADCAlphas_long_high_integral);
printf("cftLvADCAlphas_short_higher_integral: %d \n",cftLvADCAlphas_short_higher_integral);
printf("cftLvADCAlphas_alphaCut_short_low_integral: %d \n",cftLvADCAlphas_alphaCut_short_low_integral);

////////////////////// create noMaskLvEn  ///////////////////////////////////
printf("////////////////////// create noMaskLvEn  ///////////////////////////////////\n");
TH2D* noMaskLvEn = new TH2D("noMaskLvEn","noMaskLvEn",4000,0,40000*MeV_per_ADC,1000,0,10);
noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn->GetXaxis()->CenterTitle();
noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn->GetYaxis()->CenterTitle();
TH2D* noMaskLvEnFrags = (TH2D*)noMaskLvEn->Clone("noMaskLvEnFrags");
noMaskLvEnFrags->SetTitle("noMaskLvEnFrags");
TH2D* noMaskLvEnFragsUnlikely = (TH2D*)noMaskLvEn->Clone("noMaskLvEnFragsUnlikely");
noMaskLvEnFragsUnlikely->SetTitle("noMaskLvEnFragsUnlikely");
TH2D* noMaskLvEnAlphas = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas");
noMaskLvEnAlphas->SetTitle("noMaskLvEnAlphas");
TH2D* noMaskLvEnAlphas_long = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_long");
noMaskLvEnAlphas_long->SetTitle("noMaskLvEnAlphas_long");
TH2D* noMaskLvEnAlphas_long_low = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_long_low");
noMaskLvEnAlphas_long_low->SetTitle("noMaskLvEnAlphas_long_low");
TH2D* noMaskLvEnAlphas_long_high = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_long_high");
noMaskLvEnAlphas_long_high->SetTitle("noMaskLvEnAlphas_long_high");
TH2D* noMaskLvEnAlphas_short_higher = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_short_higher");
noMaskLvEnAlphas_short_higher->SetTitle("noMaskLvEnAlphas_short_higher");
TH2D* noMaskLvEnAlphas_alphaCut_short_low = (TH2D*)noMaskLvEn->Clone("noMaskLvEnAlphas_alphaCut_short_low");
noMaskLvEnAlphas_alphaCut_short_low->SetTitle("noMaskLvEnAlphas_alphaCut_short_low");
////////////////////// create cftLvEn  ///////////////////////////////////
printf("////////////////////// create cftLvEn  ///////////////////////////////////\n");
TH2D* cftLvEn = new TH2D("cftLvEn","cftLvEn",4000,0,40000*MeV_per_ADC,1000,0,10);
cftLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftLvEn->GetXaxis()->CenterTitle();
cftLvEn->GetYaxis()->SetTitle("Track Length [cm]");
cftLvEn->GetYaxis()->CenterTitle();
TH2D* cftLvEnFrags = (TH2D*)cftLvEn->Clone("cftLvEnFrags");
cftLvEnFrags->SetTitle("cftLvEnFrags");
TH2D* highEn_cftLvEnFrags = (TH2D*)cftLvEn->Clone("highEn_cftLvEnFrags");
highEn_cftLvEnFrags->SetTitle("highEn_cftLvEnFrags");
TH2D* cftLvEnFragsUnlikely = (TH2D*)cftLvEn->Clone("cftLvEnFragsUnlikely");
cftLvEnFragsUnlikely->SetTitle("cftLvEnFragsUnlikely");
TH2D* cftLvEnAlphas = (TH2D*)cftLvEn->Clone("cftLvEnAlphas");
cftLvEnAlphas->SetTitle("cftLvEnAlphas");
TH2D* cftLvEnAlphas_long = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_long");
cftLvEnAlphas_long->SetTitle("cftLvEnAlphas_long");
TH2D* cftLvEnAlphas_long_low = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_long_low");
cftLvEnAlphas_long_low->SetTitle("cftLvEnAlphas_long_low");
TH2D* cftLvEnAlphas_long_high = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_long_high");
cftLvEnAlphas_long_high->SetTitle("cftLvEnAlphas_long_high");
TH2D* cftLvEnAlphas_short_higher = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_short_higher");
cftLvEnAlphas_short_higher->SetTitle("cftLvEnAlphas_short_higher");
TH2D* cftLvEnAlphas_alphaCut_short_low = (TH2D*)cftLvEn->Clone("cftLvEnAlphas_alphaCut_short_low");
cftLvEnAlphas_alphaCut_short_low->SetTitle("cftLvEnAlphas_alphaCut_short_low");

////////////////////// create normalization histograms  ///////////////////////////////////
printf("////////////////////// create normalization histograms  ///////////////////////////////////\n");
TH2D* cftLvEn_NOT_norm = (TH2D*)cftLvEn->Clone("cftLvEn_NOT_norm");
TH2D* noMaskLvEn_NOT_norm = (TH2D*)noMaskLvEn->Clone("noMaskLvEn_NOT_norm");
TH1D* noMaskFragsEn_NOT_norm = (TH1D*)noMaskLvEnFrags->ProjectionX("noMaskFragsEn_NOT_norm");
TH1D* cftFragsEn_NOT_norm = (TH1D*)cftLvEnFrags->ProjectionX("cftFragsEn_NOT_norm");


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
		cftLvEn->SetBinContent(adcBin,lengthBin,cftLvADC->GetBinContent(adcBin,lengthBin));
		highEn_cftLvEnFrags->SetBinContent(adcBin,lengthBin,highEn_cftLvADCFrags->GetBinContent(adcBin,lengthBin));
		cftLvEnFrags->SetBinContent(adcBin,lengthBin,cftLvADCFrags->GetBinContent(adcBin,lengthBin));
		cftLvEnFragsUnlikely->SetBinContent(adcBin,lengthBin,cftLvADCFragsUnlikely->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas->SetBinContent(adcBin,lengthBin,cftLvADCAlphas->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_long->SetBinContent(adcBin,lengthBin,cftLvADCAlphas_long->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_long_low->SetBinContent(adcBin,lengthBin,cftLvADCAlphas_long_low->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_long_high->SetBinContent(adcBin,lengthBin,cftLvADCAlphas_long_high->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_short_higher->SetBinContent(adcBin,lengthBin,cftLvADCAlphas_short_higher->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas_alphaCut_short_low->SetBinContent(adcBin,lengthBin,cftLvADCAlphas_alphaCut_short_low->GetBinContent(adcBin,lengthBin));
		//////////////  create unmormalzed energy histogram for explanation plots ////////////////
		cftLvEn_NOT_norm->SetBinContent(adcBin,lengthBin,cftLvADC->GetBinContent(adcBin,lengthBin));
		noMaskLvEn_NOT_norm->SetBinContent(adcBin,lengthBin,noMaskLvADC->GetBinContent(adcBin,lengthBin));
		cftFragsEn_NOT_norm->SetBinContent(adcBin,cftFragsADC->GetBinContent(adcBin));
		noMaskFragsEn_NOT_norm->SetBinContent(adcBin,noMaskFragsADC->GetBinContent(adcBin));
	}
}

int rebin=2;
c1->SetLogx(0);c1->SetLogy(1);
TH1D* noMaskEn = new TH1D("noMaskEn","noMaskEn",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskFragsEn.png");
TH1D* noMaskFragsUnlikelyEn = new TH1D("noMaskFragsUnlikelyEn","noMaskFragsUnlikelyEn",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskFragsUnlikelyEn.png");
TH1D* noMaskAlphasEn = new TH1D("noMaskAlphasEn","noMaskAlphasEn",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskAlphasEn.png");
TH1D* noMaskAlphasEn_long = new TH1D("noMaskAlphasEn_long","noMaskAlphasEn_long",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskAlphasEn_long.png");
TH1D* noMaskAlphasEn_long_low = new TH1D("noMaskAlphasEn_long_low","noMaskAlphasEn_long_low",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskAlphasEn_long_low.png");
TH1D* noMaskAlphasEn_long_high = new TH1D("noMaskAlphasEn_long_high","noMaskAlphasEn_long_high",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskAlphasEn_long_high.png");
TH1D* noMaskAlphasEn_short_higher = new TH1D("noMaskAlphasEn_short_higher","noMaskAlphasEn_short_higher",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskAlphasEn_short_higher.png");
TH1D* noMaskAlphasEn_alphaCut_short_low = new TH1D("noMaskAlphasEn_alphaCut_short_low","noMaskAlphasEn_alphaCut_short_low",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("noMaskAlphasEn_alphaCut_short_low.png");
////////////////////// cft create ADC projection of color coded length-ADC containers ///////////////
TH1D* cftEn = new TH1D("cftEn","cftEn",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftFragsEn.png");
TH1D* highEn_cftFragsEn = new TH1D("highEn_cftFragsEn","highEn_cftFragsEn",4000,0,40000*MeV_per_ADC);
highEn_cftFragsEn = highEn_cftLvEnFrags->ProjectionX("highEn_cftFragsEn");
highEn_cftFragsEn->Rebin(rebin);
highEn_cftFragsEn->GetXaxis()->SetRangeUser(0.4,140);
highEn_cftFragsEn->GetYaxis()->SetRangeUser(0.1,maxY_energy);
highEn_cftFragsEn->SetTitle("highEn_cftFragsEn");
highEn_cftFragsEn->SetLineColor(kViolet);
highEn_cftFragsEn->SetLineWidth((Width_t)0.01);
highEn_cftFragsEn->SetFillColorAlpha(kViolet,0.7);
highEn_cftFragsEn->GetXaxis()->SetTitle("Track Energy [MeV]");
highEn_cftFragsEn->Draw();
// c1->SaveAs("highEn_cftFragsEn.png");
TH1D* cftFragsUnlikelyEn = new TH1D("cftFragsUnlikelyEn","cftFragsUnlikelyEn",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftFragsUnlikelyEn.png");
TH1D* cftAlphasEn = new TH1D("cftAlphasEn","cftAlphasEn",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftAlphasEn.png");
TH1D* cftAlphasEn_long = new TH1D("cftAlphasEn_long","cftAlphasEn_long",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftAlphasEn_long.png");
TH1D* cftAlphasEn_long_low = new TH1D("cftAlphasEn_long_low","cftAlphasEn_long_low",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftAlphasEn_long_low.png");
TH1D* cftAlphasEn_long_high = new TH1D("cftAlphasEn_long_high","cftAlphasEn_long_high",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftAlphasEn_long_high.png");
TH1D* cftAlphasEn_short_higher = new TH1D("cftAlphasEn_short_higher","cftAlphasEn_short_higher",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftAlphasEn_short_higher.png");
TH1D* cftAlphasEn_alphaCut_short_low = new TH1D("cftAlphasEn_alphaCut_short_low","cftAlphasEn_alphaCut_short_low",4000,0,40000*MeV_per_ADC);
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
// c1->SaveAs("cftAlphasEn_alphaCut_short_low.png");

//////////////  count frags ////////////////
printf("\n//////////////  count frags ////////////////\n\n");
int noMaskIntegral=noMaskADC->Integral();
printf("noMaskIntegral: %d\n",noMaskIntegral);
int noMaskFragIntegral=noMaskFragsADC->Integral();
printf("noMaskFragIntegral: %d\n",noMaskFragIntegral);
int noMaskAlphaIntegral=noMaskAlphasADC->Integral();
printf("noMaskAlphaIntegral: %d\n",noMaskAlphaIntegral);
int noMaskNotFragIntegral=noMaskIntegral-noMaskFragIntegral;
printf("noMaskNotFragIntegral: %d\n",noMaskNotFragIntegral);
double percent_FragAccepted = 100*cftFragsEn->Integral()/(double)noMaskFragsEn->Integral();
printf("percent_FragAccepted: %.4f\n",percent_FragAccepted);
double percent_AlphasRejected = 100*TMath::Abs(1-cftAlphasEn->Integral()/(double)noMaskAlphasEn->Integral());
printf("percent_AlphasRejected: %.4f\n",percent_AlphasRejected);
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
int cftIntegral=cftEn->Integral();
printf("cftIntegral: %d\n",cftIntegral);
int cftFragIntegral=cftFragsEn->Integral();
printf("cftFragIntegral: %d\n",cftFragIntegral);
int highEn_cftFragIntegral=highEn_cftFragsEn->Integral();
printf("highEn_cftFragIntegral: %d\n",highEn_cftFragIntegral);
int cftAlphaIntegral=cftAlphasEn->Integral();
printf("cftAlphaIntegral: %d\n",cftAlphaIntegral);
int cftNotFragIntegral=cftIntegral-cftFragIntegral;
printf("cftNotFragIntegral: %d\n",cftNotFragIntegral);
double cftFragsEn_percent_integral=cftFragsEn->Integral()/(double)cftIntegral*100;
printf("cftFragsEn_percent_integral: %.4f\n",cftFragsEn_percent_integral);
double highEn_cftFragsEn_percent_integral=highEn_cftFragsEn->Integral()/(double)cftIntegral*100;
printf("highEn_cftFragsEn_percent_integral: %.4f\n",highEn_cftFragsEn_percent_integral);
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
printf("highEn_cftFragIntegral: %d\n",highEn_cftFragIntegral);
printf("cftAlphaIntegral: %d\n",cftAlphaIntegral);
printf("noMaskFragIntegral: %d\n",noMaskFragIntegral);
printf("noMaskAlphaIntegral: %d\n",noMaskAlphaIntegral);
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
	highEn_cftFragsEn_percent_char[234],
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
TString hignEn_cftFragIntegral_string;
hignEn_cftFragIntegral_string = Form("%5.3g",(double)highEn_cftFragIntegral);
hignEn_cftFragIntegral_string.ReplaceAll("e+0"," #scale[1.7]{#upoint} 10^{");hignEn_cftFragIntegral_string.Append("}  CFT fragments");
TLatex* hignEn_cftFragIntegral_latex = new TLatex(.5,.5, hignEn_cftFragIntegral_string.Data());
TString noMaskFragIntegral_string;
noMaskFragIntegral_string = Form("%5.3g",(double)noMaskFragIntegral);
noMaskFragIntegral_string.ReplaceAll("e+0"," #scale[1.7]{#upoint} 10^{");noMaskFragIntegral_string.Append("} noMask Fragments");
TLatex* noMaskFragIntegral_latex = new TLatex(.5,.5, noMaskFragIntegral_string.Data());
sprintf(cft_aSF_ratioChar,"cft_aSF_ratio = %.2f",cft_aSF_ratio);
sprintf(noMask_aSF_ratioChar,"noMask_aSF_ratio = %.2f",noMask_aSF_ratio);
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
sprintf(highEn_cftFragsEn_percent_char,"%.3f%%",highEn_cftFragsEn_percent_integral);
sprintf(cftFragsUnlikelyEn_percent_char,"%.3f%%",cftFragsUnlikelyEn_percent_integral);
sprintf(cftAlphasEn_percent_char,"%.3f%%",cftAlphasEn_percent_integral);
sprintf(cftAlphasEn_long_percent_char,"%.3f%%",cftAlphasEn_long_percent_integral);
sprintf(cftAlphasEn_long_low_percent_char,"%.3f%%",cftAlphasEn_long_low_percent_integral);
sprintf(cftAlphasEn_long_high_percent_char,"%.3f%%",cftAlphasEn_long_high_percent_integral);
sprintf(cftAlphasEn_short_higher_percent_char,"%.3f%%",cftAlphasEn_short_higher_percent_integral);
sprintf(cftAlphasEn_alphaCut_short_low_percent_char,"%.3f%%",cftAlphasEn_alphaCut_short_low_percent_integral);
printf("\n\n");
printf("percent_FragAcceptedChar: %s\n",percent_FragAcceptedChar);
printf("percent_AlphasRejectedChar: %s\n",percent_AlphasRejectedChar);
printf("cft_aSF_ratioChar: %s\n",cft_aSF_ratioChar);
printf("noMask_aSF_ratioChar: %s\n",noMask_aSF_ratioChar);

////////////////////// fill length-energy & plots color cuts ///////////////////////////////////
printf("////////////////////// noMask fill length-energy & plots color cuts ///////////////////////////////////\n");
TH2D* noFrags_noAlphas_noMaskLvEn = new TH2D("noFrags_noAlphas_noMaskLvEn","noFrags_noAlphas_noMaskLvEn",4000,0,40000*MeV_per_ADC,500,0,10);
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
noMaskEn->GetXaxis()->SetRangeUser(0.4,15);
c1->SetLogx(0);c1->SetLogy(1);
noMaskEn->Draw();
c1->SaveAs("noMaskEn.lowEn.png");
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(0);
noMaskLvEn->Draw("colz");
c1->SaveAs("noMaskLvEn.fullEn.logx.png");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
// alphaCut_above_zCut_energy->Draw("same");
c1->SaveAs("noMaskLvEn.cuts.fullEn.logx.png");
c1->SetLogx(0);c1->SetLogy(0);
noMaskLvEn->GetYaxis()->SetRangeUser(0,6);
noMaskLvEn->Draw("colz");
c1->SaveAs("noMaskLvEn.fullEn.linx.png");
noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,15);
noMaskLvEn->Draw("colz");
c1->SaveAs("noMaskLvEn.lowEn.png");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
// alphaCut_above_zCut_energy->Draw("same");
c1->SaveAs("noMaskLvEn.cuts.lowEn.png");
printf("////////////////////// cft fill length-energy & plots color cuts ///////////////////////////////////\n");
cftEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(1);
cftEn->SetMinimum(0.5);
cftEn->Draw();
c1->SaveAs("cftEn.fullEn.logx.png");
cftEn->GetXaxis()->SetRangeUser(0.4,15);
c1->SetLogx(0);c1->SetLogy(1);
cftEn->Draw();
c1->SaveAs("cftEn.lowEn.png");
cftLvEn->GetXaxis()->SetRangeUser(0.4,140);
c1->SetLogx(1);c1->SetLogy(0);
cftLvEn->Draw("colz");
c1->SaveAs("cftLvEn.fullEn.logx.png");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
// alphaCut_above_zCut_energy->Draw("same");
c1->SaveAs("cftLvEn.cuts.fullEn.logx.png");
c1->SetLogx(0);c1->SetLogy(0);
cftLvEn->GetYaxis()->SetRangeUser(0,6);
cftLvEn->Draw("colz");
c1->SaveAs("cftLvEn.fullEn.linx.png");
cftLvEn->GetYaxis()->SetRangeUser(0,10);
cftLvEn->GetXaxis()->SetRangeUser(0.4,15);
cftLvEn->Draw("colz");
c1->SaveAs("cftLvEn.lowEn.png");
alphaCut_long_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
long_high_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same f");
junkCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
// alphaCut_above_zCut_energy->Draw("same");
c1->SaveAs("cftLvEn.cuts.lowEn.png");

/////////////////////////////// noMask create colorcoded ADC projection stack ///////////////////////////////////
printf("////////////////////// noMask create colorcoded ADC projection stack ///////////////////////////////////\n");
TLegend* noMask_pidCuts_stack_legend = new TLegend(stack_leg_x_start,0.6,stack_leg_x_stop,0.89);
noMask_pidCuts_stack_legend->SetTextFont(42);
noMask_pidCuts_stack_legend->SetTextSize(0.03);
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
noMask_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,15);
noMask_pidCuts_En_Stack->SetTitle("noMask_pidCuts_En_Stack_logy");
noMask_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("noMask_pidCuts_En_Stack_logy_lowEn.png");
c1->SetLogx(1);
noMask_pidCuts_En_Stack->SetTitle("noMask_pidCuts_En_Stack_logxy");
noMask_pidCuts_En_Stack->Draw();
c1->SetGrid(1);
noMask_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,140);
noMask_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("noMask_pidCuts_En_Stack_logxy_fullEn.png");
/////////////////////////////// cft create colorcoded ADC projection stack ///////////////////////////////////
printf("////////////////////// cft create colorcoded ADC projection stack ///////////////////////////////////\n");
TLegend* cft_pidCuts_stack_legend = new TLegend(stack_leg_x_start,0.6,stack_leg_x_stop,0.89);
cft_pidCuts_stack_legend->SetTextFont(42);
cft_pidCuts_stack_legend->SetTextSize(0.03);
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn,cftAlphasEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftFragsEn,cftFragsEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(highEn_cftFragsEn,highEn_cftFragsEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftFragsUnlikelyEn,cftFragsUnlikelyEn_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_alphaCut_short_low,cftAlphasEn_alphaCut_short_low_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_short_higher,cftAlphasEn_short_higher_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_long,cftAlphasEn_long_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_long_low,cftAlphasEn_long_low_percent_char,"f");
cft_pidCuts_stack_legend->AddEntry(cftAlphasEn_long_high,cftAlphasEn_long_high_percent_char,"f");
THStack* cft_pidCuts_En_Stack = new THStack("cft_pidCuts_En_Stack","");
cftFragsEn->SetFillColorAlpha(kViolet,0.7);
highEn_cftFragsEn->SetFillColorAlpha(kViolet,0.7);
cftFragsUnlikelyEn->SetFillColorAlpha(28,0.7);
cftAlphasEn->SetFillColorAlpha(kSpring,0.7);
cftAlphasEn_long->SetFillColorAlpha(kCyan,0.7);
cftAlphasEn_long_low->SetFillColorAlpha(kYellow,0.7);
cftAlphasEn_long_high->SetFillColorAlpha(kBlue,0.7);
cftAlphasEn_short_higher->SetFillColorAlpha(kOrange+7,0.7);
cftAlphasEn_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
cft_pidCuts_En_Stack->Add(cftFragsEn);
cft_pidCuts_En_Stack->Add(highEn_cftFragsEn);
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
cft_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,15);
cft_pidCuts_En_Stack->SetTitle("cft_pidCuts_En_Stack_logy");
cft_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("cft_pidCuts_En_Stack_logy_lowEn.png");
c1->SetLogx(1);
cft_pidCuts_En_Stack->SetTitle("cft_pidCuts_En_Stack_logxy");
cft_pidCuts_En_Stack->Draw();
c1->SetGrid(1);
cft_pidCuts_En_Stack->GetXaxis()->SetRangeUser(0.4,140);
cft_pidCuts_stack_legend->Draw();
gPad->RedrawAxis();
c1->ForceUpdate();
c1->SaveAs("cft_pidCuts_En_Stack_logxy_fullEn.png");

//////////////////// alpha/frag stacks comparison /////////////
printf("\n//////////////////// noMask stacks comparison /////////////\n\n");
TLegend* noMaskEnStack_legend = new TLegend(0.3,0.75,0.86,0.85);
noMaskEnStack_legend->SetTextFont(42);
noMaskEnStack_legend->SetTextSize(0.03);
noMaskEnStack_legend->AddEntry((TObject*)0,noMaskFragIntegral_string.Data(),"");
noMaskEnStack_legend->AddEntry((TObject*)0,noMaskAlphaIntegral_string.Data(),"");
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
noMaskEnStack->Draw();
c1->SetGrid(1);
noMaskEnStack->GetXaxis()->SetRangeUser(0.4,15);
c1->SetLogx(0);c1->SetLogy(1);
noMaskEnStack->SetTitle("noMaskLvEn_stack energy projection");
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
TLegend* cftEnStack_legend = new TLegend(0.3,0.75,0.86,0.85);
cftEnStack_legend->SetTextFont(42);
cftEnStack_legend->SetTextSize(0.03);
cftEnStack_legend->AddEntry((TObject*)0,cftFragIntegral_string.Data(),"");
cftEnStack_legend->AddEntry((TObject*)0,cftAlphaIntegral_string.Data(),"");
THStack* cftEnStack = new THStack("cftEnStack","");
cftFragsEn->SetFillColorAlpha(kViolet,0.3);
cftFragsEn->SetLineWidth((Width_t)1);
cftFragsEn->SetLineColor(kViolet);
highEn_cftFragsEn->SetFillColorAlpha(kViolet,0.3);
highEn_cftFragsEn->SetLineWidth((Width_t)1);
highEn_cftFragsEn->SetLineColor(kViolet);
cftAlphasEn->SetFillColorAlpha(kSpring,0.3);
cftAlphasEn->SetLineWidth((Width_t)1);
cftAlphasEn->SetLineColor(kSpring);
cftEnStack->Add(highEn_cftFragsEn);
cftEnStack->Add(cftFragsEn);
cftEnStack->Add(cftAlphasEn);
cftEnStack->SetMinimum(0.5);
cftEnStack->SetMaximum(maxY_energy);
cftEnStack->Draw();
c1->SetGrid(1);
c1->SetLogx(0);c1->SetLogy(1);
cftEnStack->GetXaxis()->SetRangeUser(0.4,15);
cftEnStack->SetTitle("cftLvEn_stack energy projection");
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
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,15);
noMaskLvEn->SetTitle("noMaskLvEn");
noMaskLvEn->Draw("colz");
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same");
highEn_fragCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
c1->SetLogx(0);
noMaskEnStack_legend->Draw();
c1->SaveAs("noMaskCuts.asf.lowEn.png");
c1->SetLogx(1);
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,140);
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same");
highEn_fragCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
c1->SaveAs("noMaskCuts.asf.fullEn.logxy.png");
printf("\n/////////////////////// cft Lenght VS Energy ////////////////////\n\n");
fragCut_energy->SetFillColorAlpha(kViolet,0.2);
alphaCut_above_zCut_energy->SetFillColorAlpha(kSpring,0.2);
c1->SetLogx(0);c1->SetLogy(0);
cftLvEn->GetXaxis()->SetRangeUser(0.4,15);
cftLvEn->SetTitle("cftLvEn");
cftLvEn->Draw("colz");
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same");
highEn_fragCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
c1->SetLogx(0);
cftEnStack_legend->Draw();
c1->SaveAs("cftCuts.asf.lowEn.png");
c1->SetLogx(1);
cftLvEn->GetXaxis()->SetRangeUser(0.4,140);
fragCut_energy->Draw("same");
fragCut_energy->Draw("same f");
highEn_fragCut_energy->Draw("same");
highEn_fragCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same f");
alphaCut_above_zCut_energy->Draw("same");
c1->SaveAs("cftCuts.asf.fullEn.logxy.png");

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
highEn_cftFragsEn->SetLineColor(kViolet);
cftFragsEn->SetLineColor(kViolet);
cftFragsUnlikelyEn->SetLineColor(kViolet);
cftAlphasEn->SetLineColor(kSpring);
cftAlphasEn_long->SetLineColor(kCyan);
cftAlphasEn_long_low->SetLineColor(kYellow);
cftAlphasEn_long_high->SetLineColor(kBlue);
cftAlphasEn_short_higher->SetLineColor(kOrange+7);
cftAlphasEn_alphaCut_short_low->SetLineColor(kRed);
TH1D* noMaskCosTheta = new TH1D("noMaskCosTheta","noMaskCosTheta",500,-1,1);
TH1D* noMaskCosThetaFrags = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaFrags");
noMaskCosThetaFrags->SetLineColor(kViolet);
noMaskCosThetaFrags->SetLineWidth((Width_t)0.01);
noMaskCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
TH1D* noMaskCosThetaFragsUnlikely = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaFragsUnlikely");
noMaskCosThetaFragsUnlikely->SetLineColor(28);
noMaskCosThetaFragsUnlikely->SetLineWidth((Width_t)0.01);
noMaskCosThetaFragsUnlikely->SetFillColorAlpha(28,0.7);
TH1D* noMaskCosThetaAlphas = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas");
noMaskCosThetaAlphas->SetLineColor(kSpring);
noMaskCosThetaAlphas->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas->SetFillColorAlpha(kSpring,0.7);
TH1D* noMaskCosThetaAlphas_long = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_long");
noMaskCosThetaAlphas_long->SetLineColor(kCyan);
noMaskCosThetaAlphas_long->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_long->SetFillColorAlpha(kCyan,0.7);
TH1D* noMaskCosThetaAlphas_long_low = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_long_low");
noMaskCosThetaAlphas_long_low->SetLineColor(kYellow);
noMaskCosThetaAlphas_long_low->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_long_low->SetFillColorAlpha(kYellow,0.7);
TH1D* noMaskCosThetaAlphas_long_high = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_long_high");
noMaskCosThetaAlphas_long_high->SetLineColor(kBlue);
noMaskCosThetaAlphas_long_high->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_long_high->SetFillColorAlpha(kBlue,0.7);
TH1D* noMaskCosThetaAlphas_short_higher = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_short_higher");
noMaskCosThetaAlphas_short_higher->SetLineColor(kOrange+7);
noMaskCosThetaAlphas_short_higher->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_short_higher->SetFillColorAlpha(kOrange+7,0.7);
TH1D* noMaskCosThetaAlphas_alphaCut_short_low = (TH1D*)noMaskCosTheta->Clone("noMaskCosThetaAlphas_alphaCut_short_low");
noMaskCosThetaAlphas_alphaCut_short_low->SetLineColor(kRed);
noMaskCosThetaAlphas_alphaCut_short_low->SetLineWidth((Width_t)0.01);
noMaskCosThetaAlphas_alphaCut_short_low->SetFillColorAlpha(kRed,0.7);
TH1D* cftCosTheta = new TH1D("cftCosTheta","cftCosTheta",500,-1,1);
TH1D* cftCosThetaFrags = (TH1D*)cftCosTheta->Clone("cftCosThetaFrags");
cftCosThetaFrags->SetLineColor(kViolet);
cftCosThetaFrags->SetLineWidth((Width_t)0.01);
cftCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
TH1D* highEn_cftCosThetaFrags = (TH1D*)cftCosTheta->Clone("highEn_cftCosThetaFrags");
highEn_cftCosThetaFrags->SetLineColor(kViolet);
highEn_cftCosThetaFrags->SetLineWidth((Width_t)0.01);
highEn_cftCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
TH1D* cftCosThetaFragsUnlikely = (TH1D*)cftCosTheta->Clone("cftCosThetaFragsUnlikely");
cftCosThetaFragsUnlikely->SetLineColor(28);
cftCosThetaFragsUnlikely->SetLineWidth((Width_t)0.01);
cftCosThetaFragsUnlikely->SetFillColorAlpha(28,0.7);
TH1D* cftCosThetaAlphas = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas");
cftCosThetaAlphas->SetLineColor(kSpring);
cftCosThetaAlphas->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas->SetFillColorAlpha(kSpring,0.7);
TH1D* cftCosThetaAlphas_long = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_long");
cftCosThetaAlphas_long->SetLineColor(kCyan);
cftCosThetaAlphas_long->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_long->SetFillColorAlpha(kCyan,0.7);
TH1D* cftCosThetaAlphas_long_low = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_long_low");
cftCosThetaAlphas_long_low->SetLineColor(kYellow);
cftCosThetaAlphas_long_low->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_long_low->SetFillColorAlpha(kYellow,0.7);
TH1D* cftCosThetaAlphas_long_high = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_long_high");
cftCosThetaAlphas_long_high->SetLineColor(kBlue);
cftCosThetaAlphas_long_high->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_long_high->SetFillColorAlpha(kBlue,0.7);
TH1D* cftCosThetaAlphas_short_higher = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_short_higher");
cftCosThetaAlphas_short_higher->SetLineColor(kOrange+7);
cftCosThetaAlphas_short_higher->SetLineWidth((Width_t)0.01);
cftCosThetaAlphas_short_higher->SetFillColorAlpha(kOrange+7,0.7);
TH1D* cftCosThetaAlphas_alphaCut_short_low = (TH1D*)cftCosTheta->Clone("cftCosThetaAlphas_alphaCut_short_low");
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
cftTracks->Draw("direction.CosTheta()>>highEn_cftCosThetaFrags",highEn_cftFragCut);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaFragsUnlikely",cftFragUnlikelyCut_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_long",cftAlphaCut_long_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("direction.CosTheta()>>cftCosThetaAlphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);
for (size_t i = 0; i < cftCosTheta->GetNbinsX(); i++) {
	cftCosTheta->SetBinContent(i,cftCosTheta->GetBinContent(i));
	cftCosThetaAlphas->SetBinContent(i,cftCosThetaAlphas->GetBinContent(i));
	highEn_cftCosThetaFrags->SetBinContent(i,highEn_cftCosThetaFrags->GetBinContent(i));
	cftCosThetaFrags->SetBinContent(i,cftCosThetaFrags->GetBinContent(i));
	cftCosThetaFragsUnlikely->SetBinContent(i,cftCosThetaFragsUnlikely->GetBinContent(i));
	cftCosThetaAlphas_long->SetBinContent(i,cftCosThetaAlphas_long->GetBinContent(i));
	cftCosThetaAlphas_long_low->SetBinContent(i,cftCosThetaAlphas_long_low->GetBinContent(i));
	cftCosThetaAlphas_long_high->SetBinContent(i,cftCosThetaAlphas_long_high->GetBinContent(i));
	cftCosThetaAlphas_short_higher->SetBinContent(i,cftCosThetaAlphas_short_higher->GetBinContent(i));
	cftCosThetaAlphas_alphaCut_short_low->SetBinContent(i,cftCosThetaAlphas_alphaCut_short_low->GetBinContent(i));
}
/////////////////////////////// colorcoded polar projection stack ///////////////////////////////////
printf("////////////////////// colorcoded polar projection stack ///////////////////////////////////\n");
//////////////  find y max for plots
double maxY_cos=0;
double dummyDub=0;
for (size_t i = 0; i < 50; i++) {
	dummyDub=noMaskCosTheta->GetBinContent(i);
	if ( dummyDub > maxY_cos) maxY_cos = dummyDub;
}
maxY_cos=10*maxY_cos;
c1->SetLogx(0);c1->SetLogy(0);
TLegend* noMask_cosTheta_stack_legend = new TLegend(stack_leg_x_start,0.6,stack_leg_x_stop,0.89);
noMask_cosTheta_stack_legend->SetTextFont(42);
noMask_cosTheta_stack_legend->SetTextSize(0.03);
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
noMask_cosTheta_Stack->SetTitle("noMask_NOalpha_NOfrag_cosTheta_Stack");
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
noMask_cosTheta_Stack->SetTitle("noMask_cosTheta_Stack");
c1->SetLogy(1);
noMask_cosTheta_stack_legend->Draw();
c1->Update();
c1->SaveAs("noMask_cosTheta_Stack.png");
c1->SetLogx(0);c1->SetLogy(0);
TLegend* cft_cosTheta_stack_legend = new TLegend(stack_leg_x_start,0.6,stack_leg_x_stop,0.89);
cft_cosTheta_stack_legend->SetTextFont(42);
cft_cosTheta_stack_legend->SetTextSize(0.03);
cftCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
highEn_cftCosThetaFrags->SetFillColorAlpha(kViolet,0.7);
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
cft_cosTheta_Stack->SetTitle("cft_NOalpha_NOfrag_cosTheta_Stack");
cft_cosTheta_stack_legend->Draw();
cft_cosTheta_Stack->GetXaxis()->SetTitle("cos #theta");
c1->Update();
c1->SaveAs("cft_NOalpha_NOfrag_cosTheta_Stack.png");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaAlphas,cftAlphasEn_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(cftCosThetaFrags,cftFragsEn_percent_char,"f");
cft_cosTheta_stack_legend->AddEntry(highEn_cftCosThetaFrags,highEn_cftFragsEn_percent_char,"f");
cft_cosTheta_Stack->Add(highEn_cftCosThetaFrags);
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
highEn_fragCut_adc->Write();
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
noMaskLvADC->Write();
noMaskLvADCFrags->Write();
noMaskLvADCFragsUnlikely->Write();
noMaskLvADCAlphas->Write();
noMaskLvADCAlphas_long->Write();
noMaskLvADCAlphas_long_low->Write();
noMaskLvADCAlphas_long_high->Write();
noMaskLvADCAlphas_short_higher->Write();
noMaskLvADCAlphas_alphaCut_short_low->Write();
cftLvADC->Write();
cftLvADCFrags->Write();
highEn_cftLvADCFrags->Write();
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
highEn_cftLvEnFrags->Write();
cftLvEnFragsUnlikely->Write();
cftLvEnAlphas->Write();
cftLvEnAlphas_long->Write();
cftLvEnAlphas_long_low->Write();
cftLvEnAlphas_long_high->Write();
cftLvEnAlphas_short_higher->Write();
cftLvEnAlphas_alphaCut_short_low->Write();
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
highEn_cftFragsADC->Write();
cftFragsUnlikeyADC->Write();
cftAlphasADC->Write();
cftAlphasADC_long->Write();
cftAlphasADC_long_low->Write();
cftAlphasADC_long_high->Write();
cftAlphasADC_short_higher->Write();
cftAlphasADC_alphaCut_short_low->Write();
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
highEn_cftFragsEn->Write();
cftFragsUnlikelyEn->Write();
cftAlphasEn->Write();
cftAlphasEn_long->Write();
cftAlphasEn_long_low->Write();
cftAlphasEn_long_high->Write();
cftAlphasEn_short_higher->Write();
cftAlphasEn_alphaCut_short_low->Write();
///////////////////////  write THStack ///////////////////
noMask_pidCuts_En_Stack->Write();
cft_pidCuts_En_Stack->Write();
noMaskEnStack->Write();
cftEnStack->Write();
noMask_cosTheta_Stack->Write();
cft_cosTheta_Stack->Write();

}
