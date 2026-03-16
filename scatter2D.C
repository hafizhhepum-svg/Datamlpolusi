#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <fstream>
#include <vector>
using namespace std;

void scatter2D() {
    gStyle->SetOptStat(0);
    vector<double> x;
    vector<double> y;

    ifstream file("out_23.txt");

    double a,b;
    TH2D *h2 = new TH2D("h2","Scatter Density Plot;X;Y",500,0,500,500,0,500);

    while(file >> a >> b){
        if (a == 0 || b == 0) continue;
        x.push_back(a);
        y.push_back(b);
        h2->Fill(a,b);
    }

    int n = x.size();

    TGraph *gr = new TGraph(n,&x[0],&y[0]);


    TCanvas *c1 = new TCanvas("c1","Scatter Plot",800,800);

    gr->SetTitle("Scatter Plot;x-axis;y-axis");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.5);
    gr->SetMarkerColor(kBlue);

    c1->cd(1);
    //gr->Draw("AP");
    h2->Draw("COLZ");

    c1->SaveAs("./output/scatter2D.png");

}
