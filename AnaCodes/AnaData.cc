/* 
 * File:   AnaData.cc
 * Author: rafopar
 *
 * Created on June 27, 2024, 5:38 PM
 */

#include <cstdlib>
#include <TH2D.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

#include <uRwellLDRDTools.h>
#include <cxxopts.hpp>

using namespace std;
using namespace uRwellLDRDTools;

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    char outputFile[256];
    char inputFile[256];

    cxxopts::Options options("AnaClustering", "Performs clustering and also does analysis on cosmic data");

    options.add_options()
            ("r,Run", "Run number", cxxopts::value<int>())
            ("t,Threshold", "Hit Threshold in terms of sigma", cxxopts::value<double>()->default_value("5"))
            ("m,MinHits", "Number of minimum hits in the cluster", cxxopts::value<int>()->default_value("1"))
            //("f,File", "File number of the given run", cxxopts::value<int>())
            ;

    auto parsed_options = options.parse(argc, argv);

    int run = 0;
    int fnum = -1;


    if (parsed_options.count("Run")) {
        run = parsed_options["Run"].as<int>();
        sprintf(inputFile, "Skim_ZeroSuppr_%d_All.hipo", run);
    } else {
        cout << "The run number is nor provided. Exiting..." << endl;
        exit(1);
    }


    const double HitThr = parsed_options["Threshold"].as<double>();
    if (!parsed_options.count("Threshold")) {
        cout << "* You didn't provide the hit threshold, will use the default value " << HitThr << endl;
    }

    const int MinClSize = parsed_options["MinHits"].as<int>();
    if (!parsed_options.count("MinHits")) {
        cout << "* You didn't provide the Minimum hits int the cluster, so will use the default value " << MinClSize << endl;
    }

    cout << "The hit threshold is " << HitThr << "\\sigma" << endl;
    cout << "The Minimum cluster size is " << MinClSize << "hits" << endl;

    hipo::reader reader;
    reader.open(inputFile);

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();
    hipo::event event;
    int evCounter = 0;

    hipo::bank buRwellHit(factory.getSchema("uRwell::Hit"));
    hipo::bank bRAWADc(factory.getSchema("RAW::adc"));
    hipo::bank bRunConf(factory.getSchema("RUN::config"));


    TFile *file_out = new TFile(Form("AnaData_%d_Thr_%1.1f_MinHits_%d.root", run, HitThr, MinClSize), "Recreate");

    TH2D h_n_GEM0_Yvs_X_Hits1("h_n_GEM0_Yvs_X_Hits1", "", 11, -0.5, 10.5, 11, -0.5, 10.5);
    TH2D h_n_GEM1_Yvs_X_Hits1("h_n_GEM1_Yvs_X_Hits1", "", 11, -0.5, 10.5, 11, -0.5, 10.5);

    TH1D h_GEM0_X_ADC1("h_GEM0_X_ADC1", "", 200, 0., 1600);
    TH1D h_GEM0_Y_ADC1("h_GEM0_Y_ADC1", "", 200, 0., 1600);
    TH1D h_GEM1_X_ADC1("h_GEM1_X_ADC1", "", 200, 0., 1600);
    TH1D h_GEM1_Y_ADC1("h_GEM1_Y_ADC1", "", 200, 0., 1600);

    TH1D h_GEM0_X_ADC2("h_GEM0_X_ADC2", "", 200, 0., 1600);
    TH1D h_GEM0_Y_ADC2("h_GEM0_Y_ADC2", "", 200, 0., 1600);
    TH1D h_GEM1_X_ADC2("h_GEM1_X_ADC2", "", 200, 0., 1600);
    TH1D h_GEM1_Y_ADC2("h_GEM1_Y_ADC2", "", 200, 0., 1600);

    TH2D h_GEM0_YXC1("h_GEM0_YXC1", "", 200, -0.1, 10.1, 200, -0.1, 10.1);
    TH2D h_GEM1_YXC1("h_GEM1_YXC1", "", 200, -0.1, 10.1, 200, -0.1, 10.1);

    TH2D h_GEM0_YX_peaktime1("h_GEM0_YX_peaktime1", "", 10, -0.5, 9.5, 10, -0.5, 9.5);
    TH2D h_GEM1_YX_peaktime1("h_GEM1_YX_peaktime1", "", 10, -0.5, 9.5, 10, -0.5, 9.5);

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            //if( evCounter > 2000 ){break;}
            if (evCounter % 1000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(buRwellHit);
            event.getStructure(bRAWADc);
            event.getStructure(bRunConf);

            int n_Hits = buRwellHit.getRows();

            vector<uRwellHit> v_GEM0_YHits;
            vector<uRwellHit> v_GEM0_XHits;
            vector<uRwellHit> v_GEM1_YHits;
            vector<uRwellHit> v_GEM1_XHits;

            for (int ihit = 0; ihit < n_Hits; ihit++) {
                uRwellHit curHit;
                curHit.sector = buRwellHit.getInt("sec", ihit);
                curHit.layer = buRwellHit.getInt("layer", ihit);
                curHit.strip = buRwellHit.getInt("strip", ihit);
                curHit.adc = double(buRwellHit.getFloat("adc", ihit));
                curHit.adcRel = double(buRwellHit.getFloat("adcRel", ihit));
                curHit.ts = buRwellHit.getInt("ts", ihit);

                if (curHit.sector == det_GEM0) {

                    if (curHit.layer == 1) {
                        v_GEM0_XHits.push_back(curHit);
                    } else {
                        v_GEM0_YHits.push_back(curHit);
                    }
                } else if (curHit.sector == det_GEM1) {
                    if (curHit.layer == 1) {
                        v_GEM1_XHits.push_back(curHit);
                    } else {
                        v_GEM1_YHits.push_back(curHit);
                    }
                }



            }


            vector< uRwellCluster > v_GEM0_Xclusters = uRwellLDRDTools::getClusters(v_GEM0_XHits);
            vector< uRwellCluster > v_GEM0_Yclusters = uRwellLDRDTools::getClusters(v_GEM0_YHits);
            vector< uRwellCluster > v_GEM1_Xclusters = uRwellLDRDTools::getClusters(v_GEM1_XHits);
            vector< uRwellCluster > v_GEM1_Yclusters = uRwellLDRDTools::getClusters(v_GEM1_YHits);

            h_n_GEM0_Yvs_X_Hits1.Fill(v_GEM0_Xclusters.size(), v_GEM0_Yclusters.size());
            h_n_GEM1_Yvs_X_Hits1.Fill(v_GEM1_Xclusters.size(), v_GEM1_Yclusters.size());


            uRwellCluster GEM0_XCluster = getMaxAdcCluster(v_GEM0_Xclusters, MinClSize);
            uRwellCluster GEM0_YCluster = getMaxAdcCluster(v_GEM0_Yclusters, MinClSize);
            uRwellCluster GEM1_XCluster = getMaxAdcCluster(v_GEM1_Xclusters, MinClSize);
            uRwellCluster GEM1_YCluster = getMaxAdcCluster(v_GEM1_Yclusters, MinClSize);

            if (GEM0_XCluster.getHits()->size() > 0 && GEM0_YCluster.getHits()->size() > 0) {

                double GEM0_X = GEM0_XCluster.getAvgStrip() * uRwellLDRDTools::GEM_Strip2Coord;
                double GEM0_Y = GEM0_YCluster.getAvgStrip() * uRwellLDRDTools::GEM_Strip2Coord;

                h_GEM0_X_ADC1.Fill(GEM0_XCluster.getPeakADC());
                h_GEM0_Y_ADC1.Fill(GEM0_YCluster.getPeakADC());
                h_GEM0_YXC1.Fill(GEM0_X, GEM0_Y);
                h_GEM0_YX_peaktime1.Fill(GEM0_XCluster.getPeakTime(), GEM0_YCluster.getPeakTime());
            }


            if (GEM1_XCluster.getHits()->size() > 0 && GEM1_YCluster.getHits()->size() > 0) {
                double GEM1_X = GEM1_XCluster.getAvgStrip() * uRwellLDRDTools::GEM_Strip2Coord;
                double GEM1_Y = GEM1_YCluster.getAvgStrip() * uRwellLDRDTools::GEM_Strip2Coord;

                h_GEM1_X_ADC1.Fill(GEM1_XCluster.getPeakADC());
                h_GEM1_Y_ADC1.Fill(GEM1_YCluster.getPeakADC());
                h_GEM1_YXC1.Fill(GEM1_X, GEM1_Y);
                h_GEM1_YX_peaktime1.Fill(GEM1_XCluster.getPeakTime(), GEM1_YCluster.getPeakTime());
            }


            if (GEM0_XCluster.getHits()->size() > 0 && GEM0_YCluster.getHits()->size() > 0 && GEM1_XCluster.getHits()->size() > 0 && GEM1_YCluster.getHits()->size() > 0) {
                h_GEM0_X_ADC2.Fill(GEM0_XCluster.getPeakADC());
                h_GEM0_Y_ADC2.Fill(GEM0_YCluster.getPeakADC());
                h_GEM1_X_ADC2.Fill(GEM1_XCluster.getPeakADC());
                h_GEM1_Y_ADC2.Fill(GEM1_YCluster.getPeakADC());
            }
        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    gDirectory->Write();
    return 0;
}