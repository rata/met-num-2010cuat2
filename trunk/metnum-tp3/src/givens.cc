#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include "matrix.hpp"
#include "vector_utils.hpp"

using namespace std;

typedef unsigned int uint;

matrix *A;
matrix *Q;
matrix *R;

double tolerancia = 1e-14;

matrix getW_i(uint i, uint j)
{
	matrix W_i = matrix::identity(A->cant_rows());
	double c, s;

	double p = R->get(j, j);
	double n = R->get(i, j);
	double norm = sqrt(p*p + n*n);

//	cout << "c = " << p << "/" << "sqrt(" << p << "*" << p<< "+" << n << "*"
//		<< n << endl;
	c = p / norm;
	s = n / norm;
	W_i.set(i, i, c);
	W_i.set(i, j, -s);
	W_i.set(j, i, s);
	W_i.set(j, j, c);

	cout << "G_" << i << "," << j << endl << W_i.print();
	return W_i;
}

matrix getW(uint j)
{
	uint n = A->cant_rows();
	matrix W = matrix::identity(n);
	for (uint i = j + 1; i <= n; i++) {
		matrix g_i = getW_i(i, j);
		W = W.mult(g_i);
		*R = g_i.mult(*R);
		*Q = Q->mult(g_i);
	}

	return W;
}

void triang_col(uint j)
{
	matrix W = getW(j);
	cout << "W_" << j << endl << W.print();
//	*R = W.mult(*R);
//	*Q = Q->mult(W);

	// Ponemos en cero lo que sabemos que debe dar cero (y chillamos si no
	// es my chico)
	for (uint i= j+1; i <= R->cant_rows(); i++) {
		double val = R->get(i, j);
		assert(val < tolerancia);
		R->set(i, j, 0);
	}
}

// devuelve <Q,R>
void givens()
{
	//uint n = A->cant_rows();
	uint m = A->cant_cols();

	*R = *A;
	*Q = matrix::identity(m);

	cout << "R inicio es: " << endl << R->print();

	for (uint j = 1; j < m; j++) {
		triang_col(j);
	}

	return;
}

int main()
{
	A = new matrix(3,3);
	Q = new matrix(3,3);
	R = new matrix(3,3);

	A->set(1,1,4);
	A->set(1,2,-1);
	A->set(1,3,1);
	A->set(2,1,3);
	A->set(2,2,3);
	A->set(2,3,9);
	A->set(3,1,5);
	A->set(3,2,3);
	A->set(3,3,5);
	cout << A->print();
	givens();

//	*R = A->mult(*A);
	cout << "A es: " << endl << A->print();
	cout << "Q es: " << endl << Q->print();
	cout << "R es: " << endl << R->print();

	delete A;
	delete Q;
	delete R;
	return 0;
}
