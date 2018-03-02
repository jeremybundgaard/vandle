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

void pidCut_zCut(int preamp, const char* actinide,const char* treeFile,const char* cuts="&&1",const char* driftVel="") {

	gStyle->SetOptStat(0);
	gStyle->SetPalette(57);
	gStyle->SetNumberContours(99);
	gStyle->SetTitleY(.98);
	gStyle->SetTitleFontSize(1.4*gStyle->GetTitleFontSize());
	gStyle->SetTitleXSize(1.3*gStyle->GetTitleXSize());
	gStyle->SetTitleYOffset(0.7);
	gStyle->SetTitleYSize(1.3*gStyle->GetTitleYSize());
	TGaxis::SetMaxDigits(3);

int nStdDev=9;
int minStdDev=3;
double fragEnergyThreshold=0;
double dEnergy=0;
double random_dEnergy=0;
double dLength=0;
double random_dLength=0;
double ADC_per_MeV=0;
char charBuff[123];
int xBuffer = 5;
double tcutStep=0.1; /*MeV*/
int zCut=0;
int nLengthBins=0;
double frag_energy_lengthline_intercept=0;
double frag_energy_lengthline_slope=0;

// char polarCuts[567]="&&((-0.1>direction.CosTheta()&&direction.CosTheta()>-0.9)||(0.1<direction.CosTheta()&&direction.CosTheta()<0.9))";
char polarCuts[567]="&&((-0.15>direction.CosTheta()&&direction.CosTheta()>-0.85)||(0.15<direction.CosTheta()&&direction.CosTheta()<0.85))";
// char polarCuts[567]="&&(1.7453>direction.Theta()||1.396>direction.Theta())";
// char polarCuts[567]="&&1";
// char digitCut[567]="&&nDigits>60";
char azCut[567]="";
char runCut[567]="";
char treeCut[567]="";
char yProj_Cut[567]="";
char extrapoCut[567]="";


TCutG* targetHolder_lvse = new TCutG("targetHolder_lvse",5);
targetHolder_lvse->SetPoint(0,2.31,1.55);
targetHolder_lvse->SetPoint(1,3.18,2.16);
targetHolder_lvse->SetPoint(2,1.54,2.15);
targetHolder_lvse->SetPoint(3,1.13,1.56);
targetHolder_lvse->SetPoint(4,2.31,1.55);
if (preamp==1){
	if (!(strcmp(actinide,"cf252"))){
		printf("actinide:%s\n preamp:%d\n",actinide,preamp);

		zCut=70;
		dEnergy=0.25;
		dLength=0.1;
		random_dLength=0;
		nLengthBins=1000;
		ADC_per_MeV=1180/(double)6.118;
		fragEnergyThreshold=1.7;
		frag_energy_lengthline_slope=0.065;
		frag_energy_lengthline_intercept=0.8;
		printf("ADC_per_MeV: %f\n",ADC_per_MeV);
		sprintf(runCut,"run>100001856&&run!=100001870");
		sprintf(treeCut,"%s  %s",polarCuts,cuts);
	}
	if (!(strcmp(actinide,"cm244"))){
		printf("actinide:%s\n preamp:%d\n",actinide,preamp);

		zCut=10;
		dLength=0.1;
		dEnergy=0.3;
		random_dLength=0;
		nLengthBins=600;
		ADC_per_MeV=1214/(double)5.800;
		fragEnergyThreshold=1.2;
		frag_energy_lengthline_slope=0.045;
		frag_energy_lengthline_intercept=0.6;
		printf("ADC_per_MeV: %f\n",ADC_per_MeV);
		sprintf(runCut,"run==100001995");
		sprintf(treeCut,"%s",runCut);
		sprintf(azCut,"1");
		sprintf(yProj_Cut,"[-targetHolder_lvse]");

	}
}
else if (preamp==2){
	if (!(strcmp(actinide,"cf252"))){
		printf("actinide:%s\n preamp:%d\n",actinide,preamp);

		zCut=30;
		dEnergy=0.25;
		dLength=0.1;
		random_dLength=0;
		nLengthBins=1000;
		ADC_per_MeV=1450/(double)6.118;
		fragEnergyThreshold=1.3;
		frag_energy_lengthline_slope=0.065;
		frag_energy_lengthline_intercept=0.7;
		printf("ADC_per_MeV: %f\n",ADC_per_MeV);
		sprintf(azCut,"!(-1<direction.Phi()&&direction.Phi()<0)");
		sprintf(treeCut,"%s %s %s",azCut,polarCuts,cuts);

	}
	if ( !(strcmp(actinide,"cm244"))){
		printf("actinide:%s\n preamp:%d\n",actinide,preamp);

		zCut=20;
		dLength=0.1;
		random_dLength=0;
		nLengthBins=666;
		ADC_per_MeV=1214/(double)5.800;
		fragEnergyThreshold=2;
		frag_energy_lengthline_slope=0.065;
		frag_energy_lengthline_intercept=0.7;
		printf("ADC_per_MeV: %f\n",ADC_per_MeV);
		sprintf(azCut,"direction.Phi()<2.2");
		sprintf(treeCut,"%s",azCut);
		sprintf(yProj_Cut,"[-targetHolder_lvse]");
	}
}
double length_per_bin=10/(double)nLengthBins;
double MeV_per_ADC_bin=10/(double)ADC_per_MeV;
printf("treeCut: %s\n", treeCut);

printf("\n\n");
printf("treeFile:%s\n",treeFile);
TFile* noMaskFile = new TFile(treeFile);
TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");
TH2D* noMaskLvEn = new TH2D("noMaskLvEn","noMaskLvEn",4000,0,40000/(double)ADC_per_MeV,nLengthBins,0,10);
TH2D* noMaskLvEn_zCut_alphas = new TH2D("noMaskLvEn_zCut_alphas","noMaskLvEn_zCut_alphas",4000,0,40000/(double)ADC_per_MeV,nLengthBins,0,10);
noMaskLvEn->GetXaxis()->SetRangeUser(0,10);
noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn->GetXaxis()->CenterTitle();
noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn->GetYaxis()->CenterTitle();
sprintf(charBuff,"length:adc*%f>>noMaskLvEn",1/(double)ADC_per_MeV);
printf("ADC_per_MeV: %f\n",ADC_per_MeV);
noMaskTracks->Draw(charBuff,treeCut,"colz");


sprintf(charBuff,"$pid/%s_pidCut_preamp%d_zCut%d_fragThresh%0.1f.driftVel%s.root",actinide,preamp,zCut,fragEnergyThreshold,driftVel);
TFile* outfile = new TFile(charBuff,"recreate");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
char c1PlotName[234];
c1->SetGridy(1);
c1->SetGridx(1);
gStyle->SetOptStat(0);
gStyle->SetOptFit(1);
gStyle->SetPalette(57);
gStyle->SetNumberContours(99);
gStyle->SetLegendBorderSize(0);
sprintf(charBuff,"noMaskLvEn_above_zCut%04d",zCut);
TH2D * noMaskLvEn_above_zCut = new TH2D(charBuff,charBuff,4000,0,40000/(double)ADC_per_MeV,nLengthBins,0,10);
noMaskLvEn_above_zCut->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_above_zCut->GetXaxis()->CenterTitle();
noMaskLvEn_above_zCut->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_above_zCut->GetYaxis()->CenterTitle();
sprintf(charBuff,"noMaskLvEn_below_zCut%04d",zCut);
TH2D * noMaskLvEn_below_zCut = new TH2D(charBuff,charBuff,4000,0,40000/(double)ADC_per_MeV,nLengthBins,0,10);
noMaskLvEn_below_zCut->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn_below_zCut->GetXaxis()->CenterTitle();
noMaskLvEn_below_zCut->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn_below_zCut->GetYaxis()->CenterTitle();
for (int energyBin = 0; energyBin < 4000; ++energyBin){
	for (int lengthBin = 0; lengthBin < nLengthBins; ++lengthBin){
		noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,0);
		noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,0);
		int counts=noMaskLvEn->GetBinContent(energyBin,lengthBin);
		if (counts<=zCut){
			noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,counts);
		}
		else{
			// printf("energyBin:%d \t lengthBin:%d counts:%d  \n",energyBin,lengthBin,counts);
			noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,counts);
		}
	}
}

c1->SetLogx(1);c1->SetLogz(0);
noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.4,140);
noMaskLvEn_below_zCut->Draw("colz");
c1->SaveAs("noMaskLvEn_below_zCut.png");
noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.4,140);
noMaskLvEn_above_zCut->Draw("colz");
c1->SaveAs("noMaskLvEn_above_zCut.png");

///////////////////////// creating alpha zCut TCutG /////////////////////////////
printf("\n\n/////////////////////// creating alpha zCut TCutG ///////////////////\n\n");
ofstream alphaPrimitive;
noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.1,15);
noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,5);
TH1D* yProj = noMaskLvEn_above_zCut->ProjectionY("yProj",0,nLengthBins,yProj_Cut);
yProj->Draw();
c1->SetLogy(0);
sprintf(charBuff,"alphaCut_zCut_energy");
TCutG* alphaCut_zCut_energy = new TCutG(charBuff,10);
alphaCut_zCut_energy->SetVarX("adc");
alphaCut_zCut_energy->SetVarY("length");
alphaCut_zCut_energy->SetLineColor(kGreen);
alphaCut_zCut_energy->SetLineWidth(1);
alphaCut_zCut_energy->SetFillColorAlpha(kGreen,0.3);
double alpha_maxCutLength=yProj->FindLastBinAbove(0)*length_per_bin;
TF1* f_alphaCut_long_low_energy = new TF1("f_alphaCut_long_low_energy","[1]*x+[0]",0,12);
f_alphaCut_long_low_energy->SetParameter(0,2);
f_alphaCut_long_low_energy->SetParameter(1,7/(double)12);
TF1* frag_energy_lengthline = new TF1("frag_energy_lengthline","[1]*x+[0]",0,100);
frag_energy_lengthline->SetLineWidth(1);
frag_energy_lengthline->SetParameter(0,frag_energy_lengthline_intercept);
// frag_energy_lengthline->SetParameter(0,0.8);
// frag_energy_lengthline->SetParameter(1,0.065);
frag_energy_lengthline->SetParameter(1,frag_energy_lengthline_slope);
TCutG* alphaCut_long_low_energy	= new TCutG("alphaCut_long_low_energy",3);
alphaCut_long_low_energy->SetVarX("adc");
alphaCut_long_low_energy->SetVarY("length");
alphaCut_long_low_energy->SetLineColor(kYellow);
alphaCut_long_low_energy->SetLineWidth(1);
alphaCut_long_low_energy->SetFillColorAlpha(kYellow,0.3);
alphaCut_long_low_energy->SetPoint(0,0,2);
alphaCut_long_low_energy->SetPoint(1,0,10);
alphaCut_long_low_energy->SetPoint(2,12,10);
alphaCut_long_low_energy->SetPoint(3,f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);
alphaCut_long_low_energy->SetPoint(4,0,2);
ofstream alphaCut_long_low_energy_primitive;
sprintf(charBuff,"alphaCut_long_low_energy.C");
alphaCut_long_low_energy_primitive.open(charBuff);
alphaCut_long_low_energy->SavePrimitive(alphaCut_long_low_energy_primitive);
TCutG* alphaCut_long_energy	= new TCutG("alphaCut_long_energy",3);
alphaCut_long_energy->SetVarX("adc");
alphaCut_long_energy->SetVarY("length");
alphaCut_long_energy->SetLineColor(kCyan);
alphaCut_long_energy->SetLineWidth(1);
alphaCut_long_energy->SetFillColorAlpha(kCyan,0.3);
alphaCut_long_energy->SetPoint(0,12,10);
alphaCut_long_energy->SetPoint(1,f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);
alphaCut_long_energy->SetPoint(2,15,alpha_maxCutLength);
alphaCut_long_energy->SetPoint(3,15,10);
alphaCut_long_energy->SetPoint(4,12,10);
TCutG* long_high_energy	= new TCutG("long_high_energy",3);
long_high_energy->SetVarX("adc");
long_high_energy->SetVarY("length");
long_high_energy->SetLineColor(kBlue);
long_high_energy->SetLineWidth(1);
long_high_energy->SetFillColorAlpha(kBlue,0.3);
long_high_energy->SetPoint(0,15,frag_energy_lengthline->Eval(15));
long_high_energy->SetPoint(1,30,frag_energy_lengthline->Eval(30));
long_high_energy->SetPoint(2,71,3.5);
long_high_energy->SetPoint(3,200,4);
long_high_energy->SetPoint(4,200,10);
long_high_energy->SetPoint(5,15,10);
long_high_energy->SetPoint(6,15,frag_energy_lengthline->Eval(15));
sprintf(charBuff,"alphaCut_short_low_energy");
TCutG* alphaCut_short_low_energy= new TCutG(charBuff,4);
alphaCut_short_low_energy->SetVarX("adc");
alphaCut_short_low_energy->SetVarY("length");
alphaCut_short_low_energy->SetLineColor(kRed);
alphaCut_short_low_energy->SetLineWidth(1);
alphaCut_short_low_energy->SetFillColorAlpha(kRed,0.3);
int short_low_energy_pointNum=1;


sprintf(charBuff,"low_energy");
TCutG* low_energy = new TCutG(charBuff,4);
low_energy->SetVarX("adc");
low_energy->SetVarY("length");
low_energy->SetLineColor(kYellow+1);
low_energy->SetLineWidth(1);
low_energy->SetFillColorAlpha(kYellow+1,0.3);
int low_energy_pointNum=1;

int zCut_pointNum=1;
int midpoint_length=(yProj->FindFirstBinAbove(0)+yProj->FindLastBinAbove(0))/2*length_per_bin;
//////////////////  low energy side of zCut //////////////////
printf("//////////////////  low energy side of zCut //////////////////\n\n");
double first_length2=0;
double first_length1=0;
double first_energy=0;
double last_length2=0;
double last_length1=0;
double last_energy=0;
double first_fragEnergyThreshold_length=0;
double first_fragEnergyThreshold_energy=0;
bool cutoff_switch=0;
for (int lengthBin = yProj->FindFirstBinAbove(0); lengthBin <= yProj->FindLastBinAbove(0); ++lengthBin ){
	TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px", lengthBin, lengthBin,yProj_Cut);
	double length1=(lengthBin-1)*length_per_bin+random_dLength;
	double length2=lengthBin*length_per_bin+random_dLength;
	double energy=(px->FindFirstBinAbove(0)-1)*MeV_per_ADC_bin;
	energy=energy-dEnergy+random_dEnergy;
	if(first_energy==0)first_energy=energy;
	if(first_length1==0)first_length1=length1;
	if(first_length2==0)first_length2=length2;
	alphaCut_short_low_energy->SetPoint(short_low_energy_pointNum,energy,length1);short_low_energy_pointNum++;
	alphaCut_short_low_energy->SetPoint(short_low_energy_pointNum,energy,length2);short_low_energy_pointNum++;
	if (energy>fragEnergyThreshold) {
		alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length1);zCut_pointNum++;
		alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length2);zCut_pointNum++;
		low_energy->SetPoint(low_energy_pointNum,energy,length2);
		if(first_fragEnergyThreshold_length==0)first_fragEnergyThreshold_length=length1;
		if(first_fragEnergyThreshold_energy==0)first_fragEnergyThreshold_energy=energy;
	}
	else{
		low_energy->SetPoint(low_energy_pointNum,energy,length1);low_energy_pointNum++;
		low_energy->SetPoint(low_energy_pointNum,energy,length2);low_energy_pointNum++;
	}
}

low_energy->SetPoint(low_energy_pointNum,first_fragEnergyThreshold_energy,first_fragEnergyThreshold_length);low_energy_pointNum++;
low_energy->SetPoint(low_energy_pointNum,first_fragEnergyThreshold_energy,0);low_energy_pointNum++;
low_energy->SetPoint(low_energy_pointNum,0,0);low_energy_pointNum++;
low_energy->SetPoint(low_energy_pointNum,0,first_length1);low_energy_pointNum++;
low_energy->SetPoint(low_energy_pointNum,first_energy,first_length1);low_energy_pointNum++;
low_energy->SetPoint(0,first_energy,first_length1);

sprintf(charBuff,"junkCut_energy");
TCutG* junkCut_energy = new TCutG(charBuff,4);
junkCut_energy->SetVarX("adc");
junkCut_energy->SetVarY("length");
junkCut_energy->SetLineColor(28);
junkCut_energy->SetLineWidth(1);
junkCut_energy->SetFillColorAlpha(28,0.3);
int junk_energy_pointNum=1;

sprintf(charBuff,"fragCut_energy");
TCutG* fragCut_energy = new TCutG(charBuff,4);
fragCut_energy->SetVarX("adc");
fragCut_energy->SetVarY("length");
fragCut_energy->SetLineColor(kViolet);
fragCut_energy->SetLineWidth(1);
fragCut_energy->SetFillColorAlpha(kViolet,0.3);
int frag_energy_pointNum=1;

alphaCut_short_low_energy->SetPoint(short_low_energy_pointNum,f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);short_low_energy_pointNum++;
alphaCut_short_low_energy->SetPoint(short_low_energy_pointNum,0,2);short_low_energy_pointNum++;
alphaCut_short_low_energy->SetPoint(short_low_energy_pointNum,0,first_length1);short_low_energy_pointNum++;
alphaCut_short_low_energy->SetPoint(0,0,first_length1);

sprintf(charBuff,"alphaCut_short_higher_energy");
TCutG* alphaCut_short_higher_energy = new TCutG(charBuff,4);
alphaCut_short_higher_energy->SetVarX("adc");
alphaCut_short_higher_energy->SetVarY("length");
alphaCut_short_higher_energy->SetLineColor(kOrange+7);
alphaCut_short_higher_energy->SetLineWidth(1);
alphaCut_short_higher_energy->SetFillColorAlpha(kOrange+7,0.3);
int alphaCut_short_higher_energy_pointNum=1;
alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,15,frag_energy_lengthline->Eval(15));alphaCut_short_higher_energy_pointNum++;
alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,15,alpha_maxCutLength);alphaCut_short_higher_energy_pointNum++;

//////////////////  high energy side of zCut //////////////////
printf("//////////////////  high energy side of zCut //////////////////\n\n");
bool fragCut_switch=0;
double last_fragEnergyThreshold_length1=0;
double last_fragEnergyThreshold_length2=0;
double tripple_point_energy=0;
double tripple_point_length=0;
for (int lengthBin = yProj->FindLastBinAbove(0); lengthBin >= yProj->FindFirstBinAbove(0); --lengthBin ){
	TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px", lengthBin, lengthBin);
	double length1=(lengthBin-1)*length_per_bin+random_dLength;
	double length2=lengthBin*length_per_bin+random_dLength;
	last_length2=length2;
	last_length1=length1;
	double energy=px->FindLastBinAbove(0)*MeV_per_ADC_bin;
	energy=energy+dEnergy*1.2+random_dEnergy;
	int adc = TMath::Ceil(energy*ADC_per_MeV);
	if (length2>frag_energy_lengthline->Eval(energy)){
		alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length2);zCut_pointNum++;
		alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length1);zCut_pointNum++;
		alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,energy,length2);alphaCut_short_higher_energy_pointNum++;
		alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,energy,length1);alphaCut_short_higher_energy_pointNum++;
	}
	else{
		if (fragCut_switch==0){
			alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length2);zCut_pointNum++;
			alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length1);zCut_pointNum++;

			alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,energy,length2);alphaCut_short_higher_energy_pointNum++;
			alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,energy,length1);alphaCut_short_higher_energy_pointNum++;

			fragCut_switch=1;
			tripple_point_energy=energy;
			tripple_point_length=length1;
			printf("tripple_point:%f,%f\n",tripple_point_energy,tripple_point_length);
			continue;
		}

		if (energy>fragEnergyThreshold) {
			alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length2);zCut_pointNum++;
			alphaCut_zCut_energy->SetPoint(zCut_pointNum,energy,length1);zCut_pointNum++;

			fragCut_energy->SetPoint(frag_energy_pointNum,energy,length2);frag_energy_pointNum++;
			fragCut_energy->SetPoint(frag_energy_pointNum,energy,length1);frag_energy_pointNum++;
			last_energy=energy;
			last_fragEnergyThreshold_length1=length1;
			last_fragEnergyThreshold_length2=length2;
			junkCut_energy->SetPoint(junk_energy_pointNum,energy,length2);
			junkCut_energy->SetPoint(junk_energy_pointNum,energy,length1);
		}
		else{
			junkCut_energy->SetPoint(junk_energy_pointNum,energy,length2);junk_energy_pointNum++;
			junkCut_energy->SetPoint(junk_energy_pointNum,energy,length1);junk_energy_pointNum++;
		}
	}
}
junkCut_energy->SetPoint(junk_energy_pointNum,0,first_length1);junk_energy_pointNum++;
junkCut_energy->SetPoint(junk_energy_pointNum,0,0);junk_energy_pointNum++;
junkCut_energy->SetPoint(junk_energy_pointNum,first_fragEnergyThreshold_energy,0	);junk_energy_pointNum++;
junkCut_energy->SetPoint(junk_energy_pointNum,first_fragEnergyThreshold_energy,last_fragEnergyThreshold_length1);junk_energy_pointNum++;
junkCut_energy->SetPoint(0,first_fragEnergyThreshold_energy,last_fragEnergyThreshold_length1);junk_energy_pointNum++;

alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,tripple_point_energy,tripple_point_length);alphaCut_short_higher_energy_pointNum++;
for (size_t en = 3; en < 15 ; en++){
	alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,en,frag_energy_lengthline->Eval(en));alphaCut_short_higher_energy_pointNum++;
}
alphaCut_short_higher_energy->SetPoint(alphaCut_short_higher_energy_pointNum,15,frag_energy_lengthline->Eval(15));
alphaCut_short_higher_energy->SetPoint(0,15,frag_energy_lengthline->Eval(15));alphaCut_short_higher_energy_pointNum++;

alphaCut_zCut_energy->SetPoint(zCut_pointNum,first_fragEnergyThreshold_energy,last_fragEnergyThreshold_length1);
alphaCut_zCut_energy->SetPoint(0,first_fragEnergyThreshold_energy,last_fragEnergyThreshold_length1);

fragCut_energy->SetPoint(frag_energy_pointNum,first_fragEnergyThreshold_energy,last_fragEnergyThreshold_length1);frag_energy_pointNum++;
fragCut_energy->SetPoint(frag_energy_pointNum,first_fragEnergyThreshold_energy,0);frag_energy_pointNum++;
fragCut_energy->SetPoint(frag_energy_pointNum,first_fragEnergyThreshold_energy,0);frag_energy_pointNum++;
fragCut_energy->SetPoint(frag_energy_pointNum,200,0);frag_energy_pointNum++;
fragCut_energy->SetPoint(frag_energy_pointNum,200,4);frag_energy_pointNum++;
fragCut_energy->SetPoint(frag_energy_pointNum,71,3.5);frag_energy_pointNum++;
fragCut_energy->SetPoint(frag_energy_pointNum,30,frag_energy_lengthline->Eval(30));frag_energy_pointNum++;
for (size_t en = 15; en >= 3; en--){
	fragCut_energy->SetPoint(frag_energy_pointNum,en,frag_energy_lengthline->Eval(en));frag_energy_pointNum++;
}
fragCut_energy->SetPoint(frag_energy_pointNum,tripple_point_energy,tripple_point_length);
	fragCut_energy->SetPoint(0,tripple_point_energy,tripple_point_length);

sprintf(charBuff,"highEn_fragCut_energy");
TCutG* highEn_fragCut_energy = new TCutG(charBuff,4);
highEn_fragCut_energy->SetVarX("adc");
highEn_fragCut_energy->SetVarY("length");
highEn_fragCut_energy->SetLineColor(kViolet);
highEn_fragCut_energy->SetLineWidth(1);
highEn_fragCut_energy->SetFillColorAlpha(kViolet,0.3);
int cft_frag_energy_pointNum=0;
highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,200,0);cft_frag_energy_pointNum++;
highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,200,4);cft_frag_energy_pointNum++;
highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,71,3.5);cft_frag_energy_pointNum++;
highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,30,frag_energy_lengthline->Eval(30));cft_frag_energy_pointNum++;
for (size_t en = 15; en >= 7; en--){
	highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,en,frag_energy_lengthline->Eval(en));cft_frag_energy_pointNum++;
}
highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,7,0);cft_frag_energy_pointNum++;
highEn_fragCut_energy->SetPoint(cft_frag_energy_pointNum,200,0);

TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px",yProj->FindFirstBinAbove(0),yProj->FindFirstBinAbove(0));
sprintf(charBuff,"alphaCut_zCut%d.C",zCut);
alphaPrimitive.open(charBuff);
alphaCut_zCut_energy->SavePrimitive(alphaPrimitive);

noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,4);
noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0,4);
c1->SetLogx(0);
noMaskLvEn_above_zCut->SetStats(kFALSE);
noMaskLvEn_above_zCut->Draw("colz [-targetHolder_lvse]");
alphaCut_zCut_energy->Draw("same");
alphaCut_zCut_energy->Draw("same f");
sprintf(charBuff,"noMaskLvEn_above_zCut%03d_lowEn.png",zCut);
c1->SaveAs(charBuff);

double energy,length;
TCutG* alphaCut_zCut_adc = new TCutG("alphaCut_zCut_adc",4);
alphaCut_zCut_adc->SetVarX("adc");
alphaCut_zCut_adc->SetVarY("length");
for (size_t i = 0; i < alphaCut_zCut_energy->GetN(); i++) {
	alphaCut_zCut_energy->GetPoint(i,energy,length);
	alphaCut_zCut_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}
TCutG* alphaCut_long_low_adc = new TCutG("alphaCut_long_low_adc",4);
alphaCut_long_low_adc->SetVarX("adc");
alphaCut_long_low_adc->SetVarY("length");
for (size_t i = 0; i < alphaCut_long_low_energy->GetN(); i++) {
	alphaCut_long_low_energy->GetPoint(i,energy,length);
	alphaCut_long_low_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* alphaCut_long_adc = new TCutG("alphaCut_long_adc",4);
alphaCut_long_adc->SetVarX("adc");
alphaCut_long_adc->SetVarY("length");
for (size_t i = 0; i < alphaCut_long_energy->GetN(); i++) {
	alphaCut_long_energy->GetPoint(i,energy,length);
	alphaCut_long_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* long_high_adc = new TCutG("long_high_adc",4);
long_high_adc->SetVarX("adc");
long_high_adc->SetVarY("length");
for (size_t i = 0; i < long_high_energy->GetN(); i++) {
	long_high_energy->GetPoint(i,energy,length);
	long_high_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* alphaCut_short_low_adc = new TCutG("alphaCut_short_low_adc",4);
alphaCut_short_low_adc->SetVarX("adc");
alphaCut_short_low_adc->SetVarY("length");
for (size_t i = 0; i < alphaCut_short_low_energy->GetN(); i++) {
	alphaCut_short_low_energy->GetPoint(i,energy,length);
	alphaCut_short_low_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* low_adc = new TCutG("low_adc",4);
low_adc->SetVarX("adc");
low_adc->SetVarY("length");
for (size_t i = 0; i < low_energy->GetN(); i++) {
	low_energy->GetPoint(i,energy,length);
	low_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* junkCut_adc = new TCutG("junkCut_adc",4);
junkCut_adc->SetVarX("adc");
junkCut_adc->SetVarY("length");
for (size_t i = 0; i < junkCut_energy->GetN(); i++) {
	junkCut_energy->GetPoint(i,energy,length);
	junkCut_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* fragCut_adc = new TCutG("fragCut_adc",4);
fragCut_adc->SetVarX("adc");
fragCut_adc->SetVarY("length");
for (size_t i = 0; i < fragCut_energy->GetN(); i++) {
	fragCut_energy->GetPoint(i,energy,length);
	fragCut_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* alphaCut_short_higher_adc = new TCutG("alphaCut_short_higher_adc",4);
alphaCut_short_higher_adc->SetVarX("adc");
alphaCut_short_higher_adc->SetVarY("length");
for (size_t i = 0; i < alphaCut_short_higher_energy->GetN(); i++) {
	alphaCut_short_higher_energy->GetPoint(i,energy,length);
	alphaCut_short_higher_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}

TCutG* highEn_fragCut_adc = new TCutG("highEn_fragCut_adc",4);
highEn_fragCut_adc->SetVarX("adc");
highEn_fragCut_adc->SetVarY("length");
for (size_t i = 0; i < highEn_fragCut_energy->GetN(); i++) {
	highEn_fragCut_energy->GetPoint(i,energy,length);
	highEn_fragCut_adc->SetPoint(i,TMath::Ceil(ADC_per_MeV*energy),length);
}


// alphaCut_short_low_energy->Draw("same f");
// alphaCut_short_higher_energy->Draw("same f");
// alphaCut_long_low_energy->Draw("same f");
// alphaCut_long_energy->Draw("same f");
// long_high_energy->Draw("same f");
// junkCut_energy->Draw("same f");
// fragCut_energy->Draw("same f");
// if (!(strcmp(actinide,"cm244"))){
// 	highEn_fragCut_energy->Draw("same f");
// }
alphaCut_short_low_energy->Draw("same");
alphaCut_short_higher_energy->Draw("same");
alphaCut_long_low_energy->Draw("same");
alphaCut_long_energy->Draw("same");
long_high_energy->Draw("same");
// junkCut_energy->Draw("same");
fragCut_energy->Draw("same");
low_energy->Draw("same");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same");
}
sprintf(charBuff,"noMaskLvEn_all_zCut%03d_allcuts_lowEn.png",zCut);
c1->SaveAs(charBuff);

noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.4,140);
noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,10);
c1->SetLogx(1);
noMaskLvEn_above_zCut->SetStats(kFALSE);
noMaskLvEn_above_zCut->Draw("colz [-targetHolder_lvse]");
alphaCut_zCut_energy->Draw("same");
alphaCut_zCut_energy->Draw("same f");
sprintf(charBuff,"noMaskLvEn_above_zCut%03d.png",zCut);
c1->SaveAs(charBuff);

alphaCut_short_low_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
alphaCut_long_energy->Draw("same f");
long_high_energy->Draw("same f");
junkCut_energy->Draw("same f");
fragCut_energy->Draw("same f");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same f");
}
alphaCut_short_low_energy->Draw("same");
alphaCut_short_higher_energy->Draw("same");
alphaCut_long_low_energy->Draw("same");
alphaCut_long_energy->Draw("same");
long_high_energy->Draw("same");
junkCut_energy->Draw("same");
fragCut_energy->Draw("same");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same");
}
sprintf(charBuff,"noMaskLvEn_all_zCut%03d_allcuts.png",zCut);
c1->SaveAs(charBuff);

noMaskLvEn_below_zCut->Draw("colz");
alphaCut_short_low_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
alphaCut_long_energy->Draw("same f");
long_high_energy->Draw("same f");
junkCut_energy->Draw("same f");
fragCut_energy->Draw("same f");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same f");
}
noMaskLvEn_below_zCut->Draw("colz same");
alphaCut_short_low_energy->Draw("same");
alphaCut_short_higher_energy->Draw("same");
alphaCut_long_low_energy->Draw("same");
alphaCut_long_energy->Draw("same");
long_high_energy->Draw("same");
junkCut_energy->Draw("same");
fragCut_energy->Draw("same");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same");
}
sprintf(charBuff,"noMaskLvEn_below_zCut%03d_allcuts.png",zCut);
c1->SaveAs(charBuff);
c1->SetLogx(0);
noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0,8);
noMaskLvEn_below_zCut->GetYaxis()->SetRangeUser(0,6);
sprintf(charBuff,"noMaskLvEn_below_zCut%03d_allcuts_lowEn.png",zCut);
c1->SaveAs(charBuff);

////////////////////////  plotting color pid cuts  ////////////////////
printf("\n\n////////////////////////  plotting color pid cuts  ////////////////////\n\n");
/////////////  draw color cuts ////////////

c1->SetLogx(1);
c1->SetLogz(1);
noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.4,140);
noMaskLvEn_below_zCut->GetYaxis()->SetRangeUser(0,10);
noMaskLvEn_below_zCut->Draw("colz");
alphaCut_zCut_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same f");
alphaCut_short_higher_energy->Draw("same f");
alphaCut_long_low_energy->Draw("same f");
alphaCut_long_energy->Draw("same f");
long_high_energy->Draw("same f");
low_energy->Draw("same f");
fragCut_energy->Draw("same f");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same f");
}
noMaskLvEn_below_zCut->Draw("colz same");
alphaCut_zCut_energy->Draw("same");
alphaCut_short_low_energy->Draw("same");
alphaCut_short_higher_energy->Draw("same");
alphaCut_long_low_energy->Draw("same");
alphaCut_long_energy->Draw("same");
long_high_energy->Draw("same");
low_energy->Draw("same");
fragCut_energy->Draw("same");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_energy->Draw("same");
}

sprintf(charBuff,"alphaCut_below_zCut_full_logxz_energy.png");
c1->SaveAs(charBuff);
c1->SetLogx(0);
noMaskLvEn->GetXaxis()->SetRangeUser(0.4,20);
sprintf(charBuff,"alphaCut_below_zCut_full_energy.png");
c1->SaveAs(charBuff);
TH2D* noMaskLvADC = new TH2D("noMaskLvADC","noMaskLvADC",4000,0,40000,nLengthBins,0,10);
noMaskLvADC->GetXaxis()->SetRangeUser(100,40000);
noMaskLvADC->Draw("colz");
alphaCut_zCut_adc->Draw("same");
alphaCut_zCut_adc->Draw("same f");
alphaCut_short_low_adc->Draw("same f");
alphaCut_short_low_adc->Draw("same");
alphaCut_short_higher_adc->Draw("same f");
alphaCut_short_higher_adc->Draw("same");
alphaCut_long_low_adc->Draw("same f");
alphaCut_long_low_adc->Draw("same");
alphaCut_long_adc->Draw("same f");
alphaCut_long_adc->Draw("same");
long_high_adc->Draw("same");
long_high_adc->Draw("same f");
low_adc->Draw("same f");
low_adc->Draw("same");
if (!(strcmp(actinide,"cm244"))){
	highEn_fragCut_adc->Draw("same f");
	highEn_fragCut_adc->Draw("same");
}
sprintf(charBuff,"alphaCut_below_zCut_full_logxz_adc.png");
c1->SaveAs(charBuff);
c1->SetLogx(0);
noMaskLvADC->GetXaxis()->SetRangeUser(100,4000);
sprintf(charBuff,"alphaCut_below_zCut_full_adc.png");
c1->SaveAs(charBuff);

sprintf(charBuff,"length:adc*%f>>noMaskLvEn",1/(double)ADC_per_MeV);
noMaskTracks->Draw(charBuff,"","colz");
noMaskLvEn->GetXaxis()->SetRangeUser(0,8);
noMaskLvEn->GetYaxis()->SetRangeUser(0,5);
noMaskLvEn->SetStats(0);
junkCut_energy->SetLineWidth(2);
alphaCut_zCut_energy->SetLineWidth(2);
low_energy->SetLineWidth(2);
fragCut_energy->SetLineWidth(2);
highEn_fragCut_energy->SetLineWidth(2);
alphaCut_short_low_energy->SetLineWidth(2);
alphaCut_short_higher_energy->SetLineWidth(2);
c1->SetLogz(0);
noMaskLvEn->Draw("colz");
alphaCut_zCut_energy->Draw("same f");
alphaCut_short_low_energy->Draw("same");
alphaCut_short_higher_energy->Draw("same");
low_energy->Draw("same");
fragCut_energy->Draw("same");
if (!(strcmp(actinide,"cm244"))) highEn_fragCut_energy->Draw("same");
junkCut_energy->Draw("same");
sprintf(charBuff,"1a_tcut_zoom.png");
c1->SaveAs(charBuff);

noMaskTracks->Draw("length:adc>>noMaskLvADC","","colz");
noMaskLvADC->GetXaxis()->SetRangeUser(0,8*ADC_per_MeV);
noMaskLvADC->GetYaxis()->SetRangeUser(0,5);
noMaskLvADC->SetStats(0);
junkCut_adc->SetLineWidth(2);
alphaCut_zCut_adc->SetLineWidth(2);
low_adc->SetLineWidth(2);
fragCut_adc->SetLineWidth(2);
highEn_fragCut_adc->SetLineWidth(2);
alphaCut_short_low_adc->SetLineWidth(2);
alphaCut_short_higher_adc->SetLineWidth(2);
c1->SetLogz(0);
noMaskLvADC->Draw("colz");
alphaCut_zCut_adc->Draw("same f");
alphaCut_short_low_adc->Draw("same");
alphaCut_short_higher_adc->Draw("same");
low_adc->Draw("same");
fragCut_adc->Draw("same");
if (!(strcmp(actinide,"cm244"))) highEn_fragCut_adc->Draw("same");
junkCut_adc->Draw("same");
sprintf(charBuff,"1b_tcut_zoom.png");
c1->SaveAs(charBuff);

if (!(strcmp(actinide,"cf252"))){
////////////// write histograms per cut region //////////////
TH2D* noMaskLvADC_alphaCut_zCut_adc = new TH2D("noMaskLvADC_alphaCut_zCut_adc","noMaskLvADC_alphaCut_zCut_adc",4000,0,40000,nLengthBins,0,10);
sprintf(charBuff,"%s && %s && adc>%.0f",azCut,alphaCut_zCut_adc->GetName(),fragEnergyThreshold*ADC_per_MeV);
printf("charBuff:%s\n",charBuff);
noMaskTracks->Draw("length:adc>>noMaskLvADC_alphaCut_zCut_adc",charBuff);
TH1D* noMaskADC_alphaCut_zCut_adc = (TH1D*)noMaskLvADC_alphaCut_zCut_adc->ProjectionX("noMaskADC_alphaCut_zCut_adc");

TH2D* noMaskLvADC_fragCut_adc = new TH2D("noMaskLvADC_fragCut_adc","noMaskLvADC_fragCut_adc",4000,0,40000,nLengthBins,0,10);
sprintf(charBuff,"%s && %s",azCut,fragCut_adc->GetName());
printf("charBuff:%s\n",charBuff);
noMaskTracks->Draw("length:adc>>noMaskLvADC_fragCut_adc",charBuff);
TH1D* noMaskADC_fragCut_adc = (TH1D*)noMaskLvADC_fragCut_adc->ProjectionX("noMaskADC_fragCut_adc");

TH2D* noMaskLvADC_low_adc = new TH2D("noMaskLvADC_low_adc","noMaskLvADC_low_adc",4000,0,40000,nLengthBins,0,10);
sprintf(charBuff,"%s && %s",azCut,low_adc->GetName());
printf("charBuff:%s\n",charBuff);
noMaskTracks->Draw("length:adc>>noMaskLvADC_low_adc",charBuff);
TH1D* noMaskADC_low_adc = (TH1D*)noMaskLvADC_low_adc->ProjectionX("noMaskADC_low_adc");

TH2D* noMaskLvADC_junkCut_adc = new TH2D("noMaskLvADC_junkCut_adc","noMaskLvADC_junkCut_adc",4000,0,40000,nLengthBins,0,10);
sprintf(charBuff,"%s && %s",azCut,junkCut_adc->GetName());
printf("charBuff:%s\n",charBuff);
noMaskTracks->Draw("length:adc>>noMaskLvADC_junkCut_adc",charBuff);
TH1D* noMaskADC_junkCut_adc = (TH1D*)noMaskLvADC_junkCut_adc->ProjectionX("noMaskADC_junkCut_adc");

noMaskLvADC_low_adc->Write();
noMaskLvADC_alphaCut_zCut_adc->Write();
noMaskLvADC_fragCut_adc->Write();
// noMaskLvADC_junkCut_adc->Write();

noMaskADC_alphaCut_zCut_adc->Write();
noMaskADC_fragCut_adc->Write();
noMaskADC_low_adc->Write();
// noMaskADC_junkCut_adc->Write();
}

// TH2D* noMaskLvADC_highEn_fragCut_adc = new TH2D("noMaskLvADC_highEn_fragCut_adc","noMaskLvADC_highEn_fragCut_adc",4000,0,40000,nLengthBins,0,10);
// sprintf(charBuff,"%s && %s",azCut,highEn_fragCut_adc->GetName());
// printf("charBuff:%s\n",charBuff);
// noMaskTracks->Draw("length:adc>>noMaskLvADC_highEn_fragCut_adc",charBuff);
// TH1D* noMaskADC_highEn_fragCut_adc = (TH1D*)noMaskLvADC_highEn_fragCut_adc->ProjectionX("noMaskADC_highEn_fragCut_adc");
//
// TH2D* noMaskLvADC_alphaCut_short_low_adc = new TH2D("noMaskLvADC_alphaCut_short_low_adc","noMaskLvADC_alphaCut_short_low_adc",4000,0,40000,nLengthBins,0,10);
// sprintf(charBuff,"%s && %s",azCut,alphaCut_short_low_adc->GetName());
// printf("charBuff:%s\n",charBuff);
// noMaskTracks->Draw("length:adc>>noMaskLvADC_alphaCut_short_low_adc",charBuff);
// TH1D* noMaskADC_alphaCut_short_low_adc = (TH1D*)noMaskLvADC_alphaCut_short_low_adc->ProjectionX("noMaskADC_alphaCut_short_low_adc");
//
// TH2D* noMaskLvADC_alphaCut_short_higher_adc = new TH2D("noMaskLvADC_alphaCut_short_higher_adc","noMaskLvADC_alphaCut_short_higher_adc",4000,0,40000,nLengthBins,0,10);
// sprintf(charBuff,"%s && %s",azCut,alphaCut_short_higher_adc->GetName());
// printf("charBuff:%s\n",charBuff);
// noMaskTracks->Draw("length:adc>>noMaskLvADC_alphaCut_short_higher_adc",charBuff);
// TH1D* noMaskADC_alphaCut_short_higher_adc = (TH1D*)noMaskLvADC_alphaCut_short_higher_adc->ProjectionX("noMaskADC_alphaCut_short_higher_adc");
//
// TH2D* noMaskLvADC_long_high_adc = new TH2D("noMaskLvADC_long_high_adc","noMaskLvADC_long_high_adc",4000,0,40000,nLengthBins,0,10);
// sprintf(charBuff,"%s && %s",azCut,long_high_adc->GetName());
// printf("charBuff:%s\n",charBuff);
// noMaskTracks->Draw("length:adc>>noMaskLvADC_long_high_adc",charBuff);
// TH1D* noMaskADC_long_high_adc = (TH1D*)noMaskLvADC_long_high_adc->ProjectionX("noMaskADC_long_high_adc");

// targetHolder_lvse->SaveAs("targetHolder_lvse.C");
// alphaCut_zCut_energy->SaveAs("alphaCut_zCut_energy.C");
// alphaCut_long_low_energy->SaveAs("alphaCut_long_low_energy.C");
// alphaCut_long_energy->SaveAs("alphaCut_long_energy.C");
// long_high_energy->SaveAs("long_high_energy.C");
// alphaCut_short_higher_energy->SaveAs("alphaCut_short_higher_energy.C");
// low_energy->SaveAs("low_energy.C");
// fragCut_energy->SaveAs("fragCut_energy.C");
// alphaCut_short_low_energy->SaveAs("alphaCut_short_low_energy.C");
// alphaCut_zCut_adc->SaveAs("alphaCut_zCut_adc.C");
// alphaCut_long_low_adc->SaveAs("alphaCut_long_low_adc.C");
// alphaCut_long_adc->SaveAs("alphaCut_long_adc.C");
// long_high_adc->SaveAs("long_high_adc.C");
// alphaCut_short_higher_adc->SaveAs("alphaCut_short_higher_adc.C");
// low_adc->SaveAs("low_adc.C");
// fragCut_adc->SaveAs("fragCut_adc.C");
// alphaCut_short_low_adc->SaveAs("alphaCut_short_low_adc.C");
// return;

// ///////////////////  plotting zCut scans  ////////////////////
// printf("\n\n////	alphaCut_short_low_energy->SetPoint(0,0,2);
/////////////// plotting zCut scans //////////////////\n\n");
// TLatex text;
// text.SetTextAlign(13);
// text.SetTextSize(0.025);
// text.SetTextFont(42);
// c1->SetLogx(1);
// TH2D* noMaskLvEn_not_frags = (TH2D*)noMaskLvEn->Clone("noMaskLvEn_not_frags");
// for (int z = 0; z <= 110; z+=2){
// 	for (int energyBin = 0; energyBin < 4000; ++energyBin){
// 		for (int lengthBin = 0; lengthBin < nLengthBins; ++lengthBin){
// 			noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,0);
// 			noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,0);
// 			int counts=noMaskLvEn_not_frags->GetBinContent(energyBin,lengthBin);
// 			if (counts < z){noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,counts);}
// 			else{noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,counts);}
// 		}
// 	}
// 	c1->cd();
// 	c1->SetLogx(0);
// 	noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.4,10);
// 	sprintf(charBuff,"noMaskLvEn_below_zCut%03d",z);
// 	noMaskLvEn_below_zCut->SetTitle(charBuff);
// 	noMaskLvEn_below_zCut->SetStats(kFALSE);
// 	noMaskLvEn_below_zCut->Draw("colz");
// 	sprintf(charBuff,"number_above_zCut%d: %d",z,(int)noMaskLvEn_above_zCut->Integral());
// 	text.DrawTextNDC(0.15,0.89,charBuff);
// 	sprintf(charBuff,"number_below_zCut%d: %d",z,(int)noMaskLvEn_below_zCut->Integral());
// 	text.DrawTextNDC(0.15,0.86,charBuff);
// 	sprintf(charBuff,"%% #alpha tracks: %1.3f",noMaskLvEn_below_zCut->Integral()/(double)noMaskLvEn_not_frags->Integral()*100);
// 	text.DrawTextNDC(0.15,0.81,charBuff);
// 	TLegend *zCutLegend = new TLegend(0.35,0.15,0.55,0.12);
// 	zCutLegend->SetTextFont(42);
// 	zCutLegend->SetTextSize(0.03);
// 	zCutLegend->SetFillColorAlpha(kWhite,04);
// 	zCutLegend->AddEntry(alphaCut_zCut_energy,">99%% alphas inside");
// 	zCutLegend->Draw();
// 	TPad *inset_pad0 = new TPad ("inset_pad0", "inset_pad0",0.55,0.55,0.9,0.9);
// 	inset_pad0->SetPhi(35);
// 	inset_pad0->SetTheta(70);
// 	inset_pad0->Draw();
// 	inset_pad0->cd();
// 	inset_pad0->SetLogz(1);
// 	inset_pad0->SetLogx(0);
// 	noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.4,10);
// 	noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,6);
// 	sprintf(charBuff,"noMaskLvEn_above_zCut%d",z);
// 	noMaskLvEn_above_zCut->SetTitle(charBuff);
// 	sprintf(charBuff,"noMaskLvEn_alphas_zCut%03d.png",z);
// 	noMaskLvEn_above_zCut->Draw("surf2");
// 	c1->cd();
// 	delete inset_pad0;
// }
//
// c1->SetLogx(1);
// for (int z = 0; z <= 150; z+=2){
// 	zCut=z;
// 	for (int energyBin = 0; energyBin < 4000; ++energyBin){
// 		for (int lengthBin = 0; lengthBin < nLengthBins; ++lengthBin){
// 			noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,0);
// 			noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,0);
// 			int counts=noMaskLvEn->GetBinContent(energyBin,lengthBin);
// 			if (counts<zCut){noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,counts);}
// 			else{noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,counts);}
// 		}
// 	}
// 	c1->cd();
// 	noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.4,140);
// 	sprintf(charBuff,"noMaskLvEn_below_zCut%d",zCut);
// 	noMaskLvEn_below_zCut->SetTitle(charBuff);
// 	noMaskLvEn_below_zCut->SetStats(kFALSE);
// 	noMaskLvEn_below_zCut->Draw("colz");
// 	sprintf(charBuff,"number_above_zCut%d: %d",zCut,(int)noMaskLvEn_above_zCut->Integral());
// 	text.DrawTextNDC(0.15,0.89,charBuff);
// 	sprintf(charBuff,"number_below_zCut%d: %d",zCut,(int)noMaskLvEn_below_zCut->Integral());
// 	text.DrawTextNDC(0.15,0.86,charBuff);
// 	sprintf(charBuff,"%% #all tracks: %1.3f",noMaskLvEn_below_zCut->Integral()/(double)noMaskLvEn->Integral()*100);
// 	text.DrawTextNDC(0.15,0.81,charBuff);
// 	TPad *inset_pad0 = new TPad ("inset_pad0", "inset_pad0",0.55,0.55,0.9,0.9);
// 	inset_pad0->Draw();
// 	inset_pad0->cd();
// 	inset_pad0->SetLogz(1);
// 	inset_pad0->SetLogx(1);
// 	noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.4,140);
// 	noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,6);
// 	sprintf(charBuff,"noMaskLvEn_above_zCut%d",zCut);
// 	noMaskLvEn_above_zCut->SetTitle(charBuff);
// 	sprintf(charBuff,"noMaskLvEn_all_zCut%03d.png",zCut);
// 	noMaskLvEn_above_zCut->Draw("surf2");
// 	c1->cd();
// 	delete inset_pad0;
// }


noMaskLvEn->Write();
noMaskLvADC->Write();
// noMaskLvEn_below_zCut->Write();
// noMaskLvEn_above_zCut->Write();
//
// targetHolder_lvse->Write();
// yProj->Write();
// f_alphaCut_long_low_energy->Write();
//
alphaCut_long_low_energy->Write();
alphaCut_long_energy->Write();
long_high_energy->Write();
low_energy->Write();
alphaCut_short_low_energy->Write();
junkCut_energy->Write();
alphaCut_short_higher_energy->Write();
fragCut_energy->Write();
highEn_fragCut_energy->Write();
alphaCut_zCut_energy->Write();

alphaCut_long_low_adc->Write();
alphaCut_long_adc->Write();
long_high_adc->Write();
low_adc->Write();
alphaCut_short_low_adc->Write();
junkCut_adc->Write();
alphaCut_short_higher_adc->Write();
fragCut_adc->Write();
highEn_fragCut_adc->Write();
alphaCut_zCut_adc->Write();

outfile->Close();
// return;

}
