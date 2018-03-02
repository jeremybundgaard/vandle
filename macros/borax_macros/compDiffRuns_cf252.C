#include "TRint.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCut.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"

#include <iomanip>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>


void compDiffRuns_cf252(const char* runType, const char* treeFileName) {


std::vector<int> runNum;
char trackCut[123];
char histName[123];
char plotName[123];


if ( !(strcmp(runType,"cfCFT1")))
{
	std::cout<<runType<<'\t'<<treeFileName<<endl;
	runNum.push_back(1865);
	runNum.push_back(1873);
	runNum.push_back(1883);

}
else if ( !(strcmp(runType,"cfCFT2")))
{
	std::cout<<runType<<'\t'<<treeFileName<<endl;

	runNum.push_back(2068);
	runNum.push_back(2069);
}
else if ( !(strcmp(runType,"cfNoMask1")))
{
	std::cout<<runType<<'\t'<<treeFileName<<endl;

	runNum.push_back(1850);
	runNum.push_back(1851);
	runNum.push_back(1852);
	runNum.push_back(1853);
	runNum.push_back(1854);
	runNum.push_back(1855);
	runNum.push_back(1856);
	runNum.push_back(1866);
	runNum.push_back(1867);
	runNum.push_back(1868);
	runNum.push_back(1869);
	runNum.push_back(1870);
	runNum.push_back(1871);
	runNum.push_back(1872);
	runNum.push_back(1872);
	runNum.push_back(1874);
	runNum.push_back(1875);
	runNum.push_back(1876);
	runNum.push_back(1877);
	runNum.push_back(1878);
	runNum.push_back(1879);
	runNum.push_back(1880);
	runNum.push_back(1881);
	runNum.push_back(1882);

}
else if ( !(strcmp(runType,"cfNoMask2")))
{
	std::cout<<runType<<'\t'<<treeFileName<<endl;
	 
	runNum.push_back(2055);
	runNum.push_back(2056);
	runNum.push_back(2057);
	runNum.push_back(2058);
	runNum.push_back(2059);
	runNum.push_back(2060);
	runNum.push_back(2061);
}


TFile* treeFile = new TFile(treeFileName);
TTree* tracks = (TTree*)treeFile->Get("tracks");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
gStyle->SetOptStat(0);


for (std::vector<int>::iterator runNumIter = runNum.begin(); runNumIter != runNum.end(); ++runNumIter)
{

sprintf(trackCut,"fitType==5 && run == 10000%d",*runNumIter);
cout<<trackCut<<endl;

c1->SetLogz(1);
TH2D* lVsEn = new TH2D("lVsEn","lVsEn",10000,0.3,180,200,0,10);
lVsEn->GetXaxis()->SetTitle("energy [MeV]");
lVsEn->GetYaxis()->SetTitle("length [cm]");
sprintf(histName,"lVsEn{%s}",trackCut);
lVsEn->SetTitle(histName);
tracks->Draw("length:adc/200>>lVsEn",trackCut,"colz");
sprintf(plotName,"lVsEn.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogx(1);
lVsEn->Draw("colz");
sprintf(plotName,"lVsEn.logx.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogx(0);

c1->SetLogy(1);
TH2D* enVsAz = new TH2D("enVsAz","enVsAz",200,-3.2,3.2,10000,0.3,180); 
enVsAz->GetXaxis()->SetTitle("#phi [radians]");
enVsAz->GetYaxis()->SetTitle("energy [MeV]");
sprintf(histName,"enVsAz{%s}",trackCut);
enVsAz->SetTitle(histName);
enVsAz->GetYaxis()->SetRangeUser(0.1,170);
tracks->Draw("adc/200:direction.Phi()>>enVsAz",trackCut,"colz");
sprintf(plotName,"enVsAz.%d.png",*runNumIter);
c1->SaveAs(plotName);

TH2D* enVsPolar = new TH2D("enVsPolar","enVsPolar",200,0,3.2,10000,0.3,180);
enVsPolar->GetXaxis()->SetTitle("#theta [radians]");
enVsPolar->GetYaxis()->SetTitle("energy [MeV]");
sprintf(histName,"enVsPolar{%s}",trackCut);
enVsPolar->SetTitle(histName);
enVsPolar->GetYaxis()->SetRangeUser(0.1,170);
tracks->Draw("adc/200:direction.Theta()>>enVsPolar",trackCut,"colz");
sprintf(plotName,"enVsPolar.%d.png",*runNumIter);
c1->SaveAs(plotName);

TH2D* enVsCosPolar = new TH2D("enVsCosPolar","enVsCosPolar",200,-1,1,10000,0.3,180);
enVsCosPolar->GetXaxis()->SetTitle("cos(#theta)");
enVsCosPolar->GetYaxis()->SetTitle("energy [MeV]");
sprintf(histName,"enVsCosPolar{%s}",trackCut);
enVsCosPolar->SetTitle(histName);
enVsCosPolar->GetYaxis()->SetRangeUser(0.1,170);	
tracks->Draw("adc/200:direction.CosTheta()>>enVsCosPolar",trackCut,"colz");
sprintf(plotName,"enVsCosPolar.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogy(0);

TH2D* xyStart = new TH2D("xyStart","xyStart",1000,-7,7,1000,-7,7);
xyStart->GetXaxis()->SetTitle("x [cm]");
xyStart->GetYaxis()->SetTitle("y [cm]");
sprintf(histName,"xyStart{%s}",trackCut);
xyStart->SetTitle(histName);
tracks->Draw("start.x():start.y()>>xyStart",trackCut,"colz");
sprintf(plotName,"xyStart.%d.png",*runNumIter);
c1->SaveAs(plotName);

TH2D* xyEnd = new TH2D("xyEnd","xyEnd",1000,-7,7,1000,-7,7);
xyEnd->GetXaxis()->SetTitle("x [cm]");
xyEnd->GetYaxis()->SetTitle("y [cm]");
sprintf(histName,"xyEnd{%s}",trackCut);
xyEnd->SetTitle(histName);
tracks->Draw("end.x():end.y()>>xyEnd",trackCut,"colz");
sprintf(plotName,"xyEnd.%d.png",*runNumIter);
c1->SaveAs(plotName);

TH1D* en = new TH1D("en","en",10000,0.3,180);
en->GetXaxis()->SetTitle("energy [MeV]");
tracks->Draw("adc/200>>en","fitType==5","");
sprintf(plotName,"en.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogy(1);
en->Draw("colz");
sprintf(plotName,"en.log.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogy(0);

TH1D* length = new TH1D("length","length",200,0,10);
length->GetXaxis()->SetTitle("length [cm]");
tracks->Draw("length>>length",trackCut,"");
sprintf(plotName,"length.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogy(1);
length->Draw("colz");
sprintf(plotName,"length.log.%d.png",*runNumIter);
c1->SaveAs(plotName);
c1->SetLogy(0);

TH1D* az = new TH1D("az","az",200,-3.2,3.2);
az->GetXaxis()->SetTitle("#phi [radians]");
tracks->Draw("direction.Phi()>>az",trackCut,"");
sprintf(plotName,"az.%d.png",*runNumIter);
c1->SaveAs(plotName);

TH1D* cosPolar = new TH1D("cosPolar","cosPolar",200,-1,1);
cosPolar->GetXaxis()->SetTitle("cos(#theta)");
tracks->Draw("direction.CosTheta()>>cosPolar",trackCut,"");
sprintf(plotName,"cosPolar.%d.png",*runNumIter);
c1->SaveAs(plotName);

TH1D* polar = new TH1D("polar","polar",200,0,3.2);
polar->GetXaxis()->SetTitle("#theta [radians]");
tracks->Draw("direction.Theta()>>polar",trackCut,"");
sprintf(plotName,"polar.%d.png",*runNumIter);
c1->SaveAs(plotName);



}

}