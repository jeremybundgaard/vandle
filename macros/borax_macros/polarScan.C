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
#include "TMarker.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TText.h"
#include "TStyle.h"

#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>


void polarScan(const int preamp, const char* noMaskFileName, const char* cftFileName, const int thetaLoopMin, const int thetaLoopMax, const int thetaLoopStep) {


TCutG *fragCut = new TCutG("fragCut",8);
fragCut->SetLineColor(kViolet);
fragCut->SetLineWidth(3);
if (preamp==1)
{
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
else if (preamp==2)
{
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

TFile* cftFile = new TFile(cftFileName);
TFile* noMaskFile = new TFile(noMaskFileName);

TTree* noMaskTracks = (TTree*)noMaskFile->Get("tracks");
TTree* cftTracks = (TTree*)cftFile->Get("tracks");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
gStyle->SetOptStat(0);

//// polar loop
std::stringstream thetaCut;
double radThetaMin=0,radThetaMax=0,piLessradThetaMin=0,piLessradThetaMax=0,origin=0;
int degThetaMin=0,degThetaMax=0;
char charContainer[123];


for (int theta = thetaLoopMin; theta < thetaLoopMax; theta+=thetaLoopStep){

	// theta cut
	double deg2rad = TMath::Pi()/(double)180;
	degThetaMin=theta;
	degThetaMax=theta+10;
	if (degThetaMax > 90) continue;
	radThetaMin=degThetaMin*deg2rad;
	radThetaMax=degThetaMax*deg2rad;
	piLessradThetaMin=( 180 - degThetaMax )*deg2rad;
	piLessradThetaMax=( 180 - degThetaMin )*deg2rad;

	thetaCut.str("");
    thetaCut<<"(("<< radThetaMin<<" < direction.Theta() && direction.Theta() < "<<radThetaMax
    <<") || ("<< piLessradThetaMin<<" < direction.Theta() && direction.Theta() < "
    <<piLessradThetaMax<<")) && bestFit";

	std::cout << thetaCut.str().c_str() << std::endl;

	sprintf(charContainer,"noMaskLvEnThetaMin%03i",degThetaMin);
	TH2D* noMaskLvEn = new TH2D("noMaskLvEn",charContainer,40000,0,168.6,1000,0,10);

	sprintf(charContainer,"cftLvEnThetaMin%03i",degThetaMin);
	TH2D* cftLvEn = new TH2D("cftLvEn",charContainer,40000,0,168.6,1000,0,10);

	cftLvEn->GetXaxis()->SetTitle("Track Energy [MeV]");
	cftLvEn->GetXaxis()->CenterTitle();
	cftLvEn->GetYaxis()->SetTitle("Track Length [cm]");
	cftLvEn->GetYaxis()->CenterTitle();
	cftLvEn->SetTitle(thetaCut.str().c_str());
	cftTracks->Draw("length:adc*168.6/40000>>cftLvEn",thetaCut.str().c_str());

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
	noMaskLvEn->SetTitle(thetaCut.str().c_str());
	noMaskTracks->Draw("length:adc*168.6/40000>>noMaskLvEn",thetaCut.str().c_str());

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
	//////////////  build theta inset to guide the eye ////////////////
	///////////////////////////////////////////////////////////////////
	double origin=0.5;
	TLine *lMin = new TLine(origin,origin,0.5*TMath::Cos(radThetaMin)+origin,0.5*TMath::Sin(radThetaMin)+origin);
	lMin->SetLineColor(3);
	lMin->SetLineWidth(2);
	TLine *lMax = new TLine(origin,origin,0.5*TMath::Cos(radThetaMax)+origin,0.5*TMath::Sin(radThetaMax)+origin);
	lMax->SetLineColor(3);
	lMax->SetLineWidth(2);
	TLine *lcath = new TLine(origin,origin,origin,2*origin);
	lcath->SetLineColor(2);
	TLine *lano = new TLine(origin,origin,2*origin,origin);
	lano->SetLineColor(kBlack);
	TMarker *m = new TMarker();
	m->SetMarkerStyle(20);
	m->SetMarkerColor(kBlue);
	TLatex *t0deg = new TLatex(2*origin-0.2,origin-0.2,"#scale[2.2]{0^{o}}");
	TLatex *t90deg = new TLatex(origin-0.2,2*origin-0.2,"#scale[2.2]{90^{o}}");


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
	sprintf(charContainer,"frags.full.log.cft.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->GetXaxis()->SetRangeUser(1,150);
	noMaskFragsRebin->GetYaxis()->SetRangeUser(0.1,10000);
	noMaskFragsRebin->SetMaximum(noMaskFragsRebin->GetMaximum());
	noMaskFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.log.noMask.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");

	c1->SetLogx(0);
	cftFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.lin.cft.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");

	cftFragsRebin->GetXaxis()->SetRangeUser(1,20);
	cftFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.lowEn.cft.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.full.lin.noMask.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");

	noMaskFragsRebin->GetXaxis()->SetRangeUser(1,20);
	noMaskFragsRebin->Draw();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"frags.lowEn.noMask.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"bothFragsCut.logxy.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	cftFragsScaleRegion->Draw("same");
	sprintf(charContainer,"bothFragsCut.logxy.showScalingRegion.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"bothFragsCut.lin.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	cftFragsScaleRegion->Draw("same");
	sprintf(charContainer,"bothFragsCut.lin.showScalingRegion.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"cftEfficiency.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"cftEfficiencyWithFrags.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"cftEfficiency.lowEn.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	fragCountText.DrawTextNDC(0.15,0.83,cftEffChar);
	fragCountText.DrawTextNDC(0.15,0.86,noMaskFragChar);
	fragCountText.DrawTextNDC(0.15,0.89,cftFragChar);
	sprintf(charContainer,"bothFragsCut.zoom.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"cftLvEn.full.log.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	cftLvEn->GetYaxis()->SetRangeUser(0,7);
	sprintf(charContainer,"cftLvEn.full.log.zoom.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"cftLvEn.full.lin.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.63,cft_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.66,cftFragChar);
	fragCountText.DrawTextNDC(0.5,0.69,cftNotFragCutChar);
	sprintf(charContainer,"cftLvEn.full.lin.plusFragCut.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"cftLvEn.lowEn.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.43,cft_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.46,cftFragChar);
	fragCountText.DrawTextNDC(0.5,0.49,cftNotFragCutChar);
	sprintf(charContainer,"cftLvEn.lowEn.plusFragCut.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"noMaskLvEn.full.log.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	sprintf(charContainer,"noMaskLvEn.full.log.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	noMaskLvEn->GetYaxis()->SetRangeUser(0,7);
	sprintf(charContainer,"noMaskLvEn.full.log.zoom.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	c1->SetLogx(0);
	sprintf(charContainer,"noMaskLvEn.full.lin.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"noMaskLvEn.full.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.63,noMask_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.66,noMaskFragChar);
	fragCountText.DrawTextNDC(0.5,0.69,noMaskNotFragCutChar);
	sprintf(charContainer,"noMaskLvEn.full.lin.plusFragCut.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
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
	c1->cd();
	sprintf(charContainer,"noMaskLvEn.lowEn.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");
	fragCut->Draw("same");
	fragCountText.DrawTextNDC(0.5,0.43,noMask_aSF_ratioChar);
	fragCountText.DrawTextNDC(0.5,0.46,noMaskFragChar);
	fragCountText.DrawTextNDC(0.5,0.49,noMaskNotFragCutChar);
	sprintf(charContainer,"noMaskLvEn.lowEn.plusFragCut.thetaMin%03i.thetaMax%03i.png",degThetaMin,degThetaMax);
	c1->SaveAs(charContainer,"recreate");

	delete noMaskLvEn;
	delete cftLvEn;

	}



}
