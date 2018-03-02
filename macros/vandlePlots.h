#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "TProof.h"

class AnamergerSelector : public TSelector {
public :

  AnamergerSelector(TTree * =0);
  virtual ~AnamergerSelector();

  virtual Int_t   Version() const { return 2; }
  virtual void    Init(TTree* mergedData){}
  virtual void    Begin(TTree *mergedData){}
  virtual void    SlaveBegin(TTree *mergedData){}
  virtual Bool_t  Notify() { return kTRUE;}
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate(){}
  virtual void    Terminate(){}

  std::set<int> output_name_set;

protected:

  TTreeReader aReader;
  TTreeReaderValue <unsigned int> vandle_barNum;
  TTreeReaderValue <double> vandle_BarQDC;
  TTreeReaderValue <double> vandle_TOF;
  TTreeReaderValue <double> vandle_TDiff;
  TTreeReaderValue <int> output_name;

  char vandle_QDCvsTOF_char[345];
  // TH2D* vandle_QDCvsTOF = new TH2D("vandle_QDCvsTOF",isotope,830,-30,800,1000,0,1000);
  // vandle_QDCvsTOF->GetXaxis()->SetTitle("TOF");
  // vandle_QDCvsTOF->GetYaxis()->SetTitle("QDC");
  //
  // char vandle_BarvsTOF_char[345];
  // TH2D* vandle_BarvsTOF = new TH2D("vandle_BarvsTOF",isotope,830,-30,200,45,0,45);
  // vandle_BarvsTOF->GetXaxis()->SetTitle("TOF");
  // vandle_BarvsTOF->GetYaxis()->SetTitle("Bar #");
  //
  // char vandle_BarvsQDC_char[345];
  // TH2D* vandle_BarvsQDC = new TH2D("vandle_BarvsQDC",isotope,830,-30,400,45,0,45);
  // vandle_BarvsQDC->GetXaxis()->SetTitle("QDC");
  // vandle_BarvsQDC->GetYaxis()->SetTitle("Bar #");
  //
  // char vandle_BarvsTDiff_char[345];
  // TH2D* vandle_BarvsTDiff = new TH2D("vandle_BarvsTDiff",isotope,1000,-1000,1000,45,0,45);
  // vandle_BarvsTDiff->GetXaxis()->SetTitle("TDiff [nsec]");
  // vandle_BarvsTDiff->GetYaxis()->SetTitle("Bar #");


  // array for histograms
  TObjArray* fHistArray;
  // output file
  TFile * fOutputFile;

  // ClassDef(AnamergerSelector,0)
};

AnamergerSelector::AnamergerSelector(TTree* mergedData):
   aReader          (mergedData),
   vandle_barNum    (aReader, "vandle_barNum"),
   vandle_BarQDC    (aReader, "vandle_BarQDC"),
   vandle_TOF       (aReader, "vandle_TOF"),
   vandle_TDiff     (aReader, "vandle_TDiff"),
   output_name      (aReader, "output_name")
{

}

AnamergerSelector::~AnamergerSelector()
{

}

Bool_t AnamergerSelector::Process(Long64_t entry)
{
  return true;
}
