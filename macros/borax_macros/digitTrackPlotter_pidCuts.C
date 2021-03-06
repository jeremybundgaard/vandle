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

void digitTrackPlotter_pidCuts(int preamp, const char *actinide = "",
                               const char *pidCut = "",
                               const char *otherTreeCuts = "1",
                               const char *TPCSummaryTree = "",
                               const char *TPCSummaryDigitTree = "",
                               const char *pidFileName = "") {

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  c1->SetGrid();
  TCanvas *dig_canvas = new TCanvas("dig_canvas", "dig_canvas", 1400, 1400);
  char c1PlotName[234];
  gStyle->SetPalette(63);
  gStyle->SetOptStat(0);
  // gStyle->SetLegendBorderSize(0);
  gStyle->SetNumberContours(99);
  char buffChar[345];

  double MeV_per_ADC = 0;
  double ADC_per_MeV = 0;
  double dEnergy = 0.1;
  double isoTopicCalib = 0.9186;
  double pi = 4 * atan(1);
  double deg2rad = pi / (double)180;
  char azCut[456];

  std::set<int> run_set_noMask;
  char noMaskRunCut[234];
  char noMaskCut[234];
  std::set<int> run_set_cft;
  char cftRunCut[234];
  char cftCut[234];
  if (preamp == 1) {
    ADC_per_MeV = 1180 / 6.118;
    MeV_per_ADC = 6.118 / 1180;
    if (!(strcmp(actinide, "cf252"))) {
      run_set_noMask.insert(100001850);
      run_set_noMask.insert(100001851);
      run_set_noMask.insert(100001852);
      run_set_noMask.insert(100001853);
      run_set_noMask.insert(100001854);
      run_set_noMask.insert(100001855);
      run_set_noMask.insert(100001856);
      run_set_noMask.insert(100001866);
      run_set_noMask.insert(100001867);
      run_set_noMask.insert(100001868);
      run_set_noMask.insert(100001869);
      run_set_noMask.insert(100001871);
      run_set_noMask.insert(100001872);
      run_set_noMask.insert(100001874);
      run_set_noMask.insert(100001875);
      run_set_noMask.insert(100001876);
      run_set_noMask.insert(100001877);
      run_set_noMask.insert(100001878);
      run_set_noMask.insert(100001879);
      run_set_noMask.insert(100001880);
      run_set_noMask.insert(100001881);
      run_set_noMask.insert(100001882);
      sprintf(noMaskRunCut, "run>100001856");
    }
    sprintf(noMaskCut, " %s", noMaskRunCut);
  } else if (preamp == 2) {
    ADC_per_MeV = 1450 / 6.118;
    MeV_per_ADC = 6.118 / 1450;
    if (!(strcmp(actinide, "cf252"))) {
      sprintf(azCut, "!(-1<direction.Phi()&&direction.Phi()<0)");
      run_set_noMask.insert(100002055);
      run_set_noMask.insert(100002056);
      run_set_noMask.insert(100002057);
      run_set_noMask.insert(100002058);
      run_set_noMask.insert(100002059);
      run_set_noMask.insert(100002060);
      run_set_noMask.insert(100002061);
    }
    if (!(strcmp(actinide, "cm244"))) {
      sprintf(azCut, "direction.Phi()<2.2");
    }
    sprintf(noMaskCut, "%s", azCut);
    sprintf(cftCut, "%s", azCut);
  }

  std::ifstream csvFile;
  char csvFileName[345];
  if (!(strcmp(pidCut, "fragCut_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "fragCut_adc_events.csv");
  if (!(strcmp(pidCut, "fragCut_unlikely_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "fragCut_unlikely_adc_events.csv");
  if (!(strcmp(pidCut, "alphaCut_zCut_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "alphaCut_zCut_adc_events.csv");
  if (!(strcmp(pidCut, "alphaCut_long_low_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "alphaCut_long_low_adc_events.csv");
  if (!(strcmp(pidCut, "alphaCut_long_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "alphaCut_long_adc_events.csv");
  if (!(strcmp(pidCut, "long_high_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "long_high_adc_events.csv");
  if (!(strcmp(pidCut, "alphaCut_short_higher_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "alphaCut_short_higher_adc_events.csv");
  if (!(strcmp(pidCut, "alphaCut_short_low_adc")))
    sprintf(csvFileName, "/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts/"
                         "alphaCut_short_low_adc_events.csv");

  std::set<long> pidCutEventSet;
  csvFile.open(csvFileName);
  long tmp = 0;
  printf("csvFile.is_open():%d\n", csvFile.is_open());
  while (csvFile >> tmp) {
    pidCutEventSet.insert(tmp);
    // std::cout<<tmp<<std::endl;
  }
  csvFile.close();
  printf("pidCutEventSet.size():%lu\n", pidCutEventSet.size());

  ////////////////////// initialize cuts from pid file
  //////////////////////////////////////
  printf("////////////////////// initialize cuts from pid file:%s "
         "///////////////////////////////////\n",
         pidFileName);
  /////////  create TCutG chars ///////////
  char fragCut_adc_char[345];
  char fragCut_unlikely_adc_char[345];
  char alphaCut_adc_char[345];
  char alpha_short_high_adc_char[345];
  char alpha_short_low_adc_char[345];
  char above_zCut_adc_char[345];
  // sprintf(fragCut_unlikely_adc_char, "fragCut_unlikely_adc");
  sprintf(fragCut_unlikely_adc_char, "low_adc");
  sprintf(fragCut_adc_char, "fragCut_adc");
  sprintf(alphaCut_adc_char, "alphaCut_adc");
  sprintf(alpha_short_high_adc_char, "alphaCut_short_higher_adc");
  sprintf(alpha_short_low_adc_char, "alphaCut_short_low_adc");
  sprintf(above_zCut_adc_char, "alphaCut_zCut_adc");
  char fragCut_energy_char[345];
  char fragCut_unlikely_energy_char[345];
  char alphaCut_energy_char[345];
  char alpha_short_high_energy_char[345];
  char alpha_short_low_energy_char[345];
  char above_zCut_energy_char[345];
  sprintf(fragCut_unlikely_energy_char, "low_energy");
  // sprintf(fragCut_unlikely_energy_char, "fragCut_unlikely_energy");
  sprintf(fragCut_energy_char, "fragCut_energy");
  sprintf(alphaCut_energy_char, "alphaCut_energy");
  sprintf(alpha_short_high_energy_char, "alphaCut_short_higher_energy");
  sprintf(alpha_short_low_energy_char, "alphaCut_short_low_energy");
  sprintf(above_zCut_energy_char, "alphaCut_zCut_energy");
  // fragCut_adc is crucial for invoking this cut in ->Draw()
  TFile *pidFile = new TFile(pidFileName);
  TCutG *fragCut_adc = (TCutG *)pidFile->Get(fragCut_adc_char);
  fragCut_adc->SetName("fragCut_adc");
  fragCut_adc->SetLineColor(kViolet);
  TCutG *fragCut_unlikely_adc =
      (TCutG *)pidFile->Get(fragCut_unlikely_adc_char);
  fragCut_unlikely_adc->SetName("fragCut_unlikely_adc");
  fragCut_unlikely_adc->SetLineColor(kViolet - 9);
  TCutG *alphaCut_zCut_adc = (TCutG *)pidFile->Get(above_zCut_adc_char);
  alphaCut_zCut_adc->SetName("alphaCut_zCut_adc");
  alphaCut_zCut_adc->SetLineColor(kSpring);
  TCutG *alphaCut_long_low_adc = (TCutG *)pidFile->Get("alphaCut_long_low_adc");
  alphaCut_long_low_adc->SetName("alphaCut_long_low_adc");
  alphaCut_long_low_adc->SetLineColor(kYellow);
  TCutG *alphaCut_long_adc = (TCutG *)pidFile->Get("alphaCut_long_adc");
  alphaCut_long_adc->SetName("alphaCut_long_adc");
  alphaCut_long_adc->SetLineColor(kCyan);
  TCutG *long_high_adc = (TCutG *)pidFile->Get("long_high_adc");
  long_high_adc->SetName("long_high_adc");
  long_high_adc->SetLineColor(kBlue);
  TCutG *alphaCut_short_higher_adc =
      (TCutG *)pidFile->Get(alpha_short_high_adc_char);
  alphaCut_short_higher_adc->SetName("alphaCut_short_higher_adc");
  alphaCut_short_higher_adc->SetLineColor(kOrange + 7);
  TCutG *alphaCut_short_low_adc =
      (TCutG *)pidFile->Get(alpha_short_low_adc_char);
  alphaCut_short_low_adc->SetName("alphaCut_short_low_adc");
  alphaCut_short_low_adc->SetLineColor(kRed);

  if (!(strcmp(pidCut, "fragCut_adc"))) {
    fragCut_adc->SetLineWidth(4);
    fragCut_adc->SetFillColorAlpha(kViolet, 0.2);
    fragCut_adc->SetLineColor(kViolet);
  };
  if (!(strcmp(pidCut, "fragCut_unlikely_adc"))) {
    fragCut_unlikely_adc->SetLineWidth(4);
    fragCut_unlikely_adc->SetFillColorAlpha(kViolet - 9, 0.2);
    fragCut_unlikely_adc->SetLineColor(kViolet - 9);
  };
  if (!(strcmp(pidCut, "alphaCut_zCut_adc"))) {
    alphaCut_zCut_adc->SetLineWidth(4);
    alphaCut_zCut_adc->SetFillColorAlpha(kSpring, 0.2);
    alphaCut_zCut_adc->SetLineColor(kSpring);
  };
  if (!(strcmp(pidCut, "alphaCut_long_low_adc"))) {
    alphaCut_long_low_adc->SetLineWidth(4);
    alphaCut_long_low_adc->SetFillColorAlpha(kYellow, 0.2);
    alphaCut_long_low_adc->SetLineColor(kYellow);
  };
  if (!(strcmp(pidCut, "alphaCut_long_adc"))) {
    alphaCut_long_adc->SetLineWidth(4);
    alphaCut_long_adc->SetFillColorAlpha(kCyan, 0.2);
    alphaCut_long_adc->SetLineColor(kCyan);
  };
  if (!(strcmp(pidCut, "long_high_adc"))) {
    long_high_adc->SetLineWidth(4);
    long_high_adc->SetFillColorAlpha(kBlue, 0.2);
    long_high_adc->SetLineColor(kBlue);
  };
  if (!(strcmp(pidCut, "alphaCut_short_higher_adc"))) {
    alphaCut_short_higher_adc->SetLineWidth(4);
    alphaCut_short_higher_adc->SetFillColorAlpha(kOrange + 7, 0.2);
    alphaCut_short_higher_adc->SetLineColor(kOrange + 7);
  };
  if (!(strcmp(pidCut, "alphaCut_short_low_adc"))) {
    alphaCut_short_low_adc->SetLineWidth(4);
    alphaCut_short_low_adc->SetFillColorAlpha(kRed, 0.2);
    alphaCut_short_low_adc->SetLineColor(kRed);
  };

  ///////// initialize track tree /////////
  printf("\n///////// initialize track tree /////////\n\n");
  printf("TPCSummaryTree: %s\n", TPCSummaryTree);
  TFile *trackFile = new TFile(TPCSummaryTree);
  TTree *tracks = (TTree *)trackFile->Get("tracks");
  ///////// initialize digit tree /////////
  printf("TPCSummaryDigitTree: %s\n", TPCSummaryDigitTree);
  TFile *digitFile = new TFile(TPCSummaryDigitTree);
  TTree *digits = (TTree *)digitFile->Get("digits");
  TDirectory *cwd = gDirectory; // we create histograms "here"
  TH3F *track3d =
      new TH3F("track3d", "track3d", 12, -7, 7, 12, -7, 7, 12, -7, 7);
  track3d->SetStats(kFALSE);
  digits->SetMarkerStyle(20);
  tracks->SetMarkerStyle(20);
  tracks->SetMarkerColor(kBlack);
  cwd->cd(); // go to the directory where histograms are

  TH2D *yx_digProjection =
      new TH2D("yx_digProjection", "", 100, -7, 7, 100, -7, 7);
  yx_digProjection->GetZaxis()->SetLabelOffset(2);
  TH2D *zy_digProjection =
      new TH2D("zy_digProjection", "", 100, -7, 7, 100, -8, 8);
  zy_digProjection->GetZaxis()->SetLabelOffset(2);
  TH2D *zx_digProjection =
      new TH2D("zx_digProjection", "", 100, -7, 7, 100, -8, 8);
  zx_digProjection->GetZaxis()->SetLabelOffset(2);

  TH2D *LvsADC = new TH2D("LvsADC", "LvsADC", 4000, 0, 40000, 1000, 0, 10);
  LvsADC->GetXaxis()->SetTitle("energy [ADC]");
  LvsADC->GetXaxis()->SetRangeUser(0, 3750);
  LvsADC->GetYaxis()->SetRangeUser(0, 10);
  LvsADC->GetYaxis()->SetTitle("length [cm]");
  TH2D *ADCvsCosTheta =
      new TH2D("ADCvsCosTheta", "ADCvsCosTheta", 1000, -1, 1, 4000, 0, 40000);
  ADCvsCosTheta->GetXaxis()->SetTitle("cos #theta");
  ADCvsCosTheta->GetYaxis()->SetTitle("energy [ADC]");
  ADCvsCosTheta->GetYaxis()->SetRangeUser(100, 40000);
  double cosTheta[10];
  TH1F *trackCount_hist =
      new TH1F("trackCount_hist", "trackCount_hist", 10, 0, 10);
  TH1F *trackADC = new TH1F("trackADC", "trackADC", 40000, 0, 40000);
  TH1F *trackCosTheta = new TH1F("trackCosTheta", "trackCosTheta", 1000, -1, 1);
  double trk_adc[10];
  TH1F *trackLength = new TH1F("trackLength", "trackLength", 1000, 0, 10);
  double trk_length[10];
  TH1F *start_hist = new TH1F("start_hist", "start_hist", 1000, -20, 20);
  double start_x[10];
  double start_y[10];
  double start_z[10];
  TH1F *end_hist = new TH1F("end_hist", "end_hist", 1000, -20, 20);
  double end_x[10];
  double end_y[10];
  double end_z[10];

  for (auto run : run_set_noMask) {

    printf("\n\n");
    printf("run:%d\n", run);
    TH1D *eventID_loopHist =
        new TH1D("eventID_loopHist", "eventID_loopHist", 20000000, 0, 20000000);
    c1->cd();
    digits->Draw("eventID>>eventID_loopHist");
    const int first_event = eventID_loopHist->FindFirstBinAbove(0);
    const int last_event = eventID_loopHist->FindLastBinAbove(0);
    printf("first_event:%d\n", first_event);
    printf("last_event:%d\n", last_event);
    delete eventID_loopHist;

    for (int event_id = first_event; event_id <= last_event; event_id++) {
      char runevenID_char[234];
      sprintf(runevenID_char, "%d%d", run, event_id);
      long runevenID;
      runevenID = atol(runevenID_char);
      std::set<long>::iterator it = pidCutEventSet.find(runevenID);
      if (it == pidCutEventSet.end())
        continue;
      std::cout << "*it:" << *it << std::endl;

      char individ_eventCut[234];
      sprintf(individ_eventCut, "eventID==%d&&run==%d", event_id, run);

      tracks->Draw("trackCount>>trackCount_hist", individ_eventCut);
      int trk_count = trackCount_hist->FindLastBinAbove(0) - 1;
      printf("\n\n**********************************************\n");
      printf("**********************************************\n");
      printf("	eventID: %d  \n", event_id);
      printf("**********************************************\n");
      printf("**********************************************\n");
      printf("trk_count:%d\n", trk_count);

      TLegend *trk_legend = new TLegend(0.105, 0.6, 0.3, 0.9);
      trk_legend->SetTextAlign(11);
      trk_legend->SetTextSize(.02);
      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        sprintf(buffChar, "%s&&trackNo==%d", individ_eventCut, trk_n);
        tracks->Draw("adc>>trackADC", buffChar);
        trk_adc[trk_n] = trackADC->GetMean();
        tracks->Draw("length>>trackLength", buffChar);
        trk_length[trk_n] = trackLength->GetMean();
        printf("\t trk_adc[%d]: %f\n", trk_n, trk_adc[trk_n]);
        printf("\t trk_length[%d]: %f\n", trk_n, trk_length[trk_n]);
        LvsADC->Fill(trk_adc[trk_n], trk_length[trk_n]);
        tracks->Draw("direction.CosTheta()>>trackCosTheta", buffChar);
        cosTheta[trk_n] = trackCosTheta->GetMean();
        ADCvsCosTheta->Fill(cosTheta[trk_n], trk_adc[trk_n]);
        tracks->Draw("start.x()>>start_hist", buffChar);
        start_x[trk_n] = start_hist->GetMean();
        tracks->Draw("start.y()>>start_hist", buffChar);
        start_y[trk_n] = start_hist->GetMean();
        tracks->Draw("start.z()>>start_hist", buffChar);
        start_z[trk_n] = start_hist->GetMean();
        tracks->Draw("end.x()>>end_hist", buffChar);
        end_x[trk_n] = end_hist->GetMean();
        tracks->Draw("end.y()>>end_hist", buffChar);
        end_y[trk_n] = end_hist->GetMean();
        tracks->Draw("end.z()>>end_hist", buffChar);
        end_z[trk_n] = end_hist->GetMean();
        printf("\t trk_n:%d, start_x:%f, start_y:%f, start_z:%f\n", trk_n,
               start_x[trk_n], start_y[trk_n], start_z[trk_n]);
        printf("\t trk_n:%d, end_x:%f, end_y:%f, end_z:%f\n\n", trk_n,
               end_x[trk_n], end_y[trk_n], end_z[trk_n]);

        sprintf(buffChar, "trackNo: %d", trk_n);
        TLegendEntry *l1 = trk_legend->AddEntry((TObject *)0, buffChar, "");
        l1->SetTextColor(2 + 2 * trk_n);
        sprintf(buffChar, "adc: %0.2f", trk_adc[trk_n]);
        l1 = trk_legend->AddEntry((TObject *)0, buffChar, "");
        l1->SetTextColor(2 + 2 * trk_n);
        sprintf(buffChar, "length: %0.2f", trk_length[trk_n]);
        l1 = trk_legend->AddEntry((TObject *)0, buffChar, "");
        l1->SetTextColor(2 + 2 * trk_n);
        // sprintf(buffChar,"start.x: %0.2f \tstart.y:
        // %0.2f",start_x[trk_n],start_y[trk_n]);
        // l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");
        // sprintf(buffChar,"end.x: %0.2f \tend.y:
        // %0.2f",end_x[trk_n],end_y[trk_n]);
        // l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");
        l1 = trk_legend->AddEntry((TObject *)0, "", "");
      }

      track3d->SetTitle(individ_eventCut);
      track3d->Reset("M");
      if (!(track3d->TestBit(TH1::kNoStats))) {
        digits->Project("track3d", "x:y:z", individ_eventCut, "");
        ((TArrayF *)track3d)->Reset();
      }
      dig_canvas->cd();

      digits->Draw("z:y:x:adc>>+track3d", individ_eventCut, "col");
      track3d->GetXaxis()->SetTitle("X [cm]");
      track3d->GetXaxis()->SetTitleOffset(1.6);
      track3d->GetYaxis()->SetTitle("Y [cm]");
      track3d->GetYaxis()->SetTitleOffset(1.6);
      track3d->GetZaxis()->SetTitle("Z [cm]");
      TPolyLine3D *pl_hex_plus = new TPolyLine3D(5);
      pl_hex_plus->SetLineColor(kSpring);
      pl_hex_plus->SetLineWidth(2);
      for (int n = 0; n <= 6; ++n)
        pl_hex_plus->SetPoint(n, 5 * TMath::Cos(n * 60 * TMath::Pi() / 180),
                              5 * TMath::Sin(n * 60 * TMath::Pi() / 180), 5);
      pl_hex_plus->Draw("same");
      TPolyLine3D *pl_hex_minus = new TPolyLine3D(5);
      pl_hex_minus->SetLineColor(kSpring);
      pl_hex_minus->SetLineWidth(2);
      for (int n = 0; n <= 6; ++n)
        pl_hex_minus->SetPoint(n, 5 * TMath::Cos(n * 60 * TMath::Pi() / 180),
                               5 * TMath::Sin(n * 60 * TMath::Pi() / 180), -5);
      pl_hex_minus->Draw("same");
      TPolyLine3D *pl_circle = new TPolyLine3D(5);
      pl_circle->SetLineColor(kRed);
      pl_circle->SetLineWidth(2);
      for (int n = 0; n <= 60; ++n)
        pl_circle->SetPoint(n, 1 * TMath::Cos(n * 6 * TMath::Pi() / 180),
                            1 * TMath::Sin(n * 6 * TMath::Pi() / 180), 0);
      pl_circle->Draw("same");
      trk_legend->Draw();

      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        if (trk_length[trk_n] == 0 || trk_adc[trk_n] == 0)
          continue;
        TPolyLine3D *pl_track = new TPolyLine3D(2);
        pl_track->SetLineColor(2 + 2 * trk_n);
        pl_track->SetLineWidth(6);
        pl_track->SetPoint(0, start_x[trk_n], start_y[trk_n], start_z[trk_n]);
        pl_track->SetPoint(1, end_x[trk_n], end_y[trk_n], end_z[trk_n]);
        pl_track->Draw();
      }

      dig_canvas->Modified();
      dig_canvas->Update();
      sprintf(buffChar, "eventID%07d.png", event_id);
      dig_canvas->SaveAs(buffChar);

      c1->cd();
      c1->SetLogz(1);
      tracks->Draw("adc:direction.CosTheta()>>ADCvsCosTheta", azCut, "colz");
      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        if (trk_length[trk_n] == 0 || trk_adc[trk_n] == 0)
          continue;
        TGraph *gpoint_adcVcostheta = new TGraph();
        gpoint_adcVcostheta->SetMarkerStyle(33);
        gpoint_adcVcostheta->SetMarkerSize(3.5);
        gpoint_adcVcostheta->SetMarkerColor(2 + 2 * trk_n);
        gpoint_adcVcostheta->SetPoint(0, cosTheta[trk_n], trk_adc[trk_n]);
        gpoint_adcVcostheta->Draw("P same");
      }
      c1->SetLogx(0);
      c1->SetLogy(1);
      c1->Modified();
      c1->Update();
      sprintf(buffChar, "eventID%07d_EnVsCosTheta.png", event_id);
      c1->SaveAs(buffChar);
      c1->SetLogy(0);

      tracks->Draw("length:adc>>LvsADC", azCut, "colz");
      c1->SetLogx(0);
      c1->Modified();
      c1->Update();
      if (!(strcmp(pidCut, "fragCut_adc")))
        fragCut_adc->Draw("same");
      if (!(strcmp(pidCut, "fragCut_unlikely_adc")))
        fragCut_unlikely_adc->Draw("same");
      if (!(strcmp(pidCut, "alphaCut_zCut_adc")))
        alphaCut_zCut_adc->Draw("same");
      if (!(strcmp(pidCut, "alphaCut_long_low_adc")))
        alphaCut_long_low_adc->Draw("same");
      if (!(strcmp(pidCut, "alphaCut_long_adc")))
        alphaCut_long_adc->Draw("same");
      if (!(strcmp(pidCut, "long_high_adc")))
        long_high_adc->Draw("same");
      if (!(strcmp(pidCut, "alphaCut_short_higher_adc")))
        alphaCut_short_higher_adc->Draw("same");
      if (!(strcmp(pidCut, "alphaCut_short_low_adc")))
        alphaCut_short_low_adc->Draw("same");

      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        if (trk_length[trk_n] == 0 || trk_adc[trk_n] == 0)
          continue;
        TGraph *gpoint_lVadc = new TGraph();
        gpoint_lVadc->SetMarkerStyle(33);
        gpoint_lVadc->SetMarkerSize(3);
        gpoint_lVadc->SetMarkerColor(2 + 2 * trk_n);
        gpoint_lVadc->SetPoint(0, trk_adc[trk_n], trk_length[trk_n]);
        gpoint_lVadc->Draw("P same");
      }
      sprintf(buffChar, "eventID%07d_LVEn.png", event_id);
      c1->SaveAs(buffChar);

      digits->Draw("y:x:adc>>yx_digProjection", individ_eventCut, "colz");
      digits->Draw("z:y:adc>>zy_digProjection", individ_eventCut, "colz");
      digits->Draw("z:x:adc>>zx_digProjection", individ_eventCut, "colz");

      TCanvas *digProjection_canvas = new TCanvas(
          "digProjection_canvas", "digProjection_canvas", 600, 1400);
      digProjection_canvas->cd();
      digProjection_canvas->Clear();
      digProjection_canvas->Modified();
      digProjection_canvas->Update();
      digProjection_canvas->Divide(1, 3, 0, 0);
      TText *t = new TText();

      digProjection_canvas->cd(1);
      gPad->SetGrid();
      yx_digProjection->Draw("colz");
      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        if (trk_length[trk_n] == 0 || trk_adc[trk_n] == 0)
          continue;
        TLine *line = new TLine(start_x[trk_n], start_y[trk_n], end_x[trk_n],
                                end_y[trk_n]);
        line->SetLineColor(2 + 2 * trk_n);
        line->SetLineWidth(6);
        line->Draw("same");
      }
      t->SetTextAngle(0);
      t->DrawTextNDC(0.25, 0.03, "X [cm]");
      t->SetTextAngle(90);
      t->DrawTextNDC(0.15, 0.2, "Y [cm]");
      digProjection_canvas->cd(2);
      gPad->SetGrid();
      zy_digProjection->Draw("colz");
      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        if (trk_length[trk_n] == 0 || trk_adc[trk_n] == 0)
          continue;
        TLine *line = new TLine(start_y[trk_n], start_z[trk_n], end_y[trk_n],
                                end_z[trk_n]);
        line->SetLineColor(2 + 2 * trk_n);
        line->SetLineWidth(6);
        line->Draw("same");
      }
      t->SetTextAngle(0);
      t->DrawTextNDC(0.25, 0.03, "Y [cm]");
      t->SetTextAngle(90);
      t->DrawTextNDC(0.15, 0.2, "Z [cm]");
      digProjection_canvas->cd(3);
      gPad->SetGrid();
      zx_digProjection->Draw("colz");
      for (int trk_n = 0; trk_n < trk_count; trk_n++) {
        if (trk_length[trk_n] == 0 || trk_adc[trk_n] == 0)
          continue;
        TLine *line = new TLine(start_x[trk_n], start_z[trk_n], end_x[trk_n],
                                end_z[trk_n]);
        line->SetLineColor(2 + 2 * trk_n);
        line->SetLineWidth(6);
        line->Draw("same");
      }
      t->SetTextAngle(0);
      t->DrawTextNDC(0.25, 0.13, "X [cm]");
      t->SetTextAngle(90);
      t->DrawTextNDC(0.15, 0.25, "Z [cm]");
      sprintf(buffChar, "2d_projections %s", individ_eventCut);
      digProjection_canvas->SetTitle(buffChar);
      digProjection_canvas->Modified();
      digProjection_canvas->Update();
      sprintf(buffChar, "eventID%07d_2d_digProjs.png", event_id);
      digProjection_canvas->SaveAs(buffChar);
      sprintf(buffChar, "gm montage -mode concatenate -tile 2x1 "
                        "eventID%07d.png eventID%07d_2d_digProjs.png "
                        "eventID%07d_digitTrack.png",
              event_id, event_id, event_id);
      system(buffChar);
      sprintf(buffChar, "rm eventID%07d_2d_digProjs.png eventID%07d.png",
              event_id, event_id);
      system(buffChar);
      sprintf(buffChar, "gm montage -mode concatenate -tile 1x2 "
                        "eventID%07d_EnVsCosTheta.png eventID%07d_LVEn.png "
                        "EnVsCosTheta_LVEn_tmp.png",
              event_id, event_id);
      system(buffChar);
      sprintf(buffChar, "gm montage -mode concatenate -tile 2x1 "
                        "EnVsCosTheta_LVEn_tmp.png eventID%07d_digitTrack.png "
                        "run%d_eventID%07d_%s_trackCount%d.png",
              event_id, run, event_id, pidCut, trk_count);
      system(buffChar);
      sprintf(buffChar, "rm eventID%07d_EnVsCosTheta.png "
                        "EnVsCosTheta_LVEn_tmp.png eventID%07d_LVEn.png "
                        "eventID%07d_digitTrack.png",
              event_id, event_id, event_id);
      system(buffChar);
    }
  }
}
