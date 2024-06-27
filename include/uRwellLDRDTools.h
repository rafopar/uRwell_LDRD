/* 
 * File:   uRwellLDRDTools.h
 * Author: rafopar
 *
 * Created on June 27, 2024, 1:46 PM
 */

#ifndef URWELLLDRDTOOLS_H
#define URWELLLDRDTOOLS_H

namespace uRwellLDRDTools {

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

};

#endif /* URWELLLDRDTOOLS_H */