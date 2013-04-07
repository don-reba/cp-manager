
#include "pixel_tbb.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Prepare the data types (globally accesible)
int* no_sensors;
int* no_hits;
int* sensor_Zs;
int* sensor_hitStarts;
int* sensor_hitNums;
int* hit_IDs;
double* hit_Xs;
double* hit_Ys;
int* hit_Zs;

// Association hit - sensor
int* hit_sensorNums;

int num_events, sens_num, hits_num;

extern vector<vector<track> > parallel_tracks_vector;



template <class T>
static std::string toString(T t){
	std::stringstream ss;
	std::string s;
	ss << t;
	ss >> s;
	return s;
}

void readFile(string filename, char*& input, bool do_malloc);
void printResultTracks(vector<track> tracks, int event_no, string track_folder_container);

int main(void){
	
	char* input;
	readFile("pixel-sft-event-0.dump", input, 1);


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
	no_sensors = (int*) &input[0];
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



	pixel_tbb(input);

	printResultTracks(parallel_tracks_vector[0], 0, "tracks");

	return 0;
}


void readFile(string filename, char*& input, bool do_malloc ){
	int size;
	
	// Give me them datas!!11!
	ifstream infile (filename.c_str(), ifstream::binary);

	// get size of file
	infile.seekg(0, ifstream::end);
	size = infile.tellg();
	infile.seekg(0);

	// read content of infile with pointers
	if (do_malloc)
		input = (char*) malloc(size);
	infile.read (input, size);
	infile.close();
}

void printResultTracks(vector<track> tracks, int event_no, string track_folder_container){
	string track_filename = track_folder_container + "//tracks_" + toString<int>(event_no) + ".txt";
	ofstream track_file(track_filename.c_str());
	track_file << std::setprecision(3);
	
	int t_no = 0;
	for (vector<track>::iterator it = tracks.begin(); it != tracks.end(); it++){
		// info() << format( "Dist%8.3f chi%7.3f ", track.distance( *itH ), track.chi2( *itH ) );
		track_file << "track " << t_no++ /*<< " chi2 " << chi2(&(*it))*/ << std::endl;
		for (vector<int>::iterator ith = (*it).hits.begin(); ith != (*it).hits.end(); ith++){
			track_file << "hit " << hit_IDs[(*ith)] << " s " << hit_sensorNums[(*ith)] << " ("
					   << hit_Xs[(*ith)] << ", " << hit_Ys[(*ith)] << ", " << hit_Zs[(*ith)] << ")" << endl;
		}
		track_file << endl;
	}

	track_file.close();
}
