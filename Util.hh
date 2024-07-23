#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <ctype.h>
#include <string.h>

using namespace std;

void th2TOdat(TH2* h2, std::string outdat){

  ofstream outfile;
  outfile.open(outdat.c_str());
  
  for (int i=0; i< h2->GetNbinsX(); i++){
    for (int j=0;j< h2->GetNbinsX(); j++){
      outfile<<i<<" "<<j<<" "<<h2->GetBinContent(i+1,j+1)<<"\n";
    }
  }
  outfile.close();
}
