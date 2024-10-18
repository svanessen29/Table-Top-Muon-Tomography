#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TLine.h>
#include <TLatex.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void graph_linescan_tung() 
{
    std::ifstream infile("Linescan_tung_data/linescan_counts.csv");
    if (!infile.is_open()) 
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    int index;
    float count;
    std::vector<float> indices, counts;

    // Read the CSV file line by line
    std::string line;
    while (std::getline(infile, line)) 
    {
        std::stringstream ss(line);
        std::string value;

        // Extract the index (first column)
        std::getline(ss, value, ',');
        index = std::stoi(value);

        // Extract the count (second column)
        std::getline(ss, value, ',');
        count = std::stof(value);

        // Store values in vectors
        indices.push_back(index);
        counts.push_back(count);
    }

    infile.close();

    int n = indices.size();
    TGraph *graph = new TGraph(n, &indices[0], &counts[0]);

    graph->SetTitle("100 Hour Tungsten Line Scan;Grid Location;Counts");
    graph->SetMarkerStyle(21);
    graph->SetLineColor(kRed);
    graph->SetLineWidth(2);

    TCanvas *c1 = new TCanvas("c1", "Linescan Counts Graph from CSV", 1200, 800);
    graph->Draw("APL");

    // Shaded Box(s)
    float max = 100;
    Double_t x1[5] = { 13, 20,  20,  13, 13};
    Double_t y1[5] = {   0,    0,   max,  max,   0};
    auto excl1 = new TGraph(5,x1,y1);
    excl1->SetLineColor(41);
    excl1->SetFillColor(41);
    excl1->SetFillStyle(3345);
    excl1->Draw("F");  

    // Lines for things
    float x_low = 13;
    float x_high = 20;  
    float y_min = graph->GetYaxis()->GetXmin();  
    float y_max = graph->GetYaxis()->GetXmax(); 
    TLine *line_low = new TLine(x_low, y_min, x_low, y_max);
    line_low->SetLineColor(kBlue);  
    line_low->SetLineStyle(4);
    line_low->SetLineWidth(2);    
    line_low->Draw("same");    
    TLine *line_high = new TLine(x_high, y_min, x_high, y_max);
    line_high->SetLineColor(kBlue);  
    line_high->SetLineStyle(4);
    line_high->SetLineWidth(2);    
    line_high->Draw("same");  

    TLatex *label_low = new TLatex();
    label_low->SetTextSize(0.028);   
    label_low->SetTextAlign(21);    
    label_low->DrawLatex(16.5, 80, "Tungsten Block");



    c1->SaveAs("Linescan_tung_data/linescan_counts_graph.png");
    c1->SaveAs("Linescan_tung_data/linescan_counts_graph.pdf");
}
