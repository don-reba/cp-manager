/*
 * GPUPixelDataSequencer.h
 *
 *  Created on: Feb 25, 2013
 *      Author: dcampora
 */
#include <stdlib.h>
#include <vector>

#include "PatPixelHitManager.h"

using namespace std;

#ifndef GPUPIXELDATASEQUENCER_H_
#define GPUPIXELDATASEQUENCER_H_

class GPUPixelDataSequencer {
public:
    PatPixelHitManager* hitManager;

    GPUPixelDataSequencer(){}
    GPUPixelDataSequencer(PatPixelHitManager* hM) :
        hitManager (hM) {}
    ~GPUPixelDataSequencer(){}

    void get(vector<char>& dataPointer);

    template<class T> void combineWithElement(vector<char>& combined_vector, T element);
    template<class T> void combineWithVector(vector<char>& combined_vector, vector<T> input);
    void buildAndCombine(vector<char>& combined_vector);

};

#endif /* GPUPIXELDATASEQUENCER_H_ */

