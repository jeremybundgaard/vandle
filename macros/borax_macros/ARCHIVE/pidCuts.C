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

void pidCuts(int preamp,const char* treeFile,const char* cuts="&&1") {


	char polarCuts[567]="&&((-0.05>direction.CosTheta()&&direction.CosTheta()>-0.95)||(0.05<direction.CosTheta()&&direction.CosTheta()<0.95))";
	char digitCut[567]="&&nDigits>60";
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
	noMaskLvEn->GetXaxis()->SetRangeUser(0,10);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
	noMaskLvEn->GetXaxis()->CenterTitle();
	noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
	noMaskLvEn->GetYaxis()->CenterTitle();

	TCanvas *c1 = new TCanvas("c1","c1",1000,700);
	char c1PlotName[234];
	// gStyle->SetOptStat(0);
	gStyle->SetPalette(55);
	gStyle->SetNumberContours(999);

	noMaskTracks->Draw("length:adc*168.6/40000>>noMaskLvEn",treeCut,"colz");

	int nStdDev=10;
	int minStdDev=2;
	double MeV_per_ADC=168.6/4000;
	double ADC_per_MeV=40000/168.6;
	char dumChar[123];
	int xBuffer = 5;
	double tcutStep=0.1; /*MeV*/


	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// defining alpha cut points along stdDev //////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// define alpha cut points ///////////////////\n\n");
	int lengthBin_start=200;
	int lengthBin_stop=375;
	int lengthBinStep=5;
	int lengthFitBins=(int)(lengthBin_stop-lengthBin_start)/lengthBinStep;
	Double_t energyCutPoint_alphas_plusStdDev[nStdDev][lengthFitBins];
	Double_t lengthCutPoint_alphas_plusStdDev[lengthFitBins];
	Double_t energyCutPoint_alphas_minusStdDev[nStdDev][lengthFitBins];
	Double_t lengthCutPoint_alphas_minusStdDev[lengthFitBins];
	int arrayPos=0;
	for (int lengthBin = lengthBin_start; lengthBin < lengthBin_stop; lengthBin+=lengthBinStep){
		TH1D *px = noMaskLvEn->ProjectionX("px", lengthBin, lengthBin);
		px->GetXaxis()->SetRangeUser(0,8);
		px->GetYaxis()->SetRangeUser(1,1000);
		TH1D* px_avg = (TH1D*)px->Clone("px_avg");
		px_avg->SetLineColor(kRed);
		int startBin = px->FindFirstBinAbove(10)-xBuffer;
		int stopBin = px->FindLastBinAbove(10)+xBuffer;
		for (int i = 0; i < px->GetNbinsX(); ++i){ 
			if( (startBin<i&&i<stopBin) ) continue;
			px_avg->SetBinContent(i,0);
		}
		px_avg->Draw();
		lengthCutPoint_alphas_plusStdDev[arrayPos] = lengthBin/(double)100;
		lengthCutPoint_alphas_minusStdDev[arrayPos] = lengthBin/(double)100;
		for (int i = 0; i < nStdDev; ++i)
		{
			energyCutPoint_alphas_plusStdDev[i][arrayPos] = px_avg->GetMean()+i*px_avg->GetStdDev();
			energyCutPoint_alphas_minusStdDev[i][arrayPos] = px_avg->GetMean()-i*px_avg->GetStdDev();
			TLine *Line_plusStdDev = new TLine(
				energyCutPoint_alphas_plusStdDev[i][arrayPos],0,
				energyCutPoint_alphas_plusStdDev[i][arrayPos],10-i);
			Line_plusStdDev->SetLineColor(i);
			Line_plusStdDev->Draw("same");
			TLine *Line_minusStdDev = new TLine(
				energyCutPoint_alphas_minusStdDev[i][arrayPos],0,
				energyCutPoint_alphas_minusStdDev[i][arrayPos],10-i);
			Line_minusStdDev->SetLineColor(i);
			Line_minusStdDev->Draw("same");
		}
		// sprintf(dumChar,"AlphaBand_proj.lengthBin%d.png",lengthBin);
		// c1->SetLogy(1);
		// c1->SaveAs(dumChar);
		// c1->SetLogy(0);
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
		sprintf(dumChar,"alphaFit_plus_%dstdDev",i);
		alphaFit_plusStdDev[i]=new TF1(dumChar,"[3]+[2]*TMath::Exp([1]*x+[0])",0,10);
		alphaFit_plusStdDev[i]->SetLineColor(i);
		alphaFit_plusStdDev[i]->SetLineWidth(1);
		alphaGraph_plusStdDev[i]->Fit(alphaFit_plusStdDev[i]);
		alphaGraph_minusStdDev[i]=new TGraph((sizeof(energyCutPoint_alphas_minusStdDev[0])/sizeof(Double_t)),energyCutPoint_alphas_minusStdDev[i],lengthCutPoint_alphas_minusStdDev);
		sprintf(dumChar,"alphaFit_minus_%dstdDev",i);
		alphaFit_minusStdDev[i]=new TF1(dumChar,"[3]+[2]*TMath::Exp([1]*x+[0])",0,10);
		alphaFit_minusStdDev[i]->SetLineColor(i);
		alphaFit_minusStdDev[i]->SetLineWidth(1);
		alphaGraph_minusStdDev[i]->Fit(alphaFit_minusStdDev[i]);
	}

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  drawing alpha cut points /////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// drawing alpha cut points ///////////////////\n\n");
	noMaskLvEn->GetXaxis()->SetRangeUser(0,8);
	noMaskLvEn->GetYaxis()->SetRangeUser(0.5,5);
	gStyle->SetPalette(52);
	noMaskLvEn->SetTitle("[3]+TMath::Exp([1]*x+[0])");
	noMaskLvEn->Draw("colz");
	for (int i = 0; i < nStdDev; ++i){
		alphaFit_plusStdDev[i]->Draw("same");
		alphaFit_minusStdDev[i]->Draw("same");
	}
	c1->SetLogz(1);
	arrayPos=0;
	for (int lengthBin = lengthBin_start; lengthBin < lengthBin_stop; lengthBin+=lengthBinStep){
		for (int i = 0; i < nStdDev; ++i)
		{
			double length=lengthBin/(double)100;
			double energy_plus=energyCutPoint_alphas_plusStdDev[i][arrayPos];
			double energy_minus=energyCutPoint_alphas_minusStdDev[i][arrayPos];
			TLine *Line_plusStdDev = new TLine(
				energy_plus,length-0.02,
				energy_plus,length+0.02);
			Line_plusStdDev->SetLineColor(i);
			Line_plusStdDev->SetLineWidth(4);
			Line_plusStdDev->Draw();
			TLine *Line_minusStdDev = new TLine(
				energy_minus,length+0.02,
				energy_minus,length-0.02);
			Line_minusStdDev->SetLineColor(i);
			Line_minusStdDev->SetLineWidth(4);
			Line_minusStdDev->Draw();
		}
		sprintf(dumChar,"noMaskLvEn_cutPoints.y%d.png",lengthBin);
		c1->SaveAs(dumChar);
		arrayPos++;		
	}
	gStyle->SetPalette(55);
	noMaskLvEn->SetTitle("noMaskLvEn");

	/////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  creating alpha TCutGs //////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// creating alpha TCutG - energy ///////////////////\n\n");
	noMaskLvEn->GetXaxis()->SetRangeUser(0,15);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
	double alpha_minCutEnergy=0.3;
	double alpha_minCutLength=0.05;
	double alpha_maxCutLength=7;
	TCutG *alphaCut_energy[nStdDev];
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(dumChar,"alphaCut_energy_preamp%d_%dstdDev",preamp,i);
		TCutG* alphaCut_energy_tmp= new TCutG(dumChar,10);
		alphaCut_energy_tmp->SetVarX("adc");
		alphaCut_energy_tmp->SetVarY("length");
		alphaCut_energy_tmp->SetLineColor(kGreen);
		alphaCut_energy_tmp->SetLineWidth(2);
		alphaCut_energy_tmp->SetFillColorAlpha(kGreen,0.15);
		int pointNum=0;
		alphaCut_energy_tmp->SetPoint(pointNum,alpha_minCutEnergy,alpha_minCutLength);pointNum++;
		alphaCut_energy_tmp->SetPoint(pointNum,alphaFit_plusStdDev[i]->GetX(alpha_minCutLength),alpha_minCutLength);pointNum++;
		for (double energy = alphaFit_plusStdDev[i]->GetX(alpha_minCutLength); energy < 20; energy+=tcutStep){
		if(alphaFit_plusStdDev[i]->Eval(energy) > alpha_maxCutLength) continue;
			alphaCut_energy_tmp->SetPoint(pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
		}
		for (double energy = 8.5; energy > alpha_minCutEnergy; energy-=tcutStep){
		if(alphaFit_minusStdDev[i]->Eval(energy) > alpha_maxCutLength)continue;
			alphaCut_energy_tmp->SetPoint(pointNum,energy,alphaFit_minusStdDev[i]->Eval(energy));pointNum++;
		}
		alphaCut_energy_tmp->SetPoint(pointNum,alpha_minCutEnergy,alpha_minCutLength);
		alphaCut_energy[i] = alphaCut_energy_tmp;
		alphaCut_energy[i]->GetXaxis()->SetRangeUser(0,8);
		alphaCut_energy[i]->GetYaxis()->SetRangeUser(0,8);
		alphaCut_energy[i]->Draw();
		sprintf(dumChar,"alphaCut_stdDev%d.png",i);
		c1->SaveAs(dumChar);
		sprintf(dumChar,"alphaCut_energy_preamp%d_%dstdDev.C",preamp,i);
		ofstream alphaPrimitive;
		alphaPrimitive.open(dumChar);
		alphaCut_energy[i]->SavePrimitive(alphaPrimitive);
	}

	printf("\n\n/////////////////////// creating alpha TCutG - ADC ///////////////////\n\n");
	TCutG *alphaCut_adc[nStdDev];
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(dumChar,"alphaCut_adc_preamp%d_%dstdDev",preamp,i);
		TCutG* alphaCut_adc_tmp= new TCutG(dumChar,10);
		alphaCut_adc_tmp->SetVarX("adc");
		alphaCut_adc_tmp->SetVarY("length");
		alphaCut_adc_tmp->SetLineColor(kGreen);
		alphaCut_adc_tmp->SetLineWidth(2);
		alphaCut_adc_tmp->SetFillColorAlpha(kGreen,0.15);
		int pointNum=0;
		alphaCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*alpha_minCutEnergy),alpha_minCutLength);pointNum++;
		alphaCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(alpha_minCutLength)),alpha_minCutLength);pointNum++;
		for (double energy = alphaFit_plusStdDev[i]->GetX(alpha_minCutLength); energy < 20; energy+=tcutStep){
		if(alphaFit_plusStdDev[i]->Eval(energy) > alpha_maxCutLength) continue;
			alphaCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*energy),alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
		}
		for (double energy = 8.5; energy > alpha_minCutEnergy; energy-=tcutStep){
		if(alphaFit_minusStdDev[i]->Eval(energy) > alpha_maxCutLength)continue;
			alphaCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*energy),alphaFit_minusStdDev[i]->Eval(energy));pointNum++;
		}
		alphaCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*alpha_minCutEnergy),alpha_minCutLength);
		alphaCut_adc[i] = alphaCut_adc_tmp;
		sprintf(dumChar,"alphaCut_adc_preamp%d_%dstdDev.C",preamp,i);
		ofstream alphaPrimitive;
		alphaPrimitive.open(dumChar);
		alphaCut_adc[i]->SavePrimitive(alphaPrimitive);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// defining frag cut points along stdDev //////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////// defining frag cut points along stdDev ///////////////////\n\n");
	int energyBin_start=200;
	int energyBin_stop=1000;
	int energyBinStep=10;
	int energyFitBins=(int)(energyBin_stop-energyBin_start)/energyBinStep;
	Double_t lengthCutPoint_frags_plusStdDev[nStdDev][energyFitBins];
	Double_t energyCutPoint_frags_plusStdDev[energyFitBins];
	noMaskLvEn->GetYaxis()->SetRangeUser(0,3);
	arrayPos=0;
	for (int energyBin = energyBin_start; energyBin < energyBin_stop; energyBin+=energyBinStep){
		TH1D *pLength = noMaskLvEn->ProjectionY("pLength", energyBin, energyBin+energyBinStep);
		energyCutPoint_frags_plusStdDev[arrayPos] = energyBin*MeV_per_ADC;
		TH1D *pLength_frags = (TH1D*)pLength->Clone("pLength_frags");
		for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){
			if (lengthBin<180) continue;
			pLength_frags->SetBinContent(lengthBin,0);
		}
		pLength_frags->Draw();
		for (int i = 0; i < nStdDev; ++i){
			lengthCutPoint_frags_plusStdDev[i][arrayPos] = pLength_frags->GetMean()+i*pLength_frags->GetStdDev();
			TLine *Line_plusStdDev = new TLine(
				lengthCutPoint_frags_plusStdDev[i][arrayPos],0,
				lengthCutPoint_frags_plusStdDev[i][arrayPos],10-i);
			Line_plusStdDev->SetLineColor(i);
			Line_plusStdDev->Draw("same");
		}
		sprintf(dumChar,"fragBand_pLength.energyBin%d.png",energyBin);
		c1->SetLogy(1);
		c1->SaveAs(dumChar);
		c1->SetLogy(0);
		arrayPos++;		
	}

	// /////////////////////////////////////////////////////////////////////////////////////
	// ////////////////////////////////  fitting frag cut points  /////////////////////////
	// /////////////////////////////////////////////////////////////////////////////////////
	// printf("\n\n///////////////////////  fit frag cut points  ///////////////////\n\n");
	// TGraph *fragGraph_plusStdDev[nStdDev];
	// TF1 *fragFit_plusStdDev[nStdDev];
	// for (int i = 0; i < nStdDev; ++i){
	// 	fragGraph_plusStdDev[i] = new TGraph((sizeof(lengthCutPoint_frags_plusStdDev[0])/sizeof(Double_t)),energyCutPoint_frags_plusStdDev,lengthCutPoint_frags_plusStdDev[i]);
	// 	sprintf(dumChar,"fragFit_plus_%dstdDev",i);
	// 	fragFit_plusStdDev[i]=new TF1(dumChar,"[0]+[4]*TMath::Power( [1]*x+[3], 0.5 )",1,200);
	// 	// fragFit_plusStdDev[i]=new TF1(dumChar,"TMath::Log([1]*x+[2])+[0]",1,200);
	// 	fragFit_plusStdDev[i]->SetLineColor(i);
	// 	fragFit_plusStdDev[i]->SetLineWidth(1);
	// 	fragGraph_plusStdDev[i]->Fit(fragFit_plusStdDev[i]);
	// }
	// ////////////////////////////////////////////////////////////////////////////////////
	// ////////////////////////////////  drawing frag cut points /////////////////////////
	// ////////////////////////////////////////////////////////////////////////////////////
	// printf("\n\n/////////////////////// drawing frag cut points ///////////////////\n\n");
	// noMaskLvEn->GetXaxis()->SetRangeUser(0,50);
	// noMaskLvEn->GetYaxis()->SetRangeUser(0,5);
	// gStyle->SetPalette(52);
	// noMaskLvEn->Draw("colz");
	// for (int i = minStdDev; i < nStdDev; ++i){
	// 	fragFit_plusStdDev[i]->Draw("same");
	// }
	// sprintf(dumChar,"noMaskLvEn_fragCutPoints.png");
	// c1->SaveAs(dumChar);
	// c1->SetLogz(1);
	// arrayPos=0;
	// for (int energyBin = energyBin_start; energyBin < energyBin_stop; energyBin+=energyBinStep){
	// 	for (int i = 0; i < nStdDev; ++i)
	// 	{
	// 		double length=lengthCutPoint_frags_plusStdDev[i][arrayPos];
	// 		double energy_plus=energyBin*MeV_per_ADC;
	// 		TLine *Line_plusStdDev = new TLine(
	// 			energy_plus,length-0.02,
	// 			energy_plus,length+0.02);
	// 		Line_plusStdDev->SetLineColor(i);
	// 		Line_plusStdDev->SetLineWidth(4);
	// 		Line_plusStdDev->Draw();
	// 	}
	// 	sprintf(dumChar,"noMaskLvEn_cutPoints.energyBin%d.png",energyBin);
	// 	c1->SaveAs(dumChar);
	// 	arrayPos++;		
	// }
	// gStyle->SetPalette(55);

	////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// creating frag TCutG - energy /////////////////////
	////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// creating frag TCutG - energy ///////////////////\n\n");
	TF1* frag_energy_lengthline = new TF1("frag_energy_lengthline","[1]*x+[0]",0,100);
	frag_energy_lengthline->SetLineWidth(5);
	frag_energy_lengthline->SetParameter(0,1.1);
	frag_energy_lengthline->SetParameter(1,0.04);
	TCutG *fragCut_energy[nStdDev];
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(dumChar,"fragCut_energy_preamp%d_%dstdDev",preamp,i);
		TCutG *fragCut_energy_tmp = new TCutG(dumChar,4);
		fragCut_energy_tmp->SetVarX("adc");
		fragCut_energy_tmp->SetVarY("length");
		fragCut_energy_tmp->SetLineColor(kViolet);
		fragCut_energy_tmp->SetLineWidth(1);
		fragCut_energy_tmp->SetFillColorAlpha(kViolet,0.15);
		int pointNum=0;
		fragCut_energy_tmp->SetPoint(pointNum,150,alpha_minCutLength);pointNum++;
		fragCut_energy_tmp->SetPoint(pointNum,alphaFit_plusStdDev[i]->GetX(alpha_minCutLength),alpha_minCutLength);pointNum++;
		double lastEnergy=0;
		for (double energy = alphaFit_plusStdDev[i]->GetX(alpha_minCutLength); energy < 4; energy+=tcutStep){
		if (energy<alpha_minCutEnergy)continue;
			if (energy<alpha_minCutEnergy)continue;
			if (alphaFit_plusStdDev[i]->Eval(energy)<alpha_minCutLength)continue;

			if (alphaFit_plusStdDev[i]->Eval(energy)>frag_energy_lengthline->Eval(energy)){
				fragCut_energy_tmp->SetPoint(pointNum,energy,frag_energy_lengthline->Eval(energy));pointNum++;
			}
			else{
				fragCut_energy_tmp->SetPoint(pointNum,energy,alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
			}
			lastEnergy=energy;
		}
		fragCut_energy_tmp->SetPoint(pointNum,lastEnergy,frag_energy_lengthline->Eval(lastEnergy));pointNum++;
		fragCut_energy_tmp->SetPoint(pointNum,30,frag_energy_lengthline->Eval(30));pointNum++;
		fragCut_energy_tmp->SetPoint(pointNum,71,3.15);pointNum++;
		fragCut_energy_tmp->SetPoint(pointNum,150,3.65);pointNum++;
		fragCut_energy_tmp->SetPoint(pointNum,150,alpha_minCutLength);pointNum++;
		fragCut_energy[i] = fragCut_energy_tmp;
		sprintf(dumChar,"fragCut_energy_preamp%d_%dstdDev.C",preamp,i);
		ofstream fragPrimitive;
		fragPrimitive.open(dumChar);
		fragCut_energy[i]->SavePrimitive(fragPrimitive);
	}

	printf("\n\n/////////////////////// creating frag TCutG - ADC ///////////////////\n\n");
	TCutG *fragCut_adc[nStdDev];
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		sprintf(dumChar,"fragCut_adc_preamp%d_%dstdDev",preamp,i);
		TCutG *fragCut_adc_tmp = new TCutG(dumChar,4);
		fragCut_adc_tmp->SetVarX("adc");
		fragCut_adc_tmp->SetVarY("length");
		fragCut_adc_tmp->SetLineColor(kViolet);
		fragCut_adc_tmp->SetLineWidth(1);
		fragCut_adc_tmp->SetFillColorAlpha(kViolet,0.15);
		int pointNum=0;
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*150),alpha_minCutLength);pointNum++;
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*alphaFit_plusStdDev[i]->GetX(alpha_minCutLength)),alpha_minCutLength);pointNum++;
		double lastEnergy=0;
		for (double energy = alphaFit_plusStdDev[i]->GetX(alpha_minCutLength); energy < 4; energy+=tcutStep){
		if (energy<alpha_minCutEnergy)continue;
			if (energy<alpha_minCutEnergy)continue;
			if (alphaFit_plusStdDev[i]->Eval(energy)<alpha_minCutLength)continue;
			if (alphaFit_plusStdDev[i]->Eval(energy)>frag_energy_lengthline->Eval(energy)){
				fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*energy),frag_energy_lengthline->Eval(energy));pointNum++;
			}
			else{
				fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*energy),alphaFit_plusStdDev[i]->Eval(energy));pointNum++;
			}
			lastEnergy=energy;
		}
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*lastEnergy),frag_energy_lengthline->Eval(lastEnergy));pointNum++;
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*30),frag_energy_lengthline->Eval(30));pointNum++;
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*71),3.15);pointNum++;
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*150),3.65);pointNum++;
		fragCut_adc_tmp->SetPoint(pointNum,TMath::Ceil(ADC_per_MeV*150),alpha_minCutLength);pointNum++;
		fragCut_adc[i] = fragCut_adc_tmp;
		sprintf(dumChar,"fragCut_adc_preamp%d_%dstdDev.C",preamp,i);
		ofstream fragPrimitive;
		fragPrimitive.open(dumChar);
		fragCut_adc[i]->SavePrimitive(fragPrimitive);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  creating noMaskLvEn_alphas ////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// creating noMaskLvEn_alphas ///////////////////\n\n");
	TH2D* noMaskLvEn_alphas[nStdDev];
	TH2D* noMaskLvEn_frags[nStdDev];
	int counts=0;
	for (int i = minStdDev; i < nStdDev; ++i){
		printf("/////////////// nStdDev %d ///////////////\n",i);
		sprintf(dumChar,"noMaskLvEn_alphas_%dstdDev",i);
		TH2D* noMaskLvEn_alphas_tmp = (TH2D*)noMaskLvEn->Clone(dumChar);
		noMaskLvEn_alphas_tmp->SetTitle(dumChar);
		sprintf(dumChar,"noMaskLvEn_frags_%dstdDev",i);
		TH2D* noMaskLvEn_frags_tmp = (TH2D*)noMaskLvEn->Clone(dumChar);
		noMaskLvEn_frags_tmp->SetTitle(dumChar);
		for (int energyBin = 0; energyBin < 4000; ++energyBin){
			for (int lengthBin = 0; lengthBin < 1000; ++lengthBin){
				double energy=energyBin*MeV_per_ADC;
				double length=lengthBin/(double)100;
				counts=noMaskLvEn->GetBinContent(energyBin,lengthBin);
				noMaskLvEn_alphas_tmp->SetBinContent(energyBin,lengthBin,0);
				noMaskLvEn_frags_tmp->SetBinContent(energyBin,lengthBin,0);
				if (alphaCut_energy[i]->IsInside(energy,length))
					noMaskLvEn_alphas_tmp->SetBinContent(energyBin,lengthBin,counts);
				if (fragCut_energy[i]->IsInside(energy,length))
					noMaskLvEn_frags_tmp->SetBinContent(energyBin,lengthBin,counts);
				noMaskLvEn_alphas[i]=noMaskLvEn_alphas_tmp;
				noMaskLvEn_frags[i]=noMaskLvEn_frags_tmp;
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
		sprintf(dumChar,"noMaskLvEn_alphas_meanShifted_tmp_stdDev%d",i);
		TH2D* noMaskLvEn_alphas_meanShifted_tmp = new TH2D(dumChar,dumChar,700,0,7,200,-100*MeV_per_ADC,100*MeV_per_ADC);
		for (int lengthBin = 0; lengthBin < 700; ++lengthBin)
		{
			c1->SetLogz(0);
			c1->SetLogy(0);
			sprintf(dumChar,"px_ybin%d",lengthBin);
			TH1D *px = noMaskLvEn_alphas[i]->ProjectionX(dumChar,lengthBin,lengthBin);
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
		sprintf(dumChar,"lengthProj_alphas_meanShifted_stdDev%d.png",i);
		c1->SaveAs(dumChar);

		c1->SetLogy(0);c1->Update();
		c1->SetLogz(1);c1->Update();
		noMaskLvEn_alphas_meanShifted[i]->Draw("colz");
		noMaskLvEn_alphas_meanShifted[i]->GetXaxis()->SetRangeUser(0,7);
		noMaskLvEn_alphas_meanShifted[i]->GetXaxis()->SetTitle("length [cm]");
		noMaskLvEn_alphas_meanShifted[i]->GetYaxis()->SetRangeUser(-80,80);
		noMaskLvEn_alphas_meanShifted[i]->GetYaxis()->SetTitle("mean shifted energy [MeV]");
		sprintf(dumChar,"noMaskLvEn_alphas_meanShifted_stdDev%d.png",i);
		c1->SaveAs(dumChar);
		c1->SetLogz(0);c1->Update();
	}

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////  plotting TCutGs  ////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	printf("\n\n/////////////////////// plots TCutGs ///////////////////\n\n");
	c1->SetLogz(1);
	for (int i = minStdDev; i < nStdDev; ++i)
	{
		noMaskLvEn->GetXaxis()->SetRangeUser(0,15);
		noMaskLvEn->GetYaxis()->SetRangeUser(0,8);
		noMaskLvEn->Draw("Colz");
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		sprintf(dumChar,"bothCuts_noMaskLvEn_stdDev%d.png",i);
		c1->SaveAs(dumChar);

		noMaskLvEn_alphas[i]->Draw("Colz");
		alphaCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f");
		sprintf(dumChar,"noMaskLvEn_alphas_stdDev%d.png",i);
		c1->SaveAs(dumChar);

		c1->SetLogz(1);c1->Update();
		noMaskLvEn->GetXaxis()->SetRangeUser(0.4,180);
		noMaskLvEn->GetYaxis()->SetRangeUser(0,8);
		noMaskLvEn->Draw("Colz");
		c1->SetLogx(1);c1->Update();
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		sprintf(dumChar,"bothCuts_noMaskLvEn_full_logEnergy_stdDev%d.png",i);
		c1->SaveAs(dumChar);

		noMaskLvEn_frags[i]->GetXaxis()->SetRangeUser(0.4,180);
		noMaskLvEn_frags[i]->GetYaxis()->SetRangeUser(0,5);
		noMaskLvEn_frags[i]->Draw("Colz");
		fragCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		sprintf(dumChar,"noMaskLvEn_frags_stdDev%d.png",i);
		c1->SaveAs(dumChar);

		noMaskLvEn_frags[i]->GetXaxis()->SetRangeUser(0.4,180);
		noMaskLvEn_frags[i]->GetYaxis()->SetRangeUser(0,5);
		noMaskLvEn_frags[i]->Draw("Colz");
		fragCut_energy[i]->Draw("same");
		fragCut_energy[i]->Draw("same f");
		sprintf(dumChar,"noMaskLvEn_frags_stdDev%d.png",i);
		c1->SaveAs(dumChar);

		c1->SetLogx(0);c1->Update();
		noMaskLvEn->Draw("Colz");
		fragCut_energy[i]->Draw("same f");
		fragCut_energy[i]->Draw("same");
		alphaCut_energy[i]->Draw("same f");
		alphaCut_energy[i]->Draw("same");
		sprintf(dumChar,"bothCuts_noMaskLvEn_full_linearEnergy_stdDev%d.png",i);
		c1->SaveAs(dumChar);
	}

	sprintf(dumChar,"$cft/pid/pidCut%d.root",preamp);
	TFile* outfile = new TFile(dumChar,"recreate");
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
		noMaskLvEn_alphas_meanShifted[i]->Write();
	}
	outfile->Close();

}
