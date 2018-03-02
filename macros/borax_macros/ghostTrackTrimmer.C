/////// ROOT libs ////////
#include "TRint.h"
#include "TROOT.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TCutG.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TStyle.h"
#include "TMath.h"
#include "TText.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TPolyLine3D.h"
#include "TPolyLine.h"
#include "TEllipse.h"
/////// c++ libs ////////
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>

double insideEllipse(TEllipse* ellipse, double px, double py);

void ghostTrackTrimmer(const char* TPCSummaryTree, const char* TPCSummaryTree_basename, const char* otherTreeCuts){

gStyle->SetLegendBorderSize(0);
gStyle->SetNumberContours(99);
char buffChar[345];
const double pi = atan(1)*4;

//////////////////////////////////////////////////////////////////////////////
//////////////////////// initialize input trees //////////////////////////////
//////////////////////////////////////////////////////////////////////////////
printf("\n\n initialize input trees \n");
printf("TPCSummaryTree: %s\n", TPCSummaryTree);
printf("TPCSummaryTree_basename: %s\n", TPCSummaryTree_basename);
printf("otherTreeCuts: %s\n", otherTreeCuts);
TFile* trackFile = new TFile(TPCSummaryTree);
TTree* tracks = (TTree*)trackFile->Get("tracks");
bool is_ghostTail;
bool has_ghostTail;
short trackNo;
short trackCount;
short fitType;
short nDigits;
unsigned int run;
int eventID;
float length;
float adc;
float energy;
TVector3* start = new TVector3();
TVector3* end = new TVector3();
TVector3* direction = new TVector3();
tracks->SetBranchAddress("run",&run);
tracks->SetBranchAddress("eventID",&eventID);
tracks->SetBranchAddress("start",&start);
tracks->SetBranchAddress("end",&end);
tracks->SetBranchAddress("direction",&direction);
tracks->SetBranchAddress("length",&length);
tracks->SetBranchAddress("adc",&adc);
tracks->SetBranchAddress("fitType",&fitType);
tracks->SetBranchAddress("trackNo",&trackNo);
tracks->SetBranchAddress("trackCount",&trackCount);
tracks->SetBranchAddress("nDigits",&nDigits);
char charBuff[345];
sprintf(charBuff,"fragCut_adc");
TCutG* fragCut_adc = new TCutG(charBuff,4);
fragCut_adc->SetVarX("adc");
fragCut_adc-> SetVarY("length");
fragCut_adc->SetLineColor(kViolet);
fragCut_adc->SetLineWidth(3);
fragCut_adc->SetFillColorAlpha(kViolet,0.15);
fragCut_adc->SetPoint(0,1000,1);
fragCut_adc->SetPoint(1,3000,2);
fragCut_adc->SetPoint(2,10000,4);
fragCut_adc->SetPoint(3,40000,5);
fragCut_adc->SetPoint(4,40000,0);
fragCut_adc->SetPoint(5,1000,0);
fragCut_adc->SetPoint(6,1000,1);
///////////////////  csv eventID of interest  ///////////////////
ofstream eventID_txtfile;
sprintf(buffChar,"eventID_%s.txt",TPCSummaryTree_basename);
eventID_txtfile.open (buffChar);
///////////////////  histograms  ///////////////////
TCanvas* ellipse_canvas  = new TCanvas("ellipse_canvas","ellipse_canvas",1000,1000);
ellipse_canvas->SetGrid(1,1);
TH2D* xyPlot = new TH2D("xyPlot","xyPlot",500,-7,7,500,-7,7);
xyPlot->SetStats(0);
xyPlot->GetXaxis()->SetTitle("X [cm]");
xyPlot->GetYaxis()->SetTitle("Y [cm]");
TH2D* zyPlot = new TH2D("zyPlot","zyPlot",100,-7,7,100,-7,7);
zyPlot->SetStats(0);
zyPlot->GetXaxis()->SetTitle("Y [cm]");
zyPlot->GetYaxis()->SetTitle("Z [cm]");
TH2D* zxPlot = new TH2D("zxPlot","zxPlot",100,-7,7,100,-7,7);
zxPlot->SetStats(0);
zxPlot->GetXaxis()->SetTitle("X [cm]");
zxPlot->GetYaxis()->SetTitle("Z [cm]");

//////////////////////////////////////////////////////////////////////////////
//////////////////////// filling treeEntry sets //////////////////////////////
//////////////////////////////////////////////////////////////////////////////
printf("\n\n filling treeEntry sets\n");
int fragCounter=0;
std::set<int> treeyEntrySet;
std::map<int,int> eventID_treeEntry_trk0;
std::map<int,int> has_ghostTail_treeEntry;
std::map<int,int> is_ghostTail_treeEntry;
std::map<int,int> ghostTail_trackNo_treeEntry;
std::map<int,int> ghostTail_trackCount_treeEntry;
std::map<int,int> ghostTail_nDigits_treeEntry;
std::map<int,double> ghostTail_adc_treeEntry;
for (int treeEntry = 0; treeEntry < tracks->GetEntries(); treeEntry++){
  if(treeEntry%500000==0)
    printf("treeEntry:%d \n",treeEntry);
  treeyEntrySet.insert(treeEntry);
  tracks->GetEntry(treeEntry);
  if (fragCut_adc->IsInside(adc,length)&&trackCount>1){
    printf("treeEntry:%d \t eventID:%d \t trackNo:%d passed fragCut_adc \n",treeEntry,eventID,trackNo);
    fragCounter++;
  }
  has_ghostTail_treeEntry[treeEntry]=0;
  is_ghostTail_treeEntry[treeEntry]=0;
  ghostTail_adc_treeEntry[treeEntry]=adc;
  ghostTail_trackCount_treeEntry[treeEntry]=trackCount;
  ghostTail_trackNo_treeEntry[treeEntry]=trackNo;
  ghostTail_nDigits_treeEntry[treeEntry]=nDigits;
  if (trackNo==0&&fitType==5&&fragCut_adc->IsInside(adc,length)&&trackCount>1){
    eventID_treeEntry_trk0[eventID]=treeEntry;
  }
}
printf("fragCount:%d\n", fragCounter);

//////////////////////////////////////////////////////////////////////////////
//////////////////////// initialize output trees /////////////////////////////
//////////////////////////////////////////////////////////////////////////////
printf("\n\n initialize output trees\n");
char onlyGhostTails_outputTreeName[234];
sprintf(onlyGhostTails_outputTreeName,"%s_noGhostTail.root",TPCSummaryTree_basename);
TFile* onlyGhostTails_file = new TFile(onlyGhostTails_outputTreeName,"create");
TTree* tracks_ghostTail = new TTree("tracks_ghostTail","tracks_ghostTail");
tracks_ghostTail->Branch("has_ghostTail",&has_ghostTail);
tracks_ghostTail->Branch("is_ghostTail",&is_ghostTail);
tracks_ghostTail->Branch("eventID",&eventID);
tracks_ghostTail->Branch("start",&start);
tracks_ghostTail->Branch("end",&end);
tracks_ghostTail->Branch("direction",&direction);
tracks_ghostTail->Branch("length",&length);
tracks_ghostTail->Branch("adc",&adc);
tracks_ghostTail->Branch("fitType",&fitType);
tracks_ghostTail->Branch("trackNo",&trackNo);
tracks_ghostTail->Branch("trackCount",&trackCount);
tracks_ghostTail->Branch("nDigits",&nDigits);
TTree* tracks_corrected = new TTree("tracks","tracks");
tracks_corrected->Branch("run",&run);
tracks_corrected->Branch("has_ghostTail",&has_ghostTail);
tracks_corrected->Branch("eventID",&eventID);
tracks_corrected->Branch("start",&start);
tracks_corrected->Branch("end",&end);
tracks_corrected->Branch("direction",&direction);
tracks_corrected->Branch("length",&length);
tracks_corrected->Branch("adc",&adc);
tracks_corrected->Branch("fitType",&fitType);
tracks_corrected->Branch("trackNo",&trackNo);
tracks_corrected->Branch("trackCount",&trackCount);
tracks_corrected->Branch("nDigits",&nDigits);

//////////////////////////////////////////////////////////////////////////////
//////////////////////// creating fragment maps //////////////////////////////
//////////////////////////////////////////////////////////////////////////////
printf("\n\n creating fragment maps \n");
int loopCount=0;
bool fragFlag=0;
bool ghostFlag=0;
int fragTrack_treeEntry=0;
std::map<int,TEllipse*> fragMap_treeEntry_to_ellipse;
std::map<int,TEllipse*>::iterator fragMap_treeEntry_to_ellipse_iter;
std::set<int> frag_eventID;
std::set<int> eventID_of_interest;
int counter=0;
for(auto treeEntry_it : eventID_treeEntry_trk0) {
  counter++;
  fragFlag=0;
  ghostFlag=0;
  fragTrack_treeEntry=0;
  int event_id = treeEntry_it.first;
  int treeEntry = treeEntry_it.second;
  /* define fragTrack_treeEntry */
  if(counter%50000==0) {
    printf("event_id:%d \t fragMap_treeEntry_to_ellipse.size():%lu\n",event_id,fragMap_treeEntry_to_ellipse.size());
  };
  for (int trk_n = 0; trk_n < trackCount; trk_n++) {
    tracks->GetEntry(treeEntry+trk_n);
    int entryColor=2+2*trk_n;
    if (fragCut_adc->IsInside(adc,length)&&trackCount>1) {
      printf("event_id:%d\n",event_id);
      printf("potential ghostTail event: %d\n",event_id);
      frag_eventID.insert(eventID);
      fragFlag=1;
      fragTrack_treeEntry=treeEntry+trk_n;
      double fragTrack_midpoint_x= (start->x()+end->x())/(double)2;
      double fragTrack_midpoint_y= (start->y()+end->y())/(double)2;
      double fragTrack_majAxis =0.99*std::pow(std::pow(start->x()-fragTrack_midpoint_x,2)+std::pow(start->y()-fragTrack_midpoint_y,2),(double)0.5);
      double fragTrack_minAxis = 0.6;
      TEllipse* fragEllipse = new TEllipse(fragTrack_midpoint_x,fragTrack_midpoint_y,fragTrack_majAxis,fragTrack_minAxis,0,360,direction->Phi()*180/(double)(pi));
      fragEllipse->SetFillStyle(1001);
      fragEllipse->SetLineWidth(2);
      fragEllipse->SetLineColor(entryColor);
      fragEllipse->SetFillColorAlpha(entryColor,0.12);
      fragEllipse->SetLineColorAlpha(entryColor,0.05);
      fragMap_treeEntry_to_ellipse.insert(std::pair<int,TEllipse*>(fragTrack_treeEntry,fragEllipse));
      eventID_of_interest.insert(event_id);
      eventID_txtfile << event_id << std::endl;
    }
  }
}
eventID_txtfile.close();

/////////////////////////////////////////////////////////////////
//////////////////// hunting for ghostTails /////////////////////
/////////////////////////////////////////////////////////////////
printf("\n\n hunting for ghostTails \n");
loopCount=0;
for(auto event_id : eventID_of_interest) {
  loopCount++;
  tracks->GetEntry(eventID_treeEntry_trk0[event_id]);
  double trk_adc[10]={0};
  double trk_adc_corrected[10]={0};
  int trk_trackCount[10]={0};
  int trk_trackCount_corrected[10]={0};
  int trk_trackNo[10]={0};
  int trk_trackNo_corrected[10]={0};
  int trk_nDigits[10]={0};
  int trk_nDigits_corrected[10]={0};
  for (int trk_n = 0; trk_n < trackCount; trk_n++) {
    tracks->GetEntry(eventID_treeEntry_trk0[event_id]+trk_n);
    trk_adc[trk_n]=adc;
    trk_adc_corrected[trk_n]=adc;
    trk_trackCount[trk_n]=trackCount;
    trk_trackCount_corrected[trk_n]=trackCount;
    trk_trackNo[trk_n]=trackNo;
    trk_trackNo_corrected[trk_n]=trackNo;
    trk_nDigits[trk_n]=nDigits;
    trk_nDigits_corrected[trk_n]=nDigits;
  }
  int ghostTail_belongs_to[10][10]={0};
  double ghostTail_zMidPoint[10][10]={0};
  bool trk_start_inside[10][10]={0};
  bool trk_end_inside[10][10]={0};
  bool ghostTailTrack[10]={0};
  int ghostTailEvent=0;
  xyPlot->Draw();

  //////////////////// discriminate ghost Tail tracks /////////////////////
  tracks->GetEntry(eventID_treeEntry_trk0[event_id]);
  for (int ellipse_trk = 0; ellipse_trk < trackCount; ellipse_trk++) {
    int ellipse_treeEntry = eventID_treeEntry_trk0[event_id]+ellipse_trk;
    tracks->GetEntry(ellipse_treeEntry);
    double ellipse_zMidPoint = (start->z()+end->z())*0.5;
    for (auto treeEntry_iter : fragMap_treeEntry_to_ellipse){
      if(treeEntry_iter.first != ellipse_treeEntry) continue;
      fragMap_treeEntry_to_ellipse[ellipse_treeEntry]->Draw("same");
      tracks->GetEntry(eventID_treeEntry_trk0[event_id]);
      for (int trk_n = 0; trk_n < trackCount; trk_n++) {
        if (ellipse_trk==trk_n) continue;
        tracks->GetEntry(eventID_treeEntry_trk0[event_id]+trk_n);
        double trk_zMidPoint = (start->z()+end->z())*0.5;
        trk_start_inside[ellipse_trk][trk_n] = ( 1 > insideEllipse(fragMap_treeEntry_to_ellipse[ellipse_treeEntry],start->x(),start->y()));
        trk_end_inside[ellipse_trk][trk_n] = ( 1 > insideEllipse(fragMap_treeEntry_to_ellipse[ellipse_treeEntry],end->x(),end->y()));
        ghostTail_belongs_to[ellipse_trk][trk_n] = (trk_start_inside[ellipse_trk][trk_n] && trk_end_inside[ellipse_trk][trk_n]);
        ghostTail_zMidPoint[ellipse_trk][trk_n] = abs(ellipse_zMidPoint)-abs(trk_zMidPoint);
        if (ghostTail_belongs_to[ellipse_trk][trk_n]&&ghostTail_zMidPoint[ellipse_trk][trk_n]>0) {
          trk_adc_corrected[ellipse_trk]=trk_adc_corrected[ellipse_trk]+trk_adc[trk_n];
          trk_trackCount_corrected[ellipse_trk]=trk_trackCount_corrected[ellipse_trk]-1;
          trk_nDigits_corrected[ellipse_trk]=trk_nDigits_corrected[ellipse_trk]+trk_nDigits[trk_n];
          has_ghostTail_treeEntry[eventID_treeEntry_trk0[event_id]+ellipse_trk]=1;
          trk_adc_corrected[trk_n]=0;
          trk_nDigits_corrected[trk_n]=0;
          ghostTailTrack[trk_n]=1;
          is_ghostTail_treeEntry[eventID_treeEntry_trk0[event_id]+trk_n]=1;
          ghostTailEvent++;
          ghostTail_adc_treeEntry[eventID_treeEntry_trk0[event_id]+ellipse_trk]=trk_adc_corrected[ellipse_trk];
          ghostTail_trackCount_treeEntry[eventID_treeEntry_trk0[event_id]+ellipse_trk]=trk_trackCount_corrected[ellipse_trk];
          ghostTail_trackNo_treeEntry[eventID_treeEntry_trk0[event_id]+ellipse_trk]=trk_trackNo_corrected[ellipse_trk];
          ghostTail_nDigits_treeEntry[eventID_treeEntry_trk0[event_id]+ellipse_trk]=trk_nDigits_corrected[ellipse_trk];
        }
      }
    }
  }

  //////////////////// output track data for log file /////////////////////
  printf("\n\n");
  printf("eventID: %d\n",eventID);
  for (int ellipse_trk = 0; ellipse_trk < trackCount; ellipse_trk++) {
    int ellipse_treeEntry = eventID_treeEntry_trk0[event_id]+ellipse_trk;
    tracks->GetEntry(ellipse_treeEntry);
    printf("\t trackNo: %d\n",trackNo);
    printf("\t length: %0.2f \t adc: %0.2f \t nDigits: %d\n",length,adc, nDigits);
    printf("\t trackCount: %d \t trackNo: %d\n",trackCount,trackNo);
    printf("\t start.x: %0.2f \t start.y: %0.2f\n",start->x(),start->y());
    printf("\t end.x: %0.2f \t end.y: %0.2f\n",end->x(),end->y());
    for (int trk_n = 0; trk_n < trackCount; trk_n++) {
      if (ellipse_trk==trk_n) continue;
      if (ghostTail_belongs_to[ellipse_trk][trk_n]&&ghostTail_zMidPoint[ellipse_trk][trk_n]>0) {
        printf("\t trk_start_inside[%d][%d]: %d \t trk_end_inside[%d][%d]: %d\n",ellipse_trk,trk_n,trk_start_inside[ellipse_trk][trk_n],ellipse_trk,trk_n,trk_end_inside[ellipse_trk][trk_n]);
        printf("\t ghostTail_belongs_to[%d][%d]: %d\n",ellipse_trk,trk_n,ghostTail_belongs_to[ellipse_trk][trk_n]);
        printf("\t ghostTail_zMidPoint[%d][%d]: %0.2f\n",ellipse_trk,trk_n,ghostTail_zMidPoint[ellipse_trk][trk_n]);
        printf("\t  trk_adc_corrected[%d]:%0.2f \n",ellipse_trk,trk_adc_corrected[ellipse_trk]);
        printf("\t  trk_trackCount_corrected[%d]:%d \n",ellipse_trk,trk_trackCount_corrected[ellipse_trk]);
        printf("\t  trk_trackNo_corrected[%d]:%d \n",ellipse_trk,trk_trackNo_corrected[ellipse_trk]);
        printf("\t  trk_nDigits_corrected[%d]:%d \n",ellipse_trk,trk_nDigits_corrected[ellipse_trk]);
        printf("\t   trk_adc_corrected[%d]:%0.2f \n",trk_n,trk_adc_corrected[trk_n]);
        printf("\t   trk_trackCount_corrected[%d]:%d \n",trk_n,trk_trackCount_corrected[trk_n]);
        printf("\t   trk_trackNo_corrected[%d]:%d \n",trk_n,trk_trackNo_corrected[trk_n]);
        printf("\t   trk_nDigits_corrected[%d]:%d \n",trk_n,trk_nDigits_corrected[trk_n]);
      }
    }
  }

  //////////////////// plot xy tracks & ellipses /////////////////////
  if(ghostTailEvent<2) continue;
  printf("loopCount:%d\n",loopCount);
  tracks->GetEntry(eventID_treeEntry_trk0[event_id]);
  TLegend* trk_legend = new TLegend(0.12,0.5,0.2,0.88);
  trk_legend->SetTextAlign(12);
  trk_legend->SetTextSize(.022);
  trk_legend->SetFillColorAlpha(kGreen,0.0);
  trk_legend->SetLineColorAlpha(kGreen,0.0);
  TPolyLine *pl_hex = new TPolyLine(5);
  pl_hex->SetLineColor(kSpring);
  pl_hex->SetLineWidth(2);
  for (int n = 0; n <= 6; ++n){
  	pl_hex->SetPoint(n,
  		7*TMath::Cos(n*60*TMath::Pi()/180),
  		7*TMath::Sin(n*60*TMath::Pi()/180));
  }
  for (int trk_n = 0; trk_n < trackCount; trk_n++) {
    int entryColor=2+2*trk_n;
    tracks->GetEntry(eventID_treeEntry_trk0[event_id]+trk_n);
    TLine *trk_n_line = new TLine(start->x(),start->y(),end->x(),end->y());
    trk_n_line->SetLineColor(entryColor);
    trk_n_line->SetLineWidth(6);
    trk_n_line->Draw("same");
    sprintf(buffChar,"trackNo: %d",trackNo);
    TLegendEntry* l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");l1->SetTextColor(entryColor);
    for (int ellipse_trk = 0; ellipse_trk < trackCount; ellipse_trk++) {
      if (ghostTail_belongs_to[ellipse_trk][trackNo]&&ghostTail_zMidPoint[ellipse_trk][trackNo]>0){
        printf("\t\t trk_start_inside[%d][%d]: %d \t trk_end_inside[%d][%d]: %d\n",ellipse_trk,trackNo,trk_start_inside[ellipse_trk][trackNo],ellipse_trk,trackNo,trk_end_inside[ellipse_trk][trackNo]);
        printf("\t\t\t ghostTail_belongs_to[%d][%d]: %d\n",ellipse_trk,trackNo,ghostTail_belongs_to[ellipse_trk][trackNo]);
        sprintf(buffChar,"***trk:%d is ghostTail of trk:%d***",trackNo,ellipse_trk);
        l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");
        l1->SetTextColor(1);
      };
    }
    sprintf(buffChar,"length: %0.2f \t adc: %0.2f \t trk_adc_corrected[%d]: %0.2f",length,adc,trk_n,trk_adc_corrected[trk_n]);
    l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");l1->SetTextColor(entryColor);
    sprintf(buffChar,"start.x: %0.2f \tstart.y: %0.2f",start->x(),start->y());
    l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");l1->SetTextColor(entryColor);
    sprintf(buffChar,"end.x: %0.2f \tend.y: %0.2f",end->x(),end->y());
    l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");l1->SetTextColor(entryColor);
    sprintf(buffChar,"Theta:%0.2f \t Phi:%0.2f",direction->Theta(),direction->Phi());
    l1 = trk_legend->AddEntry((TObject*)0,buffChar,"");l1->SetTextColor(entryColor);
    l1 = trk_legend->AddEntry((TObject*)0,"","");l1->SetTextColor(entryColor);
  }
  pl_hex->Draw("same");
  sprintf(buffChar,"xyPlot [evt %d]",event_id);
  xyPlot->SetTitle(buffChar);
  trk_legend->Draw();
  sprintf(buffChar,"ellipse_evt%d_xy.png",(int)event_id);
  ellipse_canvas->SaveAs(buffChar);
  TCanvas* zParallel_canvas  = new TCanvas("zParallel_canvas","zParallel_canvas",500,1000);
  zParallel_canvas->cd();
	zParallel_canvas->Clear();
	zParallel_canvas->Modified();
	zParallel_canvas->Update();
  zParallel_canvas->SetGrid(1,1);
  zParallel_canvas->Divide(1,2,0,0);
  zParallel_canvas->cd(1);
  gPad->SetGrid();
  zyPlot->Draw();
  tracks->GetEntry(eventID_treeEntry_trk0[event_id]);
  for (int trk_n = 0; trk_n < trackCount; trk_n++) {
    int treeEntry=eventID_treeEntry_trk0[event_id]+trk_n;
    tracks->GetEntry(treeEntry);
    TLine *trk_n_line = new TLine(start->y(),start->z(),end->y(),end->z());
    trk_n_line->SetLineColor(2+2*trk_n);
    trk_n_line->SetLineWidth(6);
    trk_n_line->Draw("same");
  }
  zParallel_canvas->cd(2);
  gPad->SetGrid();
  zxPlot->Draw();
  for (int trk_n = 0; trk_n < trackCount; trk_n++) {
    tracks->GetEntry(eventID_treeEntry_trk0[event_id]+trk_n);
    TLine *trk_n_line = new TLine(start->x(),start->z(),end->x(),end->z());
    trk_n_line->SetLineColor(2+2*trk_n);
    trk_n_line->SetLineWidth(6);
    trk_n_line->Draw("same");
  }
  sprintf(buffChar,"ellipse_evt%d_zx_zy.png",(int)event_id);
  zParallel_canvas->SaveAs(buffChar);
  delete zParallel_canvas;
  ////////////////// create montage of 2d track projections /////////////////////
  sprintf(buffChar,"gm montage -mode concatenate -tile 2x1 ellipse_evt%d_zx_zy.png ellipse_evt%d_xy.png evt%d.png",(int)event_id,(int)event_id,(int)event_id);
  system(buffChar);
  sprintf(buffChar,"rm ellipse_evt%d_xy.png ellipse_evt%d_zx_zy.png",(int)event_id,(int)event_id);
  system(buffChar);
  if (ghostTailEvent) {
    sprintf(buffChar,"mv *evt%d*.png ghostTail/.",event_id);
    system(buffChar);
  }
  else {
    sprintf(buffChar,"mv *evt%d*.png no_ghostTail/.",event_id);
    system(buffChar);
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////// filling trees ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
printf("\n\n filling trees \n");
int is_ghostTail_counter=0;
int has_ghostTail_counter=0;
int is_ghostTail_treeEntry_flag;
int wtf_counter=0;
for (auto treeEntry : treeyEntrySet){
  tracks->GetEntry(treeEntry);
  is_ghostTail=is_ghostTail_treeEntry[treeEntry];
  has_ghostTail=has_ghostTail_treeEntry[treeEntry];
  // tracks_ghostTail->Fill();
  if ((has_ghostTail||is_ghostTail)) {
    printf("\n\n");
    printf("eventID:%d\n",eventID);
    printf("has_ghostTail:%d\tghostTail:%d\n",has_ghostTail,is_ghostTail);
    printf("adc:%0.2f\tghostTail_adc_treeEntry[%d]:%0.2f\n",adc,treeEntry,ghostTail_adc_treeEntry[treeEntry]);
    printf("trackCount:%d\tghostTail_trackCount_treeEntry[%d]:%d\n",trackCount,treeEntry,ghostTail_trackCount_treeEntry[treeEntry]);
    printf("trackNo:%d\tghostTail_trackNo_treeEntry[%d]:%d\n",trackNo,treeEntry,ghostTail_trackNo_treeEntry[treeEntry]);
    printf("nDigits:%d\tghostTail_nDigits_treeEntry[%d]:%d\n",nDigits,treeEntry,ghostTail_nDigits_treeEntry[treeEntry]);
  }

  if (has_ghostTail&&is_ghostTail) wtf_counter++;
  if(has_ghostTail) {
    has_ghostTail_counter++;
    adc = ghostTail_adc_treeEntry[treeEntry];
    trackCount = ghostTail_trackCount_treeEntry[treeEntry];
    trackNo = ghostTail_trackNo_treeEntry[treeEntry];
    nDigits = ghostTail_nDigits_treeEntry[treeEntry];
  }
  if (is_ghostTail) {
    is_ghostTail_counter++;
    continue;
  }
  tracks_corrected->Fill();
  if(treeEntry%500000==0) printf("eventID:%d\n",eventID);
}
printf("wtf_counter:%d \t is_ghostTail_counter:%d\thas_ghostTail_counter:%d\n",wtf_counter,is_ghostTail_counter,has_ghostTail_counter);
fragCut_adc->Write();printf("fragCut_adc->Write() ");
tracks_corrected->Write();printf("tracks_corrected->Write() ");
tracks_ghostTail->Write();printf("tracks_ghostTail->Write() ");
TH2D* LvsADC = new TH2D("LvsADC","LvsADC",10000,100,30000,1000,0,7);
LvsADC->GetXaxis()->SetTitle("energy [ADC]");
LvsADC->GetYaxis()->SetTitle("length [cm]");
TH2D* ADCvsCosTheta = new TH2D("ADCvsCosTheta","ADCvsCosTheta",200,-1,1,1000,100,40000);
ADCvsCosTheta->GetXaxis()->SetTitle("cos #theta");ADCvsCosTheta->GetYaxis()->SetTitle("energy [ADC]");
ellipse_canvas->SetLogz(0);ellipse_canvas->SetLogx(1);
sprintf(buffChar,"!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_ghostTail->Draw("length:adc>>LvsADC",buffChar,"colz");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1a_tracks_LvsADC.png");
ellipse_canvas->SetLogz(0);
sprintf(buffChar,"fragCut_adc&&trackCount>1&&!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_ghostTail->Draw("length:adc>>LvsADC",buffChar,"colz");
fragCut_adc->Draw("same f");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1b_tracks_LvsADC_fragCut.png");
sprintf(buffChar,"(has_ghostTail||is_ghostTail)&&!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_ghostTail->Draw("length:adc>>LvsADC",buffChar,"colz");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1c_tracks_ghostTail_LvsADC.is||has_ghostTail.png");
sprintf(buffChar,"has_ghostTail&&fragCut_adc&&!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_ghostTail->Draw("length:adc>>LvsADC",buffChar,"colz");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1cc_tracks_ghostTail_LvsADC.is||has_ghostTail.png");
ellipse_canvas->SetLogz(1);
sprintf(buffChar,"!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_ghostTail->Draw("length:adc>>LvsADC",buffChar,"colz");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1d_tracks_LvsADC.png");
sprintf(buffChar,"!is_ghostTail&&!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_ghostTail->Draw("length:adc>>LvsADC",buffChar,"colz");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1e_tracks_ghostTail_!is_ghostTail.png");
sprintf(buffChar,"!(-1<direction.Phi()&&direction.Phi()<0)&&fitType==5");
tracks_corrected->Draw("length:adc>>LvsADC",buffChar,"colz");
LvsADC->SetTitle(buffChar);
ellipse_canvas->SaveAs("1f_tracks_corrected_LvsADC.png");
}


double insideEllipse(TEllipse* ellipse, double px, double py){
  const double pi = atan(1)*4;
  double theta = (ellipse->GetTheta())*pi/(double)180;
  double r1r1 = ellipse->GetR1()*ellipse->GetR1();
  double r2r2 = ellipse->GetR2()*ellipse->GetR2();
  double x1 = ellipse->GetX1();
  double y1 = ellipse->GetY1();
  double x = px - x1;
  double y = py - y1;
  double ellipseTest =
   std::pow(x*cos(theta)+y*sin(theta),2)/r1r1
  +std::pow(x*sin(theta)-y*cos(theta),2)/r2r2;
  return ellipseTest;
}
