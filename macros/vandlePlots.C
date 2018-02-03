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
#include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>

void makePlots(TTree* dst_local,
               TTree* dst_NEUTRON_local,
               TCutG* tcutg_NEUTRON_local,
               TCutG*  tcutg_BACKGROUND_local,
               const char* isotope_local,
               const char* cutChar_local);

void vandlePlots(const char *data_summary_tree_file = "",const char *otherTreeCuts = "1",const char *isotope= "") {

  // TGaxis::SetMaxDigits(3);
  // gStyle->SetPalette(kCherry);
  gStyle->SetOptStat("ne");
  gStyle->SetTitleOffset(1.2,"Y");
  gStyle->SetNumberContours(99);
  // gStyle->SetStatFont(63);
  // gStyle->SetStatFontSize(12);

  TCutG *tcutg_NEUTRON = new TCutG("tcutg_NEUTRON",3);
  tcutg_NEUTRON->SetVarX("vandle_TOF");
  tcutg_NEUTRON->SetVarY("vandle_BarQDC");
  tcutg_NEUTRON->SetLineWidth(4);
  tcutg_NEUTRON->SetLineColor(kMagenta);

  ////////   triangle
  tcutg_NEUTRON->SetPoint(0,80,100);
  tcutg_NEUTRON->SetPoint(1,400,100);
  tcutg_NEUTRON->SetPoint(2,80,3000);
  tcutg_NEUTRON->SetPoint(3,80,100);

  // //////////   square
  // tcutg_NEUTRON->SetPoint(0,80,100);
  // tcutg_NEUTRON->SetPoint(1,400,100);
  // tcutg_NEUTRON->SetPoint(2,400,3000);
  // tcutg_NEUTRON->SetPoint(3,80,3000);
  // tcutg_NEUTRON->SetPoint(4,80,100);

  TCutG *tcutg_BACKGROUND = new TCutG("tcutg_BACKGROUND",3);
  tcutg_BACKGROUND->SetVarX("vandle_TOF");
  tcutg_BACKGROUND->SetVarY("vandle_BarQDC");
  tcutg_BACKGROUND->SetLineWidth(4);
  tcutg_BACKGROUND->SetLineColor(kBlue);

  ////////   triangle
  tcutg_BACKGROUND->SetPoint(0,400+80,100);
  tcutg_BACKGROUND->SetPoint(1,400+400,100);
  tcutg_BACKGROUND->SetPoint(2,400+80,3000);
  tcutg_BACKGROUND->SetPoint(3,400+80,100);

  // //////////   square
  // tcutg_BACKGROUND->SetPoint(0,400+80,100);
  // tcutg_BACKGROUND->SetPoint(1,400+400,100);
  // tcutg_BACKGROUND->SetPoint(2,400+400,3000);
  // tcutg_BACKGROUND->SetPoint(3,400+80,3000);
  // tcutg_BACKGROUND->SetPoint(4,400+80,100);

  ///////// initialize track tree /////////
  printf("\n///////// initialize data_summary_tree /////////\n\n");
  TFile *treefile = new TFile(data_summary_tree_file);
  TTree* dst = (TTree*)treefile->Get("data_summary_tree");
  TTree* dst_NEUTRON;
  // TTree* dstree = (TTree*)treefile->Get("data_summary_tree");
  // TChain * dst = new TChain("data_summary_tree");
  // dst->Add(data_summary_tree_file);
  // TProof * pr;
  // pr = TProof::Open("lite://","workers=16");
  // dst->SetProof();
  //
  // gROOT->cd();
  // TFile* dummyfile = new TFile("dummy.root", "RECREATE");
  // dummyfile->cd();
  // TTree* dstree_NEUTRON = (TTree*)dst->CopyTree("tcutg_NEUTRON");
  // // dstree_NEUTRON->SetLineColor(kMagenta);
  // dummyfile->Write();
  // TChain * dst_NEUTRON = new TChain("data_summary_tree");
  // dst_NEUTRON->Add("dummy.root");
  // dst_NEUTRON->SetProof();

  unsigned int evtNumber=0;
  unsigned int vandle_barNum=0;
  double vandle_QDC=0;
  double vandle_TOF=0;
  double vandle_TDiff=0;
  int output_name=0;

  dst->SetBranchAddress("vandle_barNum",&vandle_barNum);
  dst->SetBranchAddress("vandle_BarQDC",&vandle_QDC);
  dst->SetBranchAddress("vandle_TOF",&vandle_TOF);
  dst->SetBranchAddress("vandle_TDiff",&vandle_TDiff);
  dst->SetBranchAddress("output_name",&output_name);

  char cutChar[345]="";
  makePlots(dst,
    dst_NEUTRON,
    tcutg_NEUTRON,
    tcutg_BACKGROUND,
    isotope,
    cutChar);

  // printf("loop through individual runs\n");
  // std::set<int> output_name_set;
  // unsigned int treeEntries = dst->GetEntries();
  // for (size_t entry = 0; entry < treeEntries; entry++) {
  //   if (entry%10000000==0) printf("%2.0f%% to go \n", (double)(1-entry/treeEntries)*100);
  //   dst->GetEntry(entry);
  //   output_name_set.insert(output_name);
  // }
  // for (auto outputName : output_name_set ){
  //   sprintf(cutChar,"output_name==%d",outputName);
  //   printf("%s\n",cutChar);
  //   makePlots(dst,
  //     dst_NEUTRON,
  //     tcutg_NEUTRON,
  //     isotope,
  //     cutChar);
  // }

}

void makePlots(TTree* dst_local,
               TTree* dst_NEUTRON_local,
               TCutG* tcutg_NEUTRON_local,
               TCutG*  tcutg_BACKGROUND_local,
               const char* isotope_local,
               const char* cutChar_local)
{
  gStyle->SetOptStat("ne");

  char buffChar[345];
  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700); //c1->SetLogz();

  c1->SetLogz(1);
  TH2D* QDCvsTOF = new TH2D("QDCvsTOF","QDCvsTOF",1030,-30,1000,3200,0,3200);
  sprintf(buffChar,"%s_QDCvsTOF",isotope_local);
  QDCvsTOF->SetTitle(buffChar);
  dst_local->Draw("vandle_BarQDC:vandle_TOF>>QDCvsTOF","","colz");
  char QDCvsTOF_char[345];
  sprintf(QDCvsTOF_char,"%s_vandle_QDCvsTOF.png",isotope_local);
  tcutg_NEUTRON_local->Draw("same");
  tcutg_BACKGROUND_local->Draw("same");
  c1->SaveAs(QDCvsTOF_char);

  char NEUTRON_TOF_char[345];
  sprintf(NEUTRON_TOF_char,"%s_NEUTRON_TOF",isotope_local);
  TH1D* NEUTRON_TOF = new TH1D("NEUTRON_TOF","NEUTRON_TOF",1030,-30,1000);
  NEUTRON_TOF->SetTitle(isotope_local);
  NEUTRON_TOF->SetLineColor(kMagenta);
  dst_local->Draw("vandle_TOF>>NEUTRON_TOF",tcutg_NEUTRON_local->GetName(),"");
  dst_local->SetLineColor(kBlue);
  TH1D* neutron_BACKGROUND = new TH1D("neutron_BACKGROUND","neutron_BACKGROUND",1030,-30,1000);
  dst_local->Draw("vandle_TOF",tcutg_BACKGROUND_local->GetName() ,"same");
  sprintf(NEUTRON_TOF_char,"../%s_vandleTOF_Neutrons.png",isotope_local);
  c1->SaveAs(NEUTRON_TOF_char);

  dst_local->Draw("(vandle_TOF-400)>>neutron_BACKGROUND",tcutg_BACKGROUND_local->GetName() ,"same");
  NEUTRON_TOF->GetXaxis()->SetRangeUser(0,500);
  NEUTRON_TOF->SetTitle("compare background");
  NEUTRON_TOF->Draw();
  neutron_BACKGROUND->Draw("same");
  c1->SaveAs("neutron_TOF_backgroundCompare.png");

  NEUTRON_TOF->Add(neutron_BACKGROUND,-1);
  NEUTRON_TOF->SetTitle("neutronTOF");
  // NEUTRON_TOF->Rebin(4);
  NEUTRON_TOF->GetXaxis()->SetRangeUser(0,500);
  NEUTRON_TOF->Draw();
  c1->SaveAs("neutron_TOF.png");

  sprintf(buffChar,
    "gm montage -mode concatenate -tile 2x2 %s %s %s %s ../%s_QDCvsTOF_grid_tmp.png",
    QDCvsTOF_char,
    "neutron_TOF.png",
    NEUTRON_TOF_char,
    "neutron_TOF_backgroundCompare.png",
    isotope_local);
  system(buffChar);

  sprintf(buffChar,
    "gm convert  ../%s_QDCvsTOF_grid_tmp.png -bordercolor Tomato -border 10x10 ../%s_QDCvsTOF_grid.png",
    isotope_local,
    isotope_local);
    system(buffChar);
    system("rm ../*_QDCvsTOF_grid_tmp.png");




  gStyle->SetOptStat(0);

  char BarvsTOF_char[345];
  TH2D* BarvsTOF = new TH2D("BarvsTOF","BarvsTOF",830,-30,200,45,0,45);
  sprintf(buffChar,"%s_BarvsTOF",isotope_local);
  BarvsTOF->SetTitle(buffChar);
  dst_local->Draw("vandle_barNum:vandle_TOF>>BarvsTOF","","colz");
  sprintf(BarvsTOF_char,"../%s_vandle_BarvsTOF.png",isotope_local);
  c1->SaveAs(BarvsTOF_char);

  char BarvsQDC_char[345];
  TH2D* BarvsQDC = new TH2D("BarvsQDC","BarvsQDC",830,-30,400,45,0,45);
  sprintf(buffChar,"%s_BarvsQDC",isotope_local);
  BarvsQDC->SetTitle(buffChar);
  dst_local->Draw("vandle_barNum:vandle_BarQDC>>BarvsQDC","","colz");
  sprintf(BarvsQDC_char,"../%s_vandle_BarvsQDC.png",isotope_local);
  c1->SaveAs(BarvsQDC_char);

  char BarvsTDiff_char[345];
  TH2D* BarvsTDiff = new TH2D("BarvsTDiff","BarvsTDiff",1000,-1000,1000,45,0,45);
  sprintf(buffChar,"%s_BarvsTDiff",isotope_local);
  dst_local->Draw("vandle_barNum:vandle_TDiff>>BarvsTDiff","","colz");
  sprintf(BarvsTDiff_char,"../%s_vandle_BarvsTDiff.png",isotope_local);
  c1->SaveAs(BarvsTDiff_char);
  c1->SetLogz(0);

  char BarvsTDiff_zoom_char[345];
  TH2D* BarvsTDiff_zoom = new TH2D("BarvsTDiff_zoom","BarvsTDiff_zoom",800,-40,40,45,0,45);
  sprintf(BarvsTDiff_zoom_char,"%s_BarvsTDiff_zoom",isotope_local);
  dst_local->Draw("vandle_barNum:vandle_TDiff>>BarvsTDiff_zoom","","colz");
  sprintf(BarvsTDiff_zoom_char,"../%s_vandle_BarvsTDiff_zoom.png",isotope_local);
  c1->SaveAs(BarvsTDiff_zoom_char);

  sprintf(buffChar,
    "gm montage -mode concatenate -tile 3x2 %s %s %s %s %s %s ../%s_grid.png",
    QDCvsTOF_char,
    BarvsQDC_char,
    BarvsTDiff_char,
    NEUTRON_TOF_char,
    BarvsTOF_char,
    BarvsTDiff_zoom_char,
    isotope_local);
  system(buffChar);

  sprintf(buffChar,
    "rm %s %s %s %s %s %s",
    QDCvsTOF_char,
    NEUTRON_TOF_char,
    BarvsTOF_char,
    BarvsQDC_char,
    BarvsTDiff_char,
    BarvsTDiff_zoom_char);
  system(buffChar);

}
