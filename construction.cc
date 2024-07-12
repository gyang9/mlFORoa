#include "construction.hh"

using namespace std;

Construction::Construction(const char* name){}

Construction::~Construction(){;}

void Construction::readSpectrum(){

  TFile* infile = new TFile(infilePath.c_str(), "READ");
  TTree* tree = (TTree*)infile->Get("tree");  
  auto h = new TH1F("","",nbin, 0, 20); // in GeV
  float recoE_mu;
  tree->Print();
  tree->SetBranchAddress("recoE_mu", &recoE_mu);
  for (int i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    //std::cout<<recoE_mu<<std::endl;
    h->Fill(recoE_mu);
  }
  for (int i=0;i<h->GetNbinsX(); i++){
    spectrum.push_back(h->GetBinContent(i+1));
    //std::cout<<i<<" "<<spectrum.at(i)<<std::endl;
  }
  infile->Close();
}

void Construction::constructData(std::vector<double> v){

  TFile* infile = new TFile(infilePath.c_str(), "READ");
  TTree* tree = (TTree*)infile->Get("tree");
  auto h = new TH1F("","",nbin, 0, 20); // in GeV
  float recoE_mu;
  tree->Print();
  tree->SetBranchAddress("recoE_mu", &recoE_mu);
  for (int i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    //std::cout<<v.at(0) + recoE_mu* v.at(1)<<std::endl;
    h->Fill(v.at(0) + recoE_mu* v.at(1) );
  }
  for (int i=0;i<h->GetNbinsX(); i++){
    fakeData.push_back(h->GetBinContent(i+1));
    //std::cout<<"filling fake data bin "<<i<<" "<<fakeData.at(i)<<std::endl; 
  }
  infile->Close();
}

std::vector<double> Construction::constructMC(std::vector<double> v){

  TFile* infile = new TFile(infilePath.c_str(), "READ");
  TTree* tree = (TTree*)infile->Get("tree");
  auto h = new TH1F("","",nbin, 0, 20); // in GeV
  float recoE_mu;
  tree->SetBranchAddress("recoE_mu", &recoE_mu);
  for (int i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    h->Fill(v.at(0) + recoE_mu* v.at(1) );
  }
  std::vector<double > spec;
  for (int i=0;i<h->GetNbinsX(); i++){
    spec.push_back(h->GetBinContent(i+1));
  }
  infile->Close();
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
      if (fakeData.at(i)> 0)
        chi2 += pow(fakeData.at(i) - mc.at(i),2)/pow(sqrt(fakeData.at(i)),2);
      //std::cout<<"in getChi2 "<<i<<" "<<fakeData.at(i)<<" "<<mc.at(i)<<" "<<pow(fakeData.at(i) - mc.at(i),2)/pow(sqrt(fakeData.at(i)),2)<<std::endl;
    }	  
  }
  return chi2;
}

double Construction::getPenalty(std::vector<double> v){
  double pen = 0;
  for (int i=0;i<v.size();i++){
    if (uncertainty.at(i)> 0)
      pen += pow(v.at(i)-nominal.at(i),2)/pow(uncertainty.at(i),2);
    //std::cout<<i<<" "<<pow(v.at(i)-nominal.at(i),2)/pow(uncertainty.at(i),2)<<std::endl;
  }
  return pen;
}



