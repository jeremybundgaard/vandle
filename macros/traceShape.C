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

void traceShape(const char *data_summary_tree = "",const char *otherTreeCuts = "1") {

 // TGaxis::SetMaxDigits(3);
 // gStyle->SetPalette(kCherry);
 gStyle->SetOptStat("n");
 gStyle->SetTitleOffset(1.2,"Y");
 gStyle->SetNumberContours(99);

 char buffChar[345];

 ///////// initialize track tree /////////
 printf("\n///////// initialize data_summary_tree /////////\n\n");
 TFile *treefile = new TFile(data_summary_tree);
 TTree *dst = (TTree *)treefile->Get("data_summary_tree");
 // dst->SetLineColor(kBlue);
 dst->SetLineWidth(5);

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

 TH2D* vandle_QDCvsTOF = new TH2D("vandle_QDCvsTOF","vandle_QDCvsTOF",430,-30,400,500,0,500);
 vandle_QDCvsTOF->SetStats(0);
 vandle_QDCvsTOF->GetXaxis()->SetTitle("ToF [4 nsec/bin]");
 vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC [ADC]");
 dst->Draw("vandle_QDC:vandle_TOF>>vandle_QDCvsTOF","vandle_QDC!=0","colz");

 TCanvas *wide_canvas = new TCanvas("wide_canvas", "wide_canvas", 3000, 700);
 wide_canvas->SetLogz();

 TCutG *tcutg_QDC = new TCutG("tcutg_QDC",5);
 tcutg_QDC->SetVarX("vandle_TOF");
 tcutg_QDC->SetVarY("vandle_QDC");
 tcutg_QDC->SetLineWidth(5);
 tcutg_QDC->SetLineColor(kGreen);

 TCutG* tcutg_gma = new TCutG("tcutg_gma",5);
 tcutg_gma->SetVarX("vandle_TOF");
 tcutg_gma->SetVarY("vandle_QDC");
 tcutg_gma->SetLineWidth(5);
 tcutg_gma->SetLineColor(kMagenta);
 tcutg_gma->SetPoint(0,-20,1);
 tcutg_gma->SetPoint(1,-20,5900);
 tcutg_gma->SetPoint(2,50,5900);
 tcutg_gma->SetPoint(3,50,1);
 tcutg_gma->SetPoint(4,-20,1);

 TCutG* tcutg_fast_nu = new TCutG("tcutg_fast_nu",5);
 tcutg_fast_nu->SetVarX("vandle_TOF");
 tcutg_fast_nu->SetVarY("vandle_QDC");
 tcutg_fast_nu->SetLineWidth(5);
 tcutg_fast_nu->SetLineColor(kBlue);
 tcutg_fast_nu->SetPoint(0,51,1);
 tcutg_fast_nu->SetPoint(1,51,5000);
 tcutg_fast_nu->SetPoint(2,100,5000);
 tcutg_fast_nu->SetPoint(3,100,1);
 tcutg_fast_nu->SetPoint(4,51,1);

 TCutG* tcutg_slower_nu = new TCutG("tcutg_slower_nu",5);
 tcutg_slower_nu->SetVarX("vandle_TOF");
 tcutg_slower_nu->SetVarY("vandle_QDC");
 tcutg_slower_nu->SetLineWidth(5);
 tcutg_slower_nu->SetLineColor(kRed);
 tcutg_slower_nu->SetPoint(0,101,1);
 tcutg_slower_nu->SetPoint(1,101,2000);
 tcutg_slower_nu->SetPoint(2,390,1500);
 tcutg_slower_nu->SetPoint(3,390,1);
 tcutg_slower_nu->SetPoint(4,101,1);
 tcutg_slower_nu->SetPoint(5,101,1);

 TCutG* tcutg_early_phase_traces = new TCutG("tcutg_early_phase_traces",5);
 tcutg_early_phase_traces->SetVarX("vandle_TOF");
 tcutg_early_phase_traces->SetVarY("vandle_QDC");
 tcutg_early_phase_traces->SetLineWidth(5);
 tcutg_early_phase_traces->SetLineColor(kRed);
 tcutg_early_phase_traces->SetPoint(0,101,1);
 tcutg_early_phase_traces->SetPoint(1,101,2000);
 tcutg_early_phase_traces->SetPoint(2,390,1500);
 tcutg_early_phase_traces->SetPoint(3,390,1);
 tcutg_early_phase_traces->SetPoint(4,101,1);
 tcutg_early_phase_traces->SetPoint(5,101,1);

 std::vector<std::string> tcutg_TOF;
 tcutg_TOF.push_back("tcutg_gma");
 tcutg_TOF.push_back("tcutg_fast_nu");
 tcutg_TOF.push_back("tcutg_slower_nu");

 char qdcVStof_QDC_char[234];
 char gamma_traces_plot_char[234],NORM_gamma_traces_plot_char[234];
 char fast_nu_traces_plot_char[234],NORM_fast_nu_traces_plot_char[234];
 char slower_nu_traces_plot_char[234],NORM_slower_nu_traces_plot_char[234];
 int QDC_width=5;

 for (int QDC_scanwindow = 20; QDC_scanwindow < 300; QDC_scanwindow+=5){

    tcutg_QDC->SetPoint(0,-200,QDC_scanwindow);
    tcutg_QDC->SetPoint(1,-200,QDC_scanwindow+QDC_width);
    tcutg_QDC->SetPoint(2,1000,QDC_scanwindow+QDC_width);
    tcutg_QDC->SetPoint(3,1000,QDC_scanwindow);
    tcutg_QDC->SetPoint(4,-200,QDC_scanwindow);

    // TCut scanCut = "tcutg_QDC&&vandle_QDC!=0";
    wide_canvas->cd();
    vandle_QDCvsTOF->Draw("colz");
    tcutg_gma->Draw("same");
    tcutg_fast_nu->Draw("same");
    tcutg_slower_nu->Draw("same");
    tcutg_QDC->Draw("same");
    sprintf(qdcVStof_QDC_char,"plots/vandle_QDCvsTOF_QDC%05d.png",QDC_scanwindow);
    wide_canvas->SaveAs(qdcVStof_QDC_char);

    for (auto i : tcutg_TOF) {
      TF1 *vandleTiming_TF1 = new TF1("vandleTimingFunc",VandleTiming,0,130,5);
      vandleTiming_TF1->SetLineWidth(5);
      vandleTiming_TF1->SetParName(0,"#phi");
      vandleTiming_TF1->SetParName(1,"#alpha");
      vandleTiming_TF1->SetParName(2,"#beta");
      vandleTiming_TF1->SetParName(3,"#gamma");
      vandleTiming_TF1->SetParName(4,"C");

      vandleTiming_TF1->SetParameter(0,50);
      // vandleTiming_TF1->SetParameter(1,3*trace_means->GetMaximum());
      vandleTiming_TF1->FixParameter(2,0.254373);
      vandleTiming_TF1->FixParameter(3,0.208072);
      vandleTiming_TF1->FixParameter(4,0);

     char cut_name[234];
     if(!strcmp("tcutg_gma",i.c_str())){
      vandleTiming_TF1->SetLineColor(kMagenta);
      sprintf(gamma_traces_plot_char,"plots/vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
      sprintf(NORM_gamma_traces_plot_char,"plots/NORM_vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
      sprintf(cut_name,"Gamma");
     }
     if(!strcmp("tcutg_fast_nu",i.c_str())){
      vandleTiming_TF1->SetLineColor(kBlue);
      sprintf(fast_nu_traces_plot_char,"plots/vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
      sprintf(NORM_fast_nu_traces_plot_char,"plots/NORM_vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
      sprintf(cut_name,"Faster Neutron");
     }
     if(!strcmp("tcutg_slower_nu",i.c_str())){
      vandleTiming_TF1->SetLineColor(kRed);
      sprintf(slower_nu_traces_plot_char,"plots/vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
      sprintf(NORM_slower_nu_traces_plot_char,"plots/NORM_vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
      sprintf(cut_name,"Slower Neutron");
     }

     TH2D* trace_hist_2d = new TH2D("trace_hist_2d","trace_hist_2d",130,0,130,150,-10,140);
     trace_hist_2d->GetXaxis()->SetTitle("tick [4 nsec/bin]");
     trace_hist_2d->GetXaxis()->SetRangeUser(20,120);
     trace_hist_2d->GetYaxis()->SetTitle("QDC");
     sprintf(buffChar,"tcutg_QDC&&%s",i.c_str());
     printf("%s\n",buffChar);
     dst->Draw("(vandle_ltrace-vandle_lAveBaseline):Iteration$>>trace_hist_2d",buffChar);
     if (trace_hist_2d->GetEntries()==0) continue;
     sprintf(buffChar,"%s traces [%d < QDC < %d]",cut_name,QDC_scanwindow-QDC_width,QDC_scanwindow+QDC_width);
     trace_hist_2d->SetTitle(buffChar);
     trace_hist_2d->Draw("colz");

     TH1D* trace_means = new TH1D("trace_means","trace_means",130,0,130);
     trace_means->SetName("fit_trace_means");
     trace_means->GetXaxis()->SetTitle("tick [nsec]");
     trace_means->GetXaxis()->SetRangeUser(20,120);
     trace_means->GetYaxis()->SetTitle("QDC");
     trace_means->SetMarkerColorAlpha(kGreen, 0.0);

     for (int tick_bin = 0; tick_bin <= trace_hist_2d->GetNbinsX(); tick_bin++) {
        TH1D* proy = trace_hist_2d->ProjectionY("proy",tick_bin,tick_bin);
        // printf("proy->GetMean():%f\n",proy->GetMean());
        proy->Draw();
        trace_means->SetBinContent(tick_bin,proy->GetMean());
        delete proy;
     }

     TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700); //c1->SetLogz();
     TH1D* NORM_trace_means = new TH1D("NORM_trace_means","NORM_trace_means",130,0,130);
     NORM_trace_means->SetName("fit_NORM_trace_means");
     NORM_trace_means->GetXaxis()->SetTitle("tick [nsec]");
     NORM_trace_means->GetXaxis()->SetRangeUser(20,120);
     NORM_trace_means->GetYaxis()->SetTitle("QDC");
     NORM_trace_means->SetMarkerColor(kGreen);
     NORM_trace_means->SetMarkerStyle(33);
     NORM_trace_means->SetMarkerSize(4);
     NORM_trace_means->SetLineWidth(5);

     // gStyle->SetStatStyle(0);
     gStyle->SetStatColor(0);
     gStyle->SetLegendFillColor(0);

     TLegend* legend = new TLegend(0.4,0.8,0.9,0.9);
     legend->AddEntry(vandleTiming_TF1,"#alpha e^{-(#tau-#phi)/#beta}(1 - e^{-(#tau-#phi)^{4}/#gamma}) + C ","l");

     trace_means->Fit("vandleTimingFunc");
     gStyle->SetOptFit(1);
     trace_hist_2d->SetStats(0);
     trace_hist_2d->GetYaxis()->SetRangeUser(-10,90);
     trace_hist_2d->Draw("colz");
     vandleTiming_TF1->Draw("same");
     trace_means->Draw("p same");

     legend->Draw();
     TPaveStats *st_trace_means = (TPaveStats*)trace_means->GetListOfFunctions()->FindObject("stats");
     st_trace_means->SetX1NDC(0.7); st_trace_means->SetX2NDC(0.9);
     st_trace_means->SetY1NDC(0.8); st_trace_means->SetY2NDC(0.35);

     sprintf(buffChar,"plots/vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
     c1->SaveAs(buffChar);

     for (int tick_bin = 0; tick_bin <= trace_means->GetNbinsX(); tick_bin++) {
       double mean = trace_means->GetBinContent(tick_bin);
        NORM_trace_means->SetBinContent(tick_bin,mean);
     }
     NORM_trace_means->Scale(1/NORM_trace_means->GetMaximum());
     NORM_trace_means->GetYaxis()->SetRangeUser(-0.1,1.4);

     vandleTiming_TF1->SetParameter(0,50);
     // vandleTiming_TF1->SetParameter(1,3*trace_means->GetMaximum());
     vandleTiming_TF1->FixParameter(2,0.254373);
     vandleTiming_TF1->FixParameter(3,0.208072);
     vandleTiming_TF1->SetParameter(4,0);

     NORM_trace_means->Fit("vandleTimingFunc");
     gStyle->SetOptFit(1);
     NORM_trace_means->Draw("hist p");
     vandleTiming_TF1->Draw("same");
     legend->Draw();
     TPaveStats *st_NORM_trace_means = (TPaveStats*)NORM_trace_means->GetListOfFunctions()->FindObject("stats");
     st_NORM_trace_means->SetX1NDC(0.7); st_NORM_trace_means->SetX2NDC(0.9);
     st_NORM_trace_means->SetY1NDC(0.8); st_NORM_trace_means->SetY2NDC(0.35);

     sprintf(buffChar,"plots/NORM_vandle_ltrace_QDC%05d_%s.png",QDC_scanwindow,i.c_str());
     c1->SaveAs(buffChar);

     delete trace_hist_2d;
     delete trace_means;
     delete NORM_trace_means;
     delete c1;
    }

  sprintf(buffChar,"gm montage -mode concatenate -tile 3x1 %s %s %s plots/traces.png",gamma_traces_plot_char,fast_nu_traces_plot_char,slower_nu_traces_plot_char);
  system(buffChar);

  sprintf(buffChar,"gm montage -mode concatenate -tile 3x1 %s %s %s plots/NORM_traces.png",NORM_gamma_traces_plot_char,NORM_fast_nu_traces_plot_char,NORM_slower_nu_traces_plot_char);
  system(buffChar);

  sprintf(buffChar,"gm montage -mode concatenate -tile 1x3 plots/NORM_traces.png plots/traces.png %s qdc_%05d.png",qdcVStof_QDC_char,QDC_scanwindow);
  system(buffChar);

  sprintf(buffChar,"rm %s %s %s %s plots/traces.png",gamma_traces_plot_char,fast_nu_traces_plot_char,slower_nu_traces_plot_char,qdcVStof_QDC_char);
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
