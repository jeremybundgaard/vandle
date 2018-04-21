#include "TAxis.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TChain.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPaveStats.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "TProof.h"
#include "TROOT.h"
#include <climits>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>

#include <sstream>
#include <stdlib.h>
#include <vector>


void vandlePlots(const char *vandle_tree_file = "",const char *otherTreeCuts = "1",int isotope = 0) {

gStyle->SetOptStat("ne");
gStyle->SetTitleOffset(1.2,"Y");
gStyle->SetNumberContours(99);

///////// initialize track tree /////////
printf("\n///////// initialize vandle_tree /////////\n\n");
TFile *treefile = new TFile(vandle_tree_file);
TTree* vandle_tree = (TTree*)treefile->Get("vandle_tree");
TTree* vandle_tree_NEUTRON;

// TTree* vandle_treeree = (TTree*)treefile->Get("vandle_tree");
// TChain * vandle_tree = new TChain("vandle_tree");
// vandle_tree->Add(vandle_tree_file);
// TProof * pr;
// pr = TProof::Open("lite://","workers=16");
// vandle_tree->SetProof();
//
// gROOT->cd();
// TFile* dummyfile = new TFile("dummy.root", "RECREATE");
// dummyfile->cd();
// TTree* vandle_treeree_NEUTRON = (TTree*)vandle_tree->CopyTree("tcutg_NEUTRON");
// // vandle_treeree_NEUTRON->SetLineColor(kRed);
// dummyfile->Write();
// TChain * vandle_tree_NEUTRON = new TChain("vandle_tree");
// vandle_tree_NEUTRON->Add("dummy.root");
// vandle_tree_NEUTRON->SetProof();

char isotopeChar[345]="";
sprintf(isotopeChar,"%dRb",isotope);
unsigned int evtNumber=0;
unsigned int vandle_bar=0;
double vandle_QDC=0;
double vandle_TOF=0;
double vandle_TDiff=0;
int runNumber=0;

vandle_tree->SetBranchAddress("vandle_bar",&vandle_bar);
vandle_tree->SetBranchAddress("vandle_QDC",&vandle_QDC);
vandle_tree->SetBranchAddress("vandle_TOF",&vandle_TOF);
vandle_tree->SetBranchAddress("vandle_TDiff",&vandle_TDiff);
vandle_tree->SetBranchAddress("runNumber",&runNumber);

char buffChar[345]="";
char cutChar[345]="-6<vandle_TDiff&&vandle_TDiff<4";
char drawChar[345]="";
char NEUTRON_TOF_char[345];

gStyle->SetOptStat("ne");
gStyle->SetOptFit(1111);

TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700); //c1->SetLogz();

int tofShift=0;
c1->SetLogy(1);
TH1D* GAMMA_TOF = new TH1D("GAMMA_TOF","GAMMA_TOF",850,-50,750);
GAMMA_TOF->SetLineColor(kGreen);
GAMMA_TOF->SetLineWidth(3);
sprintf(buffChar,"-5<vandle_TOF&&vandle_TOF<30&&3000<vandle_QDC&&vandle_QDC<10000&&%s",cutChar);
vandle_tree->Draw("vandle_TOF>>GAMMA_TOF",buffChar,"colz");
sprintf(buffChar,"%f",round(GAMMA_TOF->GetMean()));
tofShift=atoi(buffChar);


int minTOF_neutron=40;
int maxTOF_neutron=450;
int minTOF_neutron_background=maxTOF_neutron;
int maxTOF_neutron_background=700;
int minQDC=30;
int maxQDC=25000;

TCutG *tcutg_NEUTRON = new TCutG("tcutg_NEUTRON",3);
tcutg_NEUTRON->SetVarX("vandle_TOF");
tcutg_NEUTRON->SetVarY("vandle_QDC");
tcutg_NEUTRON->SetLineWidth(3);
tcutg_NEUTRON->SetLineColor(kRed);
tcutg_NEUTRON->SetPoint(0,minTOF_neutron,minQDC);
tcutg_NEUTRON->SetPoint(1,maxTOF_neutron,minQDC);
tcutg_NEUTRON->SetPoint(2,maxTOF_neutron,maxQDC);
tcutg_NEUTRON->SetPoint(3,minTOF_neutron,maxQDC);
tcutg_NEUTRON->SetPoint(4,minTOF_neutron,minQDC);

TCutG *tcutg_BACKGROUND = new TCutG("tcutg_BACKGROUND",3);
tcutg_BACKGROUND->SetVarX("vandle_TOF");
tcutg_BACKGROUND->SetVarY("vandle_QDC");
tcutg_BACKGROUND->SetLineWidth(3);
tcutg_BACKGROUND->SetLineStyle(5);
tcutg_BACKGROUND->SetLineColor(kBlue);
tcutg_BACKGROUND->SetPoint(0,minTOF_neutron_background,minQDC);
tcutg_BACKGROUND->SetPoint(1,maxTOF_neutron_background,minQDC);
tcutg_BACKGROUND->SetPoint(2,maxTOF_neutron_background,maxQDC);
tcutg_BACKGROUND->SetPoint(3,minTOF_neutron_background,maxQDC);
tcutg_BACKGROUND->SetPoint(4,minTOF_neutron_background,minQDC);

TCutG *tcutg_gamma_flash = new TCutG("tcutg_gamma_flash",3);
tcutg_gamma_flash->SetVarX("vandle_TOF");
tcutg_gamma_flash->SetVarY("vandle_QDC");
tcutg_gamma_flash->SetLineWidth(3);
tcutg_gamma_flash->SetLineStyle(7);
tcutg_gamma_flash->SetLineColor(kGreen);
tcutg_gamma_flash->SetPoint(0,-minTOF_neutron,minQDC);
tcutg_gamma_flash->SetPoint(1, minTOF_neutron,minQDC);
tcutg_gamma_flash->SetPoint(2, minTOF_neutron,maxQDC);
tcutg_gamma_flash->SetPoint(3,-minTOF_neutron,maxQDC);
tcutg_gamma_flash->SetPoint(4,-minTOF_neutron,minQDC);


sprintf(buffChar,"%s&&%s",tcutg_gamma_flash->GetName(),cutChar);
sprintf(drawChar,"(vandle_TOF-%d)>>GAMMA_TOF",tofShift);
vandle_tree->Draw(drawChar,buffChar,"");
TF1 *GAMMA_TOF_gaus = new TF1("GAMMA_TOF_gaus","gaus",-3,-1);
GAMMA_TOF_gaus->SetParLimits(2,-1,1);
// TF1 *GAMMA_TOF_expo = new TF1("GAMMA_TOF_expo","exp([0]*x+[1])",15,25);
TF1 *GAMMA_TOF_fit = new TF1("GAMMA_TOF_fit","exp([0]*x+[1])",10,40);
GAMMA_TOF->Fit(GAMMA_TOF_fit,"R");
GAMMA_TOF->GetXaxis()->SetRangeUser(-40,60);
GAMMA_TOF->Draw();
GAMMA_TOF_fit->DrawF1(10,40,"same");
c1->SaveAs("GAMMA_TOF_fit.png");


TH1D* neutron_BACKGROUND = new TH1D("neutron_BACKGROUND","neutron_BACKGROUND",850,-50,750);
neutron_BACKGROUND->SetLineColor(kBlue);
neutron_BACKGROUND->SetLineWidth(3);
sprintf(buffChar,"%s&&%s",tcutg_BACKGROUND->GetName(),cutChar);
sprintf(drawChar,"(vandle_TOF-%d)>>neutron_BACKGROUND",tofShift);
vandle_tree->Draw(drawChar,buffChar);
TF1 *background_TOF_fit_expo = new TF1("background_TOF_fit_expo","exp([0]*x+[1])",minTOF_neutron_background+5,maxTOF_neutron_background-5);
background_TOF_fit_expo->SetLineColor(kBlue);
background_TOF_fit_expo->SetLineWidth(2);
neutron_BACKGROUND->Fit("background_TOF_fit_expo","R");
c1->SaveAs("neutron_BACKGROUND_withFit.png");
sprintf(NEUTRON_TOF_char,"%s_NEUTRON_TOF",isotopeChar);

TH1D* NEUTRON_TOF = new TH1D("NEUTRON_TOF","NEUTRON_TOF",850,-50,750);
NEUTRON_TOF->SetStats(kFALSE);
NEUTRON_TOF->SetTitle(" QDC vs TOF [cut projections]");
NEUTRON_TOF->SetLineColor(kRed);
NEUTRON_TOF->SetLineWidth(3);
sprintf(buffChar,"%s&&%s",tcutg_NEUTRON->GetName(),cutChar);
sprintf(drawChar,"(vandle_TOF-%d)>>NEUTRON_TOF",tofShift);
vandle_tree->Draw(drawChar,buffChar,"");
// NEUTRON_TOF->GetYaxis()->SetRangeUser(NEUTRON_TOF->GetMinimum(),NEUTRON_TOF->GetMaximum()*10);
neutron_BACKGROUND->Draw("same");
GAMMA_TOF->Draw("same");
NEUTRON_TOF->Draw("same");
background_TOF_fit_expo->DrawF1(-50,850,"same");
GAMMA_TOF_fit->DrawF1(10,40,"same");
sprintf(NEUTRON_TOF_char,"%s_vandleTOF_Neutrons.png",isotopeChar);
c1->SaveAs(NEUTRON_TOF_char);

gStyle->SetOptFit(0);
c1->Clear();
GAMMA_TOF->GetXaxis()->SetRangeUser(-50,100);
GAMMA_TOF->SetTitle("gamma background");
GAMMA_TOF->Draw();
neutron_BACKGROUND->Draw("same");
NEUTRON_TOF->Draw("same");
background_TOF_fit_expo->DrawF1(-50,850,"same");
GAMMA_TOF_fit->DrawF1(10,40,"same");
c1->SaveAs("neutron_TOF_backgroundCompare.png");

TH1D* NEUTRON_TOF_fit_subtracted = (TH1D*)NEUTRON_TOF->Clone();
NEUTRON_TOF_fit_subtracted->SetLineColor(kViolet);
NEUTRON_TOF_fit_subtracted->SetTitle("neutronTOF [background subtracted]");
double bockground_sub=0;
double raw_NEUTRON_TOF=0;
for (int jj = 0; jj < NEUTRON_TOF_fit_subtracted->GetNbinsX(); jj++) {
  bockground_sub=background_TOF_fit_expo->Eval(NEUTRON_TOF_fit_subtracted->GetBinCenter(jj));
  raw_NEUTRON_TOF=NEUTRON_TOF_fit_subtracted->GetBinContent(jj) - bockground_sub;
  if(raw_NEUTRON_TOF<0) NEUTRON_TOF_fit_subtracted->SetBinContent(jj,0);
  else NEUTRON_TOF_fit_subtracted->SetBinContent(jj,raw_NEUTRON_TOF);
}
NEUTRON_TOF_fit_subtracted->GetXaxis()->SetRangeUser(-50,maxTOF_neutron+100);
NEUTRON_TOF_fit_subtracted->Draw();
c1->SaveAs("neutron_TOF.png");

c1->SetLogz(1);
TH2D* QDCvsTOF = new TH2D("QDCvsTOF","QDCvsTOF",850,-50,750,maxQDC*1.3,minQDC,maxQDC*1.3);
sprintf(buffChar,"%s_QDCvsTOF",isotopeChar);
QDCvsTOF->SetTitle(buffChar);
sprintf(drawChar,"vandle_QDC:(vandle_TOF-%d)>>QDCvsTOF",tofShift);
vandle_tree->Draw(drawChar,cutChar,"colz");
char QDCvsTOF_char[345];
sprintf(QDCvsTOF_char,"%s_vandle_QDCvsTOF.png",isotopeChar);
tcutg_NEUTRON->Draw("same");
tcutg_BACKGROUND->Draw("same");
tcutg_gamma_flash->Draw("same");
c1->SaveAs(QDCvsTOF_char);

sprintf(buffChar,
  "gm montage -mode concatenate -tile 2x2 %s %s %s %s ../%s_QDCvsTOF_neutronPlots.png",
  QDCvsTOF_char,
  "neutron_TOF.png",
  NEUTRON_TOF_char,
  "neutron_TOF_backgroundCompare.png",
  isotopeChar);
system(buffChar);

gStyle->SetOptStat(0);
char QDCvsBar_char[345];
TH2D* QDCvsBar = new TH2D("QDCvsBar","QDCvsBar",42,0,42,maxQDC*1.3,minQDC,maxQDC*1.3);
sprintf(buffChar,"%s_QDCvsBar",isotopeChar);
QDCvsBar->SetTitle(buffChar);
vandle_tree->Draw("vandle_QDC:vandle_bar>>QDCvsBar",cutChar,"colz");
sprintf(QDCvsBar_char,"%s_vandle_QDCvsBar.png",isotopeChar);
c1->SaveAs(QDCvsBar_char);

c1->SetLogy(0);

char BarvsTOF_char[345];
TH2D* BarvsTOF = new TH2D("BarvsTOF","BarvsTOF",300,-30,200,42,0,42);
sprintf(buffChar,"%s_BarvsTOF",isotopeChar);
BarvsTOF->SetTitle(buffChar);
sprintf(drawChar,"vandle_bar:(vandle_TOF-%d)>>BarvsTOF",tofShift);
vandle_tree->Draw(drawChar,cutChar,"colz");
sprintf(BarvsTOF_char,"%s_vandle_BarvsTOF.png",isotopeChar);
c1->SaveAs(BarvsTOF_char);

char BarvsTDiff_char[345];
TH2D* BarvsTDiff = new TH2D("BarvsTDiff","BarvsTDiff",1000,-1000,1000,42,0,42);
sprintf(buffChar,"%s_BarvsTDiff",isotopeChar);
vandle_tree->Draw("vandle_bar:vandle_TDiff>>BarvsTDiff","","colz");
sprintf(BarvsTDiff_char,"%s_vandle_BarvsTDiff.png",isotopeChar);
c1->SaveAs(BarvsTDiff_char);
c1->SetLogz(0);

if (0){
  TCutG *tdiff_cut = new TCutG("tdiff_cut",3);
  tdiff_cut->SetVarX("vandle_TDiff");
  tdiff_cut->SetVarY("vandle_bar");
  tdiff_cut->SetLineWidth(2);
  tdiff_cut->SetLineColor(kRed);
  tdiff_cut->SetPoint(0,-6, 0);
  tdiff_cut->SetPoint(1, 4, 0);
  tdiff_cut->SetPoint(2, 4,42);
  tdiff_cut->SetPoint(3,-6,42);
  tdiff_cut->SetPoint(4,-6, 0);
  char BarvsTDiff_zoom_char[345];
  TH2D* BarvsTDiff_zoom = new TH2D("BarvsTDiff_zoom","BarvsTDiff_zoom",100,-40,40,42,0,42);
  sprintf(BarvsTDiff_zoom_char,"%s_BarvsTDiff_zoom",isotopeChar);
  vandle_tree->Draw("vandle_bar:vandle_TDiff>>BarvsTDiff_zoom","","colz");
  tdiff_cut->Draw("same");
  sprintf(BarvsTDiff_zoom_char,"%s_vandle_BarvsTDiff_zoom.png",isotopeChar);
  c1->SaveAs(BarvsTDiff_zoom_char);
  sprintf(buffChar,
    "gm montage -mode concatenate -tile 2x2  %s %s %s %s %s_grid_barplots.png",
    QDCvsBar_char,
    BarvsTDiff_char,
    BarvsTOF_char,
    BarvsTDiff_zoom_char,
    isotopeChar);
  system(buffChar);
  // sprintf(buffChar,
  //   "gm montage -mode concatenate -tile 1x5  %s %s %s %s %s %s_1x5_grid.png",
  //   BarvsTDiff_zoom_char,
  //   QDCvsTOF_char,
  //   NEUTRON_TOF_char,
  //   "neutron_TOF_backgroundCompare.png",
  //   "neutron_TOF.png",
  //   isotopeChar);
  // system(buffChar);
  sprintf(buffChar,
    "gm montage -mode concatenate -tile 2x1 %s_QDCvsTOF_neutronPlots.png %s_grid_barplots.png %s_grid_noBorder.png ",
    isotopeChar,
    isotopeChar,
    isotopeChar);
  system(buffChar);
  sprintf(buffChar,
    "gm convert  %s_grid_noBorder.png -bordercolor Tomato -border 10x10 %s_dashboard.png",
    isotopeChar,
    isotopeChar);
  system(buffChar);
  sprintf(buffChar,
    "rm %s_QDCvsTOF_neutronPlots.png %s_grid_barplots.png %s_grid_noBorder.png ",
    isotopeChar,
    isotopeChar,
    isotopeChar);
  system(buffChar);
  sprintf(buffChar,"mv %s_dashboard.png ../.",isotopeChar);
  system(buffChar);
}


}
