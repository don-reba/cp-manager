/*
 * GPUPixelDataSequencer.cpp
 *
 *  Created on: Feb 25, 2013
 *      Author: dcampora
 */

// note: deprecated

#include "GPUPixelDataSequencer.h"

void GPUPixelDataSequencer::get(vector <char>& dataPointer){
    buildAndCombine(dataPointer);
}

template<class T>
void GPUPixelDataSequencer::combineWithElement(vector<char>& combined_vector, T element){
    char* t = (char*) &(element);
    for(int i=0; i<sizeof(element); i++)
        combined_vector.push_back(t[i]);
}

template<class T>
void GPUPixelDataSequencer::combineWithVector(vector<char>& combined_vector, vector<T> input){
    for (typename vector<T>::iterator it = input.begin(); it< input.end(); it++){
        combineWithElement(combined_vector, (*it));
    }
}

void GPUPixelDataSequencer::buildAndCombine(vector<char>& combined_vector){
    
    PixelEvent event = hitManager->event;

    // combine all
    // TODO: Make this use malloc and memcpy (much more efficient) 

    // Sizes of event.sensor* and event.hit*
    combineWithElement(combined_vector, event.noSensors);
    combineWithElement(combined_vector, event.noHits);

    // Data itself
    combineWithVector(combined_vector, event.sensorZs);
    combineWithVector(combined_vector, event.sensorHitStarts);
    combineWithVector(combined_vector, event.sensorHitsNums);
    combineWithVector(combined_vector, event.hitIDs);
    combineWithVector(combined_vector, event.hitXs);
    combineWithVector(combined_vector, event.hitYs);
    combineWithVector(combined_vector, event.hitZs);
}
