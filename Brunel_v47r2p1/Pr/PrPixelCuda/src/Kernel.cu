#include "Kernel.cuh"

#include <cstdio>

#define HITS_SHARED 32
#define MAX_FLOAT 100000000.0
#define POST_PROCESSING 0

__device__ int   *no_sensors;
__device__ int   *no_hits;
__device__ int   *sensor_Zs;
__device__ int   *sensor_hitStarts;
__device__ int   *sensor_hitNums;
__device__ int   *hit_IDs;
__device__ float *hit_Xs;
__device__ float *hit_Ys;
__device__ int   *hit_Zs;

__device__ int* prevs;
__device__ int* nexts;

__global__ void prepareData(char* input, int* _prevs, int* _nexts) {
  no_sensors       = (int*)   (input + 0);
  no_hits          = (int*)   (input + 4);
  sensor_Zs        = (int*)   (input + 8);
  sensor_hitStarts = (int*)   (sensor_Zs        + *no_sensors);
  sensor_hitNums   = (int*)   (sensor_hitStarts + *no_sensors);
  hit_IDs          = (int*)   (sensor_hitNums   + *no_sensors);
  hit_Xs           = (float*) (hit_IDs + *no_hits);
  hit_Ys           = (float*) (hit_Xs  + *no_hits);
  hit_Zs           = (int*)   (hit_Ys  + *no_hits);

  prevs = _prevs;
  nexts = _nexts;
}

/** fitHits, gives the fit between h0 and h1.

The accept condition requires dxmax and dymax to be in a range.

The fit (d1) depends on the distance of the tracklet to <0,0,0>.
*/
__device__ float fitHits(Hit& h0, Hit& h1, Hit &h2, Sensor& s0, Sensor& s1, Sensor& s2) {
  // Max dx, dy permissible over next hit

  // TODO: This can go outside this function (only calc once per pair
  // of sensors). Also, it could only be calculated on best fitting distance d1.
  float s_dist = fabs((float)(s1.z - s0.z));
  float dxmax = PARAM_MAXXSLOPE * s_dist;
  float dymax = PARAM_MAXYSLOPE * s_dist;

  bool accept_condition = fabs(h1.x - h0.x) < dxmax &&
              fabs(h1.y - h0.y) < dymax;

  /*float dxmax = PARAM_MAXXSLOPE * fabs((float)(s1.z - s0.z));
  float dymax = PARAM_MAXYSLOPE * fabs((float)(s1.z - s0.z));*/

  // Distance to <0,0,0> in its XY plane.
  /*float t = - s0.z / (s1.z - s0.z);
  float x = h0.x + t * (h1.x - h0.x);
  float y = h0.y + t * (h1.y - h0.y);
  float d1 = sqrtf(powf( (float) (x), 2.0f) +
        powf((float) (y), 2.0f));*/

  // Distance between the hits.
  // float d1 = sqrtf(powf( (float) (h1.x - h0.x), 2.0f) +
  //          powf((float) (h1.y - h0.y), 2.0f));

  // Distance between line <h0,h1> and h2 in XY plane (s2.z)
  // float t = s2.z - s0.z / (s1.z - s0.z);
  // float x = h0.x + t * (h1.x - h0.x);
  // float y = h0.y + t * (h1.y - h0.y);
  // float d1 = sqrtf(powf( (float) (x - h2.x), 2.0f) +
  //      powf((float) (y - h2.y), 2.0f));
  // accept_condition &= (fabs(x - h2.x) < PARAM_TOLERANCE);

  // Require chi2 of third hit below the threshold
  // float t = ((float) (s2.z - s0.z)) / ((float) (s1.z - s0.z));
  float z2_tz = ((float) s2.z - s0.z) / ((float) (s1.z - s0.z));
  float x = h0.x + (h1.x - h0.x) * z2_tz;
  float y = h0.y + (h1.y - h0.y) * z2_tz;

  float dx = x - h2.x;
  float dy = y - h2.y;
  float chi2 = dx * dx * PARAM_W + dy * dy * PARAM_W;
  accept_condition &= chi2 < PARAM_MAXCHI2;

  return accept_condition * chi2 + !accept_condition * MAX_FLOAT;
}

// TODO: Optimize with Olivier's
__device__ float fitHitToTrack(Track& t, Hit& h1, Sensor& s1) {
  // tolerance
  // TODO: To improve efficiency, try with PARAM_TOLERANCE_EXTENDED
  float x_prediction = t.x0 + t.tx * s1.z;
  bool tol_condition = fabs(x_prediction - h1.x) < PARAM_TOLERANCE;

  // chi2 of hit (taken out from function for efficiency)
  float dx = x_prediction - h1.x;
  float dy = (t.y0 + t.ty * s1.z) - h1.y;
  float chi2 = dx * dx * PARAM_W + dy * dy * PARAM_W;

  // TODO: The check for chi2_condition can totally be done after this call
  bool chi2_condition = chi2 < PARAM_MAXCHI2;

  return tol_condition * chi2_condition * chi2 + (!tol_condition || !chi2_condition) * MAX_FLOAT;
}

// Create track
__device__ void acceptTrack(Track& t, TrackFit& fit, Hit& h0, Hit& h1, Sensor& s0, Sensor& s1, int h0_num, int h1_num) {
  float wz = PARAM_W * s0.z;

  fit.s0 = PARAM_W;
  fit.sx = PARAM_W * h0.x;
  fit.sz = wz;
  fit.sxz = wz * h0.x;
  fit.sz2 = wz * s0.z;

  fit.u0 = PARAM_W;
  fit.uy = PARAM_W * h0.y;
  fit.uz = wz;
  fit.uyz = wz * h0.y;
  fit.uz2 = wz * s0.z;

  t.hitsNum = 1;
  t.hits[0] = h0_num;

  // note: This could be done here (inlined)
  updateTrack(t, fit, h1, s1, h1_num);
}

// Update track
__device__ void updateTrack(Track& t, TrackFit& fit, Hit& h1, Sensor& s1, int h1_num) {
  float wz = PARAM_W * s1.z;

  fit.s0 += PARAM_W;
  fit.sx += PARAM_W * h1.x;
  fit.sz += wz;
  fit.sxz += wz * h1.x;
  fit.sz2 += wz * s1.z;

  fit.u0 += PARAM_W;
  fit.uy += PARAM_W * h1.y;
  fit.uz += wz;
  fit.uyz += wz * h1.y;
  fit.uz2 += wz * s1.z;

  t.hits[t.hitsNum] = h1_num;
  t.hitsNum++;

  updateTrackCoords(t, fit);
}

// TODO: Check this function
__device__ void updateTrackCoords (Track& t, TrackFit& fit) {
  float den = (fit.sz2 * fit.s0 - fit.sz * fit.sz);
  if (fabs(den) < 10e-10)
    den = 1.f;
  t.tx = (fit.sxz * fit.s0  - fit.sx  * fit.sz) / den;
  t.x0 = (fit.sx  * fit.sz2 - fit.sxz * fit.sz) / den;

  den = (fit.uz2 * fit.u0 - fit.uz * fit.uz);
  if (fabs(den) < 10e-10)
    den = 1.f;
  t.ty = (fit.uyz * fit.u0  - fit.uy  * fit.uz) / den;
  t.y0 = (fit.uy  * fit.uz2 - fit.uyz * fit.uz) / den;
}

/** Simple implementation of the Kalman Filter selection on the GPU (step 4).

Will rely on pre-processing for selecting next-hits for each hit.

Implementation,
- Perform implementation searching on all hits for each sensor

The algorithm has two parts:
- Track creation (two hits)
- Track following (consecutive sensors)


Optimizations,
- Optimize with shared memory
- Optimize further with pre-processing

Then there must be a post-processing, which selects the
best tracks based on (as per the conversation with David):
- length
- chi2

For this, simply use the table with all created tracks (postProcess):

#track, h0, h1, h2, h3, ..., hn, length, chi2

*/

__global__ void gpuKalman(Track* tracks, bool* trackHolders) {
  Track t;
  TrackFit tfit;
  Sensor s0, s1, s2;
  Hit h0, h1, h2;

  float fit, best_fit;
  bool fit_is_better, accept_track;
  int best_hit, best_hit_h2;

  // 4 of the sensors are unused, because the algorithm needs 5-8-sensor spans
  const int firstSensor  = (gridDim.x - blockIdx.x - 1 + 4);
  const int secondSensor = firstSensor - 2;
  const int lastSensor   = max(firstSensor - 7, 0);

  s0.hitStart = sensor_hitStarts [firstSensor];
  s0.hitNums  = sensor_hitNums   [firstSensor];
  s0.z        = sensor_Zs        [firstSensor];

  s1.hitStart = sensor_hitStarts [secondSensor];
  s1.hitNums  = sensor_hitNums   [secondSensor];
  s1.z        = sensor_Zs        [secondSensor];

  // Iterate over all hits for the current sensor
  const int hits_per_thread = (s0.hitNums + blockDim.x - 1) / blockDim.x;
  for (int i = 0; i < hits_per_thread; ++i) {
    const int current_hit = threadIdx.x * hits_per_thread + i;
    if (current_hit < s0.hitNums) {
      h0.x = hit_Xs[s0.hitStart + current_hit];
      h0.y = hit_Ys[s0.hitStart + current_hit];

      // Initialize track
      for (int j = 0; j < MAX_TRACK_SIZE; ++j)
        t.hits[j] = -1;

      // TRACK CREATION
      // TODO: Modify with preprocessed list of hits.
      best_fit    = MAX_FLOAT;
      best_hit    = -1;
      best_hit_h2 = -1;
      for (int j = 0; j < sensor_hitNums[secondSensor]; ++j) {
        // TODO: Load in chunks of SHARED_MEMORY and take
        // them from shared memory.
        h1.x = hit_Xs[s1.hitStart + j];
        h1.y = hit_Ys[s1.hitStart + j];

        // Search in both directions
        for (int thirdSensor = firstSensor - 3; thirdSensor != lastSensor; --thirdSensor) {
          // TODO: shared memory.
          s2.hitStart = sensor_hitStarts [thirdSensor];
          s2.hitNums  = sensor_hitNums   [thirdSensor];
          s2.z        = sensor_Zs        [thirdSensor];

          // Iterate in the third! list of hits
          for (int k = 0; k < sensor_hitNums[thirdSensor]; ++k) {
            // TODO: Load in chunks of SHARED_MEMORY and take
            // them from shared memory.
            h2.x = hit_Xs[s2.hitStart + k];
            h2.y = hit_Ys[s2.hitStart + k];

            fit = fitHits(h0, h1, h2, s0, s1, s2);
            fit_is_better = fit < best_fit;

            best_fit    = fit_is_better * fit + !fit_is_better * best_fit;
            best_hit    = fit_is_better * j   + !fit_is_better * best_hit;
            best_hit_h2 = fit_is_better * k   + !fit_is_better * best_hit_h2;
          }
        }
      }

      accept_track = best_fit != MAX_FLOAT;

      // We have a best fit!

      // For those that have tracks, we go on
      if (accept_track) {
        // Fill in t (ONLY in case the best fit is acceptable)
        acceptTrack(t, tfit, h0, h1, s0, s1, s0.hitStart + current_hit, s1.hitStart + best_hit);
        updateTrack(t, tfit, h2, s2, s2.hitStart + best_hit_h2);

        // TRACK FOLLOWING
        for (int followSensor = secondSensor - 4; followSensor >= 0; followSensor -= 2) {
          s1.hitStart = sensor_hitStarts[followSensor];
          s1.hitNums  = sensor_hitNums[followSensor];
          s1.z        = sensor_Zs[followSensor];

          best_fit = MAX_FLOAT;
          for (int k = 0; k < sensor_hitNums[followSensor]; ++k) {
            // TODO: Load in chunks of SHARED_MEMORY and take them from shared memory.
            h1.x = hit_Xs[s1.hitStart + k];
            h1.y = hit_Ys[s1.hitStart + k];

            fit = fitHitToTrack(t, h1, s1);
            fit_is_better = fit < best_fit;

            best_fit = fit_is_better * fit + !fit_is_better * best_fit;
            best_hit = fit_is_better * k   + !fit_is_better * best_hit;
          }

          // We have a best fit!
          // Fill in t, ONLY in case the best fit is acceptable

          // TODO: Maybe try to do this more "parallel"
          if (best_fit != MAX_FLOAT)
            updateTrack(t, tfit, h1, s1, s1.hitStart + best_hit);
        }
      }

      // If it's a track, write it to memory, no matter what kind
      // of track it is.
      trackHolders[s0.hitStart + current_hit] = accept_track && (t.hitsNum >= MIN_HITS_TRACK);
      if (accept_track && (t.hitsNum >= MIN_HITS_TRACK))
        tracks[s0.hitStart + current_hit] = t;
    }
  }
}

__global__ void gpuKalmanBalanced(Span  * spans, Fit * fittings) {
  Sensor s0, s1, s2;
  Hit    h0, h1, h2;

  Span span = spans[blockDim.x * blockIdx.x + threadIdx.x];

  const int firstSensor  = span.firstSensor;
  const int secondSensor = firstSensor - 2;
  const int lastSensor   = max(firstSensor - 7, 0);

	s0.z        = sensor_Zs        [firstSensor];
	s0.hitNums  = sensor_hitNums   [firstSensor];
	s0.hitStart = sensor_hitStarts [firstSensor];

	s1.z        = sensor_Zs        [secondSensor];
	s1.hitNums  = sensor_hitNums   [secondSensor];
	s1.hitStart = sensor_hitStarts [secondSensor];

  float bestFit    = MAX_FLOAT;
  int   bestHit0   = -1;
  int   bestHit1   = -1;
  int   bestHit2   = -1;
  int   bestSensor = -1;

	// iterate through a portion of candidate hits defined by
	// start and end coordinates in lexicographic order
  int i = span.first.s0;
  int j = span.first.s1;
  while ((i != span.last.s0 || j != span.last.s1) && i != s0.hitNums) {
    h0.x = hit_Xs[s0.hitStart + i];
    h0.y = hit_Ys[s0.hitStart + i];

    h1.x = hit_Xs[s1.hitStart + j];
    h1.y = hit_Ys[s1.hitStart + j];

    for (int thirdSensor = firstSensor - 3; thirdSensor != lastSensor; --thirdSensor) {
      s2.z        = sensor_Zs        [thirdSensor];
      s2.hitNums  = sensor_hitNums   [thirdSensor];
      s2.hitStart = sensor_hitStarts [thirdSensor];

      for (int k = 0; k != s2.hitNums; ++k) {
        h2.x = hit_Xs[s2.hitStart + k];
        h2.y = hit_Ys[s2.hitStart + k];

        float fit = fitHits(h0, h1, h2, s0, s1, s2);

        bool isBetter = fit < bestFit;

        bestFit    = isBetter * fit         + !isBetter * bestFit;
        bestHit0   = isBetter * i           + !isBetter * bestHit0;
        bestHit1   = isBetter * j           + !isBetter * bestHit1;
        bestHit2   = isBetter * k           + !isBetter * bestHit2;
        bestSensor = isBetter * thirdSensor + !isBetter * thirdSensor;
      }
    }
    // iterate in lexicographic order
    ++j;
    if (j == s1.hitNums) {
      j = 0;
      ++i;
    }
  }
  fittings[blockDim.x * blockIdx.x + threadIdx.x] =
    { bestFit, bestHit0, bestHit1, bestHit2, firstSensor, bestSensor };
}

__global__ void consolidateHits(Fit * fittings, int n, Track * tracks, bool * trackHolders) {
  if (n == 0)
    return;

  int sensor  = fittings[0].firstSensor;
  int bestFit = -1;

  for (int i = 0; i != n; ++i) {
    Fit fit = fittings[i];
    if (fit.firstSensor == sensor)
    {
      if (fit.fitness < bestFit)
        bestFit = i;
    }
    else
    {
      addTrack(fittings[bestFit], tracks, trackHolders);
      bestFit = i;
    }
  }
  //addTrack(fittings[bestFit], tracks, trackHolders);
}

__device__ void addTrack(const Fit & fit, Track * tracks, bool * trackHolders) {
  if (fit.fitness == MAX_FLOAT)
    return;

  int firstSensor  = fit.firstSensor;
  int secondSensor = firstSensor - 2;
  int thirdSensor  = fit.thirdSensor;

  Sensor s0 = { sensor_Zs[firstSensor],  sensor_hitStarts[firstSensor],  sensor_hitNums[firstSensor]  };
  Sensor s1 = { sensor_Zs[secondSensor], sensor_hitStarts[secondSensor], sensor_hitNums[secondSensor] };
  Sensor s2 = { sensor_Zs[thirdSensor],  sensor_hitStarts[thirdSensor],  sensor_hitNums[thirdSensor]  };

  Hit h0 = { hit_Xs[s0.hitStart + fit.hit0], hit_Ys[s0.hitStart + fit.hit0] };
  Hit h1 = { hit_Xs[s1.hitStart + fit.hit1], hit_Ys[s1.hitStart + fit.hit1] };
  Hit h2 = { hit_Xs[s2.hitStart + fit.hit2], hit_Ys[s2.hitStart + fit.hit2] };

  // Fill in t (ONLY in case the best fit is acceptable)
  Track    t;
  TrackFit tfit;
  acceptTrack(t, tfit, h0, h1, s0, s1, s0.hitStart + fit.hit0, s1.hitStart + fit.hit1);
  updateTrack(t, tfit, h2, s2, s2.hitStart + fit.hit2);

  // Track following
  for (int followSensor = firstSensor - 6; followSensor >= 0; followSensor -= 2) {
    s1.hitStart = sensor_hitStarts[followSensor];
    s1.hitNums  = sensor_hitNums[followSensor];
    s1.z        = sensor_Zs[followSensor];

    float bestFit = MAX_FLOAT;
    int   bestHit = 0;
    for (int k = 0; k < sensor_hitNums[followSensor]; ++k) {
      // TODO: Load in chunks of SHARED_MEMORY and take them from shared memory.
      h1.x = hit_Xs[s1.hitStart + k];
      h1.y = hit_Ys[s1.hitStart + k];

      float fitness = fitHitToTrack(t, h1, s1);
      bool isBetter = fitness < bestFit;
      bestFit = isBetter * fitness + !isBetter * bestFit;
      bestHit = isBetter * k       + !isBetter * bestHit;
    }

    // We have a best fit!
    // Fill in t, ONLY in case the best fit is acceptable

    // TODO: Maybe try to do this more "parallel"
    if (bestFit != MAX_FLOAT)
      updateTrack(t, tfit, h1, s1, s1.hitStart + bestHit);
  }

  // If it's a track, write it to memory, no matter what kind of track it is.
  if (t.hitsNum >= MIN_HITS_TRACK) {
    const int i = s0.hitStart + fit.hit0;
    tracks[i]       = t;
    trackHolders[i] = true;
  }
}


/* Calculating the chi2 of a track is quite cumbersome.
It implies loading hit_Xs, hit_Ys, and sensor_Zs elements for each
hit of the track. This introduces branching, and is slow.

However, the track chi2 has to be calculated only when the
track has been created (the tx, ty values change).
*/

__device__ float trackChi2(Track& t) {
  float ch = 0.0;
  int nDoF  = -4 + 2 * t.hitsNum;
  Hit h;
  for (int i = 0; i < MAX_TRACK_SIZE; i++) {
    // TODO: Maybe there's a better way to do this
    if (t.hits[i] != -1) {
      h.x = hit_Xs[t.hits[i]];
      h.y = hit_Ys[t.hits[i]];

      ch += hitChi2(t, h, hit_Zs[t.hits[i]]);
    }
  }
  return ch/nDoF;
}

__device__ float hitChi2(Track& t, Hit& h, int hit_z) {
  // chi2 of a hit
  float dx = (t.x0 + t.tx * hit_z) - h.x;
  float dy = (t.y0 + t.ty * hit_z) - h.y;
  return dx * dx * PARAM_W + dy * dy * PARAM_W;
}


/** The postProcess method takes care of discarding redundant tracks. In other
 words, it (hopefully) increases the purity of our tracks.

- Inspect trackHolders and generate track_indexes and num_tracks.

The main idea is to accept tracks which have unique (> REQUIRED_UNIQUES) hits.
For this, each track is checked against all other more preferent tracks, and non
common hits are kept.

TODO: Change the preference system into something more civilized.  A track t0
has preference over another t1 one if: t0.hitsNum > t1.hitsNum || (t0.hitsNum ==
t1.hitsNum && chi2(t0) < chi2(t1))
*/
__global__ void postProcess(Track* tracks, bool* trackHolders, int* track_indexes, int* num_tracks, int* tracks_to_process) {
  // tracks_to_process holds the list of tracks with trackHolders[t] == true

  // TODO: Try with sh_tracks_to_process
  // __shared__ int sh_tracks_to_process[MAX_POST_TRACKS];

  __shared__ Track sh_tracks [BUNCH_POST_TRACKS];
  __shared__ float sh_chi2   [BUNCH_POST_TRACKS];

  __shared__ Track sh_next_tracks [BUNCH_POST_TRACKS];
  __shared__ float sh_next_chi2   [BUNCH_POST_TRACKS];

  // We will use an atomic to write on a vector concurrently on several values
  __shared__ int tracks_to_process_size;
  __shared__ int tracks_accepted_size;

  tracks_to_process_size = 0;
  tracks_accepted_size   = 0;

  __syncthreads(); // for the atomics tracks_to_process_size, and tracks_processed

  int current_track, next_track;
  bool preferent;

  const int hitsPerBlock = (*no_hits + blockDim.x - 1) / blockDim.x;
  for (int i = 0; i < hitsPerBlock; ++i) {
    current_track = blockDim.x * i + threadIdx.x;
    if (current_track < *no_hits) {
      // Iterate in all tracks (current_track)

      if (trackHolders[current_track]) {
        // Atomic add
        int current_atomic = atomicAdd(&tracks_to_process_size, 1);

        // TODO: This condition shouldn't exist,
        // redo using method to process in batches if necessary
        // if (current_atomic < MAX_POST_TRACKS)
        tracks_to_process[current_atomic] = current_track;
      }
    }
  }

  __syncthreads();

  // Iterate in all current_tracks against all next_tracks.
  // Do this processing on batches of blockDim.x size
  const int tracksPerBlock = (tracks_to_process_size + blockDim.x - 1) / blockDim.x;
  for (int i = 0; i < tracksPerBlock; ++i) {
    current_track = blockDim.x * i + threadIdx.x;
    if (current_track < tracks_to_process_size) {
      // Store all tracks in sh_tracks
      sh_tracks[threadIdx.x] = tracks[tracks_to_process[current_track]];

      // Calculate chi2
      sh_chi2[threadIdx.x] = trackChi2(sh_tracks[threadIdx.x]);
    }

    __syncthreads();

    // Iterate in all next_tracks
    for (int j = 0; j < tracksPerBlock; ++j) {
      next_track = blockDim.x * j + threadIdx.x;

      if (next_track < tracks_to_process_size) {
        // Store all tracks in sh_tracks
        sh_next_tracks[threadIdx.x] = tracks[tracks_to_process[next_track]];

        // Calculate chi2
        sh_next_chi2[threadIdx.x] = trackChi2(sh_tracks[threadIdx.x]);
      }

      __syncthreads();

      // All is loaded, commencing assault!
      for (int k=0; k<BUNCH_POST_TRACKS; ++k) {
        next_track = blockDim.x * j + k;

        if (current_track < tracks_to_process_size && next_track < tracks_to_process_size) {
          /* Compare all tracks to check uniqueness, based on
          - length
          - chi2

          preferent is a boolean storing this logic. It reads,

          TODO: Change preference system by something more civilized
          next_track is preferent if
            it's not current_track,
            its length > current_track . length OR
            (its length == current_track . length AND
            chi2 < current_track . chi2)
          */
          preferent = current_track!=next_track &&
                    (sh_next_tracks[k].hitsNum > sh_tracks[threadIdx.x].hitsNum ||
                    (sh_next_tracks[k].hitsNum == sh_tracks[threadIdx.x].hitsNum &&
                    sh_next_chi2[k] < sh_chi2[threadIdx.x]));

          // Preference system based solely on chi2
          /*preferent = current_track!=next_track &&
                    sh_next_chi2[k] < sh_chi2[threadIdx.x]; */

          // TODO: Maybe there's a better way...
          if (preferent) {
            // Eliminate hits from current_track, based on next_track's
            for (int current_hit=0; current_hit<MAX_TRACK_SIZE; ++current_hit) {
              for (int next_hit=0; next_hit<MAX_TRACK_SIZE; ++next_hit) {
                /* apply mask:
                a[i] =
                  (a[i] == b[j]) * -1 +
                  (a[i] != b[j]) * a[i]
                */
                sh_tracks[threadIdx.x].hits[current_hit] =
                  (sh_tracks[threadIdx.x].hits[current_hit] == sh_next_tracks[k].hits[next_hit]) * -1 +
                  (sh_tracks[threadIdx.x].hits[current_hit] != sh_next_tracks[k].hits[next_hit]) *
                    sh_tracks[threadIdx.x].hits[current_hit];
              }
            }
          }
        }
      }
    }

    if (current_track < tracks_to_process_size) {
      // Check how many uniques do we have
      int unique = 0;
      for (int hit=0; hit<MAX_TRACK_SIZE; ++hit)
        unique += (sh_tracks[threadIdx.x].hits[hit]!=-1);

      if (!POST_PROCESSING || ((float) unique) / sh_tracks[threadIdx.x].hitsNum > REQUIRED_UNIQUES) {
        int current_track_accepted = atomicAdd(&tracks_accepted_size, 1);

        track_indexes[current_track_accepted] = tracks_to_process[current_track];
      }
    }
  }

  __syncthreads();

  if (threadIdx.x==0)
    *num_tracks = tracks_accepted_size;
}

