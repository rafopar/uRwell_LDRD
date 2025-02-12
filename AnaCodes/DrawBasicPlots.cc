/* 
 * File:   DrawBasicPlots.cc
 * Author: rafopar
 *
 * Created on July 5, 2024, 3:13 PM
 */

#include <cstdlib>
#include <cxxopts.hpp>

#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TLine.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveText.h>

using namespace std;

template <typename T> void FormarHist_1(TCanvas *c, T h );

/*
 * 
 */
int main(int argc, char** argv) {

    cxxopts::Options options("DrawBasicPlots", "Draws plots from the root file created by analysis");

    options.add_options()
            ("r,Run", "Run number", cxxopts::value<int>())
            ("t,Threshold", "Hit Threshold in terms of sigma", cxxopts::value<double>()->default_value("5"))
            ("m,MinHits", "Number of minimum hits in the cluster", cxxopts::value<int>()->default_value("1"))            
            ;

    auto parsed_options = options.parse(argc, argv);

    int run = 0;
    int fnum = -1;

    if (parsed_options.count("Run")) {
        run = parsed_options["Run"].as<int>();
    } else {
        cout << "The run number is nor provided. Exiting..." << endl;
        exit(1);
    }


    const double threshold = parsed_options["Threshold"].as<double>();
    if (!parsed_options.count("Threshold")) {
        cout << "* You didn't provide the hit threshold. Exitint" << endl;
        exit(1);
    }

    const int MinClSize = parsed_options["MinHits"].as<int>();
    if (!parsed_options.count("MinHits")) {
        cout << "* You didn't provide the Minimum hits int the cluster. Exiting " << endl;
        exit(1);
    }

    cout << "The hit threshold is " << threshold << "\\sigma" << endl;
    cout << "The Minimum cluster size is " << MinClSize << "hits" << endl;
    
     gStyle->SetOptStat(0);
    
     std::string keyWord = Form("%d_t%1.1f_m%d", run, threshold, MinClSize);
     
    TCanvas *c1 = new TCanvas("c1", "", 950, 950);
    c1->SetTopMargin(0.04);
    c1->SetRightMargin(0.04);
    c1->SetLeftMargin(0.14);
    TLatex *lat1 = new TLatex();
    lat1->SetNDC();
    lat1->SetTextFont(42);

    TLine *line1 = new TLine();
    line1->SetLineColor(2);
    line1->SetLineWidth(2);
    TFile *file_in = new TFile(Form("AnaData_%d_Thr_%1.1f_MinHits_%d.root", run, threshold, MinClSize), "Read");

    TF1 *f_Landau = new TF1("f_Landau", "[0]*TMath::Landau(x, [1], [2])", 0., 1000.);
    f_Landau->SetNpx(4500);

    double adcMaxAxis = 1500.;
    TH1D *h_GEM0_X_ADC1 = (TH1D*)file_in->Get("h_GEM0_X_ADC1");
    h_GEM0_X_ADC1->SetTitle(";ADC (GEM0_{X}) ");
    h_GEM0_X_ADC1->SetAxisRange(0., adcMaxAxis);
    h_GEM0_X_ADC1->Draw();
    f_Landau->SetParameters(5. * h_GEM0_X_ADC1->GetMaximum(), h_GEM0_X_ADC1->GetBinCenter(h_GEM0_X_ADC1->GetMaximumBin()), 10);
    h_GEM0_X_ADC1->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    lat1->DrawLatex(0.5, 0.7, Form("MPV = %1.1f", f_Landau->GetParameter(1)));
    c1->Print(Form("Figs/GEM0_X_ADC1_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_X_ADC1_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_X_ADC1_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH1D *h_GEM0_Y_ADC1 = (TH1D*)file_in->Get("h_GEM0_Y_ADC1");
    h_GEM0_Y_ADC1->SetTitle(";ADC (GEM0_{Y}) ");
    h_GEM0_Y_ADC1->SetAxisRange(0., adcMaxAxis);
    h_GEM0_Y_ADC1->Draw();
    f_Landau->SetParameters(5. * h_GEM0_Y_ADC1->GetMaximum(), h_GEM0_Y_ADC1->GetBinCenter(h_GEM0_Y_ADC1->GetMaximumBin()), 10);
    h_GEM0_Y_ADC1->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    lat1->DrawLatex(0.5, 0.7, Form("MPV = %1.1f", f_Landau->GetParameter(1)));
    c1->Print(Form("Figs/GEM0_Y_ADC1_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_Y_ADC1_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_Y_ADC1_%d_t%1.1f_m%d.root", run, threshold, MinClSize));

    TH1D *h_GEM1_X_ADC1 = (TH1D*)file_in->Get("h_GEM1_X_ADC1");
    h_GEM1_X_ADC1->SetTitle(";ADC (GEM1_{X}) ");
    h_GEM1_X_ADC1->SetAxisRange(0., adcMaxAxis);
    h_GEM1_X_ADC1->Draw();
    f_Landau->SetParameters(5. * h_GEM1_X_ADC1->GetMaximum(), h_GEM1_X_ADC1->GetBinCenter(h_GEM1_X_ADC1->GetMaximumBin()), 10);
    h_GEM1_X_ADC1->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    lat1->DrawLatex(0.5, 0.7, Form("MPV = %1.1f", f_Landau->GetParameter(1)));
    c1->Print(Form("Figs/GEM1_X_ADC1_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_X_ADC1_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_X_ADC1_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH1D *h_GEM1_Y_ADC1 = (TH1D*)file_in->Get("h_GEM1_Y_ADC1");
    h_GEM1_Y_ADC1->SetTitle(";ADC (GEM1_{Y}) ");
    h_GEM1_Y_ADC1->SetAxisRange(0., adcMaxAxis);
    h_GEM1_Y_ADC1->Draw();
    f_Landau->SetParameters(5. * h_GEM1_Y_ADC1->GetMaximum(), h_GEM1_Y_ADC1->GetBinCenter(h_GEM1_Y_ADC1->GetMaximumBin()), 10);
    h_GEM1_Y_ADC1->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    lat1->DrawLatex(0.5, 0.7, Form("MPV = %1.1f", f_Landau->GetParameter(1)));
    c1->Print(Form("Figs/GEM1_Y_ADC1_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_Y_ADC1_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_Y_ADC1_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    c1->SetLogz();
    TH2D *h_GEM0_YXC1 = (TH2D*)file_in->Get("h_GEM0_YXC1");
    h_GEM0_YXC1->SetTitle("; GEM0_{X} [cm]; GEM0_{Y} [cm]");
    h_GEM0_YXC1->Draw();
    c1->Print(Form("Figs/GEM0_YXC1_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_YXC1_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_YXC1_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH2D *h_GEM1_YXC1 = (TH2D*)file_in->Get("h_GEM1_YXC1");
    h_GEM1_YXC1->SetTitle("; GEM1_{X} [cm]; GEM1_{Y} [cm]");
    h_GEM1_YXC1->Draw();
    c1->Print(Form("Figs/GEM1_YXC1_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_YXC1_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_YXC1_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    c1->SetLogz(0);
    TH2D *h_GEM0_YX_peaktime1 = (TH2D*)file_in->Get("h_GEM0_YX_peaktime1");
    h_GEM0_YX_peaktime1->SetTitle("; X peak time; Y peak time");
    h_GEM0_YX_peaktime1->Draw();
    c1->Print(Form("Figs/GEM0_peakTime_YvsX_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_peakTime_YvsX_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_peakTime_YvsX_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH1D *h_GEM0_X_peaktim1 = (TH1D*)h_GEM0_YX_peaktime1->ProjectionX("h_GEM0_X_peaktim1", 1, h_GEM0_YX_peaktime1->GetNbinsY());
    h_GEM0_X_peaktim1->Draw();
    c1->Print(Form("Figs/GEM0_X_peaktime_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_X_peaktime_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_X_peaktime_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH1D *h_GEM0_Y_peaktim1 = (TH1D*)h_GEM0_YX_peaktime1->ProjectionY("h_GEM0_Y_peaktim1", 1, h_GEM0_YX_peaktime1->GetNbinsX());
    h_GEM0_Y_peaktim1->Draw();
    c1->Print(Form("Figs/GEM0_Y_peaktime_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_Y_peaktime_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM0_Y_peaktime_%d_t%1.1f_m%d.root", run, threshold, MinClSize));

    TH2D *h_GEM1_YX_peaktime1 = (TH2D*)file_in->Get("h_GEM1_YX_peaktime1");
    h_GEM1_YX_peaktime1->SetTitle("; X peak time; Y peak time");
    h_GEM1_YX_peaktime1->Draw();
    c1->Print(Form("Figs/GEM1_peakTime_YvsX_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_peakTime_YvsX_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_peakTime_YvsX_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH1D *h_GEM1_X_peaktim1 = (TH1D*)h_GEM1_YX_peaktime1->ProjectionX("h_GEM1_X_peaktim1", 1, h_GEM1_YX_peaktime1->GetNbinsY());
    h_GEM1_X_peaktim1->Draw();
    c1->Print(Form("Figs/GEM1_X_peaktime_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_X_peaktime_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_X_peaktime_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH1D *h_GEM1_Y_peaktim1 = (TH1D*)h_GEM1_YX_peaktime1->ProjectionY("h_GEM1_Y_peaktim1", 1, h_GEM1_YX_peaktime1->GetNbinsX());
    h_GEM1_Y_peaktim1->Draw();
    c1->Print(Form("Figs/GEM1_Y_peaktime_%d_t%1.1f_m%d.pdf", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_Y_peaktime_%d_t%1.1f_m%d.png", run, threshold, MinClSize));
    c1->Print(Form("Figs/GEM1_Y_peaktime_%d_t%1.1f_m%d.root", run, threshold, MinClSize));
    
    TH2D *h_Item1_YX_peaktime1 = (TH2D*)file_in->Get("h_Item1_YX_peaktime1");
    h_Item1_YX_peaktime1->SetTitle("; X peak time sample; Y peak time sample");
    h_Item1_YX_peaktime1->Draw("");
    c1->Print(Form("Figs/Item1_Peak_time_YX1_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Peak_time_YX1_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Peak_time_YX1_%s.root", keyWord.c_str()));
    
    TH1D *h_X_Peaktime1 = (TH1D*)h_Item1_YX_peaktime1->ProjectionX("h_X_Peaktime1", 1, h_Item1_YX_peaktime1->GetNbinsY() );
    h_X_Peaktime1->Draw();
    c1->Print(Form("Figs/Item1_X_Peak_time_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_X_Peak_time_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_X_Peak_time_%s.root", keyWord.c_str()));
    
    TH1D *h_Y_Peaktime1 = (TH1D*)h_Item1_YX_peaktime1->ProjectionY("h_Y_Peaktime1", 1, h_Item1_YX_peaktime1->GetNbinsX() );
    h_Y_Peaktime1->Draw();
    c1->Print(Form("Figs/Item1_Y_Peak_time_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Y_Peak_time_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Y_Peak_time_%s.root", keyWord.c_str()));
    
    TH2D *h_Item1_YXC1 = (TH2D*)file_in->Get("h_Item1_YXC1");
    FormarHist_1(c1, h_Item1_YXC1);
    h_Item1_YXC1->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item1_YXC1->SetMaximum( 5.*h_Item1_YXC1->GetEntries()/( h_Item1_YXC1->GetNbinsX()*h_Item1_YXC1->GetNbinsY() ) );
    h_Item1_YXC1->Draw();
    c1->Print(Form("Figs/Item1_YXc_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_YXc_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_YXc_%s.root", keyWord.c_str()));
    
    TH2D *h_Item1_YX_ADC1 = (TH2D*)file_in->Get("h_Item1_YX_ADC1");
    h_Item1_YX_ADC1->SetTitle("; ADC_X; ADC_Y");
    h_Item1_YX_ADC1->Draw();
    c1->Print(Form("Figs/Item1_ADC_YX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_ADC_YX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_ADC_YX_%s.root", keyWord.c_str()));

    lat1->SetTextSize(0.06);
    TH1D *h_Item1_ADC_X = (TH1D*)h_Item1_YX_ADC1->ProjectionX("h_Item1_ADC_X", 1, h_Item1_YX_ADC1->GetNbinsY());
    h_Item1_ADC_X->SetAxisRange(0., adcMaxAxis);
    FormarHist_1(c1, h_Item1_ADC_X);
    h_Item1_ADC_X->Draw();
    f_Landau->SetParameters(5. * h_Item1_ADC_X->GetMaximum(), h_Item1_ADC_X->GetBinCenter(h_Item1_ADC_X->GetMaximumBin()), 10);
    h_Item1_ADC_X->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    lat1->DrawLatex(0.55, 0.7, Form("MPV = %1.1f", f_Landau->GetParameter(1)));
    c1->Print(Form("Figs/Item1_ADC_X_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_ADC_X_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_ADC_X_%s.root", keyWord.c_str()));

    TH1D *h_Item1_ADC_Y = (TH1D*)h_Item1_YX_ADC1->ProjectionY("h_Item1_ADC_Y", 1, h_Item1_YX_ADC1->GetNbinsX());
    h_Item1_ADC_Y->SetAxisRange(0., adcMaxAxis);
    FormarHist_1(c1, h_Item1_ADC_Y);
    h_Item1_ADC_Y->Draw();
    f_Landau->SetParameters(5. * h_Item1_ADC_Y->GetMaximum(), h_Item1_ADC_Y->GetBinCenter(h_Item1_ADC_Y->GetMaximumBin()), 10);
    h_Item1_ADC_Y->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    lat1->DrawLatex(0.55, 0.7, Form("MPV = %1.1f", f_Landau->GetParameter(1)));
    c1->Print(Form("Figs/Item1_ADC_Y_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_ADC_Y_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_ADC_Y_%s.root", keyWord.c_str()));

    //x vs. y plot item 1 (sara)
    TH2D *h_n_Item1_Y_X_ClSize1 = (TH2D*)file_in->Get("h_n_Item1_Y_X_ClSize1");
    h_n_Item1_Y_X_ClSize1->SetTitle("; Clst_size X; Clst_size Y");
    h_n_Item1_Y_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item1_Clst_YvsX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Clst_YvsX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Clst_YvsX_%s.root", keyWord.c_str()));

    TH1D *h_n_Item1_X_ClSize1 = (TH1D*)h_n_Item1_Y_X_ClSize1->ProjectionX("h_n_Item1_X_ClSize1", 1, h_n_Item1_Y_X_ClSize1->GetNbinsY());
    h_n_Item1_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item1_X_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_X_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_X_ClSize_%s.root", keyWord.c_str()));

    TH1D *h_n_Item1_Y_ClSize1 = (TH1D*)h_n_Item1_Y_X_ClSize1->ProjectionY("h_n_Item1_Y_ClSize1", 1, h_n_Item1_Y_X_ClSize1->GetNbinsX());
    h_n_Item1_Y_ClSize1->Draw();
    c1->Print(Form("Figs/Item1_Y_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Y_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_Y_ClSize_%s.root", keyWord.c_str()));
    //

    TH2D *h_Item2_YX_peaktime1 = (TH2D*)file_in->Get("h_Item2_YX_peaktime1");
    h_Item2_YX_peaktime1->SetTitle("; X peak time sample; Y peak time sample");
    h_Item2_YX_peaktime1->Draw("");
    c1->Print(Form("Figs/Item2_Peak_time_YX1_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Peak_time_YX1_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Peak_time_YX1_%s.root", keyWord.c_str()));
    
    TH1D *h_Item2_X_Peaktime1 = (TH1D*)h_Item2_YX_peaktime1->ProjectionX("h_Item2_X_Peaktime1", 1, h_Item2_YX_peaktime1->GetNbinsY() );
    h_Item2_X_Peaktime1->Draw();
    c1->Print(Form("Figs/Item2_X_Peak_time_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_X_Peak_time_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_X_Peak_time_%s.root", keyWord.c_str()));
    
    TH1D *h_Item2_Y_Peaktime1 = (TH1D*)h_Item2_YX_peaktime1->ProjectionY("h_Item2_Y_Peaktime1", 1, h_Item2_YX_peaktime1->GetNbinsX() );
    h_Item2_Y_Peaktime1->Draw();
    c1->Print(Form("Figs/Item2_Y_Peak_time_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Y_Peak_time_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Y_Peak_time_%s.root", keyWord.c_str()));
    
    TH2D *h_Item2_YXC1 = (TH2D*)file_in->Get("h_Item2_YXC1");
    FormarHist_1(c1, h_Item2_YXC1);
    gStyle->SetOptStat(1111);
    h_Item2_YXC1->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item2_YXC1->SetMaximum( 5.*h_Item2_YXC1->GetEntries()/( h_Item2_YXC1->GetNbinsX()*h_Item2_YXC1->GetNbinsY() ) );
    h_Item2_YXC1->Draw();
    c1->Print(Form("Figs/Item2_YXc_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_YXc_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_YXc_%s.root", keyWord.c_str()));
    
    TH2D *h_Item2_YX_ADC1 = (TH2D*)file_in->Get("h_Item2_YX_ADC1");
    h_Item2_YX_ADC1->SetTitle("; ADC_X; ADC_Y");
    h_Item2_YX_ADC1->Draw();
    c1->Print(Form("Figs/Item2_ADC_YX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_ADC_YX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_ADC_YX_%s.root", keyWord.c_str()));
    
    TH1D *h_Item2_ADC_X = (TH1D*)h_Item2_YX_ADC1->ProjectionX("h_Item2_ADC_X", 1, h_Item2_YX_ADC1->GetNbinsY());
    h_Item2_ADC_X->SetAxisRange(0., adcMaxAxis);
    h_Item2_ADC_X->Draw();
    f_Landau->SetParameters(5. * h_Item2_ADC_X->GetMaximum(), h_Item2_ADC_X->GetBinCenter(h_Item2_ADC_X->GetMaximumBin()), 10);
    h_Item2_ADC_X->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    c1->Print(Form("Figs/Item2_ADC_X_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_ADC_X_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_ADC_X_%s.root", keyWord.c_str()));

    TH1D *h_Item2_ADC_Y = (TH1D*)h_Item2_YX_ADC1->ProjectionY("h_Item2_ADC_Y", 1, h_Item2_YX_ADC1->GetNbinsX());
    h_Item2_ADC_Y->SetAxisRange(0., adcMaxAxis);
    h_Item2_ADC_Y->Draw();
    f_Landau->SetParameters(5. * h_Item2_ADC_Y->GetMaximum(), h_Item2_ADC_Y->GetBinCenter(h_Item2_ADC_Y->GetMaximumBin()), 10);
    h_Item2_ADC_Y->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    c1->Print(Form("Figs/Item2_ADC_Y_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_ADC_Y_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_ADC_Y_%s.root", keyWord.c_str()));

    //x vs. y plot item 2 (sara)
    TH2D *h_n_Item2_Y_X_ClSize1 = (TH2D*)file_in->Get("h_n_Item2_Y_X_ClSize1");
    h_n_Item2_Y_X_ClSize1->SetTitle("; Clst_size X; Clst_size Y");
    h_n_Item2_Y_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item2_Clst_YvsX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Clst_YvsX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Clst_YvsX_%s.root", keyWord.c_str()));

    TH1D *h_n_Item2_X_ClSize1 = (TH1D*)h_n_Item2_Y_X_ClSize1->ProjectionX("h_n_Item2_X_ClSize1", 1, h_n_Item2_Y_X_ClSize1->GetNbinsY());
    h_n_Item2_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item2_X_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_X_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_X_ClSize_%s.root", keyWord.c_str()));

    TH1D *h_n_Item2_Y_ClSize1 = (TH1D*)h_n_Item2_Y_X_ClSize1->ProjectionY("h_n_Item2_Y_ClSize1", 1, h_n_Item2_Y_X_ClSize1->GetNbinsX());
    h_n_Item2_Y_ClSize1->Draw();
    c1->Print(Form("Figs/Item2_Y_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Y_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_Y_ClSize_%s.root", keyWord.c_str()));

    //sara
    TH2D *h_Item3_YX_peaktime1 = (TH2D*)file_in->Get("h_Item3_YX_peaktime1");
    h_Item3_YX_peaktime1->SetTitle("; X peak time sample; Y peak time sample");
    h_Item3_YX_peaktime1->Draw("");
    c1->Print(Form("Figs/Item3_Peak_time_YX1_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Peak_time_YX1_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Peak_time_YX1_%s.root", keyWord.c_str()));
    
    TH1D *h_Item3_X_Peaktime1 = (TH1D*)h_Item3_YX_peaktime1->ProjectionX("h_Item3_X_Peaktime1", 1, h_Item3_YX_peaktime1->GetNbinsY() );
    h_Item3_X_Peaktime1->Draw();
    c1->Print(Form("Figs/Item3_X_Peak_time_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_X_Peak_time_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_X_Peak_time_%s.root", keyWord.c_str()));
    
    TH1D *h_Item3_Y_Peaktime1 = (TH1D*)h_Item2_YX_peaktime1->ProjectionY("h_Item3_Y_Peaktime1", 1, h_Item3_YX_peaktime1->GetNbinsX() );
    h_Item3_Y_Peaktime1->Draw();
    c1->Print(Form("Figs/Item3_Y_Peak_time_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Y_Peak_time_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Y_Peak_time_%s.root", keyWord.c_str()));

    TH2D *h_Item3_YXC1 = (TH2D*)file_in->Get("h_Item3_YXC1");
    FormarHist_1(c1, h_Item3_YXC1);
    h_Item3_YXC1->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item3_YXC1->SetMaximum( 5.*h_Item3_YXC1->GetEntries()/( h_Item3_YXC1->GetNbinsX()*h_Item3_YXC1->GetNbinsY() ) );
    h_Item3_YXC1->Draw();
    c1->Print(Form("Figs/Item3_YXc_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_YXc_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_YXc_%s.root", keyWord.c_str()));
    
    TH2D *h_Item3_YX_ADC1 = (TH2D*)file_in->Get("h_Item3_YX_ADC1");
    h_Item3_YX_ADC1->SetTitle("; ADC_X; ADC_Y");
    h_Item3_YX_ADC1->Draw();
    c1->Print(Form("Figs/Item3_ADC_YX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_ADC_YX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_ADC_YX_%s.root", keyWord.c_str()));

    
    TH1D *h_Item3_ADC_X = (TH1D*)h_Item3_YX_ADC1->ProjectionX("h_Item3_ADC_X", 1, h_Item3_YX_ADC1->GetNbinsY());
    h_Item3_ADC_X->SetAxisRange(0., adcMaxAxis);
    h_Item3_ADC_X->Draw();
    f_Landau->SetParameters(5. * h_Item3_ADC_X->GetMaximum(), h_Item3_ADC_X->GetBinCenter(h_Item3_ADC_X->GetMaximumBin()), 10);
    h_Item3_ADC_X->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    c1->Print(Form("Figs/Item3_ADC_X_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_ADC_X_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_ADC_X_%s.root", keyWord.c_str()));

    TH1D *h_Item3_ADC_Y = (TH1D*)h_Item3_YX_ADC1->ProjectionY("h_Item3_ADC_Y", 1, h_Item3_YX_ADC1->GetNbinsX());
    h_Item3_ADC_Y->SetAxisRange(0., adcMaxAxis);
    h_Item3_ADC_Y->Draw();
    f_Landau->SetParameters(5. * h_Item3_ADC_Y->GetMaximum(), h_Item3_ADC_Y->GetBinCenter(h_Item3_ADC_Y->GetMaximumBin()), 10);
    h_Item3_ADC_Y->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    c1->Print(Form("Figs/Item3_ADC_Y_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_ADC_Y_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_ADC_Y_%s.root", keyWord.c_str()));

    //x vs. y plot item 3 (sara)
    TH2D *h_n_Item3_Y_X_ClSize1 = (TH2D*)file_in->Get("h_n_Item3_Y_X_ClSize1");
    h_n_Item3_Y_X_ClSize1->SetTitle("; Clst_size X; Clst_size Y");
    h_n_Item3_Y_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item3_Clst_YvsX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Clst_YvsX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Clst_YvsX_%s.root", keyWord.c_str()));

    TH1D *h_n_Item3_X_ClSize1 = (TH1D*)h_n_Item3_Y_X_ClSize1->ProjectionX("h_n_Item3_X_ClSize1", 1, h_n_Item3_Y_X_ClSize1->GetNbinsY());
    h_n_Item3_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item3_X_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_X_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_X_ClSize_%s.root", keyWord.c_str()));

    TH1D *h_n_Item3_Y_ClSize1 = (TH1D*)h_n_Item3_Y_X_ClSize1->ProjectionY("h_n_Item3_Y_ClSize1", 1, h_n_Item3_Y_X_ClSize1->GetNbinsX());
    h_n_Item3_Y_ClSize1->Draw();
    c1->Print(Form("Figs/Item3_Y_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Y_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_Y_ClSize_%s.root", keyWord.c_str()));

    //ITEM 4
    TH2D *h_Item4_YXC1 = (TH2D*)file_in->Get("h_Item4_YXC1");
    FormarHist_1(c1, h_Item4_YXC1);
    h_Item4_YXC1->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item4_YXC1->SetMaximum( 5.*h_Item4_YXC1->GetEntries()/( h_Item4_YXC1->GetNbinsX()*h_Item4_YXC1->GetNbinsY() ) );
    h_Item4_YXC1->Draw();
    c1->Print(Form("Figs/Item4_YXc_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_YXc_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_YXc_%s.root", keyWord.c_str()));
    
    TH2D *h_Item4_YX_ADC1 = (TH2D*)file_in->Get("h_Item4_YX_ADC1");
    h_Item4_YX_ADC1->SetTitle("; ADC_X; ADC_Y");
    h_Item4_YX_ADC1->Draw();
    c1->Print(Form("Figs/Item4_ADC_YX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_ADC_YX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_ADC_YX_%s.root", keyWord.c_str()));

    TH1D *h_Item4_ADC_X = (TH1D*)h_Item4_YX_ADC1->ProjectionX("h_Item4_ADC_X", 1, h_Item4_YX_ADC1->GetNbinsY());
    h_Item4_ADC_X->SetAxisRange(0., adcMaxAxis);
    h_Item4_ADC_X->Draw();
    f_Landau->SetParameters(5. * h_Item4_ADC_X->GetMaximum(), h_Item4_ADC_X->GetBinCenter(h_Item4_ADC_X->GetMaximumBin()), 10);
    h_Item4_ADC_X->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    c1->Print(Form("Figs/Item4_ADC_X_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_ADC_X_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_ADC_X_%s.root", keyWord.c_str()));

    TH1D *h_Item4_ADC_Y = (TH1D*)h_Item4_YX_ADC1->ProjectionY("h_Item4_ADC_Y", 1, h_Item4_YX_ADC1->GetNbinsX());
    h_Item4_ADC_Y->SetAxisRange(0., adcMaxAxis);
    h_Item4_ADC_Y->Draw();
    f_Landau->SetParameters(5. * h_Item4_ADC_Y->GetMaximum(), h_Item4_ADC_Y->GetBinCenter(h_Item4_ADC_Y->GetMaximumBin()), 10);
    h_Item4_ADC_Y->Fit( f_Landau, "MeV", "", 0., adcMaxAxis );
    c1->Print(Form("Figs/Item4_ADC_Y_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_ADC_Y_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_ADC_Y_%s.root", keyWord.c_str()));

    //x vs. y plot item 4 (sara)
    TH2D *h_n_Item4_Y_X_ClSize1 = (TH2D*)file_in->Get("h_n_Item4_Y_X_ClSize1");
    h_n_Item4_Y_X_ClSize1->SetTitle("; Clst_size X; Clst_size Y");
    h_n_Item4_Y_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item4_Clst_YvsX_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_Clst_YvsX_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_Clst_YvsX_%s.root", keyWord.c_str()));

    TH1D *h_n_Item4_X_ClSize1 = (TH1D*)h_n_Item4_Y_X_ClSize1->ProjectionX("h_n_Item4_X_ClSize1", 1, h_n_Item4_Y_X_ClSize1->GetNbinsY());
    h_n_Item4_X_ClSize1->Draw();
    c1->Print(Form("Figs/Item4_X_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_X_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_X_ClSize_%s.root", keyWord.c_str()));

    TH1D *h_n_Item4_Y_ClSize1 = (TH1D*)h_n_Item4_Y_X_ClSize1->ProjectionY("h_n_Item4_Y_ClSize1", 1, h_n_Item4_Y_X_ClSize1->GetNbinsX());
    h_n_Item4_Y_ClSize1->Draw();
    c1->Print(Form("Figs/Item4_Y_ClSize_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_Y_ClSize_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_Y_ClSize_%s.root", keyWord.c_str()));

//efficiency
    TH2D *h_totalTracks_Item1 = (TH2D*)file_in->Get("h_totalTracks_Item1");
    h_totalTracks_Item1->Draw();
    c1->Print(Form("Figs/Item1_totalTracks_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_totalTracks_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_totalTracks_%s.root", keyWord.c_str()));

    TH2D *h_matchedHits_Item1 = (TH2D*)file_in->Get("h_matchedHits_Item1");
    h_matchedHits_Item1->Draw();
    c1->Print(Form("Figs/Item1_matchedHits_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_matchedHits_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item1_matchedHits_%s.root", keyWord.c_str()));

    TH2D *h_totalTracks_Item2 = (TH2D*)file_in->Get("h_totalTracks_Item2");
    h_totalTracks_Item2->Draw();
    c1->Print(Form("Figs/Item2_totalTracks_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_totalTracks_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_totalTracks_%s.root", keyWord.c_str()));

    TH2D *h_matchedHits_Item2 = (TH2D*)file_in->Get("h_matchedHits_Item2");
    h_matchedHits_Item2->Draw();
    c1->Print(Form("Figs/Item2_matchedHits_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_matchedHits_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item2_matchedHits_%s.root", keyWord.c_str()));

    TH2D *h_totalTracks_Item3 = (TH2D*)file_in->Get("h_totalTracks_Item3");
    h_totalTracks_Item3->Draw();
    c1->Print(Form("Figs/Item3_totalTracks_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_totalTracks_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_totalTracks_%s.root", keyWord.c_str()));
    TH2D *h_matchedHits_Item3 = (TH2D*)file_in->Get("h_matchedHits_Item3");

    h_matchedHits_Item3->Draw();
    c1->Print(Form("Figs/Item3_matchedHits_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_matchedHits_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item3_matchedHits_%s.root", keyWord.c_str()));

    TH2D *h_totalTracks_Item4 = (TH2D*)file_in->Get("h_totalTracks_Item4");
    h_totalTracks_Item4->Draw();
    c1->Print(Form("Figs/Item4_totalTracks_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_totalTracks_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_totalTracks_%s.root", keyWord.c_str()));
    TH2D *h_matchedHits_Item4 = (TH2D*)file_in->Get("h_matchedHits_Item4");

    h_matchedHits_Item4->Draw();
    c1->Print(Form("Figs/Item4_matchedHits_%s.pdf", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_matchedHits_%s.png", keyWord.c_str()));
    c1->Print(Form("Figs/Item4_matchedHits_%s.root", keyWord.c_str()));

    TCanvas *c2 = new TCanvas("c2", "Efficiency Plots", 800, 400);
    c2->Divide(2, 2);

    c2->cd(1);
    TH2D *h_efficiency_Item1 = (TH2D*)h_matchedHits_Item1->Clone("h_efficiency_Item1");
    h_efficiency_Item1->SetTitle("Efficiency for Item1; X [cm]; Y [cm]");
    h_efficiency_Item1->Divide(h_totalTracks_Item1); // Element-wise division
    h_efficiency_Item1->Draw("COLZ");

    double totalTracks_item1 = h_totalTracks_Item1->Integral(); // Sum of all bins in total tracks histogram
    double matchedHits_item1 = h_matchedHits_Item1->Integral(); 
    double Efficiency_item1 = (totalTracks_item1 > 0) ? (matchedHits_item1 / totalTracks_item1) * 100.0 : 0.0;
    TPaveText *pt1 = new TPaveText(0.15, 0.85, 0.35, 0.9, "NDC"); // Position in normalized device coordinates
    pt1->SetFillColor(0);
    pt1->SetTextAlign(12);
    pt1->AddText(Form("Efficiency: %.2f%%", Efficiency_item1)); 
    pt1->Draw("same");

    c2->cd(2);
    TH2D *h_efficiency_Item2 = (TH2D*)h_matchedHits_Item2->Clone("h_efficiency_Item2");
    h_efficiency_Item2->SetTitle("Efficiency for Item2; X [cm]; Y [cm]");
    h_efficiency_Item2->Divide(h_totalTracks_Item2); 
    h_efficiency_Item2->Draw("COLZ");

    double totalTracks_item2 = h_totalTracks_Item2->Integral(); 
    double matchedHits_item2 = h_matchedHits_Item2->Integral(); 
    double Efficiency_item2 = (totalTracks_item2 > 0) ? (matchedHits_item2 / totalTracks_item2) * 100.0 : 0.0;
    TPaveText *pt2 = new TPaveText(0.15, 0.85, 0.35, 0.9, "NDC");
    pt2->SetFillColor(0);
    pt2->SetTextAlign(12); 
    pt2->AddText(Form("Efficiency: %.2f%%", Efficiency_item2));
    pt2->Draw("same");
  
    c2->cd(3);
    TH2D *h_efficiency_Item3 = (TH2D*)h_matchedHits_Item3->Clone("h_efficiency_Item3");
    h_efficiency_Item3->SetTitle("Efficiency for Item3; X [cm]; Y [cm]");
    h_efficiency_Item3->Divide(h_totalTracks_Item3); 
    h_efficiency_Item3->Draw("COLZ");

    double totalTracks_item3 = h_totalTracks_Item3->Integral(); 
    double matchedHits_item3 = h_matchedHits_Item3->Integral(); 
    double Efficiency_item3 = (totalTracks_item3 > 0) ? (matchedHits_item3 / totalTracks_item3) * 100.0 : 0.0;
    TPaveText *pt3 = new TPaveText(0.15, 0.85, 0.35, 0.9, "NDC");
    pt3->SetFillColor(0);
    pt3->SetTextAlign(12); 
    pt3->AddText(Form("Efficiency: %.2f%%", Efficiency_item3)); 
    pt3->Draw("same");

    c2->cd(4);
    TH2D *h_efficiency_Item4 = (TH2D*)h_matchedHits_Item4->Clone("h_efficiency_Item4");
    h_efficiency_Item4->SetTitle("Efficiency for Item4; X [cm]; Y [cm]");
    h_efficiency_Item4->Divide(h_totalTracks_Item4); 
    h_efficiency_Item4->Draw("COLZ");

    double totalTracks_item4 = h_totalTracks_Item4->Integral(); 
    double matchedHits_item4 = h_matchedHits_Item4->Integral(); 
    double Efficiency_item4 = (totalTracks_item4 > 0) ? (matchedHits_item4 / totalTracks_item4) * 100.0 : 0.0;
    TPaveText *pt4 = new TPaveText(0.15, 0.85, 0.35, 0.9, "NDC");
    pt4->SetFillColor(0);
    pt4->SetTextAlign(12); 
    pt4->AddText(Form("Efficiency: %.2f%%", Efficiency_item4)); 
    pt4->Draw("same");

    c2->Print(Form("Figs/Efficiency_Plots%s.pdf", keyWord.c_str()));
    c2->Print(Form("Figs/Efficiency_Plots%s.png", keyWord.c_str()));
    c2->Print(Form("Figs/Efficiency_Plots%s.root", keyWord.c_str()));

TCanvas *c3 = new TCanvas("c3", "ADC and Cluster Plots", 1200, 800);
    c3->Divide(3, 2);
    c3->cd(1);
    h_Item1_YX_ADC1->Draw();

    c3->cd(2);
    h_Item2_YX_ADC1->Draw();

    c3->cd(3);
    h_Item3_YX_ADC1->Draw();

    c3->cd(4);
    h_Item1_YXC1->Draw();

    c3->cd(5);
    h_Item2_YXC1->Draw();

    c3->cd(6);
    h_Item3_YXC1->Draw();
    c3->Print(Form("Figs/ADC_Cluster_Plots%s.pdf", keyWord.c_str()));

    TCanvas *c4 = new TCanvas("c4", "Tracking Plots", 1200, 800);
    c4->Divide(3, 2);
    c4->cd(1);
    h_totalTracks_Item1->Draw();

    c4->cd(2);
    h_totalTracks_Item2->Draw();

    c4->cd(3);
    h_totalTracks_Item3->Draw();

    c4->cd(4);
    h_matchedHits_Item1->Draw();

    c4->cd(5);
    h_matchedHits_Item2->Draw();

    c4->cd(6);
    h_matchedHits_Item3->Draw();
    c4->Print(Form("Figs/Tracking_Plots%s.pdf", keyWord.c_str()));

    TCanvas *c5 = new TCanvas("c5", "Residual Plots", 1200, 800);
    c5->Divide(3, 2);
    c5->cd(1);
    TH1D *h_Item1_X_residual = (TH1D*)file_in->Get("h_Item1_X_residual");
    h_Item1_X_residual->Draw();

    c5->cd(2);
    TH1D *h_Item2_X_residual = (TH1D*)file_in->Get("h_Item2_X_residual");
    h_Item2_X_residual->Draw();

    c5->cd(3);
    //TH1D *h_Item3_X_residual = (TH1D*)file_in->Get("h_Item3_X_residual");
    //h_Item3_X_residual->Draw();
    TH1D *h_Item4_X_residual = (TH1D*)file_in->Get("h_Item4_X_residual");
    h_Item4_X_residual->Draw();

    c5->cd(4);
    TH1D *h_Item1_Y_residual = (TH1D*)file_in->Get("h_Item1_Y_residual");
    h_Item1_Y_residual->Draw();

    c5->cd(5);
    TH1D *h_Item2_Y_residual = (TH1D*)file_in->Get("h_Item2_Y_residual");
    h_Item2_Y_residual->Draw();

    c5->cd(6);
    //TH1D *h_Item3_Y_residual = (TH1D*)file_in->Get("h_Item3_Y_residual");
    //h_Item3_Y_residual->Draw();
    TH1D *h_Item4_Y_residual = (TH1D*)file_in->Get("h_Item4_Y_residual");
    h_Item4_Y_residual->Draw();

    c5->Print(Form("Figs/Residual_Plots%s.pdf", keyWord.c_str()));

//Filtered plots
TCanvas *ch = new TCanvas("ch", "Filtered Plots", 1200, 1200);
    ch->Divide(3, 3);
    ch->cd(1);
    TH2D *h_Item1_YX_ADC_Filtered = (TH2D*)file_in->Get("h_Item1_YX_ADC_Filtered");
    h_Item1_YX_ADC_Filtered->SetTitle("; ADC_X; ADC_Y");
    h_Item1_YX_ADC_Filtered->Draw();

    ch->cd(2);
    TH2D *h_Item2_YX_ADC_Filtered = (TH2D*)file_in->Get("h_Item2_YX_ADC_Filtered");
    h_Item2_YX_ADC_Filtered->SetTitle("; ADC_X; ADC_Y");
    h_Item2_YX_ADC_Filtered->Draw();

    ch->cd(3);
    TH2D *h_Item3_YX_ADC_Filtered = (TH2D*)file_in->Get("h_Item3_YX_ADC_Filtered");
    h_Item3_YX_ADC_Filtered->SetTitle("; ADC_X; ADC_Y");
    h_Item3_YX_ADC_Filtered->Draw();

    ch->cd(4);
    TH2D *h_Item1_YXC_Filtered = (TH2D*)file_in->Get("h_Item1_YXC_Filtered");
    h_Item1_YXC_Filtered->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item1_YXC_Filtered->Draw();

    ch->cd(5);
    TH2D *h_Item2_YXC_Filtered = (TH2D*)file_in->Get("h_Item2_YXC_Filtered");
    h_Item2_YXC_Filtered->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item2_YXC_Filtered->Draw();

    ch->cd(6);
    TH2D *h_Item3_YXC_Filtered = (TH2D*)file_in->Get("h_Item3_YXC_Filtered");
    h_Item3_YXC_Filtered->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item3_YXC_Filtered->Draw();

    ch->cd(7);
    //TH2D *h_Item1_YXC_residualFiltered = (TH2D*)file_in->Get("h_Item1_YXC_residualFiltered");
    TH2D *h_GEM0_YXC_residualFiltered = (TH2D*)file_in->Get("h_GEM0_YXC_residualFiltered");
    h_GEM0_YXC_residualFiltered->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_GEM0_YXC_residualFiltered->Draw();

    ch->cd(8);
    TH2D *h_Item2_YXC_residualFiltered = (TH2D*)file_in->Get("h_Item2_YXC_residualFiltered");
    h_Item2_YXC_residualFiltered->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_Item2_YXC_residualFiltered->Draw();

    ch->cd(9);
    //TH2D *h_Item3_YXC_residualFiltered = (TH2D*)file_in->Get("h_Item3_YXC_residualFiltered");
    TH2D *h_GEM1_YXC_residualFiltered = (TH2D*)file_in->Get("h_GEM1_YXC_residualFiltered");
    h_GEM1_YXC_residualFiltered->SetTitle(";Cluster X [cm]; Cluster Y [cm]");
    h_GEM1_YXC_residualFiltered->Draw();


    ch->Print(Form("Figs/ADC_Filtered%s.pdf", keyWord.c_str()));
 
    



    return 0;
}

template <typename T>
void FormarHist_1(TCanvas *c, T h){

    h->SetLineWidth(2);
    c->SetBottomMargin(0.12);
    h->GetXaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetTitleSize(0.06);
    c->SetLeftMargin(0.14);
    h->GetYaxis()->SetLabelSize(0.06);
    h->GetYaxis()->SetLabelOffset(0.01);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitleOffset(1.1);
    
}
