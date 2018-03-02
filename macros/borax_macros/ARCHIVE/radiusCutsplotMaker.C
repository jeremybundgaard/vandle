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
#include "TLine.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFitResult.h"
#include "TAttMarker.h"
#include "TImage.h"
#include "TGraph.h"
#include "TCutG.h"
#include "TLatex.h"


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
#include <sstream>
#include <string>
#include <unistd.h>

void radiusCutsplotMaker(const char* infile){

	TFile *inputAnaFile = new TFile(infile);
	TCanvas *c1 = new TCanvas("c1","c1",1000,700);
	// TImage *img = TImage::Create();
	char plotName[128];
	stringstream ss;
	// int sleepTime=500000;

	c1->SetLogz(1);
	if(	inputAnaFile->GetListOfKeys()->Contains("hdigADCvol0") ){
		TH2Poly * hdigADCvol0 = (TH2Poly*)inputAnaFile->Get("hdigADCvol0");
		hdigADCvol0->SetTitle("accum ADC [~ 2 pad annuli]");
		hdigADCvol0->Draw("update");hdigADCvol0->Draw("Colz");
		sprintf(plotName,"digADCvol0.%s.png",infile);
		c1->SaveAs(plotName);
		cout<< plotName <<endl;

	}
	
	if(	inputAnaFile->GetListOfKeys()->Contains("hdigADCvol1") ){
		TH2Poly * hdigADCvol1 = (TH2Poly*)inputAnaFile->Get("hdigADCvol1");
		hdigADCvol1->Draw("update");hdigADCvol1->Draw("Colz");
		sprintf(plotName,"digADCvol1.%s.png",infile);
		c1->SaveAs(plotName);
		cout<< plotName <<endl;

	}
	c1->SetLogz(0);
	c1->SetLogy(0);

	if(	inputAnaFile->GetListOfKeys()->Contains("intensityvol0") ){
		AnaH1D * intensityvol0 = (AnaH1D*)inputAnaFile->Get("intensityvol0");
		intensityvol0->Fit("gaus");
		gStyle->SetOptFit(0001);
		gStyle->SetOptStat("ne");
		intensityvol0->SetTitle("totalADC/total#ofDigit vs radius");
		intensityvol0->GetXaxis()->SetTitle("ADC/nDigits");	
		intensityvol0->SetAxisRange(1, 75, "y");
		intensityvol0->GetXaxis()->SetRange(0,100);
		intensityvol0->Draw();

		double fwhm = 2.35*intensityvol0->GetFunction("gaus")->GetParameter(2);
		char fwhmBuf[128];
		sprintf(fwhmBuf,"FWHM = %.4f ", fwhm);
		TLatex t(.48,.31,fwhmBuf);t.SetNDC(kTRUE); t.Draw();


		double resolution = fwhm/(float)intensityvol0->GetFunction("gaus")->GetParameter(1)*100;
		char resolutionBuf[128];
		sprintf(resolutionBuf,"resolution = %.1f %%",resolution);
		TLatex t2(.48,.21,resolutionBuf);t2.SetNDC(kTRUE); t2.Draw();

		sprintf(plotName,"intensityvol0.%s.png",infile);
		c1->SaveAs(plotName);
		cout<< plotName <<endl;

	}	

	if(	inputAnaFile->GetListOfKeys()->Contains("intensityvol1") ){
		AnaH1D * intensityvol1 = (AnaH1D*)inputAnaFile->Get("intensityvol1");
		intensityvol1->Fit("gaus");
		gStyle->SetOptFit(0001);
		gStyle->SetOptStat("ne");
		intensityvol1->SetTitle("totalADC/total#ofDigit vs radius");
		intensityvol1->GetXaxis()->SetTitle("ADC/nDigits");	
		intensityvol1->GetXaxis()->SetRange(0,100);
		intensityvol1->SetAxisRange(1,1.2*intensityvol1->GetMaximum(), "y");
		// intensityvol1->SetAxisRange(1,80, "y");
		intensityvol1->Draw();

		double fwhm = 2.35*intensityvol1->GetFunction("gaus")->GetParameter(2);
		char fwhmBuf[128];
		sprintf(fwhmBuf,"FWHM = %.4f ", fwhm);
		TLatex t(.48,.31,fwhmBuf);t.SetNDC(kTRUE); t.Draw();

		double resolution = fwhm/(float)intensityvol1->GetFunction("gaus")->GetParameter(1)*100;
		char resolutionBuf[128];
		sprintf(resolutionBuf,"resolution = %.1f %%",resolution);
		TLatex t2(.48,.21,resolutionBuf);t2.SetNDC(kTRUE); t2.Draw();
 
 		sprintf(plotName,"intensityvol1.%s.png",infile);
		c1->SaveAs(plotName);
		cout<< plotName <<endl;

	}	
	

	c1->Close();

}
