#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <map>

#include <cmath>

#include "tbb/tick_count.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"

#include "NewAlgorithm.h"
#include "Tools.h"
#include "PixelTbb.h"

using namespace std;
using namespace tbb;

void printResultTracks(vector<GpuTrack> tracks, int event_no, string track_folder);
bool compareTracks(vector<GpuTrack> tracks_1, vector<GpuTrack> tracks_2);
map<string, float> calcResults(vector<float> times);
void clean_vector(vector<vector<GpuTrack> >& _tracks_vector);

// Prepare the data types (globally accesible)
int* no_sensors;
int* no_hits;
int* sensor_Zs;
int* sensor_hitStarts;
int* sensor_hitNums;
int* hit_IDs;
float* hit_Xs;
float* hit_Ys;
int* hit_Zs;

// Association hit - sensor
int* hit_sensorNums;

int num_events, sens_num, hits_num;

Debug debug;

std::vector<std::vector<GpuTrack> > parallel_tracks_vector;
std::vector<std::vector<int> > hits;

template <class T>
static std::string toString(T t){
  std::stringstream ss;
  std::string s;
  ss << t;
  ss >> s;
  return s;
}

void printContentsDump(){
  debug << "Sensors: " << sens_num << ", hits: " << hits_num << endl
    << "First sensor_Zs: " << sensor_Zs[0] << ", " << sensor_Zs[1] << endl
    << "First sensor_hitStarts: " << sensor_hitStarts[0] << ", " << sensor_hitStarts[1] << endl
    << "First sensor_hitNums: " << sensor_hitNums[0] << ", " << sensor_hitNums[1] << endl
    << "First hit_IDs: " << hit_IDs[0] << ", " << hit_IDs[1] << endl;

  debug << "First hit_Xs: ";
  for(int i=0; i<40; i++)
    debug << hit_Xs[i] << ", ";
  debug << endl;

  debug << "First hit_Ys: " << hit_Ys[0] << ", " << hit_Ys[1] << endl
    << "First hit_Zs: " << hit_Zs[0] << ", " << hit_Zs[1] << endl;
}

void fill_hit_sensorNums(){
  debug << "Filling in hit_sensorNums... ";

  hit_sensorNums = (int*) malloc(hits_num * sizeof(int));

  int sensor_id = 0;
  for (int i=0; i<hits_num; i++){
    if ((sensor_id != sens_num - 1) && 
        (sensor_hitStarts[sensor_id+1] == i))
      sensor_id++;
    hit_sensorNums[i] = sensor_id;
  }

  debug << "done" << endl;
}

void quicksort_the_thing(){
  debug << "Starting quicksort... ";
  for(int i=0; i<sens_num; i++)
    quickSort(hit_Xs, hit_Ys, hit_IDs, hit_Zs,
        sensor_hitStarts[i], sensor_hitStarts[i] + sensor_hitNums[i]);

  debug << "done" << endl;
}



void pixel_tracker_implementation(const PixelEvent & data, std::vector<GpuTrack>& result)
{
  num_events = 1;
  int num_threads = 0; // auto

  debug.setMode(NO_DEBUG);
  // debug.setMode(DEBUG);

  debug << "Setup:" << endl
    << " Number of threads: " << (num_threads > 0 ? toString<int>(num_threads) : "auto") << endl;

  // Deprecated - Now we use PixelEvent and GpuTrack (more civilized)
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
  /*
     no_sensors = (int*) &data;
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
     */

  // int noSensors;
  // int noHits;
  // std::vector<int> sensorZs;
  // std::vector<int> sensorHitStarts;
  // std::vector<int> sensorHitsNums;
  // std::vector<int> hitIDs;
  // std::vector<float> hitXs;
  // std::vector<float> hitYs;
  // std::vector<int> hitZs;

  sens_num = data.noSensors;
  hits_num = data.noHits;
  sensor_Zs = (int*) &data.sensorZs[0];
  sensor_hitStarts = (int*) &data.sensorHitStarts[0];
  sensor_hitNums = (int*) &data.sensorHitsNums[0];

  hit_IDs = (int*) &data.hitIDs[0];
  hit_Xs = (float*) &data.hitXs[0];
  hit_Ys = (float*) &data.hitYs[0];
  hit_Zs = (int*) &data.hitZs[0];

  printContentsDump();

  // Fill in hit sensorNums info (only needed to cross check results)
  fill_hit_sensorNums();

  // We need to quicksort
  quicksort_the_thing();

  printContentsDump();

  debug << "Sens num " << sens_num << ", hits num " << hits_num << endl;

  // Allocate space for tracks vectors
  parallel_tracks_vector.clear();
  for (int i=0; i<num_events; ++i){
    vector<GpuTrack> t1;
    parallel_tracks_vector.push_back(t1);
  }

  debug << "Starting parallel searchByPair..." << endl;

  // Perform parallel experiments!
  if(num_threads > 0)
    task_scheduler_init(num_threads);

  tick_count parallel_start = tick_count::now();
  parallel_for(blocked_range<int>(0, num_events),
      TBBSearchByPair());
  tick_count parallel_end = tick_count::now();
  result = parallel_tracks_vector[0];


  debug << std::setprecision(3);

  debug << "Algorithm execution finished!" << endl
    << "It took: " << (parallel_end - parallel_start).seconds() << " seconds!" << endl;
  /*    << experiment_timing["mean"] << " seconds (mean), "
        << experiment_timing["deviation"] << " deviation, "
        << experiment_timing["variance"] << " variance, "
        << experiment_timing["min"] << " min, "
        << experiment_timing["max"] << " max " << endl; */

  debug << "Testing hits (first GpuTrack): " << endl;
  bool all_ok = 1;
  for (size_t i = 0; i < hits[0].size(); i++){
    if(hits[0][i] != parallel_tracks_vector[0][0].hits[i]){
      debug << "hit " << i << " differs: " << hits[0][i] << ", " << parallel_tracks_vector[0][0].hits[i] << endl;
      all_ok = 0;
    }
  }
  if(all_ok)
    debug << " All hits are correct!";

  debug << "Writing results..." << endl;
  for (int i = 0; i < num_events; i++)
    printResultTracks(parallel_tracks_vector[i], i, "parallel_tracks");

  debug << "Done!" << endl;
}



void clean_vector(vector<vector<GpuTrack> >& _tracks_vector){
  for(vector<vector<GpuTrack> >::iterator it = _tracks_vector.begin(); it != _tracks_vector.end(); it++){
    (*it).clear();
  }
}

map<string, float> calcResults(vector<float> times){
  // sqrt ( E( (X - m)2) )
  map<string, float> results;
  float deviation = 0.0, variance = 0.0, mean = 0.0, min = numeric_limits<float>::infinity(), max = 0.0;

  int n = 0;
  float seconds;
  for(vector<float>::iterator it = times.begin(); it != times.end(); it++){
    n++;
    seconds = (*it);
    mean = (mean * (n - 1) + seconds) / n;
    variance += seconds * seconds;

    if (seconds < min) min = seconds;
    if (seconds > max) max = seconds;
  }

  variance = (variance - times.size() * mean * mean) / times.size();
  deviation = sqrt(variance);

  results["variance"] = variance;
  results["deviation"] = deviation;
  results["mean"] = mean;
  results["min"] = min;
  results["max"] = max;

  return results;
}

bool compareTracks(vector<GpuTrack> tracks_1, vector<GpuTrack> tracks_2){
  if(tracks_1.size() != tracks_2.size()){
    debug << "x (tracks size)";
    return false;
  }

  for(size_t i = 0; i < tracks_1.size(); ++i){
    GpuTrack track_1 = tracks_1[i];
    GpuTrack track_2 = tracks_2[i];
    if(track_1.hits.size() != track_2.hits.size()){
      debug << "x (hits size)";
      return false;
    }

    for(size_t j = 0; j < track_1.hits.size(); ++j){
      if(track_1.hits[j] != track_2.hits[j]){
        debug << "x (hits)";
        return false;
      }
    }
  }
  debug << ".";
  return true;
}

void printResultTracks(vector<GpuTrack> tracks, int event_no, string track_folder_container){
  string track_filename = track_folder_container + "//tracks_" + toString<int>(event_no) + ".txt";
  ofstream track_file(track_filename.c_str());
  track_file << std::setprecision(3);

  int t_no = 0;
  for (vector<GpuTrack>::iterator it = tracks.begin(); it != tracks.end(); it++){
    // info() << format( "Dist%8.3f chi%7.3f ", GpuTrack.distance( *itH ), GpuTrack.chi2( *itH ) );
    track_file << "GpuTrack " << t_no++ /*<< " chi2 " << chi2(&(*it))*/ << std::endl;
    for (vector<int>::iterator ith = (*it).hits.begin(); ith != (*it).hits.end(); ith++){
      track_file << "hit " << hit_IDs[(*ith)] << " s " << hit_sensorNums[(*ith)] << " ("
        << hit_Xs[(*ith)] << ", " << hit_Ys[(*ith)] << ", " << hit_Zs[(*ith)] << ")" << endl;
    }
    track_file << endl;
  }

  track_file.close();
}
