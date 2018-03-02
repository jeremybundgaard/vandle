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


void vertexCft(const int preamp, const char* actinide, const char* noMaskFileName="", const char* cftFileName="", const char* otherTreeCuts="1",const char* pidType="",const char* driftVel="") {

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
char c1PlotName[234];
gStyle->SetOptStat(0);
gStyle->SetPalette(71);
gStyle->SetNumberContours(999);
char buffChar[345];

double MeV_per_ADC=0;
double ADC_per_MeV=0;
double dEnergy = 0.1;
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
		sprintf(above_zCut_adc_char,"noMaskLvEn_above_zCut100_adc");
		sprintf(fragCut_energy_char,"fragCut_energy_preamp%d_4stdDev",preamp);
		sprintf(alphaCut_energy_char,"alphaCut_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy_preamp%d_4stdDev",preamp);
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(above_zCut_energy_char,"noMaskLvEn_above_zCut100_energy");
	}
	if (!(strcmp(pidType,"zCut"))){
		sprintf(pidFile_char,"$cft/pid/pidCut_zCut%d.%s.root",preamp,driftVel);
  		sprintf(fragCut_adc_char,"fragCut_adc");
		sprintf(alphaCut_adc_char,"alphaCut_adc");
		sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
		sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
		sprintf(above_zCut_adc_char,"alphaCut_zCut100_adc");
		sprintf(fragCut_energy_char,"fragCut_energy");
		sprintf(alphaCut_energy_char,"alphaCut_energy");
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
		sprintf(above_zCut_energy_char,"alphaCut_zCut100_energy");
	}
	if (!(strcmp(actinide,"cf252"))){}
	sprintf(noMaskRunCut,"run>100001856&&run!=100001870");
	sprintf(noMaskCut," %s&&%s",otherTreeCuts,noMaskRunCut);
	sprintf(cftCut,"%s",otherTreeCuts);
}
else if (preamp==2)
{
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
		sprintf(above_zCut_adc_char,"noMaskLvEn_above_zCut040_adc");
		sprintf(fragCut_energy_char,"fragCut_energy_preamp%d_4stdDev",preamp);
		sprintf(alphaCut_energy_char,"alphaCut_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy_preamp%d_4stdDev",preamp);
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy_preamp%d_expoFit_4stdDev",preamp);
		sprintf(above_zCut_energy_char,"noMaskLvEn_above_zCut040_energy");
	}
	if (!(strcmp(pidType,"zCut"))){
		sprintf(pidFile_char,"$cft/pid/pidCut_zCut%d.%s.root",preamp,driftVel);
		sprintf(fragCut_adc_char,"fragCut_adc");
		sprintf(alphaCut_adc_char,"alphaCut_adc");
		sprintf(alpha_short_high_adc_char,"alphaCut_short_higher_adc");
		sprintf(alpha_short_low_adc_char,"alphaCut_short_low_adc");
		sprintf(above_zCut_adc_char,"alphaCut_zCut040_adc");
		sprintf(fragCut_energy_char,"fragCut_energy");
		sprintf(alphaCut_energy_char,"alphaCut_energy");
		sprintf(alpha_short_high_energy_char,"alphaCut_short_higher_energy");
		sprintf(alpha_short_low_energy_char,"alphaCut_short_low_energy");
		sprintf(above_zCut_energy_char,"alphaCut_zCut040_energy");
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
TH2D* noMask_xyStart = new TH2D("noMask_xyStart","noMask_xyStart",500,-6,6,500,-6,6);
noMask_xyStart->GetXaxis()->SetTitle("x_start [cm]");
noMask_xyStart->GetYaxis()->SetTitle("y_start [cm]");
TH2D* noMask_xyStart_Frags = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Frags");
noMask_xyStart_Frags->SetTitle("noMask_xyStart_Frags");
TH2D* noMask_xyStart_Alphas = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Alphas");
noMask_xyStart_Alphas->SetTitle("noMask_xyStart_Alphas");
TH2D* noMask_xyStart_Alphas_long = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Alphas_long");
noMask_xyStart_Alphas_long->SetTitle("noMask_xyStart_Alphas_long");
TH2D* noMask_xyStart_Alphas_long_low = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Alphas_long_low");
noMask_xyStart_Alphas_long_low->SetTitle("noMask_xyStart_Alphas_long_low");
TH2D* noMask_xyStart_Alphas_long_high = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Alphas_long_high");
noMask_xyStart_Alphas_long_high->SetTitle("noMask_xyStart_Alphas_long_high");
TH2D* noMask_xyStart_Alphas_short_higher = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Alphas_short_higher");
noMask_xyStart_Alphas_short_higher->SetTitle("noMask_xyStart_Alphas_short_higher");
TH2D* noMask_xyStart_Alphas_alphaCut_short_low = (TH2D*)noMask_xyStart->Clone("noMask_xyStart_Alphas_alphaCut_short_low");
noMask_xyStart_Alphas_alphaCut_short_low->SetTitle("noMask_xyStart_Alphas_alphaCut_short_low");
TH2D* cft_xyStart = new TH2D("cft_xyStart","cft_xyStart",500,-6,6,500,-6,6);
cft_xyStart->GetXaxis()->SetTitle("x_start [cm]");
cft_xyStart->GetYaxis()->SetTitle("y_start [cm]");
TH2D* cft_xyStart_Frags = (TH2D*)cft_xyStart->Clone("cft_xyStart_Frags");
cft_xyStart_Frags->SetTitle("cft_xyStart_Frags");
TH2D* cft_xyStart_Alphas = (TH2D*)cft_xyStart->Clone("cft_xyStart_Alphas");
cft_xyStart_Alphas->SetTitle("cft_xyStart_Alphas");
TH2D* cft_xyStart_Alphas_long = (TH2D*)cft_xyStart->Clone("cft_xyStart_Alphas_long");
cft_xyStart_Alphas_long->SetTitle("cft_xyStart_Alphas_long");
TH2D* cft_xyStart_Alphas_long_low = (TH2D*)cft_xyStart->Clone("cft_xyStart_Alphas_long_low");
cft_xyStart_Alphas_long_low->SetTitle("cft_xyStart_Alphas_long_low");
TH2D* cft_xyStart_Alphas_long_high = (TH2D*)cft_xyStart->Clone("cft_xyStart_Alphas_long_high");
cft_xyStart_Alphas_long_high->SetTitle("cft_xyStart_Alphas_long_high");
TH2D* cft_xyStart_Alphas_short_higher = (TH2D*)cft_xyStart->Clone("cft_xyStart_Alphas_short_higher");
cft_xyStart_Alphas_short_higher->SetTitle("cft_xyStart_Alphas_short_higher");
TH2D* cft_xyStart_Alphas_alphaCut_short_low = (TH2D*)cft_xyStart->Clone("cft_xyStart_Alphas_alphaCut_short_low");
cft_xyStart_Alphas_alphaCut_short_low->SetTitle("cft_xyStart_Alphas_alphaCut_short_low");
////////////////////// draw TTrees who pass length-energy TCutGs ///////////////////////////////////
printf("////////////////////// draw TTrees who pass length-energy TCutGs ///////////////////////////////////\n");
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart",noMaskCut);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Alphas",noMaskAlphaCut_above_zCut_adc_char);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Frags",noMaskFragCut);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Alphas_long",noMaskAlphaCut_long_char);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Alphas_long_low",noMaskAlphaCut_long_low_char);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Alphas_long_high",noMaskAlphaCut_long_high_adc_char);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Alphas_short_higher",noMaskAlphaCut_short_higher_adc_char);
noMaskTracks->Draw("start.x():start.y()>>noMask_xyStart_Alphas_alphaCut_short_low",noMaskAlphaCut_alphaCut_short_low_adc_char);
cftTracks->Draw("start.x():start.y()>>cft_xyStart",cftCut);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Alphas",cftAlphaCut_above_zCut_adc_char);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Frags",cftFragCut);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Alphas_long",cftAlphaCut_long_char);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Alphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Alphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Alphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("start.x():start.y()>>cft_xyStart_Alphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);

////////////////////// create ADC for TTrees ///////////////////////////////////
printf("////////////////////// create ADC for TTrees ///////////////////////////////////\n");
TH2D* noMask_xyEnd = new TH2D("noMask_xyEnd","noMask_xyEnd",500,-6,6,500,-6,6);
noMask_xyEnd->GetXaxis()->SetTitle("x_end [cm]");
noMask_xyEnd->GetYaxis()->SetTitle("y_end [cm]");
TH2D* noMask_xyEnd_Frags = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Frags");
noMask_xyEnd_Frags->SetTitle("noMask_xyEnd_Frags");
TH2D* noMask_xyEnd_Alphas = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Alphas");
noMask_xyEnd_Alphas->SetTitle("noMask_xyEnd_Alphas");
TH2D* noMask_xyEnd_Alphas_long = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Alphas_long");
noMask_xyEnd_Alphas_long->SetTitle("noMask_xyEnd_Alphas_long");
TH2D* noMask_xyEnd_Alphas_long_low = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Alphas_long_low");
noMask_xyEnd_Alphas_long_low->SetTitle("noMask_xyEnd_Alphas_long_low");
TH2D* noMask_xyEnd_Alphas_long_high = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Alphas_long_high");
noMask_xyEnd_Alphas_long_high->SetTitle("noMask_xyEnd_Alphas_long_high");
TH2D* noMask_xyEnd_Alphas_short_higher = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Alphas_short_higher");
noMask_xyEnd_Alphas_short_higher->SetTitle("noMask_xyEnd_Alphas_short_higher");
TH2D* noMask_xyEnd_Alphas_alphaCut_short_low = (TH2D*)noMask_xyEnd->Clone("noMask_xyEnd_Alphas_alphaCut_short_low");
noMask_xyEnd_Alphas_alphaCut_short_low->SetTitle("noMask_xyEnd_Alphas_alphaCut_short_low");
TH2D* cft_xyEnd = new TH2D("cft_xyEnd","cft_xyEnd",500,-6,6,500,-6,6);
cft_xyEnd->GetXaxis()->SetTitle("x_end [cm]");
cft_xyEnd->GetYaxis()->SetTitle("y_end [cm]");
TH2D* cft_xyEnd_Frags = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Frags");
cft_xyEnd_Frags->SetTitle("cft_xyEnd_Frags");
TH2D* cft_xyEnd_Alphas = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Alphas");
cft_xyEnd_Alphas->SetTitle("cft_xyEnd_Alphas");
TH2D* cft_xyEnd_Alphas_long = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Alphas_long");
cft_xyEnd_Alphas_long->SetTitle("cft_xyEnd_Alphas_long");
TH2D* cft_xyEnd_Alphas_long_low = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Alphas_long_low");
cft_xyEnd_Alphas_long_low->SetTitle("cft_xyEnd_Alphas_long_low");
TH2D* cft_xyEnd_Alphas_long_high = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Alphas_long_high");
cft_xyEnd_Alphas_long_high->SetTitle("cft_xyEnd_Alphas_long_high");
TH2D* cft_xyEnd_Alphas_short_higher = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Alphas_short_higher");
cft_xyEnd_Alphas_short_higher->SetTitle("cft_xyEnd_Alphas_short_higher");
TH2D* cft_xyEnd_Alphas_alphaCut_short_low = (TH2D*)cft_xyEnd->Clone("cft_xyEnd_Alphas_alphaCut_short_low");
cft_xyEnd_Alphas_alphaCut_short_low->SetTitle("cft_xyEnd_Alphas_alphaCut_short_low");
////////////////////// draw TTrees who pass length-energy TCutGs ///////////////////////////////////
printf("////////////////////// draw TTrees who pass length-energy TCutGs ///////////////////////////////////\n");
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd",noMaskCut);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Alphas",noMaskAlphaCut_above_zCut_adc_char);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Frags",noMaskFragCut);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Alphas_long",noMaskAlphaCut_long_char);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Alphas_long_low",noMaskAlphaCut_long_low_char);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Alphas_long_high",noMaskAlphaCut_long_high_adc_char);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Alphas_short_higher",noMaskAlphaCut_short_higher_adc_char);
noMaskTracks->Draw("end.x():end.y()>>noMask_xyEnd_Alphas_alphaCut_short_low",noMaskAlphaCut_alphaCut_short_low_adc_char);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd",cftCut);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Alphas",cftAlphaCut_above_zCut_adc_char);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Frags",cftFragCut);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Alphas_long",cftAlphaCut_long_char);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Alphas_long_low",cftAlphaCut_long_low_char);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Alphas_long_high",cftAlphaCut_long_high_adc_char);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Alphas_short_higher",cftAlphaCut_short_higher_adc_char);
cftTracks->Draw("end.x():end.y()>>cft_xyEnd_Alphas_alphaCut_short_low",cftAlphaCut_alphaCut_short_low_adc_char);

////////////////////// Draw xyStart's ///////////////////////////////////
printf("////////////////////// Draw xyStart's ///////////////////////////////////\n");
c1->SetLogz(0);
noMask_xyStart->Draw("colz");
c1->SaveAs("noMask_xyStart_linz.png");
noMask_xyStart_Frags->Draw("colz");
c1->SaveAs("noMask_xyStart_Frags_linz.png");
noMask_xyStart_Alphas->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_linz.png");
noMask_xyStart_Alphas_long->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_long_linz.png");
noMask_xyStart_Alphas_long_low->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_long_low_linz.png");
noMask_xyStart_Alphas_long_high->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_long_high_linz.png");
noMask_xyStart_Alphas_short_higher->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_short_higher_linz.png");
noMask_xyStart_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_alphaCut_short_low_linz.png");
cft_xyStart->Draw("colz");
c1->SaveAs("cft_xyStart_linz.png");
cft_xyStart_Frags->Draw("colz");
c1->SaveAs("cft_xyStart_Frags_linz.png");
cft_xyStart_Alphas->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_linz.png");
cft_xyStart_Alphas_long->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_long_linz.png");
cft_xyStart_Alphas_long_low->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_long_low_linz.png");
cft_xyStart_Alphas_long_high->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_long_high_linz.png");
cft_xyStart_Alphas_short_higher->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_short_higher_linz.png");
cft_xyStart_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_alphaCut_short_low_linz.png");
////////////////////// Draw xyEnd's ///////////////////////////////////
printf("////////////////////// Draw xyEnd's ///////////////////////////////////\n");
noMask_xyEnd->Draw("colz");
c1->SaveAs("noMask_xyEnd_linz.png");
noMask_xyEnd_Frags->Draw("colz");
c1->SaveAs("noMask_xyEnd_Frags_linz.png");
noMask_xyEnd_Alphas->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_linz.png");
noMask_xyEnd_Alphas_long->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_long_linz.png");
noMask_xyEnd_Alphas_long_low->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_long_low_linz.png");
noMask_xyEnd_Alphas_long_high->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_long_high_linz.png");
noMask_xyEnd_Alphas_short_higher->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_short_higher_linz.png");
noMask_xyEnd_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_alphaCut_short_low_linz.png");
cft_xyEnd->Draw("colz");
c1->SaveAs("cft_xyEnd_linz.png");
cft_xyEnd_Frags->Draw("colz");
c1->SaveAs("cft_xyEnd_Frags_linz.png");
cft_xyEnd_Alphas->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_linz.png");
cft_xyEnd_Alphas_long->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_long_linz.png");
cft_xyEnd_Alphas_long_low->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_long_low_linz.png");
cft_xyEnd_Alphas_long_high->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_long_high_linz.png");
cft_xyEnd_Alphas_short_higher->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_short_higher_linz.png");
cft_xyEnd_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_alphaCut_short_low_linz.png");

////////////////////// Draw xyStart's ///////////////////////////////////
printf("////////////////////// Draw xyStart's ///////////////////////////////////\n");
c1->SetLogz(1);
noMask_xyStart->Draw("colz");
c1->SaveAs("noMask_xyStart_logz.png");
noMask_xyStart_Frags->Draw("colz");
c1->SaveAs("noMask_xyStart_Frags_logz.png");
noMask_xyStart_Alphas->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_logz.png");
noMask_xyStart_Alphas_long->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_long_logz.png");
noMask_xyStart_Alphas_long_low->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_long_low_logz.png");
noMask_xyStart_Alphas_long_high->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_long_high_logz.png");
noMask_xyStart_Alphas_short_higher->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_short_higher_logz.png");
noMask_xyStart_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("noMask_xyStart_Alphas_alphaCut_short_low_logz.png");
cft_xyStart->Draw("colz");
c1->SaveAs("cft_xyStart_logz.png");
cft_xyStart_Frags->Draw("colz");
c1->SaveAs("cft_xyStart_Frags_logz.png");
cft_xyStart_Alphas->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_logz.png");
cft_xyStart_Alphas_long->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_long_logz.png");
cft_xyStart_Alphas_long_low->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_long_low_logz.png");
cft_xyStart_Alphas_long_high->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_long_high_logz.png");
cft_xyStart_Alphas_short_higher->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_short_higher_logz.png");
cft_xyStart_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("cft_xyStart_Alphas_alphaCut_short_low_logz.png");
////////////////////// Draw xyEnd's ///////////////////////////////////
printf("////////////////////// Draw xyEnd's ///////////////////////////////////\n");
noMask_xyEnd->Draw("colz");
c1->SaveAs("noMask_xyEnd_logz.png");
noMask_xyEnd_Frags->Draw("colz");
c1->SaveAs("noMask_xyEnd_Frags_logz.png");
noMask_xyEnd_Alphas->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_logz.png");
noMask_xyEnd_Alphas_long->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_long_logz.png");
noMask_xyEnd_Alphas_long_low->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_long_low_logz.png");
noMask_xyEnd_Alphas_long_high->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_long_high_logz.png");
noMask_xyEnd_Alphas_short_higher->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_short_higher_logz.png");
noMask_xyEnd_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("noMask_xyEnd_Alphas_alphaCut_short_low_logz.png");
cft_xyEnd->Draw("colz");
c1->SaveAs("cft_xyEnd_logz.png");
cft_xyEnd_Frags->Draw("colz");
c1->SaveAs("cft_xyEnd_Frags_logz.png");
cft_xyEnd_Alphas->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_logz.png");
cft_xyEnd_Alphas_long->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_long_logz.png");
cft_xyEnd_Alphas_long_low->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_long_low_logz.png");
cft_xyEnd_Alphas_long_high->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_long_high_logz.png");
cft_xyEnd_Alphas_short_higher->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_short_higher_logz.png");
cft_xyEnd_Alphas_alphaCut_short_low->Draw("colz");
c1->SaveAs("cft_xyEnd_Alphas_alphaCut_short_low_logz.png");

////////////////////// write xyStart's to file ///////////////////////////////////
printf("////////////////////// write xyStart's to file ///////////////////////////////////\n");
sprintf(buffChar,"$cft/vertex/vertexHistograms.premap%d.root",preamp);
TFile* polarHistograms = new TFile(buffChar,"recreate");
noMask_xyStart->Write();
noMask_xyStart_Frags->Write();
noMask_xyStart_Alphas->Write();
noMask_xyStart_Alphas_long->Write();
noMask_xyStart_Alphas_long_low->Write();
noMask_xyStart_Alphas_long_high->Write();
noMask_xyStart_Alphas_short_higher->Write();
noMask_xyStart_Alphas_alphaCut_short_low->Write();
cft_xyStart->Write();
cft_xyStart_Frags->Write();
cft_xyStart_Alphas->Write();
cft_xyStart_Alphas_long->Write();
cft_xyStart_Alphas_long_low->Write();
cft_xyStart_Alphas_long_high->Write();
cft_xyStart_Alphas_short_higher->Write();
cft_xyStart_Alphas_alphaCut_short_low->Write();
////////////////////// write xyEnd's to file ///////////////////////////////////
printf("////////////////////// write xyEnd's to file ///////////////////////////////////\n");
noMask_xyEnd->Write();
noMask_xyEnd_Frags->Write();
noMask_xyEnd_Alphas->Write();
noMask_xyEnd_Alphas_long->Write();
noMask_xyEnd_Alphas_long_low->Write();
noMask_xyEnd_Alphas_long_high->Write();
noMask_xyEnd_Alphas_short_higher->Write();
noMask_xyEnd_Alphas_alphaCut_short_low->Write();
cft_xyEnd->Write();
cft_xyEnd_Frags->Write();
cft_xyEnd_Alphas->Write();
cft_xyEnd_Alphas_long->Write();
cft_xyEnd_Alphas_long_low->Write();
cft_xyEnd_Alphas_long_high->Write();
cft_xyEnd_Alphas_short_higher->Write();
cft_xyEnd_Alphas_alphaCut_short_low->Write();



}
