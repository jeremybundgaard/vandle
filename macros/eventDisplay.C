
#include "TAxis.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TMarker.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TPolyLine.h"
#include "TPolyLine3D.h"
#include "TROOT.h"
#include "TRint.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TText.h"
#include "TTree.h"
#include "TVector3.h"
#include "TView.h"

#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>

Double_t VandleTiming(Double_t *x, Double_t *p);

void eventDisplay(const char *data_summary_tree = "",const char *otherTreeCuts = "1") {

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetGrid();
  c1->SetLogz();
  TGaxis::SetMaxDigits(3);
  // gStyle->SetPalette(kAvocado);
  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);
  gStyle->SetOptFit(1);

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize data_summary_tree /////////\n\n");
  TFile *treefile = new TFile(data_summary_tree);
  TTree *dst = (TTree *)treefile->Get("data_summary_tree");
  // dst->SetLineColor(kMagenta);
  dst->SetLineWidth(4);


  unsigned int evtNumber=0;
  unsigned int vandle_barNum=0;
  double vandle_QDC=0;
  double vandle_TOF=0;
  double vandle_TDiff=0;
  double vandle_left_phase=0;
  double vandle_right_phase=0;
  double beta_phase=0;

  dst->SetBranchAddress("evtNumber",&evtNumber);
  dst->SetBranchAddress("vandle_barNum",&vandle_barNum);
  dst->SetBranchAddress("vandle_Average_QDC",&vandle_QDC);
  dst->SetBranchAddress("vandle_TOF",&vandle_TOF);
  dst->SetBranchAddress("vandle_TDiff",&vandle_TDiff);
  dst->SetBranchAddress("vandle_left_phase",&vandle_left_phase);
  dst->SetBranchAddress("vandle_right_phase",&vandle_right_phase);
  dst->SetBranchAddress("beta_phase",&beta_phase);

  TCutG *tcutg_walk_region = new TCutG("tcutg_walk_region",4);
  tcutg_walk_region->SetVarX("vandle_TOF");
  tcutg_walk_region->SetVarY("vandle_Average_QDC");
  tcutg_walk_region->SetLineWidth(3);
  tcutg_walk_region->SetLineColor(kMagenta);
  tcutg_walk_region->SetPoint(0,60,600);
  tcutg_walk_region->SetPoint(1,80,600);
  tcutg_walk_region->SetPoint(2,80,900);
  tcutg_walk_region->SetPoint(3,60,900);
  tcutg_walk_region->SetPoint(4,60,600);

  // tcutg_walk_region->SetPoint(0,10,0);
  // tcutg_walk_region->SetPoint(1,10,900);
  // tcutg_walk_region->SetPoint(2,50,0);
  // tcutg_walk_region->SetPoint(3,10,0);

  TH2D* vandle_QDCvsTOF = new TH2D("vandle_QDCvsTOF","vandle_QDCvsTOF",500,-60,100,180,0,1800);
  vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [nsec]");
  vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
  dst->Draw("vandle_Average_QDC:vandle_TOF>>vandle_QDCvsTOF","vandle_Average_QDC!=0","colz");

  TH2D* vandle_barNum_VS_TDiff = new TH2D("vandle_barNum_VS_TDiff","vandle_barNum_VS_TDiff",500,-30,30,30,0,30);
  vandle_barNum_VS_TDiff->GetXaxis()->SetTitle("TDiff [nsec]");
  vandle_barNum_VS_TDiff->GetYaxis()->SetTitle("VANDLE bar");
  dst->Draw("vandle_barNum:vandle_TDiff>>vandle_barNum_VS_TDiff","vandle_Average_QDC!=0","colz");

  TH2D* leftTraces = new TH2D("leftTraces","leftTraces",130,0,130,3000,0,3000);
  leftTraces->GetXaxis()->SetTitle("clock tick [nsec]");
  leftTraces->GetYaxis()->SetTitle("trace [ADC]");
  leftTraces->SetMarkerStyle(33);
  leftTraces->SetMarkerColor(33);
  leftTraces->SetMarkerColor(kMagenta);

  TH2D* rightTraces = new TH2D("rightTraces","rightTraces",130,0,130,3000,0,3000);
  rightTraces->GetXaxis()->SetTitle("clock tick [nsec]");
  rightTraces->GetYaxis()->SetTitle("trace [ADC]");
  rightTraces->SetMarkerStyle(33);
  rightTraces->SetMarkerColor(33);
  rightTraces->SetMarkerColor(kMagenta);

  printf("map fill loop\n");
  std::set<int> evtNumber_set;
  std::map<int,double> evtNumber_QDC_map;
  std::map<int,double> evtNumber_TOF_map;
  std::map<int,double> evtNumber_TDiff_map;
  std::map<int,int> evtNumber_barNum_map;
  std::map<int,double> evtNumber_vandle_left_phase_map;
  std::map<int,double> evtNumber_vandle_right_phase_map;
  std::map<int,double> evtNumber_beta_phase_map;
  for (int treeEntry = 0; treeEntry < dst->GetEntries(); treeEntry++){
    dst->GetEntry(treeEntry);
    if ( !(0<vandle_QDC&&vandle_QDC<4500) )continue;
    if ( !(-600<vandle_TOF&&vandle_TOF<1000) )continue;
    // if ( !(-30<vandle_TDiff&&vandle_TDiff<30) )continue;

    if (tcutg_walk_region->IsInside(vandle_TOF,vandle_QDC)==0)
      continue;

    evtNumber_set.insert(evtNumber);
    evtNumber_QDC_map[evtNumber]=vandle_QDC;
    evtNumber_TOF_map[evtNumber]=vandle_TOF;
    evtNumber_TDiff_map[evtNumber]=vandle_TDiff;
    evtNumber_barNum_map[evtNumber]=vandle_barNum;
    evtNumber_vandle_left_phase_map[evtNumber]=vandle_left_phase;
    evtNumber_vandle_right_phase_map[evtNumber]=vandle_right_phase;
    evtNumber_beta_phase_map[evtNumber]=beta_phase;

    // printf("evtNumber %d \n",evtNumber);
    // printf("evtNumber_QDC_map[%d] = %f \n",evtNumber,evtNumber_QDC_map[evtNumber]);
    // printf("evtNumber_TOF_map[%d] = %f \n",evtNumber,evtNumber_TOF_map[evtNumber]);
    // printf("evtNumber_TDiff_map[%d] = %f \n",evtNumber,evtNumber_TDiff_map[evtNumber]);
    // printf("evtNumber_barNum_map[%d] = %d \n\n\n",evtNumber,evtNumber_barNum_map[evtNumber]);
  }

  char evtNumber_char[345]="";
  printf("eventDisplay loop\n");

  TF1 *vandleTiming_TF1 = new TF1("vandleTimingFunc",VandleTiming,50,80,5);
  vandleTiming_TF1->SetLineColor(kBlue);
  vandleTiming_TF1->SetLineWidth(3);
  vandleTiming_TF1->SetParName(0,"#phi");
  vandleTiming_TF1->SetParName(1,"#alpha");
  vandleTiming_TF1->SetParName(2,"#beta");
  vandleTiming_TF1->SetParName(3,"#gamma");
  vandleTiming_TF1->SetParName(4,"C");

  for (auto evtN : evtNumber_set){

    sprintf(evtNumber_char,"evtNumber==%d",evtN);
    TGraph *point = new TGraph();
    point->SetMarkerStyle(33);
    point->SetMarkerSize(3.5);
    point->SetMarkerColor(kMagenta);

    TLegend* qdc_tof_legend = new TLegend(0.5,0.8,0.9,0.9);
    qdc_tof_legend->SetNColumns(2);
    sprintf(buffChar,"QDC = %.2f",evtNumber_QDC_map[evtN]);
    qdc_tof_legend->AddEntry((TObject*)0,buffChar, "");
    sprintf(buffChar,"TOF = %.2f",evtNumber_TOF_map[evtN]);
    qdc_tof_legend->AddEntry((TObject*)0,buffChar, "");

    c1->SetLogz(0);
    vandle_QDCvsTOF->SetTitle("");
    vandle_QDCvsTOF->Draw("colz");
    point->SetPoint(0,evtNumber_TOF_map[evtN],evtNumber_QDC_map[evtN]);
    point->Draw("P same");
    qdc_tof_legend->Draw();
    tcutg_walk_region->Draw("same");
    sprintf(buffChar,"plots/vandle_QDCvsTOF_evt%05d.png",evtN);
    c1->SaveAs(buffChar);

    TLegend* diff_bar_legend = new TLegend(0.5,0.8,0.9,0.9);
    diff_bar_legend->SetNColumns(2);
    sprintf(buffChar,"TDiff = %.2f",evtNumber_TDiff_map[evtN]);
    diff_bar_legend->AddEntry((TObject*)0,buffChar, "");
    sprintf(buffChar,"VANDLE bar = %d",evtNumber_barNum_map[evtN]);
    diff_bar_legend->AddEntry((TObject*)0,buffChar, "");

    vandle_barNum_VS_TDiff->SetTitle("");
    vandle_barNum_VS_TDiff->Draw("colz");
    point->SetPoint(0,evtNumber_TDiff_map[evtN],evtNumber_barNum_map[evtN]);
    point->Draw("P same");
    diff_bar_legend->Draw();
    sprintf(buffChar,"plots/vandle_barNum_VS_TDiff_evt%05d.png",evtN);
    c1->SaveAs(buffChar);

    c1->SetLogz(0);
    leftTraces->Draw("colz");
    dst->Draw("vandle_left_trace:Iteration$>>leftTraces",evtNumber_char,"P *",evtNumber);
    TH1D* leftTraces_1d = new TH1D("leftTraces_1d","leftTraces",130,0,130);
    leftTraces_1d->GetXaxis()->SetTitle("clock tick [nsec]");
    leftTraces_1d->GetYaxis()->SetTitle("trace [ADC]");
    leftTraces_1d->SetMarkerStyle(33);
    leftTraces_1d->SetMarkerSize(2);
    leftTraces_1d->SetMarkerColor(33);
    leftTraces_1d->SetMarkerColor(kMagenta);
    for (int tick_bin = 0; tick_bin <= leftTraces->GetNbinsX(); tick_bin++) {
      TH1D* proy = leftTraces->ProjectionY("proy",tick_bin,tick_bin);
      proy->Draw();
      leftTraces_1d->SetBinContent(tick_bin,proy->GetBinCenter(proy->FindFirstBinAbove(0)));
      delete proy;
    }
    vandleTiming_TF1->SetParameter(0,50);
    vandleTiming_TF1->SetParameter(1,3*leftTraces_1d->GetMaximum());
    vandleTiming_TF1->FixParameter(2,0.254373);
    vandleTiming_TF1->FixParameter(3,0.208072);
    vandleTiming_TF1->SetParameter(4,400);
    leftTraces_1d->Fit("vandleTimingFunc");

    rightTraces->Draw("colz");
    dst->Draw("vandle_right_trace:Iteration$>>rightTraces",evtNumber_char,"P *",evtNumber);
    TH1D* rightTraces_1d = new TH1D("rightTraces_1d","rightTraces",130,0,130);
    rightTraces_1d->GetXaxis()->SetTitle("clock tick [nsec]");
    rightTraces_1d->GetYaxis()->SetTitle("trace [ADC]");
    rightTraces_1d->SetMarkerStyle(33);
    rightTraces_1d->SetMarkerSize(2);
    rightTraces_1d->SetMarkerColor(33);
    rightTraces_1d->SetMarkerColor(kMagenta);
    for (int tick_bin = 0; tick_bin <= rightTraces->GetNbinsX(); tick_bin++) {
      TH1D* proy = rightTraces->ProjectionY("proy",tick_bin,tick_bin);
      proy->Draw();
      rightTraces_1d->SetBinContent(tick_bin,proy->GetBinCenter(proy->FindFirstBinAbove(0)));
      delete proy;
    }
    vandleTiming_TF1->SetParameter(0,50);
    vandleTiming_TF1->SetParameter(1,3*rightTraces_1d->GetMaximum());
    vandleTiming_TF1->FixParameter(2,0.254373);
    vandleTiming_TF1->FixParameter(3,0.208072);
    vandleTiming_TF1->SetParameter(4,400);
    rightTraces_1d->Fit("vandleTimingFunc");

    int trace_vertRange_max=3000,trace_vertRange_min=350;
    if (leftTraces_1d->GetMaximum() > rightTraces_1d->GetMaximum())
      trace_vertRange_max = 1.5*leftTraces_1d->GetMaximum();
    else
      trace_vertRange_max = 1.5*rightTraces_1d->GetMaximum();


    TLine* left_phase_line = new TLine(evtNumber_vandle_left_phase_map[evtN],trace_vertRange_min,evtNumber_vandle_left_phase_map[evtN],trace_vertRange_max);
    left_phase_line->SetLineWidth(3);
    left_phase_line->SetLineColor(kGreen);
    TLegend* left_phase_legend = new TLegend(0.1,0.8,0.4,0.9);
    sprintf(buffChar,"#phi [paass] = %2.2f",evtNumber_vandle_left_phase_map[evtN]);
    left_phase_legend->AddEntry(left_phase_line,buffChar);
    leftTraces_1d->Draw("P0");
    leftTraces_1d->GetXaxis()->SetRangeUser(20,90);
    leftTraces_1d->GetYaxis()->SetRangeUser(trace_vertRange_min,trace_vertRange_max);
    left_phase_line->Draw("same");
    left_phase_legend->Draw();
    c1->Update();
    sprintf(buffChar,"plots/vandle_ltrace_evt%05d.png",evtN);
    c1->SaveAs(buffChar);

    rightTraces_1d->Draw("P0");
    rightTraces_1d->GetXaxis()->SetRangeUser(20,90);
    rightTraces_1d->GetYaxis()->SetRangeUser(trace_vertRange_min,trace_vertRange_max);
    TLine* right_phase_line = new TLine(evtNumber_vandle_right_phase_map[evtN],trace_vertRange_min,evtNumber_vandle_right_phase_map[evtN],trace_vertRange_max);
    right_phase_line->SetLineWidth(3);
    right_phase_line->SetLineColor(kGreen);
    TLegend* right_phase_legend = new TLegend(0.1,0.8,0.4,0.9);
    sprintf(buffChar,"#phi [paass] = %2.2f",evtNumber_vandle_right_phase_map[evtN]);
    right_phase_legend->AddEntry(right_phase_line,buffChar);
    right_phase_line->Draw("same");
    right_phase_legend->Draw();
    c1->Update();
    sprintf(buffChar,"plots/vandle_rtrace_evt%05d.png",evtN);
    c1->SaveAs(buffChar);

    sprintf(buffChar,"gm montage -mode concatenate -tile 2x2 plots/vandle_barNum_VS_TDiff_evt%05d.png plots/vandle_QDCvsTOF_evt%05d.png plots/vandle_ltrace_evt%05d.png plots/vandle_rtrace_evt%05d.png evt%05d.png",evtN,evtN,evtN,evtN,evtN);
		system(buffChar);

    delete leftTraces_1d;
    delete rightTraces_1d;
    delete diff_bar_legend;
    delete qdc_tof_legend;

  }
}

///This defines the stock VANDLE timing function. Here is a breakdown of the
/// parameters:
/// * p[0] = phase
/// * p[1] = amplitude
/// * p[2] = beta
/// * p[3] = gamma
/// * p[4] = baseline
Double_t VandleTiming(Double_t *x, Double_t *p) {
    if (x[0] < p[0])
        return p[4];

    Float_t xx =x[0];
    Double_t fitval = p[1] * std::exp(-p[2] * (xx - p[0])) *
           (1 - std::exp(-std::pow(p[3] * (xx - p[0]), 4.))) + p[4];

    return fitval;
}
