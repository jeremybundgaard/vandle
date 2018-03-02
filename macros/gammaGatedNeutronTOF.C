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


void gammaGatedNeutronTOF(const char *gamma_tree_file = "",const char *vandle_tree_file = "",const char *otherTreeCuts = "1",const char *isotope= "") {

// TGaxis::SetMaxDigits(3);
// gStyle->SetPalette(kCherry);
gStyle->SetOptStat("ne");
gStyle->SetTitleOffset(1.2,"Y");
gStyle->SetNumberContours(99);
// gStyle->SetStatFont(63);
// gStyle->SetStatFontSize(12);

///////// initialize track tree /////////
printf("\n///////// initialize vandle_tree: %s /////////\n\n",vandle_tree_file);
TFile *vandle_TFile = new TFile(vandle_tree_file);
TTree* vandle_tree = (TTree*)vandle_TFile->Get("vandle_tree");

///////// initialize track tree /////////
printf("\n///////// initialize gamma_tree: %s /////////\n\n",gamma_tree_file);
TFile *gamma_TFile = new TFile(gamma_tree_file);
TTree	*HPGe_Tree = (TTree*)gamma_TFile->Get("HPGe_tree");

TH1D* HPGe_hist  = new TH1D("HPGe_hist","HPGe",1000,30,3000);
HPGe_hist->GetXaxis()->SetTitle("#gamma Energy [keV]");
HPGe_hist->SetLineColor(kViolet);
TH1D* betaGated_HPGe_hist = (TH1D*)HPGe_hist->Clone();
betaGated_HPGe_hist->SetLineColor(kBlue);

char buffChar[345];
TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);

HPGe_Tree->Draw("aux_GeEn>>HPGe_hist");
HPGe_Tree->Draw("aux_GeEn>>betaGated_HPGe_hist","aux_GeHasLRBeta","same");
c1->SaveAs("GeEn.png");

// cut 814 keV

}
