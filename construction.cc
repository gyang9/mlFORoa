#include "construction.hh"

using namespace std;

Construction::Construction(const char* name){}

Construction::~Construction(){;}

void Construction::readSpectrum(){

  TTree* tree = (TTree*)infile->Get("cafTree");  
  auto h = new TH1F("","",nbin, 0, 20); // in GeV
  double Ev_reco_numu;
  tree->Print();
  tree->SetBranchAddress("Ev_reco_numu", &Ev_reco_numu);
  for (int i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    h->Fill(Ev_reco_numu);
  }
  infile->Close();
  for (int i=0;i<h->GetNbinsX(); i++){
    spectrum.push_back(h->GetBinContent(i+1));
  }

}

void Construction::constructData(std::vector<double> v){

  TTree* tree = (TTree*)infile->Get("caf");
  auto h = new TH1F("","",nbin, 0, 20); // in GeV
  double Ev_reco_numu;
  tree->SetBranchAddress("Ev_reco_numu", &Ev_reco_numu);
  for (int i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    h->Fill(v[0] + Ev_reco_numu* v[1] );
  }
  infile->Close();
  for (int i=0;i<h->GetNbinsX(); i++){
    fakeData.push_back(h->GetBinContent(i+1));
  }
}

std::vector<double> Construction::constructMC(std::vector<double> v){

  TTree* tree = (TTree*)infile->Get("caf");
  auto h = new TH1F("","",nbin, 0, 20); // in GeV
  double Ev_reco_numu;
  tree->SetBranchAddress("Ev_reco_numu", &Ev_reco_numu);
  for (int i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    h->Fill(v[0] + Ev_reco_numu* v[1] );
  }
  infile->Close();
  std::vector<double > spec;
  for (int i=0;i<h->GetNbinsX(); i++){
    spec.push_back(h->GetBinContent(i+1));
  }
  return spec;
}

double Construction::getChi2(std::vector<double> mc){

  double chi2 = 0;
  if (mc.size() != fakeData.size()){
    std::cout<<"the data and mc have different binning, exit."<<std::endl;
    exit(0);
  }
  else {
    for (int i=0;i<mc.size();i++){
      chi2 += pow(fakeData.at(i) - mc.at(i),2)/pow(sqrt(fakeData.at(i)),2);
    }	  
  }
  return chi2;
}

double Construction::getPenalty(std::vector<double> v){
  double pen = 0;
  for (int i=0;i<v.size();i++){
    pen += pow(v.at(i)-nominal.at(i),2)/pow(uncertainty.at(i),2);
  }
  return pen;
}



