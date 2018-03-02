#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "/g/g21/jerbundg/niffte/trunk/common/geometry/NiffteGeometryMap.h"
#include "/g/g21/jerbundg/niffte/trunk/common/db/NiffteCalibManager.h"
#include "AnaH2PadPlane.h"
#include "TFile.h"
#include "TF1.h"
#include "TPaletteAxis.h"
#include "Arboretum.h"


void make_accumADC(){
  TCanvas* c1 = new TCanvas("c1","c1",1100,1000);
  c1->SetCanvasSize(1000,1000);
  char c1PlotName[234];
  // gStyle->SetTitleAlign(22);
  gStyle->SetTitleX(.4);
  gStyle->SetTitleY(.95);
  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(999);
  // gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.13);
  gPad->SetTopMargin(0.13);
  // gPad->SetBottomMargin(0.15);
  char buffChar[345];

  TFile* cm244_file = new TFile("trees/digTrees/cm244.nEv10000.driftVel4.4.bsDigitTrees_RawToHDKK.29Jan17_0643.03/noMask/1stPreamp/niffte-run100001922_20Jun2013_11-04.trees.root")
  TTree* digits = new (TTree*)cm244_file->Get("digits");
  AnaH2PadPlane *padplane_xy = new AnaH2PadPlane(NiffteGeometryMap::Instance(),"padplane_xy"," ",0);

  double x=0;
  double y=0;
  double adc=0;

  tracks->SetBranchAddress("x",&x);
  tracks->SetBranchAddress("y",&y);
  tracks->SetBranchAddress("adc",&adc);
  for (int treeEntry = 0; treeEntry < tracks->GetEntries(); treeEntry++){
    digits->GetEntry(treeEntry);
    padplane_xy->Fill(x,y,adc);
  }

  padplane_xy->Draw("colz");
  c1->SaveAs("accumADC_cm244.png");

}
