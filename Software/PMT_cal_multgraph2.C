#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLine.h>
#include <TLatex.h>
#include <vector>
#include <iostream>
using namespace std;


void PMT_cal_multgraph2() 
{
    vector<TString> inputFiles = 
    {
        // "PMT_cal/PMT1.csv", "PMT_cal/PMT2.csv", "PMT_cal/PMT3.csv", 
        // "PMT_cal/PMT4.csv", "PMT_cal/PMT5.csv", "PMT_cal/PMT6.csv",
        // "PMT_cal/PMT7.csv", "PMT_cal/PMT8.csv", "PMT_cal/PMT9.csv",
        // "PMT_cal/PMT10.csv", "PMT_cal/PMT11.csv"
        "PMT_cal/PMT8.csv"
    };

    TCanvas *c1 = new TCanvas("c1", "PMT Calibration", 0, 0, 900, 700);
    TMultiGraph *mg = new TMultiGraph();
    TLegend *legend = new TLegend(0.25, 0.6, 0.1, 0.9);
    legend->SetMargin(0.6);
    legend->SetTextSize(0.02);


    for (size_t i = 0; i < inputFiles.size(); ++i) 
    {
        // Load the data from CSV into a TGraph
        TGraph *gr = new TGraph(inputFiles[i], "%*lg %*lg %*lg %*lg %lg %lg", ",");
        
        // Flip the axes by swapping x and y for each point
        double x, y;
        for (Int_t j = 0; j < gr->GetN(); ++j) 
        {
            gr->GetPoint(j, x, y);
            gr->SetPoint(j, y, x);
        }

        // Extract the file name from the path
        TString filePath = inputFiles[i];
        TString fileName = filePath;
        
        // Replace all '/' with a placeholder (e.g., '_') to handle paths
        fileName.ReplaceAll("/", "_");
        
        // Find the position of the last '_'
        Int_t pos = fileName.Last('_');
        
        // Extract the file name after the last '_'
        fileName = fileName(pos+1, fileName.Length() - pos - 1);
        
        // Remove the .csv extension from the file name
        fileName.ReplaceAll(".csv", "");
        
        TString legendEntry = TString::Format("%s", fileName.Data());

        mg->Add(gr);
        
        legend->AddEntry(gr, legendEntry, "lp");
    }

    // Shaded Box(s)
    float max = 1000;
    Double_t x1[5] = { 900, 1000,  1000,  900, 900};
    Double_t y1[5] = {   0,    0,   max,  max,   0};
    auto excl1 = new TGraph(5,x1,y1);
    excl1->SetLineColor(41);
    excl1->SetFillColor(41);
    excl1->SetFillStyle(3001);
    mg->Add(excl1,"F");

    gPad->SetTicks();
    // gPad->SetLogx();
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();

    mg->Draw("ALP PLC");

    mg->SetTitle("PMT 8 Count Rate vs Voltage;Voltage (V);Counts/Min");
    mg->GetXaxis()->SetRangeUser(0.1, 10000); 
    mg->GetYaxis()->SetRangeUser(0.1, 3500);
    mg->GetXaxis()->SetMoreLogLabels();
    mg->GetXaxis()->SetNoExponent();
    mg->GetYaxis()->SetNoExponent();

    // Lines for things
    double low = 700;
    double high = 1000;
    TLine *line_low = new TLine(low, 0, low, 1000);
    line_low->SetLineColor(kRed);  
    line_low->SetLineStyle(4);
    line_low->SetLineWidth(2);    
    line_low->Draw("same");    
    TLine *line_high = new TLine(high, 0, high, 1000);
    line_high->SetLineColor(kBlue);  
    line_high->SetLineStyle(4);
    line_high->SetLineWidth(2);    
    line_high->Draw("same");  

    double x_min = 900;
    double x_max = 1000;
    double y_min = 0;
    double y_max = 1000;

    // Labels for things
    TLatex *label_low = new TLatex();
    label_low->SetTextSize(0.03);   
    label_low->SetTextAlign(21);    
    label_low->DrawLatex(low, 1000, "Lower Limit");
    TLatex *label_high = new TLatex();
    label_high->SetTextSize(0.03);   
    label_high->SetTextAlign(21);    
    label_high->DrawLatex(high, 1000, "Upper Limit");     
    TLatex *label_box = new TLatex();
    label_box->SetTextSize(0.02);   
    label_box->SetTextAlign(21);    
    label_box->DrawLatex(950, 500, "Optimum Region");     

    // legend->Draw();
    c1->Update();
    c1->SaveAs("PMT_graphs/PMT_Calibration_8.png");
    c1->SaveAs("PMT_graphs/PMT_Calibration_8.pdf");
    // c1->SaveAs("PMT_graphs/PMT_Calibration_Multiple4.pdf");
}
