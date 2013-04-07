
#ifndef TRACK
#define TRACK 1

#include <vector>

struct track {
	double m_x0;
	double m_tx;
	double m_y0;
	double m_ty;

	double m_s0;
	double m_sx;
	double m_sz;
	double m_sxz;
	double m_sz2;

	double m_u0;
	double m_uy;
	double m_uz;
	double m_uyz;
	double m_uz2;
	
	int trackHitsNum;
	std::vector<int> hits;
};

void pixel_tbb(char*& input);

#endif
