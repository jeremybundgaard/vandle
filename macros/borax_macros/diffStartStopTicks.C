#include "TRint.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include <iostream>
#include <fstream>
#include <stdio.h>


void diffStartStopTicks(const char* treeFileName) {

	TFile* trackFile = new TFile(treeFileName);
	TTree* tracks = (TTree*)trackFile->Get("tracks");

	Int_t nentries = tracks->GetEntries();
	ULong64_t tick;
	tracks->SetBranchAddress("tick",&tick);
	UInt_t runNumber;
	tracks->SetBranchAddress("run",&runNumber);
	tracks->GetEntry(0);
	ULong64_t startTick = tick;
	cout<<"runNumber = "<<runNumber<<endl;
	cout<<"startTick = "<<startTick<<endl;


	tracks->GetEntry(nentries-1);
	ULong64_t stopTick = tick;
	cout<<"stopTick = "<<stopTick<<endl;


	ULong64_t diffTick   = stopTick-startTick;
	cout<<"diffTick = "<<diffTick<<endl;


	Double_t runtime   = diffTick/(double)50000000;
	cout<<"runtime = "<<runtime<<endl;


	char runNumChar[324];
	sprintf(runNumChar,"%d.difftick",runNumber);
	cout<<runNumChar<<endl;
	ofstream outFile;
	outFile.open(runNumChar);
	outFile<<"startTick = "<<startTick<<endl;
	outFile<<"stopTick = "<<stopTick<<endl;
	outFile<<"diffTick = "<<diffTick<<endl;
	outFile<<"runtime = "<<runtime<<endl;

	outFile.close();

}
