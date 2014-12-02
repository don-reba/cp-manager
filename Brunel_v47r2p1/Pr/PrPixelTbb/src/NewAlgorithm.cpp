#include "NewAlgorithm.h"

// would be nice to have some expalanations for all of these constants

const double m_maxXSlope         = 0.400;
const double m_maxYSlope         = 0.400;
const double m_maxZForRBeamCut   = 200.0;
const double m_maxR2Beam         = 1.0;
const int    m_maxMissed         = 4;
const double m_extraTol          = 0.200;
const double m_maxChi2ToAdd      = 60.0;
const double m_maxChi2SameSensor = 12.0;
const double m_maxChi2Short      = 6.0;
const double m_maxChi2PerHit     = 12.0;
const int    m_sensNum           = 48;

const double  w                   = 1.0 / ((0.055 / sqrt( 12. )) * ((0.055 / sqrt( 12. ))));

double zBeam(GpuTrack *tr) {
  return -( tr->m_x0 * tr->m_tx + tr->m_y0 * tr->m_ty ) / ( tr->m_tx * tr->m_tx + tr->m_ty * tr->m_ty );
}

double r2AtZ(double z , const GpuTrack *tr) {
  double xx = tr->m_x0 + z * tr->m_tx;
  double yy = tr->m_y0 + z * tr->m_ty;
  return xx*xx + yy * yy;
}

void solve (GpuTrack *tr) {
  double den = ( tr->m_sz2 * tr->m_s0 - tr->m_sz * tr->m_sz );
  if ( fabs(den) < 10e-10 ) den = 1.;
  tr->m_tx     = ( tr->m_sxz * tr->m_s0  - tr->m_sx  * tr->m_sz ) / den;
  tr->m_x0     = ( tr->m_sx  * tr->m_sz2 - tr->m_sxz * tr->m_sz ) / den;

  den = ( tr->m_uz2 * tr->m_u0 - tr->m_uz * tr->m_uz );
  if ( fabs(den) < 10e-10 ) den = 1.;
  tr->m_ty     = ( tr->m_uyz * tr->m_u0  - tr->m_uy  * tr->m_uz ) / den;
  tr->m_y0     = ( tr->m_uy  * tr->m_uz2 - tr->m_uyz * tr->m_uz ) / den;
}

void addHit ( GpuTrack *tr, int offset) {

  // track_ids[offset] = tr->internalId;
  tr->trackHitsNum++;

  double z = hit_Zs[offset];
  double x = hit_Xs[offset];
  // double w = hit_Ws[offset];

  double wz = w * z;

  tr->m_s0  += w;
  tr->m_sx  += w * x;
  tr->m_sz  += wz;
  tr->m_sxz += wz * x;
  tr->m_sz2 += wz * z;

  double y = hit_Ys[offset];

  tr->m_u0  += w;
  tr->m_uy  += w * y;
  tr->m_uz  += wz;
  tr->m_uyz += wz * y;
  tr->m_uz2 += wz * z;

  if( tr->trackHitsNum > 1 ) solve(tr);

  tr->hits.push_back(offset);
}

void setTrack(GpuTrack *tr, int hit0_offset, int hit1_offset){
  // track_ids[hit0_offset] = tr->internalId;
  tr->trackHitsNum = 1;

  double z = hit_Zs[hit0_offset];
  double x = hit_Xs[hit0_offset];
  // double w = hit_Ws[hit0_offset];

  double wz = w * z;

  tr->m_s0  = w;
  tr->m_sx  = w * x;
  tr->m_sz  = wz;
  tr->m_sxz = wz * x;
  tr->m_sz2 = wz * z;

  double y = hit_Ys[hit0_offset];

  tr->m_u0  = w;
  tr->m_uy  = w * y;
  tr->m_uz  = wz;
  tr->m_uyz = wz * y;
  tr->m_uz2 = wz * z;

  // TODO: Remove when not needed
  tr->hits.push_back(hit0_offset);

  addHit (tr, hit1_offset);
}

double chi2Hit( double x, double y, double hitX, double hitY, double hitW){
  double dx = x - hitX;
  double dy = y - hitY;
  return dx * dx * (hitW) + dy * dy * (hitW);
}
double xAtHit(GpuTrack *tr, double z )
{
  return tr->m_x0 + tr->m_tx * z;
}
double yAtHit( GpuTrack *tr, double z  )
{
  return tr->m_y0 + tr->m_ty * z;
}
double chi2Track(GpuTrack *tr, int offset)
{
  double z = hit_Zs[offset];
  return chi2Hit( xAtHit( tr, z ), yAtHit(tr, z ), hit_Xs[offset], hit_Ys[offset], w);
}
double chi2(GpuTrack *t)
{
  double ch = 0.0;
  int nDoF  = -4;
  int hitNumber;
  for (size_t i = 0; i < t->hits.size(); i++){
    hitNumber = t->hits[i];
    ch += chi2Track(t, hitNumber);
    nDoF += 2;
  }
  return ch/nDoF;
}

bool addHitsOnSensor(SensorInfo *sensor, double xTol, double maxChi2, GpuTrack *tr, int eventId ) {
  if (sensor->hitsNum == 0) return false;
  int offset = eventId * hits_num;

  double xGuess = xAtHit(tr, sensor->z) - xTol; // - 1;
  int lastHit = sensor->startPosition + sensor->hitsNum - 1;

  debug << "last hit x: " << hit_Xs[offset + lastHit] << ", xguess: " << xGuess << endl;
  if(hit_Xs[offset + lastHit] < xGuess) return false;

  int hitStart = sensor->startPosition;

  debug << "(";
  for (int i = hitStart; i <= lastHit; i++)
    debug << hit_IDs[i] << ", ";
  debug << ")";

  unsigned int step = sensor->hitsNum;
  while ( step > 2 ) {
    step = step/2;
    if (hit_Xs[offset + hitStart + step] < xGuess) hitStart += step;
  }

  bool added = false;
  int tmpOffset = 0;
  double xPred;
  for(int iH=hitStart; iH<=lastHit; ++iH){

    debug << hit_IDs[iH] << ", ";
    tmpOffset = offset + iH;

    if ( hit_IDs[iH] == 172290464 ){
      debug << " -- The chi2 of 172290464 is: " << chi2Track(tr, tmpOffset) << std::endl;
      debug << "x: " << hit_Xs[tmpOffset] << ", y:" << hit_Ys[tmpOffset] << endl;
    }

    if ( hit_IDs[iH] == 163710761 ){
      debug << " -- The chi2 of 163710761 is: " << chi2Track(tr, tmpOffset) << std::endl;
      debug << "x: " << hit_Xs[tmpOffset] << ", y:" << hit_Ys[tmpOffset] << endl;
      debug << "maxChi2: " << maxChi2 << endl;
    }


    xPred = xAtHit(tr, hit_Zs[tmpOffset]);
    if ( hit_Xs[tmpOffset] + xTol < xPred ) continue;
    if ( hit_Xs[tmpOffset] - xTol > xPred ) break;
    if ( chi2Track(tr, tmpOffset) < maxChi2 ) {
      addHit(tr, tmpOffset);
      // *usedHit = tmpOffset; - Used hits are tagged by the end of the algorithm, not before.
      added = true;
    }
  }

  debug << endl;
  return added;
}

void removeHit(GpuTrack *tr, int worstHitOffset){
  tr->trackHitsNum--;

  double z = hit_Zs[worstHitOffset];
  // double w = hit_Ws[worstHitOffset];
  double x = hit_Xs[worstHitOffset];

  double wz = w * z;

  tr->m_s0  -= w;
  tr->m_sx  -= w * x;
  tr->m_sz  -= wz;
  tr->m_sxz -= wz * x;
  tr->m_sz2 -= wz * z;

  double y = hit_Ys[worstHitOffset];

  tr->m_u0  -= w;
  tr->m_uy  -= w * y;
  tr->m_uz  -= wz;
  tr->m_uyz -= wz * y;
  tr->m_uz2 -= wz * z;

  vector<int>::iterator it = find(tr->hits.begin(), tr->hits.end(), worstHitOffset);

  tr->hits.erase(it);

  if( tr->trackHitsNum > 1 ) solve(tr);
}

//== Remove the worst hit until all chi2 are good
void removeWorstHit(GpuTrack* tr)
{
  double topChi2 = 1.e9;
  int worstHitOffset;
  while( topChi2 > m_maxChi2PerHit ) {
    topChi2 = 0.0;

    // This for loop gets the worst hit
    for (size_t i = 0; i < tr->hits.size(); i++){
      double myChi2 = chi2Track(tr, tr->hits[i]);
      if (myChi2 > topChi2){
        topChi2 = myChi2;
        worstHitOffset = tr->hits[i];
      }
    }

    // If it's bad, we remove it
    if ( topChi2 > m_maxChi2PerHit ) {
      // hit_isUseds[worstHitOffset] = 0;
      // It has still not been added to isUseds, no need to do this :)

      removeHit(tr, worstHitOffset);
      // This changes the chi2 of the GpuTrack, which is why
    }

    // And the algorithm goes on... ?
    // Every hit with chi2 > maxChi2 will be removed... is this the desired behaviour?
    // -> yes, read description above
  }
}

bool all3SensorsAreDifferent(GpuTrack/* *t*/) {
  /* if ( t->sensorNums[0] == t->sensorNums[1] ) return false;
     if ( t->sensorNums[0] == t->sensorNums[2] ) return false;
     if ( t->sensorNums[1] == t->sensorNums[2] ) return false; */
  return true;
}

int nbUnused(GpuTrack *t, bool*& hit_isUseds, int event_hit_displ) {
  int nn = 0;
  for (vector<int>::iterator it = t->hits.begin(); it != t->hits.end(); ++it){
    if (!hit_isUseds[(*it) - event_hit_displ])
      ++nn;
  }
  return nn;
}

// Note: parallel_tracks_vector should have n items allocated already.
void TBBSearchByPair::operator() (const blocked_range<int>& r) const{
  for (int i = r.begin(); i != r.end(); ++i){
    searchByPair(i, parallel_tracks_vector[i]);
  }
}

void searchByPair(int eventId, vector<GpuTrack>& tracks_vector) {
  int lastSensor  = sens_num - 1;
  int firstSensor = 2;

  GpuTrack m_track;

  bool* hit_isUseds = (bool*) calloc(hits_num, sizeof(bool));

  // helper variables
  int event_sensor_displ = eventId * sens_num;
  int event_hit_displ    = eventId * hits_num;

  int sens0, sens1, first1, hit0_no, hit1_no;
  SensorInfo sensor0, sensor1, extra_sensor;
  double dxMax, dyMax;

  debug << "-- searchByPair --" << endl
    << "Number of sensors: " << sens_num << endl
    << "Number of hits: " << hits_num << endl;

  debug << "Starting hits processing..." << endl;

  // Iterate from the last until the first+1 (including it)
  for ( sens0 = lastSensor; firstSensor <= sens0; sens0 -= 1 ) {
    // sens1 is next sensor on the same side
    sens1 = sens0 - 2;

    sensor0.startPosition = sensor_hitStarts [event_sensor_displ + sens0];
    sensor0.hitsNum       = sensor_hitNums   [event_sensor_displ + sens0];
    sensor0.z             = sensor_Zs        [event_sensor_displ + sens0];
    sensor1.startPosition = sensor_hitStarts [event_sensor_displ + sens1];
    sensor1.hitsNum       = sensor_hitNums   [event_sensor_displ + sens1];
    sensor1.z             = sensor_Zs        [event_sensor_displ + sens1];

    /*
       debug << "sensor 0: " << endl
       << " Z: " << sensor0.z << endl
       << " sP: " << sensor0.startPosition << endl
       << " hitsNum: " << sensor0.hitsNum << endl;
       */

    // Indicator of max dx, dy permissible over next hit
    dxMax = m_maxXSlope * fabs( sensor1.z - sensor0.z );
    dyMax = m_maxYSlope * fabs( sensor1.z - sensor0.z );

    first1 = 0;
    for (hit0_no = 0; hit0_no < sensor0.hitsNum; hit0_no++){
      int hit0_offset = event_hit_displ + sensor0.startPosition + hit0_no;
      if ( hit_isUseds[hit0_offset - event_hit_displ] ){
        continue;
      }
      double x0  = hit_Xs[hit0_offset];
      double y0  = hit_Ys[hit0_offset];

      // Min and max x permissible over next hit
      double xMin = x0 - dxMax;
      double xMax = x0 + dxMax;

      // Iterate hits in s1
      for (hit1_no = first1; hit1_no < sensor1.hitsNum; hit1_no++) {

        int hit1_offset = event_hit_displ + sensor1.startPosition + hit1_no;
        double x1 = hit_Xs[hit1_offset];

        //  debug << " hit " << hit_IDs[hit0_offset] << " against: " << hit_IDs[hit1_offset] << endl;

        // Require second hit not to be used, and to be within x limits
        if ( x1 < xMin ) {
          first1 = hit1_no + 1; // Start on the item (hit1_no+1) for next iteration
          continue;
        }
        if ( x1 > xMax ){
          break; // hits are ordered by x (clever)
        }
        if ( hit_isUseds[hit1_offset - event_hit_displ] ){
          continue;
        }

        // Check hit is within y limits
        double y1  = hit_Ys[hit1_offset];
        if ( fabs( y1 - y0 ) > dyMax ){
          continue;
        }

        // Creates a GpuTrack starting at itH0 (first hit) and containing itH1 (second hit - addHit)
        m_track.hits.clear();
        setTrack(&m_track, hit0_offset, hit1_offset);

        debug << endl << "hit" << hit_IDs[hit0_offset] << " and hit" << hit_IDs[hit1_offset] << " are compatible, creating GpuTrack" << endl;

        //== Cut on R2Beam if needed : backward tracks, i.e zBeam > first hit
        if (sensor0.z < m_maxZForRBeamCut) {
          double z_beam  = zBeam(&m_track);
          if ( z_beam > sensor0.z ) {
            double r2Beam = r2AtZ(z_beam, &m_track);
            if ( r2Beam > m_maxR2Beam ){
              continue;
            }
          }
        }

        //== Extend downstream, on both sides of the detector as soon as one hit is missed
        int extraStep = 2;
        int extraSens = sens1-extraStep;

        int nbMissed = 0;

        double lastZ = sensor1.z;

        while ( extraSens >= 0 ) {
          extra_sensor.startPosition = sensor_hitStarts[event_sensor_displ + extraSens];
          extra_sensor.hitsNum       = sensor_hitNums[event_sensor_displ + extraSens];
          extra_sensor.z             = sensor_Zs[event_sensor_displ + extraSens];

          double tol     = m_extraTol;
          double maxChi2 = m_maxChi2ToAdd;
          if ( extra_sensor.z < lastZ - 100.0 ) {
            tol     = 2 * tol;
            maxChi2 = 2 * maxChi2;
          }

          debug << "s" << extraSens << " ";
          bool added = addHitsOnSensor(&extra_sensor, tol, maxChi2, &m_track, eventId);
          if ( added ) {
            nbMissed = 0;
            lastZ = extra_sensor.z;
          } else {
            nbMissed += extraStep;
            extraStep = 1;
          }
          if ( m_maxMissed < nbMissed ){
            break;
          }

          extraSens -= extraStep;
        }

        debug << endl;

        //== Try upstream if almost forward tracks
        if ( sensor0.z > m_maxZForRBeamCut ) {
          int extraStep = 1;
          int extraSens = sens0 + 3;  // + 2 already tried...
          nbMissed = 2;

          while ( extraSens <= lastSensor ) {
            extra_sensor.startPosition = sensor_hitStarts[event_sensor_displ + extraSens];
            extra_sensor.hitsNum       = sensor_hitNums[event_sensor_displ + extraSens];
            extra_sensor.z = sensor_Zs[event_sensor_displ + extraSens];

            bool added = addHitsOnSensor(&extra_sensor, m_extraTol, m_maxChi2ToAdd, &m_track, eventId);

            if ( added ) {
              nbMissed = 0;
            } else {
              nbMissed += extraStep;
            }
            if ( m_maxMissed < nbMissed ){
              break;
            }
            extraSens += extraStep;
          }

        }

        removeWorstHit(&m_track);

        if ( m_track.trackHitsNum < 3 ){
          debug << "Track only has " << m_track.trackHitsNum << " hits!" << endl;
          continue;
        }

        //== Add compatible hits in sens0 and sens1.
        int next_hit = hit0_no + 1 + sensor0.startPosition;
        if ( next_hit != sensor0.startPosition + sensor0.hitsNum ) {
          if ( chi2Track(&m_track, event_hit_displ + next_hit) < m_maxChi2SameSensor ) {
            hit0_no++;
            addHit(&m_track, event_hit_displ + next_hit);
          }
        }

        next_hit = hit1_no + 1 + sensor1.startPosition;
        if ( next_hit != sensor1.startPosition + sensor1.hitsNum ) {
          if ( chi2Track(&m_track, event_hit_displ + next_hit) < m_maxChi2SameSensor ) {
            hit1_no++;
            addHit(&m_track, event_hit_displ + next_hit);
          }
        }

        //== Final check: if only 3 hits, all should be unused and chi2 good.
        if ( m_track.trackHitsNum == 3 ) {
          /*if ( !all3SensorsAreDifferent(&m_track) ) {
          // debug << "Not all three sensors are different" << endl;
          continue;
          } */
          if( nbUnused(&m_track, hit_isUseds, event_hit_displ) != 3){
            // debug << "There is less than 3 non-used hits" << endl;
            continue;
          }
          if( chi2(&m_track) > m_maxChi2Short){
            // debug << "Chi2 test not passed" << endl;
            continue;
          }
        } else {
          if ( nbUnused(&m_track, hit_isUseds, event_hit_displ) < .6 * m_track.trackHitsNum ) {
            // debug << "More than 60% of the hits are used already" << endl;
            continue;
          }
        }

        // debug << endl << "++ Writing GpuTrack" << endl;
        tracks_vector.push_back(m_track);
        addHitIDs(m_track.hits);

        // Tag used hits IF the GpuTrack is bigger than 3!!
        if (m_track.trackHitsNum > 3){
          for (size_t i = 0; i < m_track.hits.size(); i++){
            hit_isUseds[m_track.hits[i] - event_hit_displ] = 1;
          }
          break;
        }
      } // itH1
    } // itH0
  } // sens0
  // return tracks;
  free(hit_isUseds);
}

void addHitIDs(vector<int>& localHitIDs){
  vector<int> realHitIDs;
  for (size_t i = 0; i < localHitIDs.size(); i++){
    realHitIDs.push_back(hit_IDs[localHitIDs[i]]);
  }
  hits.push_back(realHitIDs);
}
