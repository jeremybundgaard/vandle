//
//
#include <climits>
#include <vector>
#include "TROOT.h"
#include "TRint.h"
#include "MsgLog.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCut.h"
#include "TCutG.h"
#include "TGaxis.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TChain.h"
#include "TTree.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TMath.h"
#include "NiffteGeometryMap.h"
#include "NifftePlotUtilities.h"
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

void DrawInfoPrec(double number = 1.0,const char* word = "word",Float_t start_x=0.125, Float_t start_y=0.5, Float_t text_height=0.13){
  std::stringstream sstr;
  sstr << word <<" = "<< fixed << setprecision(3) << number;
  std::string str1 = sstr.str();
  const char* entries = str1.c_str();
  Float_t niffte_start_y = start_y - text_height * 1.1;
  TLatex *niffte = new TLatex(start_x,niffte_start_y,entries);
  niffte->SetNDC();
  niffte->SetTextColor(1);
  //niffte->SetTextFont(42);
  niffte->SetTextFont(60);
  niffte->SetTextSize(text_height);
  niffte->SetLineWidth(2);
  niffte->Draw();
}

void ReadBT(const char* inroot = "inroot.root",const char* outroot = "outroot.root",const char* outdir = "plots/") {

  //UseNifftePlotStyle();
  gROOT->SetStyle("Plain");
  gStyle->SetNumberContours(999);
  //gStyle->SetTitleX(0.225f);
  gStyle->SetTitleX(0.2f);
  gStyle->SetTitleW(0.55f);
  gStyle->SetTitleY(0.95f);
  gStyle->SetTitleBorderSize(0);


  TFile* f = new TFile(inroot);
  TTree* tracks = (TTree*)f->Get("tracks");

  stringstream* outRootFile;
  outRootFile = new stringstream;
  *outRootFile << outdir << outroot;
  TFile* fout = new TFile((char*)(outRootFile->str().c_str()),"RECREATE");
  fout->cd();
  delete outRootFile;


  //---------------------------------------------
  TCut alpha_cut = "adc > 50 && adc < 2500";
  TCut frag_cut = "adc > 2500";
  //TCut theta_Cut = "cosTheta > 0";
  //----------------------------------------------
  gROOT->SetStyle("Plain");
  TCanvas *c1 = new TCanvas("cazi","cazi",900,624);
  TGaxis *axis = new TGaxis();
  axis->SetMaxDigits(4);
  axis->Draw();
  gPad->SetRightMargin(0.13);

  stringstream* outputFileName;

  TH1D* hADC = new TH1D("hADC","",8000,0,80000);
  tracks->Draw("adc>>hADC","vol == 0");
  hADC->SetStats(0);
  hADC->SetTitle("Track ADC");
  hADC->GetYaxis()->SetTitle("Counts/10 ADC");
  hADC->GetYaxis()->SetTitleOffset(1.15);
  hADC->GetXaxis()->SetRangeUser(2500,80000);
  hADC->GetXaxis()->SetTitle("ADC");
  hADC->GetXaxis()->SetLabelOffset(0.015);
  hADC->GetXaxis()->SetTitleOffset(1.15);
  hADC->Draw();
  hADC->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "ADC" << ".png";
  c1->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  TF1 *PolFit = new TF1("PolFit","pol1",0.2,0.9);
  Double_t parPolFit[2];
  double slopePolFit = 0;

  TF1 *PolFitV0 = new TF1("PolFitV0","pol1",-0.9,-0.2);
  Double_t parPolFitV0[2];
  double slopePolFitV0 = 0;

  TH1D* hCosTheta = new TH1D("hCosTheta","",100,-1,1);
  tracks->Draw("cosTheta>>hCosTheta",alpha_cut);
  hCosTheta->SetStats(0);
  hCosTheta->SetTitle("Alphas, PCA");
  hCosTheta->GetYaxis()->SetTitle("Counts/bin");
  hCosTheta->GetYaxis()->SetTitleOffset(1.15);
  //hCosTheta->GetXaxis()->SetRangeUser(200,2000);
  hCosTheta->GetXaxis()->SetTitle("cos(theta)");
  hCosTheta->GetXaxis()->SetLabelOffset(0.015);
  hCosTheta->GetXaxis()->SetTitleOffset(1.15);
  hCosTheta->Fit(PolFit,"R");
  PolFit->GetParameters(&parPolFit[0]);
  slopePolFit = parPolFit[1]/parPolFit[0];
  hCosTheta->Fit(PolFitV0,"R+");
  PolFitV0->GetParameters(&parPolFitV0[0]);
  slopePolFitV0 = parPolFitV0[1]/parPolFitV0[0];
  hCosTheta->Draw("e");
  DrawInfoPrec(slopePolFit,"Slope",0.7,0.9,0.04);
  DrawInfoPrec(slopePolFitV0,"Slope",0.15,0.9,0.04);
  hCosTheta->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "CosTheta" << ".png";
  c1->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  TH1D* hFCosTheta = new TH1D("hFCosTheta","",100,-1,1);
  tracks->Draw("cosTheta>>hFCosTheta",frag_cut);
  hFCosTheta->SetStats(0);
  hFCosTheta->SetTitle("Fragments, PCA");
  hFCosTheta->GetYaxis()->SetTitle("Counts/bin");
  hFCosTheta->GetYaxis()->SetTitleOffset(1.15);
  //hFCosTheta->GetYaxis()->SetRangeUser(0,330);
  hFCosTheta->GetXaxis()->SetTitle("cos(theta)");
  hFCosTheta->GetXaxis()->SetLabelOffset(0.015);
  hFCosTheta->GetXaxis()->SetTitleOffset(1.15);
  //hFCosTheta->GetXaxis()->SetRangeUser(200,2000);
  hFCosTheta->Fit(PolFit,"R");
  PolFit->GetParameters(&parPolFit[0]);
  slopePolFit = parPolFit[1]/parPolFit[0];
  hFCosTheta->Fit(PolFitV0,"R+");
  PolFitV0->GetParameters(&parPolFitV0[0]);
  slopePolFitV0 = parPolFitV0[1]/parPolFitV0[0];
  hFCosTheta->Draw("e");
  DrawInfoPrec(slopePolFit,"Slope",0.7,0.9,0.04);
  DrawInfoPrec(slopePolFitV0,"Slope",0.15,0.9,0.04);
  hFCosTheta->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "FCosTheta" << ".png";
  c1->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  /*
  TH2D* hLvsADCV0 = new TH2D("hLvsADCV0","",8000,0,80000,100,0,10);
  gPad->SetLogz();
  tracks->Draw("length:adc>>hLvsADCV0","vol == 0");
  hLvsADCV0->SetStats(0);
  hLvsADCV0->SetTitle("Vol. 0 Track Length vs. ADC");
  hLvsADCV0->GetYaxis()->SetRangeUser(0,10);
  hLvsADCV0->GetYaxis()->SetTitle("Track Length (cm)");
  //hLvsADCV0->GetXaxis()->SetRangeUser(0,3000);
  hLvsADCV0->GetXaxis()->SetTitle("ADC");
  hLvsADCV0->GetXaxis()->SetLabelOffset(0.015);
  hLvsADCV0->Draw("COLZ");
  c1->Update();
  TPaletteAxis *LvsADCV0pal = (TPaletteAxis*)hLvsADCV0->GetListOfFunctions()->FindObject("palette");
  LvsADCV0pal->GetAxis()->SetTitle("Counts/10 ADC/mm");
  LvsADCV0pal->GetAxis()->CenterTitle();  
  hLvsADCV0->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "LvsADCV0" << ".png";
  c1->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;
  */

  TH2D* hLvsADCV1 = new TH2D("hLvsADCV1","",8000,0,80000,100,0,10);
  gPad->SetLogz();
  tracks->Draw("length:adc>>hLvsADCV1","vol == 1");
  hLvsADCV1->SetStats(0);
  hLvsADCV1->SetTitle("Vol. 1 Track Length vs. ADC");
  hLvsADCV1->GetYaxis()->SetRangeUser(0,10);
  hLvsADCV1->GetYaxis()->SetTitle("Track Length (cm)");
  hLvsADCV1->GetXaxis()->SetRangeUser(0,3000);
  hLvsADCV1->GetXaxis()->SetTitle("ADC");
  hLvsADCV1->GetXaxis()->SetLabelOffset(0.015);
  hLvsADCV1->Draw("COLZ");
  c1->Update();
  TPaletteAxis *LvsADCV1pal = (TPaletteAxis*)hLvsADCV1->GetListOfFunctions()->FindObject("palette");
  LvsADCV1pal->GetAxis()->SetTitle("Counts/10 ADC/mm");
  LvsADCV1pal->GetAxis()->CenterTitle();  
  hLvsADCV1->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "LvsADCV1" << ".png";
  c1->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  TCutG *cutFragPeak = new TCutG("cutFragPeak",2);
  cutFragPeak->SetPoint(0,-1,27500);
  cutFragPeak->SetPoint(1,0,25500);

  TH2D* hADCvsPolarV1 = new TH2D("hADCvsPolarV1","",100,-1,1,8000,0,80000);
  gPad->SetLogz();
  tracks->Draw("adc:cosTheta>>hADCvsPolarV1","vol == 1");
  hADCvsPolarV1->SetStats(0);
  hADCvsPolarV1->SetTitle("Vol. 0 ADC vs. cos(theta)");
  hADCvsPolarV1->GetYaxis()->SetRangeUser(0,3000);
  hADCvsPolarV1->GetYaxis()->SetTitle("ADC");
  hADCvsPolarV1->GetXaxis()->SetTitle("cos(theta)");
  hADCvsPolarV1->GetXaxis()->SetLabelOffset(0.015);
  hADCvsPolarV1->Draw("COLZ");
  cutFragPeak->Draw("same");
  c1->Update();
  TPaletteAxis *ADCvsPolarV1pal = (TPaletteAxis*)hADCvsPolarV1->GetListOfFunctions()->FindObject("palette");
  ADCvsPolarV1pal->GetAxis()->SetTitle("Counts/10 ADC/mm");
  ADCvsPolarV1pal->GetAxis()->CenterTitle();  
  hADCvsPolarV1->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "ADCvsPolarV1" << ".png";
  c1->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  //---------------------------------------------------------------
  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  c2->cd();
  c2->Update();

  TH2D* hStartXYV0 = new TH2D("hStartXYV0","",1000,-10,10,1000,-10,10);
  gPad->SetLogz();
  gPad->SetRightMargin(0.15);
  tracks->Draw("yVertex:xVertex>>hStartXYV0","vol == 0");
  hStartXYV0->SetStats(0);
  hStartXYV0->SetTitle("Vol. 0 Track Vertex");
  hStartXYV0->GetYaxis()->SetRangeUser(-3,3);
  hStartXYV0->GetYaxis()->SetTitle("Y (cm)");
  hStartXYV0->GetYaxis()->SetTitleOffset(1.);
  hStartXYV0->GetXaxis()->SetRangeUser(-3,3);
  hStartXYV0->GetXaxis()->SetTitle("X (cm)");
  hStartXYV0->GetXaxis()->SetLabelOffset(0.015);
  hStartXYV0->GetXaxis()->SetTitleOffset(1.15);
  hStartXYV0->Draw("COLZ");
  c2->Update();
  TPaletteAxis *StartXYV0pal = (TPaletteAxis*)hStartXYV0->GetListOfFunctions()->FindObject("palette");
  StartXYV0pal->GetAxis()->SetTitle("Counts/0.2 x 0.2 mm");
  //StartXYV0pal->GetAxis()->CenterTitle();
  StartXYV0pal->GetAxis()->SetTitleOffset(1.25);
  hStartXYV0->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "StartXYV0" << ".png";
  c2->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  TH2D* hStartXYV1 = new TH2D("hStartXYV1","",1000,-10,10,1000,-10,10);
  gPad->SetLogz();
  gPad->SetRightMargin(0.15);
  tracks->Draw("yVertex:xVertex>>hStartXYV1","vol == 1");
  hStartXYV1->SetStats(0);
  hStartXYV1->SetTitle("Vol. 1 Track Vertex");
  hStartXYV1->GetYaxis()->SetRangeUser(-3,3);
  hStartXYV1->GetYaxis()->SetTitle("Y (cm)");
  hStartXYV1->GetYaxis()->SetTitleOffset(1.);
  hStartXYV1->GetXaxis()->SetRangeUser(-3,3);
  hStartXYV1->GetXaxis()->SetTitle("X (cm)");
  hStartXYV1->GetXaxis()->SetLabelOffset(0.015);
  hStartXYV1->GetXaxis()->SetTitleOffset(1.15);
  hStartXYV1->Draw("COLZ");
  c2->Update();
  TPaletteAxis *StartXYV1pal = (TPaletteAxis*)hStartXYV1->GetListOfFunctions()->FindObject("palette");
  StartXYV1pal->GetAxis()->SetTitle("Counts/0.2 x 0.2 mm");
  StartXYV1pal->GetAxis()->SetTitleOffset(1.25);
  hStartXYV1->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "StartXYV1" << ".png";
  c2->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  /*
  TH2D* hFStartXYV0 = new TH2D("hFStartXYV0","",1000,-10,10,1000,-10,10);
  gPad->SetLogz();
  gPad->SetRightMargin(0.15);
  tracks->Draw("yVertex:xVertex>>hFStartXYV0","vol == 0 && adc > 2500");
  hFStartXYV0->SetStats(0);
  hFStartXYV0->SetTitle("Vol. 0 Track Vertex");
  hFStartXYV0->GetYaxis()->SetRangeUser(-3,3);
  hFStartXYV0->GetYaxis()->SetTitle("Y (cm)");
  hFStartXYV0->GetYaxis()->SetTitleOffset(1.);
  hFStartXYV0->GetXaxis()->SetRangeUser(-3,3);
  hFStartXYV0->GetXaxis()->SetTitle("X (cm)");
  hFStartXYV0->GetXaxis()->SetLabelOffset(0.015);
  hFStartXYV0->GetXaxis()->SetTitleOffset(1.15);
  hFStartXYV0->Draw("COLZ");
  c2->Update();
  TPaletteAxis *FStartXYV0pal = (TPaletteAxis*)hFStartXYV0->GetListOfFunctions()->FindObject("palette");
  FStartXYV0pal->GetAxis()->SetTitle("Counts/0.2 x 0.2 mm");
  //FStartXYV0pal->GetAxis()->CenterTitle();
  FStartXYV0pal->GetAxis()->SetTitleOffset(1.25);
  hFStartXYV0->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "FStartXYV0" << ".png";
  c2->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;
  */

  TH2D* hFStartXYV1 = new TH2D("hFStartXYV1","",1000,-10,10,1000,-10,10);
  gPad->SetLogz();
  gPad->SetRightMargin(0.15);
  tracks->Draw("yVertex:xVertex>>hFStartXYV1","vol == 1 && adc > 2500");
  hFStartXYV1->SetStats(0);
  hFStartXYV1->SetTitle("Vol. 1 Track Vertex");
  hFStartXYV1->GetYaxis()->SetRangeUser(-3,3);
  hFStartXYV1->GetYaxis()->SetTitle("Y (cm)");
  hFStartXYV1->GetYaxis()->SetTitleOffset(1.);
  hFStartXYV1->GetXaxis()->SetRangeUser(-3,3);
  hFStartXYV1->GetXaxis()->SetTitle("X (cm)");
  hFStartXYV1->GetXaxis()->SetLabelOffset(0.015);
  hFStartXYV1->GetXaxis()->SetTitleOffset(1.15);
  hFStartXYV1->Draw("COLZ");
  c2->Update();
  TPaletteAxis *FStartXYV1pal = (TPaletteAxis*)hFStartXYV1->GetListOfFunctions()->FindObject("palette");
  FStartXYV1pal->GetAxis()->SetTitle("Counts/0.2 x 0.2 mm");
  FStartXYV1pal->GetAxis()->SetTitleOffset(1.25);
  hFStartXYV1->Write();
  outputFileName = new stringstream;
  *outputFileName << outdir << "FStartXYV1" << ".png";
  c2->Print((char*)(outputFileName->str().c_str()));
  delete outputFileName;

  //--------------------------------------------------
  fout->Close();
  gApplication->Terminate();
}




