/* 
 * File:   SkimZeroSuppression.cc
 * Author: rafopar
 *
 * Created on June 27, 2024, 3:43 PM
 */

#include <cstdio>
#include <cstdlib>
#include <fstream>

#include <TH2D.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

#include <uRwellLDRDTools.h>

using namespace std;

bool fileExists(const char* filename);

/*
 * 
 */
int main(int argc, char** argv) {

    char outputFile[256];
    char inputFile[256];

    int run = 0;
    int fnum = -1;
    if (argc > 2) {
        run = atoi(argv[1]);
        fnum = atoi(argv[2]);
        sprintf(inputFile, "Data/decoded_%d_%d.hipo", run, fnum);
        sprintf(outputFile, "Skim_ZeroSuppr_%d_%d.hipo", run, fnum);
        std::string outFileName_Str = outputFile;

        if (fileExists(outputFile)) {
            cout << "The file " << outFileName_Str.c_str() << " exists. The program will not run" << endl;
            cout << "Exiting" << endl;
            exit(1);
        }

    } else {
        std::cout << " *** please provide a run number..." << std::endl;
        exit(0);
    }

    /**
     * Creating schema for uRwell::Hit
     */

    hipo::schema sch("uRwell::Hit", 90, 1);
    sch.parse("sec/S,layer/S,strip/S,adc/F,adcRel/F,ts/S");
    sch.show();

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

    hipo::writer writer;
    writer.getDictionary().addSchema(sch);
    writer.getDictionary().addSchema(factory.getSchema("RAW::adc"));
    writer.getDictionary().addSchema(factory.getSchema("RUN::config"));

    writer.open(outputFile);

    int __bank_Sec_INDEX_ = buRWellADC.getSchema().getEntryOrder("sector");
    int __bank_Layer_INDEX_ = buRWellADC.getSchema().getEntryOrder("layer");
    int __bank_Component_INDEX_ = buRWellADC.getSchema().getEntryOrder("component");
    int __bank_Order_INDEX_ = buRWellADC.getSchema().getEntryOrder("order");
    int __bank_ADC_INDEX_ = buRWellADC.getSchema().getEntryOrder("ADC");
    int __bank_Time_INDEX_ = buRWellADC.getSchema().getEntryOrder("time");
    int __bank_Ped_INDEX_ = buRWellADC.getSchema().getEntryOrder("ped");

    const double sigm_threshold = 3.; // Represents the threshold of the ADC in units of the sigma.
    const int n_ts = 9;
    const int MaxChPerDet = 512; // No detector has more than 512 channels
    const int MaxChePerLayer = 256; // No detector has more than 256 channels per layer


    std::map<int, double> m_ped_mean; // Mean value of the pedestal
    std::map<int, double> m_ped_rms; // rms of the pedestal

    ifstream inp_ped(Form("PedFiles/Pedestals_%d.dat", run));

    if (!inp_ped.is_open()) {
        cout << Form("Can not find the pedestal file  PedFiles/Pedestals_%d.dat", run) << endl;
        cout << "Exiting..." << endl;
        exit(1);
    }

    cout << "Kuku" << endl;
    while (!inp_ped.eof()) {
        int ch;
        double mean, rms;
        inp_ped>>ch;
        inp_ped>>mean;
        inp_ped>>rms;

        m_ped_mean[ch] = mean;
        m_ped_rms[ch] = rms;
    }


    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            //if( evCounter > 200 ){break;}
            if (evCounter % 1000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(buRWellADC);
            event.getStructure(bRAWADc);
            event.getStructure(bRunConf);

            int ev_Number = bRunConf.getInt("event", 0);

            int n_uRwellADC = buRWellADC.getRows();

            if (n_uRwellADC == 0) {
                continue;
            }

            std::map<int, int> m_ts; // Time sample that has the highest ADC for the given strip
            std::map<int, double> m_MaxADC; // Maximum ADC from all time samples of the given strip
            std::map<int, double> m_ADC; // ADC
            std::map<int, double> m_ADCRel; // Relative ADC, in units of rms for the given channel
            std::map<int, uRwellLDRDTools::uRwellHit> m_Hit_temp; // A temporary map
            //            std::map<int, double> m_sector; // Sector for the given channel
            //            std::map<int, double> m_layer;  // Layer for the given channel
            //            std::map<int, double> m_Channel;// Layer for the given channel

            for (int i = 0; i < n_uRwellADC; i++) {
                int sector = buRWellADC.getInt(__bank_Sec_INDEX_, i);
                int layer = buRWellADC.getInt(__bank_Layer_INDEX_, i);
                int channel = buRWellADC.getInt(__bank_Component_INDEX_, i);
                int ADC = buRWellADC.getInt(__bank_ADC_INDEX_, i);

                int uniqueChan = sector * MaxChPerDet + (layer - 1) * MaxChePerLayer + channel;

                int ts = buRWellADC.getInt(__bank_Ped_INDEX_, i);

                m_ADC[uniqueChan] = m_ADC[uniqueChan] + double(ADC);

                if (m_Hit_temp.find(uniqueChan) == m_Hit_temp.end()) {
                    uRwellLDRDTools::uRwellHit tmpHit;
                    tmpHit.sector = sector;
                    tmpHit.layer = layer;
                    tmpHit.strip = channel;
                    m_Hit_temp[ uniqueChan ] = tmpHit;
                }

                if (m_ped_mean[uniqueChan] - ADC > m_MaxADC[uniqueChan]) {
                    m_MaxADC[uniqueChan] = m_ped_mean[uniqueChan] - ADC;
                    m_ts[uniqueChan] = ts;
                }

            }


            vector<uRwellLDRDTools::uRwellHit> v_Hits;

            for (auto it = m_ADC.begin(); it != m_ADC.end(); ++it) {
                int ch = it->first;
                m_ADC[ch] = m_ped_mean[ch] - m_ADC[ch] / double(n_ts);
                m_ADCRel[ch] = m_ADC[ch] / m_ped_rms[ch];

                if (m_ADCRel[ch] > sigm_threshold) {
                    uRwellLDRDTools::uRwellHit curHit;
                    curHit.adc = m_ADC[ch];
                    curHit.adcRel = m_ADCRel[ch];
                    curHit.sector = m_Hit_temp[ch].sector;
                    curHit.layer = m_Hit_temp[ch].layer;
                            curHit.strip = m_Hit_temp[ch].strip;

                    curHit.ts = m_ts[ch];
                    v_Hits.push_back(curHit);
                }
            }

            int n_TotHits = v_Hits.size();
            hipo::bank bHits(sch, n_TotHits);
            hipo::event outEvent;

            int col = 0;
            //     *********** Writing Hits hits above the threshold **********
            //sch.parse("sec/S,layer/S,strip/S,adc/F,adcRel/F,ts/S,slot/S");
            for (auto curHit : v_Hits) {
                bHits.putShort("sec", col, short(curHit.sector));
                bHits.putShort("layer", col, short(curHit.layer));
                bHits.putShort("strip", col, short(curHit.strip));
                bHits.putFloat("adc", col, float(curHit.adc));
                bHits.putFloat("adcRel", col, float(curHit.adcRel));
                bHits.putShort("ts", col, short(curHit.ts));

                col = col + 1;
            }

            outEvent.addStructure(bRAWADc);
            outEvent.addStructure(bRunConf);
            outEvent.addStructure(bHits);
            writer.addEvent(outEvent);


        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    writer.close();
    writer.showSummary();

    return 0;
}

bool fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}
