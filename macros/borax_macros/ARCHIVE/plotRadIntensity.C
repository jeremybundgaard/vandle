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
#include "TAttMarker.h"
#include "TImage.h"
#include "TGraph.h"
#include "TCutG.h"


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

void plotRadIntensity(){

	TFile *inputAnaFile = new TFile("mapXYtoRadius.root.hist","update");
	TCanvas *c1 = new TCanvas("c1","c1",1000,700);

	if(	inputAnaFile->GetListOfKeys()->Contains("radVsIntensity") ){
		AnaH1D * radVsIntensity = (AnaH1D*)inputAnaFile->Get("radVsIntensity");
		radVsIntensity->GetXaxis()->SetTitle("radius [cm]");
		radVsIntensity->SetTitle("radius vs average intensity");
		radVsIntensity->Draw();
		c1->SaveAs("radVsIntensity.png");

		c1->SetLogy(1);
		radVsIntensity->Draw();
		c1->Update();
		c1->SaveAs("radVsIntensity_LOG.png");

		int radbins=radVsIntensity->GetXaxis()->GetNbins();
		double intenseAdj=0,radius=0,intensity=0;

		AnaH1D *adjustradVintensity = new AnaH1D("adjustradVintensity","adjustradVintensity",radbins,0,5);

		for (int i=0; i<radbins;++i)
		{

			radius=radVsIntensity->GetBinContent(i);
			intensity=radVsIntensity->GetBinCenter(i);
			if(intensity!=0)
			{
				intenseAdj=2*TMath::Pi()*radius*intensity;
				adjustradVintensity->SetBinContent(i,intenseAdj);
			}
		}


		adjustradVintensity->Write();
		c1->SetLogy(0);
		adjustradVintensity->GetXaxis()->SetTitle("radius [cm]");
		adjustradVintensity->SetTitle("radius vs average intensity[*2pi*r]");
		adjustradVintensity->Draw();
		c1->SaveAs("adjustradVintensity.png");

		c1->SetLogy(1);
		adjustradVintensity->Draw();
		c1->Update();
		c1->SaveAs("adjustradVintensity_LOG.png");

	}


	

	if(	inputAnaFile->GetListOfKeys()->Contains("averageIntensityVsRadius") ){
		AnaH1D * averageIntensityVsRadius = (AnaH1D*)inputAnaFile->Get("averageIntensityVsRadius");
		averageIntensityVsRadius->Draw();
		c1->SaveAs("averageIntensityVsRadius.png");

		c1->SetLogy(1);
		averageIntensityVsRadius->Draw();
		c1->Update();
		c1->SaveAs("averageIntensityVsRadius_LOG.png");
	}

	if(	inputAnaFile->GetListOfKeys()->Contains("radiusManyBins") ){
		AnaH1D * radiusManyBins = (AnaH1D*)inputAnaFile->Get("radiusManyBins");
		c1->SetLogy(0);
		radiusManyBins->Draw();
		c1->SaveAs("radiusManyBins.png");
	}
	
	c1->Close();

}

	