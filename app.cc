#include <iostream>

#include "construction.hh"

int main(int argc, char** argv){

  std::cout<<"creating object"<<std::endl;
  Construction * rep = new Construction ("_rep"); 
  
  std::cout<<"setting bins"<<std::endl;
  rep->setNbins(100); // number of bins from 0 to 20 GeV

  rep->setInFile("/home/guang/work/theia/readyForCAF_neutrino_swap0.root");

  std::cout<<"setting file"<<std::endl;
  rep->setInFile(rep->getInfile());

  std::cout<<"setting penalty"<<std::endl;
  std::vector<double> penalty_nominal = {0, 1};
  std::vector<double> penalty_uncertainty = {0.1, 0.1};

  rep->setPullNominal(penalty_nominal);
  rep->setPullUncertainty(penalty_uncertainty);

  std::cout<<"reading spectrum"<<std::endl;
  rep->readSpectrum();

  std::cout<<"constructing data"<<std::endl;
  std::vector<double> data_shift = {0.0,1.0};
  rep->constructData(data_shift);

  std::cout<<"testing mc"<<std::endl;
  //std::vector<double> mc_shift = {0.01,1.01};
  //std::vector<double> mc_test = rep->constructMC(mc_shift);

  TH2F* h2 = new TH2F("","#chi^{2} map; systematic a; systematic b", 100,0,1, 100,0.5,1.5);
  std::cout<<"getting chi2"<<std::endl;
  for (int i = 0;i<100;i++){
    for (int j = 0; j< 100; j++){
      double ashift = i*0.01;
      double bshift = 0.5 + j*0.01;
      std::vector<double> mc_shift = {ashift,bshift};
      std::vector<double> mc_test = rep->constructMC(mc_shift);
      double chi2 = rep->getChi2(mc_test) + rep->getPenalty(mc_shift);
      h2->SetBinContent(i+1, j+1, chi2);
    }
  }
  TFile* outfile = new TFile("output.root","RECREATE");
  h2->Write("chi2_systematic");
  outfile->Write();
  outfile->Close();

  return 0;
}

