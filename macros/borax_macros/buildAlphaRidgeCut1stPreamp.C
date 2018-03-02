#include <climits>
#include <vector>
#include "TROOT.h"
#include "TRint.h"
#include "MsgLog.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCut.h"
#include "TCutG.h"
#include "TGaxis.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TChain.h"
#include "TTree.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TLine.h"
#include "TMarker.h"
#include "TLegend.h"
#include "TFormula.h"
#include "TStyle.h"

#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>


void buildAlphaRidgeCut1stPreamp() {

TFile* noMaskFile = new TFile("$cftrees/cfNoMask.1stPreamp.trees.1850-56.66-72.74-82.root");
TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");
TH2D * noMaskLvEn = new TH2D("noMaskLvEn","",40000,0,168.6,1000,0,10);
noMaskLvEn->GetXaxis()->SetRangeUser(1,7);
noMaskLvEn->GetYaxis()->SetRangeUser(0,6);
noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
noMaskLvEn->GetXaxis()->CenterTitle();
noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
noMaskLvEn->GetYaxis()->CenterTitle();
noMaskTracks->Draw("length:adc*168.6/40000>>noMaskLvEn","trackNo==1 && fitType==10","colz");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
gStyle->SetOptStat(0);


Double_t xCut[40];
Double_t yCut[40];
char projname[123];

int arrayPos=0;
for (int lbinStep = 65; lbinStep < 265; lbinStep+=5){
	TH1D *px = noMaskLvEn->ProjectionX("px", lbinStep, lbinStep);
	px->GetXaxis()->SetRangeUser(1,7);
	px->GetYaxis()->SetRangeUser(1,300);
	int xBuffer = 30;
	int xCutBin = px->FindLastBinAbove(10)+xBuffer;
	xCut[arrayPos] = px->GetBinCenter(xCutBin);
	yCut[arrayPos] = lbinStep/(double)100;
	// std::cout << "xCut , yCut"<<'\t'<<xCut[arrayPos] <<" , "<<yCut[arrayPos] << std::endl;
	arrayPos++;
}

c1->SetLogy(0);
TGraph *g = new TGraph((sizeof(xCut) / sizeof(Double_t)), xCut, yCut);
TF1 *f = new TF1("f", "[4] + [3]*TMath::Exp( [2]*x*x + [1]*x + [0] )",1,xCut[40 - 1]);
f->SetLineWidth(3);
f->SetLineColor(kRed);
g->Fit(f);
noMaskLvEn->SetTitle(f->GetTitle());
noMaskLvEn->Draw("Colz");
f->DrawF1(1,xCut[40 - 1],"same");
sprintf(projname,"alphaRidgeFit.LvEn.png");
c1->SaveAs(projname);
noMaskLvEn->GetXaxis()->SetRangeUser(1,3);
noMaskLvEn->GetYaxis()->SetRangeUser(0,1);
noMaskLvEn->Draw("Colz");
f->DrawF1(1,xCut[40 - 1],"same");
sprintf(projname,"alphaRidgeFit.zoom.LvEn.png");
c1->SaveAs(projname);
c1->SetLogz(1);
noMaskLvEn->Draw("Colz");
f->DrawF1(1,xCut[40 - 1],"same");
sprintf(projname,"alphaRidgeFit.zoom.log.LvEn.png");
c1->SaveAs(projname);
noMaskLvEn->GetXaxis()->SetRangeUser(0,7);
noMaskLvEn->GetYaxis()->SetRangeUser(0,6);

// show projection of alpha ridge
arrayPos=0;
for (int lbinStep = 65; lbinStep < 265; lbinStep+=5){
	c1->SetLogz(0);
	c1->SetLogy(0);
	TH1D *px = noMaskLvEn->ProjectionX("px", lbinStep, lbinStep);
	int xBuffer = 30;
	int xCutBin = px->FindLastBinAbove(10)+xBuffer;
	xCut[arrayPos] = px->GetBinCenter(xCutBin);
	yCut[arrayPos] = lbinStep/(double)100;
	std::cout << "xCut , yCut"<<'\t'<<xCut[arrayPos] <<" , "<<yCut[arrayPos] << std::endl;
	TLine *startLine = new TLine(px->GetBinCenter(px->FindLastBinAbove(10)),0,px->GetBinCenter(px->FindLastBinAbove(10)),px->GetBinContent(px->FindLastBinAbove(10)));
	startLine->SetLineColor(kGreen);
	startLine->SetLineWidth(3);
	TLine *linePlusBuffer = new TLine(xCut[arrayPos],0,xCut[arrayPos],px->GetBinContent(px->FindLastBinAbove(10)));
	linePlusBuffer->SetLineColor(kRed);
	linePlusBuffer->SetLineWidth(3);
	sprintf(projname,"projx.LvEn.ybin%d.zoom.png",lbinStep);
	px->SetTitle(projname);
	px->Draw();
	startLine->Draw("same");
	linePlusBuffer->Draw("same");
	c1->SetLogy();
	c1->SetGridx();
	c1->SetGridy();
	c1->SaveAs(projname);
	arrayPos++;
}

noMaskLvEn->GetYaxis()->SetRangeUser(0,3);
noMaskLvEn->SetTitle("LvEn [No Trigger]");
arrayPos=0;
for (int lbinStep = 65; lbinStep < 265; lbinStep+=5){
	c1->SetLogz(0);
	c1->SetLogy(0);
	TH1D *px = noMaskLvEn->ProjectionX("px", lbinStep, lbinStep);
	int xBuffer = 30;
	int xCutBin = px->FindLastBinAbove(10)+xBuffer;
	xCut[arrayPos] = px->GetBinCenter(xCutBin);
	yCut[arrayPos] = lbinStep/(double)100;
	// std::cout << "xCut , yCut"<<'\t'<<xCut[arrayPos] <<" , "<<yCut[arrayPos] << std::endl;
	TLine *startLine = new TLine(px->GetBinCenter(px->FindLastBinAbove(10)),yCut[arrayPos]-0.05,px->GetBinCenter(px->FindLastBinAbove(10)),yCut[arrayPos]+0.05);
	startLine->SetLineColor(kGreen);
	startLine->SetLineWidth(5);
	TLine *linePlusBuffer = new TLine(xCut[arrayPos],yCut[arrayPos]-0.05,xCut[arrayPos],yCut[arrayPos]+0.05);
	linePlusBuffer->SetLineColor(kRed);
	linePlusBuffer->SetLineWidth(5);
	noMaskLvEn->Draw("Colz");
	startLine->Draw("same");
	linePlusBuffer->Draw("same");
	sprintf(projname,"LvEn.y%d.zoom.png",lbinStep);
	c1->SaveAs(projname);
	arrayPos++;
}

TCutG *fragCutSimple = new TCutG("fragCutSimple",8);
fragCutSimple->SetLineColor(kViolet);
fragCutSimple->SetLineWidth(3);
fragCutSimple->GetXaxis()->SetRangeUser(1,3);
fragCutSimple->GetYaxis()->SetRangeUser(0,1);
fragCutSimple->SetPoint(1, 15,1.50);
fragCutSimple->SetPoint(2, 35,1.95);
fragCutSimple->SetPoint(3, 71,2.75);
fragCutSimple->SetPoint(4,150,3.25);
fragCutSimple->SetPoint(5,150,1.25);
fragCutSimple->SetPoint(6,  1,0.00);
fragCutSimple->SetPoint(7,  1,0.70);
fragCutSimple->SetPoint(0,  1,0.70);

int newCutPoint=7;
noMaskLvEn->GetXaxis()->SetRangeUser(1,4);
noMaskLvEn->GetYaxis()->SetRangeUser(0,2);

for (double interpoAlphaRidge = 1; interpoAlphaRidge <= 3.1; interpoAlphaRidge+=0.2)
{
	fragCutSimple->SetPoint(newCutPoint,interpoAlphaRidge,f->Eval(interpoAlphaRidge));
	fragCutSimple->SetPoint(0,interpoAlphaRidge,f->Eval(interpoAlphaRidge));
	newCutPoint++;
	noMaskLvEn->Draw("Colz");
	f->DrawF1(1,3,"same");
	fragCutSimple->Draw("same");
	sprintf(projname,"alphaRidgeFit.zoom.log.LvEn.%0.1f.png",interpoAlphaRidge);
	c1->SaveAs(projname);
}

ofstream cutfile;
cutfile.open("alphaRidgeCut1stPreamp.C");
fragCutSimple->SavePrimitive(cutfile,"");

noMaskLvEn->SetTitle(f->GetTitle());
noMaskLvEn->GetXaxis()->SetRangeUser(1,7);
noMaskLvEn->GetYaxis()->SetRangeUser(0,6);	
noMaskLvEn->Draw("Colz");
f->DrawF1(1,xCut[40 - 1],"same");
fragCutSimple->Draw("same");
sprintf(projname,"alphaRidgeFit.plusFragCut.LvEn.png");
c1->SaveAs(projname);

}
