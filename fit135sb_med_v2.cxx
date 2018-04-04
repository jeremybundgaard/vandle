/*This ROOT macro fits the TOF spectrum for the ANL experiment with 135Sb medium bars
using a gaussian with 2 Exponential tails


Author: S.Taylor(staylo65@vols.utk.edu)
Date Created:2/6/18
Date last updated:2/19/18 by S.Taylor

To run:
First create root file from simulation root file with histogram in it using create_his_from_sim.cxx
Change filenames to fit(~Line 74)
 (linux terminal)~$ root simulation_fit.cxx


 TO Do: Incorporate this with CalcBGT and CalcIB, and maybe roll into a fitting suite part of PAASS
*/
#include "TH1.h"        //needed for histogram stuff
#include "TMath.h"      //needed for math functions
#include "TF1.h"        //needed for ROOT fitting
#include "TCanvas.h"    //needed for ROOT canvas
#include "TFile.h"      //needed for ROOT file I/O
#include "TSystem.h"    //Contains a bunch of root things

double fitf(double *x, double *par)  //function to create the fit function
{
    /*Corresponding parameters from here and fitting_equation.pdf
     par[0,even]=Area    (total area of function)
     par[odd]=x0      (tof, gaussian centroid)
    */


    //USER MAY NEED TO CHANGE THESE
    //uses parametrization calculated from simulation to calculate these values for fit function based on x
    //K#=lambda#, D#=DToF# from spreadsheet and simulation_fit.cxx
    double sigma = 3.73582e-07*TMath::Power(x[0],3.0)-3.10167e-05*TMath::Power(x[0],2.0)+2.54158e-02*x[0]+2.58065;
    double K1 = 4.14950e-07*TMath::Power(x[0],3.0)-1.69101e-04*TMath::Power(x[0],2.0)+2.15233e-02*x[0]-5.72480e-01;
    double D1 = -5.89880e-07*TMath::Power(x[0],3.0)+2.96928e-04*TMath::Power(x[0],2.0)-4.75744e-02*x[0]+3.69891;
    double K2 = 2.77764e-08*TMath::Power(x[0],3.0)-5.58535e-06*TMath::Power(x[0],2.0)+5.18301e-04*x[0]+1.13972e-02;
    double D2 = -1.07014e-05*TMath::Power(x[0],3.0)+4.57840e-03*TMath::Power(x[0],2.0)-6.73114e-01*x[0]+4.50232e+01;


    //See fitting_equation.pdf for details on these
    double amplitude1  = par[0]*1./(sigma*TMath::Sqrt(2.*TMath::Pi()))
            *TMath::Exp(-0.5*TMath::Power((K1+D1),2.)-TMath::Power(K1,2.)/2.+K1*(K1+D1));
    double amplitude2 = amplitude1*TMath::Exp((TMath::Power(K1,2.)-TMath::Power(K2,2.))/2.-(K1-K2)*(K2+D2));

    //variable used to normalize the fit function
    double Normalization = 1./2.*(1.+1./(K1*K2)*TMath::Sqrt(2./TMath::Pi())
                            *TMath::Exp(-TMath::Power(D1,2.)/2.-D1*K1-K1/2.*(2.*D2+K1+2.*K2))
                            *((K1-K2)*TMath::Exp(K1*(D1+K1))+K2*TMath::Exp(K1*(D2+K2)))
                            +TMath::Erf((D1+K1)/TMath::Sqrt(2.)));

    double gaussVariable = (x[0]-par[1])/sigma;  //variable to simplify range separation calls below
    double simResponse; //variable to return the value of the function

    //This section will select the correct part the the equation to use for the fit based on the ranges
    if (gaussVariable>(K1+D1) && gaussVariable<=(K2+D2)) {  //First Exponential part
        simResponse = amplitude1/Normalization*TMath::Exp(TMath::Power(K1,2)/2.-K1*gaussVariable);
    }
    else if(gaussVariable>(K2+D2)) {  //Second exponential part
        simResponse = amplitude2/Normalization*TMath::Exp(TMath::Power(K2,2)/2.-K2*gaussVariable);
    }
    else {  //Gaussian part
        simResponse = par[0]/Normalization*1/(sigma*TMath::Sqrt(2*TMath::Pi()))*TMath::Exp(-0.5*(TMath::Power(gaussVariable,2)));
    }

    //returns the fit function for ROOT to use
    return simResponse;
}


//the total fit function
double tfit (double *x, double *par){
    return fitf(x,par)+fitf(x,&par[2])+fitf(x,&par[4])+fitf(x,&par[6])+fitf(x,&par[8])+fitf(x,&par[10])+fitf(x,&par[12]);
//return fitf(x,par)+fitf(x,&par[2]);
}



void fit135sb_med_v2()
{
    //USER MAY NEED TO CHANGE THESE!!!!!!!!!!!!!!!!!!!!!!!
    //variables for fit format
    int range_low = 50;//low range of fit
    int range_hi = 600;//hi range of fit
    int num_peaks = 7;//Number of peaks to fit with
//int num_peaks = 2;//Number of peaks to fit with

    int num_param = num_peaks*2;//number of fit parameters
    double par[num_param];  //array to hold parameters of fit

    //USER MAY NEED TO CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!
    //reads in root file
    TFile *rootfile = new TFile("135m_noy.root", "READ");


    //USER MAY NEED TO CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!
    //reads in hisotgram to fit
    TH1F *hcut_px = (TH1F*)rootfile->Get("hcut_px");

    //creates a canvas for the fit plot
    TCanvas *c2 = new TCanvas("c2","the fit canvas",800,800);


    //USER MAY NEED TO CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!
    //Creates a Root function based on function fitf above on specified range
    //User will need to change number of fitf calls based on number of peaks
    TF1 *total = new TF1("tfit",tfit,range_low,range_hi,num_param);
    total->SetNpx(range_hi-range_low);  //Sets number of points to use to draw the fit result, currently set to 1 point per ns based on fit range
    total->SetLineWidth(2);  //Sets the width of the fit line
    total->SetLineColor(kMagenta);  //Sets the color of the fit line


    //USER MAY NEED TO CHANGE THESE!!!!!!!!!!!!!!!!!!!!!!!
    // Sets initial values for parameters
    //List starting guess for area(0 and even #'s) and mu/peak location(odd #'s)
    //Need to have same # listed here as num_params
    total->SetParameter(0,100);//area1
    total->SetParameter(1,54);//tof1
    total->SetParameter(2,100);//area2
    total->SetParameter(3,61);//tof2
    total->SetParameter(4,100);//area3
    total->SetParameter(5,64);//tof3
    total->SetParameter(6,100);//area4
    total->SetParameter(7,67);//tof4
    total->SetParameter(8,100);//area5
    total->SetParameter(9,73);//tof5
    total->SetParameter(10,100);//area6
    total->SetParameter(11,81);//tof6
    total->SetParameter(12,100);//area7
    total->SetParameter(13,180);//tof7

    total->SetParName(0, "area1");
    total->SetParName(1, "tof1");
    total->SetParName(2, "area2");
    total->SetParName(3, "tof2");
    total->SetParName(4, "area3");
    total->SetParName(5, "tof3");
    total->SetParName(6, "area4");
    total->SetParName(7, "tof4");
    total->SetParName(8, "area5");
    total->SetParName(9, "tof5");
    total->SetParName(10, "area6");
    total->SetParName(11, "tof6");
    total->SetParName(12, "area7");
    total->SetParName(13, "tof7");


    //Fit histogram in range defined by function, will print fit parameter results matrix to ROOT terminal
    hcut_px->Draw();
    hcut_px->Fit(total,"r");

    //Gets integral of function between fit limits and prints to ROOT terminal after fit parameter results matrix
    printf("Integral of function = %g\n",total->Integral(range_low,range_hi));

}
