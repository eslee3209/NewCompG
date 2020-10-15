#include <iostream>
#include "homocomp.h"

using namespace std;

int main() {

	// User control setting
	long logq = 1064;
	long logp = 40;
	long logn = 16; 	// log2(The number of slots)
	long alpha = 8;
	double msg = 0.1;	// message
	long no_f = 2, no_g = 4;

	newcompG(logq, logp, logn, alpha, no_f, no_g, msg);

	return 0;

}
