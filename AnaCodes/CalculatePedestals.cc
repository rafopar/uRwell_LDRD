/* 
 * File:   CalculatePedestals.cc
 * Author: rafopar
 *
 * Created on June 27, 2024, 1:16 PM
 */

#include <cstdlib>

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
            
            
            
        }
    }catch( const char msg ){
        cerr<<msg<<endl;
    }
    
    
    return 0;
}