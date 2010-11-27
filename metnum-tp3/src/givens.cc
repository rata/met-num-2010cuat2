#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include "matrix.hpp"
#include "convenciones.hpp"
#include "vector_utils.hpp"

using namespace std;

typedef unsigned int uint;

typedef struct
{
	double autovalor;
	matrix autovector;
} autocosa;

matrix *A;
matrix *B;
matrix *Q;
matrix *R;
matrix *E;

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

//	cout << "G_" << i << "," << j << endl << W_i.print();
	return W_i;
}

matrix getW(uint j)
{
	uint n = A->cant_rows();
	matrix W = matrix::identity(n);
	for (uint i = j + 1; i <= n; i++) {
		matrix g_i = getW_i(i, j);
//		W = g_i.mult(W);
		*R = g_i.mult(*R);
		*Q = g_i.mult(*Q);
	}

	return W;
}

void triang_col(uint j)
{
	matrix W = getW(j);
//	cout << "W_" << j << endl << W.print();
//	*R = W.mult(*R);
//	*Q = Q->mult(W);

	// Ponemos en cero lo que sabemos que debe dar cero (y chillamos si no
	// es my chico)
	for (uint i= j+1; i <= R->cant_rows(); i++) {
		double val = R->get(i, j);
		assert(abs(val) < convenciones::tolerancia());
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

//	cout << "R inicio es: " << endl << R->print();

	for (uint j = 1; j < m; j++) {
		triang_col(j);
	}
//	matrix ident = matrix::identity(3);
//	ident.set(3,3,-1);
//	*R = ident.mult(*R);
//	*Q = (Q->transpose()).mult(ident);
	*Q = Q->transpose();
	return;
}

bool esTriangularSup(const matrix& M, double cota)
{
	int n = M.cant_rows();
	int m = M.cant_cols();

	for ( int i = 1 ; i <= n; i++) {
		for (int j = 1 ; j < i ; j++ ) {
			if (M.get(i,j) > cota) {
				return false;
			}
		}
	}
	return true;
}

autocosa calcularAutovalores()
{
	*E = matrix::identity(3);
	int rat = 0;
	do {
		givens();
		cout << "A es: " << endl << A->print() << endl;
		cout << "Q es: " << endl << Q->print()<< endl;
		cout << "R es: " << endl << R->print()<< endl;
		cout << "E es: " << endl << E->print()<< endl;

		*A = R->mult(*Q);
		*E = Q->mult(*E);

		rat++;
	} while(!esTriangularSup(*A, 1e-117));
//	} while(rat < 2);

	uint n = R->cant_rows();
	uint m = R->cant_cols();
	autocosa res[3];
	for ( uint i = 1 ; i <= n ; i++) {
		autocosa a_i = { R->get(i,i), Q->columna(i) };
		res[i] = a_i;
	}
	return *res;
}

int main()
{
	A = new matrix(3,3);
	B = new matrix(3,3);
	Q = new matrix(3,3);
	R = new matrix(3,3);
	E = new matrix(3,3);

	A->set(1,1,4);
	A->set(1,2,-1);
	A->set(1,3,1);
	A->set(2,1,3);
	A->set(2,2,3);
	A->set(2,3,9);
	A->set(3,1,5);
	A->set(3,2,3);
	A->set(3,3,5);
	*B = *A;
	cout << A->print();
//	givens();
	autocosa autovalores = calcularAutovalores();

//	*E = E->transpose();
	for (int mati = 1; mati <= 3; mati++) {
		matrix x(3,1);
		matrix v(3,1);
		matrix y(3,1);
		double lamb = A->get(mati,mati);
		v = E->columna(mati);

		y = v;
		y.mult(lamb);

		x = B->mult(v);

		cout << "E*A*Et es:\n" << ((E->mult(*B)).mult((E->transpose()))).print();
		cout << "E*Et es:\n" << (E->mult(E->transpose())).print();
		cout << "B es:\n" << B->print();
		cout << "v es:\n" << v.print();
		cout << "lambda: " << lamb << endl;
		cout << "B*v es:\n" << x.print();
		cout << "lambda*v es:\n" << y.print();
		cout << "------------------------------------------------\n";
		cout << "B*E es:\n" << (B->mult(*E)).print();
		cout << "A*E es:\n" << (A->mult(*E)).print();
		cout << "------------------------------------------------\n";
	}

//	*R = A->mult(*A);
	cout << "A es: " << endl << A->print();
	cout << "Q es: " << endl << Q->print();
	cout << "R es: " << endl << R->print();

	cout << "Q transpuesta es: " << endl << Q->transpose().print();

	cout << "Q transpuesta * Q  es: " << endl << Q->transpose().mult(*Q).print();

	cout << "Q * R  es: " << endl << Q->mult(*R).print();

	cout << "Q traspuesta * A  es: " << endl << Q->transpose().mult(*A).print();

	delete A;
	delete Q;
	delete R;
	return 0;
}
