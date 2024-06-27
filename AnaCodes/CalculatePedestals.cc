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

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

using namespace std;

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

    TFile *file_out = new TFile(Form("Pedestals_%d.root", run), "Recreate");

    TH2D *hADC_UniqueChan1 = new TH2D("hADC_UniqueChan1", "", n_MaxUniqueChannels + 1, -0.5, n_MaxUniqueChannels + 0.5, 400, -400., 400.);

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

            for (int i = 0; i < n_ADC; i++) {


                int sector = buRWellADC.getInt(__bank_Sec_INDEX_, i);
                int layer = buRWellADC.getInt(__bank_Layer_INDEX_, i);
                int channel = buRWellADC.getInt(__bank_Component_INDEX_, i);
                int ADC = buRWellADC.getInt(__bank_ADC_INDEX_, i);
                int ts = buRWellADC.getInt(__bank_Ped_INDEX_, i);

                int uniqueChan = sector * MaxChPerDet + (layer - 1) * MaxChePerLayer + channel;

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
                hADC_UniqueChan1->Fill(ch, avg_ADC);
            }

            if (nMaxEventsToProcess > 2000) {
                break;
            }

        }

        std::string pedFileName = Form("PedFiles/Pedestals_%d.dat", run);

        cout<<"********************************************************** "<<endl;
        cout<<"*                                                          "<<endl;
        cout<<"*                  Done looping over events                "<<endl;
        cout<<"*                                                          "<<endl;
        cout<<"*                    Calculating pedestals                 "<<endl;
        cout<<"********************************************************** "<<endl;
        cout<<"*                                                          "<<endl;
        cout<<"*  Generating the pedestal file "<<pedFileName.c_str()<<"  "<<endl;
        /*
         * Done looping over events, now let's calculate pedestals foe each channel and put
         * values in the pedestal file.
         */
        
        ofstream out_dat( pedFileName.c_str() );
        for( int i = 0; i < hADC_UniqueChan1->GetNbinsX(); i++ ){
            TH1D *h_curHist = (TH1D*)hADC_UniqueChan1->ProjectionY(Form("ADC_%d", i+1), i+1, i+1);
            double mean_val = h_curHist->GetEntries() > 0 ? h_curHist->GetBinCenter( h_curHist->GetMaximumBin() ) : 0 ;
            double rms = h_curHist->GetEntries() > 0 ? h_curHist->GetRMS() : 100000 ; // Just very big number to indicate RMS is not calculated for this channel
            out_dat<<setw(5)<<i<<setw(9)<<mean_val<<setw(9)<<rms<<endl;
        }

    } catch (const char msg) {
        cerr << msg << endl;
    }

    gDirectory->Write();
    file_out->Close();

    return 0;
}