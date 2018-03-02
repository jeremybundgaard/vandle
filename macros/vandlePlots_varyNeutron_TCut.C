
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


void makePlots(TTree* vandle_tree_local,
               TTree* vandle_tree_NEUTRON_local,
               std::vector<TCutG*> tcutg_NEUTRON_local,
               std::vector<TCutG*> tcutg_BACKGROUND_local,
               const char* isotope_local,
               int background_TOF_Shift_local,
               const char* cutChar_local);

void vandlePlots_varyNeutron_TCut(const char *vandle_tree_file = "",const char *otherTreeCuts = "1",const char *isotope= "") {

// TGaxis::SetMaxDigits(3);
// gStyle->SetPalette(kCherry);
gStyle->SetOptStat("ne");
gStyle->SetTitleOffset(1.2,"Y");
gStyle->SetNumberContours(99);
// gStyle->SetStatFont(63);
// gStyle->SetStatFontSize(12);


char buffChar[345]="";
char cutChar[345]="";
TF1* line_triangleSlope = new TF1("line_triangleSlope","(-2900/320)*x+3725",0,500);

std::vector<int> first;

std::vector<TCutG*> tcutg_NEUTRON;
std::vector<TCutG*> tcutg_BACKGROUND;
int cutWidth_tof=300;
int minTOF=70;
int maxTOF=minTOF+cutWidth_tof;
int minQDC=0;
int maxQDC=minQDC+3500;
int background_TOF_Shift = 350;

int color=1;
for (size_t cut_qdc = 0; cut_qdc <= 350; cut_qdc+=40) {

  sprintf(buffChar,"cut_tcutg_NEUTRON_qdcMax%zu",cut_qdc);
  TCutG* tcutg_NEUTRON_loopDummy = new TCutG(buffChar,3);
  tcutg_NEUTRON_loopDummy->SetName(buffChar);
  tcutg_NEUTRON_loopDummy->SetVarX("vandle_TOF");
  tcutg_NEUTRON_loopDummy->SetVarY("vandle_QDC");
  tcutg_NEUTRON_loopDummy->SetLineWidth(2);
  tcutg_NEUTRON_loopDummy->SetLineColor(color);
  tcutg_NEUTRON_loopDummy->SetPoint(0,minTOF,cut_qdc);
  tcutg_NEUTRON_loopDummy->SetPoint(1,maxTOF,cut_qdc);
  tcutg_NEUTRON_loopDummy->SetPoint(2,maxTOF,maxQDC);
  tcutg_NEUTRON_loopDummy->SetPoint(3,minTOF,maxQDC);
  tcutg_NEUTRON_loopDummy->SetPoint(4,minTOF,cut_qdc);
  tcutg_NEUTRON.push_back(tcutg_NEUTRON_loopDummy);

  sprintf(buffChar,"cut_tcutg_NEUTRON_BACKGROUND_qdcMax%zu",cut_qdc);
  TCutG* tcutg_BACKGROUND_loopDummy = new TCutG(buffChar,3);
  tcutg_BACKGROUND_loopDummy->SetName(buffChar);
  tcutg_BACKGROUND_loopDummy->SetVarX("vandle_TOF");
  tcutg_BACKGROUND_loopDummy->SetVarY("vandle_QDC");
  tcutg_BACKGROUND_loopDummy->SetLineWidth(2);
  tcutg_BACKGROUND_loopDummy->SetLineColor(kOrange);
  tcutg_BACKGROUND_loopDummy->SetPoint(0,background_TOF_Shift+minTOF,cut_qdc);
  tcutg_BACKGROUND_loopDummy->SetPoint(1,background_TOF_Shift+maxTOF,cut_qdc);
  tcutg_BACKGROUND_loopDummy->SetPoint(2,background_TOF_Shift+maxTOF,maxQDC);
  tcutg_BACKGROUND_loopDummy->SetPoint(3,background_TOF_Shift+minTOF,maxQDC);
  tcutg_BACKGROUND_loopDummy->SetPoint(4,background_TOF_Shift+minTOF,cut_qdc);
  tcutg_BACKGROUND.push_back(tcutg_BACKGROUND_loopDummy);
  color++;

}

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
  vandle_tree_NEUTRON,
  tcutg_NEUTRON,
  tcutg_BACKGROUND,
  isotope,
  background_TOF_Shift,
  "1");
sprintf(buffChar,"mv %s_grid1.png ../plots/.",isotope);
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
               std::vector<TCutG*> tcutg_NEUTRON_local,
               std::vector<TCutG*> tcutg_BACKGROUND_local,
               const char* isotope_local,
               int background_TOF_Shift_local,
               const char* cutChar_local)
{
  gStyle->SetOptStat("ne");

  char buffChar_local[345];
  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700); //c1->SetLogz();

  c1->SetLogz(1);
  TH2D* QDCvsTOF = new TH2D("QDCvsTOF","QDCvsTOF",1000,-30,970,3200,0,3200);
  sprintf(buffChar_local,"%s_QDCvsTOF",isotope_local);
  QDCvsTOF->SetTitle(buffChar_local);
  vandle_tree_local->Draw("vandle_QDC:vandle_TOF>>QDCvsTOF",cutChar_local,"colz");
  char QDCvsTOF_char[345];
  sprintf(QDCvsTOF_char,"%s_vandle_QDCvsTOF.png",isotope_local);
  for (int i = 0; i < tcutg_NEUTRON_local.size(); i++) {
    tcutg_NEUTRON_local[i]->Draw("same");
    tcutg_BACKGROUND_local[i]->Draw("same");
  }
  c1->SaveAs(QDCvsTOF_char);

  char NEUTRON_TOF_char[345];
  TH1D *NEUTRON_TOF_array[tcutg_NEUTRON_local.size()];
  for (int i_cut = 0; i_cut < tcutg_NEUTRON_local.size(); i_cut++) {

    sprintf(NEUTRON_TOF_char,"%s_NEUTRON_TOF",isotope_local);
    TH1D* NEUTRON_TOF = new TH1D("NEUTRON_TOF","NEUTRON_TOF",1000,-30,970);
    NEUTRON_TOF->SetTitle(" QDC vs TOF [cut projections]");
    NEUTRON_TOF->SetLineColor(i_cut+1);
    NEUTRON_TOF->SetLineWidth(2);
    sprintf(buffChar_local,"%s&&%s",tcutg_NEUTRON_local[i_cut]->GetName(),cutChar_local);
    vandle_tree_local->Draw("vandle_TOF>>NEUTRON_TOF",buffChar_local,"");
    TH1D* neutron_BACKGROUND = new TH1D("neutron_BACKGROUND","neutron_BACKGROUND",1000,-30,970);
    neutron_BACKGROUND->SetLineColor(kOrange);
    neutron_BACKGROUND->SetLineWidth(2);
    sprintf(buffChar_local,"%s&&%s",tcutg_BACKGROUND_local[i_cut]->GetName(),cutChar_local);
    vandle_tree_local->Draw("vandle_TOF",buffChar_local,"same");
    sprintf(NEUTRON_TOF_char,"%s_vandleTOF_Neutrons_%d.png",isotope_local,i_cut);
    c1->SaveAs(NEUTRON_TOF_char);

    sprintf(buffChar_local,"%s&&%s",tcutg_BACKGROUND_local[i_cut]->GetName(),cutChar_local);
    char drawChar[345];
    sprintf(drawChar,"(vandle_TOF-%d)>>neutron_BACKGROUND",background_TOF_Shift_local);
    vandle_tree_local->Draw(drawChar,buffChar_local,"");
    NEUTRON_TOF->GetXaxis()->SetRangeUser(0,500);
    NEUTRON_TOF->SetTitle("compare background");
    int rebin = 4;
    // NEUTRON_TOF->Rebin(rebin);
    // neutron_BACKGROUND->Rebin(rebin);
    NEUTRON_TOF->Draw();
    neutron_BACKGROUND->Draw("same");
    sprintf(NEUTRON_TOF_char,"%sneutron_TOF_backgroundCompare_%d.png",isotope_local,i_cut);
    c1->SaveAs(NEUTRON_TOF_char);

    NEUTRON_TOF->Add(neutron_BACKGROUND,-1);
    NEUTRON_TOF->SetTitle("neutronTOF [background subtracted]");
    NEUTRON_TOF->GetXaxis()->SetRangeUser(0,500);
    NEUTRON_TOF->Draw();
    sprintf(NEUTRON_TOF_char,"%sneutron_TOF_%d.png",isotope_local,i_cut);
    c1->SaveAs(NEUTRON_TOF_char);

    NEUTRON_TOF_array[i_cut] = (TH1D*)NEUTRON_TOF->Clone();

    delete NEUTRON_TOF;
    delete neutron_BACKGROUND;

  }
  gStyle->SetOptStat(0);

  for (int i_cut = 0; i_cut < tcutg_NEUTRON_local.size(); i_cut++) {
    if(i_cut==0)NEUTRON_TOF_array[i_cut]->Draw();
    NEUTRON_TOF_array[i_cut]->Draw("same");
  }
  c1->SaveAs("neutron_TOF.png");

  for (int i_cut = 0; i_cut < tcutg_NEUTRON_local.size(); i_cut++) {
    NEUTRON_TOF_array[i_cut]->Scale(1/NEUTRON_TOF_array[i_cut]->Integral(), "width");
    NEUTRON_TOF_array[i_cut]->SetTitle("NORMALIZED neutron_TOF");
    NEUTRON_TOF_array[i_cut]->SetMaximum(0.020);
    NEUTRON_TOF_array[i_cut]->SetLineStyle(1);
    gPad->Update();
    if(i_cut==0)NEUTRON_TOF_array[i_cut]->Draw("Cl");
    NEUTRON_TOF_array[i_cut]->Draw("same Cl");
  }
  c1->SaveAs("neutron_TOF_Norms.png");

  sprintf(buffChar_local,
    "gm montage -mode concatenate -tile 2x2 %s %s %s %s %s_QDCvsTOF_grid_tmp.png",
    QDCvsTOF_char,
    "neutron_TOF.png",
    NEUTRON_TOF_char,
    "neutron_TOF_backgroundCompare.png",
    isotope_local);
  system(buffChar_local);


  // char BarvsTDiff_char[345];
  // TH2D* BarvsTDiff = new TH2D("BarvsTDiff","BarvsTDiff",1000,-1000,1000,45,0,45);
  // sprintf(buffChar_local,"%s_BarvsTDiff",isotope_local);
  // vandle_tree_local->Draw("vandle_bar:vandle_TDiff>>BarvsTDiff",cutChar_local,"colz");
  // sprintf(BarvsTDiff_char,"%s_vandle_BarvsTDiff.png",isotope_local);
  // c1->SaveAs(BarvsTDiff_char);
  // c1->SetLogz(0);
  //
  // char BarvsQDC_char[345];
  // TH2D* BarvsQDC = new TH2D("BarvsQDC","BarvsQDC",45,0,45,100,-30,370);
  // sprintf(buffChar_local,"%s_BarvsQDC",isotope_local);
  // BarvsQDC->SetTitle(buffChar_local);
  // vandle_tree_local->Draw("vandle_QDC:vandle_bar>>BarvsQDC",cutChar_local,"colz");
  // sprintf(BarvsQDC_char,"%s_vandle_BarvsQDC.png",isotope_local);
  // c1->SaveAs(BarvsQDC_char);
  //
  // char BarvsTDiff_zoom_char[345];
  // TH2D* BarvsTDiff_zoom = new TH2D("BarvsTDiff_zoom","BarvsTDiff_zoom",100,-40,40,45,0,45);
  // sprintf(BarvsTDiff_zoom_char,"%s_BarvsTDiff_zoom",isotope_local);
  // vandle_tree_local->Draw("vandle_bar:vandle_TDiff>>BarvsTDiff_zoom",cutChar_local,"colz");
  // sprintf(BarvsTDiff_zoom_char,"%s_vandle_BarvsTDiff_zoom.png",isotope_local);
  // c1->SaveAs(BarvsTDiff_zoom_char);
  //
  // char BarvsTOF_char[345];
  // TH2D* BarvsTOF = new TH2D("BarvsTOF","BarvsTOF",300,-30,70,45,0,45);
  // sprintf(buffChar_local,"%s_BarvsTOF",isotope_local);
  // BarvsTOF->SetTitle(buffChar_local);
  // vandle_tree_local->Draw("vandle_bar:vandle_TOF>>BarvsTOF",cutChar_local,"colz");
  // sprintf(BarvsTOF_char,"%s_vandle_BarvsTOF.png",isotope_local);
  // c1->SaveAs(BarvsTOF_char);
  //
  // sprintf(buffChar_local,
  //   "gm montage -mode concatenate -tile 2x2  %s %s %s %s %s_grid%stmp.png",
  //   BarvsQDC_char,
  //   BarvsTDiff_char,
  //   BarvsTOF_char,
  //   BarvsTDiff_zoom_char,
  //   isotope_local,
  //   cutChar_local);
  // system(buffChar_local);

  sprintf(buffChar_local,
    "gm montage -mode concatenate -tile 1x5  %s %s %s %s_varied_TCutGs.png",
    QDCvsTOF_char,
    "neutron_TOF.png",
    "neutron_TOF_Norms.png",
    isotope_local);
  system(buffChar_local);

  // sprintf(buffChar_local,
  //   "gm montage -mode concatenate -tile 2x1 %s_QDCvsTOF_grid_tmp.png %s_grid%stmp.png %s_grid%s_noBorder.png ",
  //   isotope_local,
  //   isotope_local,
  //   cutChar_local,
  //   isotope_local,
  //   cutChar_local);
  // system(buffChar_local);
  //
  // sprintf(buffChar_local,
  //   "gm convert  %s_grid%s_noBorder.png -bordercolor Tomato -border 10x10 %s_grid%s.png",
  //   isotope_local,
  //   cutChar_local,
  //   isotope_local,
  //   cutChar_local);
  // system(buffChar_local);
  //
  // sprintf(buffChar_local,
  //   "rm %s_QDCvsTOF_grid_tmp.png %s_grid%stmp.png %s_grid%s_noBorder.png ",
  //   isotope_local,
  //   isotope_local,
  //   cutChar_local,
  //   isotope_local,
  //   cutChar_local);
  // system(buffChar_local);

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
  // delete BarvsTOF;
  // delete BarvsQDC;
  // delete BarvsTDiff;
  // delete BarvsTDiff_zoom;

}
