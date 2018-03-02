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

void makePlots(TTree* vandle_tree_local,
               TTree* vandle_tree_NEUTRON_local,
               TCutG* tcutg_NEUTRON_local,
               TCutG*  tcutg_BACKGROUND_local,
               const char* isotope_local,
               const char* cutChar_local);

void ornl2016_gammaPlots(const char *gamma_tree_file = "",const char *otherTreeCuts = "1",const char *isotope= "") {

  char buffChar[345];
  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetLogy(1);

  // TGaxis::SetMaxDigits(3);
  // gStyle->SetPalette(kCherry);
  gStyle->SetOptStat("ne");
  gStyle->SetTitleOffset(1.2,"Y");
  gStyle->SetNumberContours(99);
  // gStyle->SetStatFont(63);
  // gStyle->SetStatFontSize(12);
  gStyle->SetLegendTextSize(.03);

  ///////// initialize track tree /////////
  printf("\n///////// initialize gamma_tree /////////\n\n");
  TFile *treefile = new TFile(gamma_tree_file);
  TTree	*LaBr_tree = (TTree*)treefile->Get("LaBr_tree");
  TTree	*NaI_tree = (TTree*)treefile->Get("NaI_tree");
  TTree	*HPGe_tree = (TTree*)treefile->Get("HPGe_tree");

  TH1D* LaBr_hist = new TH1D("LaBr_hist","LaBr",1000,10,6000);
  LaBr_hist->GetXaxis()->SetTitle("#gamma Energy [channel]");
  LaBr_hist->SetLineColor(kGreen);
  LaBr_hist->SetLineWidth(3);
  TH1D* NaI_hist  = new TH1D("NaI_hist","NaI",1000,10,6000);
  NaI_hist->GetXaxis()->SetTitle("#gamma Energy [channel]");
  NaI_hist->SetLineColor(kViolet);
  NaI_hist->SetLineWidth(3);
  TH1D* HPGe_hist = new TH1D("HPGe_hist","HPGe",1000,10,6000);
  HPGe_hist->GetXaxis()->SetTitle("#gamma Energy [channel]");
  HPGe_hist->SetLineColor(kOrange);
  HPGe_hist->SetLineWidth(3);

  // NaI_tree->Draw("aux_NaIEn>>NaI_hist","","",  10000000);
  // LaBr_tree->Draw("aux_LaBrEn>>LaBr_hist","","",3000000);
  // HPGe_tree->Draw("aux_GeEn>>HPGe_hist","","",   100000);

  NaI_tree->Draw("aux_NaIEn>>NaI_hist");
  LaBr_tree->Draw("aux_LaBrEn>>LaBr_hist");
  HPGe_tree->Draw("aux_GeEn>>HPGe_hist");

  auto legend = new TLegend();
  // legend->SetHeader("Gammas from ORNL 2016 runs"); // option "C" allows to center the header
  legend->AddEntry(LaBr_hist,"LaBr");
  legend->AddEntry(NaI_hist,"NaI");
  legend->AddEntry(HPGe_hist,"HPGe");


  THStack *hs = new THStack("hs","Gamma-Ray Detector Type Comparison");
  hs->Add(LaBr_hist);
  hs->Add(NaI_hist);
  hs->Add(HPGe_hist);
  hs->Draw("nostack");
  legend->Draw();

  sprintf(buffChar,"%s_gammas.png",isotope);
  c1->SaveAs(buffChar);


}
