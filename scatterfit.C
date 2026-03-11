#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLegend.h"

void scatterfit() {

    std::ifstream infile("out_23.txt");

    if (!infile.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    std::vector<double> x_vals;
    std::vector<double> y_vals;

    double x, y;

    while (infile >> x >> y) {
        if (x == 0 || y == 0) continue;
        x_vals.push_back(x);
        y_vals.push_back(y);
    }

    infile.close();
    int n = x_vals.size();
    if (n < 2) {
        std::cout << "Not enough valid data points!" << std::endl;
        return;
    }

    TCanvas *c1 = new TCanvas("c1","Scatter Plot with Linear Fit",1200,1200);

    TGraph *gr = new TGraph(n, &x_vals[0], &y_vals[0]);
    gr->SetTitle("Scatter Plot with Linear Fit;X axis;Y axis");
    gr->SetMarkerStyle(4);
    gr->SetMarkerSize(0.6);
    gr->Draw("AP");

    // Linear fit
    TF1 *fit = new TF1("fit","pol1",
                       *std::min_element(x_vals.begin(), x_vals.end()),
                       *std::max_element(x_vals.begin(), x_vals.end()));

    gr->Fit(fit,"R");
    // --- Calculate R-square manually ---
    double y_mean = 0.0;
    for (int i = 0; i < n; i++)
        y_mean += y_vals[i];
    y_mean /= n;
    double SS_tot = 0.0;
    double SS_res = 0.0;
    for (int i = 0; i < n; i++) {
        double y_fit = fit->Eval(x_vals[i]);
        SS_tot += pow(y_vals[i] - y_mean, 2);
        SS_res += pow(y_vals[i] - y_fit, 2);
    }

    double R2 = 1 - (SS_res / SS_tot);

    // --- Display equation and R² ---
    double p0 = fit->GetParameter(0);
    double p1 = fit->GetParameter(1);

    TPaveText *pt = new TPaveText(0.15,0.75,0.5,0.88,"NDC");
    pt->AddText(Form("y = %.4f + %.4f x", p0, p1));
    pt->AddText(Form("R^{2} = %.4f", R2));
    pt->SetFillColor(0);
    pt->Draw();

    c1->SaveAs("scatter_linear_fit.png");

    std::cout << "Linear Fit Results:" << std::endl;
    std::cout << "Intercept (p0) = " << p0 << std::endl;
    std::cout << "Slope (p1)     = " << p1 << std::endl;
    std::cout << "R^2            = " << R2 << std::endl;
}
