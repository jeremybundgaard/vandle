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

char buffChar[345]="";
char cutChar[345]="-6<vandle_TDiff&&vandle_TDiff<4";
std::vector<int> first;
std::vector<TCutG*> tcutg_NEUTRON;
std::vector<TCutG*> tcutg_BACKGROUND;
int cutStep_qdc=1000;
int minTOF=30;
int maxTOF=780;
int minQDC=3000;
int maxQDC=22000;
int qdc_bins=500;
int tof_bins=500;
void makePlots(TTree* vandle_tree_local,
               std::vector<TCutG*> tcutg_BACKGROUND_local,
               const char* isotope_local,
               const char* cutChar_local);

void background_ornl2016(const char *vandle_tree_file = "",const char *otherTreeCuts = "1",const char *isotope= "") {

// TGaxis::SetMaxDigits(3);
// gStyle->SetPalette(kCherry);
gStyle->SetOptStat("ne");
gStyle->SetTitleOffset(1.2,"Y");
gStyle->SetNumberContours(99);
// gStyle->SetStatFont(63);
// gStyle->SetStatFontSize(12);



for (size_t cut_qdc = minQDC; cut_qdc <= maxQDC*0.8; cut_qdc+=cutStep_qdc) {

  sprintf(buffChar,"tcutg_BACKGROUND_qdcMax%zu",cut_qdc);
  TCutG* tcutg_BACKGROUND_loopDummy = new TCutG(buffChar,3);
  // ///////  thick slices
  // tcutg_BACKGROUND_loopDummy->SetName(buffChar);
  // tcutg_BACKGROUND_loopDummy->SetVarX("vandle_TOF");
  // tcutg_BACKGROUND_loopDummy->SetVarY("vandle_QDC");
  // tcutg_BACKGROUND_loopDummy->SetLineColor(kYellow);
  // if(cut_qdc<10000) minTOF=200;
  // tcutg_BACKGROUND_loopDummy->SetPoint(0,minTOF,cut_qdc);
  // tcutg_BACKGROUND_loopDummy->SetPoint(1,maxTOF,cut_qdc);
  // tcutg_BACKGROUND_loopDummy->SetPoint(2,maxTOF,cut_qdc+cutStep_qdc);
  // tcutg_BACKGROUND_loopDummy->SetPoint(3,minTOF,cut_qdc+cutStep_qdc);
  // tcutg_BACKGROUND_loopDummy->SetPoint(4,minTOF,cut_qdc);

  minTOF=120;
  tcutg_BACKGROUND_loopDummy->SetName(buffChar);
  tcutg_BACKGROUND_loopDummy->SetVarX("vandle_TOF");
  tcutg_BACKGROUND_loopDummy->SetVarY("vandle_QDC");
  tcutg_BACKGROUND_loopDummy->SetLineColor(kYellow);
  tcutg_BACKGROUND_loopDummy->SetPoint(0,minTOF,cut_qdc);
  tcutg_BACKGROUND_loopDummy->SetPoint(1,maxTOF,cut_qdc);
  tcutg_BACKGROUND_loopDummy->SetPoint(2,maxTOF,maxQDC);
  tcutg_BACKGROUND_loopDummy->SetPoint(3,minTOF,maxQDC);
  tcutg_BACKGROUND_loopDummy->SetPoint(4,minTOF,cut_qdc);

  tcutg_BACKGROUND.push_back(tcutg_BACKGROUND_loopDummy);
  minTOF=40;

}

///////// initialize track tree /////////
printf("\n///////// initialize vandle_tree /////////\n\n");
TFile *treefile = new TFile(vandle_tree_file);
TTree* vandle_tree = (TTree*)treefile->Get("vandle_tree");
// TChain * vandle_tree = new TChain("vandle_tree");
// vandle_tree->Add(*treefile);
// TProof * pr;
// pr = TProof::Open("lite://","workers=16");
// vandle_tree->SetProof();

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

makePlots(vandle_tree,
  tcutg_BACKGROUND,
  isotope,
  cutChar);
}

void makePlots(TTree* vandle_tree_local,
               std::vector<TCutG*> tcutg_BACKGROUND_local,
               const char* isotope_local,
               const char* cutChar_local)
{
  gStyle->SetOptStat("ne");

  char buffChar_local[345];
  char drawChar[345];
  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetLogz(1);
  TH2D* QDCvsTOF = new TH2D("QDCvsTOF","QDCvsTOF",tof_bins,-30,970,qdc_bins,0,maxQDC*1.3);
  sprintf(buffChar_local,"%s_QDCvsTOF",isotope_local);
  QDCvsTOF->SetTitle(buffChar_local);
  vandle_tree_local->Draw("vandle_QDC:vandle_TOF>>QDCvsTOF",cutChar_local,"colz");
  char QDCvsTOF_char[345];
  sprintf(QDCvsTOF_char,"%s_vandle_QDCvsTOF.png",isotope_local);
  for (int i = 0; i < tcutg_BACKGROUND_local.size(); i++) {
    tcutg_BACKGROUND_local[i]->Draw("same");
  }
  c1->SaveAs(QDCvsTOF_char);

  int color=1;
  TH1D* backgrounds = new TH1D("backgrounds","backgrounds",tof_bins,-30,970);
  TH1D *backgrounds_array[tcutg_BACKGROUND_local.size()];
  for (int i_cut = tcutg_BACKGROUND_local.size()-1; i_cut >= 0; i_cut--) {
    backgrounds->SetBins(tof_bins,-30,970);
    backgrounds->SetLineColor(kRed);color++;
    backgrounds->SetLineWidth(2);
    sprintf(buffChar_local,"%s&&%s",tcutg_BACKGROUND_local[i_cut]->GetName(),cutChar_local);
    sprintf(drawChar,"vandle_TOF>>backgrounds");
    vandle_tree_local->Draw(drawChar,buffChar_local);

    // TH1D* backgrounds = new TH1D("backgrounds","backgrounds",tof_bins,-30,970);
    // int isBinWidthSet=0;
    // while (isBinCalcd) {
    //   TH1D* checkBinning = new TH1D("checkBinning","checkBinning",tof_bins,-30,970);
    //   for (int k = backgrounds->FindFirstBinAbove(0)+5; k < backgrounds->FindLastBinAbove(0)-5; k++) {
    //   }
    //   backgrounds->SetBins((int)ceil(pow(backgrounds->GetEntries(),0.55)),-30,970);
    // }

    backgrounds->SetBins((int)ceil(pow(backgrounds->GetEntries(),0.45)),-30,970);
    printf("backgrounds->GetEntries(): %d\t backgrounds->GetNbinsX():%d \n",(int)backgrounds->GetEntries(),backgrounds->GetNbinsX());
    vandle_tree_local->Draw(drawChar,buffChar_local);
    // TF1 *background_TOF_fit = new TF1("background_TOF_fit","[1]*x+[2]",backgrounds->FindFirstBinAbove(0)+10,backgrounds->FindLastBinAbove(0)-10);
    TF1 *background_TOF_fit_line = new TF1("background_TOF_fit_line","[1]*x+[0]",
                                      ceil(backgrounds->GetBinCenter(backgrounds->FindFirstBinAbove(0)+10)),
                                      ceil(backgrounds->GetBinCenter(backgrounds->FindLastBinAbove(0)-10)));
    background_TOF_fit_line->SetLineColor(kBlue);
    background_TOF_fit_line->SetLineWidth(4);

    TF1 *background_TOF_fit_line_expo = new TF1("background_TOF_fit_line_expo","[2]+exp([1]*x+[0])",
                                      ceil(backgrounds->GetBinCenter(backgrounds->FindFirstBinAbove(0)+10)),
                                      ceil(backgrounds->GetBinCenter(backgrounds->FindLastBinAbove(0)-10)));
    background_TOF_fit_line_expo->SetLineColor(kGreen);
    background_TOF_fit_line_expo->SetLineWidth(4);
    // printf("backgrounds->GetBinCenter(backgrounds->FindFirstBinAbove(0)+10):%f\n",backgrounds->GetBinCenter(backgrounds->FindFirstBinAbove(0)+10));
    // printf("backgrounds->GetBinCenter(backgrounds->FindLastBinAbove(0)-10):%f\n",backgrounds->GetBinCenter(backgrounds->FindLastBinAbove(0)-10));
    gStyle->SetOptFit();
    backgrounds->Fit("background_TOF_fit_line","R");
    backgrounds->Fit("background_TOF_fit_line_expo","R");
    backgrounds->Draw();

    background_TOF_fit_line->Draw("same");
    background_TOF_fit_line->SetRange(0,900);
    background_TOF_fit_line_expo->Draw("same");
    background_TOF_fit_line_expo->SetRange(0,900);
    char TOF_background_char[345];
    sprintf(TOF_background_char,"%s_TOF_background_%02d.png",isotope_local,i_cut);
    c1->SaveAs(TOF_background_char);
    backgrounds_array[i_cut] = (TH1D*)backgrounds->Clone();

    QDCvsTOF->Draw("colz");
    for (int ii = 0; ii < tcutg_BACKGROUND_local.size(); ii++) {
      tcutg_BACKGROUND_local[ii]->Draw("same");
    }
    TCutG* tmp_tcutg_BACKGROUND = (TCutG*)tcutg_BACKGROUND_local[i_cut]->Clone();
    tmp_tcutg_BACKGROUND->SetLineColor(kRed);
    tmp_tcutg_BACKGROUND->Draw("same");
    char QDCvsTOF_background_char[345];
    sprintf(QDCvsTOF_background_char,"%s_QDCvsTOF_background_%02d.png",isotope_local,i_cut);
    c1->SaveAs(QDCvsTOF_background_char);

    sprintf(buffChar_local,
      "gm montage -mode concatenate -tile 2x1  %s %s ../plots/%s_background_projections_%02d.png",
      QDCvsTOF_background_char,
      TOF_background_char,
      isotope_local,
      i_cut);
    system(buffChar_local);

    sprintf(buffChar_local,
      "rm %s %s",
      TOF_background_char,
      QDCvsTOF_background_char);
    system(buffChar_local);

  }

  sprintf(buffChar_local,
    "gm convert -loop 0 -delay 15 `ls ../plots/*_background_projections_*.png| sort` `ls ../plots/*_background_projections_*.png| sort -r` ../%sRb_background_projections.gif",
    isotope_local);
  system(buffChar_local);

  delete backgrounds;
  delete c1;
  delete QDCvsTOF;

}
