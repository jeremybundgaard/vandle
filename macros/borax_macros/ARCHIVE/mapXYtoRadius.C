//
// A simple test macro for loading Niffte
// code into ROOT and running it
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
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>
#include <climits>
#include <vector>
#include <map>
#include <set>

using namespace std;

TObjArray *sigArray;
TObjArray *digArray;
NiffteGeometryMap* ngm = NiffteGeometryMap::Instance("v2_03");

void mapXYtoRadius(const char* infile = "infile-list.txt", const char* outfile="Analysis-RunXXX.root", int nevt = -1) {


// create maps [and their interators]
typedef map<pair<float,float>,pair<int,int> > mapPairFloatToPairInt;
mapPairFloatToPairInt xy2binmap;
mapPairFloatToPairInt::iterator xy2binmapIter;

typedef map<pair<float,float>,float> mapPairTofloat;
mapPairTofloat mapYX_radius;
mapPairTofloat::iterator mapYX_radiusIter;

map<float,int> radBinTallyMap;
map<float,int>::iterator radBinTallyMapIter;

map<float,int> radBinMap;
map<float,int>::iterator radBinMapIter;

	
// Open file list -----------------------------------------
int nSkip = 0, SparseFactor = 0;
int sample=0,trig=0;
int totNumEvents=0;
double xdig=0,ydig=0,radius=0;

//Niffte Analysis------------------------------
string geo("v2_03");
string ofile(outfile);
NiffteAnalysisManager aMgr(infile,"",geo);

AnaH1D *radVsIntensity = new AnaH1D("radVsIntensity","radVsIntensity",1000,0,5);


if(nevt == -1)
nevt = INT_MAX;
aMgr.Execute(nevt,nSkip,SparseFactor);
TPCResult_t status;
cout <<'\n'<<" ### filling xy to rad map ###"<<'\n' << endl;
while (true)
{
	status = aMgr.NextEvent();
	if (status != kTPCSuccess && status != kTPCNewRun ) break;

	const NiffteTPCADCSignal* currsig = aMgr.GetSignal(0);
	currsig->GetSampleAndTrigger(0,sample,trig);
	if (trig != 5) continue;

	if(aMgr.EventNumber()%1000==0)
	{
		cout <<"#### Event " << aMgr.EventNumber() << " ###" << endl;
	  totNumEvents+=1000;
	}

	//Digits______________________________________________
	for(int iDig = 0; iDig < aMgr.NDigits(); iDig++){
		const NiffteTPCDigit* currdig = aMgr.GetDigit(iDig);
		xdig = 0;
		ydig = 0;
		ngm->MapVolRowColtoXY(currdig->Volume(),currdig->PadRow(),currdig->Column(),xdig,ydig);

		// build maps
		radius=sqrt(pow(xdig,2)+pow(ydig,2));
		radBinMap[radius]=0;
		mapYX_radius[make_pair(xdig,ydig)] = radius;
		// fill radius vs intensity histogram
		radVsIntensity->Fill(radius,currdig->ADC());

	}
}
aMgr.Terminate();
totNumEvents=0;


int iterstep=0;
int nRadBinTally[radBinMap.size()];
for (radBinMapIter = radBinMap.begin(); radBinMapIter != radBinMap.end(); ++radBinMapIter)
{
	radBinMap[radBinMapIter->first]=iterstep;
	nRadBinTally[radBinMapIter->second]=0;
	iterstep++;

	// cout<<radBinMapIter->second.first<<endl;
}

int rbin=0,nbin=0,maxNbins=0;
for ( mapYX_radiusIter = mapYX_radius.begin(); mapYX_radiusIter != mapYX_radius.end(); ++mapYX_radiusIter)
{
	xdig = mapYX_radiusIter->first.first;
	ydig = mapYX_radiusIter->first.second;	
	radius=mapYX_radiusIter->second;
	rbin=radBinMap[radius];
	nbin=nRadBinTally[rbin];

	xy2binmap[make_pair(xdig,ydig)]=make_pair(rbin,nbin);
	nRadBinTally[rbin]++;

	if (nbin > maxNbins)
		maxNbins=nbin;
	// if(rbin == 162)
	// cout << fixed << setw(8) << setprecision(10) << xdig<<'\t'<< ydig <<'\t'<<radius<<'\t'<< rbin<<","<< nbin  <<endl;
}

for (radBinMapIter=radBinMap.begin();radBinMapIter!=radBinMap.end(); ++radBinMapIter)
{
	radBinTallyMap[radBinMapIter->first]=nRadBinTally[radBinMapIter->second];
}



//Niffte Analysis------------------------------
NiffteAnalysisManager aMgr2(infile,ofile,geo);
AnaH2D *nRadiusBins = new AnaH2D("nRadiusBins","nRadiusBins",radBinMap.size(),0,radBinMap.size(),maxNbins,0,maxNbins);
if(nevt == -1)
nevt = INT_MAX;
aMgr2.Execute(nevt,nSkip,SparseFactor);
TPCResult_t status2;
cout <<"#### filling intensity vs radius histograms ### " << endl;

while (true)
{
	status2 = aMgr2.NextEvent();
	if (status2 != kTPCSuccess && status2 != kTPCNewRun ) break;

	const NiffteTPCADCSignal* currsig = aMgr2.GetSignal(0);
	currsig->GetSampleAndTrigger(0,sample,trig);
	if (trig != 5) continue;

	if(aMgr2.EventNumber()%1000==0)
	{
		cout <<"#### Event " << aMgr2.EventNumber() << " ### " << endl;
		totNumEvents+=1000;
	}

	//Digits______________________________________________
	for(int iDig = 0; iDig < aMgr2.NDigits(); iDig++){
		const NiffteTPCDigit* currdig = aMgr2.GetDigit(iDig);
		xdig = 0;
		ydig = 0;
		ngm->MapVolRowColtoXY(currdig->Volume(),currdig->PadRow(),currdig->Column(),xdig,ydig);
		radius=sqrt(pow(xdig,2)+pow(ydig,2));
		rbin=xy2binmap[make_pair(xdig,ydig)].first;
		nbin=xy2binmap[make_pair(xdig,ydig)].second;
		nRadiusBins->Fill(rbin,nbin,currdig->ADC());

	}
}

AnaH1D *averageIntensityVsRadius = new AnaH1D("averageIntensityVsRadius","averageIntensityVsRadius",radBinMap.size(),0,radBinMap.size());
float avgIntensity=0;
for ( int i=0; i<nRadiusBins->GetXaxis()->GetNbins(); ++i )
{
	avgIntensity=nRadiusBins->Integral(i,i,0,nRadBinTally[i])/(float)nRadBinTally[i];
	averageIntensityVsRadius->Fill(i,avgIntensity);
}

int radbin=0;
float adjustedCounts=0;
for(radBinTallyMapIter=radBinTallyMap.begin();radBinTallyMapIter!=radBinTallyMap.end();++radBinTallyMapIter)
{

	cout << fixed << setw(8) << setprecision(10) << radBinTallyMapIter->first << '\t' << radBinTallyMapIter->second <<endl;
	radbin=radVsIntensity->FindBin(radBinTallyMapIter->first);
	adjustedCounts=radVsIntensity->GetBinContent(radbin)/(float)radBinTallyMapIter->second;
	radVsIntensity->SetBinContent(radbin,adjustedCounts);
}


aMgr2.Terminate();

	cout<<"wrote ~"<<totNumEvents<<" events"<<endl;
	gROOT->SetStyle("Plain");
	gApplication->Terminate();
}
