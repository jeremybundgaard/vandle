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
#include "TStopwatch.h"

// C++ libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>
#include <climits>
#include <vector>
#include <time.h>
#include <map>
#include <set>


TObjArray *sigArray;
TObjArray *digArray;
TObjArray *trkArray;

NiffteGeometryMap* ngm = NiffteGeometryMap::Instance("v2_03");

void radiusCutsAnaHistMaker(const char* infile = "infile-list.txt", const char* outfile="Analysis-RunXXX.root", int nCutEvt = -1, double radMin = 0,double radMax = 7) {

// stopwatch
TStopwatch stopwatch;
stopwatch.Start();

//Niffte Analysis------------------------------
std::string geo("v2_03");
std::string ofile(outfile);
NiffteAnalysisManager aMgr(infile,ofile,geo);
          
//__Define some histograms___________________________________________  
AnaH1D* intensityvol0 = new AnaH1D("intensityvol0","intensityvol0",100,0,1);
AnaH1D* intensityvol1 = new AnaH1D("intensityvol1","intensityvol1",100,0,1);
AnaH2PadPlane *hdigADCvol0 = new AnaH2PadPlane(NiffteGeometryMap::Instance(),"hdigADCvol0"," ",0);
AnaH2PadPlane *hdigADCvol1 = new AnaH2PadPlane(NiffteGeometryMap::Instance(),"hdigADCvol1"," ",1);

typedef map<pair<float,float>,pair<int,int> > mapPairInt2PairInt;

mapPairInt2PairInt mapXY_totalDigADCvol0;
mapPairInt2PairInt::iterator mapXY_totalDigADCvol0Iter;

mapPairInt2PairInt mapXY_totalDigADCvol1;
mapPairInt2PairInt::iterator mapXY_totalDigADCvol1Iter;


//Open file list -----------------------------------------
int nevt=-1;
if(nevt == -1)
	nevt = INT_MAX;

int nSkip = 0, SparseFactor = 0;
int iTrk = 0;
int totNumEvents=0;
int volume=0;
int runningTallyDigvol0=0,runningTallyADCvol0=0;
int runningTallyDigvol1=0,runningTallyADCvol1=0;
double xdig=0, ydig=0, radius=0;

aMgr.Execute(nevt,nSkip,SparseFactor);
TPCResult_t status;
while (true){

	status = aMgr.NextEvent();
	if (status != kTPCSuccess && status != kTPCNewRun ) break;
    if(aMgr.EventNumber()%1000==0)
    {
		cout <<"#### Event " << aMgr.EventNumber() << " ###" << endl;
		totNumEvents+=1000;
    }
    if(aMgr.NTracks()==1) continue;
	for(iTrk = 0; iTrk < aMgr.NTracks(); iTrk++){
	    //Digits______________________________________________
		for(int iDig = 0; iDig < aMgr.NDigits(); iDig++){

			const NiffteTPCDigit* currdig = aMgr.GetDigit(iDig);
			xdig=0;
			ydig=0;
			radius=0;
			ngm->MapVolRowColtoXY(currdig->Volume(),currdig->PadRow(),currdig->Column(),xdig,ydig);
			volume=currdig->Volume();
			// radius=sqrt(xdig*xdig+ydig*ydig);
			radius = sqrt(pow(xdig-0.17,2) + pow(ydig+0.1,2));
			if(radMin < radius && radius < radMax && aMgr.GetTrack(iTrk)->TotalADC() < 3000)
			{

				if (volume==0)
				{
					hdigADCvol0->Fill(xdig,ydig,currdig->ADC());
					runningTallyDigvol0=mapXY_totalDigADCvol0[make_pair(xdig,ydig)].first + currdig->ADC();
					runningTallyADCvol0=mapXY_totalDigADCvol0[make_pair(xdig,ydig)].second +1;
					mapXY_totalDigADCvol0[make_pair(xdig,ydig)] = make_pair(runningTallyDigvol0,runningTallyADCvol0);
				}
				else
					hdigADCvol0->Fill(xdig,ydig,0);
				if (volume==1)
				{
					hdigADCvol1->Fill(xdig,ydig,currdig->ADC());
					runningTallyDigvol1=mapXY_totalDigADCvol1[make_pair(xdig,ydig)].first + currdig->ADC();
					runningTallyADCvol1=mapXY_totalDigADCvol1[make_pair(xdig,ydig)].second +1;
					mapXY_totalDigADCvol1[make_pair(xdig,ydig)] = make_pair(runningTallyDigvol1,runningTallyADCvol1);
				}
				else
					hdigADCvol1->Fill(xdig,ydig,0);
			}
		}
	}
	if(!(nCutEvt > aMgr.EventNumber() || nCutEvt==-1))
		break;
}

int totalDig=0,totalADC=0;
float ADCperDig=0;


for (mapXY_totalDigADCvol0Iter = mapXY_totalDigADCvol0.begin(); mapXY_totalDigADCvol0Iter != mapXY_totalDigADCvol0.end(); ++mapXY_totalDigADCvol0Iter)
{
	xdig = mapXY_totalDigADCvol0Iter->first.first;
	ydig = mapXY_totalDigADCvol0Iter->first.second;
	totalDig = mapXY_totalDigADCvol0Iter->second.first;
	totalADC = mapXY_totalDigADCvol0Iter->second.second;
	ADCperDig = totalADC/(float)totalDig;
	cout << "xdig,ydig " << xdig << ","<<ydig<< '\t' <<"ADC/dig " << ADCperDig <<endl;
	intensityvol0->Fill(ADCperDig);
}


for (mapXY_totalDigADCvol1Iter = mapXY_totalDigADCvol1.begin(); mapXY_totalDigADCvol1Iter != mapXY_totalDigADCvol1.end(); ++mapXY_totalDigADCvol1Iter)
{
	xdig = mapXY_totalDigADCvol1Iter->first.first;
	ydig = mapXY_totalDigADCvol1Iter->first.second;
	totalDig = mapXY_totalDigADCvol1Iter->second.first;
	totalADC = mapXY_totalDigADCvol1Iter->second.second;
	ADCperDig = totalADC/(float)totalDig;
	cout << "xdig,ydig " << xdig << ","<<ydig<< '\t' <<"ADC/dig " << ADCperDig <<endl;
	intensityvol1->Fill(ADCperDig);
}


std::cout<<"wrote ~"<<totNumEvents<<" events"<<std::endl;
stopwatch.Stop();
cout<<"this program ran for "<<stopwatch.RealTime()<< " sec" << endl;
aMgr.Terminate();
//fRootIO->Close();
gROOT->SetStyle("Plain");
gApplication->Terminate();
}
