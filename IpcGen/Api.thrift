struct GpuTrack
{
	1:  required double X0;
	2:  required double Tx;
	3:  required double Y0;
	4:  required double Ty;
	
	5:  required double S0;
	6:  required double Sx;
	7:  required double Sz;
	8:  required double Sxz;
	9:  required double Sz2;
	
	10: required double U0;
	11: required double Uy;
	12: required double Uz;
	13: required double Uyz;
	14: required double Uz2;
	
	15: required i32 TrackHitsNum;
	
	16: required list<i32> Hits;
}

service Tracker
{
	list<GpuTrack> searchByPair(1: binary data);
}
