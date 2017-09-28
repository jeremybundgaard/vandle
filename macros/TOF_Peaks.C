
#include "TAxis.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TMarker.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TPolyLine.h"
#include "TPolyLine3D.h"
#include "TROOT.h"
#include "TRint.h"
#include "TStyle.h"
#include "TText.h"
#include "TTree.h"
#include "TVector3.h"
#include "TView.h"

#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>


void TOF_Peaks(const char *data_summary_tree = "",const char *otherTreeCuts = "1") {

  TCanvas *wideCanvas = new TCanvas("wideCanvas", "wideCanvas",3000, 1200);
  wideCanvas->SetGrid();
  wideCanvas->SetLogz();
  TGaxis::SetMaxDigits(4);
  gStyle->SetPalette(kCherry);
  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize data_summary_tree /////////\n\n");
  TFile *treefile = new TFile(data_summary_tree);
  TTree *dst = (TTree *)treefile->Get("data_summary_tree");
  // dst->SetLineColor(kBlue);
  dst->SetLineWidth(4);


  unsigned int evtNumber=0;
  unsigned int vandle_barNum=0;
  double vandle_BarQDC=0;
  double vandle_BarCorTof=0;
  double vandle_TDiff=0;
  // dst->SetBranchAddress("evtNumber",&evtNumber);
  // dst->SetBranchAddress("vandle_barNum",&vandle_barNum);
  dst->SetBranchAddress("vandle_BarQDC",&vandle_BarQDC);
  dst->SetBranchAddress("vandle_BarCorTof",&vandle_BarCorTof);
  // dst->SetBranchAddress("vandle_TDiff",&vandle_TDiff);

  TCutG *tcutg_gma = new TCutG("tcutg_gma",5);
  tcutg_gma->SetVarX("vandle_BarCorTof");
  tcutg_gma->SetVarY("vandle_BarQDC");
  tcutg_gma->SetLineWidth(7);
  tcutg_gma->SetLineColor(kMagenta);
  tcutg_gma->SetPoint(0,-20,1);
  tcutg_gma->SetPoint(1,-20,5900);
  tcutg_gma->SetPoint(2,40,5900);
  tcutg_gma->SetPoint(3,40,1);
  tcutg_gma->SetPoint(4,-20,1);

  TCutG *tcutg_fast_nu = new TCutG("tcutg_fast_nu",5);
  tcutg_fast_nu->SetVarX("vandle_BarCorTof");
  tcutg_fast_nu->SetVarY("vandle_BarQDC");
  tcutg_fast_nu->SetLineWidth(7);
  tcutg_fast_nu->SetLineColor(kGreen);
  tcutg_fast_nu->SetPoint(0,45,1);
  tcutg_fast_nu->SetPoint(1,45,5000);
  tcutg_fast_nu->SetPoint(2,80,5000);
  tcutg_fast_nu->SetPoint(3,80,1);
  tcutg_fast_nu->SetPoint(4,45,1);

  TCutG *tcutg_slower_nu = new TCutG("tcutg_slower_nu",5);
  tcutg_slower_nu->SetVarX("vandle_BarCorTof");
  tcutg_slower_nu->SetVarY("vandle_BarQDC");
  tcutg_slower_nu->SetLineWidth(7);
  tcutg_slower_nu->SetLineColor(kOrange);
  tcutg_slower_nu->SetPoint(0,95,1);
  tcutg_slower_nu->SetPoint(1,95,2000);
  tcutg_slower_nu->SetPoint(2,390,1500);
  tcutg_slower_nu->SetPoint(3,390,1);
  tcutg_slower_nu->SetPoint(4,95,1);
  tcutg_slower_nu->SetPoint(5,95,1);


  TH2D* vandle_QDCvsTOF = new TH2D("vandle_QDCvsTOF","vandle_QDCvsTOF",400,-50,400,6000,0,6000);
  vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [nsec]");
  vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
  dst->Draw("vandle_BarQDC:vandle_BarCorTof>>vandle_QDCvsTOF","","colz");
  wideCanvas->SetLogz(1);
  vandle_QDCvsTOF->SetTitle("");
  vandle_QDCvsTOF->Draw("colz");
  sprintf(buffChar,"vandle_QDCvsTOF.svg");
  wideCanvas->SaveAs(buffChar);
  tcutg_gma->Draw("same");
  tcutg_fast_nu->Draw("same");
  tcutg_slower_nu->Draw("same");
  sprintf(buffChar,"vandle_QDCvsTOF.png");
  wideCanvas->SaveAs(buffChar);

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetGrid();
  c1->SetLogz();

  TCutG* square = new TCutG("square ",5);
  square->SetLineWidth(7);
  square->SetPoint(0,45,300);
  square->SetPoint(1,45,3000);
  square->SetPoint(2,95,3000);
  square->SetPoint(3,95,300);
  square->SetPoint(4,45,300);
  square->SetLineColor(kGreen);
  dst->Draw("vandle_ltrace:Iteration$>>left_trace_hist(50,45,95,270,300,3000)","tcutg_fast_nu","colz");
  square->Draw("same");
  sprintf(buffChar,"fast_nu_trace.png");
  c1->SaveAs(buffChar);
  square->SetLineColor(kOrange);
  dst->Draw("vandle_ltrace:Iteration$>>left_trace_hist(50,45,95,270,300,3000)","tcutg_slower_nu","colz");
  square->Draw("same");
  sprintf(buffChar,"slower_nu_trace.png");
  c1->SaveAs(buffChar);
  square->SetLineColor(kMagenta);
  dst->Draw("vandle_ltrace:Iteration$>>left_trace_hist(50,45,95,270,300,3000)","tcutg_gma","colz");
  square->Draw("same");
  sprintf(buffChar,"gma_trace.png");
  c1->SaveAs(buffChar);


  sprintf(buffChar,"gm montage -mode concatenate -tile 3x1 gma_trace.png fast_nu_trace.png slower_nu_trace.png trace_TOF_Peakstmp.png");
  system(buffChar);
  sprintf(buffChar,"rm gma_trace.png fast_nu_trace.png slower_nu_trace.png");
	system(buffChar);
  sprintf(buffChar,"gm montage -mode concatenate -tile 1x2 vandle_QDCvsTOF.png trace_TOF_Peakstmp.png trace_TOF_Peaks.png");
  system(buffChar);
  sprintf(buffChar,"rm vandle_QDCvsTOF.png trace_TOF_Peakstmp.png");
  system(buffChar);


}
