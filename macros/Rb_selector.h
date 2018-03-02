//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb  1 14:19:31 2018 by ROOT version 6.08/02
// from TTree data_summary_tree/data_summary_tree
// found on file: ../100Rb.root
//////////////////////////////////////////////////////////

#ifndef Rb_selector_h
#define Rb_selector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <string>



class Rb_selector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<UInt_t> evtNumber = {fReader, "evtNumber"};
   TTreeReaderValue<Int_t> output_name = {fReader, "output_name"};
   TTreeReaderValue<string> vandle_subtype = {fReader, "vandle_subtype"};
   TTreeReaderValue<Double_t> vandle_BarQDC = {fReader, "vandle_BarQDC"};
   TTreeReaderValue<Double_t> vandle_lQDC = {fReader, "vandle_lQDC"};
   TTreeReaderValue<Double_t> vandle_rQDC = {fReader, "vandle_rQDC"};
   TTreeReaderValue<Double_t> vandle_QDCPos = {fReader, "vandle_QDCPos"};
   TTreeReaderValue<Double_t> vandle_TOF = {fReader, "vandle_TOF"};
   TTreeReaderValue<Double_t> vandle_lSnR = {fReader, "vandle_lSnR"};
   TTreeReaderValue<Double_t> vandle_rSnR = {fReader, "vandle_rSnR"};
   TTreeReaderValue<Double_t> vandle_lAmp = {fReader, "vandle_lAmp"};
   TTreeReaderValue<Double_t> vandle_rAmp = {fReader, "vandle_rAmp"};
   TTreeReaderValue<Double_t> vandle_lMaxAmpPos = {fReader, "vandle_lMaxAmpPos"};
   TTreeReaderValue<Double_t> vandle_rMaxAmpPos = {fReader, "vandle_rMaxAmpPos"};
   TTreeReaderValue<Double_t> vandle_lAveBaseline = {fReader, "vandle_lAveBaseline"};
   TTreeReaderValue<Double_t> vandle_rAveBaseline = {fReader, "vandle_rAveBaseline"};
   TTreeReaderValue<UInt_t> vandle_barNum = {fReader, "vandle_barNum"};
   TTreeReaderValue<Double_t> vandle_TAvg = {fReader, "vandle_TAvg"};
   TTreeReaderValue<Double_t> vandle_Corrected_TAvg = {fReader, "vandle_Corrected_TAvg"};
   TTreeReaderValue<Double_t> vandle_TDiff = {fReader, "vandle_TDiff"};
   TTreeReaderValue<Double_t> vandle_Corrected_TDiff = {fReader, "vandle_Corrected_TDiff"};
   TTreeReaderValue<Double_t> beta_BarQDC = {fReader, "beta_BarQDC"};
   TTreeReaderValue<Double_t> beta_lQDC = {fReader, "beta_lQDC"};
   TTreeReaderValue<Double_t> beta_rQDC = {fReader, "beta_rQDC"};
   TTreeReaderValue<Double_t> beta_lSnR = {fReader, "beta_lSnR"};
   TTreeReaderValue<Double_t> beta_rSnR = {fReader, "beta_rSnR"};
   TTreeReaderValue<Double_t> beta_lAmp = {fReader, "beta_lAmp"};
   TTreeReaderValue<Double_t> beta_rAmp = {fReader, "beta_rAmp"};
   TTreeReaderValue<Double_t> beta_lMaxAmpPos = {fReader, "beta_lMaxAmpPos"};
   TTreeReaderValue<Double_t> beta_rMaxAmpPos = {fReader, "beta_rMaxAmpPos"};
   TTreeReaderValue<Double_t> beta_lAveBaseline = {fReader, "beta_lAveBaseline"};
   TTreeReaderValue<Double_t> beta_rAveBaseline = {fReader, "beta_rAveBaseline"};
   TTreeReaderValue<UInt_t> beta_barNum = {fReader, "beta_barNum"};
   TTreeReaderValue<Double_t> beta_TAvg = {fReader, "beta_TAvg"};
   TTreeReaderValue<Double_t> beta_Corrected_TAvg = {fReader, "beta_Corrected_TAvg"};
   TTreeReaderValue<Double_t> beta_TDiff = {fReader, "beta_TDiff"};
   TTreeReaderValue<Double_t> beta_Corrected_TDiff = {fReader, "beta_Corrected_TDiff"};


   Rb_selector(TTree * /*tree*/ =0) { }
   virtual ~Rb_selector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(Rb_selector,0);


   // TCutG *tcutg_NEUTRON = new TCutG("tcutg_NEUTRON",3);
   // tcutg_NEUTRON->SetVarX("vandle_TOF");
   // tcutg_NEUTRON->SetVarY("vandle_BarQDC");
   // tcutg_NEUTRON->SetLineWidth(4);
   // tcutg_NEUTRON->SetLineColor(kMagenta);
   // tcutg_NEUTRON->SetPoint(0,80,100);
   // tcutg_NEUTRON->SetPoint(1,400,100);
   // tcutg_NEUTRON->SetPoint(2,80,3000);
   // tcutg_NEUTRON->SetPoint(3,80,100);


};

#endif

#ifdef Rb_selector_cxx
void Rb_selector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t Rb_selector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef Rb_selector_cxx
