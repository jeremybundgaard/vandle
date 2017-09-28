#include "TAxis.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPaveStats.h"

#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>


void scan_walk(const char *data_summary_tree = "",const char *otherTreeCuts = "1") {

  // TGaxis::SetMaxDigits(3);
  gStyle->SetPalette(kCherry);
  // gStyle->SetOptStat("n");
  gStyle->SetTitleOffset(1.2,"Y");
  gStyle->SetNumberContours(99);

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize data_summary_tree /////////\n\n");
  TFile *treefile = new TFile(data_summary_tree);
  TTree *dst = (TTree *)treefile->Get("data_summary_tree");
  // dst->SetLineColor(kBlue);
  dst->SetLineWidth(6);

  unsigned int evtNumber=0;
  unsigned int vandle_barNum=0;
  double vandle_QDC=0;
  double vandle_TOF=0;
  double vandle_TDiff=0;
  double beta_lAmp=0;
  double vandle_lAmp=0;

  dst->SetBranchAddress("evtNumber",&evtNumber);
  dst->SetBranchAddress("vandle_barNum",&vandle_barNum);
  dst->SetBranchAddress("vandle_QDC",&vandle_QDC);
  dst->SetBranchAddress("vandle_TOF",&vandle_TOF);
  dst->SetBranchAddress("vandle_TDiff",&vandle_TDiff);
  dst->SetBranchAddress("vandle_TDiff",&vandle_TDiff);
  dst->SetBranchAddress("beta_lAmp",&beta_lAmp);
  dst->SetBranchAddress("vandle_lAmp",&vandle_lAmp);

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetLogz();

  TH2D* vandle_walk = new TH2D("vandle_walk","vandle_walk",400,-20,20,400,0,4000);
  vandle_walk->SetStats(0);
  vandle_walk->GetXaxis()->SetTitle("TDiff [nsec]");
  vandle_walk->GetYaxis()->SetTitle("left VANDLE amplitudes [ADC]");

  for (int start_amp = 0; start_amp < 3000; start_amp+=50) {


    int beta_lAmp_cut_MIN = start_amp;
    int beta_lAmp_cut_MAX = start_amp + 100;
    sprintf(buffChar,"vandle_QDC!=0 && %d < beta_lAmp  && beta_lAmp < %d",beta_lAmp_cut_MIN, beta_lAmp_cut_MAX);
    dst->Draw("0.5*(vandle_lAmp+vandle_rAmp):vandle_TDiff>>vandle_walk",buffChar,"colz");
    vandle_walk->Draw("colz");
    vandle_walk->SetTitle(buffChar);
    sprintf(buffChar,"vandle_amp_vs_tdiff_betaAmpABOVE%d.png",start_amp);
    c1->SaveAs(buffChar);

  }
}
