
#include "TROOT.h"
#include "TRint.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"

void niffteTreePlots(const char* infile) {

TFile* trackFile = new TFile(infile);
TTree* tracks = (TTree*)trackFile->Get("tracks");
char trackCut[345],histName[345];
sprintf(trackCut,"fitType==5 && run!=100001995");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
gStyle->SetOptStat(0);

c1->SetLogz(1);
TH2D* lVsEn = new TH2D("lVsEn","lVsEn",10000,0.3,180,200,0,10);
lVsEn->GetXaxis()->SetTitle("energy [MeV]");
lVsEn->GetYaxis()->SetTitle("length [cm]");
sprintf(histName,"lVsEn{%s}",trackCut);
lVsEn->SetTitle(histName);
tracks->Draw("length:adc/200>>lVsEn",trackCut,"colz");
c1->SaveAs("lVsEn.png");
c1->SetLogx(1);
lVsEn->Draw("colz");
c1->SaveAs("lVsEn.logx.png");
c1->SetLogx(0);

c1->SetLogy(1);
TH2D* enVsAz = new TH2D("enVsAz","enVsAz",200,-3.2,3.2,10000,0.3,180); 
enVsAz->GetXaxis()->SetTitle("#phi [radians]");
enVsAz->GetYaxis()->SetTitle("energy [MeV]");
sprintf(histName,"enVsAz{%s}",trackCut);
enVsAz->SetTitle(histName);
enVsAz->GetYaxis()->SetRangeUser(0.1,170);
tracks->Draw("adc/200:direction.Phi()>>enVsAz",trackCut,"colz");
c1->SaveAs("enVsAz.png");

TH2D* enVsPolar = new TH2D("enVsPolar","enVsPolar",200,0,3.2,10000,0.3,180);
enVsPolar->GetXaxis()->SetTitle("#theta [radians]");
enVsPolar->GetYaxis()->SetTitle("energy [MeV]");
sprintf(histName,"enVsPolar{%s}",trackCut);
enVsPolar->SetTitle(histName);
enVsPolar->GetYaxis()->SetRangeUser(0.1,170);
tracks->Draw("adc/200:direction.Theta()>>enVsPolar",trackCut,"colz");
c1->SaveAs("enVsPolar.png");

TH2D* enVsCosPolar = new TH2D("enVsCosPolar","enVsCosPolar",200,-1,1,10000,0.3,180);
enVsCosPolar->GetXaxis()->SetTitle("cos(#theta)");
enVsCosPolar->GetYaxis()->SetTitle("energy [MeV]");
sprintf(histName,"enVsCosPolar{%s}",trackCut);
enVsCosPolar->SetTitle(histName);
enVsCosPolar->GetYaxis()->SetRangeUser(0.1,170);	
tracks->Draw("adc/200:direction.CosTheta()>>enVsCosPolar",trackCut,"colz");
c1->SaveAs("enVsCosPolar.png");
c1->SetLogy(0);

TH2D* xyStart = new TH2D("xyStart","xyStart",1000,-7,7,1000,-7,7);
xyStart->GetXaxis()->SetTitle("x [cm]");
xyStart->GetYaxis()->SetTitle("y [cm]");
sprintf(histName,"xyStart{%s}",trackCut);
xyStart->SetTitle(histName);
tracks->Draw("start.x():start.y()>>xyStart",trackCut,"colz");
c1->SaveAs("xyStart.png");

TH2D* xyEnd = new TH2D("xyEnd","xyEnd",1000,-7,7,1000,-7,7);
xyEnd->GetXaxis()->SetTitle("x [cm]");
xyEnd->GetYaxis()->SetTitle("y [cm]");
sprintf(histName,"xyEnd{%s}",trackCut);
xyEnd->SetTitle(histName);
tracks->Draw("end.x():end.y()>>xyEnd",trackCut,"colz");
c1->SaveAs("xyEnd.png");

TH1D* en = new TH1D("en","en",10000,0.3,180);
en->GetXaxis()->SetTitle("energy [MeV]");
tracks->Draw("adc/200>>en","fitType==5","");
c1->SaveAs("en.png");
c1->SetLogy(1);
en->Draw("colz");
c1->SaveAs("en.log.png");
c1->SetLogy(0);

TH1D* length = new TH1D("length","length",200,0,10);
length->GetXaxis()->SetTitle("length [cm]");
tracks->Draw("length>>length",trackCut,"");
c1->SaveAs("length.png");
c1->SetLogy(1);
length->Draw("colz");
c1->SaveAs("length.log.png");
c1->SetLogy(0);

TH1D* az = new TH1D("az","az",200,-3.2,3.2);
az->GetXaxis()->SetTitle("#phi [radians]");
tracks->Draw("direction.Phi()>>az",trackCut,"");
c1->SaveAs("az.png");

TH1D* cosPolar = new TH1D("cosPolar","cosPolar",200,-1,1);
cosPolar->GetXaxis()->SetTitle("cos(#theta)");
tracks->Draw("direction.CosTheta()>>cosPolar",trackCut,"");
c1->SaveAs("cosPolar.png");

TH1D* polar = new TH1D("polar","polar",200,0,3.2);
polar->GetXaxis()->SetTitle("#theta [radians]");
tracks->Draw("direction.Theta()>>polar",trackCut,"");
c1->SaveAs("polar.png");


}
