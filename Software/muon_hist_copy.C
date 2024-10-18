#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
using namespace std;

void muon_hist_copy() 
{
    // Open the ROOT file and retrieve tree
    TFile *file = TFile::Open("Coincidence_data/Coinc_sheet1.ROOT");
    TTree *tree = (TTree*)file->Get("T");
    TCanvas *c1 = new TCanvas("c1", "Histo", 800, 600);

    if (!tree) 
    {
        std::cerr << "Error: TTree not found in file!" << std::endl;
        return;
    }

    float x_axis = -.015;
    TH1F *hist = new TH1F("hist", "2hr Coincidence at 8cm ", 12, 0, 20);
    
    // TH2F *hist = new TH2F("hist2D", "2D Histogram with Shaded Region Lines", 5, 0, 20, 5, 0, 10);
    // TH2F *hist = new TH2F("hist2D", "Density vs Counts/Min", 20, 0, 20, 10, 0, 8); 

    float value_x, value_y;
    tree->SetBranchAddress("C9", &value_x);
    tree->SetBranchAddress("C4", &value_y);


    // Loop over the entries in the tree and fill the histogram
    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) 
    {
        tree->GetEntry(i);
        hist->Fill(value_x,value_y);
    }

    hist->SetFillColor(kBlue);
    hist->SetFillStyle(3001);
    hist->Draw("bar");
    // hist->Draw("COLZ");


    hist->SetStats(0);
    hist->GetXaxis()->SetTitle("Density (g/cm^{3})");
    hist->GetYaxis()->SetTitle("Counts/Min");

    // Draw a horizontal line at y = 20, for example
    // double low_TH = 20;
    // double high_TH = 300;
    // TLine *line_low = new TLine(x_axis, low_TH, .01, low_TH);
    // line_low->SetLineColor(kRed);  
    // line_low->SetLineWidth(2);    
    // line_low->Draw("same");    
    // TLine *line_high = new TLine(x_axis, high_TH, .01, high_TH);
    // line_high->SetLineColor(kBlue);  
    // line_high->SetLineWidth(2);    
    // line_high->Draw("same");      

    // double
    // TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);  
    // legend->SetHeader("Legend", "C"); 
    // legend->AddEntry(hist, "Data", "l"); 
    // legend->AddEntry(line_low, "Low Threshold", "l"); 
    // legend->AddEntry(hist, "High Threshold", "l"); 
    // legend->Draw(); 


    gPad->Update();

    c1->SaveAs("Coincidence_data/density_cpm.png");
}