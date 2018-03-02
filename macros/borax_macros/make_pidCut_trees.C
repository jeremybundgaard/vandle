#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TTree.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <set>

const char* make_pidCut_trees(const char* inFileName,const char* outFileName, const char* pidFileName, const char* pidCut, const char* dataType="")
{

  printf("\n\n!(strcmp(dataType,\"tracks\")):%d\n",!(strcmp(dataType,"tracks")));
  printf("!(strcmp(dataType,\"digits\")):%d\n",!(strcmp(dataType,"digits")));
  printf("dataType:%s",dataType);
  printf("\ninFileName:%s\n",inFileName);
  printf("outFileName:%s\n",outFileName);
  printf("pidCut:%s\n\n",pidCut);

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
  int dig_row;
  int dig_column;
  Double_t dig_x;
  Double_t dig_y;
  Double_t dig_z;
  Double_t dig_adc;
  int dig_bucket;
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
  if ( !(strcmp(dataType,"digits")) ){
    printf("\n\n**********************dataType:%s\n\n",dataType);
    tracks->SetBranchAddress("dig_row",&dig_row);
    tracks->SetBranchAddress("dig_column",&dig_column);
    tracks->SetBranchAddress("dig_x",&dig_x);
    tracks->SetBranchAddress("dig_y",&dig_y);
    tracks->SetBranchAddress("dig_z",&dig_z);
    tracks->SetBranchAddress("dig_adc",&dig_adc);
    tracks->SetBranchAddress("dig_bucket",&dig_bucket);
  }

	TFile *fragTreeFile = new TFile(outFileName,"recreate");
	fragTreeFile->cd();

  char finalCut[567];
  sprintf(finalCut,"%s&&!(-1<direction.Phi()&&direction.Phi()<0)",pidCut);

  TTree* cutTracks = new TTree("tracks","tracks");
  cutTracks->Branch("run",&run);
  cutTracks->Branch("eventID",&eventID);
  cutTracks->Branch("start",&start);
  cutTracks->Branch("end",&end);
  cutTracks->Branch("direction",&direction);
  cutTracks->Branch("length",&length);
  cutTracks->Branch("adc",&adc);
  cutTracks->Branch("fitType",&fitType);
  cutTracks->Branch("trackNo",&trackNo);
  cutTracks->Branch("trackCount",&trackCount);
  cutTracks->Branch("nDigits",&nDigits);
  if ( !(strcmp(dataType,"digits")) ){
    printf("\n\n**********************dataType:%s\n\n",dataType);
    cutTracks->Branch("dig_row",&dig_row);
    cutTracks->Branch("dig_column",&dig_column);
    cutTracks->Branch("dig_x",&dig_x);
    cutTracks->Branch("dig_y",&dig_y);
    cutTracks->Branch("dig_z",&dig_z);
    cutTracks->Branch("dig_adc",&dig_adc);
    cutTracks->Branch("dig_bucket",&dig_bucket);
  }

  std::set<int> eventID_set;
  std::set<int>::iterator iter_eventID_set;
  for (int entry = 0; entry < tracks->GetEntries(); entry++) {
  	tracks->GetEntry(entry);
    iter_eventID_set=eventID_set.find(eventID);
    // if(run!=100002055)break;
    if( (-1<direction->Phi()&&direction->Phi()<0) || fitType!=5) continue;
    if(eventID_set.size()%1000==0) printf("\t entry:%d \t run:%d \t eventID:%d \t events_written:%lu \n",entry,run,eventID,eventID_set.size());

    if((*iter_eventID_set==eventID && !(strcmp(pidCut,"fragCut_adc")))||(!(strcmp(pidCut,"fragCut_adc")) && fragCut_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  	if((*iter_eventID_set==eventID && !(strcmp(pidCut,"fragCut_unlikely_adc")))||(!(strcmp(pidCut,"fragCut_unlikely_adc")) && fragCut_unlikely_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  	if((*iter_eventID_set==eventID && !(strcmp(pidCut,"alphaCut_zCut_adc")))||(!(strcmp(pidCut,"alphaCut_zCut_adc")) && alphaCut_zCut_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  	if((*iter_eventID_set==eventID && !(strcmp(pidCut,"alphaCut_long_low_adc")))||(!(strcmp(pidCut,"alphaCut_long_low_adc")) && alphaCut_long_low_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  	if((*iter_eventID_set==eventID && !(strcmp(pidCut,"alphaCut_long_adc")))||(!(strcmp(pidCut,"alphaCut_long_adc")) && alphaCut_long_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  	if((*iter_eventID_set==eventID && !(strcmp(pidCut,"long_high_adc")))||(!(strcmp(pidCut,"long_high_adc")) && long_high_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  	if((*iter_eventID_set==eventID && !(strcmp(pidCut,"alphaCut_short_higher_adc")))||(!(strcmp(pidCut,"alphaCut_short_higher_adc")) && alphaCut_short_higher_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
    if((*iter_eventID_set==eventID && !(strcmp(pidCut,"alphaCut_short_low_adc")))||(!(strcmp(pidCut,"alphaCut_short_low_adc")) && alphaCut_short_low_adc->IsInside(adc,length)==1)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
    if((*iter_eventID_set==eventID && !(strcmp(pidCut,"blobs")))||(!(strcmp(pidCut,"blobs")) && length<1&&adc<5000 && 2.967<direction->Theta()&&direction->Theta()<3.141)){
      eventID_set.insert(eventID);
      cutTracks->Fill();
    }
  }

  cutTracks->Write();
	return outFileName;

}
