
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


void sim_scinton_pathlength(const char *sim_tree = "",const char *otherTreeCuts = "1") {


  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetGrid();
  c1->SetLogz();
  TGaxis::SetMaxDigits(3);
  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);
  gStyle->SetOptFit(1);
  gROOT->ProcessLine("#include <vector>");

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize sim_tree /////////\n\n");
  TFile *treefile = new TFile(sim_tree);
  TTree *dst = (TTree *)treefile->Get("PMTHitInfo");
  // dst->SetLineColor(kMagenta);
  dst->SetLineWidth(4);

  int eventID=0;
  int detID=0;
  int pmtID=0;
  double time=0;
  int trackId=0;
  dst->SetBranchAddress("eventID",&eventID);
  dst->SetBranchAddress("detID",&detID);
  dst->SetBranchAddress("pmtID",&pmtID);
  dst->SetBranchAddress("time",&time);
  dst->SetBranchAddress("trackId",&trackId);

  std::vector<int> * stepIds = 0;
  std::vector<double> * times = 0;
  std::vector<double> * xPos = 0;
  std::vector<double> * yPos = 0;
  std::vector<double> * zPos = 0;
  dst->SetBranchAddress("stepIds",&stepIds);
  dst->SetBranchAddress("times",&times);
  dst->SetBranchAddress("xPos",&xPos);
  dst->SetBranchAddress("yPos",&yPos);
  dst->SetBranchAddress("zPos",&zPos);

  printf("map fill loop\n");
  std::set<int> eventID_set;
  std::map<int,std::set<int>> map_eventID_detID;
  std::map<int,std::set<int>> map_eventID_pmtID;
  std::map<int,std::set<double>> map_eventID_time;
  std::map<int,std::set<int>> map_eventID_trackId;

  for (int treeEntry = 0; treeEntry < dst->GetEntries(); treeEntry++){

  dst->GetEntry(treeEntry);

  eventID_set.insert(eventID);
  map_eventID_detID[eventID].insert(detID);
  map_eventID_pmtID[eventID].insert(pmtID);
  map_eventID_time[eventID].insert(time);
  map_eventID_trackId[eventID].insert(trackId);

  // printf("eventID[%d]: %d\n",treeEntry,eventID);
  }

  char evtNumber_char[345]="";
  printf("eventDisplay loop\n");

  for (auto loop_eventID_set : eventID_set){

  }
}
