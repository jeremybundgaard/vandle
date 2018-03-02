#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"


void makeFitType5trees(const char* inFileName, const char* outFileName){


TFile* infile = new TFile(inFileName);
TTree* tracks = (TTree*)infile->Get("tracks");

TFile *fragTreeFile = new TFile(outFileName,"recreate");
fragTreeFile->cd();

char treeCut[345] = "fitType==5";
TTree *cutTracks = (TTree*)tracks->CopyTree(treeCut);

cutTracks->Write();

}
