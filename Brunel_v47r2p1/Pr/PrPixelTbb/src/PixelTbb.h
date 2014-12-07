#pragma once

#include <vector>

#define MAX_TRACK_SIZE 24

struct PixelEvent {
  int noSensors;
  int noHits;
  std::vector<int> sensorZs;
  std::vector<int> sensorHitStarts;
  std::vector<int> sensorHitsNums;
  std::vector<int> hitIDs;
  std::vector<float> hitXs;
  std::vector<float> hitYs;
  std::vector<int> hitZs;
};

struct GpuTrack {
  float m_x0;
  float m_tx;
  float m_y0;
  float m_ty;

  float m_s0;
  float m_sx;
  float m_sz;
  float m_sxz;
  float m_sz2;

  float m_u0;
  float m_uy;
  float m_uz;
  float m_uyz;
  float m_uz2;

  int trackHitsNum;
  std::vector<int> hits;
};

struct SolutionTrack { // 57 + 24*4 = 324 B
  float x0;
  float tx;
  float y0;
  float ty;

  int hitsNum;
  int hits[MAX_TRACK_SIZE];
};

void pixel_tracker_implementation(const PixelEvent & data, std::vector<GpuTrack>& result);
