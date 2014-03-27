
#ifndef DEBUG_PRINT
#define DEBUG_PRINT 1

#include <iostream>

using namespace std;

enum verbosity {
	DEBUG,
	NO_DEBUG
};

class Debug : public ostream {
private:
	verbosity _mode;

public:

	Debug() : _mode (NO_DEBUG){}

	void setMode(verbosity mode){
		_mode = mode;
	}

	template <class T>
	Debug& operator<<(T t){
		if(_mode==DEBUG) cout << t;
		return (*this);
	}

	typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
	typedef CoutType& (*StandardEndLine)(CoutType&);
	Debug& operator<<(StandardEndLine)
	{
		if(_mode==DEBUG) cout << endl;
		return (*this);
	}
};



//  quickSort
//
//  This public-domain C implementation by Darel Rex Finley.
//
//  * This function assumes it is called with valid parameters.
//
//  * Example calls:
//    quickSort(&myArray[0],5); // sorts elements 0, 1, 2, 3, and 4
//    quickSort(&myArray[3],5); // sorts elements 3, 4, 5, 6, and 7

// Slightly mod to match my needs
void quickSort(float*& hit_Xs, float*& hit_Ys, int*& hit_IDs, int*& hit_Zs, int _beginning, int _end);

#endif

