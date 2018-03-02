#include "TRint.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCut.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "NiffteGeometryMap.h"

#include <iomanip>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>


void compDiffRuns_cm244(const char* runType,const char* recoType, const char* treeFileName) {


std::vector<int> runNum;
char runCut[123];
char bestCut[123];
char azCut[123];
char plotName[123];
char histName[123];


if ( !(strcmp(runType,"cmCFT1")))
{

	runNum.push_back(1941);
	runNum.push_back(1942);
	runNum.push_back(1943);
	runNum.push_back(1944);
	runNum.push_back(1945);
	runNum.push_back(1946);

	runNum.push_back(1953);
	runNum.push_back(1954);
	runNum.push_back(1955);
	runNum.push_back(1956);
	runNum.push_back(1957);
	runNum.push_back(1958);
	runNum.push_back(1959);
	runNum.push_back(1960);
	runNum.push_back(1961);
	runNum.push_back(1962);

	runNum.push_back(1975);
	runNum.push_back(1976);
	runNum.push_back(1977);
	runNum.push_back(1978);
	runNum.push_back(1979);
	runNum.push_back(1980);
	runNum.push_back(1981);
	runNum.push_back(1982);
	runNum.push_back(1983);
	runNum.push_back(1984);
	runNum.push_back(1985);
	runNum.push_back(1986);
	runNum.push_back(1987);
	runNum.push_back(1988);

	runNum.push_back(1989);
	runNum.push_back(1990);
	runNum.push_back(1991);
	runNum.push_back(1992);
	runNum.push_back(1993);
	runNum.push_back(1994);

	runNum.push_back(1996);
	runNum.push_back(1997);
	runNum.push_back(1998);
	runNum.push_back(1999);
	runNum.push_back(2000);
	runNum.push_back(2001);
	runNum.push_back(2002);
	runNum.push_back(2003);
	runNum.push_back(2004);
	runNum.push_back(2005);

}
else if ( !(strcmp(runType,"cmCFT2")))
{


	runNum.push_back(2092);
	runNum.push_back(2093);
	runNum.push_back(2094);
	runNum.push_back(2095);
	runNum.push_back(2096);
	runNum.push_back(2097);
	runNum.push_back(2098);
	runNum.push_back(2099);

	runNum.push_back(2100);
	runNum.push_back(2101); //looks bad
	runNum.push_back(2102);
	runNum.push_back(2103);
	runNum.push_back(2104);
	runNum.push_back(2105);
	runNum.push_back(2106);
	runNum.push_back(2107);
	runNum.push_back(2108);
	runNum.push_back(2109);

	runNum.push_back(2110);
	runNum.push_back(2111);
	runNum.push_back(2112);
	runNum.push_back(2113);
	runNum.push_back(2114); //looks bad
	runNum.push_back(2115); //looks bad
	runNum.push_back(2116);
	runNum.push_back(2117);
	runNum.push_back(2118);
	runNum.push_back(2119);

	runNum.push_back(2120);
	runNum.push_back(2121);
	runNum.push_back(2122);
	runNum.push_back(2123);
	runNum.push_back(2124);
	runNum.push_back(2125);
	runNum.push_back(2126);
	runNum.push_back(2127);
	runNum.push_back(2128);
	runNum.push_back(2129); //looks bad

	runNum.push_back(2130);
	runNum.push_back(2131);
	runNum.push_back(2132);
	runNum.push_back(2133);
	runNum.push_back(2134);
	runNum.push_back(2135);
	runNum.push_back(2136);
	runNum.push_back(2137);
	runNum.push_back(2138);
	runNum.push_back(2139);

	runNum.push_back(2140);
	runNum.push_back(2141);
	runNum.push_back(2142);
	runNum.push_back(2143);
	runNum.push_back(2144);
	runNum.push_back(2145);
	runNum.push_back(2146);
	runNum.push_back(2147);
	runNum.push_back(2148);
	runNum.push_back(2149);

	runNum.push_back(2150);
	runNum.push_back(2151);
	runNum.push_back(2152);
	runNum.push_back(2153);
	runNum.push_back(2154);
	runNum.push_back(2155);
	runNum.push_back(2156);
	runNum.push_back(2157); //looks bad
	runNum.push_back(2158); //looks bad
	runNum.push_back(2159);

	runNum.push_back(2160);
	runNum.push_back(2161);
	runNum.push_back(2162);
	runNum.push_back(2163);
	runNum.push_back(2164);
	runNum.push_back(2165);
	runNum.push_back(2166);
	runNum.push_back(2167);
	runNum.push_back(2168);
	runNum.push_back(2169);

	runNum.push_back(2170);
	runNum.push_back(2171);
	runNum.push_back(2172);
	runNum.push_back(2173);
	runNum.push_back(2174);
	runNum.push_back(2175);
	runNum.push_back(2176);
	runNum.push_back(2177);//looks bad
	runNum.push_back(2178);//looks bad
	runNum.push_back(2179);

	runNum.push_back(2180);
	runNum.push_back(2181);
	runNum.push_back(2182);//looks bad
	runNum.push_back(2183);//looks bad
	runNum.push_back(2184);//looks bad
	runNum.push_back(2185);//looks bad
	runNum.push_back(2186);//looks bad
	runNum.push_back(2187);//looks bad
	runNum.push_back(2188);//looks bad
	runNum.push_back(2189);//looks bad

	runNum.push_back(2190);
	runNum.push_back(2191);
	runNum.push_back(2192);
	runNum.push_back(2193);
	runNum.push_back(2194);
	runNum.push_back(2195);
	runNum.push_back(2196);
	runNum.push_back(2197);
	runNum.push_back(2198);
	runNum.push_back(2199);

	runNum.push_back(2200);
	runNum.push_back(2201);
	runNum.push_back(2202);
	runNum.push_back(2203);
	runNum.push_back(2204);
	runNum.push_back(2206);

	runNum.push_back(2235);
	runNum.push_back(2236);
	runNum.push_back(2237);
	runNum.push_back(2238);
	runNum.push_back(2239);
	runNum.push_back(2240);

	runNum.push_back(2241);
	runNum.push_back(2242);
	runNum.push_back(2243);
	runNum.push_back(2244);
	runNum.push_back(2245);
	runNum.push_back(2246);
	runNum.push_back(2247);
	runNum.push_back(2248);
	runNum.push_back(2249);


}
else if ( !(strcmp(runType,"cmNoMask1")))
{
	runNum.push_back(1922);
	runNum.push_back(1923);
}
else if ( !(strcmp(runType,"cmNoMask2")))
{
	
	runNum.push_back(2250);
	runNum.push_back(2251);
	runNum.push_back(2252);
	runNum.push_back(2253);
	runNum.push_back(2254);
	runNum.push_back(2255);
	runNum.push_back(2256);

}


TFile* treeFile = new TFile(treeFileName);
TTree* tracks = (TTree*)treeFile->Get("tracks");

TCanvas *c1 = new TCanvas("c1","c1",1000,700);
gStyle->SetOptStat(0);


for (std::vector<int>::iterator runNumIter = runNum.begin(); runNumIter != runNum.end(); ++runNumIter)
{

	sprintf(runCut,"run == 10000%d",*runNumIter);
	if (!(strcmp(recoType,"hk"))){
		sprintf(bestCut,"fitType==5 && %s",runCut);
	}
	if ( !(strcmp(recoType,"fp"))){
		sprintf(bestCut,"bestFit && %s",runCut);
	}


	cout<<runCut<<endl;

	c1->SetLogx(0);
	c1->SetLogy(0);
	c1->SetLogz(0);
	c1->SetLogz(1);

	TH2D* lVsEn = new TH2D("lVsEn","lVsEn",40000,0,168.6,1000,0,10);
	lVsEn->GetXaxis()->SetTitle("MeV");
	lVsEn->GetYaxis()->SetTitle("cm");
	sprintf(histName,"lVsEn{%s}",runCut);
	lVsEn->SetTitle(histName);
	tracks->Draw("length:adc*168.6/40000>>lVsEn",runCut,"colz");
	lVsEn->GetZaxis()->SetRangeUser(1,lVsEn->GetMaximum());
	lVsEn->Draw("Colz");
	sprintf(plotName,"%s.lVsEn.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(histName,"lVsEn{%s}",bestCut);
	cout<<bestCut<<endl;
	lVsEn->SetTitle(histName);
	tracks->Draw("length:adc*168.6/40000>>lVsEn",bestCut,"colz");
	sprintf(plotName,"%s.lVsEn.bestFit.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(plotName,"%s.lVsEn.bestFit.logx.run%d.png",runType,*runNumIter);
	c1->SetLogx(1);
	tracks->Draw("length:adc*168.6/40000>>lVsEn",bestCut,"colz");
	c1->SaveAs(plotName);
	c1->SetLogx(0);
	delete lVsEn;

	c1->SetLogy(1);	

	TH2D* enVsAz = new TH2D("enVsAz","enVsAz",2000,-3.15,3.15,40000,0,168.6); 
	enVsAz->GetXaxis()->SetTitle("adc");
	enVsAz->GetYaxis()->SetTitle("cm");
	sprintf(histName,"enVsAz{%s}",runCut);
	enVsAz->SetTitle(histName);
	enVsAz->GetYaxis()->SetRangeUser(0.1,170);
	tracks->Draw("adc*168.6/40000:direction.Phi()>>enVsAz",runCut,"colz");
	enVsAz->GetZaxis()->SetRangeUser(1,enVsAz->GetMaximum());
	enVsAz->Draw("Colz");
	sprintf(plotName,"%s.enVsAz.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(histName,"enVsAz{%s}",bestCut);
	enVsAz->SetTitle(histName);
	tracks->Draw("adc*168.6/40000:direction.Phi()>>enVsAz",bestCut,"colz");
	sprintf(plotName,"%s.enVsAz.bestFit.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	delete enVsAz;

	TH2D* enVsPolar = new TH2D("enVsPolar","enVsPolar",1000,0,3.15,40000,0,168.6);
	enVsPolar->GetXaxis()->SetTitle("MeV");
	enVsPolar->GetYaxis()->SetTitle("#theta");
	sprintf(histName,"enVsPolar{%s}",runCut);
	enVsPolar->SetTitle(histName);
	enVsPolar->GetYaxis()->SetRangeUser(0.1,170);
	tracks->Draw("adc*168.6/40000:direction.Theta()>>enVsPolar",runCut,"colz");
	enVsPolar->GetZaxis()->SetRangeUser(1,enVsPolar->GetMaximum());
	enVsPolar->Draw("Colz");
	sprintf(plotName,"%s.enVsPolar.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(histName,"enVsPolar{%s}",bestCut);
	enVsPolar->SetTitle(histName);
	tracks->Draw("adc*168.6/40000:direction.Theta()>>enVsPolar",bestCut,"colz");
	sprintf(plotName,"%s.enVsPolar.bestFit.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	delete enVsPolar;

	TH2D* enVsCosPolar = new TH2D("enVsCosPolar","enVsCosPolar",1000,-1,1,40000,0,168.6);
	enVsCosPolar->GetXaxis()->SetTitle("MeV");
	enVsCosPolar->GetYaxis()->SetTitle("cos(#theta)");
	sprintf(histName,"enVsCosPolar{%s}",runCut);
	enVsCosPolar->SetTitle(histName);
	enVsCosPolar->GetYaxis()->SetRangeUser(0.1,170);	
	tracks->Draw("adc*168.6/40000:direction.CosTheta()>>enVsCosPolar",runCut,"colz");
	enVsCosPolar->GetZaxis()->SetRangeUser(1,enVsCosPolar->GetMaximum());
	enVsCosPolar->Draw("Colz");
	sprintf(plotName,"%s.enVsCosPolar.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(histName,"enVsCosPolar{%s}",bestCut);
	enVsCosPolar->SetTitle(histName);
	tracks->Draw("adc*168.6/40000:direction.CosTheta()>>enVsCosPolar",bestCut,"colz");
	sprintf(plotName,"%s.enVsCosPolar.bestFit.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	delete enVsCosPolar;


	c1->SetLogy(0);

	TH2D* xyStart = new TH2D("xyStart","xyStart",1000,-7,7,1000,-7,7);
	xyStart->GetXaxis()->SetTitle("cm");
	xyStart->GetYaxis()->SetTitle("cm");
	sprintf(histName,"xyStart{%s}",runCut);
	xyStart->SetTitle(histName);
	tracks->Draw("start.x():start.y()>>xyStart",runCut,"colz");
	xyStart->GetZaxis()->SetRangeUser(1,xyStart->GetMaximum());
	xyStart->Draw("Colz");
	sprintf(plotName,"%s.xyStart.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(histName,"xyStart{%s}",bestCut);
	xyStart->SetTitle(histName);
	tracks->Draw("start.x():start.y()>>xyStart",bestCut,"colz");
	sprintf(plotName,"%s.xyStart.bestFit.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	delete xyStart;

	TH2D* xyEnd = new TH2D("xyEnd","xyEnd",1000,-7,7,1000,-7,7);
	xyEnd->GetXaxis()->SetTitle("cm");
	xyEnd->GetYaxis()->SetTitle("cm");
	sprintf(histName,"xyEnd{%s}",runCut);
	xyEnd->SetTitle(histName);
	tracks->Draw("end.x():end.y()>>xyEnd",runCut,"colz");
	xyEnd->GetZaxis()->SetRangeUser(1,xyEnd->GetMaximum());
	xyEnd->Draw("Colz");
	sprintf(plotName,"%s.xyEnd.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	sprintf(histName,"xyEnd{%s}",bestCut);
	xyEnd->SetTitle(histName);
	tracks->Draw("end.x():end.y()>>xyEnd",bestCut,"colz");
	sprintf(plotName,"%s.xyEnd.bestFit.run%d.png",runType,*runNumIter);
	c1->SaveAs(plotName);
	delete xyEnd;

	c1->SetLogz(0);

	c1->SetLogy(1);

	// TH1D* en = new TH1D("en","en",4000,0,168.6);
	// en->GetXaxis()->SetTitle("MeV");
	// tracks->Draw("energy*168.6/40000>>en",runCut,"");
	// sprintf(plotName,"%s.en.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("energy*168.6/40000>>en",bestCut,"");
	// sprintf(plotName,"%s.en.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// delete en;

	// TH1D* adc = new TH1D("adc","adc",50000,0,50000);
	// adc->GetXaxis()->SetTitle("adc");
	// tracks->Draw("adc>>adc",runCut,"");
	// sprintf(plotName,"%s.adc.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("adc>>adc",bestCut,"");
	// sprintf(plotName,"%s.adc.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// delete adc;

	// TH1D* length = new TH1D("length","length",1000,0,10);
	// length->GetXaxis()->SetTitle("cm");
	// tracks->Draw("length>>length",runCut,"");
	// sprintf(plotName,"%s.length.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("length>>length",bestCut,"");
	// sprintf(plotName,"%s.length.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// delete length;

	// TH1D* digCharge = new TH1D("digCharge","digCharge",50000,0,50000);
	// digCharge->GetXaxis()->SetTitle("adcPerDig");
	// tracks->Draw("digitCharge>>digCharge",runCut,"");
	// sprintf(plotName,"%s.digCharge.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("digitCharge>>digCharge",bestCut,"");
	// sprintf(plotName,"%s.digCharge.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// c1->SetLogy(0);
	// delete digCharge;

	// TH1D* az = new TH1D("az","az",2000,-3.15,3.15);
	// en->GetXaxis()->SetTitle("#phi");
	// tracks->Draw("direction.Phi()>>az",runCut,"");
	// sprintf(plotName,"%s.az.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("direction.Phi()>>az",bestCut,"");
	// sprintf(plotName,"%s.az.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// delete az;

	// TH1D* cosPolar = new TH1D("cosPolar","cosPolar",1000,-1,1);
	// en->GetXaxis()->SetTitle("cos(#theta)");
	// tracks->Draw("direction.CosTheta()>>cosPolar",runCut,"");
	// sprintf(plotName,"%s.cosPolar.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("direction.CosTheta()>>cosPolar",bestCut,"");
	// sprintf(plotName,"%s.cosPolar.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// delete cosPolar;

	// TH1D* polar = new TH1D("polar","polar",1000,0,3.15);
	// polar->GetXaxis()->SetTitle("#theta");
	// tracks->Draw("direction.Theta()>>polar",runCut,"");
	// sprintf(plotName,"%s.polar.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// tracks->Draw("direction.Theta()>>polar",bestCut,"");
	// sprintf(plotName,"%s.polar.bestFit.run%d.png",runType,*runNumIter);
	// c1->SaveAs(plotName);
	// delete polar;



}

}