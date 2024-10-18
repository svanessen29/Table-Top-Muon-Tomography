#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraph.h>
#include <iostream>
using namespace std;

void graph_linescan_copy() 
{
    // Open the ROOT file and retrieve tree
    TFile *file = TFile::Open("Linescan_tung_data/linescan_singlept.root");
    TTree *tree = (TTree*)file->Get("T");
    TCanvas *c1 = new TCanvas("c1", "Hist", 1200, 800);
    c1->Divide(2,2);

    if (!tree) 
    {
        std::cerr << "Error: TTree not found in file!" << std::endl;
        return;
    }

    float x_axis_neg = -.015;
    float x_axis_pos = 0.015;
    float y_axis_neg = -0.25;
    float y_axis_pos = 0.05;

    TGraph *grPMT1 = new TGraph();
    TGraph *grPMT2 = new TGraph();
    TGraph *grPMT3 = new TGraph();
    TGraph *grPMT4 = new TGraph();

    float mV, PMT1, PMT2, PMT3, PMT4;
    tree->SetBranchAddress("C1", &mV);
    tree->SetBranchAddress("C2", &PMT1);
    tree->SetBranchAddress("C3", &PMT2);
    tree->SetBranchAddress("C4", &PMT3);
    tree->SetBranchAddress("C5", &PMT4);

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) 
    {
        tree->GetEntry(i);
        grPMT1->SetPoint(i, mV, PMT1);
        grPMT2->SetPoint(i, mV, PMT2);
        grPMT3->SetPoint(i, mV, PMT3);
        grPMT4->SetPoint(i, mV, PMT4);
    }

    // Note: where the graphs are placed on the canvas represent visually on plate
    grPMT1->SetStats(0);
    grPMT1->SetTitle("PMT1 Pulse Height");
    grPMT1->GetXaxis()->SetTitle("Time (#mus)");
    grPMT1->GetXaxis()->SetRangeUser(x_axis_neg,x_axis_pos);
    grPMT1->GetYaxis()->SetTitle("Pulse Height (V)");
    grPMT1->GetYaxis()->SetRangeUser(y_axis_neg,y_axis_pos);
    c1->cd(1);
    grPMT1->Draw();

    grPMT2->SetStats(0);
    grPMT2->SetTitle("PMT2 Pulse Height");
    grPMT2->GetXaxis()->SetTitle("Time (#mus)");
    grPMT2->GetXaxis()->SetRangeUser(x_axis_neg,x_axis_pos);
    grPMT2->GetYaxis()->SetTitle("Pulse Height (V)");
    grPMT2->GetYaxis()->SetRangeUser(y_axis_neg,y_axis_pos);
    c1->cd(3);
    grPMT2->Draw();    
    
    grPMT3->SetStats(0);
    grPMT3->SetTitle("PMT3 Pulse Height");
    grPMT3->GetXaxis()->SetTitle("Time (#mus)");
    grPMT3->GetXaxis()->SetRangeUser(x_axis_neg,x_axis_pos);
    grPMT3->GetYaxis()->SetTitle("Pulse Height (V)");
    grPMT3->GetYaxis()->SetRangeUser(y_axis_neg,y_axis_pos);
    c1->cd(4);
    grPMT3->Draw();

    grPMT4->SetStats(0);
    grPMT4->SetTitle("PMT4 Pulse Height");
    grPMT4->GetXaxis()->SetTitle("Time (#mus)");
    grPMT4->GetXaxis()->SetRangeUser(x_axis_neg,x_axis_pos);
    grPMT4->GetYaxis()->SetTitle("Pulse Height (V)");
    grPMT4->GetYaxis()->SetRangeUser(y_axis_neg,y_axis_pos);
    c1->cd(2);
    grPMT4->Draw();

    c1->SaveAs("Linescan_data/linescan_event10.png");
    c1->SaveAs("Linescan_data/linescan_event10.pdf");
}