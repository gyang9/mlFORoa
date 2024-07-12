#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <sstream>
#include <TList.h>

#include <TROOT.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TH1.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TFile.h>
#include <TRint.h>
#include <TH2.h>
#include <TFormula.h>
#include <TF1.h>
#include <TSpline.h>

#include <TF2.h>
#include <TMath.h>
#include <Math/DistFunc.h>
#include <TLine.h>
#include <TTree.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TRandom2.h>
#include <TRandom3.h>
#include <TGraphErrors.h>
#include <TVirtualFFT.h>
#include <TFoamIntegrand.h>
#include <TMatrixD.h>
#include <TVectorT.h>
#include <TDecompChol.h>

#include <RooFit.h>
#include "RooGlobalFunc.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooDataSet.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooWorkspace.h"
#include "RooNLLVar.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooChi2Var.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRandom.h"
#include <RooMsgService.h>
#include <RooHist.h>
#include <RooTrace.h>
#include <RooCategory.h>
#include "RooConstVar.h"
#include "RooBinning.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TMinuit.h"
#include "TGraph2D.h"

#include "RooFit.h"

#include "RooFitResult.h"
#include "RooMinimizer.h"
#include "RooFormulaVar.h"
#include "TMinuit.h"
#include <RooRealVar.h>

using namespace RooFit;

class Construction  {

public:
  Construction (const char* name);
    
  Construction (const Construction & other, const char* name = 0) {};
  virtual ~Construction () ;

  void readSpectrum();
  void constructData(std::vector<double> v);
  std::vector<double> constructMC(std::vector<double> v);

  std::string getInfile(){return infilePath;}
  double getChi2(std::vector<double> v);
  double getPenalty(std::vector<double> v);

  void setNbins(double n) {nbin = n;}
  void setInFile(std::string infile) {infilePath = infile;}
  void setPullNominal(std::vector<double> v){nominal = v;}
  void setPullUncertainty(std::vector<double> v){uncertainty = v;}

private:
  std::vector<double> spectrum;
  std::vector<double> systList;

  std::vector<double> fakeData;
  int nbin;
  std::string infilePath;

  std::vector<double> nominal;
  std::vector<double> uncertainty;
};  
