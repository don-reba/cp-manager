struct PixelEvent
{
	1: required i32         noSensors;
	2: required i32         noHits;
	3: required list<i32>   sensorZs;
	4: required list<i32>   sensorHitStarts;
	5: required list<i32>   sensorHitsNums;
	6: required list<i32>   hitIDs;
	7: required list<float> hitXs;
	8: required list<float> hitYs;
	9: required list<i32>   hitZs;
}

struct GpuTrack
{
	1:  required float x0;
	2:  required float tx;
	3:  required float y0;
	4:  required float ty;
	
	5:  required float s0;
	6:  required float sx;
	7:  required float sz;
	8:  required float sxz;
	9:  required float sz2;
	
	10: required float u0;
	11: required float uy;
	12: required float uz;
	13: required float uyz;
	14: required float uz2;
	
	15: required i32 trackHitsNum;
	
	16: required list<i32> hits;
}

service Tracker
{
	list<GpuTrack> searchByPair(1: PixelEvent data);
}

service Admin
{
	oneway void Exit();
}
