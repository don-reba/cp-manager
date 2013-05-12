
//#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>

#include "tbb/parallel_for.h"
#include "tbb/tick_count.h"

#include "tools.h"
#include "pixel_tbb.h"

extern int* no_sensors;
extern int* no_hits;
extern int* sensor_Zs;
extern int* sensor_hitStarts;
extern int* sensor_hitNums;
extern int* hit_IDs;
extern double* hit_Xs;
extern double* hit_Ys;
extern int* hit_Zs;

extern int num_events;
extern int hits_num;
extern int sens_num;

extern Debug debug;

using namespace std;
using namespace tbb;

#ifndef MAX
#define MAX(a,b) (a > b ? a : b)
#endif


typedef struct {
	int startPosition;
	int hitsNum;
	double z;
} sensorInfo;


extern vector< vector<track> > parallel_tracks_vector;
extern vector< vector<int> > hits;

class TBBSearchByPair {
public:
	void operator() (const blocked_range<int>& r) const;
};

// void searchByPair_tbb(const blocked_range<int>& r);

double zBeam(track *tr);
double r2AtZ( double z , track *tr);
void solve (track *tr);
inline double chi2Hit( double x, double y, double hitX, double hitY, double hitW);
inline double xAtHit(track *tr, double z );
inline double yAtHit( track *tr, double z  );
inline double chi2Track(track *tr, int offset);
inline double chi2(track *t);
inline bool addHitsOnSensor( sensorInfo sensor, double xTol, double maxChi2,
							 track *tr, int threadId );
inline void removeHit(track *t, int worstHitOffset);
inline void removeWorstHit(track* t);
inline bool all3SensorsAreDifferent(track t);
inline int nbUnused(track t);

void addHit ( track *tr, int offset);
void setTrack(track *tr, int hit0offset, int hit1offset);

void searchByPair(int event_no, vector<track>& vector_tracks);
void addHitIDs(vector<int>& hitLocalIDs);
