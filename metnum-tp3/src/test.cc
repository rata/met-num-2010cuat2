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

void sep()
{
	cout << "=================================================" << endl;
}

int main()
{
	matrix A(4,4);
	matrix Vect(4,4);
	matrix Val(4,4);

	A.set(1,1,4);	A.set(1,2,2);	A.set(1,3,2);	A.set(1,4,1);

	A.set(2,1,2);	A.set(2,2,-1);	A.set(2,3,1);	A.set(2,4,1);

	A.set(3,1,2);	A.set(3,2,1);	A.set(3,3,3);	A.set(3,4,1);

	A.set(4,1,1);	A.set(4,2,1);	A.set(4,3,1);	A.set(4,4,2);
	
	cout << "A es: " << endl << A.print();
	
	calcular_autovalores(A, Vect, Val, 1e-8);

	for (int i = 1; i <= 3; i++) {
		cout << "autov " << i << "------------------------------------" << endl;
		double lambda = Val.get(i, i);
		matrix v = Vect.column(i);
		cout << "v es: " << v.transpose().print() << endl;
		cout << "A * v " << endl << (A * v).transpose().print() << endl;
		v.scale(lambda);
		cout << "lambda* v" << endl << v.transpose().print() << endl;

		cout << "autov " << i <<
			" finished----------------------------------------" << endl;
	}

	return 0;
}
