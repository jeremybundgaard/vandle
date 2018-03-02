// ROOT Libraries
#include "TF1.h"
#include "TH2.h"
#include "TH1.h"
#include "TH2Poly.h"
#include "AnaHistoList.h"
#include "AnaH1D.h"
#include "AnaH2D.h"
#include "AnaH2PadPlane.h"
#include "AnaH1DR.h"
#include "AnaH2DR.h"
#include "TFile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFitResult.h"
#include "TAttAxis.h"
#include "TCutG.h"
#include "TGaxis.h"

// NIFFTE Libraries
#include "NiffteGeometryMap.h"
#include "NiffteTPCADCSignal.h"
#include "NiffteRootIO.h"
#include "NiffteTPCDetectorConstruction.h"

// c++ Libraries
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <string>
#include <unistd.h>

void plotMaker(const char* infile){

	TFile *inputAnaFile = new TFile(infile);
	TCanvas *c1 = new TCanvas("c1","c1",1000,700);
	// TImage *img = TImage::Create();
	char plotName[128];
	// int sleepTime=500000;
	TGaxis::SetMaxDigits(3); 
	// gStyle->SetOptStat(0);	

	if(	inputAnaFile->GetListOfKeys()->Contains("hAzimuthvol0") ){
		AnaH1D * hAzimuthvol0 = (AnaH1D*)inputAnaFile->Get("hAzimuthvol0");
		hAzimuthvol0->GetXaxis()->SetTitle("Degree");
		hAzimuthvol0->GetYaxis()->SetTitle("Events");
		hAzimuthvol0->GetXaxis()->CenterTitle();
		hAzimuthvol0->GetYaxis()->CenterTitle();
		hAzimuthvol0->Draw();
		sprintf(plotName,"azimuth.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}

	if(	inputAnaFile->GetListOfKeys()->Contains("hAzimuthvol1") ){
		AnaH1D * hAzimuthvol1 = (AnaH1D*)inputAnaFile->Get("hAzimuthvol1");
		hAzimuthvol1->GetXaxis()->SetTitle("Degree");
		hAzimuthvol1->GetYaxis()->SetTitle("Events");
		hAzimuthvol1->GetXaxis()->CenterTitle();
		hAzimuthvol1->GetYaxis()->CenterTitle();
		hAzimuthvol1->Draw();
		sprintf(plotName,"azimuth.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}


	if(	inputAnaFile->GetListOfKeys()->Contains("hTotalADCvol0") ){
		AnaH1D * hTotalADCvol0 = (AnaH1D*)inputAnaFile->Get("hTotalADCvol0");
		hTotalADCvol0->GetXaxis()->SetTitle("ADC");
		hTotalADCvol0->GetXaxis()->SetRange(0,50000);
		hTotalADCvol0->GetYaxis()->SetTitle("Events");
		hTotalADCvol0->GetXaxis()->CenterTitle();
		hTotalADCvol0->GetYaxis()->CenterTitle();
		hTotalADCvol0->GetXaxis()->SetNoExponent(kTRUE);
		hTotalADCvol0->Draw();
		sprintf(plotName,"hTotalADCvol0.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

		hTotalADCvol0->GetXaxis()->SetRange(0,3000);
		hTotalADCvol0->SetTitle("total ADC [alphas]");
		hTotalADCvol0->GetXaxis()->CenterTitle();
		hTotalADCvol0->GetYaxis()->CenterTitle();
		hTotalADCvol0->Draw();
		c1->SetLogy();
		sprintf(plotName,"alphaTotalADC.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;		

	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("hTotalADCvol1") ){
		AnaH1D * hTotalADCvol1 = (AnaH1D*)inputAnaFile->Get("hTotalADCvol1");
		hTotalADCvol1->GetXaxis()->SetTitle("ADC");
		hTotalADCvol1->GetXaxis()->SetRange(0,50000);
		hTotalADCvol1->GetYaxis()->SetTitle("Events");
		hTotalADCvol1->GetXaxis()->CenterTitle();
		hTotalADCvol1->GetYaxis()->CenterTitle();
		hTotalADCvol1->GetXaxis()->SetNoExponent(kTRUE);
		hTotalADCvol1->Draw();
		sprintf(plotName,"hTotalADCvol1.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

		hTotalADCvol1->GetXaxis()->SetRange(0,3000);
		hTotalADCvol1->SetTitle("total ADC [alphas]");
		hTotalADCvol1->GetXaxis()->CenterTitle();
		hTotalADCvol1->GetYaxis()->CenterTitle();
		hTotalADCvol1->Draw();
		c1->SetLogy();
		sprintf(plotName,"alphaTotalADC.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;		

	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("hNumTrkvol0") ){
		AnaH1D * hNumTrkvol0 = (AnaH1D*)inputAnaFile->Get("hNumTrkvol0");
		hNumTrkvol0->GetXaxis()->SetTitle("Number Track in Event ");
		hNumTrkvol0->GetYaxis()->SetTitle("Events");
		hNumTrkvol0->GetXaxis()->CenterTitle();
		hNumTrkvol0->GetYaxis()->CenterTitle();
		hNumTrkvol0->Draw();
		sprintf(plotName,"hNumTrkvol0.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("hNumTrkvol1") ){
		AnaH1D * hNumTrkvol1 = (AnaH1D*)inputAnaFile->Get("hNumTrkvol1");
		hNumTrkvol1->GetXaxis()->SetTitle("Number Track in Event ");
		hNumTrkvol1->GetYaxis()->SetTitle("Events");
		hNumTrkvol1->GetXaxis()->CenterTitle();
		hNumTrkvol1->GetYaxis()->CenterTitle();
		hNumTrkvol1->Draw();
		sprintf(plotName,"hNumTrkvol1.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}	

	c1->SetLogz(1);
	c1->SetLogy(0);
	
	if(	inputAnaFile->GetListOfKeys()->Contains("LengthvsADCvol0") ){
		AnaH2D * LengthvsADCvol0 = (AnaH2D*)inputAnaFile->Get("LengthvsADCvol0");
		LengthvsADCvol0->GetXaxis()->SetTitle("ADC");
		LengthvsADCvol0->GetYaxis()->SetTitle("cm");
		LengthvsADCvol0->GetXaxis()->CenterTitle();
		LengthvsADCvol0->GetYaxis()->CenterTitle();
		LengthvsADCvol0->GetXaxis()->SetNoExponent(kTRUE);
		LengthvsADCvol0->Draw("Colz");
		sprintf(plotName,"LengthvsADCvol0.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

		LengthvsADCvol0->GetXaxis()->SetRange(0,3500);
		LengthvsADCvol0->SetTitle("length vs ADC [alphas]");
		LengthvsADCvol0->Draw("Colz");
		c1->Update();
		sprintf(plotName,"alphaLengthvsADC.%s.png",infile);
		c1->SaveAs(plotName);
	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("LengthvsADCvol1") ){
		AnaH2D * LengthvsADCvol1 = (AnaH2D*)inputAnaFile->Get("LengthvsADCvol1");
		LengthvsADCvol1->GetXaxis()->SetTitle("ADC");
		LengthvsADCvol1->GetYaxis()->SetTitle("cm");
		LengthvsADCvol1->GetXaxis()->CenterTitle();
		LengthvsADCvol1->GetYaxis()->CenterTitle();
		LengthvsADCvol1->GetXaxis()->SetNoExponent(kTRUE);
		LengthvsADCvol1->Draw("Colz");
		sprintf(plotName,"LengthvsADCvol1.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

		LengthvsADCvol1->GetXaxis()->SetRange(0,3500);
		LengthvsADCvol1->SetTitle("length vs ADC [alphas]");
		LengthvsADCvol1->Draw("Colz");
		c1->Update();
		sprintf(plotName,"alphaLengthvsADC.%s.png",infile);
		c1->SaveAs(plotName);
	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("hStartXYvol0") ){
		AnaH2D * hStartXYvol0 = (AnaH2D*)inputAnaFile->Get("hStartXYvol0");
		hStartXYvol0->GetXaxis()->SetTitle("cm");
		hStartXYvol0->GetYaxis()->SetTitle("cm");
		hStartXYvol0->GetXaxis()->CenterTitle();
		hStartXYvol0->GetYaxis()->CenterTitle();
		hStartXYvol0->GetXaxis()->SetRange(150,850);
		hStartXYvol0->GetYaxis()->SetRange(150,850);
		hStartXYvol0->Draw("Colz");
		sprintf(plotName,"startXY.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}

	if(	inputAnaFile->GetListOfKeys()->Contains("hStartXYvol1") ){
		AnaH2D * hStartXYvol1 = (AnaH2D*)inputAnaFile->Get("hStartXYvol1");
		hStartXYvol1->GetXaxis()->SetTitle("cm");
		hStartXYvol1->GetYaxis()->SetTitle("cm");
		hStartXYvol1->GetXaxis()->CenterTitle();
		hStartXYvol1->GetYaxis()->CenterTitle();
		hStartXYvol1->GetXaxis()->SetRange(150,850);
		hStartXYvol1->GetYaxis()->SetRange(150,850);
		hStartXYvol1->Draw("Colz");
		sprintf(plotName,"startXY.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}

	if(	inputAnaFile->GetListOfKeys()->Contains("hdigADCvol0") ){
		TH2Poly * hdigADCvol0 = (TH2Poly*)inputAnaFile->Get("hdigADCvol0");
		hdigADCvol0->GetXaxis()->SetTitle("cm");
		hdigADCvol0->GetYaxis()->SetTitle("cm");
		hdigADCvol0->GetXaxis()->CenterTitle();
		hdigADCvol0->GetYaxis()->CenterTitle();
		hdigADCvol0->Draw("update");hdigADCvol0->Draw("Colz");
		sprintf(plotName,"digADCvol0.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

	}
	
	if(	inputAnaFile->GetListOfKeys()->Contains("hdigADCvol1") ){
		TH2Poly * hdigADCvol1 = (TH2Poly*)inputAnaFile->Get("hdigADCvol1");
		hdigADCvol1->GetXaxis()->SetTitle("cm");
		hdigADCvol1->GetYaxis()->SetTitle("cm");
		hdigADCvol1->GetXaxis()->CenterTitle();
		hdigADCvol1->GetYaxis()->CenterTitle();
		hdigADCvol1->Draw("update");hdigADCvol1->Draw("Colz");
		sprintf(plotName,"digADCvol1.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

	}

	if(	inputAnaFile->GetListOfKeys()->Contains("hStartZ") ){
		AnaH1D * hStartZ = (AnaH1D*)inputAnaFile->Get("hStartZ");
		hStartZ->GetXaxis()->SetTitle("cm");
		hStartZ->GetYaxis()->SetTitle("Events");
		hStartZ->GetXaxis()->CenterTitle();
		hStartZ->GetYaxis()->CenterTitle();
		hStartZ->Draw();
		sprintf(plotName,"hStartZ.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}	

	c1->SetLogy();

	if(	inputAnaFile->GetListOfKeys()->Contains("hPolar") ){
		AnaH1D * hPolar = (AnaH1D*)inputAnaFile->Get("hPolar");
		hPolar->GetXaxis()->SetTitle("cos(theta)");
		hPolar->GetYaxis()->SetTitle("Events");
		hPolar->GetXaxis()->CenterTitle();
		hPolar->GetYaxis()->CenterTitle();
		hPolar->Draw();
		sprintf(plotName,"hPolar.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("hADCvsPolar") ){
		AnaH2D * hADCvsPolar = (AnaH2D*)inputAnaFile->Get("hADCvsPolar");
		hADCvsPolar->GetXaxis()->SetTitle("cos(theta)");
		hADCvsPolar->GetYaxis()->SetTitle("ADC");
		hADCvsPolar->GetXaxis()->CenterTitle();
		hADCvsPolar->GetYaxis()->CenterTitle();
		hADCvsPolar->Draw("Colz");
		sprintf(plotName,"ADCvsPolar.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;

		hADCvsPolar->GetYaxis()->SetRange(0,4000);
		hADCvsPolar->SetTitle("ADC vs cos(polar) [alphas]");
		hADCvsPolar->Draw("Colz");
		sprintf(plotName,"alphaADCvsPolar.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}

	if(	inputAnaFile->GetListOfKeys()->Contains("hTrkLvsPolar") ){
		AnaH2D * hTrkLvsPolar = (AnaH2D*)inputAnaFile->Get("hTrkLvsPolar");
		hTrkLvsPolar->GetXaxis()->SetTitle("cos(theta)");
		hTrkLvsPolar->GetYaxis()->SetTitle("cm");
		hTrkLvsPolar->GetXaxis()->CenterTitle();
		hTrkLvsPolar->GetYaxis()->CenterTitle();
		hTrkLvsPolar->Draw("update");hTrkLvsPolar->Draw("Colz");
		sprintf(plotName,"trkLvsPolar.%s.png",infile);
		c1->SaveAs(plotName);
		// std::cout<< plotName <<std::endl;
	}

	c1->Close();

}

	