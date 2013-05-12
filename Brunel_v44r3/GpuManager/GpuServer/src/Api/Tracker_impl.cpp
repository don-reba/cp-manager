// Automatically generated file
#include "Tracker.h"
#include "../pixeltbb/pixel_tbb.h"

// Result type 
extern std::vector<std::vector<GpuTrack> > parallel_tracks_vector;
extern std::vector<std::vector<int> > hits;

// service functions for the user to implement
void Tracker::searchByPair(const std::vector<int8_t> & data, std::vector<GpuTrack> & result) const {
  // add function implementation here

	// TODO: Do everything more "class"y.
	
	parallel_tracks_vector.clear();

	char* dataPointer = (char*) &data[0];
	pixel_tbb(dataPointer);

	result = parallel_tracks_vector[0];

	// Print result
	/*
	buildTypestruct(dataPointer);
	fill_hit_sensorNums();
	printResultTracks(parallel_tracks_vector[0], event_no, "tracks");
	*/
}
