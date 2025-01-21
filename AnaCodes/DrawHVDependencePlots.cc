/* 
 * File:   DrawHVDependecePlots.cc
 *
 * Created on January 13, 2024, 11:22 AM
 */

#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <cxxopts.hpp>

#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TEfficiency.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>

//#include <uRwellTools.h>

using namespace std;


int main(int argc, char **argv) {

    cxxopts::Options options("DrawHVDependencePlots", "Draws plots of efficiencies (or others staff) as a function of HV");

    options.add_options()
            ("s,Series", "HV Scan series", cxxopts::value<int>())
            ("t,Threshold", "Hit Threshold in terms of sigma", cxxopts::value<double>())
            ("m,MinHits", "Number of minimum hits in the cluster", cxxopts::value<int>())
            ;

    auto parsed_options = options.parse(argc, argv);

    if (!parsed_options.count("Series")) {
        cout << "The Series of runs is nor provided. Exiting..." << endl;
        exit(1);
    }
    const int series = parsed_options["Series"].as<int>();


    if (!parsed_options.count("Threshold")) {
        cout << "* You didn't provide the hit threshold. Exiting" << endl;
        exit(1);
    }
    const double threshold = parsed_options["Threshold"].as<double>();

     
    if (!parsed_options.count("MinHits")) {
        cout << "* You didn't provide the Minimum hits int the cluster. Exiting" << endl;
    }
    const int MinClSize = parsed_options["MinHits"].as<int>();

    cout << "The hit threshold is " << threshold << "\\sigma" << endl;
    cout << "The Minimum cluster size is " << MinClSize << "hits" << endl;

    std::map<int, std::vector<int> > mv_runs;
    mv_runs[1] = {2560, 2558, 2556, 2553, 2549, 2547};

    //std::map<int, int> mv_runs;
    std::map<int, double> m_MESH_HV; // The key is the run number, the value is the MESH_HV
    std::map<int, double> m_Cathode_HV; // The key is the run number, the value is the Cathode_HV
    std::map<int, double> m_Drift_HV; // The key is the run number, the value is the Drift_HV = Hathode_HV - MESH_HV

    std::map<int, std::string> m_HVType;
    m_HVType[1] = "MESH";
    m_HVType[2] = "CATHODE";
    m_HVType[3] = "DRIFT";

    std::map<int, std::string> m_XTitle;
    m_XTitle[1] = "MESH HV [V]";
    m_XTitle[2] = "CATHODE HV [V]";
    m_XTitle[3] = "DRIFT HV [V]";



    std::string hvTablefileName = Form("HV_Table_%d.dat", series);
        ifstream inp_HVTable(hvTablefileName.c_str());

        if (inp_HVTable.is_open()) {

            while (!inp_HVTable.eof()) {
                int run, HV_MESH, HV_Cathode, HV_GEM, HV_Drift;

                inp_HVTable >> run >> HV_MESH >> HV_Cathode;

                cout << run << "  " << HV_MESH << "   " << HV_Cathode << endl;
                m_MESH_HV[run] = HV_MESH;
                m_Cathode_HV[run] = HV_Cathode;
                m_Drift_HV[run] = HV_Cathode - HV_MESH;
            }

        } else {
            cout << "Can not open the file" << hvTablefileName.c_str() << endl;
        }  

        TGraph *effGraph_Item1 = new TGraph();
        TGraph *effGraph_Item2 = new TGraph();
        TGraph *effGraph_Item3 = new TGraph();
        
    for (int i = 0; i < mv_runs[series].size(); i++) {

            int run = mv_runs[series].at(i);

            TFile *file_in = new TFile(Form("AnaData_%d_Thr_%1.1f_MinHits_%d.root", run, threshold, MinClSize), "Read");

            double HV_Value;

            if (strcmp(m_HVType[series].c_str(), "MESH") == 0) {
                HV_Value = m_MESH_HV[run];
            } else if (strcmp(m_HVType[series].c_str(), "DRIFT") == 0) {
                HV_Value = m_Drift_HV[run];
            } else if (strcmp(m_HVType[series].c_str(), "CATHODE") == 0) {
                HV_Value = m_Cathode_HV[run];
            } else if (strcmp(m_HVType[series].c_str(), "FILE_IND") == 0) {
                HV_Value = m_Cathode_HV[run];
            }

            TH2D *h_totalTracks_Item1 = (TH2D*)file_in->Get("h_totalTracks_Item1");
            TH2D *h_matchedHits_Item1 = (TH2D*)file_in->Get("h_matchedHits_Item1");
            
            TH2D *h_totalTracks_Item2 = (TH2D*)file_in->Get("h_totalTracks_Item2");
            TH2D *h_matchedHits_Item2 = (TH2D*)file_in->Get("h_matchedHits_Item2");

            TH2D *h_totalTracks_Item3 = (TH2D*)file_in->Get("h_totalTracks_Item3");
            TH2D *h_matchedHits_Item3 = (TH2D*)file_in->Get("h_matchedHits_Item3");

        

        double eff_Item1 = h_matchedHits_Item1->Integral() / h_totalTracks_Item1->Integral();
        double eff_Item2 = h_matchedHits_Item2->Integral() / h_totalTracks_Item2->Integral();
        double eff_Item3 = h_matchedHits_Item3->Integral() / h_totalTracks_Item3->Integral();

        effGraph_Item1->SetPoint(i, HV_Value, eff_Item1 * 100.0);
        effGraph_Item2->SetPoint(i, HV_Value, eff_Item2 * 100.0);
        effGraph_Item3->SetPoint(i, HV_Value, eff_Item3 * 100.0);

        file_in->Close();
        delete file_in;
    }    


    TCanvas *c2 = new TCanvas("c2", "Efficiency vs HV", 1200, 400);
    c2->Divide(3, 1);

    c2->cd(1);
    effGraph_Item1->SetTitle("Efficiency vs HV (Item1); HV [V]; Efficiency [%]");
    effGraph_Item1->Draw("ALP");

    c2->cd(2);
    effGraph_Item2->SetTitle("Efficiency vs HV (Item2); HV [V]; Efficiency [%]");
    effGraph_Item2->Draw("ALP");

    c2->cd(3);
    effGraph_Item3->SetTitle("Efficiency vs HV (Item3); HV [V]; Efficiency [%]");
    effGraph_Item3->Draw("ALP");

    c2->Print("Efficiency_vs_HV.pdf");

    delete effGraph_Item1;
    delete effGraph_Item2;
    delete effGraph_Item3;
    

 return 0;

}