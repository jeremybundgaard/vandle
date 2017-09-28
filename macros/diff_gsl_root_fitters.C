
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
#include "TProfile.h"
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


void diff_gsl_root_fitters(const char *dst_root_fitter = "",const char *dst_gsl_fitter = "",const char *otherTreeCuts = "1") {

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetGrid();
  // c1->SetLogz();
  TGaxis::SetMaxDigits(3);
  // gStyle->SetPalette(kAvocado);
  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);
  gStyle->SetOptFit(1);

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize dst /////////\n\n");
  TFile *root_treefile = new TFile(dst_root_fitter);
  TTree *dst_root = (TTree *)root_treefile->Get("data_summary_tree");
  // dst->SetLineColor(kMagenta);
  dst_root->SetLineWidth(4);

  TH2D* root_vandle_QDCvsTOF = new TH2D("root_vandle_QDCvsTOF","ROOT vandle_QDCvsTOF",500,-60,460,180,0,1800);
  root_vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [nsec]");
  root_vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
  dst_root->Draw("vandle_QDC:vandle_TOF>>root_vandle_QDCvsTOF","vandle_QDC!=0","colz");
  c1->SaveAs("root_fitter_vandle_QDCvsTOF.png");

  TH2D* root_vandle_barNum_VS_TDiff = new TH2D("root_vandle_barNum_VS_TDiff","ROOT vandle_barNum_VS_TDiff",500,-30,30,30,0,30);
  root_vandle_barNum_VS_TDiff->GetXaxis()->SetTitle("TDiff [nsec]");
  root_vandle_barNum_VS_TDiff->GetYaxis()->SetTitle("VANDLE bar");
  dst_root->Draw("vandle_barNum:vandle_TDiff>>root_vandle_barNum_VS_TDiff","vandle_QDC!=0","colz");
  c1->SaveAs("root_fitter_vandle_barNum_VS_TDiff.png");

  ///////// initialize track tree /////////
  printf("\n///////// initialize dst /////////\n\n");
  TFile *gsl_treefile = new TFile(dst_gsl_fitter);
  TTree *dst_gsl = (TTree *)gsl_treefile->Get("data_summary_tree");
  // dst->SetLineColor(kMagenta);
  dst_gsl->SetLineWidth(4);

  TH2D* gsl_vandle_QDCvsTOF = new TH2D("gsl_vandle_QDCvsTOF","GSL vandle_QDCvsTOF",500,-60,460,180,0,1800);
  gsl_vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [nsec]");
  gsl_vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
  dst_gsl->Draw("vandle_QDC:vandle_TOF>>gsl_vandle_QDCvsTOF","vandle_QDC!=0","colz");
  c1->SaveAs("gsl_fitter_vandle_QDCvsTOF.png");

  TH2D* gsl_vandle_barNum_VS_TDiff = new TH2D("gsl_vandle_barNum_VS_TDiff","GSL vandle_barNum_VS_TDiff",500,-30,30,30,0,30);
  gsl_vandle_barNum_VS_TDiff->GetXaxis()->SetTitle("TDiff [nsec]");
  gsl_vandle_barNum_VS_TDiff->GetYaxis()->SetTitle("VANDLE bar");
  dst_gsl->Draw("vandle_barNum:vandle_TDiff>>gsl_vandle_barNum_VS_TDiff","vandle_QDC!=0","colz");
  c1->SaveAs("gsl_fitter_vandle_barNum_VS_TDiff.png");

  TH2D* diff_QDCvsTOF = (TH2D*)gsl_vandle_QDCvsTOF->Clone("diff_QDCvsTOF");
  diff_QDCvsTOF->Scale(-1);
  diff_QDCvsTOF->Add(root_vandle_QDCvsTOF);
  diff_QDCvsTOF->Draw("colz");
  c1->SaveAs("diff_QDCvsTOF.png");

  TH2D* barNum_VS_TDiff = (TH2D*)gsl_vandle_barNum_VS_TDiff->Clone("barNum_VS_TDiff");
  barNum_VS_TDiff->Scale(-1);
  barNum_VS_TDiff->Add(root_vandle_barNum_VS_TDiff);
  barNum_VS_TDiff->Draw("colz");
  c1->SaveAs("diff_barNum_VS_TDiff.png");


  sprintf(buffChar,"gm montage -mode concatenate -tile 3x1 gsl_fitter_vandle_barNum_VS_TDiff.png root_fitter_vandle_barNum_VS_TDiff.png diff_barNum_VS_TDiff.png barNum_VS_TDiff.png");
  system(buffChar);
  sprintf(buffChar,"rm gsl_fitter_vandle_barNum_VS_TDiff.png root_fitter_vandle_barNum_VS_TDiff.png");
  system(buffChar);

  sprintf(buffChar,"gm montage -mode concatenate -tile 3x1 gsl_fitter_vandle_QDCvsTOF.png root_fitter_vandle_QDCvsTOF.png diff_QDCvsTOF.png QDC_vs_TOF.png");
  system(buffChar);
  sprintf(buffChar,"rm gsl_fitter_vandle_QDCvsTOF.png root_fitter_vandle_QDCvsTOF.png");
  system(buffChar);

  sprintf(buffChar,"gm montage -mode concatenate -tile 1x2 QDC_vs_TOF.png barNum_VS_TDiff.png grid.png");
  system(buffChar);

}
