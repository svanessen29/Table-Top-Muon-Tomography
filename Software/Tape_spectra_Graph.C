#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Tape_spectra_Graph() 
{
    TCanvas *c1 = new TCanvas("c1", "Tape Spectra", 0, 0, 1300, 800);

    vector<double> x_values;
    vector<double> y_values;

    ifstream file("scintillator_tape_spectra/scintillator_tape_T2.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open file!" << endl;
        return;
    }
    
    double value1, value2;

    // Read values from the file and store in vectors
    while (file >> value1 >> value2) 
    {
        x_values.push_back(value1);
        y_values.push_back(value2);
    }

    int n = x_values.size();
    TGraph *gr = new TGraph(n, &x_values[0], &y_values[0]);

    gr->SetTitle("Scintillator Tape Transmission");
    gr->GetXaxis()->SetTitle("#gamma (nm)");
    gr->GetYaxis()->SetTitle("Transmission (%)");

    gr->SetMarkerStyle(kFullCircle);
    gr->SetMarkerSize(0.4);
    gr->SetMarkerColor(kBlue);

    gr->Draw("AP");

    c1->SaveAs("scintillator_tape_spectra/tape_spectra_T2.png");
    c1->SaveAs("scintillator_tape_spectra/tape_spectra_T2.pdf");

}
