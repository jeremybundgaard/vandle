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

  printf("\ninFileName:%s\n",inFileName);
  printf("outFileName:%s\n",outFileName);
  printf("treeCut:%s\n\n",treeCut);

	TFile* infile = new TFile(inFileName);
	TTree* tracks = (TTree*)infile->Get("tracks");

	TFile *fragTreeFile = new TFile(outFileName,"recreate");
	fragTreeFile->cd();

	TTree* cutTracks = (TTree*)tracks->CopyTree(treeCut);
  cutTracks->Write();

	return outFileName;

}
