#include "TROOT.h"
#include "TRint.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

void p2p_truthDiff(){


TFile *truthFile = new TFile("/p/lscratchd/tpcuser/data/simulation/maps/spiral.root");
TH1D *truthPerPadIndex = (TH1D*)truthFile->Get("Hindex");
TH1D *truthPerPadIndexResidual = (TH1D*)truthPerPadIndex->Clone("truthPerPadIndexResidual");

TFile *p2pfile = new TFile("~/p2p.simSpiral.n50000_23Mar16_0943.55/scalefactors/scalehists.root");
TH1D *scalePerPadIndex1d = (TH1D*)p2pfile->Get("scalePerPadIndex1d");

for (int i = 0; i < 6141; ++i)
{

	if (!(scalePerPadIndex1d->GetBinContent(i))){
		scalePerPadIndex1d->SetBinContent(i,1)
		continue;
	}

	// truthPerPadIndexResidual->SetBinContent(i,1);

	// double residual=truthPerPadIndex->GetBinContent(i) - scalePerPadIndex1d->GetBinContent(i);
	double residual=truthPerPadIndex->GetBinContent(i)/scalePerPadIndex1d->GetBinContent(i);
	// double residual=truthPerPadIndex->GetBinContent(i)*scalePerPadIndex1d->GetBinContent(i);
	// truthPerPadIndexResidual->SetBinContent(i,scalePerPadIndex1d->GetBinContent(i));
	truthPerPadIndexResidual->SetBinContent(i,residual);
	// printf("index %d, residual %3.3f \n",i,residual);
	printf("index %d, scalePerPadIndex1d %3.3f , truthPerPadIndex %3.3f \n",
		i,scalePerPadIndex1d->GetBinContent(i),truthPerPadIndex->GetBinContent(i));
}

truthPerPadIndexResidual->GetXaxis()->SetTitle("pad index");
truthPerPadIndexResidual->GetXaxis()->CenterTitle();
truthPerPadIndexResidual->GetYaxis()->SetTitle("residual");
truthPerPadIndexResidual->GetYaxis()->CenterTitle();
truthPerPadIndexResidual->SetTitle("truthPerPadIndexResidual");
truthPerPadIndexResidual->SetLineColor(kGreen);

scalePerPadIndex1d->GetXaxis()->SetTitle("pad index");
scalePerPadIndex1d->GetXaxis()->CenterTitle();
scalePerPadIndex1d->GetYaxis()->SetTitle("scalefactors");
scalePerPadIndex1d->GetYaxis()->CenterTitle();
scalePerPadIndex1d->SetTitle("scalePerPadIndex1d");
scalePerPadIndex1d->SetLineColor(kRed);

truthPerPadIndex->GetXaxis()->SetTitle("pad index");
truthPerPadIndex->GetXaxis()->CenterTitle();
truthPerPadIndex->GetYaxis()->SetTitle("truth gain");
truthPerPadIndex->GetYaxis()->CenterTitle();
truthPerPadIndex->SetTitle("truthPerPadIndex");
truthPerPadIndex->SetLineColor(kBlue);
truthPerPadIndex->SetLineWidth(2);

TCanvas *c1 = new TCanvas("c1","c1",1000,700);

scalePerPadIndex1d->Draw();
c1->SaveAs("scalePerPadIndex1d.png");

truthPerPadIndex->Draw();
c1->SaveAs("truthPerPadIndex.png");

truthPerPadIndex->Draw();
scalePerPadIndex1d->Draw("same");
c1->SaveAs("truthPerPadIndexResidual.png");


}