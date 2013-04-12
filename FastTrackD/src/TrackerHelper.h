
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "../pixeltbb/pixel_tbb.h"

template <class T>
static std::string toString(T t){
  std::stringstream ss;
  std::string s;
  ss << t;
  ss >> s;
  return s;
}

class TrackerHelper {
private:
  // Typecast
  int* no_sensors;
  int* no_hits;
  int* sensor_Zs;
  int* sensor_hitStarts;
  int* sensor_hitNums;
  int* hit_IDs;
  double* hit_Xs;
  double* hit_Ys;
  int* hit_Zs;
  int* hit_sensorNums;
  int num_events, sens_num, hits_num;

  void fill_hit_sensorNums();
  void buildTypestruct(char* dataPointer);
  void printResultTracks(std::vector<track> tracks, int event_no, std::string track_folder_container) const;

public:
  int event_no;
  TrackerHelper();

  std::vector<char> searchByPair(std::vector<char> data);

};
