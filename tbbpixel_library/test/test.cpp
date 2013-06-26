
#include "pixel_tbb.h"
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

void readFile(string filename, char*& input, bool do_malloc);
void printResultTracks(vector<GpuTrack> tracks, int event_no, string track_folder_container);
void fill_hit_sensorNums();

int main(void){
	
	char* input;
	readFile("pixel-sft-event-0.dump", input, 1);

	// Optional, needed to print
	fill_hit_sensorNums();


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


	// Convert to format expected by pixel_tracker_implementation
	std::vector<GpuTrack> tracks;
	PixelEvent event;

	event.noSensors = sens_num;
	event.noHits = hits_num;
	for(int i=0; i<sens_num; ++i){
		event.sensorZs.push_back(sensor_Zs[i]);
		event.sensorHitStarts.push_back(sensor_hitStarts[i]);
		event.sensorHitsNums.push_back(sensor_hitNums[i]);
	}

	for(int i=0; i<hits_num; ++i){
		event.hitIDs.push_back(hit_IDs[i]);
		event.hitXs.push_back(((float) hit_Xs[i]));
		event.hitYs.push_back(((float) hit_Ys[i]));
		event.hitZs.push_back(hit_Zs[i]);
	}


	// Call the .so function definition with the input file
	// (This will be done by the daemon)
	// Results are written in global parallel_tracks_vector variable.
	
	

	pixel_tracker_implementation(const_cast<const PixelEvent&>(event), tracks);

	/*
	bool all_ok = 1;
	if(hits.size() != parallel_tracks_vector[0].size())
		cout << "Size differs" << endl;

	for(int i=0; i<hits.size(); i++){
		if(i<=parallel_tracks_vector[0].size())
			if(hits[i].size() != parallel_tracks_vector[0][i].hits.size())
				cout << "GpuTrack " << i << "'s hits differ: " << hits[i].size() << ", " << parallel_tracks_vector[0][i].hits.size() << endl;
	}

	printResultTracks(parallel_tracks_vector[0], 0, "tracks");
	*/

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

void printResultTracks(vector<GpuTrack> tracks, int event_no, string track_folder_container){
	string track_filename = track_folder_container + "//tracks_" + toString<int>(event_no) + ".txt";
	ofstream track_file(track_filename.c_str());
	track_file << std::setprecision(3);
	
	int t_no = 0;
	for (vector<GpuTrack>::iterator it = tracks.begin(); it != tracks.end(); it++){
		// info() << format( "Dist%8.3f chi%7.3f ", GpuTrack.distance( *itH ), GpuTrack.chi2( *itH ) );
		track_file << "GpuTrack " << t_no /*<< " chi2 " << chi2(&(*it))*/ << std::endl;
		for (vector<int>::iterator ith = (*it).hits.begin(); ith != (*it).hits.end(); ith++){
			track_file << "hit " << hit_IDs[(*ith)] << " s " << hit_sensorNums[(*ith)] << " ("
					   << hit_Xs[(*ith)] << ", " << hit_Ys[(*ith)] << ", " << hit_Zs[(*ith)] << ")" << endl;
		}

		/*
		cout << "size: " << hits.size() << ", " << t_no << endl;
		for (vector<int>::iterator it2 = hits[t_no].begin(); it2 != hits[t_no].end(); it2++){
			track_file << (*it2) << ", ";
		}
		track_file << endl;
		*/

		track_file << endl;

		t_no ++;
	}

	track_file.close();
}

void fill_hit_sensorNums(){
	hit_sensorNums = (int*) malloc(hits_num * sizeof(int));

	int sensor_id = 0;
	for (int i=0; i<hits_num; i++){
		if ((sensor_id != sens_num - 1) && 
		    (sensor_hitStarts[sensor_id+1] == i))
			sensor_id++;
		hit_sensorNums[i] = sensor_id;
	}
}
