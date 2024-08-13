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

// Simple function to trim whitespace from strings
std::string trim(const std::string &str) {
    std::string trimmed = str;
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), trimmed.end());
    return trimmed;
}

// Function to read configuration file
std::map<std::string, std::string> read_config(const std::string &filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> config;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == '[') continue;  // Ignore comments and section headers
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                config[trim(key)] = trim(value);
            }
        }
    }
    return config;
}
