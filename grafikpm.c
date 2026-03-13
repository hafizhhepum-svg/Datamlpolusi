#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TPaveText.h"

void grafikpm() {

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

    TCanvas *c1 = new TCanvas("c1","Scatter Plot PM2.5",1200,1000);

    // PERBAIKAN AGAR SUMBU Y TIDAK TERPOTONG
    c1->SetLeftMargin(0.15);
    c1->SetBottomMargin(0.12);

    TGraph *gr = new TGraph(n, &x_vals[0], &y_vals[0]);

    gr->SetTitle(";PM2.5 Observed;PM2.5 Predicted");


    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.8);

    gr->Draw("AP");

    // Pengaturan sumbu
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();

    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleSize(0.05);

    gr->GetXaxis()->SetTitleOffset(1.2);
    gr->GetYaxis()->SetTitleOffset(1.6);

    // LINEAR FIT
    TF1 *fit = new TF1("fit","pol1",
                       *std::min_element(x_vals.begin(), x_vals.end()),
                       *std::max_element(x_vals.begin(), x_vals.end()));

    gr->Fit(fit,"R");

    double c = fit->GetParameter(0);
    double m = fit->GetParameter(1);

    double y_mean = 0.0;
    for(int i=0;i<n;i++)
        y_mean += y_vals[i];

    y_mean /= n;

    double SS_tot = 0.0;
    double SS_res = 0.0;

    for(int i=0;i<n;i++){

        double y_fit = fit->Eval(x_vals[i]);

        SS_tot += pow(y_vals[i] - y_mean,2);
        SS_res += pow(y_vals[i] - y_fit,2);

    }

    double R2 = 1 - (SS_res / SS_tot);

    TPaveText *pt = new TPaveText(0.15,0.75,0.5,0.88,"NDC");

    pt->AddText(Form("y = %.4f x + %.4f", m, c));
    pt->AddText(Form("R^{2} = %.4f", R2));

    pt->SetFillColor(0);
    pt->Draw();

    c1->SaveAs("scatter_PM25_fit.png");

}
