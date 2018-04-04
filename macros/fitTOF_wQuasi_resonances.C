#include "TAxis.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TChain.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPaveStats.h"
// #include "/home/jeremy/vandle/macros/vandlePlots.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "TProof.h"
#include "TROOT.h"
#include <climits>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>


void gammaGatedNeutronTOF(const char *gamma_tree_file = "",const char *vandle_tree_file = "",const char *otherTreeCuts = "1",const char *isotope= "") {

// TGaxis::SetMaxDigits(2);
// gStyle->SetPalette(kCherry);
gStyle->SetOptStat("ne");
gStyle->SetTitleOffset(1.2,"Y");
gStyle->SetNumberContours(99);
// gStyle->SetStatFont(63);
// gStyle->SetStatFontSize(12);

int cutWidth_tof=300;
int minTOF=70;
int maxTOF=minTOF+cutWidth_tof;
int minQDC=0;
int maxQDC=minQDC+3500;
int background_TOF_Shift = 350;

TCutG *tcutg_NEUTRON = new TCutG("tcutg_NEUTRON",3);
tcutg_NEUTRON->SetVarX("vandle_TOF");
tcutg_NEUTRON->SetVarY("vandle_QDC");
tcutg_NEUTRON->SetLineWidth(4);
tcutg_NEUTRON->SetLineColor(kMagenta);
tcutg_NEUTRON->SetPoint(0,minTOF,minQDC);
tcutg_NEUTRON->SetPoint(1,maxTOF,minQDC);
tcutg_NEUTRON->SetPoint(2,maxTOF,maxQDC);
tcutg_NEUTRON->SetPoint(3,minTOF,maxQDC);
tcutg_NEUTRON->SetPoint(4,minTOF,minQDC);

///////// initialize track tree /////////
printf("\n///////// initialize vandle_tree: %s /////////\n\n",vandle_tree_file);
TFile *vandle_TFile = new TFile(vandle_tree_file);
TTree* vandle_tree = (TTree*)vandle_TFile->Get("vandle_tree");

unsigned int evtNumber=0;
// std::string dataRun;
double vandle_QDC=0;
double vandle_lQDC=0;
double vandle_rQDC=0;
double vandle_QDCPos=-500;
double vandle_TOF=0;
double vandle_lSnR=0;
double vandle_rSnR=0;
double vandle_lAmp=0;
double vandle_rAmp=0;
double vandle_lMaxAmpPos=0;
double vandle_rMaxAmpPos=0;
double vandle_lAveBaseline=0;
double vandle_rAveBaseline=0;
unsigned int vandle_bar=0;
double vandle_TAvg=0;
double vandle_Corrected_TAvg=0;
double vandle_TDiff=0;
double vandle_Corrected_TDiff=0;

vandle_tree->SetBranchAddress("evtNumber", &evtNumber);
// vandle_tree->SetBranchAddress("dataRun", &dataRun);
vandle_tree->SetBranchAddress("vandle_QDC", &vandle_QDC);
vandle_tree->SetBranchAddress("vandle_lQDC", &vandle_lQDC);
vandle_tree->SetBranchAddress("vandle_rQDC", &vandle_rQDC);
vandle_tree->SetBranchAddress("vandle_QDCPos", &vandle_QDCPos);
vandle_tree->SetBranchAddress("vandle_TOF", &vandle_TOF);
vandle_tree->SetBranchAddress("vandle_lSnR", &vandle_lSnR);
vandle_tree->SetBranchAddress("vandle_rSnR", &vandle_rSnR);
vandle_tree->SetBranchAddress("vandle_lAmp", &vandle_lAmp);
vandle_tree->SetBranchAddress("vandle_rAmp", &vandle_rAmp);
vandle_tree->SetBranchAddress("vandle_lMaxAmpPos", &vandle_lMaxAmpPos);
vandle_tree->SetBranchAddress("vandle_rMaxAmpPos", &vandle_rMaxAmpPos);
vandle_tree->SetBranchAddress("vandle_lAveBaseline", &vandle_lAveBaseline);
vandle_tree->SetBranchAddress("vandle_rAveBaseline", &vandle_rAveBaseline);
vandle_tree->SetBranchAddress("vandle_bar", &vandle_bar);
vandle_tree->SetBranchAddress("vandle_TAvg", &vandle_TAvg);
vandle_tree->SetBranchAddress("vandle_Corrected_TAvg", &vandle_Corrected_TAvg);
vandle_tree->SetBranchAddress("vandle_TDiff", &vandle_TDiff);
vandle_tree->SetBranchAddress("vandle_Corrected_TDiff", &vandle_Corrected_TDiff);

double beta_BarQDC=0;
double beta_lQDC=0;
double beta_rQDC=0;
double beta_lSnR=0;
double beta_rSnR=0;
double beta_lAmp=0;
double beta_rAmp=0;
double beta_lMaxAmpPos=0;
double beta_rMaxAmpPos=0;
double beta_lAveBaseline=0;
double beta_rAveBaseline=0;
unsigned int beta_barNum=0;
double beta_TAvg=0;
double beta_Corrected_TAvg=0;
double beta_TDiff=0;
double beta_Corrected_TDiff=0;

vandle_tree->SetBranchAddress("beta_BarQDC", &beta_BarQDC);
vandle_tree->SetBranchAddress("beta_lQDC", &beta_lQDC);
vandle_tree->SetBranchAddress("beta_rQDC", &beta_rQDC);
vandle_tree->SetBranchAddress("beta_lSnR", &beta_lSnR);
vandle_tree->SetBranchAddress("beta_rSnR", &beta_rSnR);
vandle_tree->SetBranchAddress("beta_lAmp", &beta_lAmp);
vandle_tree->SetBranchAddress("beta_rAmp", &beta_rAmp);
vandle_tree->SetBranchAddress("beta_lMaxAmpPos", &beta_lMaxAmpPos);
vandle_tree->SetBranchAddress("beta_rMaxAmpPos", &beta_rMaxAmpPos);
vandle_tree->SetBranchAddress("beta_lAveBaseline", &vandle_lAveBaseline);
vandle_tree->SetBranchAddress("beta_rAveBaseline", &vandle_rAveBaseline);
vandle_tree->SetBranchAddress("beta_barNum", &beta_barNum);
vandle_tree->SetBranchAddress("beta_TAvg", &beta_TAvg);
vandle_tree->SetBranchAddress("beta_Corrected_TAvg", &beta_Corrected_TAvg);
vandle_tree->SetBranchAddress("beta_TDiff", &beta_TDiff);
vandle_tree->SetBranchAddress("beta_Corrected_TDiff", &beta_Corrected_TDiff);

///////// initialize track tree /////////
printf("\n///////// initialize gamma_tree: %s /////////\n\n",gamma_tree_file);
TFile *gamma_TFile = new TFile(gamma_tree_file);
TTree	*HPGe_tree = (TTree*)gamma_TFile->Get("HPGe_tree");

double aux_GeEn=0;
double aux_GeNum=0;
double aux_GeTime=0;
bool aux_GeHasLRBeta=false;
double aux_GeTdiff = 0;
double aux_GeMulti=0;
double aux_BetaEn=0;
double aux_BetaTime=0;
double aux_BetaMulti=0;

int aux_cycle=-1;
double aux_cycleSTime=0;
double aux_eventNum=-1;

HPGe_tree->SetBranchAddress("aux_GeEn",&aux_GeEn);
HPGe_tree->SetBranchAddress("aux_GeNum",&aux_GeNum);
HPGe_tree->SetBranchAddress("aux_GeTime",&aux_GeTime);
HPGe_tree->SetBranchAddress("aux_GeTdiff",&aux_GeTdiff);
HPGe_tree->SetBranchAddress("aux_gMulti",&aux_GeMulti);
HPGe_tree->SetBranchAddress("aux_GeHasLRBeta",&aux_GeHasLRBeta);
HPGe_tree->SetBranchAddress("aux_BetaEn",&aux_BetaEn);
HPGe_tree->SetBranchAddress("aux_BetaTime",&aux_BetaTime);
HPGe_tree->SetBranchAddress("aux_BetaMulti",&aux_BetaMulti);
HPGe_tree->SetBranchAddress("aux_cycle",&aux_cycle);
HPGe_tree->SetBranchAddress("aux_cycleSTime",&aux_cycleSTime);
HPGe_tree->SetBranchAddress("aux_eventNum",&aux_eventNum);


TH1D* HPGe_hist  = new TH1D("HPGe_hist","HPGe",1000,30,3000);
HPGe_hist->GetXaxis()->SetTitle("#gamma Energy [keV]");
HPGe_hist->SetLineColor(kViolet);
TH1D* betaGated_HPGe_hist = (TH1D*)HPGe_hist->Clone();
betaGated_HPGe_hist->SetLineColor(kBlue);

char buffChar[345];
TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);

HPGe_tree->Draw("aux_GeEn>>HPGe_hist");
HPGe_tree->Draw("aux_GeEn>>betaGated_HPGe_hist","aux_GeHasLRBeta","same");
c1->SaveAs("GeEn.png");

THStack* hs = new THStack("hs","event times");
auto legend = new TLegend(0.1,0.7,0.48,0.9);

double minTime=211.06e12;
double maxTime=211.29e12;
int plotBins=1000;

TH1D* GeTime_hist  = new TH1D("GeTime_hist","GeTime",plotBins,minTime,maxTime);
GeTime_hist->SetMaximum(2300);
GeTime_hist->SetLineWidth(2);
GeTime_hist->SetLineColor(kMagenta);
HPGe_tree->Draw("aux_GeTime*8>>GeTime_hist");
hs->Add(GeTime_hist);legend->AddEntry(GeTime_hist);
c1->SaveAs("GeTime.png");

TH2D* GeTime_BetaTime  = new TH2D("GeTime_BetaTime","GeTime_BetaTime",plotBins,minTime,maxTime,plotBins,minTime,maxTime);
HPGe_tree->Draw("aux_GeTime:aux_BetaTime>>GeTime_BetaTime");
c1->SaveAs("GeTime_BetaTime.png");

//
//
// TH1D* betaGated_GeTime_hist  = new TH1D("betaGated_GeTime_hist","betaGated_GeTime",plotBins,minTime,maxTime);
// betaGated_GeTime_hist->SetMaximum(2300);
// betaGated_GeTime_hist->SetLineWidth(2);
// betaGated_GeTime_hist->SetLineColor(kBlue);
// HPGe_tree->Draw("aux_GeTime*8>>betaGated_GeTime_hist","aux_GeHasLRBeta","");
// hs->Add(betaGated_GeTime_hist);legend->AddEntry(betaGated_GeTime_hist);
// c1->SaveAs("betaGated_GeTime_hist.png");
//
// TH1D* beta_TAvg_hist  = new TH1D("beta_TAvg_hist","beta_TAvg",plotBins,minTime,maxTime);
// beta_TAvg_hist->SetMaximum(2300);
// beta_TAvg_hist->SetLineWidth(2);
// beta_TAvg_hist->SetLineColor(kGreen);
// vandle_tree->Draw("beta_TAvg>>beta_TAvg_hist");
// hs->Add(beta_TAvg_hist);legend->AddEntry(beta_TAvg_hist);
// c1->SaveAs("beta_TAvg_hist.png");
//
// TH1D* beta_TAvg_NEUTRON_tcutg_hist  = new TH1D("beta_TAvg_NEUTRON_tcutg_hist","beta_TAvg_NEUTRON_tcutg_hist",plotBins,minTime,maxTime);
// beta_TAvg_NEUTRON_tcutg_hist->SetMaximum(2300);
// beta_TAvg_NEUTRON_tcutg_hist->SetLineWidth(2);
// beta_TAvg_NEUTRON_tcutg_hist->SetLineColor(kRed);
// vandle_tree->Draw("beta_TAvg>>beta_TAvg_NEUTRON_tcutg_hist","tcutg_NEUTRON","");
// hs->Add(beta_TAvg_NEUTRON_tcutg_hist);legend->AddEntry(beta_TAvg_NEUTRON_tcutg_hist);
// c1->SaveAs("beta_TAvg_NEUTRON_tcutg_hist.png");
//
// hs->Draw("nostack");legend->Draw();
//
// c1->SaveAs("event_times.png");
//
// std::set<double> beta_neutron_time_set;
// std::set<double>::iterator beta_neutron_time_set_iter;
// int dummy_int=0;
// for (int i = 0; i < vandle_tree->GetEntries(); i++) {
//   vandle_tree->GetEntry(i);
//   // printf("treeEntry: %d '\t' vandle_TOF:%f '\t' vandle_QDC:%f \n", i,vandle_TOF,vandle_QDC);
//   if(tcutg_NEUTRON->IsInside(vandle_TOF,vandle_QDC)==1){
//     // printf(" floor(beta_TAvg):%.7f \n",floor(beta_TAvg));
//     sprintf(buffChar,"echo %f >> floor_beta_TAvg.txt",floor(beta_TAvg));
//     system(buffChar);
//     beta_neutron_time_set.insert(floor(beta_TAvg));
//   }
// }
// printf("beta_neutron_time_set.size():%lu \n",beta_neutron_time_set.size());
//
// double neutronGated_GeEn=0;
// double neutronGated_GeNum=0;
// double neutronGated_GeTime=0;
// bool   neutronGated_GeHasLRBeta=false;
// double neutronGated_GeTdiff = 0;
// double neutronGated_GeMulti=0;
// double neutronGated_BetaEn=0;
// double neutronGated_BetaTime=0;
// double neutronGated_BetaMulti=0;
// int    neutronGated_cycle=-1;
// double neutronGated_cycleSTime=0;
// double neutronGated_eventNum=-1;
//
// TTree	*neutronGated_HPGe_tree = new TTree("neutronGated_HPGe_tree","neutronGated_HPGe_tree");
// neutronGated_HPGe_tree->Branch("neutronGated_GeEn",&neutronGated_GeEn);
// neutronGated_HPGe_tree->Branch("neutronGated_GeNum",&neutronGated_GeNum);
// neutronGated_HPGe_tree->Branch("neutronGated_GeTime",&neutronGated_GeTime);
// neutronGated_HPGe_tree->Branch("neutronGated_GeHasLRBeta",&neutronGated_GeHasLRBeta);
// neutronGated_HPGe_tree->Branch("neutronGated_GeTdiff",&neutronGated_GeTdiff);
// neutronGated_HPGe_tree->Branch("neutronGated_GeMulti",&neutronGated_GeMulti);
// neutronGated_HPGe_tree->Branch("neutronGated_BetaEn",&neutronGated_BetaEn);
// neutronGated_HPGe_tree->Branch("neutronGated_BetaTime",&neutronGated_BetaTime);
// neutronGated_HPGe_tree->Branch("neutronGated_BetaMulti",&neutronGated_BetaMulti);
// neutronGated_HPGe_tree->Branch("neutronGated_cycle",&neutronGated_cycle);
// neutronGated_HPGe_tree->Branch("neutronGated_cycleSTime",&neutronGated_cycleSTime);
// neutronGated_HPGe_tree->Branch("neutronGated_eventNum",&neutronGated_eventNum);
//
// int N_neutron_gated_gammas=0;
// double aux_BetaTime_nS=0;
// for (int i = 0; i < HPGe_tree->GetEntries(); i++) {
//   HPGe_tree->GetEntry(i);
//   if(!aux_BetaTime) continue;
//
//   neutronGated_GeEn = aux_GeEn;
//   neutronGated_GeNum = aux_GeNum;
//   neutronGated_GeTime = aux_GeTime;
//   neutronGated_GeHasLRBeta = aux_GeHasLRBeta;
//   neutronGated_GeTdiff = aux_GeTdiff;
//   neutronGated_GeMulti = aux_GeMulti;
//   neutronGated_BetaEn = aux_BetaEn;
//   neutronGated_BetaTime = aux_BetaTime*8+1e-9;;
//   neutronGated_BetaMulti = aux_BetaMulti;
//   neutronGated_cycle = aux_cycle;
//   neutronGated_cycleSTime = aux_cycleSTime;
//   neutronGated_eventNum = aux_eventNum;
//   // printf("aux_BetaTime:%f \t aux_BetaTime_nS:%f \n",aux_BetaTime,aux_BetaTime_nS);
//
//   if(beta_neutron_time_set.find(aux_BetaTime_nS)!=beta_neutron_time_set.end()){
//     N_neutron_gated_gammas++;
//     printf("N_neutron_gated_gammas:%d\n",N_neutron_gated_gammas);
//     neutronGated_HPGe_tree->Fill();
//   }
// }
//
// printf("GeTime_hist->Integral():%f\n",GeTime_hist->Integral());
// printf("betaGated_GeTime_hist->Integral():%f\n",betaGated_GeTime_hist->Integral());
// printf("beta_TAvg_hist->Integral():%f\n",beta_TAvg_hist->Integral());
// printf("beta_TAvg_NEUTRON_tcutg_hist->Integral():%f\n",beta_TAvg_NEUTRON_tcutg_hist->Integral());
// printf("neutronGated_HPGe_tree->GetEntries():%lld\n",neutronGated_HPGe_tree->GetEntries());
// // cut 814 keV
//
}
