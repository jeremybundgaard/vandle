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


void cftAnaPlots(const int preamp, const char* actinide, const char* noMaskFileName, const char* cftFileName, const char* otherTreeCuts="&&1",const char* pidType="linear", int pidStep=8) {

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
char c1PlotName[234];
gStyle->SetOptStat(0);
gStyle->SetPalette(55);
gStyle->SetNumberContours(999);

double MevPerADC = 168.6/40000;
double dEnergy = 0.1;
double deg2rad = TMath::Pi()/(double)180;

char noMaskCut[234];
char cftCut[234];
char fitCut[234];
char noMaskRunCut[234];
char cftRunCut[234];
char azCut[234];
char buffChar[345];

sprintf(fitCut,"fitType==5");

char pidFile_char[345];
char fragCutADC_char[345];
char alphaCutADC_char[345];
char fragCutEnergy_char[345];
char alphaCutEnergy_char[345];

TCutG* vertexCut = new TCutG("vertexCut",4);
vertexCut->SetVarX("start.y()");
vertexCut->SetVarY("start.x()");

TCutG* grassCut_800adc4cm = new TCutG("grassCut_800adc4cm",5);
grassCut_800adc4cm->SetVarX("adc");
grassCut_800adc4cm->SetVarY("length");

TCutG* grassCut_300adc2cm = new TCutG("grassCut_300adc2cm",11);
grassCut_300adc2cm->SetVarX("adc");
grassCut_300adc2cm->SetVarY("length");

TCutG* grassCut_1200adc5cm = new TCutG("grassCut_1200adc5cm",6);
grassCut_1200adc5cm->SetVarX("adc");
grassCut_1200adc5cm->SetVarY("length");

TCutG* grassCut_2200adc7cm = new TCutG("grassCut_2200adc7cm",5);
grassCut_2200adc7cm->SetVarX("adc");
grassCut_2200adc7cm->SetVarY("length");

TCutG* grassCut_2300adc5cm = new TCutG("grassCut_2300adc5cm",5);
grassCut_2300adc5cm->SetVarX("adc");
grassCut_2300adc5cm->SetVarY("length");

TCutG* grassCut_1500adc3cm = new TCutG("grassCut_1500adc3cm",5);
grassCut_1500adc3cm->SetVarX("adc");
grassCut_1500adc3cm->SetVarY("length");

TCutG* grassCut_500adc6cm = new TCutG("grassCut_500adc6cm",7);
grassCut_500adc6cm->SetVarX("adc");
grassCut_500adc6cm->SetVarY("length");

TCutG* grassCut_2000adc4cm = new TCutG("grassCut_2000adc4cm",6);
grassCut_2000adc4cm->SetVarX("adc");
grassCut_2000adc4cm->SetVarY("length");

if (preamp==1)
{

	vertexCut->SetPoint(0,-1.33527,-0.127485);
	vertexCut->SetPoint(1,-0.44236,1.21056);
	vertexCut->SetPoint(2,0.978926,0.47083);
	vertexCut->SetPoint(3,0.925679,-0.323297);
	vertexCut->SetPoint(4,0.294907,-1.07391);
	vertexCut->SetPoint(5,-0.323578,-1.14462);
	vertexCut->SetPoint(6,-1.15915,-0.361371);
	vertexCut->SetPoint(7,-1.33527,-0.127485);

	grassCut_800adc4cm->SetPoint(0,827.76,3.10022);
	grassCut_800adc4cm->SetPoint(1,1152.01,4.76138);
	grassCut_800adc4cm->SetPoint(2,846.114,4.77974);
	grassCut_800adc4cm->SetPoint(3,635.044,3.48568);
	grassCut_800adc4cm->SetPoint(4,827.76,3.10022);

	grassCut_300adc2cm->SetPoint(0,679.769,2.4577);
	grassCut_300adc2cm->SetPoint(1,487.114,1.71976);
	grassCut_300adc2cm->SetPoint(2,351.241,1.33279);
	grassCut_300adc2cm->SetPoint(3,272.151,1.12581);
	grassCut_300adc2cm->SetPoint(4,113.971,0.945824);
	grassCut_300adc2cm->SetPoint(5,57.1882,0.927826);
	grassCut_300adc2cm->SetPoint(6,166.698,1.2698);
	grassCut_300adc2cm->SetPoint(7,337.046,2.25972);
	grassCut_300adc2cm->SetPoint(8,387.744,2.4937);
	grassCut_300adc2cm->SetPoint(9,592.567,2.4847);
	grassCut_300adc2cm->SetPoint(10,679.769,2.4577);

	grassCut_1200adc5cm->SetPoint(0,953.543,4.86051);
	grassCut_1200adc5cm->SetPoint(1,1204.09,5.97642);
	grassCut_1200adc5cm->SetPoint(2,1647.1,5.80544);
	grassCut_1200adc5cm->SetPoint(3,1748.77,4.63553);
	grassCut_1200adc5cm->SetPoint(4,1636.2,4.67153);
	grassCut_1200adc5cm->SetPoint(5,953.543,4.86051);

	grassCut_2200adc7cm->SetPoint(0,1447.38,5.96742);
	grassCut_2200adc7cm->SetPoint(1,2754.6,5.59845);
	grassCut_2200adc7cm->SetPoint(2,2605.72,8.46022);
	grassCut_2200adc7cm->SetPoint(3,1661.62,8.51422);
	grassCut_2200adc7cm->SetPoint(4,1447.38,5.96742);

	grassCut_2300adc5cm->SetPoint(0,1672.51,5.80544);
	grassCut_2300adc5cm->SetPoint(1,2790.91,5.50846);
	grassCut_2300adc5cm->SetPoint(2,2780.02,4.34755);
	grassCut_2300adc5cm->SetPoint(3,1799.6,4.66253);
	grassCut_2300adc5cm->SetPoint(4,1672.51,5.80544);

	grassCut_2000adc4cm->SetPoint(0,1436.29,4.59053);
	grassCut_2000adc4cm->SetPoint(1,1400.42,3.62761);
	grassCut_2000adc4cm->SetPoint(2,2701.36,3.36663);
	grassCut_2000adc4cm->SetPoint(3,2742.02,4.32055);
	grassCut_2000adc4cm->SetPoint(4,1783.05,4.57253);
	grassCut_2000adc4cm->SetPoint(5,1436.29,4.59053);

	grassCut_1500adc3cm->SetPoint(0,955.616,1.2698);
	grassCut_1500adc3cm->SetPoint(1,1386.07,3.44762);
	grassCut_1500adc3cm->SetPoint(2,2282.86,3.24064);
	grassCut_1500adc3cm->SetPoint(3,1321.51,1.32379);
	grassCut_1500adc3cm->SetPoint(4,955.616,1.2698);

	grassCut_500adc6cm->SetPoint(0,551.464,8.55022);
	grassCut_500adc6cm->SetPoint(1,1455.43,8.10925);
	grassCut_500adc6cm->SetPoint(2,604.076,3.57361);
	grassCut_500adc6cm->SetPoint(3,68.3949,2.35871);
	grassCut_500adc6cm->SetPoint(4,125.789,5.42747);
	grassCut_500adc6cm->SetPoint(5,341.018,8.62221);
	grassCut_500adc6cm->SetPoint(6,551.464,8.55022);


	if (!(strcmp(pidType,"expo"))){
		printf("test\n");
		sprintf(pidFile_char,"$cft/pid/pidCut_alphaExponential1.root");
		sprintf(fragCutADC_char,"fragCut_adc_preamp1_%dstdDev",pidStep);
		sprintf(alphaCutADC_char,"alphaCut_adc_preamp1_%dstdDev",pidStep);
		sprintf(fragCutEnergy_char,"fragCut_energy_preamp1_%dstdDev",pidStep);
		sprintf(alphaCutEnergy_char,"alphaCut_energy_preamp1_%dstdDev",pidStep);
	}

	if (!(strcmp(pidType,"linear"))){
		printf("test\n");
		sprintf(pidFile_char,"$cft/pid/pidCut_alphaLinear1.root");
		sprintf(fragCutADC_char,"fragCut_adc_preamp1_adcShift%d",pidStep);
		sprintf(alphaCutADC_char,"alphaCut_adc_preamp1_adcShift%d",pidStep);
		sprintf(fragCutEnergy_char,"fragCut_energy_preamp1_energyShift%d",pidStep);
		sprintf(alphaCutEnergy_char,"alphaCut_energy_preamp1_energyShift%d",pidStep);
	}

	if (!(strcmp(actinide,"cf252"))){}
	if (!(strcmp(actinide,"cf252_driftVel4.42"))){}
	if (!(strcmp(actinide,"cf252_driftVel4.9"))){}
	if (!(strcmp(actinide,"cm244"))){}
	sprintf(noMaskRunCut,"run>100001856&&run!=100001870");
	sprintf(noMaskCut," %s && %s %s",fitCut,noMaskRunCut,otherTreeCuts);
	sprintf(cftCut,"%s %s",fitCut,otherTreeCuts);

}
else if (preamp==2)
{

	vertexCut->SetPoint(0,-0.430136,0.866337);
	vertexCut->SetPoint(1,-0.646507,-0.485149);
	vertexCut->SetPoint(2,0.80292,-0.693069);
	vertexCut->SetPoint(3,0.915016,0.767327);
	vertexCut->SetPoint(4,-0.35975,0.861386);
	vertexCut->SetPoint(5,-0.430136,0.866337);

	if (!(strcmp(pidType,"expo"))){
		printf("test\n");
		sprintf(pidFile_char,"$cft/pid/pidCut_alphaExponential2.root");
		sprintf(fragCutADC_char,"fragCut_adc_preamp2_%dstdDev",pidStep);
		sprintf(alphaCutADC_char,"alphaCut_adc_preamp2_%dstdDev",pidStep);
		sprintf(fragCutEnergy_char,"fragCut_energy_preamp2_%dstdDev",pidStep);
		sprintf(alphaCutEnergy_char,"alphaCut_energy_preamp2_%dstdDev",pidStep);
	}
	if (!(strcmp(pidType,"linear"))){
		printf("test\n");
		sprintf(pidFile_char,"$cft/pid/pidCut_alphaLinear2.root");
		sprintf(fragCutADC_char,"fragCut_adc_preamp2_adcShift%d",pidStep);
		sprintf(alphaCutADC_char,"alphaCut_adc_preamp2_adcShift%d",pidStep);
		sprintf(fragCutEnergy_char,"fragCut_energy_preamp2_energyShift%d",pidStep);
		sprintf(alphaCutEnergy_char,"alphaCut_energy_preamp2_energyShift%d",pidStep);
	}

	if (!(strcmp(actinide,"cf252"))){
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
	}
	if (!(strcmp(actinide,"cf252_driftVel4.42"))){
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
	}
	if (!(strcmp(actinide,"cf252_driftVel4.9"))){
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
	}
	if ( !(strcmp(actinide,"cm244"))){
		sprintf(azCut,"direction.Phi()<2.2");
	}
	sprintf(noMaskCut,"%s&&%s%s",fitCut,azCut,otherTreeCuts);
	sprintf(cftCut,"%s&&%s%s",fitCut,azCut,otherTreeCuts);
}


TFile* pidFile = new TFile(pidFile_char);

// fragCutADC is crucial for invoking this cut in ->Draw()
TCutG* fragCutADC = (TCutG*)pidFile->Get(fragCutADC_char);
fragCutADC->Draw();
fragCutADC->SetName("fragCutADC");
fragCutADC->SetLineColor(kBlue);
fragCutADC->SetLineWidth(2);
fragCutADC->SetFillColorAlpha(kViolet,0.2);

TCutG* alphaCutADC = (TCutG*)pidFile->Get(alphaCutADC_char);
alphaCutADC->SetName("alphaCutADC");
alphaCutADC->SetLineColor(kRed);
alphaCutADC->SetLineWidth(2);
alphaCutADC->SetFillColorAlpha(kGreen,0.2);

// fragCutEnergy is crucial for invoking this cut in ->Draw()
TCutG* fragCutEnergy = (TCutG*)pidFile->Get(fragCutEnergy_char);
fragCutEnergy->SetName("fragCutEnergy");
fragCutEnergy->SetLineColor(kViolet);
fragCutEnergy->SetLineWidth(2);
fragCutEnergy->SetFillColorAlpha(kViolet,0.2);
TCutG* alphaCutEnergy = (TCutG*)pidFile->Get(alphaCutEnergy_char);
alphaCutEnergy->SetName("alphaCutEnergy");
alphaCutEnergy->SetLineColor(kGreen);
alphaCutEnergy->SetLineWidth(2);
alphaCutEnergy->SetFillColorAlpha(kGreen,0.2);



char cftFragCut[234];sprintf(cftFragCut,"%s && %s", cftCut,"fragCutADC");
char cftAlphaCut[234];sprintf(cftAlphaCut,"%s && %s", cftCut,"alphaCutADC");
char noMaskFragCut[234];sprintf(noMaskFragCut,"%s && %s", noMaskCut,"fragCutADC");
char noMaskAlphaCut[234];sprintf(noMaskAlphaCut,"%s && %s", noMaskCut,"alphaCutADC");

std::cout <<"noMaskCut = " << noMaskCut << std::endl;
std::cout <<"cftCut = " << cftCut << std::endl;
std::cout <<"otherTreeCuts = " << otherTreeCuts << std::endl;

TFile* cftFile = new TFile(cftFileName);
TTree* cftTracks = (TTree*)cftFile->Get("tracks");
TFile* noMaskFile = new TFile(noMaskFileName);
TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");

TH2D* noMaskLvADC = new TH2D("noMaskLvADC","noMaskLvADC",40000,0,40000,1000,0,10);
TH2D* noMaskLvADCFrags = new TH2D("noMaskLvADCFrags","noMaskLvADCFrags",40000,0,40000,1000,0,10);
TH2D* noMaskLvADCAlphas = new TH2D("noMaskLvADCAlphas","noMaskLvADCAlphas",40000,0,40000,1000,0,10);
TH2D* cftLvADC = new TH2D("cftLvADC","cftLvADC",40000,0,40000,1000,0,10);
TH2D* cftLvADCFrags = new TH2D("cftLvADCFrags","cftLvADCFrags",40000,0,40000,1000,0,10);
TH2D* cftLvADCAlphas = new TH2D("cftLvADCAlphas","cftLvADCAlphas",40000,0,40000,1000,0,10);

cftTracks->Draw("length:adc>>cftLvADCFrags",cftFragCut);
cftTracks->Draw("length:adc>>cftLvADCAlphas",cftAlphaCut);
cftTracks->Draw("length:adc>>cftLvADC",cftCut);
TH1D* cftFragsADC = cftLvADCFrags->ProjectionX("cftFragsADC",0,40000);
TH1D* cftAlphasADC = cftLvADCAlphas->ProjectionX("cftAlphasADC",0,40000);
TH1D* cftADC = cftLvADC->ProjectionX("cftADC",0,40000);

noMaskTracks->Draw("length:adc>>noMaskLvADCFrags",noMaskFragCut);
noMaskTracks->Draw("length:adc>>noMaskLvADCAlphas",noMaskAlphaCut);
noMaskTracks->Draw("length:adc>>noMaskLvADC",noMaskCut);
TH1D* noMaskFragsADC = noMaskLvADCFrags->ProjectionX("noMaskFragsADC",0,40000);
TH1D* noMaskAlphasADC = noMaskLvADCAlphas->ProjectionX("noMaskAlphasADC",0,40000);
TH1D* noMaskADC = noMaskLvADC->ProjectionX("noMaskADC",0,40000);

/* make energy histograms */
TH2D* noMaskLvEn = new TH2D("noMaskLvEn","noMaskLvEn",40000,0,168.6,1000,0,10);
TH2D* noMaskLvEnFrags = new TH2D("noMaskLvEnFrags","noMaskLvEnFrags",40000,0,168.6,1000,0,10);
TH2D* noMaskLvEnAlphas = new TH2D("noMaskLvEnAlphas","noMaskLvEnAlphas",40000,0,168.6,1000,0,10);
TH1D* noMaskFrags = new TH1D("noMaskFrags","noMaskFrags",40000,0,168.6);
TH1D* noMaskAlphas = new TH1D("noMaskAlphas","noMaskAlphas",40000,0,168.6);

TH2D* cftLvEn = new TH2D("cftLvEn","cftLvEn",40000,0,168.6,1000,0,10);
TH2D* cftLvEnFrags = new TH2D("cftLvEnFrags","cftLvEnFrags",40000,0,168.6,1000,0,10);
TH2D* cftLvEnAlphas = new TH2D("cftLvEnAlphas","cftLvEnAlphas",40000,0,168.6,1000,0,10);
TH1D* cftFrags = new TH1D("cftFrags","cftFrags",40000,0,168.6);
TH1D* cftAlphas = new TH1D("cftAlphas","cftAlphas",40000,0,168.6);

for (int adcBin = 0; adcBin < 40000; ++adcBin)
{
	noMaskFrags->SetBinContent(adcBin,noMaskFragsADC->GetBinContent(adcBin));
	noMaskAlphas->SetBinContent(adcBin,noMaskAlphasADC->GetBinContent(adcBin));
	cftFrags->SetBinContent(adcBin,cftFragsADC->GetBinContent(adcBin));
	cftAlphas->SetBinContent(adcBin,cftAlphasADC->GetBinContent(adcBin));
	for (int lengthBin = 0; lengthBin < 1000; ++lengthBin)
	{
		noMaskLvEn->SetBinContent(adcBin,lengthBin,noMaskLvADC->GetBinContent(adcBin,lengthBin));
		noMaskLvEnFrags->SetBinContent(adcBin,lengthBin,noMaskLvADCFrags->GetBinContent(adcBin,lengthBin));
		noMaskLvEnAlphas->SetBinContent(adcBin,lengthBin,noMaskLvADCAlphas->GetBinContent(adcBin,lengthBin));
		cftLvEn->SetBinContent(adcBin,lengthBin,cftLvADC->GetBinContent(adcBin,lengthBin));
		cftLvEnFrags->SetBinContent(adcBin,lengthBin,cftLvADCFrags->GetBinContent(adcBin,lengthBin));
		cftLvEnAlphas->SetBinContent(adcBin,lengthBin,cftLvADCAlphas->GetBinContent(adcBin,lengthBin));
	}
}

cftLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
cftLvEn->GetXaxis()->CenterTitle();
cftLvEn->GetYaxis()->SetTitle("Track Length [cm]");
cftLvEn->GetYaxis()->CenterTitle();
cftLvEn->SetTitle("Cf-252 [Fission Trigger]");

cftLvEnFrags->GetXaxis()->SetTitle("Track Energy [MeV]");
cftLvEnFrags->GetXaxis()->CenterTitle();
cftLvEnFrags->GetYaxis()->SetTitle("Track Length [cm]");
cftLvEnFrags->GetYaxis()->CenterTitle();
cftLvEnFrags->SetTitle("fragments [Fission Trigger]");

cftLvEnAlphas->GetXaxis()->SetTitle("Track Energy [MeV]");
cftLvEnAlphas->GetXaxis()->CenterTitle();
cftLvEnAlphas->GetYaxis()->SetTitle("Track Length [cm]");
cftLvEnAlphas->GetYaxis()->CenterTitle();
cftLvEnAlphas->SetTitle("alphas [Fission Trigger]");

cftFrags->GetXaxis()->SetTitle("MeV");
cftFrags->GetXaxis()->CenterTitle();
cftFrags->GetYaxis()->SetTitle("counts");
cftFrags->GetYaxis()->CenterTitle();
cftFrags->SetTitle("cftFrags");
cftFrags->SetLineColor(kBlue);
cftFrags->SetLineWidth(1);

cftAlphas->GetXaxis()->SetTitle("MeV");
cftAlphas->GetXaxis()->CenterTitle();
cftAlphas->GetYaxis()->SetTitle("counts");
cftAlphas->GetYaxis()->CenterTitle();
cftAlphas->SetTitle("cftAlphas");
cftAlphas->SetLineColor(kGreen);
cftAlphas->SetLineWidth(1);

noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn->GetXaxis()->CenterTitle();
noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn->GetYaxis()->CenterTitle();
noMaskLvEn->SetTitle("Cf-252 [no trigger]");

noMaskLvEnFrags->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEnFrags->GetXaxis()->CenterTitle();
noMaskLvEnFrags->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEnFrags->GetYaxis()->CenterTitle();
noMaskLvEnFrags->SetTitle("fragments [no trigger]");

noMaskLvEnAlphas->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEnAlphas->GetXaxis()->CenterTitle();
noMaskLvEnAlphas->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEnAlphas->GetYaxis()->CenterTitle();
noMaskLvEnAlphas->SetTitle("alphas [no trigger]");

noMaskFrags->GetXaxis()->SetTitle("MeV");
noMaskFrags->GetXaxis()->CenterTitle();
noMaskFrags->GetYaxis()->SetTitle("counts");
noMaskFrags->GetYaxis()->CenterTitle();
noMaskFrags->SetTitle("noMaskFrags");
noMaskFrags->SetLineColor(kRed);
noMaskFrags->SetLineWidth(1);

noMaskAlphas->GetXaxis()->SetTitle("MeV");
noMaskAlphas->GetXaxis()->CenterTitle();
noMaskAlphas->GetYaxis()->SetTitle("counts");
noMaskAlphas->GetYaxis()->CenterTitle();
noMaskAlphas->SetTitle("noMaskAlphas");
noMaskAlphas->SetLineColor(kGreen);
noMaskAlphas->SetLineWidth(1);

double scaleCFT = noMaskFrags->Integral(5000,40000)/cftFrags->Integral(5000,40000);
TH1D* scaledCftFrags = (TH1D*)cftFrags->Clone("scaledCftFrags");
scaledCftFrags->Scale(scaleCFT);
cftAlphas->Scale(scaleCFT);

// //////////////////////////////////////////
// //////////////  rebinning ////////////////
// //////////////////////////////////////////
int rebinInt=10;
cftFrags = (TH1D*)scaledCftFrags->Clone("cftFrags");
cftFrags->Rebin(rebinInt);
noMaskFrags->Rebin(rebinInt);

TH1D* cftEfficiency = (TH1D*)cftFrags->Clone("cftEfficiency");
cftEfficiency->SetTitle("cftEfficiency = cft / noMask");
cftEfficiency->Divide(noMaskFrags);

// // ////////////////////////////////////////////
// // //////////////  count frags ////////////////
// // ////////////////////////////////////////////
double isoTopicCalib=0.9186;
int cftIntegral=scaledCftFrags->Integral();
int cftFragIntegral=cftFrags->Integral();
int cftAlphaIntegral=cftLvEnAlphas->Integral();

int noMaskIntegral=noMaskLvEn->Integral();
int noMaskFragIntegral=noMaskFrags->Integral();
int noMaskAlphaIntegral=noMaskLvEnAlphas->Integral();

double noMask_aSF_ratio=2*noMaskAlphaIntegral/(double)noMaskFragIntegral*isoTopicCalib;
double cft_aSF_ratio=2*cftAlphaIntegral/(double)cftFragIntegral*isoTopicCalib;

char noMaskFragChar[234],cftFragChar[234],
	noMaskAlphaChar[234],cftAlphaChar[234],
	noMask_aSF_ratioChar[234],cft_aSF_ratioChar[234],
	nFragRejectedChar[234],nAlphasRejectedChar[234];

sprintf(nFragRejectedChar,"nFragRejected = %.2f%%",100*TMath::Abs(1-noMaskFragIntegral/(double)cftFragIntegral));
sprintf(nAlphasRejectedChar,"nAlphasRejected = %.2f%%",100*TMath::Abs(1-cftAlphaIntegral/(double)noMaskAlphaIntegral));
sprintf(cft_aSF_ratioChar,"cft_aSF_ratio = %.3f",cft_aSF_ratio);
sprintf(noMask_aSF_ratioChar,"noMask_aSF_ratio = %.3f",noMask_aSF_ratio);
sprintf(cftAlphaChar,"cftAlphaIntegral = %d",cftAlphaIntegral);
sprintf(noMaskAlphaChar,"noMaskAlphaIntegral = %d",noMaskAlphaIntegral);
sprintf(cftFragChar,"nFragsCFT = %d",cftFragIntegral);
sprintf(noMaskFragChar,"nFragsNoMask = %d",noMaskFragIntegral);

TText fragCountText;
fragCountText.SetTextAlign(13);
fragCountText.SetTextSize(0.025);
fragCountText.SetTextFont(42);

//////////////////////////////////////////////////////// /////////////////
//////////////////// fragment rejection & cft efficiency /////////////////
//////////////////////////////////////////////////////// /////////////////
TLegend *legend1=new TLegend(0.4,0.8,0.6,0.89);
legend1->SetTextFont(42);
legend1->SetTextSize(0.03);
// legend1->SetFillColorAlpha(kWhite,0.2);
noMaskFrags->SetLineColor(kRed);
legend1->AddEntry(noMaskFrags,"No Trigger");
cftFrags->SetLineColor(kBlue);
legend1->AddEntry(cftFrags,"Fission Trigger");

// for plotting the scaled region
TH1D* cftFragsScaleRegion = (TH1D*)scaledCftFrags->Clone("cftFragsScaleRegion"); 
for (int i = 0; i < 5000; ++i) cftFragsScaleRegion->SetBinContent(i,0);
cftFragsScaleRegion->Rebin(rebinInt);
cftFragsScaleRegion->SetFillColorAlpha(kBlue,0.2);
cftFragsScaleRegion->SetLineColor(kBlue);

c1->SetLogx(1);
c1->SetLogy(1);
noMaskFrags->GetXaxis()->SetRangeUser(0.1,200);
noMaskFrags->GetYaxis()->SetRangeUser(0.5,10000);
cftFrags->GetXaxis()->SetRangeUser(0.1,200);
cftFrags->GetYaxis()->SetRangeUser(0.5,10000);
noMaskFrags->Draw();
noMaskFrags->SetTitle("fragCut [Trigger vs No Trigger]");
cftFrags->Draw("same");
legend1->Draw("same");
fragCountText.DrawTextNDC(0.1,0.83,nFragRejectedChar);
fragCountText.DrawTextNDC(0.1,0.86,noMaskFragChar);
fragCountText.DrawTextNDC(0.1,0.89,cftFragChar);
cftFragsScaleRegion->Draw("same");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.bothFragsCut.logxy.showScalingRegion.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

TLegend *legendCFTefficiency=new TLegend(0.65,0.15,0.85,0.3);
legendCFTefficiency->SetTextFont(42);
legendCFTefficiency->SetTextSize(0.03);
legendCFTefficiency->SetFillColorAlpha(kWhite,0.2);
legendCFTefficiency->AddEntry(noMaskFrags,"No Trigger");
legendCFTefficiency->AddEntry(cftFrags,"Fission Trigger");
legendCFTefficiency->AddEntry(cftEfficiency,"CFT / no CFT");

c1->SetLogy(1);
noMaskFrags->GetYaxis()->SetRangeUser(0.001,8000);
cftFrags->GetYaxis()->SetRangeUser(0.001,8000);
cftEfficiency->GetYaxis()->SetRangeUser(0.001,8000);
noMaskFrags->Draw();
cftFrags->Draw("same");
cftEfficiency->Draw("same");
legendCFTefficiency->Draw("same");
fragCountText.DrawTextNDC(0.1,0.83,nFragRejectedChar);
fragCountText.DrawTextNDC(0.1,0.86,noMaskFragChar);
fragCountText.DrawTextNDC(0.1,0.89,cftFragChar);
cftFragsScaleRegion->Draw("same");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.bothFragsCut.zoom.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

//////////////////////////////////////////////////////////////////
//////////////////// cft vs noMask stacks comparison /////////////
//////////////////////////////////////////////////////////////////
THStack* noMaskEnergyStack = new THStack("noMaskEnergyStack","");
noMaskFrags->SetFillColorAlpha(kViolet,0.2);
noMaskAlphas->SetFillColorAlpha(kGreen,0.2);
noMaskEnergyStack->Add(noMaskFrags);
noMaskEnergyStack->Add(noMaskAlphas);
noMaskEnergyStack->SetMinimum(0.5);
noMaskEnergyStack->Draw();

THStack* cftEnergyStack = new THStack("cftEnergyStack","");
cftFrags->SetFillColorAlpha(kViolet,0.2);
cftAlphas->SetFillColorAlpha(kGreen,0.2);
cftEnergyStack->Add(cftFrags);
cftEnergyStack->Add(cftAlphas);
cftEnergyStack->SetMinimum(0.5);
cftEnergyStack->Draw();

c1->SetLogx(1);
c1->SetLogy(1);
noMaskEnergyStack->Draw();
noMaskEnergyStack->GetXaxis()->SetRangeUser(0.1,200);
fragCountText.DrawTextNDC(0.65,0.73,nFragRejectedChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftFragChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskEnergy.full.log.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

cftEnergyStack->Draw();
cftEnergyStack->GetXaxis()->SetRangeUser(0.1,200);
fragCountText.DrawTextNDC(0.65,0.73,nFragRejectedChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftFragChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftEnergy.full.log.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");


c1->SetLogx(0);
cftEnergyStack->Draw();
fragCountText.DrawTextNDC(0.65,0.73,nFragRejectedChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftFragChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftEnergy.full.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

noMaskEnergyStack->Draw();
fragCountText.DrawTextNDC(0.65,0.73,nFragRejectedChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftFragChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskEnergy.full.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

cftEnergyStack->Draw();
cftEnergyStack->GetXaxis()->SetRangeUser(0,14);
fragCountText.DrawTextNDC(0.65,0.73,nFragRejectedChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftFragChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftEnergy.lowEn.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

noMaskEnergyStack->Draw();
noMaskEnergyStack->GetXaxis()->SetRangeUser(0,14);
fragCountText.DrawTextNDC(0.65,0.73,nFragRejectedChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftFragChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskEnergy.lowEn.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

///////////////////////////////////////////////////////////////////
///////////////////////  Plot lenght VS Energy ////////////////////
///////////////////////////////////////////////////////////////////
c1->cd();
c1->SetGridy(1);
c1->SetGridx(1);
c1->SetLogy(0);
c1->SetLogz(1);
cftLvEn->GetZaxis()->SetRangeUser(1,1000);
noMaskLvEn->GetZaxis()->SetRangeUser(1,1000);


c1->SetLogx(1);
cftLvEn->GetXaxis()->SetRangeUser(0.1,200);
cftLvEn->GetYaxis()->SetRangeUser(0,10);
cftLvEn->Draw("Colz");
fragCutEnergy->Draw("same f");
alphaCutEnergy->Draw("same f");
fragCountText.DrawTextNDC(0.65,0.73,cft_aSF_ratioChar);
fragCountText.DrawTextNDC(0.65,0.76,cftFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftAlphaChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftLvEn.full.log.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

noMaskLvEn->GetXaxis()->SetRangeUser(0.1,200);
noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
noMaskLvEn->Draw("colz");
fragCutEnergy->Draw("same f");
alphaCutEnergy->Draw("same f");
fragCountText.DrawTextNDC(0.65,0.73,noMask_aSF_ratioChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,noMaskAlphaChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskLvEn.full.log.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

c1->SetLogx(0);

cftLvEn->GetXaxis()->SetRangeUser(0.1,200);
cftLvEn->GetYaxis()->SetRangeUser(0,10);
cftLvEn->Draw("Colz");
fragCutEnergy->Draw("same f");
alphaCutEnergy->Draw("same f");
fragCountText.DrawTextNDC(0.65,0.73,cft_aSF_ratioChar);
fragCountText.DrawTextNDC(0.65,0.76,cftFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftAlphaChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftLvEn.full.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

noMaskLvEn->GetXaxis()->SetRangeUser(0.1,200);
noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
noMaskLvEn->Draw("colz");
fragCutEnergy->Draw("same f");
alphaCutEnergy->Draw("same f");
fragCountText.DrawTextNDC(0.65,0.73,noMask_aSF_ratioChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,noMaskAlphaChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskLvEn.full.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

cftLvEn->GetXaxis()->SetRangeUser(0,14);
cftLvEn->GetYaxis()->SetRangeUser(0,9);
cftLvEn->Draw("Colz");
fragCutEnergy->Draw("same f");
alphaCutEnergy->Draw("same f");
fragCountText.DrawTextNDC(0.65,0.73,cft_aSF_ratioChar);
fragCountText.DrawTextNDC(0.65,0.76,cftFragChar);
fragCountText.DrawTextNDC(0.65,0.79,cftAlphaChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftLvEn.lowEn.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

noMaskLvEn->GetXaxis()->SetRangeUser(0,14);
noMaskLvEn->GetYaxis()->SetRangeUser(0,9);
noMaskLvEn->Draw("Colz");
fragCutEnergy->Draw("same f");
alphaCutEnergy->Draw("same f");
fragCountText.DrawTextNDC(0.65,0.73,noMask_aSF_ratioChar);
fragCountText.DrawTextNDC(0.65,0.76,noMaskFragChar);
fragCountText.DrawTextNDC(0.65,0.79,noMaskAlphaChar);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskLvEn.lowEn.lin.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName,"recreate");

//////////////////////////////////////////////////////////////////
//////////////////// polar cuts & projections ////////////////////
//////////////////////////////////////////////////////////////////


TH1D* noMaskVsTheta0to180 = new TH1D("noMaskVsTheta0to180","noMaskVsTheta0to180",1000,0,180);
TH1D* noMaskVsCosThetaNeg1to1 = new TH1D("noMaskVsCosThetaNeg1to1","noMaskVsCosThetaNeg1to1",1000,-1,1);
noMaskTracks->Draw("direction.Theta()/TMath::Pi()*180>>noMaskVsTheta0to180",noMaskCut);
noMaskTracks->Draw("direction.CosTheta()>>noMaskVsCosThetaNeg1to1",noMaskCut);

TH1D* noMaskFragVsTheta0to180 = new TH1D("noMaskFragVsTheta0to180","noMaskFragVsTheta0to180",1000,0,180);
TH1D* noMaskFragVsCosThetaNeg1to1 = new TH1D("noMaskFragVsCosThetaNeg1to1","noMaskFragVsCosThetaNeg1to1",1000,-1,1);
noMaskTracks->Draw("direction.Theta()/TMath::Pi()*180>>noMaskFragVsTheta0to180",noMaskFragCut);
noMaskTracks->Draw("direction.CosTheta()>>noMaskFragVsCosThetaNeg1to1",noMaskFragCut);

TH1D* noMaskAlphaVsTheta0to180 = new TH1D("noMaskAlphaVsTheta0to180","noMaskAlphaVsTheta0to180",1000,0,180);
TH1D* noMaskAlphaVsCosThetaNeg1to1 = new TH1D("noMaskAlphaVsCosThetaNeg1to1","noMaskAlphaVsCosThetaNeg1to1",1000,-1,1);
noMaskTracks->Draw("direction.Theta()/TMath::Pi()*180>>noMaskAlphaVsTheta0to180",noMaskAlphaCut);
noMaskTracks->Draw("direction.CosTheta()>>noMaskAlphaVsCosThetaNeg1to1",noMaskAlphaCut);

TH1D* cftVsTheta0to180 = new TH1D("cftVsTheta0to180","cftVsTheta0to180",1000,0,180);
TH1D* cftVsCosThetaNeg1to1 = new TH1D("cftVsCosThetaNeg1to1","cftVsCosThetaNeg1to1",1000,-1,1);
cftTracks->Draw("direction.Theta()/TMath::Pi()*180>>cftVsTheta0to180",cftCut);
cftTracks->Draw("direction.CosTheta()>>cftVsCosThetaNeg1to1",cftCut);

TH1D* cftFragVsTheta0to180 = new TH1D("cftFragVsTheta0to180","cftFragVsTheta0to180",1000,0,180);
TH1D* cftFragVsCosThetaNeg1to1 = new TH1D("cftFragVsCosThetaNeg1to1","cftFragVsCosThetaNeg1to1",1000,-1,1);
cftTracks->Draw("direction.Theta()/TMath::Pi()*180>>cftFragVsTheta0to180",cftFragCut);
cftTracks->Draw("direction.CosTheta()>>cftFragVsCosThetaNeg1to1",cftFragCut);

TH1D* cftAlphaVsTheta0to180 = new TH1D("cftAlphaVsTheta0to180","cftAlphaVsTheta0to180",1000,0,180);
TH1D* cftAlphaVsCosThetaNeg1to1 = new TH1D("cftAlphaVsCosThetaNeg1to1","cftAlphaVsCosThetaNeg1to1",1000,-1,1);
cftTracks->Draw("direction.Theta()/TMath::Pi()*180>>cftAlphaVsTheta0to180",cftAlphaCut);
cftTracks->Draw("direction.CosTheta()>>cftAlphaVsCosThetaNeg1to1",cftAlphaCut);


c1->SetLogy(0);
c1->SetLogx(0);

noMaskFragVsTheta0to180->SetTitle("noMaskFragVsTheta0to180");
noMaskFragVsTheta0to180->SetLineColor(kViolet);
noMaskFragVsTheta0to180->SetFillColorAlpha(kViolet,0.2);
noMaskFragVsTheta0to180->GetXaxis()->SetTitle("#theta");
noMaskFragVsTheta0to180->GetYaxis()->SetTitle("allTrks");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskFragVsTheta0to180.png",actinide,preamp,pidStep);
noMaskFragVsTheta0to180->Draw();
c1->SaveAs(c1PlotName);
noMaskFragVsCosThetaNeg1to1->SetTitle("noMaskFragVsCosThetaNeg1to1");
noMaskFragVsCosThetaNeg1to1->SetLineColor(kViolet);
noMaskFragVsCosThetaNeg1to1->SetFillColorAlpha(kViolet,0.2);
noMaskFragVsCosThetaNeg1to1->GetXaxis()->SetTitle("Cos #theta");
noMaskFragVsCosThetaNeg1to1->GetYaxis()->SetTitle("allTrks");
noMaskFragVsCosThetaNeg1to1->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskFragVsCosThetaNeg1to1.png",actinide,preamp,pidStep);
noMaskFragVsCosThetaNeg1to1->Draw();
c1->SaveAs(c1PlotName);

noMaskAlphaVsTheta0to180->SetTitle("noMaskAlphaVsTheta0to180");
noMaskAlphaVsTheta0to180->SetFillColorAlpha(kGreen,0.2);
noMaskAlphaVsTheta0to180->SetLineColor(kGreen);
noMaskAlphaVsTheta0to180->GetXaxis()->SetTitle("#theta");
noMaskAlphaVsTheta0to180->GetYaxis()->SetTitle("notFragTrks");
noMaskAlphaVsTheta0to180->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskAlphaVsTheta0to180.png",actinide,preamp,pidStep);
noMaskAlphaVsTheta0to180->Draw();
c1->SaveAs(c1PlotName);
noMaskAlphaVsCosThetaNeg1to1->SetTitle("noMaskAlphaVsCosThetaNeg1to1");
noMaskAlphaVsCosThetaNeg1to1->SetFillColorAlpha(kGreen,0.2);
noMaskAlphaVsCosThetaNeg1to1->SetLineColor(kGreen);
noMaskAlphaVsCosThetaNeg1to1->GetXaxis()->SetTitle("Cos #theta");
noMaskAlphaVsCosThetaNeg1to1->GetYaxis()->SetTitle("notFragTrks");
noMaskAlphaVsCosThetaNeg1to1->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskAlphaVsCosThetaNeg1to1.png",actinide,preamp,pidStep);
noMaskAlphaVsCosThetaNeg1to1->Draw();
c1->SaveAs(c1PlotName);

cftFragVsTheta0to180->SetTitle("cftFragVsTheta0to180");
cftFragVsTheta0to180->SetFillColorAlpha(kViolet,0.2);
cftFragVsTheta0to180->SetLineColor(kViolet);
cftFragVsTheta0to180->GetXaxis()->SetTitle("#theta");
cftFragVsTheta0to180->GetYaxis()->SetTitle("allTrks");
cftFragVsTheta0to180->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftFragVsTheta0to180.png",actinide,preamp,pidStep);
cftFragVsTheta0to180->Draw();
c1->SaveAs(c1PlotName);
cftFragVsCosThetaNeg1to1->SetTitle("cftFragVsCosThetaNeg1to1");
cftFragVsCosThetaNeg1to1->SetFillColorAlpha(kViolet,0.2);
cftFragVsCosThetaNeg1to1->SetLineColor(kViolet);
cftFragVsCosThetaNeg1to1->GetXaxis()->SetTitle("Cos #theta");
cftFragVsCosThetaNeg1to1->GetYaxis()->SetTitle("allTrks");
cftFragVsCosThetaNeg1to1->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftFragVsCosThetaNeg1to1.png",actinide,preamp,pidStep);
cftFragVsCosThetaNeg1to1->Draw();
c1->SaveAs(c1PlotName);

cftAlphaVsTheta0to180->SetTitle("cftAlphaVsTheta0to180");
cftAlphaVsTheta0to180->SetFillColorAlpha(kGreen,0.2);
cftAlphaVsTheta0to180->SetLineColor(kGreen);
cftAlphaVsTheta0to180->GetXaxis()->SetTitle("#theta");
cftAlphaVsTheta0to180->GetYaxis()->SetTitle("notFragTrks");
cftAlphaVsTheta0to180->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftAlphaVsTheta0to180.png",actinide,preamp,pidStep);
cftAlphaVsTheta0to180->Draw();
c1->SaveAs(c1PlotName);
cftAlphaVsCosThetaNeg1to1->SetTitle("cftAlphaVsCosThetaNeg1to1");
cftAlphaVsCosThetaNeg1to1->SetFillColorAlpha(kGreen,0.2);
cftAlphaVsCosThetaNeg1to1->SetLineColor(kGreen);
cftAlphaVsCosThetaNeg1to1->GetXaxis()->SetTitle("Cos #theta");
cftAlphaVsCosThetaNeg1to1->GetYaxis()->SetTitle("notFragTrks");
cftAlphaVsCosThetaNeg1to1->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftAlphaVsCosThetaNeg1to1.png",actinide,preamp,pidStep);
cftAlphaVsCosThetaNeg1to1->Draw();
c1->SaveAs(c1PlotName);

////////////////////////////////////////////////////////////////////////
//////////////////// polar stack cuts & projections ////////////////////
////////////////////////////////////////////////////////////////////////

THStack *noMaskVsTheta0to180Stack = new THStack("noMaskVsTheta0to180Stack","");
THStack *noMaskVsCosThetaNeg1to1Stack = new THStack("noMaskVsCosThetaNeg1to1Stack","");

noMaskVsTheta0to180Stack->Add(noMaskFragVsTheta0to180);
noMaskVsTheta0to180Stack->Add(noMaskAlphaVsTheta0to180);
noMaskVsCosThetaNeg1to1Stack->Add(noMaskFragVsCosThetaNeg1to1);
noMaskVsCosThetaNeg1to1Stack->Add(noMaskAlphaVsCosThetaNeg1to1);

noMaskVsTheta0to180Stack->Draw();
noMaskVsTheta0to180Stack->SetTitle("noMaskVsTheta0to180Stack");
noMaskVsTheta0to180Stack->GetXaxis()->SetTitle("#theta");
noMaskVsTheta0to180Stack->GetYaxis()->SetTitle("allTrks");
noMaskVsTheta0to180Stack->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskVsTheta0to180.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);
noMaskVsCosThetaNeg1to1Stack->Draw();
noMaskVsCosThetaNeg1to1Stack->SetTitle("noMaskVsCosThetaNeg1to1Stack");
noMaskVsCosThetaNeg1to1Stack->GetXaxis()->SetTitle("Cos #theta");
noMaskVsCosThetaNeg1to1Stack->GetYaxis()->SetTitle("allTrks");
noMaskVsCosThetaNeg1to1Stack->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskVsCosThetaNeg1to1.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

THStack *cftVsTheta0to180Stack = new THStack("cftVsTheta0to180Stack","");
THStack *cftVsCosThetaNeg1to1Stack = new THStack("cftVsCosThetaNeg1to1Stack","");

cftVsTheta0to180Stack->Add(cftFragVsTheta0to180);
cftVsTheta0to180Stack->Add(cftAlphaVsTheta0to180);
cftVsCosThetaNeg1to1Stack->Add(cftFragVsCosThetaNeg1to1);
cftVsCosThetaNeg1to1Stack->Add(cftAlphaVsCosThetaNeg1to1);

cftVsTheta0to180Stack->Draw();
cftVsTheta0to180Stack->SetTitle("cftVsTheta0to180Stack");
cftVsTheta0to180Stack->GetXaxis()->SetTitle("#theta");
cftVsTheta0to180Stack->GetYaxis()->SetTitle("allTrks");
cftVsTheta0to180Stack->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftVsTheta0to180.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);
cftVsCosThetaNeg1to1Stack->Draw();
cftVsCosThetaNeg1to1Stack->SetTitle("cftVsCosThetaNeg1to1Stack");
cftVsCosThetaNeg1to1Stack->GetXaxis()->SetTitle("Cos #theta");
cftVsCosThetaNeg1to1Stack->GetYaxis()->SetTitle("allTrks");
cftVsCosThetaNeg1to1Stack->GetYaxis()->SetTitleOffset(1.4);
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftVsCosThetaNeg1to1.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);


////////////////////////////////////////////////////////////////////////
////////////////////////// azimuth vs energy ///////////////////////////
////////////////////////////////////////////////////////////////////////
TH2D* noMaskAzVsADC = new TH2D("noMaskAzVsADC","noMaskAzVsADC",500,-3.2,3.2,40000,0,40000);
TH1D* noMaskAzFrags = new TH1D("noMaskAzFrags","noMaskAzFrags",500,-3.2,3.2);
TH1D* noMaskAzAlphas = new TH1D("noMaskAzAlphas","noMaskAzAlphas",500,-3.2,3.2);

TH2D* cftAzVsADC = new TH2D("cftAzVsADC","cftAzVsADC",500,-3.2,3.2,40000,0,40000);
TH1D* cftAzFrags = new TH1D("cftAzFrags","cftAzFrags",500,-3.2,3.2);
TH1D* cftAzAlphas = new TH1D("cftAzAlphas","cftAzAlphas",500,-3.2,3.2);

noMaskTracks->Draw("adc:direction.Phi()>>noMaskAzVsADC",noMaskCut);
noMaskTracks->Draw("direction.Phi()>>noMaskAzFrags",noMaskFragCut);
noMaskTracks->Draw("direction.Phi()>>noMaskAzAlphas",noMaskAlphaCut);

cftTracks->Draw("adc:direction.Phi()>>cftAzVsADC",cftCut);
cftTracks->Draw("direction.Phi()>>cftAzFrags",cftFragCut);
cftTracks->Draw("direction.Phi()>>cftAzAlphas",cftAlphaCut);

/* make energy histograms */
TH2D* noMaskAzVsEnergy = new TH2D("noMaskAzVsEnergy","noMaskAzVsEnergy",500,-3.2,3.2,40000,0,168.6);
TH2D* cftAzVsEnergy = new TH2D("cftAzVsEnergy","cftAzVsEnergy",500,-3.2,3.2,40000,0,168.6);

for (int azBin = 0; azBin < 1000; ++azBin){
	for (int adcBin = 0; adcBin < 40000; ++adcBin){
		noMaskAzVsEnergy->SetBinContent(azBin,adcBin,noMaskAzVsADC->GetBinContent(azBin,adcBin));
		cftAzVsEnergy->SetBinContent(azBin,adcBin,cftAzVsADC->GetBinContent(azBin,adcBin));
	}
}

c1->SetLogy(1);
noMaskAzVsEnergy->SetMinimum(1);
noMaskAzVsEnergy->GetYaxis()->SetRangeUser(0.4,180);
noMaskAzVsEnergy->GetYaxis()->SetTitle("Track Energy [MeV]");
noMaskAzVsEnergy->GetYaxis()->CenterTitle();
noMaskAzVsEnergy->GetXaxis()->SetTitle("frags Azimuth");
noMaskAzVsEnergy->GetXaxis()->CenterTitle();
noMaskAzVsEnergy->SetTitle("noMaskAzVsEnergy");
noMaskAzVsEnergy->SetLineColor(kGreen);
noMaskAzVsEnergy->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskAzVsEnergy.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

cftAzVsEnergy->SetMinimum(1);
cftAzVsEnergy->GetYaxis()->SetRangeUser(0.4,180);
cftAzVsEnergy->GetYaxis()->SetTitle("Track Energy [MeV]");
cftAzVsEnergy->GetYaxis()->CenterTitle();
cftAzVsEnergy->GetXaxis()->SetTitle("frags Azimuth");
cftAzVsEnergy->GetXaxis()->CenterTitle();
cftAzVsEnergy->SetTitle("cftAzVsEnergy");
cftAzVsEnergy->SetLineColor(kGreen);
cftAzVsEnergy->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftAzVsEnergy.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);
c1->SetLogy(0);

noMaskAzAlphas->GetXaxis()->SetTitle("Azimuth");
noMaskAzAlphas->GetXaxis()->CenterTitle();
noMaskAzAlphas->SetTitle("noMaskAzAlphas");
noMaskAzAlphas->SetLineColor(kGreen);
noMaskAzAlphas->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskAzAlphas.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

noMaskAzFrags->GetXaxis()->SetTitle("Azimuth");
noMaskAzFrags->GetXaxis()->CenterTitle();
noMaskAzFrags->SetTitle("noMaskAzFrags");
noMaskAzFrags->SetLineColor(kViolet);
noMaskAzFrags->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskAzFrags.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

cftAzAlphas->GetXaxis()->SetTitle("Azimuth");
cftAzAlphas->GetXaxis()->CenterTitle();
cftAzAlphas->SetTitle("cftAzAlphas");
cftAzAlphas->SetLineColor(kGreen);
cftAzAlphas->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftAzAlphas.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

cftAzFrags->GetXaxis()->SetTitle("Azimuth");
cftAzFrags->GetXaxis()->CenterTitle();
cftAzFrags->SetTitle("cftAzFrags");
cftAzFrags->SetLineColor(kViolet);
cftAzFrags->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftAzFrags.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

////////////////////////////////////////////////////////////////////////
////////////////////////// vertex vs energy ///////////////////////////
////////////////////////////////////////////////////////////////////////
TH2D* noMaskVertex = new TH2D("noMaskVertex","noMaskVertex",500,-3.2,3.2,500,-3.2,3.2);
TH2D* noMaskVertexFrags = new TH2D("noMaskVertexFrags","noMaskVertexFrags",500,-3.2,3.2,500,-3.2,3.2);
TH2D* noMaskVertexAlphas = new TH2D("noMaskVertexAlphas","noMaskVertexAlphas",500,-3.2,3.2,500,-3.2,3.2);

TH2D* cftVertex = new TH2D("cftVertex","cftVertex",500,-3.2,3.2,500,-3.2,3.2);
TH2D* cftVertexFrags = new TH2D("cftVertexFrags","cftVertexFrags",500,-3.2,3.2,500,-3.2,3.2);
TH2D* cftVertexAlphas = new TH2D("cftVertexAlphas","cftVertexAlphas",500,-3.2,3.2,500,-3.2,3.2);

noMaskTracks->Draw("start.x():start.y()>>noMaskVertex",noMaskCut);
noMaskTracks->Draw("start.x():start.y()>>noMaskVertexFrags",noMaskFragCut);
noMaskTracks->Draw("start.x():start.y()>>noMaskVertexAlphas",noMaskAlphaCut);

cftTracks->Draw("start.x():start.y()>>cftVertex",cftCut);
cftTracks->Draw("start.x():start.y()>>cftVertexFrags",cftFragCut);
cftTracks->Draw("start.x():start.y()>>cftVertexAlphas",cftAlphaCut);

noMaskVertex->GetYaxis()->SetTitle("[cm]");
noMaskVertex->GetYaxis()->CenterTitle();
noMaskVertex->GetXaxis()->SetTitle("[cm]");
noMaskVertex->GetXaxis()->CenterTitle();
noMaskVertex->SetTitle("noMask fragment vertex");
noMaskVertex->SetLineColor(kGreen);
noMaskVertex->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskVertex.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

noMaskVertexFrags->GetYaxis()->SetTitle("[cm]");
noMaskVertexFrags->GetYaxis()->CenterTitle();
noMaskVertexFrags->GetXaxis()->SetTitle("[cm]");
noMaskVertexFrags->GetXaxis()->CenterTitle();
noMaskVertexFrags->SetTitle("noMask fragment vertex");
noMaskVertexFrags->SetLineColor(kGreen);
noMaskVertexFrags->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskVertexFrags.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

noMaskVertexAlphas->GetYaxis()->SetTitle("[cm]");
noMaskVertexAlphas->GetYaxis()->CenterTitle();
noMaskVertexAlphas->GetXaxis()->SetTitle("[cm]");
noMaskVertexAlphas->GetXaxis()->CenterTitle();
noMaskVertexAlphas->SetTitle("noMask alpha vertex");
noMaskVertexAlphas->SetLineColor(kViolet);
noMaskVertexAlphas->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.noMaskVertexAlphas.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

cftVertex->GetYaxis()->SetTitle("[cm]");
cftVertex->GetYaxis()->CenterTitle();
cftVertex->GetXaxis()->SetTitle("[cm]");
cftVertex->GetXaxis()->CenterTitle();
cftVertex->SetTitle("cft fragment vertex");
cftVertex->SetLineColor(kGreen);
cftVertex->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftVertex.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

cftVertexFrags->GetYaxis()->SetTitle("[cm]");
cftVertexFrags->GetYaxis()->CenterTitle();
cftVertexFrags->GetXaxis()->SetTitle("[cm]");
cftVertexFrags->GetXaxis()->CenterTitle();
cftVertexFrags->SetTitle("cft fragment vertex");
cftVertexFrags->SetLineColor(kGreen);
cftVertexFrags->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftVertexFrags.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);

cftVertexAlphas->GetYaxis()->SetTitle("[cm]");
cftVertexAlphas->GetYaxis()->CenterTitle();
cftVertexAlphas->GetXaxis()->SetTitle("[cm]");
cftVertexAlphas->GetXaxis()->CenterTitle();
cftVertexAlphas->SetTitle("cft alpha vertex");
cftVertexAlphas->SetLineColor(kViolet);
cftVertexAlphas->Draw("colz");
sprintf(c1PlotName,"%s.preamp%d.pidStep%02d.cftVertexAlphas.png",actinide,preamp,pidStep);
c1->SaveAs(c1PlotName);


///////////////////  write histograms to file ///////////////////
TFile* plotTreeHistgrams = new TFile("plotTreeHistgrams.root","recreate");

noMaskLvADC->Write();
noMaskLvADCFrags->Write();
noMaskLvADCAlphas->Write();
cftLvADC->Write();
cftLvADCFrags->Write();
cftLvADCAlphas->Write();
noMaskLvEn->Write();
noMaskLvEnFrags->Write();
noMaskLvEnAlphas->Write();
cftLvEn->Write();
cftLvEnFrags->Write();
cftLvEnAlphas->Write();
noMaskAzVsADC->Write();
cftAzVsADC->Write();
noMaskAzVsEnergy->Write();
cftAzVsEnergy->Write();
noMaskVertex->Write();
noMaskVertexFrags->Write();
noMaskVertexAlphas->Write();
cftVertex->Write();
cftVertexFrags->Write();
cftVertexAlphas->Write();

cftFragsADC->Write();
cftAlphasADC->Write();
cftADC->Write();
noMaskFragsADC->Write();
noMaskAlphasADC->Write();
noMaskADC->Write();
noMaskFrags->Write();
noMaskAlphas->Write();
cftFrags->Write();
cftAlphas->Write();
scaledCftFrags->Write();
cftEfficiency->Write();
cftFragsScaleRegion->Write();
noMaskVsTheta0to180->Write();
noMaskVsCosThetaNeg1to1->Write();
noMaskFragVsTheta0to180->Write();
noMaskFragVsCosThetaNeg1to1->Write();
noMaskAlphaVsTheta0to180->Write();
noMaskAlphaVsCosThetaNeg1to1->Write();
cftVsTheta0to180->Write();
cftVsCosThetaNeg1to1->Write();
cftFragVsTheta0to180->Write();
cftFragVsCosThetaNeg1to1->Write();
cftAlphaVsTheta0to180->Write();
cftAlphaVsCosThetaNeg1to1->Write();
noMaskAzFrags->Write();
noMaskAzAlphas->Write();
cftAzFrags->Write();
cftAzAlphas->Write();

vertexCut->Write();
fragCutADC->Write();
alphaCutADC->Write();
fragCutEnergy->Write();
alphaCutEnergy->Write();

}

