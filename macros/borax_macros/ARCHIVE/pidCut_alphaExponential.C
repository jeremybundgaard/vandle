#include <climits>
#include <vector>
#include "TROOT.h"
#include "TRint.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TGraph.h"
#include "TPad.h"
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
#include <ostream>
#include <sstream>

void pidCut_alphaExponential(int preamp,const char* treeFile,const char* cuts="&&1") {

	int nStdDev=9;
	int minStdDev=3;
	double MeV_per_ADC=168.6/4000;
	double ADC_per_MeV=40000/168.6;
	char charBuff[123];
	int xBuffer = 5;
	double tcutStep=0.1; /*MeV*/

	char polarCuts[567]="&&((-0.15>direction.CosTheta()&&direction.CosTheta()>-0.85)||(0.15<direction.CosTheta()&&direction.CosTheta()<0.85))";
	// char polarCuts[567]="&&(1.7453>direction.Theta()||1.396>direction.Theta())";
	// char polarCuts[567]="&&1";
	// char digitCut[567]="&&nDigits>60";
	char digitCut[567]="&&1";
	char treeCut[567];
	if(preamp==1){
		sprintf(treeCut,
			"run>100001856&&run!=100001870 %s %s %s",polarCuts,digitCut,cuts);
	}
	if(preamp==2){
		sprintf(treeCut,
			"!(-1<direction.Phi()&&direction.Phi()<0) %s %s %s",polarCuts,digitCut,cuts);
	}

	TFile* noMaskFile = new TFile(treeFile);
	TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");
	TH2D * noMaskLvEn = new TH2D("noMaskLvEn","noMaskLvEn",4000,0,168.6,1000,0,10);
	TH2D * noMaskLvEn_zCut_alphas = new TH2D("noMaskLvEn_zCut_alphas","noMaskLvEn_zCut_alphas",4000,0,168.6,1000,0,10);
	noMaskLvEn->GetXaxis()->SetRangeUser(0,10);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
	noMaskLvEn->GetXaxis()->CenterTitle();
	noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
	noMaskLvEn->GetYaxis()->CenterTitle();

	TCanvas *c1 = new TCanvas("c1","c1",1000,700);
	char c1PlotName[234];
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1);
	gStyle->SetPalette(52);
	gStyle->SetNumberContours(99);
	gStyle->SetLegendBorderSize(0);

	noMaskTracks->Draw("length:adc*168.6/40000>>noMaskLvEn",treeCut,"colz");

	sprintf(charBuff,"$cft/pid/pidCut_alphaExponential%d.root",preamp);
	TFile* outfile = new TFile(charBuff,"recreate");
	noMaskLvEn->Write();

	int zCut=0;
	if (preamp==1) zCut=60;
	if (preamp==2) zCut=40;

	sprintf(charBuff,"noMaskLvEn_above_zCut%04d",zCut);
	TH2D * noMaskLvEn_above_zCut = new TH2D(charBuff,charBuff,4000,0,168.6,1000,0,10);
	noMaskLvEn_above_zCut->GetXaxis()->SetTitle("Track Energy [MeV]");
	noMaskLvEn_above_zCut->GetXaxis()->CenterTitle();
	noMaskLvEn_above_zCut->GetYaxis()->SetTitle("Track Length [cm]");
	noMaskLvEn_above_zCut->GetYaxis()->CenterTitle();
	sprintf(charBuff,"noMaskLvEn_below_zCut%04d",zCut);
	TH2D * noMaskLvEn_below_zCut = new TH2D(charBuff,charBuff,4000,0,168.6,1000,0,10);
	noMaskLvEn_below_zCut->GetXaxis()->SetTitle("Track Energy [MeV]");
	noMaskLvEn_below_zCut->GetXaxis()->CenterTitle();
	noMaskLvEn_below_zCut->GetYaxis()->SetTitle("Track Length [cm]");
	noMaskLvEn_below_zCut->GetYaxis()->CenterTitle();

	for (int energyBin = 0; energyBin < 4000; ++energyBin){
		for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){

			noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,0);
			noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,0);
			int counts=noMaskLvEn->GetBinContent(energyBin,lengthBin);
			if (counts<=zCut){noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,counts);}
			else{noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,counts);}
		}
	}
	noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0,7);
	noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,5);
	c1->SetLogz(1);
	noMaskLvEn_above_zCut->SetStats(kFALSE);
	noMaskLvEn_above_zCut->Draw("colz");
	sprintf(charBuff,"noMaskLvEn_above_zCut%03d.png",zCut);
	c1->SaveAs(charBuff);
	noMaskLvEn_below_zCut->Write();
	noMaskLvEn_above_zCut->Write();

	/////////////////////////////////////////////////////////////////////////////////
	///////////////////////// creating alpha zCut TCutG /////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// creating alpha zCut TCutG ///////////////////\n\n");
	ofstream alphaPrimitive;
	noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0,15);
	noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,5);
	TH1D* yProj = noMaskLvEn_above_zCut->ProjectionY("yProj",0,1000);
	yProj->Write();
	yProj->Draw();
	c1->SetLogy(1);
	c1->SaveAs("yProj.png");
	c1->SetLogy(0);
	sprintf(charBuff,"noMaskLvEn_above_zCut%03d_energy",zCut);
	TCutG *alphaCut_zCut_energy = new TCutG(charBuff,10);
	alphaCut_zCut_energy->SetVarX("adc");
	alphaCut_zCut_energy->SetVarY("length");
	alphaCut_zCut_energy->SetLineColor(kGreen);
	alphaCut_zCut_energy->SetLineWidth(1);
	alphaCut_zCut_energy->SetFillColorAlpha(kGreen,0.15);
	sprintf(charBuff,"noMaskLvEn_above_zCut%03d_adc",zCut);
	TCutG *alphaCut_zCut_adc = new TCutG(charBuff,10);
	alphaCut_zCut_adc->SetVarX("adc");
	alphaCut_zCut_adc->SetVarY("length");
	alphaCut_zCut_adc->SetLineColor(kGreen);
	alphaCut_zCut_adc->SetLineWidth(1);
	alphaCut_zCut_adc->SetFillColorAlpha(kGreen,0.15);
	int pointNum=0;
	for (int lengthBin = yProj->FindFirstBinAbove(0); lengthBin <= yProj->FindLastBinAbove(0); ++lengthBin ){
		TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px", lengthBin, lengthBin);
		double length=lengthBin*0.01;
		double energy=(px->FindFirstBinAbove(0)-1)*MeV_per_ADC;
		// printf("alphaCut_zCut_energy->SetPoint(%d,%1.3f,%1.3f)\n",pointNum,energy,length);
		alphaCut_zCut_energy->SetPoint(pointNum,energy,length);
		alphaCut_zCut_adc->SetPoint(pointNum,energy*ADC_per_MeV,length);
		pointNum++;
	}
	for (int lengthBin = yProj->FindLastBinAbove(0); lengthBin >= yProj->FindFirstBinAbove(0); --lengthBin ){
		TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px", lengthBin, lengthBin);
		double length=lengthBin*0.01;
		double energy=px->FindLastBinAbove(0)*MeV_per_ADC;
		// printf("alphaCut_zCut_energy->SetPoint(%d,%1.3f,%1.3f)\n",pointNum,energy,length);
		alphaCut_zCut_energy->SetPoint(pointNum,energy,length);
		alphaCut_zCut_adc->SetPoint(pointNum,energy*ADC_per_MeV,length);
		pointNum++;
	}
	TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px",yProj->FindFirstBinAbove(0),yProj->FindFirstBinAbove(0));
	alphaCut_zCut_energy->SetPoint(pointNum,(px->FindFirstBinAbove(0)-1)*MeV_per_ADC,yProj->FindFirstBinAbove(0)*0.01);
	alphaCut_zCut_energy->Write();
	alphaCut_zCut_adc->SetPoint(pointNum,(px->FindFirstBinAbove(0)-1)*MeV_per_ADC*ADC_per_MeV,yProj->FindFirstBinAbove(0)*0.01);pointNum++;
	alphaCut_zCut_adc->Write();
	sprintf(charBuff,"alphaCut_zCut%d_preamp%03d.C",zCut,preamp);
	alphaPrimitive.open(charBuff);
	alphaCut_zCut_energy->SavePrimitive(alphaPrimitive);

	/////////////////////////////////////////////////////////////////////////////////
	///////////////// creating alpha TCutG from stdDev of zCut //////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	printf("\n\n////////////// creating alpha TCutG from stdDev of zCut ///////////////////\n\n");
	int stdDev_zCut=0;
	TCutG *alphaCut_zCut_stdDev_energy[nStdDev];
	TCutG *alphaCut_zCut_stdDev_adc[nStdDev];
	for (int i = minStdDev; i < nStdDev; ++i){
		stdDev_zCut=i;
		sprintf(charBuff,"noMaskLvEn_above_zCut%03d_stddev%d_energy",zCut,stdDev_zCut);
		TCutG *alphaCut_zCut_stdDev_energy_tmp = new TCutG(charBuff,10);
		alphaCut_zCut_stdDev_energy_tmp->SetVarX("adc");
		alphaCut_zCut_stdDev_energy_tmp->SetVarY("length");
		alphaCut_zCut_stdDev_energy_tmp->SetLineColor(kGreen);
		alphaCut_zCut_stdDev_energy_tmp->SetLineWidth(1);
		alphaCut_zCut_stdDev_energy_tmp->SetFillColorAlpha(kGreen,0.15);

		sprintf(charBuff,"noMaskLvEn_above_zCut%03d_stddev%d_adc",zCut,stdDev_zCut);
		TCutG *alphaCut_zCut_stdDev_adc_tmp = new TCutG(charBuff,10);
		alphaCut_zCut_stdDev_adc_tmp->SetVarX("adc");
		alphaCut_zCut_stdDev_adc_tmp->SetVarY("length");
		alphaCut_zCut_stdDev_adc_tmp->SetLineColor(kGreen);
		alphaCut_zCut_stdDev_adc_tmp->SetLineWidth(1);
		alphaCut_zCut_stdDev_adc_tmp->SetFillColorAlpha(kGreen,0.15);
		sprintf(charBuff,"noMaskLvEn_above_zCut%03d_adc",zCut);

		pointNum=0;
		for (int lengthBin = yProj->FindFirstBinAbove(0); lengthBin <= yProj->FindLastBinAbove(0); ++lengthBin ){
			TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px", lengthBin, lengthBin);
			double length=lengthBin*0.01;
			double minus_stdDev=(px->GetMean()-stdDev_zCut*px->GetStdDev());
			printf("alphaCut_zCut_stdDev_energy_tmp->SetPoint(%d,%1.3f,%1.3f)\n",pointNum,minus_stdDev,length);
			alphaCut_zCut_stdDev_energy_tmp->SetPoint(pointNum,minus_stdDev,length);
			alphaCut_zCut_stdDev_adc_tmp->SetPoint(pointNum,minus_stdDev*ADC_per_MeV,length);
			pointNum++;
		}
		for (int lengthBin = yProj->FindLastBinAbove(0); lengthBin >= yProj->FindFirstBinAbove(0); --lengthBin ){
			TH1D *px = noMaskLvEn_above_zCut->ProjectionX("px", lengthBin, lengthBin);
			double length=lengthBin*0.01;
			double plus_stdDev=(px->GetMean()+stdDev_zCut*px->GetStdDev());
			printf("alphaCut_zCut_stdDev_energy_tmp->SetPoint(%d,%1.3f,%1.3f)\n",pointNum,plus_stdDev,length);
			alphaCut_zCut_stdDev_energy_tmp->SetPoint(pointNum,plus_stdDev,length);
			alphaCut_zCut_stdDev_adc_tmp->SetPoint(pointNum,plus_stdDev*ADC_per_MeV,length);
			pointNum++;
		}
		px = noMaskLvEn_above_zCut->ProjectionX("px",yProj->FindFirstBinAbove(0),yProj->FindFirstBinAbove(0));
		alphaCut_zCut_stdDev_energy_tmp->SetPoint(pointNum,(px->GetMean()-stdDev_zCut*px->GetStdDev()),yProj->FindFirstBinAbove(0)*0.01);
		alphaCut_zCut_stdDev_energy_tmp->Write();
		alphaCut_zCut_stdDev_adc_tmp->SetPoint(pointNum,(px->GetMean()-stdDev_zCut*px->GetStdDev())*ADC_per_MeV,yProj->FindFirstBinAbove(0)*0.01);
		pointNum++;
		alphaCut_zCut_stdDev_adc_tmp->Write();
		delete px;
		sprintf(charBuff,"alphaCut_zCut%d_stdDev%d_preamp%d.C",zCut,stdDev_zCut,preamp);
		alphaPrimitive.open(charBuff);
		alphaCut_zCut_stdDev_energy_tmp->SavePrimitive(alphaPrimitive);

		alphaCut_zCut_stdDev_adc[i]=alphaCut_zCut_stdDev_adc_tmp;
		alphaCut_zCut_stdDev_energy[i]=alphaCut_zCut_stdDev_energy_tmp;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// defining alpha cut points along stdDev //////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// define alpha cut points ///////////////////\n\n");
	int lengthBin_start=70;
	int lengthBin_stop=430;
	int lengthBinStep=5;
	int lengthFitBins=(int)(lengthBin_stop-lengthBin_start)/lengthBinStep;
	Double_t energyCutPoint_alphas_plusStdDev[nStdDev][lengthFitBins];
	Double_t lengthCutPoint_alphas_plusStdDev[lengthFitBins];
	Double_t energyCutPoint_alphas_minusStdDev[nStdDev][lengthFitBins];
	Double_t lengthCutPoint_alphas_minusStdDev[lengthFitBins];
	int arrayPos=0;
	for (int lengthBin = lengthBin_start; lengthBin < lengthBin_stop; lengthBin+=lengthBinStep){
		sprintf(charBuff,"alphaBand_energyProj_lengthBin%03d",lengthBin);
		TH1D *px = noMaskLvEn_above_zCut->ProjectionX(charBuff, lengthBin, lengthBin);
		TF1 *fGaus = new TF1("fGaus", "gaus", px->FindFirstBinAbove(0), px->FindLastBinAbove(0)+1);
		px->SetTitle(charBuff);
		px->GetXaxis()->SetRangeUser(0,10);
		px->GetYaxis()->SetRangeUser(1,100000);
		// TH1D* px_avg = (TH1D*)px->Clone("px_avg");
		// px_avg->SetLineColor(kRed);
		// int startBin = px->FindFirstBinAbove(10)-xBuffer;
		// int stopBin = px->FindLastBinAbove(10)+xBuffer;
		// for (int i = 0; i < px->GetNbinsX(); ++i){ 
		// 	if( (startBin<i&&i<stopBin) ) continue;
		// 	px_avg->SetBinContent(i,0);
		// }
		gStyle->SetOptStat(1111);
		// px_avg->Fit(fGaus);
		// px_avg->Draw();
		px->Fit(fGaus);
		px->Draw();
		lengthCutPoint_alphas_plusStdDev[arrayPos] = lengthBin/(double)100;
		lengthCutPoint_alphas_minusStdDev[arrayPos] = lengthBin/(double)100;
		for (int i = 0; i < nStdDev; ++i)
		{
			// energyCutPoint_alphas_plusStdDev[i][arrayPos] = px_avg->GetMean()+i*px_avg->GetStdDev();
			// energyCutPoint_alphas_minusStdDev[i][arrayPos] = px_avg->GetMean()-i*px_avg->GetStdDev();
			energyCutPoint_alphas_plusStdDev[i][arrayPos] = fGaus->GetParameter(1)+i*fGaus->GetParameter(2);
			energyCutPoint_alphas_minusStdDev[i][arrayPos] = fGaus->GetParameter(1)-i*fGaus->GetParameter(2);
			TLine *Line_plusStdDev = new TLine(
				energyCutPoint_alphas_plusStdDev[i][arrayPos],0,
				energyCutPoint_alphas_plusStdDev[i][arrayPos],10-i);
			Line_plusStdDev->SetLineColor(i+1);
			Line_plusStdDev->Draw("same");
			TLine *Line_minusStdDev = new TLine(
				energyCutPoint_alphas_minusStdDev[i][arrayPos],0,
				energyCutPoint_alphas_minusStdDev[i][arrayPos],10-i);
			Line_minusStdDev->SetLineColor(i+1);
			Line_minusStdDev->Draw("same");
		}
		sprintf(charBuff,"AlphaBand_proj.lengthBin%03d.png",lengthBin);
		c1->SetLogy(1);
		c1->SaveAs(charBuff);
		c1->SetLogy(0);
		arrayPos++;		
	}

	/////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  fitting alpha cut points  /////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n///////////////////////  fit alpha cut points  ///////////////////\n\n");
	TGraph *alphaGraph_plusStdDev[nStdDev];
	TF1 *alphaFit_plusStdDev[nStdDev];
	TGraph *alphaGraph_minusStdDev[nStdDev];
	TF1 *alphaFit_minusStdDev[nStdDev];
	for (int i = 0; i < nStdDev; ++i)
	{
		alphaGraph_plusStdDev[i] = new TGraph((sizeof(energyCutPoint_alphas_plusStdDev[0])/sizeof(Double_t)),energyCutPoint_alphas_plusStdDev[i],lengthCutPoint_alphas_plusStdDev);
		sprintf(charBuff,"alphaFit_plus_%dstdDev",i);
		alphaFit_plusStdDev[i]=new TF1(charBuff,"[3]+[2]*TMath::Exp([1]*x+[0])",0,10);
		alphaFit_plusStdDev[i]->SetLineColor(i+1);
		alphaFit_plusStdDev[i]->SetLineWidth(1);
		alphaGraph_plusStdDev[i]->Fit(alphaFit_plusStdDev[i]);
		alphaGraph_minusStdDev[i]=new TGraph((sizeof(energyCutPoint_alphas_minusStdDev[0])/sizeof(Double_t)),energyCutPoint_alphas_minusStdDev[i],lengthCutPoint_alphas_minusStdDev);
		sprintf(charBuff,"alphaFit_minus_%dstdDev",i);
		alphaFit_minusStdDev[i]=new TF1(charBuff,"[3]+[2]*TMath::Exp([1]*x+[0])",0,10);
		alphaFit_minusStdDev[i]->SetLineColor(i+1);
		alphaFit_minusStdDev[i]->SetLineWidth(1);
		alphaGraph_minusStdDev[i]->Fit(alphaFit_minusStdDev[i]);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  drawing alpha bands proj std points //////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// drawing alpha bands proj std points ///////////////////\n\n");
	noMaskLvEn->SetTitle("[3]+TMath::Exp([1]*x+[0])");
	noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0,10);
	noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,6);
	noMaskLvEn_above_zCut->SetTitle("[3]+TMath::Exp([1]*x+[0])");
	noMaskLvEn_above_zCut->SetStats(kFALSE);
	arrayPos=0;
	for (int lengthBin = lengthBin_start; lengthBin < lengthBin_stop; lengthBin+=lengthBinStep){
		noMaskLvEn_above_zCut->Draw("colz");
		c1->Update();
		for (int i = 0; i < nStdDev; ++i){
			alphaFit_plusStdDev[i]->Draw("same");
			alphaFit_minusStdDev[i]->Draw("same");
		}
		c1->SetLogz(1);
		for (int i = 0; i < nStdDev; ++i)
		{
			double length=lengthBin/(double)100;
			double energy_plus=energyCutPoint_alphas_plusStdDev[i][arrayPos];
			double energy_minus=energyCutPoint_alphas_minusStdDev[i][arrayPos];
			TLine *Line_plusStdDev = new TLine(
				energy_plus,length-0.02,
				energy_plus,length+0.02);
			Line_plusStdDev->SetLineColor(i+1);
			Line_plusStdDev->SetLineWidth(1);
			Line_plusStdDev->Draw();
			TLine *Line_minusStdDev = new TLine(
				energy_minus,length+0.02,
				energy_minus,length-0.02);
			Line_minusStdDev->SetLineColor(i+1);
			Line_minusStdDev->SetLineWidth(1);
			Line_minusStdDev->Draw();
		}
		sprintf(charBuff,"noMaskLvEn_cutPoints.y%03d.png",lengthBin);
		c1->SaveAs(charBuff);
		arrayPos++;		
	}
	noMaskLvEn->SetTitle("noMaskLvEn");

	/////////////////////////////////////////////////////////////////////////////////
	///////////////////// creating alpha TCutG from exponentials ////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//////////////// creating alpha TCutG from exponential - energy /////////////////
	/////////////////////////////////////////////////////////////////////////////////
	noMaskLvEn->GetXaxis()->SetRangeUser(0,16);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn->SetStats(kFALSE);
	noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0,16);
	noMaskLvEn_below_zCut->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn_below_zCut->SetStats(kFALSE);
	double alpha_maxCutLength=(yProj->FindLastBinAbove(0)+5)*0.01;
	TCutG *alphaCut_energy[nStdDev];
	TCutG *alphaCut_short_low_energy[nStdDev];
	TF1* f_alphaCut_long_low_energy = new TF1("f_alphaCut_long_low_energy","[1]*x+[0]",0,10);
	f_alphaCut_long_low_energy->SetParameter(0,3);
	f_alphaCut_long_low_energy->SetParameter(1,0.7);
	f_alphaCut_long_low_energy->Write();
  	TF1* frag_energy_lengthline = new TF1("frag_energy_lengthline","[1]*x+[0]",0,100);
	frag_energy_lengthline->SetLineWidth(1);
	frag_energy_lengthline->SetParameter(0,1.1);
	frag_energy_lengthline->SetParameter(1,0.04);
	TCutG *fragCut_energy[nStdDev];
	TCutG *alphaCut_short_higher_energy[nStdDev];
	TCutG *alphaCut_long_low_energy	= new TCutG("alphaCut_long_low_energy",3);
	alphaCut_long_low_energy->SetVarX("adc");
	alphaCut_long_low_energy->SetVarY("length");
	alphaCut_long_low_energy->SetLineColor(kYellow);
	alphaCut_long_low_energy->SetLineWidth(1);
	alphaCut_long_low_energy->SetFillColorAlpha(kYellow,0.15);
	alphaCut_long_low_energy->SetPoint(0,0,3);
	alphaCut_long_low_energy->SetPoint(1,0,10);
	alphaCut_long_low_energy->SetPoint(2,10,10);
	alphaCut_long_low_energy->SetPoint(3,f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);
	alphaCut_long_low_energy->SetPoint(4,0,3);
	ofstream alphaCut_long_low_energy_primitive;
	sprintf(charBuff,"alphaCut_long_low_energy_preamp%d.C",preamp);
	alphaCut_long_low_energy_primitive.open(charBuff);
	alphaCut_long_low_energy->SavePrimitive(alphaCut_long_low_energy_primitive);
	alphaCut_long_low_energy->Write();
	TCutG *alphaCut_long_energy	= new TCutG("alphaCut_long_energy",3);
	alphaCut_long_energy->SetVarX("adc");
	alphaCut_long_energy->SetVarY("length");
	alphaCut_long_energy->SetLineColor(kCyan);
	alphaCut_long_energy->SetLineWidth(1);
	alphaCut_long_energy->SetFillColorAlpha(kCyan,0.15);
	alphaCut_long_energy->SetPoint(0,10,10);
	alphaCut_long_energy->SetPoint(1,f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);
	alphaCut_long_energy->SetPoint(2,15,alpha_maxCutLength);
	alphaCut_long_energy->SetPoint(3,15,10);
	alphaCut_long_energy->SetPoint(4,10,10);
	alphaCut_long_energy->Write();
	TCutG *long_high_energy	= new TCutG("long_high_energy",3);
	long_high_energy->SetVarX("adc");
	long_high_energy->SetVarY("length");
	long_high_energy->SetLineColor(kBlue);
	long_high_energy->SetLineWidth(1);
	long_high_energy->SetFillColorAlpha(kBlue,0.15);
	long_high_energy->SetPoint(0,15,frag_energy_lengthline->Eval(15));
	long_high_energy->SetPoint(1,30,frag_energy_lengthline->Eval(30));
	long_high_energy->SetPoint(2,71,3.15);
	long_high_energy->SetPoint(3,200,3.65);
	long_high_energy->SetPoint(4,200,10);
	long_high_energy->SetPoint(5,15,10);
	long_high_energy->SetPoint(6,15,frag_energy_lengthline->Eval(15));
	long_high_energy->Write();
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(charBuff,"alphaCut_energy_preamp%d_expoFit_%dstdDev",preamp,i);
		TCutG* alphaCut_energy_tmp= new TCutG(charBuff,4);
		alphaCut_energy_tmp->SetVarX("adc");
		alphaCut_energy_tmp->SetVarY("length");
		alphaCut_energy_tmp->SetLineColor(kGreen);
		alphaCut_energy_tmp->SetLineWidth(3);
		alphaCut_energy_tmp->SetFillColorAlpha(kGreen,0.15);
		int alpha_pointNum=0;
		alphaCut_energy_tmp->SetPoint(alpha_pointNum,0,0);alpha_pointNum++;
		alphaCut_energy_tmp->SetPoint(alpha_pointNum,alphaFit_plusStdDev[i]->GetX(0),0);alpha_pointNum++;
		sprintf(charBuff,"fragCut_energy_preamp%d_%dstdDev",preamp,i);
		TCutG *fragCut_energy_tmp = new TCutG(charBuff,4);
		fragCut_energy_tmp->SetVarX("adc");
		fragCut_energy_tmp->SetVarY("length");
		fragCut_energy_tmp->SetLineColor(kViolet);
		fragCut_energy_tmp->SetLineWidth(3);
		fragCut_energy_tmp->SetFillColorAlpha(kViolet,0.15);
		int frag_pointNum=0;
		fragCut_energy_tmp->SetPoint(frag_pointNum,200,0);frag_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,alphaFit_plusStdDev[i]->GetX(0),0);frag_pointNum++;
		sprintf(charBuff,"alphaCut_short_higher_energy_preamp%d_%dstdDev",preamp,i);
		TCutG *alphaCut_short_higher_energy_tmp = new TCutG(charBuff,4);
		alphaCut_short_higher_energy_tmp->SetVarX("adc");
		alphaCut_short_higher_energy_tmp->SetVarY("length");
		alphaCut_short_higher_energy_tmp->SetLineColor(kOrange+7);
		alphaCut_short_higher_energy_tmp->SetLineWidth(1);
		alphaCut_short_higher_energy_tmp->SetFillColorAlpha(kOrange+7,0.15);
		int alphaCut_short_higher_pointNum=0;
		alphaCut_short_higher_energy_tmp->SetPoint(alphaCut_short_higher_pointNum,alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength),alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_energy_tmp->SetPoint(alphaCut_short_higher_pointNum,15,alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_energy_tmp->SetPoint(alphaCut_short_higher_pointNum,15,frag_energy_lengthline->Eval(15));alphaCut_short_higher_pointNum++;
		//////////////  plus stdDev exponential fit loop ////////////////////		
		double energy=0;
		double fragEnergy=0;
		double alphaCut_short_higherEnergy=0;
		for (double en_loop = alphaFit_plusStdDev[i]->GetX(0); en_loop < alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength); en_loop+=tcutStep){
			energy=en_loop;
			if (energy <= 0 || alphaFit_plusStdDev[i]->Eval(energy) <= 0 ) continue;
			if (energy >= alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength) || alphaFit_plusStdDev[i]->Eval(energy) >= alpha_maxCutLength ) continue;
			if ( alphaFit_plusStdDev[i]->Eval(energy) < frag_energy_lengthline->Eval(energy) )
			{
				fragCut_energy_tmp->SetPoint(frag_pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));frag_pointNum++;
				fragEnergy=energy;
			}
			if ( alphaFit_plusStdDev[i]->Eval(energy) > frag_energy_lengthline->Eval(energy) )
			{
				alphaCut_short_higher_energy_tmp->SetPoint(alphaCut_short_higher_pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));alphaCut_short_higher_pointNum++;
			}
			alphaCut_energy_tmp->SetPoint(alpha_pointNum,en_loop,alphaFit_plusStdDev[i]->Eval(energy));alpha_pointNum++;
			printf("alphaCut_energy_tmp->SetPoint(%d,%1.3f,%1.3f)\n",frag_pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));
		}
		energy = alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength);
		alphaCut_energy_tmp->SetPoint(alpha_pointNum,energy,alpha_maxCutLength);alpha_pointNum++;
		energy = alphaFit_minusStdDev[i]->GetX(alpha_maxCutLength);
 		alphaCut_energy_tmp->SetPoint(alpha_pointNum,energy,alpha_maxCutLength);alpha_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,fragEnergy+tcutStep,alphaFit_plusStdDev[i]->Eval(fragEnergy+tcutStep));frag_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,15,frag_energy_lengthline->Eval(15));frag_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,30,frag_energy_lengthline->Eval(30));frag_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,71,3.15);frag_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,200,3.65);frag_pointNum++;
		fragCut_energy_tmp->SetPoint(frag_pointNum,200,0);frag_pointNum++;
		fragCut_energy[i] = fragCut_energy_tmp;
		sprintf(charBuff,"fragCut_energy_preamp%d_%dstdDev.C",preamp,i);
		ofstream fragPrimitive;
		fragPrimitive.open(charBuff);
		fragCut_energy[i]->SavePrimitive(fragPrimitive);
		fragCut_energy[i]->Write();
		alphaCut_short_higher_energy_tmp->SetPoint(alphaCut_short_higher_pointNum,alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength),alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_energy_tmp->SetPoint(alphaCut_short_higher_pointNum,15,alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_energy[i] = alphaCut_short_higher_energy_tmp;
		ofstream alphaCut_short_higher;
		sprintf(charBuff,"alphaCut_short_higher_energy_preamp%d_%dstdDev.C",preamp,i);
		alphaCut_short_higher.open(charBuff);
		alphaCut_short_higher_energy[i]->SavePrimitive(alphaCut_short_higher);
		alphaCut_short_higher_energy[i]->Write();
		sprintf(charBuff,"alphaCut_short_low_energy_preamp%d_expoFit_%dstdDev",preamp,i);
		TCutG* alphaCut_short_low_energy_tmp= new TCutG(charBuff,4);
		alphaCut_short_low_energy_tmp->SetVarX("adc");
		alphaCut_short_low_energy_tmp->SetVarY("length");
		alphaCut_short_low_energy_tmp->SetLineColor(kRed);
		alphaCut_short_low_energy_tmp->SetLineWidth(1);
		alphaCut_short_low_energy_tmp->SetFillColorAlpha(kRed,0.15);
		int short_low_pointNum=0;
		alphaCut_short_low_energy_tmp->SetPoint(short_low_pointNum,0,3);short_low_pointNum++;
		alphaCut_short_low_energy_tmp->SetPoint(short_low_pointNum,f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);short_low_pointNum++;
		alphaCut_short_low_energy_tmp->SetPoint(short_low_pointNum,energy,alpha_maxCutLength);short_low_pointNum++;
		//////////////  plus stdDev exponential fit loop ////////////////////
		for (double en_loop = alphaFit_minusStdDev[i]->GetX(alpha_maxCutLength); en_loop > 0; en_loop-=tcutStep){
			energy=en_loop;
			if (energy <= 0 || alphaFit_minusStdDev[i]->Eval(energy) <= 0 ) continue;
			if (energy >= alpha_maxCutLength || alphaFit_minusStdDev[i]->Eval(energy) >= alpha_maxCutLength ) continue;
			alphaCut_energy_tmp->SetPoint(alpha_pointNum,energy,alphaFit_minusStdDev[i]->Eval(energy));alpha_pointNum++;
			alphaCut_short_low_energy_tmp->SetPoint(short_low_pointNum,energy,alphaFit_minusStdDev[i]->Eval(energy));short_low_pointNum++;
		}
		alphaCut_energy_tmp->SetPoint(alpha_pointNum,0,alphaFit_minusStdDev[i]->Eval(0));alpha_pointNum++;
		alphaCut_energy_tmp->SetPoint(alpha_pointNum,0,0);
		alphaCut_energy[i] = alphaCut_energy_tmp;
		ofstream alphaCut_energy_primitive;
		sprintf(charBuff,"alphaCut_energy_preamp%d_expoFit_%dstdDev.C",preamp,i);
		alphaCut_energy_primitive.open(charBuff);
		alphaCut_energy[i]->SavePrimitive(alphaCut_energy_primitive);
		alphaCut_energy[i]->Write();
		alphaCut_short_low_energy_tmp->SetPoint(short_low_pointNum,0,alphaFit_minusStdDev[i]->Eval(0));short_low_pointNum++;
		alphaCut_short_low_energy_tmp->SetPoint(short_low_pointNum,0,3);short_low_pointNum++;
		alphaCut_short_low_energy[i] = alphaCut_short_low_energy_tmp;
		ofstream alphaCut_short_low_energy_primitive;
		sprintf(charBuff,"alphaCut_short_low_energy_preamp%d_expoFit_%dstdDev.C",preamp,i);
		alphaCut_short_low_energy_primitive.open(charBuff);
		alphaCut_short_low_energy[i]->SavePrimitive(alphaCut_short_low_energy_primitive);
		alphaCut_short_low_energy[i]->Write();
		noMaskLvEn->GetXaxis()->SetRangeUser(0,15);
		noMaskLvEn->GetYaxis()->SetRangeUser(0,15);
		noMaskLvEn->Draw("colz");
		/////////////  draw color cuts ////////////
		alphaCut_short_low_energy[i]->Draw("same f");
		alphaCut_short_low_energy[i]->Draw("same");
		alphaCut_short_higher_energy[i]->Draw("same f");
		alphaCut_short_higher_energy[i]->Draw("same");
		alphaCut_long_low_energy->Draw("same f");
		alphaCut_long_low_energy->Draw("same");
		alphaCut_long_energy->Draw("same f");
		alphaCut_long_energy->Draw("same");
		long_high_energy->Draw("same");
		long_high_energy->Draw("same f");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		c1->SetLogz(1);
		sprintf(charBuff,"alphaCut_stdDev%d.png",i);
		c1->SaveAs(charBuff);
		noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.3,20);
		noMaskLvEn_below_zCut->GetYaxis()->SetRangeUser(0,10);
		noMaskLvEn_below_zCut->Draw("colz");
		/////////////  draw color cuts ////////////
		alphaCut_short_low_energy[i]->Draw("same f");
		alphaCut_short_low_energy[i]->Draw("same");
		alphaCut_short_higher_energy[i]->Draw("same f");
		alphaCut_short_higher_energy[i]->Draw("same");
		alphaCut_long_low_energy->Draw("same f");
		alphaCut_long_low_energy->Draw("same");
		alphaCut_long_energy->Draw("same f");
		alphaCut_long_energy->Draw("same");
		long_high_energy->Draw("same");
		long_high_energy->Draw("same f");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		c1->SetLogz(0);
		sprintf(charBuff,"alphaCut_below_zCut_stdDev%d.png",i);
		c1->SaveAs(charBuff);
		noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.3,200);
		noMaskLvEn_below_zCut->GetYaxis()->SetRangeUser(0,10);
		noMaskLvEn_below_zCut->Draw("colz");
		/////////////  draw color cuts ////////////
		alphaCut_short_low_energy[i]->Draw("same f");
		alphaCut_short_low_energy[i]->Draw("same");
		alphaCut_short_higher_energy[i]->Draw("same f");
		alphaCut_short_higher_energy[i]->Draw("same");
		alphaCut_long_low_energy->Draw("same f");
		alphaCut_long_low_energy->Draw("same");
		alphaCut_long_energy->Draw("same f");
		alphaCut_long_energy->Draw("same");
		long_high_energy->Draw("same");
		long_high_energy->Draw("same f");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		c1->SetLogx(1);
		c1->SetLogz(0);
		sprintf(charBuff,"alphaCut_below_zCut_full_logxz_stdDev%d.png",i);
		c1->SaveAs(charBuff);
		c1->SetLogx(0);
		c1->SetLogz(0);
		sprintf(charBuff,"alphaCut_below_zCut_full_stdDev%d.png",i);
		c1->SaveAs(charBuff);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////// creating alpha TCutG from exponential - ADC ////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	noMaskLvEn->GetXaxis()->SetRangeUser(0,16);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn->SetStats(kFALSE);
	noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0,16);
	noMaskLvEn_below_zCut->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn_below_zCut->SetStats(kFALSE);
	TCutG *alphaCut_adc[nStdDev];
	TCutG *alphaCut_short_low_adc[nStdDev];
	TCutG *fragCut_adc[nStdDev];
	TCutG *alphaCut_short_higher_adc[nStdDev];
	TCutG *alphaCut_long_low_adc	= new TCutG("alphaCut_long_low_adc",3);
	alphaCut_long_low_adc->SetVarX("adc");
	alphaCut_long_low_adc->SetVarY("length");
	alphaCut_long_low_adc->SetLineColor(kYellow);
	alphaCut_long_low_adc->SetLineWidth(1);
	alphaCut_long_low_adc->SetFillColorAlpha(kYellow,0.15);
	alphaCut_long_low_adc->SetPoint(0,ADC_per_MeV*0,3);
	alphaCut_long_low_adc->SetPoint(1,ADC_per_MeV*0,10);
	alphaCut_long_low_adc->SetPoint(2,ADC_per_MeV*10,10);
	alphaCut_long_low_adc->SetPoint(3,ADC_per_MeV*f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);
	alphaCut_long_low_adc->SetPoint(4,ADC_per_MeV*0,3);
	ofstream alphaCut_long_low_adc_primitive;
	sprintf(charBuff,"alphaCut_long_low_adc_preamp%d.C",preamp);
	alphaCut_long_low_adc_primitive.open(charBuff);
	alphaCut_long_low_adc->SavePrimitive(alphaCut_long_low_adc_primitive);
	alphaCut_long_low_adc->Write();
	TCutG *alphaCut_long_adc	= new TCutG("alphaCut_long_adc",3);
	alphaCut_long_adc->SetVarX("adc");
	alphaCut_long_adc->SetVarY("length");
	alphaCut_long_adc->SetLineColor(kCyan);
	alphaCut_long_adc->SetLineWidth(1);
	alphaCut_long_adc->SetFillColorAlpha(kCyan,0.15);
	alphaCut_long_adc->SetPoint(0,ADC_per_MeV*10,10);
	alphaCut_long_adc->SetPoint(1,ADC_per_MeV*f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);
	alphaCut_long_adc->SetPoint(2,ADC_per_MeV*15,alpha_maxCutLength);
	alphaCut_long_adc->SetPoint(3,ADC_per_MeV*15,10);
	alphaCut_long_adc->SetPoint(4,ADC_per_MeV*10,10);
	alphaCut_long_adc->Write();
	TCutG *long_high_adc	= new TCutG("long_high_adc",3);
	long_high_adc->SetVarX("adc");
	long_high_adc->SetVarY("length");
	long_high_adc->SetLineColor(kBlue);
	long_high_adc->SetLineWidth(1);
	long_high_adc->SetFillColorAlpha(kBlue,0.15);
	long_high_adc->SetPoint(0,ADC_per_MeV*15,frag_energy_lengthline->Eval(15));
	long_high_adc->SetPoint(1,ADC_per_MeV*30,frag_energy_lengthline->Eval(30));
	long_high_adc->SetPoint(2,ADC_per_MeV*71,3.15);
	long_high_adc->SetPoint(3,ADC_per_MeV*200,3.65);
	long_high_adc->SetPoint(4,ADC_per_MeV*200,10);
	long_high_adc->SetPoint(5,ADC_per_MeV*15,10);
	long_high_adc->SetPoint(6,ADC_per_MeV*15,frag_energy_lengthline->Eval(15));
	long_high_adc->Write();
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(charBuff,"alphaCut_adc_preamp%d_expoFit_%dstdDev",preamp,i);
		TCutG* alphaCut_adc_tmp= new TCutG(charBuff,4);
		alphaCut_adc_tmp->SetVarX("adc");
		alphaCut_adc_tmp->SetVarY("length");
		alphaCut_adc_tmp->SetLineColor(kGreen);
		alphaCut_adc_tmp->SetLineWidth(3);
		alphaCut_adc_tmp->SetFillColorAlpha(kGreen,0.15);
		int alpha_pointNum=0;
		alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*0,0);alpha_pointNum++;
		alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(0),0);alpha_pointNum++;
		sprintf(charBuff,"fragCut_adc_preamp%d_%dstdDev",preamp,i);
		TCutG *fragCut_adc_tmp = new TCutG(charBuff,4);
		fragCut_adc_tmp->SetVarX("adc");
		fragCut_adc_tmp->SetVarY("length");
		fragCut_adc_tmp->SetLineColor(kViolet);
		fragCut_adc_tmp->SetLineWidth(3);
		fragCut_adc_tmp->SetFillColorAlpha(kViolet,0.15);
		int frag_pointNum=0;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*200,0);frag_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(0),0);frag_pointNum++;
		sprintf(charBuff,"alphaCut_short_higher_adc_preamp%d_%dstdDev",preamp,i);
		TCutG *alphaCut_short_higher_adc_tmp = new TCutG(charBuff,4);
		alphaCut_short_higher_adc_tmp->SetVarX("adc");
		alphaCut_short_higher_adc_tmp->SetVarY("length");
		alphaCut_short_higher_adc_tmp->SetLineColor(kOrange+7);
		alphaCut_short_higher_adc_tmp->SetLineWidth(1);
		alphaCut_short_higher_adc_tmp->SetFillColorAlpha(kOrange+7,0.15);
		int alphaCut_short_higher_pointNum=0;
		alphaCut_short_higher_adc_tmp->SetPoint(alphaCut_short_higher_pointNum,ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength),alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_adc_tmp->SetPoint(alphaCut_short_higher_pointNum,ADC_per_MeV*15,alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_adc_tmp->SetPoint(alphaCut_short_higher_pointNum,ADC_per_MeV*15,frag_energy_lengthline->Eval(15));alphaCut_short_higher_pointNum++;
		//////////////  plus stdDev exponential fit loop ////////////////////		
		double energy=0;
		double fragEnergy=0;
		double alphaCut_short_higherEnergy=0;
		for (double en_loop = alphaFit_plusStdDev[i]->GetX(0); en_loop < alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength); en_loop+=tcutStep){
			energy=en_loop;
			if (energy <= 0 || alphaFit_plusStdDev[i]->Eval(energy) <= 0 ) continue;
			if (energy >= alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength) || alphaFit_plusStdDev[i]->Eval(energy) >= alpha_maxCutLength ) continue;
			if ( alphaFit_plusStdDev[i]->Eval(energy) < frag_energy_lengthline->Eval(energy) )
			{
				fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*energy,alphaFit_plusStdDev[i]->Eval(energy));frag_pointNum++;
				fragEnergy=energy;
			}
			if ( alphaFit_plusStdDev[i]->Eval(energy) > frag_energy_lengthline->Eval(energy) )
			{
				alphaCut_short_higher_adc_tmp->SetPoint(alphaCut_short_higher_pointNum,ADC_per_MeV*energy,alphaFit_plusStdDev[i]->Eval(energy));alphaCut_short_higher_pointNum++;
			}
			alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*en_loop,alphaFit_plusStdDev[i]->Eval(energy));alpha_pointNum++;
			printf("alphaCut_adc_tmp->SetPoint(%d,%1.3f,%1.3f)\n",frag_pointNum,ADC_per_MeV*energy,alphaFit_plusStdDev[i]->Eval(energy));
		}
		energy = alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength);
		alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*energy,alpha_maxCutLength);alpha_pointNum++;
		energy = alphaFit_minusStdDev[i]->GetX(alpha_maxCutLength);
 		alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*energy,alpha_maxCutLength);alpha_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*(fragEnergy+tcutStep),alphaFit_plusStdDev[i]->Eval(fragEnergy+tcutStep));frag_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*15,frag_energy_lengthline->Eval(15));frag_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*30,frag_energy_lengthline->Eval(30));frag_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*71,3.15);frag_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*200,3.65);frag_pointNum++;
		fragCut_adc_tmp->SetPoint(frag_pointNum,ADC_per_MeV*200,0);frag_pointNum++;
		fragCut_adc[i] = fragCut_adc_tmp;
		sprintf(charBuff,"fragCut_adc_preamp%d_%dstdDev.C",preamp,i);
		ofstream fragPrimitive;
		fragPrimitive.open(charBuff);
		fragCut_adc[i]->SavePrimitive(fragPrimitive);
		fragCut_adc[i]->Write();
		alphaCut_short_higher_adc_tmp->SetPoint(alphaCut_short_higher_pointNum,ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength),alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_adc_tmp->SetPoint(alphaCut_short_higher_pointNum,ADC_per_MeV*15,alpha_maxCutLength);alphaCut_short_higher_pointNum++;
		alphaCut_short_higher_adc[i] = alphaCut_short_higher_adc_tmp;
		ofstream alphaCut_short_higher;
		sprintf(charBuff,"alphaCut_short_higher_adc_preamp%d_%dstdDev.C",preamp,i);
		alphaCut_short_higher.open(charBuff);
		alphaCut_short_higher_adc[i]->SavePrimitive(alphaCut_short_higher);
		alphaCut_short_higher_adc[i]->Write();
		sprintf(charBuff,"alphaCut_short_low_adc_preamp%d_expoFit_%dstdDev",preamp,i);
		TCutG* alphaCut_short_low_adc_tmp= new TCutG(charBuff,4);
		alphaCut_short_low_adc_tmp->SetVarX("adc");
		alphaCut_short_low_adc_tmp->SetVarY("length");
		alphaCut_short_low_adc_tmp->SetLineColor(kRed);
		alphaCut_short_low_adc_tmp->SetLineWidth(1);
		alphaCut_short_low_adc_tmp->SetFillColorAlpha(kRed,0.15);
		int short_low_pointNum=0;
		alphaCut_short_low_adc_tmp->SetPoint(short_low_pointNum,ADC_per_MeV*0,3);short_low_pointNum++;
		alphaCut_short_low_adc_tmp->SetPoint(short_low_pointNum,ADC_per_MeV*f_alphaCut_long_low_energy->GetX(alpha_maxCutLength),alpha_maxCutLength);short_low_pointNum++;
		alphaCut_short_low_adc_tmp->SetPoint(short_low_pointNum,ADC_per_MeV*energy,alpha_maxCutLength);short_low_pointNum++;
		//////////////  plus stdDev exponential fit loop ////////////////////
		for (double en_loop = alphaFit_minusStdDev[i]->GetX(alpha_maxCutLength); en_loop > 0; en_loop-=tcutStep){
			energy=en_loop;
			if (energy <= 0 || alphaFit_minusStdDev[i]->Eval(energy) <= 0 ) continue;
			if (energy >= alpha_maxCutLength || alphaFit_minusStdDev[i]->Eval(energy) >= alpha_maxCutLength ) continue;

			alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*energy,alphaFit_minusStdDev[i]->Eval(energy));alpha_pointNum++;
			alphaCut_short_low_adc_tmp->SetPoint(short_low_pointNum,ADC_per_MeV*energy,alphaFit_minusStdDev[i]->Eval(energy));short_low_pointNum++;
		}
		alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*0,alphaFit_minusStdDev[i]->Eval(0));alpha_pointNum++;
		alphaCut_adc_tmp->SetPoint(alpha_pointNum,ADC_per_MeV*0,0);
		alphaCut_adc[i] = alphaCut_adc_tmp;
		ofstream alphaCut_adc_primitive;
		sprintf(charBuff,"alphaCut_adc_preamp%d_expoFit_%dstdDev.C",preamp,i);
		alphaCut_adc_primitive.open(charBuff);
		alphaCut_adc[i]->SavePrimitive(alphaCut_adc_primitive);
		alphaCut_adc[i]->Write();
		alphaCut_short_low_adc_tmp->SetPoint(short_low_pointNum,ADC_per_MeV*0,alphaFit_minusStdDev[i]->Eval(0));short_low_pointNum++;
		alphaCut_short_low_adc_tmp->SetPoint(short_low_pointNum,ADC_per_MeV*0,3);short_low_pointNum++;
		alphaCut_short_low_adc[i] = alphaCut_short_low_adc_tmp;
		ofstream alphaCut_short_low_adc_primitive;
		sprintf(charBuff,"alphaCut_short_low_adc_preamp%d_expoFit_%dstdDev.C",preamp,i);
		alphaCut_short_low_adc_primitive.open(charBuff);
		alphaCut_short_low_adc[i]->SavePrimitive(alphaCut_short_low_adc_primitive);
		alphaCut_short_low_adc[i]->Write();
		noMaskLvEn->GetXaxis()->SetRangeUser(0,15);
		noMaskLvEn->GetYaxis()->SetRangeUser(0,15);
		noMaskLvEn->Draw("colz");
	}


	// //////////////////////////////////////////////////////////////////////////////////////
	// //////////////////////// defining frag cut points along stdDev //////////////////////
	// //////////////////////////////////////////////////////////////////////////////////////
	// // printf("\n\n/////////////////// defining frag cut points along stdDev ///////////////////\n\n");
	// // int energyBin_start=200;
	// // int energyBin_stop=1000;
	// // int energyBinStep=10;
	// // int energyFitBins=(int)(energyBin_stop-energyBin_start)/energyBinStep;
	// // Double_t lengthCutPoint_frags_plusStdDev[nStdDev][energyFitBins];
	// // Double_t energyCutPoint_frags_plusStdDev[energyFitBins];
	// // noMaskLvEn->GetYaxis()->SetRangeUser(0,3);
	// // arrayPos=0;
	// // for (int energyBin = energyBin_start; energyBin < energyBin_stop; energyBin+=energyBinStep){
	// // 	TH1D *pLength = noMaskLvEn->ProjectionY("pLength", energyBin, energyBin+energyBinStep);
	// // 	energyCutPoint_frags_plusStdDev[arrayPos] = energyBin*MeV_per_ADC;
	// // 	TH1D *pLength_frags = (TH1D*)pLength->Clone("pLength_frags");
	// // 	for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){
	// // 		if (lengthBin<180) continue;
	// // 		pLength_frags->SetBinContent(lengthBin,0);
	// // 	}
	// // 	pLength_frags->Draw();
	// // 	for (int i = 0; i < nStdDev; ++i){
	// // 		lengthCutPoint_frags_plusStdDev[i][arrayPos] = pLength_frags->GetMean()+i*pLength_frags->GetStdDev();
	// // 		TLine *Line_plusStdDev = new TLine(
	// // 			lengthCutPoint_frags_plusStdDev[i][arrayPos],0,
	// // 			lengthCutPoint_frags_plusStdDev[i][arrayPos],10-i);
	// // 		Line_plusStdDev->SetLineColor(i+1);
	// // 		Line_plusStdDev->Draw("same");
	// // 	}
	// // 	sprintf(charBuff,"fragBand_pLength.energyBin%d.png",energyBin);
	// // 	c1->SetLogy(1);
	// // 	c1->SaveAs(charBuff);
	// // 	c1->SetLogy(0);
	// // 	arrayPos++;		
	// // }

	// // /////////////////////////////////////////////////////////////////////////////////////
	// // ////////////////////////////////  fitting frag cut points  /////////////////////////
	// // /////////////////////////////////////////////////////////////////////////////////////
	// // printf("\n\n///////////////////////  fit frag cut points  ///////////////////\n\n");
	// // TGraph *fragGraph_plusStdDev[nStdDev];
	// // TF1 *fragFit_plusStdDev[nStdDev];
	// // for (int i = 0; i < nStdDev; ++i){
	// // 	fragGraph_plusStdDev[i] = new TGraph((sizeof(lengthCutPoint_frags_plusStdDev[0])/sizeof(Double_t)),energyCutPoint_frags_plusStdDev,lengthCutPoint_frags_plusStdDev[i]);
	// // 	sprintf(charBuff,"fragFit_plus_%dstdDev",i);
	// // 	fragFit_plusStdDev[i]=new TF1(charBuff,"[0]+[4]*TMath::Power( [1]*x+[3], 0.5 )",1,200);
	// // 	// fragFit_plusStdDev[i]=new TF1(charBuff,"TMath::Log([1]*x+[2])+[0]",1,200);
	// // 	fragFit_plusStdDev[i]->SetLineColor(i+1);
	// // 	fragFit_plusStdDev[i]->SetLineWidth(1);
	// // 	fragGraph_plusStdDev[i]->Fit(fragFit_plusStdDev[i]);
	// // }
	// // ////////////////////////////////////////////////////////////////////////////////////
	// // ////////////////////////////////  drawing frag cut points /////////////////////////
	// // ////////////////////////////////////////////////////////////////////////////////////
	// // printf("\n\n/////////////////////// drawing frag cut points ///////////////////\n\n");
	// // noMaskLvEn->GetXaxis()->SetRangeUser(0,50);
	// // noMaskLvEn->GetYaxis()->SetRangeUser(0,5);
	// // noMaskLvEn->Draw("colz");
	// // for (int i = minStdDev; i < nStdDev; ++i){
	// // 	fragFit_plusStdDev[i]->Draw("same");
	// // }
	// // sprintf(charBuff,"noMaskLvEn_fragCutPoints.png");
	// // c1->SaveAs(charBuff);
	// // c1->SetLogz(1);
	// // arrayPos=0;
	// // for (int energyBin = energyBin_start; energyBin < energyBin_stop; energyBin+=energyBinStep){
	// // 	for (int i = 0; i < nStdDev; ++i)
	// // 	{
	// // 		double length=lengthCutPoint_frags_plusStdDev[i][arrayPos];
	// // 		double energy_plus=energyBin*MeV_per_ADC;
	// // 		TLine *Line_plusStdDev = new TLine(
	// // 			energy_plus,length-0.02,
	// // 			energy_plus,length+0.02);
	// // 		Line_plusStdDev->SetLineColor(i+1);
	// // 		Line_plusStdDev->SetLineWidth(1);
	// // 		Line_plusStdDev->Draw();
	// // 	}
	// // 	sprintf(charBuff,"noMaskLvEn_cutPoints.energyBin%d.png",energyBin);
	// // 	c1->SaveAs(charBuff);
	// // 	arrayPos++;		
	// // }

	// ////////////////////////////////////////////////////////////////////////////////
	// ///////////////////////////// creating frag TCutG - energy /////////////////////
	// ////////////////////////////////////////////////////////////////////////////////
	// printf("\n\n/////////////////////// creating frag TCutG - energy ///////////////////\n\n");
	// TF1* frag_energy_lengthline = new TF1("frag_energy_lengthline","[1]*x+[0]",0,100);
	// frag_energy_lengthline->SetLineWidth(1);
	// frag_energy_lengthline->SetParameter(0,1.1);
	// frag_energy_lengthline->SetParameter(1,0.04);
	// TCutG *fragCut_energy[nStdDev];
	// TCutG *alphaCut_short_higher_energy[nStdDev];
	// for (int i = minStdDev; i < nStdDev; ++i)
	// {
	// 	sprintf(charBuff,"fragCut_energy_preamp%d_%dstdDev",preamp,i);
	// 	TCutG *fragCut_energy_tmp = new TCutG(charBuff,4);
	// 	fragCut_energy_tmp->SetVarX("adc");
	// 	fragCut_energy_tmp->SetVarY("length");
	// 	fragCut_energy_tmp->SetLineColor(kViolet);
	// 	fragCut_energy_tmp->SetLineWidth(1);
	// 	fragCut_energy_tmp->SetFillColorAlpha(kViolet,0.15);
	// 	int pointNum=0;
	// 	fragCut_energy_tmp->SetPoint(pointNum,200,0);pointNum++;
	// 	fragCut_energy_tmp->SetPoint(pointNum,alphaFit_plusStdDev[i]->GetX(0),0);pointNum++;
	// 	double lastEnergy=0;
	// 	for (double energy = alphaFit_plusStdDev[i]->GetX(0); energy < 4; energy+=tcutStep){
	// 		if (energy<0)continue;
	// 		if (alphaFit_plusStdDev[i]->Eval(energy)<0)continue;
	// 		if (alphaFit_plusStdDev[i]->Eval(energy)>frag_energy_lengthline->Eval(energy)){
	// 			fragCut_energy_tmp->SetPoint(pointNum,energy,frag_energy_lengthline->Eval(energy));pointNum++;
	// 		}
	// 		else{
	// 			fragCut_energy_tmp->SetPoint(pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
	// 		}
	// 		lastEnergy=energy;
	// 	}
	// 	fragCut_energy_tmp->SetPoint(pointNum,lastEnergy,frag_energy_lengthline->Eval(lastEnergy));pointNum++;
	// 	fragCut_energy_tmp->SetPoint(pointNum,30,frag_energy_lengthline->Eval(30));pointNum++;
	// 	fragCut_energy_tmp->SetPoint(pointNum,71,3.15);pointNum++;
	// 	fragCut_energy_tmp->SetPoint(pointNum,200,3.65);pointNum++;
	// 	fragCut_energy_tmp->SetPoint(pointNum,200,0);pointNum++;
	// 	fragCut_energy[i] = fragCut_energy_tmp;

	// 	sprintf(charBuff,"alphaCut_short_higher_energy_preamp%d_%dstdDev",preamp,i);
	// 	TCutG *alphaCut_short_higher_energy_tmp = new TCutG(charBuff,4);
	// 	alphaCut_short_higher_energy_tmp->SetVarX("adc");
	// 	alphaCut_short_higher_energy_tmp->SetVarY("length");
	// 	alphaCut_short_higher_energy_tmp->SetLineColor(kOrange+7);
	// 	alphaCut_short_higher_energy_tmp->SetLineWidth(1);
	// 	alphaCut_short_higher_energy_tmp->SetFillColorAlpha(kOrange+7,0.15);
	// 	pointNum=0;
	// 	alphaCut_short_higher_energy_tmp->SetPoint(pointNum,15,alpha_maxCutLength);pointNum++;
	// 	alphaCut_short_higher_energy_tmp->SetPoint(pointNum,15,frag_energy_lengthline->Eval(15));pointNum++;
	// 	alphaCut_short_higher_energy_tmp->SetPoint(pointNum,lastEnergy,frag_energy_lengthline->Eval(lastEnergy));pointNum++;

	// 	for (double energy = lastEnergy; energy < alphaFit_plusStdDev[i]->GetX(alpha_maxCutLength); energy+=tcutStep){
	// 		alphaCut_short_higher_energy_tmp->SetPoint(pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
	// 		lastEnergy=energy;
	// 	}
	// 	alphaCut_short_higher_energy_tmp->SetPoint(pointNum,lastEnergy,frag_energy_lengthline->Eval(lastEnergy));pointNum++;
	// 	alphaCut_short_higher_energy_tmp->SetPoint(pointNum,15,alpha_maxCutLength);pointNum++;
	// 	alphaCut_short_higher_energy[i] = alphaCut_short_higher_energy_tmp;

	// 	sprintf(charBuff,"fragCut_energy_preamp%d_%dstdDev.C",preamp,i);
	// 	ofstream fragPrimitive;
	// 	fragPrimitive.open(charBuff);
	// 	fragCut_energy[i]->SavePrimitive(fragPrimitive);
	// 	fragCut_energy[i]->Write();


	// 	alphaCut_energy[i]->GetXaxis()->SetRangeUser(0,8);
	// 	alphaCut_energy[i]->GetYaxis()->SetRangeUser(0,8);
	// 	noMaskLvEn->Draw("colz");
	// 	alphaCut_short_higher_energy[i]->Draw("same f");
	// 	alphaCut_short_higher_energy[i]->Draw("same");
	// 	alphaCut_short_low_energy[i]->Draw("same f");
	// 	alphaCut_short_low_energy[i]->Draw("same");
	// 	alphaCut_energy[i]->Draw("same f");
	// 	alphaCut_energy[i]->Draw("same");
	// 	fragCut_energy[i]->Draw("same f");
	// 	fragCut_energy[i]->Draw("same");
	// 	alphaCut_long_low_energy->Draw("same f");
	// 	alphaCut_long_low_energy->Draw("same");
	// 	alphaCut_long_energy->Draw("same f");
	// 	alphaCut_long_energy->Draw("same");
	// 	sprintf(charBuff,"alphaCut_stdDev%d.png",i);
	// 	c1->SaveAs(charBuff);

	// 	noMaskLvEn_below_zCut->Draw("colz");
	// 	alphaCut_energy[i]->Draw("same f");
	// 	alphaCut_energy[i]->Draw("same");
	// 	alphaCut_short_higher_energy[i]->Draw("same f");
	// 	alphaCut_short_higher_energy[i]->Draw("same");
	// 	alphaCut_short_low_energy[i]->Draw("same f");
	// 	alphaCut_short_low_energy[i]->Draw("same");
	// 	fragCut_energy[i]->Draw("same f");
	// 	fragCut_energy[i]->Draw("same");
	// 	alphaCut_long_low_energy->Draw("same f");
	// 	alphaCut_long_low_energy->Draw("same");
	// 	alphaCut_long_energy->Draw("same f");
	// 	alphaCut_long_energy->Draw("same");
	// 	c1->SetLogz(0);
	// 	sprintf(charBuff,"alphaCut_below_zCut_stdDev%d.png",i);
	// 	c1->SaveAs(charBuff);

	// }

	// printf("\n\n/////////////////////// creating frag TCutG - ADC ///////////////////\n\n");
	// TCutG *fragCut_adc[nStdDev];
	// for (int i = minStdDev; i < nStdDev; ++i)
	// {
	// 	sprintf(charBuff,"fragCut_adc_preamp%d_%dstdDev",preamp,i);
	// 	TCutG *fragCut_adc_tmp = new TCutG(charBuff,4);
	// 	fragCut_adc_tmp->SetVarX("adc");
	// 	fragCut_adc_tmp->SetVarY("length");
	// 	fragCut_adc_tmp->SetLineColor(kViolet);
	// 	fragCut_adc_tmp->SetLineWidth(1);
	// 	fragCut_adc_tmp->SetFillColorAlpha(kViolet,0.15);
	// 	int pointNum=0;
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*150),0);pointNum++;
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(0)),0);pointNum++;
	// 	double lastEnergy=0;
	// 	for (double energy = alphaFit_plusStdDev[i]->GetX(0); energy < 4; energy+=tcutStep){
	// 	if (energy<0)continue;
	// 		if (energy<0)continue;
	// 		if (alphaFit_plusStdDev[i]->Eval(energy)<0)continue;
	// 		if (alphaFit_plusStdDev[i]->Eval(energy)>frag_energy_lengthline->Eval(energy)){
	// 			fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*energy),frag_energy_lengthline->Eval(energy));pointNum++;
	// 		}
	// 		else{
	// 			fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*energy),alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
	// 		}
	// 		lastEnergy=energy;
	// 	}
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*lastEnergy),frag_energy_lengthline->Eval(lastEnergy));pointNum++;
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*30),frag_energy_lengthline->Eval(30));pointNum++;
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*71),3.15);pointNum++;
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*150),3.65);pointNum++;
	// 	fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*150),0);pointNum++;
	// 	fragCut_adc[i] = fragCut_adc_tmp;
	// 	sprintf(charBuff,"fragCut_adc_preamp%d_%dstdDev.C",preamp,i);
	// 	ofstream fragPrimitive;
	// 	fragPrimitive.open(charBuff);
	// 	fragCut_adc[i]->SavePrimitive(fragPrimitive);
	// }

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  creating noMaskLvEn_alphas //////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// creating noMaskLvEn_alphas ///////////////////\n\n");
	TH2D* noMaskLvEn_alphas[nStdDev];
	TH2D* noMaskLvEn_frags[nStdDev];
	TH2D* noMaskLvEn_not_alphas[nStdDev];
	TH2D* noMaskLvEn_not_frags[nStdDev];
	int counts=0;
	for (int i = minStdDev; i < nStdDev; ++i){
		printf("/////////////// nStdDev %d ///////////////\n",i);
		sprintf(charBuff,"noMaskLvEn_alphas_%dstdDev",i);
		TH2D* noMaskLvEn_alphas_tmp = (TH2D*)noMaskLvEn->Clone(charBuff);
		noMaskLvEn_alphas_tmp->SetTitle(charBuff);
		sprintf(charBuff,"noMaskLvEn_frags_%dstdDev",i);
		TH2D* noMaskLvEn_frags_tmp = (TH2D*)noMaskLvEn->Clone(charBuff);
		noMaskLvEn_frags_tmp->SetTitle(charBuff);

		sprintf(charBuff,"noMaskLvEn_not_alphas_%dstdDev",i);
		TH2D* noMaskLvEn_not_alphas_tmp = (TH2D*)noMaskLvEn->Clone(charBuff);
		noMaskLvEn_not_alphas_tmp->SetTitle(charBuff);
		sprintf(charBuff,"noMaskLvEn_not_frags_%dstdDev",i);
		TH2D* noMaskLvEn_not_frags_tmp = (TH2D*)noMaskLvEn->Clone(charBuff);
		noMaskLvEn_not_frags_tmp->SetTitle(charBuff);
		for (int energyBin = 0; energyBin < 4000; ++energyBin){
			for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){
				double energy=energyBin*MeV_per_ADC;
				double length=lengthBin/(double)100;
				counts=noMaskLvEn->GetBinContent(energyBin,lengthBin);
				noMaskLvEn_alphas_tmp->SetBinContent(energyBin,lengthBin,0);
				noMaskLvEn_frags_tmp->SetBinContent(energyBin,lengthBin,0);
				noMaskLvEn_not_alphas_tmp->SetBinContent(energyBin,lengthBin,0);
				noMaskLvEn_not_frags_tmp->SetBinContent(energyBin,lengthBin,0);
				// if (alphaCut_zCut_stdDev_energy[i]->IsInside(energy,length)){
				if (alphaCut_zCut_stdDev_energy[i]->IsInside(energy,length)){
					noMaskLvEn_alphas_tmp->SetBinContent(energyBin,lengthBin,counts);
				}
				else{
					noMaskLvEn_not_alphas_tmp->SetBinContent(energyBin,lengthBin,counts);
				}
				if (fragCut_energy[i]->IsInside(energy,length)){
					noMaskLvEn_frags_tmp->SetBinContent(energyBin,lengthBin,counts);
				}
				else{
					noMaskLvEn_not_frags_tmp->SetBinContent(energyBin,lengthBin,counts);
				}
				// if (fragCut_energy[i]->IsInside(energy,length))
				// 	noMaskLvEn_frags_tmp->SetBinContent(energyBin,lengthBin,counts);
				noMaskLvEn_alphas[i]=noMaskLvEn_alphas_tmp;
				noMaskLvEn_frags[i]=noMaskLvEn_frags_tmp;
				noMaskLvEn_not_alphas[i]=noMaskLvEn_not_alphas_tmp;
				noMaskLvEn_not_frags[i]=noMaskLvEn_not_frags_tmp;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////// creating noMaskLvEn_alphas_meanShifted //////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// creating noMaskLvEn_alphas_meanShifted ///////////////////\n\n");
	c1->SetLogz(1);c1->Update();
	TH2D* noMaskLvEn_alphas_meanShifted[nStdDev];
	TH1D* lengthProj_alphas_meanShifted[nStdDev];
	int meanShift[700];
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(charBuff,"noMaskLvEn_alphas_meanShifted_tmp_stdDev%d",i);
		TH2D* noMaskLvEn_alphas_meanShifted_tmp = new TH2D(charBuff,charBuff,700,0,7,200,-100*MeV_per_ADC,100*MeV_per_ADC);
		for (int lengthBin = 0; lengthBin < 700; ++lengthBin)
		{
			c1->SetLogz(0);
			c1->SetLogy(0);
			sprintf(charBuff,"px_ybin%d",lengthBin);
			TH1D *px = noMaskLvEn_alphas[i]->ProjectionX(charBuff,lengthBin,lengthBin);
			if (i==minStdDev)
				meanShift[lengthBin]=px->FindBin(px->GetMean());
			int alphaStartBin=px->FindBin(alphaFit_minusStdDev[i]->GetX(lengthBin/(double)100));
			int alphaStopBin=px->FindBin(alphaFit_plusStdDev[i]->GetX(lengthBin/(double)100));
			int alphaShift = (alphaStartBin+alphaStopBin)/2;
			for (int i = alphaStartBin; i < alphaStopBin; ++i)
				noMaskLvEn_alphas_meanShifted_tmp->SetBinContent(lengthBin,i+100-meanShift[lengthBin],px->GetBinContent(i));
		}
		noMaskLvEn_alphas_meanShifted[i]=noMaskLvEn_alphas_meanShifted_tmp;
		TH1D* lengthProj_alphas_meanShifted_tmp = noMaskLvEn_alphas_meanShifted[i]->ProjectionX("lengthProj_alphas_meanShifted");
		lengthProj_alphas_meanShifted[i] = lengthProj_alphas_meanShifted_tmp;
		c1->SetLogy(1);c1->Update();
		lengthProj_alphas_meanShifted[i]->Draw();
		lengthProj_alphas_meanShifted[i]->GetXaxis()->SetRangeUser(0,7);
		lengthProj_alphas_meanShifted[i]->GetXaxis()->SetTitle("length [cm]");		
		sprintf(charBuff,"lengthProj_alphas_meanShifted_stdDev%d.png",i);
		c1->SaveAs(charBuff);
		c1->SetLogy(0);c1->Update();
		c1->SetLogz(1);c1->Update();
		noMaskLvEn_alphas_meanShifted[i]->SetStats(kFALSE);
		noMaskLvEn_alphas_meanShifted[i]->Draw("colz");
		noMaskLvEn_alphas_meanShifted[i]->GetXaxis()->SetRangeUser(0,7);
		noMaskLvEn_alphas_meanShifted[i]->GetXaxis()->SetTitle("length [cm]");
		noMaskLvEn_alphas_meanShifted[i]->GetYaxis()->SetRangeUser(-80,80);
		noMaskLvEn_alphas_meanShifted[i]->GetYaxis()->SetTitle("mean shifted energy [MeV]");
		sprintf(charBuff,"noMaskLvEn_alphas_meanShifted_stdDev%d.png",i);
		c1->SaveAs(charBuff);
		c1->SetLogz(0);c1->Update();
	}

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////  plotting zCut scans  ////////////////////
	///////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////// plotting zCut scans //////////////////\n\n");
	TLatex text;
	text.SetTextAlign(13);
	text.SetTextSize(0.025);
	text.SetTextFont(42);
	c1->SetLogx(1);
	for (int z = 0; z <= 110; z+=2){
		for (int energyBin = 0; energyBin < 4000; ++energyBin){
			for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){
				noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,0);
				noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,0);
				int counts=noMaskLvEn_not_frags[5]->GetBinContent(energyBin,lengthBin);
				if (counts < z){noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,counts);}
				else{noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,counts);}
			}
		}
		c1->cd();
		c1->SetLogz(0);
		c1->SetLogx(0);
		noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.4,10);
		sprintf(charBuff,"noMaskLvEn_below_zCut%03d",z);
		noMaskLvEn_below_zCut->SetTitle(charBuff);
		noMaskLvEn_below_zCut->SetStats(kFALSE);
		noMaskLvEn_below_zCut->Draw("colz");
		alphaCut_zCut_energy->Draw("same f");
		alphaCut_zCut_energy->Draw("same");
		sprintf(charBuff,"number_above_zCut%d: %d",z,(int)noMaskLvEn_above_zCut->Integral());
		text.DrawTextNDC(0.15,0.89,charBuff);
		sprintf(charBuff,"number_below_zCut%d: %d",z,(int)noMaskLvEn_below_zCut->Integral());
		text.DrawTextNDC(0.15,0.86,charBuff);
		sprintf(charBuff,"%% #alpha tracks: %1.3f",noMaskLvEn_below_zCut->Integral()/(double)noMaskLvEn_not_frags[5]->Integral()*100);
		text.DrawTextNDC(0.15,0.81,charBuff);
		TLegend *zCutLegend = new TLegend(0.35,0.15,0.55,0.12);
		zCutLegend->SetTextFont(42);
		zCutLegend->SetTextSize(0.03);
		zCutLegend->SetFillColorAlpha(kWhite,0.2);
		zCutLegend->AddEntry(alphaCut_zCut_energy,">99%% alphas inside");
		zCutLegend->Draw();
		TPad *inset_pad0 = new TPad ("inset_pad0", "inset_pad0",0.55,0.55,0.9,0.9);
		inset_pad0->SetPhi(35);
		inset_pad0->SetTheta(70);
		inset_pad0->Draw();
		inset_pad0->cd();
		inset_pad0->SetLogz(1);
		inset_pad0->SetLogx(0);
		noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.4,10);
		noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,6);
		sprintf(charBuff,"noMaskLvEn_above_zCut%d",z);
		noMaskLvEn_above_zCut->SetTitle(charBuff);
		sprintf(charBuff,"noMaskLvEn_alphas_zCut%03d.png",z);
		noMaskLvEn_above_zCut->Draw("surf2");
		c1->cd();
		c1->SaveAs(charBuff);
		delete inset_pad0;
	}

	c1->SetLogx(1);
	for (int z = 0; z <= 150; z+=2){
		zCut=z;
		for (int energyBin = 0; energyBin < 4000; ++energyBin){
			for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){
				noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,0);
				noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,0);
				int counts=noMaskLvEn->GetBinContent(energyBin,lengthBin);
				if (counts<zCut){noMaskLvEn_below_zCut->SetBinContent(energyBin,lengthBin,counts);}
				else{noMaskLvEn_above_zCut->SetBinContent(energyBin,lengthBin,counts);}
			}
		}
		c1->cd();
		c1->SetLogz(0);
		noMaskLvEn_below_zCut->GetXaxis()->SetRangeUser(0.4,200);
		sprintf(charBuff,"noMaskLvEn_below_zCut%d",zCut);
		noMaskLvEn_below_zCut->SetTitle(charBuff);
		noMaskLvEn_below_zCut->SetStats(kFALSE);
		noMaskLvEn_below_zCut->Draw("colz");
		alphaCut_zCut_energy->Draw("same");
		sprintf(charBuff,"number_above_zCut%d: %d",zCut,(int)noMaskLvEn_above_zCut->Integral());
		text.DrawTextNDC(0.15,0.89,charBuff);
		sprintf(charBuff,"number_below_zCut%d: %d",zCut,(int)noMaskLvEn_below_zCut->Integral());
		text.DrawTextNDC(0.15,0.86,charBuff);
		sprintf(charBuff,"%% #all tracks: %1.3f",noMaskLvEn_below_zCut->Integral()/(double)noMaskLvEn->Integral()*100);
		text.DrawTextNDC(0.15,0.81,charBuff);
		TPad *inset_pad0 = new TPad ("inset_pad0", "inset_pad0",0.55,0.55,0.9,0.9);
		inset_pad0->Draw();
		inset_pad0->cd();
		inset_pad0->SetLogz(1);
		inset_pad0->SetLogx(1);
		noMaskLvEn_above_zCut->GetXaxis()->SetRangeUser(0.4,200);
		noMaskLvEn_above_zCut->GetYaxis()->SetRangeUser(0,6);
		sprintf(charBuff,"noMaskLvEn_above_zCut%d",zCut);
		noMaskLvEn_above_zCut->SetTitle(charBuff);
		sprintf(charBuff,"noMaskLvEn_all_zCut%03d.png",zCut);
		noMaskLvEn_above_zCut->Draw("surf2");
		c1->cd();
		c1->SaveAs(charBuff);
		delete inset_pad0;
	}

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////  plotting TCutGs  ////////////////////////
	///////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// plotting TCutGs ///////////////////\n\n");
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		c1->SetLogz(1);
		c1->SetLogx(0);
		c1->SetLogy(0);

		noMaskLvEn->GetXaxis()->SetRangeUser(0,15);
		noMaskLvEn->GetYaxis()->SetRangeUser(0,8);
		sprintf(charBuff,"bothCuts_noMaskLvEn_stdDev%d",i);
		noMaskLvEn->SetTitle(charBuff);
		noMaskLvEn->SetStats(kFALSE);
		noMaskLvEn->Draw("Colz");
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		sprintf(charBuff,"bothCuts_noMaskLvEn_stdDev%d.png",i);
		c1->SaveAs(charBuff);

		noMaskLvEn_alphas[i]->GetXaxis()->SetRangeUser(0,10);
		noMaskLvEn_alphas[i]->GetYaxis()->SetRangeUser(0,5);
		sprintf(charBuff,"noMaskLvEn_alphas_stdDev%d",i);
		noMaskLvEn->SetTitle(charBuff);
		noMaskLvEn_alphas[i]->SetStats(kFALSE);
		noMaskLvEn_alphas[i]->Draw("Colz");
		alphaCut_zCut_stdDev_energy[i]->Draw("same");
		// alphaCut_energy[i]->Draw("same");
		// alphaCut_energy[i]->Draw("same f");
		sprintf(charBuff,"noMaskLvEn_alphas_stdDev%d.png",i);
		c1->SaveAs(charBuff);

		noMaskLvEn_not_frags[i]->GetXaxis()->SetRangeUser(0,5);
		noMaskLvEn_not_frags[i]->GetYaxis()->SetRangeUser(0,3);
		sprintf(charBuff,"noMaskLvEn_not_frags_stdDev%d",i);
		noMaskLvEn_not_frags[i]->SetTitle(charBuff);
		noMaskLvEn_not_frags[i]->SetStats(kFALSE);
		noMaskLvEn_not_frags[i]->Draw("Colz");
		fragCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		sprintf(charBuff,"noMaskLvEn_not_frags_stdDev%d.png",i);
		c1->SaveAs(charBuff);

		noMaskLvEn_not_alphas[i]->GetXaxis()->SetRangeUser(0,15);
		noMaskLvEn_not_alphas[i]->GetYaxis()->SetRangeUser(0,8);
		sprintf(charBuff,"noMaskLvEn_not_alphas_stdDev%d",i);
		noMaskLvEn_not_alphas[i]->SetTitle(charBuff);
		noMaskLvEn_not_alphas[i]->SetStats(kFALSE);
		noMaskLvEn_not_alphas[i]->Draw("Colz");
		alphaCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f");
		sprintf(charBuff,"noMaskLvEn_not_alphas_stdDev%d.png",i);
		c1->SaveAs(charBuff);

		noMaskLvEn_frags[i]->GetXaxis()->SetRangeUser(0,5);
		noMaskLvEn_frags[i]->GetYaxis()->SetRangeUser(0,3);
		sprintf(charBuff,"noMaskLvEn_frags_stdDev%d",i);
		noMaskLvEn_frags[i]->SetTitle(charBuff);
		noMaskLvEn_frags[i]->SetStats(kFALSE);
		noMaskLvEn_frags[i]->Draw("Colz");
		fragCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		sprintf(charBuff,"noMaskLvEn_frags_stdDev%d.png",i);
		c1->SaveAs(charBuff);

		c1->SetLogz(1);c1->Update();
		noMaskLvEn->GetXaxis()->SetRangeUser(0.4,180);
		noMaskLvEn->GetYaxis()->SetRangeUser(0,8);
		sprintf(charBuff,"noMaskLvEn_full_logEnergy_stdDev%d",i);
		noMaskLvEn->SetTitle(charBuff);
		noMaskLvEn->SetStats(kFALSE);
		noMaskLvEn->Draw("Colz");
		c1->SetLogx(1);c1->Update();
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f"); 
		alphaCut_energy[i]->Draw("same");
		sprintf(charBuff,"bothCuts_noMaskLvEn_full_logEnergy_stdDev%d.png",i);
		c1->SaveAs(charBuff);

		noMaskLvEn_frags[i]->GetXaxis()->SetRangeUser(0.4,180);
		noMaskLvEn_frags[i]->GetYaxis()->SetRangeUser(0,5);
		sprintf(charBuff,"noMaskLvEn_frags_Logx_stdDev%d",i);
		noMaskLvEn_frags[i]->SetTitle(charBuff);
		noMaskLvEn_frags[i]->SetStats(kFALSE);
		noMaskLvEn_frags[i]->Draw("Colz");
		fragCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		sprintf(charBuff,"noMaskLvEn_frags_Logx_stdDev%d.png",i);
		c1->SaveAs(charBuff);
	}

	outfile->cd();
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		alphaCut_energy[i]->Write();
		fragCut_energy[i]->Write();
		alphaCut_adc[i]->Write();
		fragCut_adc[i]->Write();
		alphaFit_plusStdDev[i]->Write();
		noMaskLvEn_alphas[i]->Write();
		noMaskLvEn_frags[i]->Write();
		noMaskLvEn_not_alphas[i]->Write();
		noMaskLvEn_not_frags[i]->Write();
		// noMaskLvEn_alphas_meanShifted[i]->Write();
	}
	outfile->Close();

}
