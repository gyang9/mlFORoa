#include <iostream>

#include "construction.hh"

int main(int argc, char** argv){

  std::cout<<"creating object"<<std::endl;
  Construction * rep = new Construction ("_rep"); 
  
  std::cout<<"setting bins"<<std::endl;
  rep->setNbins(100); // number of bins from 0 to 20 GeV

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
  std::vector<double> data_shift = {0.05,0.18};
  rep->constructData(data_shift);

  std::cout<<"testing mc"<<std::endl;
  std::vector<double> mc_shift = {0.06,0.18};
  std::vector<double> mc_test = rep->constructMC(mc_shift);

  std::cout<<"getting chi2"<<std::endl;
  double chi2 = rep->getChi2(mc_test) + rep->getPenalty(mc_shift);

  return 0;
}

