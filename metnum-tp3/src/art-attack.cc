#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include "matrix.hpp"
#include "matrix_utils.hpp"
#include "convenciones.hpp"

using namespace std;

typedef unsigned int uint;

int main(int argc, char** argv)
{
	if (argc != 5) {
		cout << "Usage: " << argv[0] << " <src> <width> <height> <dst>"
			<< endl;
		return 1;
	}

	if (atoi(argv[2]) < 0 || atoi(argv[3]) < 0) {
		cout << "width y heigth deben ser positivos" << endl;
		return 1;
	}

	string src = argv[1];
	uint w = atoi(argv[2]);
	uint h = atoi(argv[3]);
	string dst = argv[4];

//	cout << src << endl<< w <<endl << h << endl << dst << endl;

	return 0;
}
