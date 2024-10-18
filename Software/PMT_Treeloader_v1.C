#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TString.h>
#include <TSystem.h>
#include <TDirectory.h>
#include <iostream>
#include <filesystem>  // For iterating through files in a directory (C++17)

using namespace std;
namespace fs = std::filesystem;

void PMT_Treeloader_v1() 
{
    // Directory containing your CSV files
    string directory = "Scint_scan_test_v1";

    // Output ROOT file
    TString output_filename = "Scint_scan_data/PMT_scintscanv2.root";
    TFile *f = new TFile(output_filename, "RECREATE");

    // Create a single TTree
    TTree *T = new TTree("Scintscan", "Combined data from all CSV files");

    // Define the structure of the CSV files
    // A = analog input to oscilloscope
    TString csv_column_headers = "A_0:A_1:A_2:A_3:A_4";

    // Iterate over each CSV file in the directory
    for (const auto &entry : fs::directory_iterator(directory)) 
    {
        string filePath = entry.path().string();
        TString input_csv(filePath.c_str());

        // Load CSV data into the existing TTree
        Long64_t nlines = T->ReadFile(input_csv, csv_column_headers, ',');
        printf("Found %lld entries in %s and loaded them into the tree\n", nlines, filePath.c_str());
    }

    T->Write();

    f->Close();
}
