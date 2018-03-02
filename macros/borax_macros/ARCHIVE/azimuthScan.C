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
#include "TMarker.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TText.h"
#include "NiffteGeometryMap.h"
#include "NifftePlotUtilities.h"

#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>


void azimuthScan(const int preamp, const char* recoType, const char* noMaskFileName, const char* cftFileName, const int phiLoopMin, const int phiLoopMax, const int phiLoopStep) {

TCutG *fragCut = new TCutG("fragCut",8);
fragCut->SetLineColor(kViolet);
fragCut->SetLineWidth(3);

char noMaskCut[234];
char cftCut[234];
char fitCut[123];
char noMaskRunCut[123];
char cftRunCut[123];
char azCut[123];



if (preamp==1)
{

	sprintf(noMaskRunCut,"run > 100001856 && run !=100001870");

	if (!(strcmp(recoType,"hk"))){
		sprintf(fitCut,"fitType==5");

		fragCut->SetPoint(0,2.71991,1.03077);
		fragCut->SetPoint(1,15,1.9);
		fragCut->SetPoint(2,35,2.35);
		fragCut->SetPoint(3,71,3.15);
		fragCut->SetPoint(4,150,3.65);
		fragCut->SetPoint(5,150,1.65);
		fragCut->SetPoint(6,2.01991,0.5);
		fragCut->SetPoint(7,2.01991,0.5);
		fragCut->SetPoint(8,2.11991,0.573987);
		fragCut->SetPoint(9,2.21991,0.648579);
		fragCut->SetPoint(10,2.31991,0.723779);
		fragCut->SetPoint(11,2.41991,0.799594);
		fragCut->SetPoint(12,2.51991,0.876028);
		fragCut->SetPoint(13,2.61991,0.953086);
		fragCut->SetPoint(14,2.71991,1.03077);


	}
	if ( !(strcmp(recoType,"fp"))){
		
		sprintf(fitCut,"bestFit");
		
		fragCut->SetPoint(0,2,0.856419);
		fragCut->SetPoint(1,15,1.5);
		fragCut->SetPoint(2,35,2.5);
		fragCut->SetPoint(3,60,3.25);
		fragCut->SetPoint(4,140,3.5);
		fragCut->SetPoint(5,140,1.25);
		fragCut->SetPoint(6,1,0);
		fragCut->SetPoint(7,1,0.434625);
		fragCut->SetPoint(8,1.2,0.496664);
		fragCut->SetPoint(9,1.4,0.56915);
		fragCut->SetPoint(10,1.6,0.652886);
		fragCut->SetPoint(11,1.8,0.748508);
		fragCut->SetPoint(12,2,0.856419);


	}
	sprintf(noMaskCut," %s && %s ",fitCut,noMaskRunCut);
	sprintf(cftCut,"%s",fitCut);


}
else if (preamp==2)
{

	if (!(strcmp(recoType,"hk"))){
		sprintf(fitCut,"fitType==5");

		fragCut->SetPoint(0,2.49341,0.982877);
		fragCut->SetPoint(1,15,1.9);
		fragCut->SetPoint(2,35,2.35);
		fragCut->SetPoint(3,71,3.15);
		fragCut->SetPoint(4,150,3.65);
		fragCut->SetPoint(5,150,1.65);
		fragCut->SetPoint(6,1.59341,0.5);
		fragCut->SetPoint(7,1.59341,0.5);
		fragCut->SetPoint(8,1.69341,0.55162);
		fragCut->SetPoint(9,1.79341,0.603737);
		fragCut->SetPoint(10,1.89341,0.656355);
		fragCut->SetPoint(11,1.99341,0.709481);
		fragCut->SetPoint(12,2.09341,0.763117);
		fragCut->SetPoint(13,2.19341,0.81727);
		fragCut->SetPoint(14,2.29341,0.871945);
		fragCut->SetPoint(15,2.39341,0.927145);
		fragCut->SetPoint(16,2.49341,0.982877);


	}
	if ( !(strcmp(recoType,"fp"))){
		
		sprintf(fitCut,"bestFit");

		fragCut->SetPoint(0,2,0.700327);
		fragCut->SetPoint(1,15,1.5);
		fragCut->SetPoint(2,35,2.5);
		fragCut->SetPoint(3,60,3.25);
		fragCut->SetPoint(4,140,3.5);
		fragCut->SetPoint(5,140,1.25);
		fragCut->SetPoint(6,1,0);
		fragCut->SetPoint(7,1,0.295582);
		fragCut->SetPoint(8,1.2,0.363805);
		fragCut->SetPoint(9,1.4,0.438273);
		fragCut->SetPoint(10,1.6,0.519138);
		fragCut->SetPoint(11,1.8,0.606485);
		fragCut->SetPoint(12,2,0.700327);


	}
	sprintf(noMaskCut,"%s",fitCut);
	sprintf(cftCut,"%s",fitCut);

}



std::cout <<"noMaskCut = " << noMaskCut << std::endl;
std::cout <<"cftCut = " << cftCut << std::endl;


TFile* cftFile = new TFile(cftFileName);
TFile* noMaskFile = new TFile(noMaskFileName);

TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");
TTree* cftTracks = (TTree*)cftFile->Get("tracks");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
gStyle->SetOptStat(0);

//// polar loop
std::stringstream phiCut;
double radPhiMin=0,radPhiMax=0,piLessradPhiMin=0,piLessradPhiMax=0,origin=0;
int degPhiMin=0,degPhiMax=0;
char charContainer[123];


// for (int phi = 0; phi < 60; phi+=60){
for (int phi = phiLoopMin; phi < phiLoopMax; phi+=phiLoopStep){

	// phi cut
	double deg2rad = TMath::Pi()/(double)180;
	degPhiMin=phi;
	degPhiMax=phi+2*phiLoopStep;
	radPhiMin=degPhiMin*deg2rad;
	radPhiMax=degPhiMax*deg2rad;

	phiCut.str("");
    phiCut<< radPhiMin-TMath::Pi() <<" < direction.Phi() && direction.Phi() < "<< radPhiMax-TMath::Pi() <<" && fitType==5";

	std::cout << phiCut.str().c_str() << std::endl;

	sprintf(charContainer,"noMaskLvEnPhiMin%03i",degPhiMin);
	TH2D* noMaskLvEn = new TH2D("noMaskLvEn",charContainer,40000,0,168.6,1000,0,10);

	sprintf(charContainer,"cftLvEnPhiMin%03i",degPhiMin);
	TH2D* cftLvEn = new TH2D("cftLvEn",charContainer,40000,0,168.6,1000,0,10);

	cftLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
	cftLvEn->GetXaxis()->CenterTitle();
	cftLvEn->GetYaxis()->SetTitle("Track Length [cm]");
	cftLvEn->GetYaxis()->CenterTitle();
	cftLvEn->SetTitle(phiCut.str().c_str());
	cftTracks->Draw("length:adc*168.6/40000>>cftLvEn",phiCut.str().c_str());

	TH1D * cftFrags = cftLvEn->ProjectionX("cftFrags",0,40000,"[fragCut]");
	cftFrags->GetXaxis()->SetTitle("MeV");
	cftFrags->GetXaxis()->CenterTitle();
	cftFrags->GetYaxis()->SetTitle("counts");
	cftFrags->GetYaxis()->CenterTitle();
	cftFrags->SetTitle("fragCut [Fission Trigger]");
	cftFrags->SetLineWidth(2);

	noMaskLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
	noMaskLvEn->GetXaxis()->CenterTitle();
	noMaskLvEn->GetYaxis()->SetTitle("Track Length [cm]");
	noMaskLvEn->GetYaxis()->CenterTitle();
	noMaskLvEn->SetTitle(phiCut.str().c_str());
	noMaskTracks->Draw("length:adc*168.6/40000>>noMaskLvEn",phiCut.str().c_str());

	TH1D * noMaskFrags = noMaskLvEn->ProjectionX("noMaskFrags",0,40000,"[fragCut]");
	noMaskFrags->GetXaxis()->SetTitle("MeV");
	noMaskFrags->GetXaxis()->CenterTitle();
	noMaskFrags->GetYaxis()->SetTitle("counts");
	noMaskFrags->GetYaxis()->CenterTitle();
	noMaskFrags->SetTitle("fragCut [No Trigger]");
	noMaskFrags->SetLineColor(2);
	noMaskFrags->SetLineWidth(2);

	double scaleCFT = noMaskFrags->Integral(5000,40000)/cftFrags->Integral(5000,40000);
	TH1D* scaledCftFrags = (TH1D*)cftFrags->Clone("scaledCftFrags");
	scaledCftFrags->Scale(scaleCFT);
	cftLvEn->Scale(scaleCFT);

	//////////////////////////////////////////
	//////////////  rebinning ////////////////
	//////////////////////////////////////////
	int rebinInt=10;
	TH1D* cftFragsRebin = (TH1D*)scaledCftFrags->Clone("cftFragsRebin");
	cftFragsRebin->Rebin(rebinInt);
	TH1D* noMaskFragsRebin = (TH1D*)noMaskFrags->Clone("noMaskFragsRebin");
	noMaskFragsRebin->Rebin(rebinInt);

	TH1D* cftEfficiency = (TH1D*)cftFragsRebin->Clone("cftEfficiency");
	cftEfficiency->SetTitle("cftEfficiency = cft / noMask");
	cftEfficiency->Divide(noMaskFragsRebin);

	////////////////////////////////////////////
	//////////////  count frags ////////////////
	////////////////////////////////////////////
	int cftFragIntegral=cftFragsRebin->Integral();
	int cftIntegral=cftLvEn->Integral();
	int noMaskFragIntegral=noMaskFragsRebin->Integral();
	int noMaskIntegral=noMaskLvEn->Integral();
	int cftNotFragIntegral=cftIntegral - cftFragIntegral;
	int noMaskNotFragIntegral=noMaskIntegral - noMaskFragIntegral;
	double noMask_aSF_ratio=2*noMaskNotFragIntegral/(double)noMaskFragIntegral;
	double cft_aSF_ratio=2*cftNotFragIntegral/(double)cftFragIntegral;
	char cftFragChar[123], noMaskFragChar[123],
		noMask_aSF_ratioChar[123],cft_aSF_ratioChar[123],
		noMaskNotFragCutChar[123],cftNotFragCutChar[123],
		cftEffChar[123];
	sprintf(cftEffChar,"fragRatio = %.3f",noMaskFragIntegral/(double)cftFragIntegral);
	sprintf(cft_aSF_ratioChar,"cft_aSF_ratio = %.3f",cft_aSF_ratio);
	sprintf(noMask_aSF_ratioChar,"noMask_aSF_ratio = %.3f",noMask_aSF_ratio);
	sprintf(cftNotFragCutChar,"cftNotFragIntegral = %d",cftNotFragIntegral);
	sprintf(noMaskNotFragCutChar,"noMaskNotFragIntegral = %d",noMaskNotFragIntegral);
	sprintf(cftFragChar,"nFragsCFT = %d",cftFragIntegral);
	sprintf(noMaskFragChar,"nFragsNoMask = %d",noMaskFragIntegral);

	TText fragCountText;
	fragCountText.SetTextAlign(13);
	fragCountText.SetTextSize(0.03);
	fragCountText.SetTextFont(42);

	///////////////////////////////////////////////////////////////////
	//////////////  build phi inset to guide the eye ////////////////
	///////////////////////////////////////////////////////////////////
	double origin=0.5;
	TLine *lMin = new TLine(origin,origin,0.5*TMath::Cos(radPhiMin -TMath::Pi())+origin,0.5*TMath::Sin(radPhiMin -TMath::Pi())+origin);
	lMin->SetLineColor(3);
	lMin->SetLineWidth(2);
	TLine *lMax = new TLine(origin,origin,0.5*TMath::Cos(radPhiMax -TMath::Pi())+origin,0.5*TMath::Sin(radPhiMax -TMath::Pi())+origin);
	lMax->SetLineColor(3);
	lMax->SetLineWidth(2);
	TLine *lcath = new TLine(origin,0,origin,2*origin);
	lcath->SetLineColor(2);
	TLine *lano = new TLine(0,origin,2*origin,origin);
	lano->SetLineColor(kBlack);
	TMarker *m = new TMarker();
	m->SetMarkerStyle(20);
	m->SetMarkerColor(kBlue);
	TLatex *t0deg = new TLatex(2*origin-0.1,origin-0.1,"#scale[2]{0^{o}}");
	TLatex *t90deg = new TLatex(origin-0.1,2*origin-0.1,"#scale[2]{90^{o}}");
	TLatex *t180deg = new TLatex(0.1,origin-0.1,"#scale[2]{180^{o}}");
	TLatex *t270deg = new TLatex(origin-0.1,0.1,"#scale[2]{270^{o}}");


	//////////////////////////////////////////////////////////////////
	//////////////////// plot fragment cut projection ////////////////
	//////////////////////////////////////////////////////////////////
	c1->SetLogx(1);
	c1->SetLogy(1);
	cftFragsRebin->GetXaxis()->SetRangeUser(1,150);
	cftFragsRebin->GetYaxis()->SetRangeUser(0.1,10000);
	cftFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.log.cft.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->GetXaxis()->SetRangeUser(1,150);
	noMaskFragsRebin->GetYaxis()->SetRangeUser(0.1,10000);
	noMaskFragsRebin->SetMaximum(noMaskFragsRebin->GetMaximum());
	noMaskFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.log.noMask.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->SetLogx(0);
	cftFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.lin.cft.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	cftFragsRebin->GetXaxis()->SetRangeUser(1,20);
	cftFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.lowEn.cft.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.lin.noMask.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->GetXaxis()->SetRangeUser(1,20);
	noMaskFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.lowEn.noMask.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	TLegend *legend1=new TLegend(0.4,0.8,0.6,0.89);
	legend1->SetTextFont(42);
	legend1->SetTextSize(0.03);
	legend1->SetFillColor(kWhite);
	legend1->AddEntry(noMaskFragsRebin,"No Trigger");
	legend1->AddEntry(cftFragsRebin,"Fission Trigger");

	//////////////////////////////////////////////////////////////////
	//////////////////// cft & noMask fragment same plot /////////////
	//////////////////////////////////////////////////////////////////

	// for plotting the scaled region
	TH1D* cftFragsScaleRegion = (TH1D*)scaledCftFrags->Clone("cftFragsScaleRegion");
	for (int i = 0; i < 5000; ++i) cftFragsScaleRegion->SetBinContent(i,0);
	cftFragsScaleRegion->Rebin(rebinInt);
	cftFragsScaleRegion->SetFillColor(9);

	// cft & noMask fragment same plot
	c1->SetLogx(1);
	c1->SetLogy(1);
	noMaskFragsRebin->GetXaxis()->SetRangeUser(1,150);
	noMaskFragsRebin->GetYaxis()->SetRangeUser(0.1,10000);
	cftFragsRebin->GetXaxis()->SetRangeUser(1,150);
	cftFragsRebin->GetYaxis()->SetRangeUser(0.1,10000);
	noMaskFragsRebin->Draw();
	noMaskFragsRebin->SetTitle("fragCut [Tirgger vs No Trigger]");
	cftFragsRebin->Draw("same");
	legend1->Draw("same");
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	TPad *inset_pad0 = new TPad ("inset_pad0", "inset_pad0", 0.7, 0.6, 0.9,0.9);
	inset_pad0->SetFillStyle(4000);
	inset_pad0->Draw();
	inset_pad0->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"bothFragsCut.logxy.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	cftFragsScaleRegion->Draw("same");
	sprintf(charContainer,"bothFragsCut.logxy.showScalingRegion.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->SetLogx(0);
	noMaskFragsRebin->SetMinimum(1);
	noMaskFragsRebin->GetXaxis()->SetRangeUser(0,150);
	cftFragsRebin->GetXaxis()->SetRangeUser(0,150);
	noMaskFragsRebin->Draw();
	cftFragsRebin->Draw("same");
	legend1->Draw("same");
	TPad *inset_pad1 = new TPad ("inset_pad1", "inset_pad1", 0.7, 0.6, 0.9,0.9);
	inset_pad1->SetFillStyle(4000);
	inset_pad1->Draw();
	inset_pad1->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"bothFragsCut.lin.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	cftFragsScaleRegion->Draw("same");
	sprintf(charContainer,"bothFragsCut.lin.showScalingRegion.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	TLegend *legendCFTefficiency=new TLegend(0.4,0.75,0.6,0.89);
	legendCFTefficiency->SetTextFont(42);
	legendCFTefficiency->SetTextSize(0.03);
	legendCFTefficiency->SetFillColor(kWhite);
	legendCFTefficiency->AddEntry(noMaskFragsRebin,"No Trigger");
	legendCFTefficiency->AddEntry(cftFragsRebin,"Fission Trigger");
	legendCFTefficiency->AddEntry(cftEfficiency,"CFT Efficiency");

	// cft efficiency -> noMask / cft fragments
	c1->SetGridx(1);
	c1->SetGridy(1);
	c1->SetLogx(0);
	c1->SetLogy(0);
	noMaskFragsRebin->GetXaxis()->SetRangeUser(1,20);
	cftFragsRebin->GetXaxis()->SetRangeUser(1,20);
	cftEfficiency->GetXaxis()->SetRangeUser(1,20);
	cftEfficiency->GetYaxis()->SetRangeUser(0,2);
	cftEfficiency->SetLineColor(6);
	cftEfficiency->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	TPad *inset_pad2 = new TPad ("inset_pad2", "inset_pad2", 0.7, 0.6, 0.9,0.9);
	inset_pad2->SetFillStyle(4000);
	inset_pad2->Draw();
	inset_pad2->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"cftEfficiency.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	noMaskFragsRebin->GetYaxis()->SetRangeUser(0,400);
	cftFragsRebin->GetYaxis()->SetRangeUser(0,400);
	noMaskFragsRebin->Draw();
	cftFragsRebin->Draw("same");
	legend1->Draw("same");
	TPad *inset_pad3 = new TPad ("inset_pad3", "inset_pad3", 0.7, 0.6, 0.9,0.9);
	inset_pad3->SetFillStyle(4000);
	inset_pad3->Draw();
	inset_pad3->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"cftEfficiencyWithFrags.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->SetLogy(1);
	cftEfficiency->GetXaxis()->SetRangeUser(0,20);
	cftEfficiency->GetYaxis()->SetRangeUser(0.01,10);
	cftEfficiency->SetLineColor(6);
	cftEfficiency->Draw();
	TPad *inset_pad4 = new TPad ("inset_pad4", "inset_pad4", 0.7, 0.6, 0.9,0.9);
	inset_pad4->SetFillStyle(4000);
	inset_pad4->Draw();
	inset_pad4->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"cftEfficiency.lowEn.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->GetYaxis()->SetRangeUser(0.001,8000);
	cftFragsRebin->GetYaxis()->SetRangeUser(0.001,8000);
	cftEfficiency->GetYaxis()->SetRangeUser(0.001,8000);
	noMaskFragsRebin->Draw();
	cftFragsRebin->Draw("same");
	cftEfficiency->Draw("same");
	legendCFTefficiency->Draw("same");
	TPad *inset_pad5 = new TPad ("inset_pad5", "inset_pad5", 0.7, 0.6, 0.9,0.9);
	inset_pad5->SetFillStyle(4000);
	inset_pad5->Draw();
	inset_pad5->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"bothFragsCut.zoom.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	// ///////////////////////////////////////////////////////////////////
	// ///////////////////////  Plot lenght VS ADC ///////////////////////
	// ///////////////////////////////////////////////////////////////////

	c1->cd();
	c1->SetGridy(0);
	c1->SetGridx(0);
	c1->SetLogy(0);
	c1->SetLogx(1);
	c1->SetLogz(1);
	cftLvEn->GetZaxis()->SetRangeUser(1,1000);
	noMaskLvEn->GetZaxis()->SetRangeUser(1,1000);

	cftLvEn->GetXaxis()->SetRangeUser(0.1,150);
	cftLvEn->GetYaxis()->SetRangeUser(0,10);
	cftLvEn->Draw("colz");
	TPad *inset_pad6 = new TPad ("inset_pad6", "inset_pad6", 0.7, 0.6, 0.9,0.9);
	inset_pad6->SetFillStyle(4000);
	inset_pad6->Draw();
	inset_pad6->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"cftLvEn.full.log.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	cftLvEn->GetYaxis()->SetRangeUser(0,7);
	sprintf(charContainer,"cftLvEn.full.log.zoom.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->cd();
	c1->SetLogx(0);
	cftLvEn->GetXaxis()->SetRangeUser(0,150);
	cftLvEn->GetYaxis()->SetRangeUser(0,7);
	cftLvEn->Draw("Colz");
	// fragCut->Draw("same");
	TPad *inset_pad7 = new TPad ("inset_pad7", "inset_pad7", 0.7, 0.6, 0.9,0.9);
	inset_pad7->SetFillStyle(4000);
	inset_pad7->Draw();
	inset_pad7->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"cftLvEn.full.lin.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.63,cft_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.66,cftFragChar);
	fragCountText.DrawTextNDC(0.5,0.69,cftNotFragCutChar);
	sprintf(charContainer,"cftLvEn.full.lin.plusFragCut.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");


	c1->cd();
	c1->SetLogx(0);
	cftLvEn->GetXaxis()->SetRangeUser(1,20);
	cftLvEn->Draw("Colz");
	fragCut->Draw("same");
	TPad *inset_pad8 = new TPad ("inset_pad8", "inset_pad8", 0.7, 0.6, 0.9,0.9);
	inset_pad8->SetFillStyle(4000);
	inset_pad8->Draw();
	inset_pad8->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"cftLvEn.lowEn.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.43,cft_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.46,cftFragChar);
	fragCountText.DrawTextNDC(0.5,0.49,cftNotFragCutChar);
	sprintf(charContainer,"cftLvEn.lowEn.plusFragCut.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->cd();
	c1->SetLogx(1);
	noMaskLvEn->GetXaxis()->SetRangeUser(0.1,150);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,10);
	noMaskLvEn->Draw("colz");
	TPad *inset_pad9 = new TPad ("inset_pad9", "inset_pad9", 0.7, 0.6, 0.9,0.9);
	inset_pad9->SetFillStyle(4000);
	inset_pad9->Draw();
	inset_pad9->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"noMaskLvEn.full.log.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	sprintf(charContainer,"noMaskLvEn.full.log.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	noMaskLvEn->GetYaxis()->SetRangeUser(0,7);
	sprintf(charContainer,"noMaskLvEn.full.log.zoom.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	c1->SetLogx(0);
	sprintf(charContainer,"noMaskLvEn.full.lin.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->cd();
	c1->SetLogx(0);
	noMaskLvEn->GetXaxis()->SetRangeUser(0,150);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,7);
	noMaskLvEn->Draw("Colz");
	TPad *inset_pad10 = new TPad ("inset_pad10", "inset_pad10", 0.7, 0.6, 0.9,0.9);
	inset_pad10->SetFillStyle(4000);
	inset_pad10->Draw();
	inset_pad10->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"noMaskLvEn.full.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.63,noMask_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.66,noMaskFragChar);
	fragCountText.DrawTextNDC(0.5,0.69,noMaskNotFragCutChar);
	sprintf(charContainer,"noMaskLvEn.full.lin.plusFragCut.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	c1->cd();
	c1->SetLogx(0);
	noMaskLvEn->GetXaxis()->SetRangeUser(0,20);
	noMaskLvEn->GetYaxis()->SetRangeUser(0,7);
	noMaskLvEn->Draw("Colz");
	TPad *inset_pad11 = new TPad ("inset_pad11", "inset_pad11", 0.7, 0.6, 0.9,0.9);
	inset_pad11->SetFillStyle(4000);
	inset_pad11->Draw();
	inset_pad11->cd();
	m->DrawMarker(origin,origin);
	lMin->Draw();
	lMax->Draw();
	lcath->Draw();
	lano->Draw();
	t0deg->Draw("same");
	t90deg->Draw("same");
	t180deg->Draw("same");
	t270deg->Draw("same");
	c1->cd();
	sprintf(charContainer,"noMaskLvEn.lowEn.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.43,noMask_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.46,noMaskFragChar);
	fragCountText.DrawTextNDC(0.5,0.49,noMaskNotFragCutChar);
	sprintf(charContainer,"noMaskLvEn.lowEn.plusFragCut.phiMin%03i.phiMax%03i.png",degPhiMin,degPhiMax);
	c1->SaveAs(charContainer,"recreate");

	delete noMaskLvEn;
	delete cftLvEn;

	}



}
