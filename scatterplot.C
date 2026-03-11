#include <iostream>
#include <fstream>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TStyle.h"

void scatterplot() {

    std::ifstream infile("out_23.txt");

    if (!infile.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    std::vector<double> x_vals;
    std::vector<double> y_vals;

    double x, y;

    while (infile >> x >> y) {

        // Skip if any value is zero
        if (x == 0 || y == 0) continue;

        x_vals.push_back(x);
        y_vals.push_back(y);
    }

    infile.close();

    int n = x_vals.size();

    TCanvas *c1 = new TCanvas("c1","Scatter Plot",1200,1200);

    TGraph *gr = new TGraph(n, &x_vals[0], &y_vals[0]);

    gr->SetTitle("Scatter Plot;X axis;Y axis");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.2);

    gr->Draw("AP");

    c1->SaveAs("scatter_plot.png");

    std::cout << "Plot saved as scatter_plot.png" << std::endl;
}
