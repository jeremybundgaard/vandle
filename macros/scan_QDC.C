
#include "TAxis.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
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


void scan_QDC(const char *data_summary_tree = "",const char *otherTreeCuts = "1") {

  // TGaxis::SetMaxDigits(3);
  gStyle->SetPalette(kCherry);
  gStyle->SetOptStat("n");
  gStyle->SetTitleOffset(1.2,"Y");
  gStyle->SetNumberContours(99);

  char buffChar[345];

  ///////// initialize track tree /////////
  printf("\n///////// initialize data_summary_tree /////////\n\n");
  TFile *treefile = new TFile(data_summary_tree);
  TTree *dst = (TTree *)treefile->Get("data_summary_tree");
  // dst->SetLineColor(kBlue);
  dst->SetLineWidth(4);

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

  TH2D* vandle_QDCvsTOF = new TH2D("vandle_QDCvsTOF","vandle_QDCvsTOF",400,-300,1100,300,0,6000);
  vandle_QDCvsTOF->SetStats(0);
  vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [nsec]");
  vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
  dst->Draw("vandle_QDC:vandle_TOF>>vandle_QDCvsTOF","vandle_QDC!=0","colz");

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetLogz();

  TCutG *tcutg_QDC = new TCutG("tcutg_QDC",5);
  tcutg_QDC->SetVarX("vandle_TOF");
  tcutg_QDC->SetVarY("vandle_QDC");
  tcutg_QDC->SetLineWidth(5);
  tcutg_QDC->SetLineColor(kGreen);

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
  trace_means->SetMarkerSize(1.5);
  trace_means->SetLineWidth(4);
  TH1D* trace_stddev_up = (TH1D*)trace_means->Clone("trace_stddev_up");
  trace_stddev_up->SetMarkerStyle(23);
  trace_stddev_up->SetMarkerSize(1.5);
  trace_stddev_up->SetMarkerColor(kGreen);
  trace_stddev_up->SetLineColorAlpha(kGreen,0.7);
  trace_stddev_up->SetLineWidth(2);
  TH1D* trace_stddev_down = (TH1D*)trace_means->Clone("trace_stddev_down");
  trace_stddev_down->SetMarkerStyle(22);
  trace_stddev_down->SetMarkerSize(1.5);
  trace_stddev_down->SetMarkerColor(kGreen);
  trace_stddev_down->SetLineColorAlpha(kGreen,0.7);
  trace_stddev_down->SetLineWidth(2);


  char QDC_char[234];
  int QDC_width=20;
  for (int QDC_scanwindow = 3500; QDC_scanwindow >= QDC_width; QDC_scanwindow-=100){

    tcutg_QDC->SetPoint(0,-200,QDC_scanwindow);
    tcutg_QDC->SetPoint(1,-200,QDC_scanwindow+QDC_width);
    tcutg_QDC->SetPoint(2,1000,QDC_scanwindow+QDC_width);
    tcutg_QDC->SetPoint(3,1000,QDC_scanwindow);
    tcutg_QDC->SetPoint(4,-200,QDC_scanwindow);

    // TCut scanCut = "tcutg_QDC&&vandle_QDC!=0";
    c1->cd();
    c1->SetLogz(1);
    vandle_QDCvsTOF->Draw("colz");
    tcutg_QDC->Draw("same");
    sprintf(buffChar,"plots/vandle_QDCvsTOF_QDC.png");
    c1->SaveAs(buffChar);

    c1->SetLogz(0);
    dst->Draw("vandle_ltrace:Iteration$>>trace_hist_2d","tcutg_QDC");
    sprintf(buffChar,"trace_hist_2d [%d < QDC < %d]",QDC_scanwindow-QDC_width,QDC_scanwindow+QDC_width);
    trace_hist_2d->SetTitle(buffChar);
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

    sprintf(buffChar,"plots/trace_means.png");
    c1->SaveAs(buffChar);

    sprintf(buffChar,"plots/vandle_ltrace_QDC.png");
    c1->SaveAs(buffChar);

    sprintf(buffChar,"gm montage -mode concatenate -tile 1x2 plots/vandle_QDCvsTOF_QDC.png plots/vandle_ltrace_QDC.png plots/QDC%04d.png",QDC_scanwindow);
		system(buffChar);
    sprintf(buffChar,"rm plots/vandle_QDCvsTOF_QDC.png plots/vandle_ltrace_QDC.png");
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
