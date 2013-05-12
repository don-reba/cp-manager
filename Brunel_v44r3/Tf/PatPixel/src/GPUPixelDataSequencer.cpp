/*
 * GPUPixelDataSequencer.cpp
 *
 *  Created on: Feb 25, 2013
 *      Author: dcampora
 */

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
    
    pixelEvent* event = hitManager->event;

    cout << "GPUDataSequencer  Packing data..." << endl;

    // combine all
    // TODO: Make this use malloc and memcpy (much more efficient) 

    // Sizes of event->sensor* and event->hit*
    combineWithElement(combined_vector, event->noSensors);
    combineWithElement(combined_vector, event->noHits);

    // Data itself
    combineWithVector(combined_vector, event->sensorZs);
    combineWithVector(combined_vector, event->sensorHitStarts);
    combineWithVector(combined_vector, event->sensorHitsNums);
    combineWithVector(combined_vector, event->hitIDs);
    combineWithVector(combined_vector, event->hitXs);
    combineWithVector(combined_vector, event->hitYs);
    combineWithVector(combined_vector, event->hitZs);

    // cout << "GPUDataSequencer  Sending to server..." << endl;

/*    cout << "no sensors " << event->noSensors << endl
         << "no hits " << event->noHits << endl
         << "sizes: " << event->sensorZs.size() << ", " << event->sensorHitStarts.size()
         << ", " << event->sensorHitsNums.size() << endl
         << "more sizes: " << event->hitIDs.size() << ", " << event->hitXs.size()
         << ", " << event->hitYs.size() <<  ", " << event->hitZs.size() << endl;

    cout << "first two values: " << event->sensorZs[0] << "; " << event->sensorZs[1] << endl
            << event->sensorHitStarts[0] << ", " << event->sensorHitStarts[1] << ", " << event->sensorHitStarts[2] << ", " << event->sensorHitStarts[3] << endl 
            << event->sensorHitsNums[0] << ", " << event->sensorHitsNums[1] << endl
            << event->hitIDs[0] << ", " << event->hitIDs[1] << endl
            << event->hitXs[0] << ", " << event->hitXs[1] << endl
            << event->hitYs[0] << ", " << event->hitYs[1] << endl
            << event->hitZs[0] << ", " << event->hitZs[1] << endl;

    cout << "combined_vector, some values:" << endl;
	int* int_vector = (int*) (&combined_vector[0]);
	for (int i=0; i<100; i++)
		cout << int_vector[i] << ", ";
	cout << endl; */
}
