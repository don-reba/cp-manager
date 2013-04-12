
#include "Tracker.h"

//-------------------------------
// constructor and helper / printer functions
//-------------------------------

Tracker::Tracker() {
	event_no = 0;
}


void Tracker::buildTypestruct(char* dataPointer){
	/* Expected format:
	no_sensors - int
	no_hits    - int
	sensor_Zs  -  no_sensors * int
	sensor_hitStarts - no_sensors * int
	sensor_hitNums   - no_sensors * int
	hit_IDs          - no_hits * int
	hit_Xs   - no_hits * double
	hit_Ys   - no_hits * double
	hit_Zs   - no_hits * int
	*/
	no_sensors = (int*) &dataPointer[0];
	no_hits = (int*) (no_sensors + 1);
	sensor_Zs = (int*) (no_hits + 1);
	sensor_hitStarts = (int*) (sensor_Zs + no_sensors[0]);
	sensor_hitNums = (int*) (sensor_hitStarts + no_sensors[0]);
	hit_IDs = (int*) (sensor_hitNums + no_sensors[0]);
	hit_Xs = (double*) (hit_IDs + no_hits[0]);
	hit_Ys = (double*) (hit_Xs + no_hits[0]);
	hit_Zs = (int*) (hit_Ys + no_hits[0]);

	sens_num = no_sensors[0];
	hits_num = no_hits[0];
}

void Tracker::fill_hit_sensorNums(){
	hit_sensorNums = (int*) malloc(hits_num * sizeof(int));

	int sensor_id = 0;
	for (int i=0; i<hits_num; i++){
		if ((sensor_id != sens_num - 1) && 
		    (sensor_hitStarts[sensor_id+1] == i))
			sensor_id++;
		hit_sensorNums[i] = sensor_id;
	}
}

void Tracker::printResultTracks(std::vector<track> tracks, int event_no, std::string track_folder_container) const{
	std::string track_filename = track_folder_container + "//tracks_" + toString<int>(event_no) + ".txt";
	std::ofstream track_file(track_filename.c_str());
	track_file << std::setprecision(3);

	int t_no = 0;
	for (std::vector<track>::iterator it = tracks.begin(); it != tracks.end(); it++){
		// info() << format( "Dist%8.3f chi%7.3f ", track.distance( *itH ), track.chi2( *itH ) );
		track_file << "track " << t_no++ /*<< " chi2 " << chi2(&(*it))*/ << std::endl;
		for (std::vector<int>::iterator ith = (*it).hits.begin(); ith != (*it).hits.end(); ith++){
			track_file << "hit " << hit_IDs[(*ith)] << " s " << hit_sensorNums[(*ith)] << " ("
		         << hit_Xs[(*ith)] << ", " << hit_Ys[(*ith)] << ", " << hit_Zs[(*ith)] << ")" << std::endl;
		}
		track_file << std::endl;
	}

	track_file.close();
}

//-------------------------------
// searchByPair
//-------------------------------

std::vector<char> Tracker::searchByPair(std::vector<char> data){
	std::cout << "Hey there! I'm about to process data of size: " << data.size() << std::endl;

	// searchByPair algorithm
	// Call pixel implementation defined in .so
	char* dataPointer = (char*) &data[0];
	pixel_tbb(dataPointer);

	// Print result
	buildTypestruct(dataPointer);
	fill_hit_sensorNums();
	printResultTracks(parallel_tracks_vector[0], event_no, "tracks");

	event_no ++;

	std::vector<char> result;
	result.push_back(0);

	return result;
};
