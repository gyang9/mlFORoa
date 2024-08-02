#include <iostream>

#include "construction.hh"
#include "Util.hh"
#include <random>
#include <vector>
#include <omp.h>
#include <mpi.h>

int main(int argc, char** argv){

  int size, rank;
  MPI_Init(&argc, &argv);                           
  MPI_Comm_size(MPI_COMM_WORLD, &size);           
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::cout<<"creating object"<<std::endl;
  Construction * rep = new Construction ("_rep"); 
  
  std::cout<<"setting bins"<<std::endl;
  rep->setNbins(100); // number of bins from 0 to 20 GeV

  rep->setInFile("./data/readyForCAF_neutrino_swap0.root");

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
  rep->setflux("./data/flux_uncertainty.root", 10);

  ofstream out;
  out.open("result_"+std::to_string(rank)+".dat");

  out << std::fixed << std::setprecision(2);
  if (0 == rank) {
    out << "sys0   sys1   sys2   sys3   sys4   sys5   sys6   sys7   sys8   sys9   sys10  sys11  chi2" << endl;
    out << "# the next 2 lines are nominal and uncertainty" << endl;
  
    // Print nominal and uncertainty
    for (int i = 0; i < 12; i++)
        out << std::setw(5) << penalty_nominal.at(i) << "  " ;
    out << endl;
    for (int i = 0; i < 12; i++)
        out << std::setw(5) << penalty_uncertainty.at(i) << "  " ;
    out << endl;
    out << "#high dim space scan " << endl;
  }

  //TH2F* h2 = new TH2F("","#chi^{2} map; systematic a; systematic b", 100,0,1, 100,0.5,1.5);
  std::cout<<"getting chi2"<<std::endl;
  
  int ibgn = rank * 5;
  int iend = ibgn + 5;
  for (int i = ibgn; i < iend; i++){ 
    
    for (int j = 0; j < 100; j++){ 
      // if you want to add more systematics, be careful, it can be super slow.
      // the first two variations are always energy scale, the next 10 are flux uncertainties
      
      //std::cout<<"looping over "<<i<<" "<<j<<endl;
      out<<"looping over "<<i<<" "<<j << " " << rank <<endl;
      
      // for each energy generate a random sample 
      // Initialize the random number generator
      const int numPoints = 1000; // x 10^-3 is the size of output file in GB
      const int dimensions = 10;
      const int maxValue = 200;

      std::random_device rd;
      std::mt19937 gen(rd()+rank);
      std::uniform_int_distribution<> dis(0, maxValue - 1);

      // Sample random points
      int k0, k1, k2, k3, k4, k5, k6, k7, k8, k9; 
      
      for (int np = 0; np < numPoints; ++np) 
      {
         //#pragma omp parallel for num_threads(10) 
         for (int d = 0; d < dimensions; ++d) {
             k0 = dis(gen); 
             k1 = dis(gen);
             k2 = dis(gen);
             k3 = dis(gen);
             k4 = dis(gen);
             k5 = dis(gen);
             k6 = dis(gen);
             k7 = dis(gen);
             k8 = dis(gen);
             k9 = dis(gen);
         }

      //for (int k0 = 0; k0 < 200; k0++) //200
      //for (int k1 = 0; k1 < 200; k1++) //200
      //for (int k2 = 0; k2 < 200; k2++)
      //for (int k3 = 0; k3 < 200; k3++)
      //for (int k4 = 0; k4 < 200; k4++)
      //for (int k5 = 0; k5 < 200; k5++)
      //for (int k6 = 0; k6 < 200; k6++)
      //for (int k7 = 0; k7 < 200; k7++)
      //for (int k8 = 0; k8 < 200; k8++)
      //for (int k9 = 0; k9 < 200; k9++)
      //{

         double ashift = i*0.01;
         double bshift = 0.5 + j*0.01;
	 
	 double cshift = -1.0 + k0*0.01;
	 double dshift = -1.0 + k1*0.01;
	 double eshift = -1.0 + k2*0.01;
	 double fshift = -1.0 + k3*0.01;
	 double gshift = -1.0 + k4*0.01;
	 double hshift = -1.0 + k5*0.01;
	 double ishift = -1.0 + k6*0.01;
	 double jshift = -1.0 + k7*0.01;
	 double kshift = -1.0 + k8*0.01;
	 double lshift = -1.0 + k9*0.01;
	 
	 // if you have more loops, you need to add more shifts
         
	 std::vector<double> mc_shift = {ashift, bshift, cshift, dshift, eshift, 
	 	fshift, gshift, hshift, ishift, jshift, kshift, lshift}; // add more shift values here
         std::vector<double> mc_test = rep->constructMC(mc_shift); 
         
	 double chi2 = rep->getChi2(mc_test) + rep->getPenalty(mc_shift);
         
	 out << std::setw(5) << ashift << "  " << std::setw(5) << bshift << "  " 
	     << std::setw(5) << cshift << "  " << std::setw(5) << dshift << "  "
	     << std::setw(5) << eshift << "  " << std::setw(5) << fshift << "  "
	     << std::setw(5) << gshift << "  " << std::setw(5) << hshift << "  "
	     << std::setw(5) << ishift << "  " << std::setw(5) << jshift << "  "
	     << std::setw(5) << kshift << "  " << std::setw(5) << lshift << "  "
	     << chi2 << endl;
         
	 //h2->SetBinContent(i+1, k+1, chi2);
      }
    }
  }
  out.close();
  //TFile* outfile = new TFile("output.root","RECREATE");
  //h2->Write("chi2_systematic");
  //outfile->Write();
  //outfile->Close();

  //th2TOdat(h2, "systematic_variation.dat");

  MPI_Finalize();          
  return 0;
}

