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
// #include "/home/jeremy/vandle/macros/vandlePlots.h"

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
// #include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>
// #include

int cutWidth_tof=300;
int minTOF=70;
int maxTOF=minTOF+cutWidth_tof;
int minQDC=0;
int maxQDC=minQDC+6500;
int background_TOF_Shift=cutWidth_tof+10;

void makePlots(TTree* vandle_tree_local,
               TTree* vandle_tree_NEUTRON_local,
               TCutG* tcutg_NEUTRON_local,
               TCutG*  tcutg_BACKGROUND_local,
               int background_TOF_Shift_local,
               const char* isotope_local,
               const char* cutChar_local);

void vandlePlots(const char *vandle_tree_file = "",const char *otherTreeCuts = "1",int isotope = 0) {

// TGaxis::SetMaxDigits(3);
// gStyle->SetPalette(kCherry);
gStyle->SetOptStat("ne");
gStyle->SetTitleOffset(1.2,"Y");
gStyle->SetNumberContours(99);
// gStyle->SetStatFont(63);
// gStyle->SetStatFontSize(12);

TCutG *tcutg_NEUTRON = new TCutG("tcutg_NEUTRON",3);
tcutg_NEUTRON->SetVarX("vandle_TOF");
tcutg_NEUTRON->SetVarY("vandle_QDC");
tcutg_NEUTRON->SetLineWidth(4);
tcutg_NEUTRON->SetLineColor(kMagenta);
tcutg_NEUTRON->SetPoint(0,minTOF,minQDC);
tcutg_NEUTRON->SetPoint(1,maxTOF,minQDC);
tcutg_NEUTRON->SetPoint(2,maxTOF,maxQDC);
tcutg_NEUTRON->SetPoint(3,minTOF,maxQDC);
tcutg_NEUTRON->SetPoint(4,minTOF,minQDC);

TCutG *tcutg_BACKGROUND = new TCutG("tcutg_BACKGROUND",3);
tcutg_BACKGROUND->SetVarX("vandle_TOF");
tcutg_BACKGROUND->SetVarY("vandle_QDC");
tcutg_BACKGROUND->SetLineWidth(4);
tcutg_BACKGROUND->SetLineColor(kBlue);
tcutg_BACKGROUND->SetPoint(0,background_TOF_Shift+minTOF,minQDC);
tcutg_BACKGROUND->SetPoint(1,background_TOF_Shift+maxTOF,minQDC);
tcutg_BACKGROUND->SetPoint(2,background_TOF_Shift+maxTOF,maxQDC);
tcutg_BACKGROUND->SetPoint(3,background_TOF_Shift+minTOF,maxQDC);
tcutg_BACKGROUND->SetPoint(4,background_TOF_Shift+minTOF,minQDC);


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
// // vandle_treeree_NEUTRON->SetLineColor(kMagenta);
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
int dataRun=0;

vandle_tree->SetBranchAddress("vandle_bar",&vandle_bar);
vandle_tree->SetBranchAddress("vandle_QDC",&vandle_QDC);
vandle_tree->SetBranchAddress("vandle_TOF",&vandle_TOF);
vandle_tree->SetBranchAddress("vandle_TDiff",&vandle_TDiff);
vandle_tree->SetBranchAddress("dataRun",&dataRun);

char buffChar[345]="";
char cutChar[345]="-6<vandle_TDiff&&vandle_TDiff<4";

// if (isotope==100) sprintf(cutChar,"-6<vandle_TDiff&&vandle_TDiff<4");

makePlots(vandle_tree,
  vandle_tree_NEUTRON,
  tcutg_NEUTRON,
  tcutg_BACKGROUND,
  background_TOF_Shift,
  isotopeChar,
  cutChar);
sprintf(buffChar,"mv %s_dashBoard.png ../.",isotopeChar);
system(buffChar);

// printf("loop through individual runs\n");
// std::set<int> dataRun_set;
// unsigned int treeEntries = vandle_tree->GetEntries();
// for (size_t entry = 0; entry < treeEntries; entry++) {
//   if (entry%10000000==0) printf("%2.0f%% to go \n", (double)(1-entry/treeEntries)*100);
//   vandle_tree->GetEntry(entry);
//   dataRun_set.insert(dataRun);
// }
// for (auto outputName : dataRun_set ){
//   sprintf(cutChar,"dataRun==%d",outputName);
//   printf("%s\n",cutChar);
//   makePlots(vandle_tree,
//     vandle_tree_NEUTRON,
//     tcutg_NEUTRON,
//     tcutg_BACKGROUND,
//     isotope,
//     cutChar);
//   sprintf(buffChar,
//     "mv %s_grid%s.png ../plots/individual_runs/.",
//     isotope,
//     cutChar);
//   system(buffChar);
// }

}

void makePlots(TTree* vandle_tree_local,
               TTree* vandle_tree_NEUTRON_local,
               TCutG* tcutg_NEUTRON_local,
               TCutG*  tcutg_BACKGROUND_local,
               int background_TOF_Shift_local,
               const char* isotope_local,
               const char* cutChar_local)
{
  gStyle->SetOptStat("ne");

  char buffChar_local[345];
  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700); //c1->SetLogz();

  c1->SetLogz(1);
  TH2D* QDCvsTOF = new TH2D("QDCvsTOF","QDCvsTOF",1000,-30,970,maxQDC+1000,0,maxQDC+1000);
  sprintf(buffChar_local,"%s_QDCvsTOF",isotope_local);
  QDCvsTOF->SetTitle(buffChar_local);
  vandle_tree_local->Draw("vandle_QDC:vandle_TOF>>QDCvsTOF",cutChar_local,"colz");
  char QDCvsTOF_char[345];
  sprintf(QDCvsTOF_char,"%s_vandle_QDCvsTOF.png",isotope_local);
  tcutg_NEUTRON_local->Draw("same");
  tcutg_BACKGROUND_local->Draw("same");
  c1->SaveAs(QDCvsTOF_char);

  c1->SetLogy(1);
  char NEUTRON_TOF_char[345];
  TH1D* neutron_BACKGROUND = new TH1D("neutron_BACKGROUND","neutron_BACKGROUND",1000,-30,970);
  neutron_BACKGROUND->SetLineColor(kBlue);
  neutron_BACKGROUND->SetLineWidth(3);
  sprintf(buffChar_local,"%s&&%s",tcutg_BACKGROUND_local->GetName(),cutChar_local);
  vandle_tree_local->Draw("vandle_TOF>>neutron_BACKGROUND",buffChar_local);
  TF1 *background_TOF_fit_above = new TF1("background_TOF_fit_above","[0]+exp([1]*x+[2])",minTOF+background_TOF_Shift,maxTOF+background_TOF_Shift);
  TF1 *background_TOF_fit = new TF1("background_TOF_fit","exp([1]*x+[2])",minTOF+background_TOF_Shift,maxTOF+background_TOF_Shift);
  neutron_BACKGROUND->Fit("background_TOF_fit","B");
  c1->SaveAs("neutron_BACKGROUND_withFit.png");
  sprintf(NEUTRON_TOF_char,"%s_NEUTRON_TOF",isotope_local);

  TH1D* NEUTRON_TOF = new TH1D("NEUTRON_TOF","NEUTRON_TOF",1000,-30,970);
  NEUTRON_TOF->SetTitle(" QDC vs TOF [cut projections]");
  NEUTRON_TOF->SetLineColor(kMagenta);
  NEUTRON_TOF->SetLineWidth(3);
  sprintf(buffChar_local,"%s&&%s",tcutg_NEUTRON_local->GetName(),cutChar_local);
  vandle_tree_local->Draw("vandle_TOF>>NEUTRON_TOF",buffChar_local,"");
  neutron_BACKGROUND->Draw("same");
  sprintf(NEUTRON_TOF_char,"%s_vandleTOF_Neutrons.png",isotope_local);
  c1->SaveAs(NEUTRON_TOF_char);

  sprintf(buffChar_local,"%s&&%s",tcutg_BACKGROUND_local->GetName(),cutChar_local);
  char drawChar[345];
  sprintf(drawChar,"(vandle_TOF-%d)>>neutron_BACKGROUND",background_TOF_Shift_local);
  vandle_tree_local->Draw(drawChar,buffChar_local,"");
  NEUTRON_TOF->GetXaxis()->SetRangeUser(0,500);
  NEUTRON_TOF->SetTitle("compare background");
  int rebin = 5;
  NEUTRON_TOF->Rebin(rebin);
  neutron_BACKGROUND->Rebin(rebin);
  NEUTRON_TOF->Draw();
  neutron_BACKGROUND->Draw("same");
  c1->SaveAs("neutron_TOF_backgroundCompare.png");

  NEUTRON_TOF->Add(neutron_BACKGROUND,-1);
  NEUTRON_TOF->SetTitle("neutronTOF [background subtracted]");
  NEUTRON_TOF->GetXaxis()->SetRangeUser(0,500);
  NEUTRON_TOF->Draw();
  c1->SaveAs("neutron_TOF.png");
  c1->SetLogy(0);

  sprintf(buffChar_local,
    "gm montage -mode concatenate -tile 2x2 %s %s %s %s %s_QDCvsTOF_neutronPlots.png",
    QDCvsTOF_char,
    "neutron_TOF.png",
    NEUTRON_TOF_char,
    "neutron_TOF_backgroundCompare.png",
    isotope_local);
  system(buffChar_local);

  gStyle->SetOptStat(0);

  char BarvsTDiff_char[345];
  TH2D* BarvsTDiff = new TH2D("BarvsTDiff","BarvsTDiff",1000,-1000,1000,45,0,45);
  sprintf(buffChar_local,"%s_BarvsTDiff",isotope_local);
  vandle_tree_local->Draw("vandle_bar:vandle_TDiff>>BarvsTDiff",cutChar_local,"colz");
  sprintf(BarvsTDiff_char,"%s_vandle_BarvsTDiff.png",isotope_local);
  c1->SaveAs(BarvsTDiff_char);
  c1->SetLogz(0);

  char BarvsQDC_char[345];
  TH2D* BarvsQDC = new TH2D("BarvsQDC","BarvsQDC",45,0,45,100,-30,370);
  sprintf(buffChar_local,"%s_BarvsQDC",isotope_local);
  BarvsQDC->SetTitle(buffChar_local);
  vandle_tree_local->Draw("vandle_QDC:vandle_bar>>BarvsQDC",cutChar_local,"colz");
  sprintf(BarvsQDC_char,"%s_vandle_BarvsQDC.png",isotope_local);
  c1->SaveAs(BarvsQDC_char);

  char BarvsTDiff_zoom_char[345];
  TH2D* BarvsTDiff_zoom = new TH2D("BarvsTDiff_zoom","BarvsTDiff_zoom",100,-40,40,45,0,45);
  sprintf(BarvsTDiff_zoom_char,"%s_BarvsTDiff_zoom",isotope_local);
  vandle_tree_local->Draw("vandle_bar:vandle_TDiff>>BarvsTDiff_zoom",cutChar_local,"colz");
  sprintf(BarvsTDiff_zoom_char,"%s_vandle_BarvsTDiff_zoom.png",isotope_local);
  c1->SaveAs(BarvsTDiff_zoom_char);

  char BarvsTOF_char[345];
  TH2D* BarvsTOF = new TH2D("BarvsTOF","BarvsTOF",300,-30,70,45,0,45);
  sprintf(buffChar_local,"%s_BarvsTOF",isotope_local);
  BarvsTOF->SetTitle(buffChar_local);
  vandle_tree_local->Draw("vandle_bar:vandle_TOF>>BarvsTOF",cutChar_local,"colz");
  sprintf(BarvsTOF_char,"%s_vandle_BarvsTOF.png",isotope_local);
  c1->SaveAs(BarvsTOF_char);

  sprintf(buffChar_local,
    "gm montage -mode concatenate -tile 2x2  %s %s %s %s %s_grid_barplots.png",
    BarvsQDC_char,
    BarvsTDiff_char,
    BarvsTOF_char,
    BarvsTDiff_zoom_char,
    isotope_local);
  system(buffChar_local);

  sprintf(buffChar_local,
    "gm montage -mode concatenate -tile 1x5  %s %s %s %s %s %s_1x5_grid.png",
    BarvsTDiff_zoom_char,
    QDCvsTOF_char,
    NEUTRON_TOF_char,
    "neutron_TOF_backgroundCompare.png",
    "neutron_TOF.png",
    isotope_local);
  system(buffChar_local);

  sprintf(buffChar_local,
    "gm montage -mode concatenate -tile 2x1 %s_QDCvsTOF_neutronPlots.png %s_grid_barplots.png %s_grid_noBorder.png ",
    isotope_local,
    isotope_local,
    isotope_local);
  system(buffChar_local);

  sprintf(buffChar_local,
    "gm convert  %s_grid_noBorder.png -bordercolor Tomato -border 10x10 %s_dashBoard.png",
    isotope_local,
    isotope_local);
  system(buffChar_local);

  sprintf(buffChar_local,
    "rm %s_QDCvsTOF_neutronPlots.png %s_grid_barplots.png %s_grid_noBorder.png ",
    isotope_local,
    isotope_local,
    isotope_local);
  system(buffChar_local);

  // sprintf(buffChar_local,
  //   "rm %s %s %s %s %s %s %s %s",
  //   QDCvsTOF_char,
  //   NEUTRON_TOF_char,
  //   BarvsTOF_char,
  //   BarvsQDC_char,
  //   BarvsTDiff_char,
  //   BarvsTDiff_zoom_char,
  //   "neutron_TOF.png",
  //   "neutron_TOF_backgroundCompare.png");
  // system(buffChar_local);

  delete c1;
  delete QDCvsTOF;
  delete NEUTRON_TOF;
  delete neutron_BACKGROUND;
  delete BarvsTOF;
  delete BarvsQDC;
  delete BarvsTDiff;
  delete BarvsTDiff_zoom;

}
