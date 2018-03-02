#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

const char* makeNewTree(const char* inFileName,const char* outFileName, const char* treeCut)
{

  char pidFileName[345]="";
  sprintf(pidFileName,"/p/lscratche/jerbundg/analysis/pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.42.root");
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
	TTree* tracks = (TTree*)infile->Get("tracks_corrected");

	TFile *fragTreeFile = new TFile(outFileName,"recreate");
	fragTreeFile->cd();

	TTree* cutTracks = (TTree*)tracks->CopyTree(treeCut);
	cutTracks->Write();

	std::cout<<"outFileName: "<<outFileName<<std::endl;
	return outFileName;

}
