/* 
 * File:   uRwellLDRDTools.cc
 * Author: rafopar
 * 
 * Created on June 27, 2024, 1:46 PM
 */

#include "uRwellLDRDTools.h"
#include <algorithm>
#include <vector>

using namespace std;

namespace uRwellLDRDTools {

    uRwellCluster getMaxAdcCluster(std::vector<uRwellCluster> &v_clusters, int MinHits) {
        int ind_Max = 0;
        double ADC_Max = 0;

        for (int ind = 0; ind < v_clusters.size(); ind++) {

            if (v_clusters.at(ind).getHits()->size() < MinHits) {
                continue;
            }

            if (v_clusters.at(ind).getPeakADC() > ADC_Max) {
                ADC_Max = v_clusters.at(ind).getPeakADC();
                ind_Max = ind;
            }
        }

        // Check if there is such a cluster, otherwise make a new cluster with 0
        // energy that easily can be checked 
        if (ADC_Max > 0) {
            return v_clusters.at(ind_Max);
        } else {
            uRwellCluster tmp;
            tmp.setEnergy(0);
            return tmp;
        }
    }

    std::vector<uRwellCluster> getClusters(std::vector<uRwellHit> v_Hits) {
        //
        // Let's 1st sort the vector of strips
        //
        std::vector<uRwellCluster> v_Clusters;
        std::sort(v_Hits.begin(), v_Hits.end(), [ ](const auto& lhs, const auto& rhs) {
            return lhs.strip < rhs.strip;
        });


        double clEnergy = 0;
        bool newCluster = true;
        int prev_Strip = -10000; // Some number that clearly is not a real strip number
        //        vector<int> v_strips;
        vector<uRwellHit> v_ClHits;

        for (int i = 0; i < v_Hits.size(); i++) {

            int curStrip = v_Hits.at(i).strip;
            double curHitEnergy = v_Hits.at(i).adc;

            //cout << "v_Hits.at(i).strip = " << v_Hits.at(i).strip << "    curStrip =  " << curStrip << endl;

            /*
             * Since the v_Hits is sorted by Strip numbers, in the following if statement we 
             * don't need to chack fro absalute value for the curStrip - prev_Strip
             */
            if ((curStrip - prev_Strip <= (clStripGap + 1)) || i == 0) {
                clEnergy = clEnergy + curHitEnergy;
                v_ClHits.push_back(v_Hits.at(i));

                if (i == v_Hits.size() - 1) {
                    uRwellLDRDTools::uRwellCluster curCluster;
                    curCluster.setEnergy(clEnergy);
                    curCluster.setHits(v_ClHits);
                    curCluster.FinalizeCluster();
                    v_Clusters.push_back(curCluster);
                }

            } else {
                uRwellLDRDTools::uRwellCluster curCluster;
                curCluster.setEnergy(clEnergy);
                curCluster.setHits(v_ClHits);
                curCluster.FinalizeCluster();
                v_Clusters.push_back(curCluster);

                v_ClHits.clear();
                v_ClHits.shrink_to_fit();

                clEnergy = curHitEnergy;
                v_ClHits.push_back(v_Hits.at(i));
            }
            prev_Strip = curStrip;
        }

        //cout<<"The size of the cluster is "<<v_Clusters.size()<<endl;
        return v_Clusters;
    }

    uRwellCluster::uRwellCluster() {
        fnStrips = 0;
        fEnergy = 0;
    }

    void uRwellCluster::setHits(std::vector<uRwellHit> aHits) {
        fv_Hits = aHits;
    }

    void uRwellCluster::findPeakEnergy() {

        double MaxADC = 0;

        for (auto curHit : fv_Hits) {
            MaxADC = MaxADC > curHit.adc ? MaxADC : curHit.adc;
        }

        fPeakADC = MaxADC;
    }

    void uRwellCluster::findAvgStrip() {

        double WeightedSum = 0;
        double ADCSum = 0;

        for (auto curHit : fv_Hits) {
            WeightedSum = WeightedSum + curHit.adc * curHit.strip;
            ADCSum = ADCSum + curHit.adc;
        }

        fAvgStrip = WeightedSum / ADCSum;
    }

    void uRwellCluster::findPeakTime() {

        int time = -1;

        double MaxADC = 0;

        for (auto curHit : fv_Hits) {

            if (curHit.adc > MaxADC) {
                MaxADC = curHit.adc;
                time = curHit.ts;
            }
        }
        fPeakTime = time;
    }

    void uRwellCluster::FinalizeCluster() {

        double MaxADC = 0;
        double WeightedSum = 0;
        double ADCSum = 0;
        int time = -1;

        for (auto curHit : fv_Hits) {


            if (curHit.adc > MaxADC) {
                MaxADC = curHit.adc;
                time = curHit.ts;
            }

            WeightedSum = WeightedSum + curHit.adc * curHit.strip;
            ADCSum = ADCSum + curHit.adc;
        }

        fPeakADC = MaxADC; // ADC of the strip w/ highest ADC
        fAvgStrip = WeightedSum / ADCSum; // The weighted average strip number of the cluster
        fPeakTime = time; // Time of the highest energy strip
    }

};