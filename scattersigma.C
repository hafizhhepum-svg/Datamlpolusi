#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TMath.h"

void scattersigma() {

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
        std::cout << "Not enough data!" << std::endl;
        return;
    }

    TCanvas *c1 = new TCanvas("c1","Scatter + Linear Fit",1200,1200);

    TGraph *gr = new TGraph(n, &x_vals[0], &y_vals[0]);
    gr->GetXaxis()->SetRangeUser(0,500);
    gr->GetXaxis()->SetNdivisions(414);
    gr->GetXaxis()->CenterTitle();
    gr->GetXaxis()->SetTitle("PM2.5 Observed");

    gr->GetYaxis()->SetRangeUser(0,500);
    gr->GetYaxis()->SetNdivisions(414);
    gr->GetYaxis()->CenterTitle();
    gr->GetYaxis()->SetTitle("PM2.5 Predicted");

    //gr->SetTitle("Scatter Plot with Linear Fit;X;Y");
    gr->SetMarkerStyle(4);
    gr->SetMarkerSize(0.6);
    gr->SetMarkerColor(4);
    gr->Draw("AP");

    // ======================
    // Linear Fit
    // ======================
    TF1 *fit = new TF1("fit","pol1",
                       TMath::MinElement(n,&x_vals[0]),
                       TMath::MaxElement(n,&x_vals[0]));

    gr->Fit(fit,"R");

    double p0 = fit->GetParameter(0);
    double p1 = fit->GetParameter(1);

    // ======================
    // Calculate R^2
    // ======================
    double mean_y = 0;
    for(int i=0;i<n;i++) mean_y += y_vals[i];
    mean_y /= n;

    double ss_tot = 0;
    double ss_res = 0;

    for(int i=0;i<n;i++){
        double yi = y_vals[i];
        double yfit = fit->Eval(x_vals[i]);
        ss_tot += pow(yi - mean_y, 2);
        ss_res += pow(yi - yfit, 2);
    }

    double r2 = 1 - ss_res/ss_tot;
    std::cout << "R^2 = " << r2 << std::endl;

    // ======================
    // Sigma from residual RMS
    // ======================
    double sigma = sqrt(ss_res/n);
    std::cout << "Residual sigma = " << sigma << std::endl;

    double xmin = fit->GetXmin();
    double xmax = fit->GetXmax();

    // 1σ band
    TF1 *band1_up = new TF1("band1_up",
        Form("%f + %f*x + %f", p0, p1, sigma), xmin, xmax);
    TF1 *band1_dn = new TF1("band1_dn",
        Form("%f + %f*x - %f", p0, p1, sigma), xmin, xmax);

    // 2σ band
    TF1 *band2_up = new TF1("band2_up",
        Form("%f + %f*x + %f", p0, p1, 2*sigma), xmin, xmax);
    TF1 *band2_dn = new TF1("band2_dn",
        Form("%f + %f*x - %f", p0, p1, 2*sigma), xmin, xmax);

    // 3σ band
    TF1 *band3_up = new TF1("band3_up",
        Form("%f + %f*x + %f", p0, p1, 3*sigma), xmin, xmax);
    TF1 *band3_dn = new TF1("band3_dn",
        Form("%f + %f*x - %f", p0, p1, 3*sigma), xmin, xmax);

    band1_up->SetLineStyle(2);
    band1_dn->SetLineStyle(2);

    band2_up->SetLineStyle(3);
    band2_dn->SetLineStyle(3);

    band3_up->SetLineStyle(4);
    band3_dn->SetLineStyle(4);

    /*band1_up->Draw("same");
    band1_dn->Draw("same");
    band2_up->Draw("same");
    band2_dn->Draw("same");
    band3_up->Draw("same");
    band3_dn->Draw("same");*/

    // Legend
    TLegend *leg = new TLegend(0.15,0.65,0.45,0.85);
    leg->AddEntry(gr,"Data","p");
    leg->AddEntry(fit,"Linear Fit (pol1)","l");
    //leg->AddEntry(band1_up,"1#sigma band","l");
    //leg->AddEntry(band2_up,"2#sigma band","l");
    //leg->AddEntry(band3_up,"3#sigma band","l");
    leg->Draw();

    c1->SaveAs("scatter_fit_sigma.png");

    std::cout << "Plot saved as scatter_fit_sigma.png" << std::endl;
}
