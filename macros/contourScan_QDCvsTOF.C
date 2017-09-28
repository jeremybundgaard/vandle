
#include "TMath.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPaveStats.h"

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

void contourScan_QDCvsTOF(const char *data_summary_tree = "",const char *otherTreeCuts = "1") {

  // TGaxis::SetMaxDigits(3);
  gStyle->SetPalette(kCMYK);
  gStyle->SetOptStat("ne");
  gStyle->SetTitleOffset(1.2,"Y");
  gStyle->SetNumberContours(99);

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize data_summary_tree /////////\n\n");
  TFile *treefile = new TFile(data_summary_tree);
  TTree *dst = (TTree *)treefile->Get("data_summary_tree");
  // dst->SetLineColor(kBlue);
  dst->SetLineWidth(6);

  unsigned int evtNumber=0;
  unsigned int vandle_barNum=0;
  double vandle_QDC=0;
  double vandle_TOF=0;
  double vandle_TDiff=0;
  dst->SetBranchAddress("evtNumber",&evtNumber);
  dst->SetBranchAddress("vandle_barNum",&vandle_barNum);
  dst->SetBranchAddress("vandle_QDC",&vandle_QDC);
  dst->SetBranchAddress("vandle_TOF",&vandle_TOF);
  dst->SetBranchAddress("vandle_TDiff",&vandle_TDiff);

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  // c1->SetLogz();

  TCutG *tcutg_walk_region = new TCutG("tcutg_walk_region",4);
  tcutg_walk_region->SetVarX("vandle_TOF");
  tcutg_walk_region->SetVarY("vandle_QDC");
  tcutg_walk_region->SetLineWidth(6);
  tcutg_walk_region->SetLineColor(kGreen);
  tcutg_walk_region->SetPoint(0,6,0);
  tcutg_walk_region->SetPoint(1,6,900);
  tcutg_walk_region->SetPoint(2,17,0);
  tcutg_walk_region->SetPoint(3,6,0);

  TCutG *tcutg_walk_region_mirrored = new TCutG("tcutg_walk_region_mirrored",4);
  tcutg_walk_region_mirrored->SetVarX("vandle_TOF");
  tcutg_walk_region_mirrored->SetVarY("vandle_QDC");
  tcutg_walk_region_mirrored->SetLineWidth(6);
  tcutg_walk_region_mirrored->SetLineColor(kBlue);
  tcutg_walk_region_mirrored->SetPoint(0,0,0);
  tcutg_walk_region_mirrored->SetPoint(1,0,900);
  tcutg_walk_region_mirrored->SetPoint(2,-3,0);
  tcutg_walk_region_mirrored->SetPoint(3,0,0);

  TH2D* trace_hist_2d = new TH2D("trace_hist_2d","trace_hist_2d",30,50,80,100,300,1700);
  trace_hist_2d->GetXaxis()->SetTitle("tick [nsec]");
  trace_hist_2d->GetYaxis()->SetTitle("QDC");

  TH1D* trace_means = new TH1D("trace_means","trace_means",30,50,80);
  trace_means->SetName("fit_trace_means");
  trace_means->GetXaxis()->SetTitle("tick [nsec]");
  trace_means->GetYaxis()->SetTitle("QDC");
  trace_means->SetMarkerColor(kGreen);
  trace_means->SetLineColor(kGreen);
  trace_means->SetMarkerStyle(33);
  trace_means->SetMarkerSize(2.5);
  trace_means->SetLineWidth(6);
  TH1D* trace_stddev_up = (TH1D*)trace_means->Clone("trace_stddev_up");
  trace_stddev_up->SetMarkerStyle(23);
  trace_stddev_up->SetMarkerSize(1.5);
  trace_stddev_up->SetMarkerColor(4);
  trace_stddev_up->SetLineColorAlpha(4,0.7);
  trace_stddev_up->SetLineWidth(6);
  TH1D* trace_stddev_down = (TH1D*)trace_means->Clone("trace_stddev_down");
  trace_stddev_down->SetMarkerStyle(22);
  trace_stddev_down->SetMarkerSize(1.5);
  trace_stddev_down->SetMarkerColor(4);
  trace_stddev_down->SetLineColorAlpha(4,0.7);
  trace_stddev_down->SetLineWidth(6);

  TH2D* vandle_QDCvsTOF = new TH2D("vandle_QDCvsTOF","vandle_QDCvsTOF",200,-10,40,50,0,1000);
  // vandle_QDCvsTOF->SetStats(0);
  vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [nsec]");
  vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
  dst->Draw("vandle_QDC:vandle_TOF>>vandle_QDCvsTOF","vandle_QDC!=0","colz");
  for (int zCut = 10; zCut < 400; zCut+=10) {
    TH2D *qdcVStof_zCut = (TH2D*)vandle_QDCvsTOF->Clone("qdcVStof_zCut");
    for (int tof_bin = 0; tof_bin < vandle_QDCvsTOF->GetNbinsX(); tof_bin++) {
      for (int qdc_bin = 0; qdc_bin < vandle_QDCvsTOF->GetNbinsY(); qdc_bin++) {
      if (vandle_QDCvsTOF->GetBinContent(tof_bin,qdc_bin)>zCut) qdcVStof_zCut->SetBinContent(tof_bin,qdc_bin,0);
      }
    }
    qdcVStof_zCut->SetEntries(qdcVStof_zCut->Integral());
    sprintf(buffChar,"vandle_QDCvsTOF{color > %03d}",zCut);
    qdcVStof_zCut->SetTitle(buffChar);
    qdcVStof_zCut->Draw("colz");
    tcutg_walk_region->Draw("same");
    tcutg_walk_region_mirrored->Draw("same");
    sprintf(buffChar,"qdcVStof_zCut%03d.png",zCut);
    c1->SaveAs(buffChar);

    c1->SetLogz(0);
    dst->Draw("vandle_ltrace:Iteration$>>trace_hist_2d","tcutg_walk_region");
    trace_hist_2d->Draw("colz");

    for (int tick_bin = 0; tick_bin <= trace_hist_2d->GetNbinsX(); tick_bin++) {
      TH1D* proy = trace_hist_2d->ProjectionY("proy",tick_bin,tick_bin);
      // printf("proy->GetMean():%f\n",proy->GetMean());
      proy->Draw();
      trace_stddev_up->SetBinContent(tick_bin,proy->GetMean()+proy->GetStdDev());
      trace_means->SetBinContent(tick_bin,proy->GetMean());
      trace_stddev_down->SetBinContent(tick_bin,proy->GetMean()-proy->GetStdDev());
      delete proy;
    }

    TF1 *vandleTiming_TF1 = new TF1("vandleTimingFunc",VandleTiming,50,80,5);
    vandleTiming_TF1->SetLineColor(kGreen);
    vandleTiming_TF1->SetLineWidth(6);
    vandleTiming_TF1->SetParName(0,"#phi");
    vandleTiming_TF1->SetParName(1,"#alpha");
    vandleTiming_TF1->SetParName(2,"#beta");
    vandleTiming_TF1->SetParName(3,"#gamma");
    vandleTiming_TF1->SetParName(4,"C");

    vandleTiming_TF1->SetParameter(0,50);
    vandleTiming_TF1->SetParameter(1,3*trace_means->GetMaximum());
    vandleTiming_TF1->SetParameter(2,0.25);
    vandleTiming_TF1->SetParameter(3,0.19);
    vandleTiming_TF1->SetParameter(4,400);

    // gStyle->SetStatStyle(0);
    gStyle->SetStatColor(0);
    gStyle->SetLegendFillColor(0);

    TLegend* legend = new TLegend(0.4,0.8,0.9,0.9);
    legend->AddEntry(vandleTiming_TF1,"#alpha e^{-(#tau-#phi)/#beta}(1 - e^{-(#tau-#phi)^{4}/#gamma}) + C ","l");
    // legend->AddEntry(trace_means,"avg/tick","p");
    // legend->AddEntry(trace_stddev_up,"#pm #sigma/tick","p");

    trace_means->Fit("vandleTimingFunc");
    gStyle->SetOptFit(1);
    trace_hist_2d->SetStats(0);
    trace_hist_2d->Draw("colz");
    vandleTiming_TF1->Draw("same");
    trace_means->Draw("p same");
    // trace_stddev_up->Draw("p same");
    // trace_stddev_down->Draw("p same");

    legend->Draw();
    TPaveStats *st = (TPaveStats*)trace_means->GetListOfFunctions()->FindObject("stats");
    st->SetX1NDC(0.6); st->SetX2NDC(0.9);
    st->SetY1NDC(0.8); st->SetY2NDC(0.35);

    // sprintf(buffChar,"plots/vandle_ltrace_QDC%d.svg",QDC_scanwindow);
    // c1->SaveAs(buffChar);

    sprintf(buffChar,"plots/vandle_ltrace_QDC.png");
    c1->SaveAs(buffChar);

    sprintf(buffChar,"gm montage -mode concatenate -tile 1x2 plots/qdcVStof_zCut%03d.png plots/vandle_ltrace_QDC.png plots/QDC_zCut%03d.png",zCut,zCut);
		system(buffChar);
    sprintf(buffChar,"rm plots/vandle_QDCvsTOF.png plots/vandle_ltrace_QDC.png");
		system(buffChar);

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
