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


void polarCft(const int preamp, const char* actinide, const char* noMaskFileName="", const char* cftFileName="", const char* otherTreeCuts="&&1",const char* pidType="zCut",const char* driftVel="") {

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
char c1PlotName[234];
gStyle->SetOptStat(0);
gStyle->SetPalette(71);
gStyle->SetNumberContours(999);
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

char pidFile_char[345];

char fragCut_adc_char[345];
char alphaCut_adc_char[345];
char alpha_short_high_adc_char[345];
char alpha_short_low_adc_char[345];
char above_zCut_adc_char[345];

char fragCut_energy_char[345];
char alphaCut_energy_char[345];
char alpha_short_high_energy_char[345];
char alpha_short_low_energy_char[345];
char above_zCut_energy_char[345];

TCutG* vertexCut = new TCutG("vertexCut",4);
vertexCut->SetVarX("start.y()");
vertexCut->SetVarY("start.x()");

if (preamp==1)
{
	zCut=70;
	fragEnergyThreshold=2.5;
	ADC_per_MeV=1180/6.118;
	MeV_per_ADC=6.118/1180;
	vertexCut->SetPoint(0,-1.33527,-0.127485);
	vertexCut->SetPoint(1,-0.44236,1.21056);
	vertexCut->SetPoint(2,0.978926,0.47083);
	vertexCut->SetPoint(3,0.925679,-0.323297);
	vertexCut->SetPoint(4,0.294907,-1.07391);
	vertexCut->SetPoint(5,-0.323578,-1.14462);
	vertexCut->SetPoint(6,-1.15915,-0.361371);
	vertexCut->SetPoint(7,-1.33527,-0.127485);
	if (!(strcmp(pidType,"expo"))){
		sprintf(pidFile_char,"$cft/pid/pidCut_alphaExponential%d.root",preamp);
		sprintf(fragCut_adc_char,"fragCut_adc_preamp%d_4stdDev",preamp);
		sprintf(alphaCut_adc_char,"alphaCut_adc_preamp%d_expoFit_4stdDev",preamp);
		sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc_preamp%d_4stdDev",preamp);
		sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc_preamp%d_expoFit_4stdDev",preamp);
		sprintf(above_zCut_adc_char,"noMaskLvEn_above_zCut%d_adc",zCut);
		sprintf(fragCut_energy_char,"fragCut_energy_preamp%d_4stdDev",preamp);
		sprintf(alphaCut_energy_char,"alphaCut_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy_preamp%d_4stdDev",preamp);
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(above_zCut_energy_char,"noMaskLvEn_above_zCut%d_energy",zCut);
	}
	if (!(strcmp(pidType,"zCut"))){
		sprintf(pidFile_char,"$cft/pid/pidCut_preamp%d_zCut%d_fragThresh%0.1f.driftVel%s.root",preamp,zCut,fragEnergyThreshold,driftVel);
  		sprintf(fragCut_adc_char,"fragCut_adc");
		sprintf(alphaCut_adc_char,"alphaCut_adc");
		sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
		sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
		sprintf(above_zCut_adc_char,"noMaskLvEn_above_zCut%d_adc",zCut);
		sprintf(fragCut_energy_char,"fragCut_energy");
		sprintf(alphaCut_energy_char,"alphaCut_energy");
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
		sprintf(above_zCut_energy_char,"noMaskLvEn_above_zCut%d_energy",zCut);
	}
	if (!(strcmp(actinide,"cf252"))){}
	sprintf(noMaskRunCut,"run>100001856&&run!=100001870");
	sprintf(noMaskCut," %s&&%s",otherTreeCuts,noMaskRunCut);
	sprintf(cftCut,"%s",otherTreeCuts);
}
else if (preamp==2)
{
	zCut=25;
	fragEnergyThreshold=2.0;
	ADC_per_MeV=1450/6.118;
	MeV_per_ADC=6.118/1450;
	vertexCut->SetPoint(0,-0.430136,0.866337);
	vertexCut->SetPoint(1,-0.646507,-0.485149);
	vertexCut->SetPoint(2,0.80292,-0.693069);
	vertexCut->SetPoint(3,0.915016,0.767327);
	vertexCut->SetPoint(4,-0.35975,0.861386);
	vertexCut->SetPoint(5,-0.430136,0.866337);
	if (!(strcmp(pidType,"expo"))){
		sprintf(pidFile_char,"$cft/pid/pidCut_alphaExponential%d.root",preamp);
		sprintf(fragCut_adc_char,"fragCut_adc_preamp%d_4stdDev",preamp);
		sprintf(alphaCut_adc_char,"alphaCut_adc_preamp%d_expoFit_4stdDev",preamp);
		sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc_preamp%d_4stdDev",preamp);
		sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc_preamp%d_expoFit_4stdDev",preamp);
		sprintf(above_zCut_adc_char,"alphaCut_zCut%03d_adc",zCut);
		sprintf(fragCut_energy_char,"fragCut_energy_preamp%d_4stdDev",preamp);
		sprintf(alphaCut_energy_char,"alphaCut_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy_preamp%d_4stdDev",preamp);
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(above_zCut_energy_char,"alphaCut_zCut%03d_energy",zCut);
	}
	if (!(strcmp(pidType,"zCut"))){
		sprintf(pidFile_char,"$cft/pid/pidCut_preamp%d_zCut%d_fragThresh%0.1f.driftVel%s.root",preamp,zCut,fragEnergyThreshold,driftVel);
		sprintf(fragCut_adc_char,"fragCut_adc");
		sprintf(alphaCut_adc_char,"alphaCut_adc");
		sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
		sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
		sprintf(above_zCut_adc_char,"alphaCut_zCut%03d_adc",zCut);
		sprintf(fragCut_energy_char,"fragCut_energy");
		sprintf(alphaCut_energy_char,"alphaCut_energy");
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
		sprintf(above_zCut_energy_char,"alphaCut_zCut%03d_energy",zCut);
	}
	if (!(strcmp(actinide,"cf252"))){
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
	}
	if ( !(strcmp(actinide,"cm244"))){
		sprintf(azCut,"direction.Phi()<2.2");
	}
	sprintf(noMaskCut,"%s&&%s",otherTreeCuts,azCut);
	sprintf(cftCut,"%s&&%s",otherTreeCuts,azCut);
}

TFile* pidFile = new TFile(pidFile_char);
TCutG* fragCut_adc = (TCutG*)pidFile->Get(fragCut_adc_char);
fragCut_adc->SetName("fragCut_adc");
fragCut_adc->SetLineColor(kViolet);
fragCut_adc->SetLineWidth(1);
fragCut_adc->SetFillColorAlpha(kViolet,0.2);
TCutG* alphaCut_above_zCut_adc = (TCutG*)pidFile->Get(above_zCut_adc_char);
alphaCut_above_zCut_adc->SetName("alphaCut_above_zCut_adc");
alphaCut_above_zCut_adc->SetLineColor(kSpring);
alphaCut_above_zCut_adc->SetLineWidth(2);
alphaCut_above_zCut_adc->SetFillColorAlpha(kSpring,0.2);
TCutG* alphaCut_long_low_adc = (TCutG*)pidFile->Get("alphaCut_long_low_adc");
alphaCut_long_low_adc->SetName("alphaCut_long_low_adc");
alphaCut_long_low_adc->SetLineColor(kYellow);
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
TCutG* alphaCut_above_zCut_energy = (TCutG*)pidFile->Get(above_zCut_energy_char);
alphaCut_above_zCut_energy->SetName("above_zCut_energy");
alphaCut_above_zCut_energy->SetLineColor(kSpring);
alphaCut_above_zCut_energy->SetLineWidth(2);
alphaCut_above_zCut_energy->SetFillColorAlpha(kSpring,0.2);
TCutG* alphaCut_long_low_energy = (TCutG*)pidFile->Get("alphaCut_long_low_energy");
alphaCut_long_low_energy->SetName("alphaCut_long_low_energy");
alphaCut_long_low_energy->SetLineColor(kYellow);
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

char noMaskFragCut[234];sprintf(noMaskFragCut,"%s&&%s", noMaskCut,"fragCut_adc&&adc>600");
char noMaskAlphaCut_above_zCut_adc_char[234];sprintf(noMaskAlphaCut_above_zCut_adc_char,"%s&&%s", noMaskCut,"alphaCut_above_zCut_adc");
char noMaskAlphaCut_long_char[234];sprintf(noMaskAlphaCut_long_char,"%s&&%s", noMaskCut,"alphaCut_long_adc");
char noMaskAlphaCut_long_low_char[234];sprintf(noMaskAlphaCut_long_low_char,"%s&&%s", noMaskCut,"alphaCut_long_low_adc");
char noMaskAlphaCut_long_high_adc_char[234];sprintf(noMaskAlphaCut_long_high_adc_char,"%s&&%s", noMaskCut,"long_high_adc");
char noMaskAlphaCut_short_higher_adc_char[234];sprintf(noMaskAlphaCut_short_higher_adc_char,"%s&&%s", noMaskCut,"alphaCut_short_higher_adc");
char noMaskAlphaCut_alphaCut_short_low_adc_char[234];sprintf(noMaskAlphaCut_alphaCut_short_low_adc_char,"%s&&%s", noMaskCut,"alphaCut_short_low_adc");
char cftFragCut[234];sprintf(cftFragCut,"%s&&%s", cftCut,"fragCut_adc&&adc>600");
char cftAlphaCut_above_zCut_adc_char[234];sprintf(cftAlphaCut_above_zCut_adc_char,"%s&&%s", cftCut,"alphaCut_above_zCut_adc");
char cftAlphaCut_long_char[234];sprintf(cftAlphaCut_long_char,"%s&&%s", cftCut,"alphaCut_long_adc");
char cftAlphaCut_long_low_char[234];sprintf(cftAlphaCut_long_low_char,"%s&&%s", cftCut,"alphaCut_long_low_adc");
char cftAlphaCut_long_high_adc_char[234];sprintf(cftAlphaCut_long_high_adc_char,"%s&&%s", cftCut,"long_high_adc");
char cftAlphaCut_short_higher_adc_char[234];sprintf(cftAlphaCut_short_higher_adc_char,"%s&&%s", cftCut,"alphaCut_short_higher_adc");
char cftAlphaCut_alphaCut_short_low_adc_char[234];sprintf(cftAlphaCut_alphaCut_short_low_adc_char,"%s&&%s", cftCut,"alphaCut_short_low_adc");
printf("noMaskCut = %s\n",noMaskCut);
printf("cftCut = %s\n",cftCut);
printf("otherTreeCuts = %s\n",otherTreeCuts);

////////////////////// noMask get track trees ///////////////////////////////////
printf("////////////////////// noMask get track trees ///////////////////////////////////\n");
TFile* cftFile = new TFile(cftFileName);
TTree* cftTracks = (TTree*)cftFile->Get("tracks");
TFile* noMaskFile = new TFile(noMaskFileName);
TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");

////////////////////// create ADC for TTrees ///////////////////////////////////
printf("////////////////////// create ADC for TTrees ///////////////////////////////////\n");
TH2D* noMask_ADCvsCosTheta = new TH2D("noMask_ADCvsCosTheta","noMask_ADCvsCosTheta",1000,-1,1,40000,0,40000);
TH2D* noMask_ADCvsCosTheta_Frags = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Frags");
TH2D* noMask_ADCvsCosTheta_Alphas = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas");
TH2D* noMask_ADCvsCosTheta_Alphas_long = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_long");
TH2D* noMask_ADCvsCosTheta_Alphas_long_low = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_long_low");
TH2D* noMask_ADCvsCosTheta_Alphas_long_high = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_long_high");
TH2D* noMask_ADCvsCosTheta_Alphas_short_higher = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_short_higher");
TH2D* noMask_ADCvsCosTheta_Alphas_alphaCut_short_low = (TH2D*)noMask_ADCvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_alphaCut_short_low");
TH2D* cft_ADCvsCosTheta = new TH2D("cft_ADCvsCosTheta","cft_ADCvsCosTheta",1000,-1,1,40000,0,40000);
TH2D* cft_ADCvsCosTheta_Frags = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Frags");
TH2D* cft_ADCvsCosTheta_Alphas = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Alphas");
TH2D* cft_ADCvsCosTheta_Alphas_long = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Alphas_long");
TH2D* cft_ADCvsCosTheta_Alphas_long_low = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Alphas_long_low");
TH2D* cft_ADCvsCosTheta_Alphas_long_high = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Alphas_long_high");
TH2D* cft_ADCvsCosTheta_Alphas_short_higher = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Alphas_short_higher");
TH2D* cft_ADCvsCosTheta_Alphas_alphaCut_short_low = (TH2D*)cft_ADCvsCosTheta->Clone("cft_ADCvsCosTheta_Alphas_alphaCut_short_low");
////////////////////// draw TTrees who pass length-energy TCutGs ///////////////////////////////////
printf("////////////////////// draw TTrees who pass length-energy TCutGs ///////////////////////////////////\n");
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta",noMaskCut);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Alphas",noMaskAlphaCut_above_zCut_adc_char);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Frags",noMaskFragCut);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Alphas_long",noMaskAlphaCut_long_char);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Alphas_long_low",noMaskAlphaCut_long_low_char);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Alphas_long_high",noMaskAlphaCut_long_high_adc_char);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Alphas_short_higher",noMaskAlphaCut_short_higher_adc_char);
noMaskTracks->Draw("adc:direction.CosTheta()>>noMask_ADCvsCosTheta_Alphas_alphaCut_short_low",noMaskAlphaCut_alphaCut_short_low_adc_char);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta",cftCut);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Alphas",cftAlphaCut_above_zCut_adc_char);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Frags",cftFragCut);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Alphas_long",cftAlphaCut_long_char);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Alphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Alphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Alphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("adc:direction.CosTheta()>>cft_ADCvsCosTheta_Alphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);

////////////////////// write ADCvsCosTheta's to file ///////////////////////////////////
printf("////////////////////// write ADCvsCosTheta's to file ///////////////////////////////////\n");
sprintf(buffChar,"$cft/polar/polarHistograms.premap%d.root",preamp);
TFile* polarHistograms = new TFile(buffChar,"recreate");
noMask_ADCvsCosTheta->Write();
noMask_ADCvsCosTheta->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta.png");
noMask_ADCvsCosTheta_Frags->Write();
noMask_ADCvsCosTheta_Frags->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Frags.png");
noMask_ADCvsCosTheta_Alphas->Write();
noMask_ADCvsCosTheta_Alphas->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Alphas.png");
noMask_ADCvsCosTheta_Alphas_long->Write();
noMask_ADCvsCosTheta_Alphas_long->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Alphas_long.png");
noMask_ADCvsCosTheta_Alphas_long_low->Write();
noMask_ADCvsCosTheta_Alphas_long_low->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Alphas_long_low.png");
noMask_ADCvsCosTheta_Alphas_long_high->Write();
noMask_ADCvsCosTheta_Alphas_long_high->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Alphas_long_high.png");
noMask_ADCvsCosTheta_Alphas_short_higher->Write();
noMask_ADCvsCosTheta_Alphas_short_higher->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Alphas_short_higher.png");
noMask_ADCvsCosTheta_Alphas_alphaCut_short_low->Write();
noMask_ADCvsCosTheta_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("noMask_ADCvsCosTheta_Alphas_alphaCut_short_low.png");
cft_ADCvsCosTheta->Write();
cft_ADCvsCosTheta->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta.png");
cft_ADCvsCosTheta_Frags->Write();
cft_ADCvsCosTheta_Frags->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Frags.png");
cft_ADCvsCosTheta_Alphas->Write();
cft_ADCvsCosTheta_Alphas->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Alphas.png");
cft_ADCvsCosTheta_Alphas_long->Write();
cft_ADCvsCosTheta_Alphas_long->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Alphas_long.png");
cft_ADCvsCosTheta_Alphas_long_low->Write();
cft_ADCvsCosTheta_Alphas_long_low->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Alphas_long_low.png");
cft_ADCvsCosTheta_Alphas_long_high->Write();
cft_ADCvsCosTheta_Alphas_long_high->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Alphas_long_high.png");
cft_ADCvsCosTheta_Alphas_short_higher->Write();
cft_ADCvsCosTheta_Alphas_short_higher->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Alphas_short_higher.png");
cft_ADCvsCosTheta_Alphas_alphaCut_short_low->Write();
cft_ADCvsCosTheta_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("cft_ADCvsCosTheta_Alphas_alphaCut_short_low.png");

////////////////////// create EnvsCosTheta containers ///////////////////////////////////
printf("////////////////////// create EnvsCosTheta containers ///////////////////////////////////\n");
TH2D* noMask_EnvsCosTheta = new TH2D("noMask_EnvsCosTheta","noMask_EnvsCosTheta",1000,-1,1,40000,0,40000*MeV_per_ADC);
noMask_EnvsCosTheta->GetXaxis()->SetTitle("cos(#theta)");
noMask_EnvsCosTheta->GetXaxis()->CenterTitle();
noMask_EnvsCosTheta->GetYaxis()->SetTitle("MeV");
noMask_EnvsCosTheta->GetYaxis()->CenterTitle();
TH2D* noMask_EnvsCosTheta_Frags = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Frags");
noMask_EnvsCosTheta_Frags->SetTitle("noMask_EnvsCosTheta_Frags");
TH2D* noMask_EnvsCosTheta_Alphas = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas");
noMask_EnvsCosTheta_Alphas->SetTitle("noMask_EnvsCosTheta_Alphas");
TH2D* noMask_EnvsCosTheta_Alphas_long = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_long");
noMask_EnvsCosTheta_Alphas_long->SetTitle("noMask_EnvsCosTheta_Alphas_long");
TH2D* noMask_EnvsCosTheta_Alphas_long_low = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_long_low");
noMask_EnvsCosTheta_Alphas_long_low->SetTitle("noMask_EnvsCosTheta_Alphas_long_low");
TH2D* noMask_EnvsCosTheta_Alphas_long_high = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_long_high");
noMask_EnvsCosTheta_Alphas_long_high->SetTitle("noMask_EnvsCosTheta_Alphas_long_high");
TH2D* noMask_EnvsCosTheta_Alphas_short_higher = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_short_higher");
noMask_EnvsCosTheta_Alphas_short_higher->SetTitle("noMask_EnvsCosTheta_Alphas_short_higher");
TH2D* noMask_EnvsCosTheta_Alphas_alphaCut_short_low = (TH2D*)noMask_EnvsCosTheta->Clone("noMask_ADCvsCosTheta_Alphas_alphaCut_short_low");
noMask_EnvsCosTheta_Alphas_alphaCut_short_low->SetTitle("noMask_EnvsCosTheta_Alphas_alphaCut_short_low");
TH2D* cft_EnvsCosTheta = new TH2D("cft_EnvsCosTheta","cft_EnvsCosTheta",1000,-1,1,40000,0,40000*MeV_per_ADC);
cft_EnvsCosTheta->GetXaxis()->SetTitle("cos(#theta)");
cft_EnvsCosTheta->GetXaxis()->CenterTitle();
cft_EnvsCosTheta->GetYaxis()->SetTitle("MeV");
cft_EnvsCosTheta->GetYaxis()->CenterTitle();
TH2D* cft_EnvsCosTheta_Frags = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Frags");
cft_EnvsCosTheta_Frags->SetTitle("cft_EnvsCosTheta_Frags");
TH2D* cft_EnvsCosTheta_Alphas = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Alphas");
cft_EnvsCosTheta_Alphas->SetTitle("cft_EnvsCosTheta_Alphas");
TH2D* cft_EnvsCosTheta_Alphas_long = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Alphas_long");
cft_EnvsCosTheta_Alphas_long->SetTitle("cft_EnvsCosTheta_Alphas_long");
TH2D* cft_EnvsCosTheta_Alphas_long_low = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Alphas_long_low");
cft_EnvsCosTheta_Alphas_long_low->SetTitle("cft_EnvsCosTheta_Alphas_long_low");
TH2D* cft_EnvsCosTheta_Alphas_long_high = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Alphas_long_high");
cft_EnvsCosTheta_Alphas_long_high->SetTitle("cft_EnvsCosTheta_Alphas_long_high");
TH2D* cft_EnvsCosTheta_Alphas_short_higher = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Alphas_short_higher");
cft_EnvsCosTheta_Alphas_short_higher->SetTitle("cft_EnvsCosTheta_Alphas_short_higher");
TH2D* cft_EnvsCosTheta_Alphas_alphaCut_short_low = (TH2D*)cft_EnvsCosTheta->Clone("cft_EnvsCosTheta_Alphas_alphaCut_short_low");
cft_EnvsCosTheta_Alphas_alphaCut_short_low->SetTitle("cft_EnvsCosTheta_Alphas_alphaCut_short_low");

////////////////////// create noMask cosTheta projectoins ///////////////////////////////////
printf("////////////////////// create noMask cosTheta projectoins ///////////////////////////////////\n");
TH1D* noMask_CosTheta = noMask_ADCvsCosTheta->ProjectionX("noMask_CosTheta");
noMask_CosTheta->SetTitle("noMask_CosTheta");
noMask_CosTheta->Draw();
c1->SaveAs("noMask_CosTheta.png");
noMask_CosTheta->SetTitle("noMask_CosTheta");
TH1D* noMask_Frags_CosTheta = noMask_ADCvsCosTheta_Frags->ProjectionX("noMask_Frags_CosTheta");
noMask_Frags_CosTheta->SetLineColor(kViolet);
noMask_Frags_CosTheta->SetLineWidth((Width_t)0.0);
noMask_Frags_CosTheta->SetFillColorAlpha(kViolet,0.9);
noMask_Frags_CosTheta->SetTitle("noMask_Frags_CosTheta");
noMask_Frags_CosTheta->Draw();
c1->SaveAs("noMask_Frags_CosTheta.png");
noMask_Frags_CosTheta->SetTitle("noMask_Frags_CosTheta");
TH1D* noMask_Alphas_CosTheta = noMask_ADCvsCosTheta_Alphas->ProjectionX("noMask_Alphas_CosTheta");
noMask_Alphas_CosTheta->SetLineColor(kSpring);
noMask_Alphas_CosTheta->SetLineWidth((Width_t)0.0);
noMask_Alphas_CosTheta->SetFillColorAlpha(kSpring,0.9);
noMask_Alphas_CosTheta->SetTitle("noMask_Alphas_CosTheta");
noMask_Alphas_CosTheta->Draw();
c1->SaveAs("noMask_Alphas_CosTheta.png");
noMask_Alphas_CosTheta->SetTitle("noMask_Alphas_CosTheta");
TH1D* noMask_CosTheta_long = noMask_ADCvsCosTheta_Alphas_long->ProjectionX("noMask_CosTheta_long");
noMask_CosTheta_long->SetLineColor(kCyan);
noMask_CosTheta_long->SetLineWidth((Width_t)0.0);
noMask_CosTheta_long->SetFillColorAlpha(kCyan,0.9);
noMask_CosTheta_long->SetTitle("noMask_CosTheta_long");
noMask_CosTheta_long->Draw();
c1->SaveAs("noMask_CosTheta_long.png");
noMask_CosTheta_long->SetTitle("noMask_CosTheta_long");
TH1D* noMask_CosTheta_long_low = noMask_ADCvsCosTheta_Alphas_long_low->ProjectionX("noMask_CosTheta_long_low");
noMask_CosTheta_long_low->SetLineColor(kYellow);
noMask_CosTheta_long_low->SetLineWidth((Width_t)0.0);
noMask_CosTheta_long_low->SetFillColorAlpha(kYellow,0.9);
noMask_CosTheta_long_low->SetTitle("noMask_CosTheta_long_low");
noMask_CosTheta_long_low->Draw();
c1->SaveAs("noMask_CosTheta_long_low.png");
noMask_CosTheta_long_low->SetTitle("noMask_CosTheta_long_low");
TH1D* noMask_CosTheta_long_high = noMask_ADCvsCosTheta_Alphas_long_high->ProjectionX("noMask_CosTheta_long_high");
noMask_CosTheta_long_high->SetLineColor(kBlue);
noMask_CosTheta_long_high->SetLineWidth((Width_t)0.0);
noMask_CosTheta_long_high->SetFillColorAlpha(kBlue,0.9);
noMask_CosTheta_long_high->SetTitle("noMask_CosTheta_long_high");
noMask_CosTheta_long_high->Draw();
c1->SaveAs("noMask_CosTheta_long_high.png");
noMask_CosTheta_long_high->SetTitle("noMask_CosTheta_long_high");
TH1D* noMask_CosTheta_short_higher = noMask_ADCvsCosTheta_Alphas_short_higher->ProjectionX("noMask_CosTheta_short_higher");
noMask_CosTheta_short_higher->SetLineColor(kOrange+7);
noMask_CosTheta_short_higher->SetLineWidth((Width_t)0.01);
noMask_CosTheta_short_higher->SetFillColorAlpha(kOrange+7,0.9);
noMask_CosTheta_short_higher->SetTitle("noMask_CosTheta_short_higher");
noMask_CosTheta_short_higher->Draw();
c1->SaveAs("noMask_CosTheta_short_higher.png");
noMask_CosTheta_short_higher->SetTitle("noMask_CosTheta_short_higher");
TH1D* noMask_CosTheta_alphaCut_short_low = noMask_ADCvsCosTheta_Alphas_alphaCut_short_low->ProjectionX("noMask_CosTheta_alphaCut_short_low");
noMask_CosTheta_alphaCut_short_low->SetLineColor(kRed);
noMask_CosTheta_alphaCut_short_low->SetLineWidth((Width_t)0.0);
noMask_CosTheta_alphaCut_short_low->SetFillColorAlpha(kRed,0.9);
noMask_CosTheta_alphaCut_short_low->SetTitle("noMask_CosTheta_alphaCut_short_low");
noMask_CosTheta_alphaCut_short_low->Draw();
c1->SaveAs("noMask_CosTheta_alphaCut_short_low.png");
noMask_CosTheta_alphaCut_short_low->SetTitle("noMask_CosTheta_alphaCut_short_low");
////////////////////// create cft cosTheta projectoins ///////////////////////////////////
printf("////////////////////// create cft cosTheta projectoins ///////////////////////////////////\n");
TH1D* cft_CosTheta = cft_ADCvsCosTheta->ProjectionX("cft_CosTheta");
cft_CosTheta->SetTitle("cft_CosTheta");
cft_CosTheta->Draw();
c1->SaveAs("cft_CosTheta.png");
cft_CosTheta->SetTitle("cft_CosTheta");
TH1D* cft_Frags_CosTheta = cft_ADCvsCosTheta_Frags->ProjectionX("cft_Frags_CosTheta");
cft_Frags_CosTheta->SetLineColor(kViolet);
cft_Frags_CosTheta->SetLineWidth((Width_t)0.0);
cft_Frags_CosTheta->SetFillColorAlpha(kViolet,0.9);
cft_Frags_CosTheta->SetTitle("cft_Frags_CosTheta");
cft_Frags_CosTheta->Draw();
c1->SaveAs("cft_Frags_CosTheta.png");
cft_Frags_CosTheta->SetTitle("cft_Frags_CosTheta");
TH1D* cft_Alphas_CosTheta = cft_ADCvsCosTheta_Alphas->ProjectionX("cft_Alphas_CosTheta");
cft_Alphas_CosTheta->SetLineColor(kSpring);
cft_Alphas_CosTheta->SetLineWidth((Width_t)0.0);
cft_Alphas_CosTheta->SetFillColorAlpha(kSpring,0.9);
cft_Alphas_CosTheta->SetTitle("cft_Alphas_CosTheta");
cft_Alphas_CosTheta->Draw();
c1->SaveAs("cft_Alphas_CosTheta.png");
cft_Alphas_CosTheta->SetTitle("cft_Alphas_CosTheta");
TH1D* cft_CosTheta_long = cft_ADCvsCosTheta_Alphas_long->ProjectionX("cft_CosTheta_long");
cft_CosTheta_long->SetLineColor(kCyan);
cft_CosTheta_long->SetLineWidth((Width_t)0.0);
cft_CosTheta_long->SetFillColorAlpha(kCyan,0.9);
cft_CosTheta_long->SetTitle("cft_CosTheta_long");
cft_CosTheta_long->Draw();
c1->SaveAs("cft_CosTheta_long.png");
cft_CosTheta_long->SetTitle("cft_CosTheta_long");
TH1D* cft_CosTheta_long_low = cft_ADCvsCosTheta_Alphas_long_low->ProjectionX("cft_CosTheta_long_low");
cft_CosTheta_long_low->SetLineColor(kYellow);
cft_CosTheta_long_low->SetLineWidth((Width_t)0.0);
cft_CosTheta_long_low->SetFillColorAlpha(kYellow,0.9);
cft_CosTheta_long_low->SetTitle("cft_CosTheta_long_low");
cft_CosTheta_long_low->Draw();
c1->SaveAs("cft_CosTheta_long_low.png");
cft_CosTheta_long_low->SetTitle("cft_CosTheta_long_low");
TH1D* cft_CosTheta_long_high = cft_ADCvsCosTheta_Alphas_long_high->ProjectionX("cft_CosTheta_long_high");
cft_CosTheta_long_high->SetLineColor(kBlue);
cft_CosTheta_long_high->SetLineWidth((Width_t)0.0);
cft_CosTheta_long_high->SetFillColorAlpha(kBlue,0.9);
cft_CosTheta_long_high->SetTitle("cft_CosTheta_long_high");
cft_CosTheta_long_high->Draw();
c1->SaveAs("cft_CosTheta_long_high.png");
cft_CosTheta_long_high->SetTitle("cft_CosTheta_long_high");
TH1D* cft_CosTheta_short_higher = cft_ADCvsCosTheta_Alphas_short_higher->ProjectionX("cft_CosTheta_short_higher");
cft_CosTheta_short_higher->SetLineColor(kOrange+7);
cft_CosTheta_short_higher->SetLineWidth((Width_t)0.01);
cft_CosTheta_short_higher->SetFillColorAlpha(kOrange+7,0.9);
cft_CosTheta_short_higher->SetTitle("cft_CosTheta_short_higher");
cft_CosTheta_short_higher->Draw();
c1->SaveAs("cft_CosTheta_short_higher.png");
cft_CosTheta_short_higher->SetTitle("cft_CosTheta_short_higher");
TH1D* cft_CosTheta_alphaCut_short_low = cft_ADCvsCosTheta_Alphas_alphaCut_short_low->ProjectionX("cft_CosTheta_alphaCut_short_low");
cft_CosTheta_alphaCut_short_low->SetLineColor(kRed);
cft_CosTheta_alphaCut_short_low->SetLineWidth((Width_t)0.0);
cft_CosTheta_alphaCut_short_low->SetFillColorAlpha(kRed,0.9);
cft_CosTheta_alphaCut_short_low->SetTitle("cft_CosTheta_alphaCut_short_low");
cft_CosTheta_alphaCut_short_low->Draw();
c1->SaveAs("cft_CosTheta_alphaCut_short_low.png");
cft_CosTheta_alphaCut_short_low->SetTitle("cft_CosTheta_alphaCut_short_low");

for (int cosThetaBin = 0; cosThetaBin < 1000; ++cosThetaBin)
{
	for (int adcBin = 0; adcBin < 40000; ++adcBin)
	{
		noMask_EnvsCosTheta->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Alphas->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Alphas->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Frags->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Frags->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Alphas_long->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Alphas_long->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Alphas_long_low->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Alphas_long_low->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Alphas_long_high->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Alphas_long_high->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Alphas_short_higher->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Alphas_short_higher->GetBinContent(cosThetaBin,adcBin));
		noMask_EnvsCosTheta_Alphas_alphaCut_short_low->SetBinContent(cosThetaBin,adcBin,noMask_ADCvsCosTheta_Alphas_alphaCut_short_low->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Alphas->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Alphas->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Frags->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Frags->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Alphas_long->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Alphas_long->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Alphas_long_low->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Alphas_long_low->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Alphas_long_high->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Alphas_long_high->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Alphas_short_higher->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Alphas_short_higher->GetBinContent(cosThetaBin,adcBin));
		cft_EnvsCosTheta_Alphas_alphaCut_short_low->SetBinContent(cosThetaBin,adcBin,cft_ADCvsCosTheta_Alphas_alphaCut_short_low->GetBinContent(cosThetaBin,adcBin));
	}
}
	
////////////////////// noMask fill color coded length-ADC containers ///////////////////////////////////
printf("////////////////////// noMask fill color coded length-ADC containers ///////////////////////////////////\n");
c1->SetGridx(1);c1->SetGridy(1);
c1->SetLogy(0);c1->SetLogz(1);
noMask_EnvsCosTheta->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta.png");
noMask_EnvsCosTheta_Alphas->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Alphas->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta_Alphas.png");
noMask_EnvsCosTheta_Frags->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Frags->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta_Frags.png");
noMask_EnvsCosTheta_Alphas_long->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Alphas_long->Draw("colz"); 
c1->SaveAs("noMask_EnvsCosTheta_Alphas_long.png");
noMask_EnvsCosTheta_Alphas_long_low->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Alphas_long_low->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta_Alphas_long_low.png");
noMask_EnvsCosTheta_Alphas_long_high->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Alphas_long_high->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta_Alphas_long_high.png");
noMask_EnvsCosTheta_Alphas_short_higher->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Alphas_short_higher->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta_Alphas_short_higher.png");
noMask_EnvsCosTheta_Alphas_alphaCut_short_low->GetYaxis()->SetRangeUser(0.4,20);
noMask_EnvsCosTheta_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("noMask_EnvsCosTheta_Alphas_alphaCut_short_low.png");
////////////////////// cft fill color coded length-ADC containers ///////////////////////////////////
printf("////////////////////// cft fill color coded length-ADC containers ///////////////////////////////////\n");
c1->SetGridx(1);c1->SetGridy(1);
c1->SetLogy(0);c1->SetLogz(1);
cft_EnvsCosTheta->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta.png");
cft_EnvsCosTheta_Alphas->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Alphas->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta_Alphas.png");
cft_EnvsCosTheta_Frags->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Frags->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta_Frags.png");
cft_EnvsCosTheta_Alphas_long->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Alphas_long->Draw("colz"); 
c1->SaveAs("cft_EnvsCosTheta_Alphas_long.png");
cft_EnvsCosTheta_Alphas_long_low->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Alphas_long_low->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta_Alphas_long_low.png");
cft_EnvsCosTheta_Alphas_long_high->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Alphas_long_high->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta_Alphas_long_high.png");
cft_EnvsCosTheta_Alphas_short_higher->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Alphas_short_higher->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta_Alphas_short_higher.png");
cft_EnvsCosTheta_Alphas_alphaCut_short_low->GetYaxis()->SetRangeUser(0.4,20);
cft_EnvsCosTheta_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("cft_EnvsCosTheta_Alphas_alphaCut_short_low.png");

/////////////////////////////// noMask create colorcoded ADC projection stack ///////////////////////////////////
printf("////////////////////// noMask create colorcoded ADC projection stack ///////////////////////////////////\n");
TLegend* noMask_pidCuts_stack_legend = new TLegend(0.75,0.65,0.9,0.9);
noMask_pidCuts_stack_legend->SetTextFont(42);
noMask_pidCuts_stack_legend->SetTextSize(0.03);
noMask_pidCuts_stack_legend->AddEntry(noMask_Alphas_CosTheta);
noMask_pidCuts_stack_legend->AddEntry(noMask_Frags_CosTheta);
noMask_pidCuts_stack_legend->AddEntry(noMask_CosTheta_alphaCut_short_low);
noMask_pidCuts_stack_legend->AddEntry(noMask_CosTheta_short_higher);
noMask_pidCuts_stack_legend->AddEntry(noMask_CosTheta_long);
noMask_pidCuts_stack_legend->AddEntry(noMask_CosTheta_long_low);
noMask_pidCuts_stack_legend->AddEntry(noMask_CosTheta_long_high);
THStack* noMask_pidCuts_CosTheta_Stack = new THStack("noMask_pidCuts_CosTheta_Stack","");
noMask_pidCuts_CosTheta_Stack->Add(noMask_Alphas_CosTheta);
noMask_pidCuts_CosTheta_Stack->Add(noMask_Frags_CosTheta);
noMask_pidCuts_CosTheta_Stack->Add(noMask_CosTheta_alphaCut_short_low);
noMask_pidCuts_CosTheta_Stack->Add(noMask_CosTheta_short_higher);
noMask_pidCuts_CosTheta_Stack->Add(noMask_CosTheta_long);
noMask_pidCuts_CosTheta_Stack->Add(noMask_CosTheta_long_low);
noMask_pidCuts_CosTheta_Stack->Add(noMask_CosTheta_long_high);
noMask_pidCuts_CosTheta_Stack->SetMinimum(0.5);
noMask_pidCuts_CosTheta_Stack->Draw();
c1->SetLogx(0);c1->SetLogy(0);
noMask_pidCuts_CosTheta_Stack->GetXaxis()->SetTitle("cos(#theta)");
noMask_pidCuts_CosTheta_Stack->GetXaxis()->SetTitle();
noMask_pidCuts_CosTheta_Stack->GetYaxis()->SetRangeUser(0.4,20);
noMask_pidCuts_CosTheta_Stack->SetTitle("noMask_pidCuts_CosTheta_Stack");
noMask_pidCuts_CosTheta_Stack->Draw();
c1->SaveAs("noMask_pidCuts_CosTheta_Stack_lowEn.png");
c1->SetLogx(0);c1->SetLogy(1);
noMask_pidCuts_CosTheta_Stack->SetTitle("noMask_pidCuts_CosTheta_Stack");
noMask_pidCuts_CosTheta_Stack->Draw();
noMask_pidCuts_CosTheta_Stack->GetYaxis()->SetRangeUser(0.4,160);
noMask_pidCuts_CosTheta_Stack->Draw();
c1->Update();
c1->SaveAs("noMask_pidCuts_CosTheta_Stack_fullEn.png");
/////////////////////////////// cft create colorcoded ADC projection stack ///////////////////////////////////
printf("////////////////////// cft create colorcoded ADC projection stack ///////////////////////////////////\n");
TLegend* cft_pidCuts_stack_legend = new TLegend(0.75,0.65,0.9,0.9);
cft_pidCuts_stack_legend->SetTextFont(42);
cft_pidCuts_stack_legend->SetTextSize(0.03);
cft_pidCuts_stack_legend->AddEntry(cft_Alphas_CosTheta);
cft_pidCuts_stack_legend->AddEntry(cft_Frags_CosTheta);
cft_pidCuts_stack_legend->AddEntry(cft_CosTheta_alphaCut_short_low);
cft_pidCuts_stack_legend->AddEntry(cft_CosTheta_short_higher);
cft_pidCuts_stack_legend->AddEntry(cft_CosTheta_long);
cft_pidCuts_stack_legend->AddEntry(cft_CosTheta_long_low);
cft_pidCuts_stack_legend->AddEntry(cft_CosTheta_long_high);
THStack* cft_pidCuts_CosTheta_Stack = new THStack("cft_pidCuts_CosTheta_Stack","");
cft_pidCuts_CosTheta_Stack->Add(cft_Alphas_CosTheta);
cft_pidCuts_CosTheta_Stack->Add(cft_Frags_CosTheta);
cft_pidCuts_CosTheta_Stack->Add(cft_CosTheta_alphaCut_short_low);
cft_pidCuts_CosTheta_Stack->Add(cft_CosTheta_short_higher);
cft_pidCuts_CosTheta_Stack->Add(cft_CosTheta_long);
cft_pidCuts_CosTheta_Stack->Add(cft_CosTheta_long_low);
cft_pidCuts_CosTheta_Stack->Add(cft_CosTheta_long_high);
cft_pidCuts_CosTheta_Stack->SetMinimum(0.5);
cft_pidCuts_CosTheta_Stack->Draw();
c1->SetLogx(0);c1->SetLogy(0);
cft_pidCuts_CosTheta_Stack->GetXaxis()->SetTitle("cos(#theta)");
cft_pidCuts_CosTheta_Stack->GetXaxis()->SetTitle();
cft_pidCuts_CosTheta_Stack->GetYaxis()->SetRangeUser(0.4,20);
cft_pidCuts_CosTheta_Stack->SetTitle("cft_pidCuts_CosTheta_Stack");
cft_pidCuts_CosTheta_Stack->Draw();
c1->SaveAs("cft_pidCuts_CosTheta_Stack_lowEn.png");
c1->SetLogx(0);c1->SetLogy(1);
cft_pidCuts_CosTheta_Stack->SetTitle("cft_pidCuts_CosTheta_Stack");
cft_pidCuts_CosTheta_Stack->Draw();
cft_pidCuts_CosTheta_Stack->GetYaxis()->SetRangeUser(0.4,160);
cft_pidCuts_CosTheta_Stack->Draw();
c1->Update();
c1->SaveAs("cft_pidCuts_CosTheta_Stack_fullEn.png");

}
