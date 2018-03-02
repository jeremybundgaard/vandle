//
// A simple test macro for loading Niffte
// code into ROOT and running it
//
#include <climits>
#include <vector>
#include "TROOT.h"
#include "TRint.h"
#include "TTree.h"
#include "TBranch.h"
#include "Arboretum.h"
#include "TObjArray.h"
#include "NiffteRootIO.h"
#include "NiffteDataBucket.h"
#include "NiffteEventHandle.h"
#include "NiffteEventHandle.h"
#include "NiffteTPCTrack.h"
#include "NiffteTPCDigit.h"
#include "NiffteTPCADCSignal.h"
#include "NiffteMCParticle.h"
#include "NiffteMCHit.h"
#include "NiffteGeometryMap.h"
#include "NiffteAnalysisManager.h"
#include "TPCGeometry.h"
#include "AnaHistoList.h"
#include "AnaH1D.h"
#include "AnaH2D.h"
#include "AnaH2PadPlane.h"
#include "AnaH1DR.h"
#include "AnaH2DR.h"
#include "MsgLog.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2Poly.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaletteAxis.h"
#include "TGraph.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>

NiffteGeometryMap* ngm = NiffteGeometryMap::Instance("v2_03");
TPCGeometry* geo = TPCGeometry::GetInstance();
//__________________________________________________________________
void makeTrees(const char* infile = "infile-list.txt", const char* outfile="Analysis-RunXXX.root", int nevt = -1) {

  //ngm->SetDriftParameters(5.0,5.0,50.0);
  //geo->SetNExtraBuckets(200);
  ngm->SetBeamDirection(1);
  //Niffte Analysis------------------------------
  //NiffteGeometryMap* ngm = NiffteGeometryMap::Instance("v2_03");
  std::string geoMap("v2_03");
  std::string ofile(outfile);
  NiffteAnalysisManager aMgr(infile,"foo.root",geoMap);

  TFile* fout = new TFile(outfile,"RECREATE");
  TTree *tracks = new TTree("tracks","Data Tree");
  //----------------------------------------------------------
  double trkADC = 0;
  double trkL = 0;
  double cosTheta = 0;
  double phi = 0;
  double xVertex = 0;
  double yVertex = 0;
  double zVertex = 0;
  double xEnd = 0;
  double yEnd = 0;
  double zEnd = 0;
  int vol = 0;
  int eventID = 0;

  tracks->Branch("trkADC",&trkADC,"trkADC/D");
  tracks->Branch("trkL",&trkL,"trkL/D");
  tracks->Branch("cosTheta",&cosTheta,"cosTheta/D");
  tracks->Branch("phi",&phi,"phi/D");
  tracks->Branch("xVertex",&xVertex,"xVertex/D");
  tracks->Branch("yVertex",&yVertex,"yVertex/D");
  tracks->Branch("xEnd",&xEnd,"xEnd/D");
  tracks->Branch("yEnd",&yEnd,"yEnd/D");
  tracks->Branch("vol",&vol,"vol/I");
  tracks->Branch("eventID",&eventID,"eventID/I");
  //-----------------------------------------------------------

  //Open file list_____________________________________________
  if(nevt == -1) nevt = INT_MAX;
  int n = 0, nSkip = 0, SparseFactor = 0;
  
  aMgr.Execute(nevt,nSkip,SparseFactor);
  TPCResult_t status;
  while (true){
    status = aMgr.NextEvent();
    if (status != kTPCSuccess && status != kTPCNewRun) break;
    if(n%10000==0) {
      cout << "### Event " << aMgr.EventNumber() << " ###" << endl;
      }
    //Tracks_____________________________________________
    for(int iTrk = 0; iTrk < aMgr.NTracks(); iTrk++){
      const NiffteTPCTrack* currtrk = aMgr.GetTrack(iTrk);
      const NiffteTPCADCSignal *currsig = aMgr.GetSignal(iTrk);
      int fitPCA;
      for(int fc = 0; fc < currtrk->FitCount(); fc++){
      	if(currtrk->Fitted(fc) == kFitPCA) fitPCA = fc;
      }
      if(currtrk->FitCount() > 0){
        
      	NiffteTPCTrackFit *currfit = currtrk->GetTrackFit(fitPCA);
      	trkADC = 0;
      	trkL = currtrk->Length();
      	xVertex = currtrk->TrackStartPoint(0,fitPCA);
      	zVertex = currtrk->TrackStartPoint(2,fitPCA);
      	xEnd = currtrk->TrackEndPoint(0,fitPCA);
      	yEnd = currtrk->TrackEndPoint(1,fitPCA);
      	zEnd = currtrk->TrackEndPoint(2,fitPCA);
      	TVector3 direction(
          currtrk->TrackDirection(0,fitPCA),
      		currtrk->TrackDirection(1,fitPCA),
      		currtrk->TrackDirection(2,fitPCA));
      	cosTheta = cos(direction.Theta());
      	phi = direction.Phi();
      	eventID = aMgr.EventNumber();
      	//--Track Digits-------------------------------------------
      	Int_t* dlist = currtrk->Digits();
      	int Vol0 = 0, Vol1 = 0;
      	for (int id = 0; id < currtrk->Ndigits(); id++){
      	  const NiffteTPCDigit* dig = aMgr.GetDigit(dlist[id]);
      	  double xdig = 0, ydig = 0;
      	  ngm->MapVolRowColtoXY(
            dig->Volume(),
            dig->PadRow(),
            dig->Column(),
      			xdig,
            ydig);
      	  
          trkADC += dig->ADC();
      	  if(dig->Volume() == 1){
      	    Vol1++;
      	  }
      	  else if(dig->Volume() == 0){
      	    Vol0++;
      	  }
      	}
      	if(Vol0 > 0 && Vol1 == 0) vol = 0;
      	else if(Vol0 == 0 && Vol1 > 0) vol = 1;
      	else if(Vol0 > 0 && Vol1 > 0) vol = 3;

      	tracks->Fill();
      }
    }//--Tracks-----------------------------------------*/
    ++n;
    /*/---------------------------------
    if(writeEvt > 0) aMgr.SaveEvent(0);
    //----------------------------------*/
  }	
  aMgr.Terminate();

  fout->cd();
  tracks->Write();
  tracks->Print();
  fout->Write();
  fout->Close();

  //return;
  gApplication->Terminate();
}
