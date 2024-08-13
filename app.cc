#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <functional>
#include <fstream>

#include "construction.hh"
#include "Util.hh"

// MCMC function using Metropolis-Hastings algorithm
void run_mcmc(Construction* rep, int iterations, std::ofstream& out) {
    // Initial values
    double ashift = 0.0;
    double bshift = 0.5;
    double cshift = -0.5;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> uniform_dist(0.0, 1.0);
    std::normal_distribution<> proposal_dist(0.0, 0.01); // Proposal distribution

    for (int n = 0; n < iterations; ++n) {
        // Propose new shifts
        double ashift_new = ashift + proposal_dist(gen);
        double bshift_new = bshift + proposal_dist(gen);
        double cshift_new = cshift + proposal_dist(gen);

        // Construct MC test with proposed shifts
        std::vector<double> mc_shift_new = {ashift_new, bshift_new, cshift_new};
        std::vector<double> mc_test_new = rep->constructMC(mc_shift_new);
        double chi2_new = rep->getChi2(mc_test_new) + rep->getPenalty(mc_shift_new);

        // Construct MC test with current shifts
        std::vector<double> mc_shift = {ashift, bshift, cshift};
        std::vector<double> mc_test = rep->constructMC(mc_shift);
        double chi2_current = rep->getChi2(mc_test) + rep->getPenalty(mc_shift);

        // Calculate acceptance ratio
        double acceptance_ratio = std::exp(-0.5 * (chi2_new - chi2_current));

        // Accept or reject the new state
        if (uniform_dist(gen) < acceptance_ratio) {
            ashift = ashift_new;
            bshift = bshift_new;
            cshift = cshift_new;
            chi2_current = chi2_new;
        }

        // Output the results
        out << ashift << " " << bshift << " " << cshift << " " << chi2_current << std::endl;
    }
}

int main(int argc, char** argv){

  std::cout<<"creating object"<<std::endl;
  Construction * rep = new Construction ("_rep"); 
  
  std::cout<<"setting bins"<<std::endl;
  rep->setNbins(100); // number of bins from 0 to 20 GeV

  rep->setInFile("../data/readyForCAF_neutrino_swap0.root");

  std::cout<<"setting file"<<std::endl;
  rep->setInFile(rep->getInfile());

  std::cout<<"setting penalty"<<std::endl;
  std::vector<double> penalty_nominal = {0, 1, 0,0,0,0,0,0,0,0,0,0 };
  std::vector<double> penalty_uncertainty = {0.1, 0.1, 1,1,1,1,1,1,1,1,1,1,1};

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

  // setting up flux systematics
  rep->setflux("../data/flux_uncertainty.root", 10);

/*
  ofstream out;
  out.open("result.dat");

  //TH2F* h2 = new TH2F("","#chi^{2} map; systematic a; systematic b", 100,0,1, 100,0.5,1.5);
  std::cout<<"getting chi2"<<std::endl;
  for (int i = 0;i<10;i++){
    for (int j = 0; j< 10; j++){
      // if you want to add more systematics, be careful, it can be super slow.
      // the first two variations are always energy scale, the next 10 are flux uncertainties
      for (int k = 0; k< 2; k++){
	std::cout<<"looping over "<<i<<" "<<j<<" "<<k<<" "<<std::endl;

        double ashift = i*0.01;
        double bshift = 0.5 + j*0.01;
	double cshift = -0.5 + k*0.01;
	// if you have more loops, you need to add more shifts
        std::vector<double> mc_shift = {ashift,bshift, cshift}; // add more shift values here
        std::vector<double> mc_test = rep->constructMC(mc_shift); 
        double chi2 = rep->getChi2(mc_test) + rep->getPenalty(mc_shift);
        out<<i<<" "<<j<<" "<<k<<" "<<chi2<<endl;
        //h2->SetBinContent(i+1, k+1, chi2);
      }
    }
  }
  out.close();
*/

  std::ofstream out("mcmc_output.txt");

  int iterations = 10000; // Number of MCMC iterations
  run_mcmc(rep, iterations, out);

  out.close();
  return 0;

  //TFile* outfile = new TFile("output.root","RECREATE");
  //h2->Write("chi2_systematic");
  //outfile->Write();
  //outfile->Close();

  //th2TOdat(h2, "systematic_variation.dat");

  //return 0;
}

