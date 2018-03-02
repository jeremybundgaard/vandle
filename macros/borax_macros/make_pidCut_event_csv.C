#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <set>

void make_pidCut_event_csv(const char* inFileName,const char* pidFileName)
{

  printf("\ninFileName:%s\n",inFileName);
  printf("pidFileName:%s\n",pidFileName);

  TFile* pidFile = new TFile(pidFileName);
  TCutG* fragCut_adc = (TCutG*)pidFile->Get("fragCut_adc");
  TCutG* fragCut_unlikely_adc = (TCutG*)pidFile->Get("fragCut_unlikely_adc");
  TCutG* alphaCut_zCut_adc = (TCutG*)pidFile->Get("alphaCut_zCut_adc");
  TCutG* alphaCut_long_low_adc = (TCutG*)pidFile->Get("alphaCut_long_low_adc");
  TCutG* alphaCut_long_adc = (TCutG*)pidFile->Get("alphaCut_long_adc");
  TCutG* long_high_adc = (TCutG*)pidFile->Get("long_high_adc");
  TCutG* alphaCut_short_higher_adc = (TCutG*)pidFile->Get("alphaCut_short_higher_adc");
  TCutG* alphaCut_short_low_adc = (TCutG*)pidFile->Get("alphaCut_short_low_adc");

	TFile* infile = new TFile(inFileName);
	TTree* tracks = (TTree*)infile->Get("tracks");
  bool is_ghostTail;
  bool has_ghostTail;
  short trackNo;
  short trackCount;
  short fitType;
  short nDigits;
  unsigned int run;
  int eventID;
  float length;
  float adc;
  float energy;
  TVector3* start = new TVector3();
  TVector3* end = new TVector3();
  TVector3* direction = new TVector3();

  tracks->SetBranchAddress("run",&run);
  tracks->SetBranchAddress("eventID",&eventID);
  tracks->SetBranchAddress("start",&start);
  tracks->SetBranchAddress("end",&end);
  tracks->SetBranchAddress("direction",&direction);
  tracks->SetBranchAddress("length",&length);
  tracks->SetBranchAddress("adc",&adc);
  tracks->SetBranchAddress("fitType",&fitType);
  tracks->SetBranchAddress("trackNo",&trackNo);
  tracks->SetBranchAddress("trackCount",&trackCount);
  tracks->SetBranchAddress("nDigits",&nDigits);

  ofstream fragCut_adc_events_csvfile;
  ofstream fragCut_unlikely_adc_events_csvfile;
  ofstream alphaCut_zCut_adc_events_csvfile;
  ofstream alphaCut_long_low_adc_events_csvfile;
  ofstream alphaCut_long_adc_events_csvfile;
  ofstream long_high_adc_events_csvfile;
  ofstream alphaCut_short_higher_adc_events_csvfile;
  ofstream alphaCut_short_low_adc_events_csvfile;
  ofstream blobs_events_csvfile;

  fragCut_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/fragCut_adc_events.csv");
  fragCut_unlikely_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/fragCut_unlikely_adc_events.csv");
  alphaCut_zCut_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_zCut_adc_events.csv");
  alphaCut_long_low_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_long_low_adc_events.csv");
  alphaCut_long_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_long_adc_events.csv");
  long_high_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/long_high_adc_events.csv");
  alphaCut_short_higher_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_short_higher_adc_events.csv");
  alphaCut_short_low_adc_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_short_low_adc_events.csv");
  blobs_events_csvfile.open("/p/lscratche/jerbundg/analysis/pid/eventID_csv/blobs_events.csv");

  for (int entry = 0; entry < tracks->GetEntries(); entry++) {
  	tracks->GetEntry(entry);
    if( (-1<direction->Phi()&&direction->Phi()<0) || fitType!=5) continue;
    if(entry%100000==0) printf("\t entry:%d \t run:%d \t eventID:%d \n",entry,run,eventID);
    //////////////////////  fill csv files /////////////////////
    if(fragCut_adc->IsInside(adc,length)==1)fragCut_adc_events_csvfile<<run<<eventID<<'\n';
    if(fragCut_unlikely_adc->IsInside(adc,length)==1)fragCut_unlikely_adc_events_csvfile<<run<<eventID<<'\n';
    if(alphaCut_zCut_adc->IsInside(adc,length)==1)alphaCut_zCut_adc_events_csvfile<<run<<eventID<<'\n';
    if(alphaCut_long_low_adc->IsInside(adc,length)==1)alphaCut_long_low_adc_events_csvfile<<run<<eventID<<'\n';
    if(alphaCut_long_adc->IsInside(adc,length)==1)alphaCut_long_adc_events_csvfile<<run<<eventID<<'\n';
    if(long_high_adc->IsInside(adc,length)==1)long_high_adc_events_csvfile<<run<<eventID<<'\n';
    if(alphaCut_short_higher_adc->IsInside(adc,length)==1)alphaCut_short_higher_adc_events_csvfile<<run<<eventID<<'\n';
    if(alphaCut_short_low_adc->IsInside(adc,length)==1)alphaCut_short_low_adc_events_csvfile<<run<<eventID<<'\n';
    if(length<1&&adc<5000&&2.967<direction->Theta()&&direction->Theta()<3.141)blobs_events_csvfile<<run<<eventID<<'\n';
  }

  fragCut_adc_events_csvfile.close();
  fragCut_unlikely_adc_events_csvfile.close();
  alphaCut_zCut_adc_events_csvfile.close();
  alphaCut_long_low_adc_events_csvfile.close();
  alphaCut_long_adc_events_csvfile.close();
  long_high_adc_events_csvfile.close();
  alphaCut_short_higher_adc_events_csvfile.close();
  alphaCut_short_low_adc_events_csvfile.close();
  blobs_events_csvfile.close();

}
