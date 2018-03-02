
// NIFFTE Libraries
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
#include "AnaHistoList.h"
#include "AnaH1D.h"
#include "AnaH2D.h"
#include "AnaH2PadPlane.h"
#include "AnaH1DR.h"
#include "AnaH2DR.h"
#include "MsgLog.h"

// ROOT libraries
#include "TROOT.h"
#include "TRint.h"
#include "TObjArray.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2Poly.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaletteAxis.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCutG.h"

// C++ libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>
#include <climits>
#include <vector>


TObjArray *sigArray;
TObjArray *digArray;
TObjArray *trkArray;

NiffteGeometryMap* ngm = NiffteGeometryMap::Instance("v2_03");

void anaHistMaker(const char* infile = "infile-list.txt", const char* outfile="Analysis-RunXXX.root", int nevt = -1) {

//Niffte Analysis------------------------------
std::string geo("v2_03");
std::string ofile(outfile);
NiffteAnalysisManager aMgr(infile,ofile,geo);
          
//__Define some histograms___________________________________________  
TH1D* hNumTrkvol0 = new TH1D("hNumTrkvol0","[vol0]  # of Tracks per Evt",10,0,10);
TH1D* hNumTrkvol1 = new TH1D("hNumTrkvol1","[vol1]  # of Tracks per Evt",10,0,10);
TH1D* hAzimuthvol0 = new TH1D("hAzimuthvol0","[vol0]  Azimuthal Angles",361,-180.5,180.5);
TH1D* hAzimuthvol1 = new TH1D("hAzimuthvol1","[vol1]  Azimuthal Angles",361,-180.5,180.5);
TH1D* hTotalADCvol0 = new TH1D("hTotalADCvol0","[vol0]  Total ADC",100000,0,100000);
TH1D* hTotalADCvol1 = new TH1D("hTotalADCvol1","[vol1]  Total ADC",100000,0,100000);
TH1D* hAlphaADCvol0 = new TH1D("hAlphaADCvol0","[vol0]  ALpha ADC",200,1000,3000);
TH1D* hAlphaADCvol1 = new TH1D("hAlphaADCvol1","[vol1]  ALpha ADC",200,1000,3000);
TH2D* hLengthvsADCvol0 = new TH2D("LengthvsADCvol0","[vol0]  Track Length vs. ADC",40000,0,40000,1000,0.0,10.0);
TH2D* hLengthvsADCvol1 = new TH2D("LengthvsADCvol1","[vol1]  Track Length vs. ADC",40000,0,40000,1000,0.0,10.0);
TH2D* hStartXYvol0 = new TH2D("hStartXYvol0","[vol0]  X-Y start position of track",1000,-10,10,1000,-10,10);
TH2D* hStartXYvol1 = new TH2D("hStartXYvol1","[vol1]  X-Y start position of track",1000,-10,10,1000,-10,10);

TH1D* hStartZ = new TH1D("hStartZ","Z start position of track",1000,-10,10);
TH2D* hADCvsPolar = new TH2D("hADCvsPolar","ADC vs Cos(Polar)",200,-1,1,20000,0,20000);
TH2D* hTrkLvsPolar = new TH2D("hTrkLvsPolar","Track L vs Cos(Polar)",200,-1,1,100,0,10);
TH1D* hPolar = new TH1D("hPolar","Track Polar Angles",200,-1,1);

AnaH2PadPlane *hdigADCvol0 = new AnaH2PadPlane(NiffteGeometryMap::Instance(),"hdigADCvol0"," ",0);
AnaH2PadPlane *hdigADCvol1 = new AnaH2PadPlane(NiffteGeometryMap::Instance(),"hdigADCvol1"," ",1);


//Open file list -----------------------------------------
if(nevt == -1)
	nevt = INT_MAX;

int nSkip = 0, SparseFactor = 0;
int iTrk = 0;
int Fit = 0;
int sample=0,trig=0;
int totNumEvents=0;
int volume=0;
double trkADC = 0;
double trkL = 0;
double xStart = 0;
double yStart = 0;
double zStart = 0;
double xdig = 0, ydig = 0;

aMgr.Execute(nevt,nSkip,SparseFactor);
TPCResult_t status;
while (true){

    status = aMgr.NextEvent();
    if (status != kTPCSuccess && status != kTPCNewRun ) break;
      if(aMgr.NSignals() > 0){
        const NiffteTPCADCSignal* currsig = aMgr.GetSignal(0);
        currsig->GetSampleAndTrigger(0,sample,trig);
        // if (trig != 5) continue;
      }

    if(aMgr.EventNumber()%1000==0){
	      cout <<"#### Event " << aMgr.EventNumber() << " ###" << endl;
	  totNumEvents+=1000;
    }

	for(iTrk = 0; iTrk < aMgr.NTracks(); iTrk++){
		const NiffteTPCTrack* currtrk = aMgr.GetTrack(iTrk);
		Fit = 0;
		for(int fc = 0; fc < currtrk->FitCount(); fc++){
			if(currtrk->Fitted(fc) == kFitKalmanSmooth)
				Fit = fc;
		}
		NiffteTPCTrackFit *currfit = currtrk->GetTrackFit(Fit);
		trkADC = currtrk->TotalADC();
		trkL = currfit->Length();
		xStart = currtrk->TrackStartPoint(0,Fit);
		yStart = currtrk->TrackStartPoint(1,Fit);
		zStart = currtrk->TrackStartPoint(2,Fit);
		TVector3 direction(currtrk->TrackDirection(0,Fit),currtrk->TrackDirection(1,Fit),currtrk->TrackDirection(2,Fit));

		// fill histograms
    hStartZ->Fill(zStart);
    hADCvsPolar->Fill(cos(direction.Theta()),trkADC);
    hTrkLvsPolar->Fill(cos(direction.Theta()),trkL);
    hPolar->Fill(cos(direction.Theta()));

    //Digits______________________________________________
    for(int iDig = 0; iDig < aMgr.NDigits(); iDig++){
      const NiffteTPCDigit* currdig = aMgr.GetDigit(iDig);
      xdig = 0;
      ydig = 0;
      ngm->MapVolRowColtoXY(currdig->Volume(),currdig->PadRow(),currdig->Column(),xdig,ydig);
      volume=currdig->Volume();
      if (volume==1)
        hdigADCvol1->Fill(xdig,ydig,currdig->ADC());
      else
        hdigADCvol1->Fill(xdig,ydig,0);
      if (volume==0)
        hdigADCvol0->Fill(xdig,ydig,currdig->ADC());
      else
        hdigADCvol0->Fill(xdig,ydig,0);
    }

    if (volume==0){
      hNumTrkvol0->Fill(aMgr.NTracks());
      hAzimuthvol0->Fill(direction.Phi()*TMath::RadToDeg());
      hLengthvsADCvol0->Fill(trkADC,trkL);
      hTotalADCvol0->Fill(trkADC);
      hAlphaADCvol0->Fill(trkADC);
      hStartXYvol0->Fill(xStart,yStart);
    }
    if (volume==1){
      hNumTrkvol1->Fill(aMgr.NTracks());
      hAzimuthvol1->Fill(direction.Phi()*TMath::RadToDeg());
      hLengthvsADCvol1->Fill(trkADC,trkL);
      hTotalADCvol1->Fill(trkADC);
      hAlphaADCvol1->Fill(trkADC);
      hStartXYvol1->Fill(xStart,yStart);
    }
	}
}

std::cout<<"wrote ~"<<totNumEvents<<" events"<<std::endl;
aMgr.Terminate();

TFile * f = new TFile(outfile,"UPDATE");
hNumTrkvol0->Write();
hNumTrkvol1->Write();
hAzimuthvol0->Write();
hAzimuthvol1->Write();
hTotalADCvol0->Write();
hTotalADCvol1->Write();
hAlphaADCvol0->Write();
hAlphaADCvol1->Write();
hLengthvsADCvol0->Write();
hLengthvsADCvol1->Write();
hStartXYvol0->Write();
hStartXYvol1->Write();

hStartZ->Write();
hADCvsPolar->Write();
hTrkLvsPolar->Write();
hPolar->Write();

gApplication->Terminate();

}
