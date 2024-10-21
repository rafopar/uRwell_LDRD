/* 
 * File:   CalculatePedestals.cc
 * Author: rafopar
 *
 * Created on June 27, 2024, 1:16 PM
 */

#include <cstdlib>
#include <iomanip>

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

using namespace std;

int getSlot(int sector, int layer, int component);

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 2) {

        cout << "You should provide the run number." << endl;
        cout << "Exiting" << endl;
        exit(1);
    }

    int run = atoi(argv[1]);

    char inputFile[256];
    sprintf(inputFile, "Data/decoded_%d_0.hipo", run); // We usually need about 2K events, so the 1st file is plenty for calculating pedestals.

    const int nMaxEventsToProcess = 2000;
    const int MaxChPerDet = 512; // No detector has more than 512 channels
    const int MaxChePerLayer = 256; // No detector has more than 256 channels per layer
    const int n_ts = 9;
    const int n_MaxDet = 6; // The maximum number of detectors
    const int n_MaxUniqueChannels = n_MaxDet*MaxChPerDet;
    const int nSlot = 16;
    const int nCh_PerSlot = 128;


    TFile *file_out = new TFile(Form("Pedestals_%d.root", run), "Recreate");

    TH2D hADC_UniqueChan1("hADC_UniqueChan1", "", n_MaxUniqueChannels + 1, -0.5, n_MaxUniqueChannels + 0.5, 400, -400., 400.);

    TH2D h_ADC_Slot_[nSlot];
    TGraph gr_ADC_mean_[nSlot];
    TGraph gr_ADC_rms_[nSlot];

    for (int i = 0; i < nSlot; i++) {
        h_ADC_Slot_[i] = TH2D(Form("ADC_Slot_%d", i), "", 128, -0.5, 127.5, 400, -400., 400.);
        gr_ADC_mean_[i] = TGraph();
        gr_ADC_rms_[i] = TGraph();
    }

    hipo::reader reader;
    reader.open(inputFile);

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();
    hipo::event event;
    int evCounter = 0;

    hipo::bank buRWellADC(factory.getSchema("URWELL::adc"));
    hipo::bank bRAWADc(factory.getSchema("RAW::adc"));
    hipo::bank bRunConf(factory.getSchema("RUN::config"));

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            if (evCounter > 2000) {
                break;
            }
            if (evCounter % 500 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(buRWellADC);
            event.getStructure(bRAWADc);
            event.getStructure(bRunConf);

            int __bank_Sec_INDEX_ = buRWellADC.getSchema().getEntryOrder("sector");
            int __bank_Layer_INDEX_ = buRWellADC.getSchema().getEntryOrder("layer");
            int __bank_Component_INDEX_ = buRWellADC.getSchema().getEntryOrder("component");
            int __bank_Order_INDEX_ = buRWellADC.getSchema().getEntryOrder("order");
            int __bank_ADC_INDEX_ = buRWellADC.getSchema().getEntryOrder("ADC");
            int __bank_Time_INDEX_ = buRWellADC.getSchema().getEntryOrder("time");
            int __bank_Ped_INDEX_ = buRWellADC.getSchema().getEntryOrder("ped");

            int n_ADC = buRWellADC.getRows();

            std::map< int, int > m_ADC_[n_ts]; // The key of the map is unique_channel, and the value is the ADC value
            std::map<int, int> m_Slot_ADC_[nSlot][n_ts];

            for (int i = 0; i < n_ADC; i++) {
                int sector = buRWellADC.getInt(__bank_Sec_INDEX_, i);
                int layer = buRWellADC.getInt(__bank_Layer_INDEX_, i);
                int channel = buRWellADC.getInt(__bank_Component_INDEX_, i);
                int ADC = buRWellADC.getInt(__bank_ADC_INDEX_, i);
                int ts = buRWellADC.getInt(__bank_Ped_INDEX_, i);

                int uniqueChan = sector * MaxChPerDet + (layer - 1) * MaxChePerLayer + channel;

                int slot = getSlot(sector, layer, channel);
                int chLocal = (channel - 1) % 128; // chLocal is the local channel in the Hybrid, not in the detector

                //cout<<"Sec = "<<sector<<"    layer = "<<layer<<"    channel = "<<channel<<"    slot = "<<slot<<"    ts = "<<ts<<endl;

                m_Slot_ADC_[slot][ts][chLocal] = ADC;
                m_ADC_[ts][uniqueChan] = ADC;
            }

            for (auto map0 : m_ADC_[0]) {
                int ch = map0.first;
                //double avg_ADC = (m_ADC_[0][ch] + m_ADC_[1][ch] + m_ADC_[2][ch]) / 3.;
                double avg_ADC = 0.;
                for (int i_ts = 0; i_ts < n_ts; i_ts++) {
                    avg_ADC = avg_ADC + m_ADC_[i_ts][ch];
                }
                avg_ADC = avg_ADC / double(n_ts);
                hADC_UniqueChan1.Fill(ch, avg_ADC);

            }

            for (int isl = 0; isl < nSlot; isl++) {
                for (int ch = 0; ch < nCh_PerSlot; ch++) {

                    if (m_Slot_ADC_[isl][0].find(ch) == m_Slot_ADC_[isl][0].end()) {
                        continue;
                    }

                    double avg_ADC = 0;
                    for (int its = 0; its < n_ts; its++) {
                        avg_ADC = avg_ADC + m_Slot_ADC_[isl][its][ch];
                    }
                    avg_ADC = avg_ADC / double(n_ts);
                    h_ADC_Slot_[isl].Fill(ch, avg_ADC);
                }
            }
            if (nMaxEventsToProcess > 2000) {
                break;
            }

        }

        std::string pedFileName = Form("PedFiles/Pedestals_%d.dat", run);

        cout << "********************************************************** " << endl;
        cout << "*                                                          " << endl;
        cout << "*                  Done looping over events                " << endl;
        cout << "*                                                          " << endl;
        cout << "*                    Calculating pedestals                 " << endl;
        cout << "********************************************************** " << endl;
        cout << "*                                                          " << endl;
        cout << "*  Generating the pedestal file " << pedFileName.c_str() << "  " << endl;
        /*
         * Done looping over events, now let's calculate pedestals foe each channel and put
         * values in the pedestal file.
         */

        ofstream out_dat(pedFileName.c_str());
        for (int i = 0; i < hADC_UniqueChan1.GetNbinsX(); i++) {
            TH1D *h_curHist = (TH1D*) hADC_UniqueChan1.ProjectionY(Form("ADC_%d", i + 1), i + 1, i + 1);
            double mean_val = h_curHist->GetEntries() > 0 ? h_curHist->GetBinCenter(h_curHist->GetMaximumBin()) : 0;
            double rms = h_curHist->GetEntries() > 0 ? h_curHist->GetRMS() : 100000; // Just very big number to indicate RMS is not calculated for this channel
            out_dat << setw(5) << i << setw(9) << mean_val << setw(9) << rms << endl;
        }

        
        cout << "********************************************************** " << endl;
        cout << "*                                                          " << endl;
        cout << "*                  Pedestal calculation is done            " << endl;
        cout << "*                                                          " << endl;
        cout << "*                       Generating plots                   " << endl;
        cout << "********************************************************** " << endl;


        TCanvas *c1 = new TCanvas("c1", "", 1800, 900);
        c1->SetRightMargin(0.02);
        c1->SetGridx();
        c1->SetGridy();
        //c1->SetTopMargin(0.02);
        for( int isl = 0; isl < nSlot; isl++ ){
            
            // Don't draw anything that is empty
            if( h_ADC_Slot_[isl].GetEntries() < 1 ){
                continue;
            }
            h_ADC_Slot_[isl].SetStats(0);
            h_ADC_Slot_[isl].SetTitle(Form("Slot %d; Channel local; ADC", isl));
            h_ADC_Slot_[isl].Draw();
            c1->Print(Form("Figs/Pedestals_ADC_Slot_%d_Run_%d.pdf", isl, run));
            c1->Print(Form("Figs/Pedestals_ADC_Slot_%d_Run_%d.png", isl, run));
            c1->Print(Form("Figs/Pedestals_ADC_Slot_%d_Run_%d.root", isl, run));
            
            for( int ich = 0; ich < nCh_PerSlot; ich++ ){
                int bin = h_ADC_Slot_[isl].GetXaxis()->FindBin(ich);
                
                TH1D *h_tmp = (TH1D*)h_ADC_Slot_[isl].ProjectionY(Form("h_tmp_%d", nCh_PerSlot*isl + ich), bin, bin);
                double mean = h_tmp->GetMean();
                double rms = h_tmp->GetRMS();
                gr_ADC_mean_[isl].SetPoint(ich, ich, mean);
                gr_ADC_rms_[isl].SetPoint(ich, ich, rms);
            }
            
            gr_ADC_mean_[isl].SetMarkerColor(2);
            gr_ADC_mean_[isl].SetMarkerStyle(21);
            gr_ADC_mean_[isl].SetTitle(Form("Slot %d; channel local; ADC mean", isl));
            gr_ADC_mean_[isl].Draw("APL");
            c1->Print(Form("Figs/Pedestals_ADC_Mean_Slot_%d_Run_%d.pdf", isl, run));
            c1->Print(Form("Figs/Pedestals_ADC_Mean_Slot_%d_Run_%d.png", isl, run));
            c1->Print(Form("Figs/Pedestals_ADC_Mean_Slot_%d_Run_%d.root", isl, run));

            c1->Clear();
            gr_ADC_rms_[isl].SetMarkerColor(4);
            gr_ADC_rms_[isl].SetMarkerStyle(20);
            gr_ADC_rms_[isl].SetTitle(Form("Slot %d; channel local; ADC RMS", isl));
            gr_ADC_rms_[isl].Draw("APL");
            c1->Print(Form("Figs/Pedestals_ADC_RMS_Slot_%d_Run_%d.pdf", isl, run));
            c1->Print(Form("Figs/Pedestals_ADC_RMS_Slot_%d_Run_%d.png", isl, run));
            c1->Print(Form("Figs/Pedestals_ADC_RMS_Slot_%d_Run_%d.root", isl, run));
        }
        
    } catch (const char msg) {
        cerr << msg << endl;
    }

    
    
    gDirectory->Write();
    file_out->Close();

    return 0;
}

/*
 * This is using the informatin from the TT
 */
int getSlot(int sector, int layer, int component) {

    int slot = -1;

    if (sector == 0) {
        // GEM0        
        slot = 2 * (layer - 1) + (component - 1) / 128;
    } else if (sector == 1) {
        // GEM1
        slot = 4 + 2 * (layer - 1) + (component - 1) / 128;
    } else if (sector == 2) {
        // Item1
        slot = 8 + layer - 1;
    } else if (sector == 3) {
        // Item2
        slot = 10 + layer - 1;
    } else if (sector == 4) {
        // Item3
        slot = 12 + layer - 1;
    } else if (sector == 5) {
        // Item4
        // *** Note Item4 has three layers: X,Y and U, but there is no enough slots on SRS.
        // We will not able to read all detectors together
        slot = 14 + layer - 1;
    }

    return slot;
}