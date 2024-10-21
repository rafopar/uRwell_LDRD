/* 
 * File:   uRwellLDRDTools.h
 * Author: rafopar
 *
 * Created on June 27, 2024, 1:46 PM
 */

#ifndef URWELLLDRDTOOLS_H
#define URWELLLDRDTOOLS_H
#include <vector>
#include <cmath>

namespace uRwellLDRDTools {

    const int clStripGap = 2; // The Max length of the gap in between strips with a given cluster 
    const double GEM_Strip2Coord = 10./256;
    const double uRwell_Strip2Coord = 10./128;
    const double delta_Z_GEM = 44.608750 ; // cm The Delta_Z between GEMs is 17.(9/16) inch, in cm this is 44.608750
    
    struct uRwellHit {
        int sector;
        int layer;
        int strip;
        int stripLocal;
        double adc;
        double adcRel;
        int ts;
        int slot;
    };

    enum LDRD_detectors {
        det_GEM0 = 0,
        det_GEM1 = 1,
        det_uRWELL0 = 2,
        det_uRWELL1 = 3,
        det_uRWELL2 = 4,
        det_uRWELL3 = 5,
    };

    class uRwellCluster {
    public:
        uRwellCluster();

        void setEnergy(double aEnergy) {
            fEnergy = aEnergy;
        }

        void setHits(std::vector<uRwellHit>);

        std::vector<uRwellHit>* getHits() {
            return &fv_Hits;
        }

        void setNStrips(int aNStrips) {
            fnStrips = fv_Hits.size();
        }

        const double getEnergy() {
            return fEnergy;
        }

        const double getPeakADC() {
            return fPeakADC;
        }

        const double getAvgStrip() {
            return fAvgStrip;
        }

        const double getPeakTime() {
            return fPeakTime;
        }

        // This method will be called when all hits of the cluster are identifed, and set into the cluster
        // I till find the Peak energy, the weighted avg strip etc.
        void FinalizeCluster();

    private:
        int fnStrips;
        double fEnergy;
        double fPeakADC;
        double fAvgStrip;
        double fPeakTime; // This is the peak time of the highest ADC strip
        std::vector<uRwellHit> fv_Hits;

        void findPeakTime();
        void findPeakEnergy();
        void findAvgStrip();

    };


    std::vector<uRwellCluster> getClusters(std::vector<uRwellHit>);
    uRwellCluster getMaxAdcCluster(std::vector<uRwellCluster> &, int);
};

#endif /* URWELLLDRDTOOLS_H */