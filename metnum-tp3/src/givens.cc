#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include "matrix.hpp"
#include "convenciones.hpp"

using namespace std;

typedef unsigned int uint;

matrix getG_i(uint i, uint j, matrix& Q, matrix& R)
{

	// Nos armamos la matriz de Givens que tiene como pivote el elemento (j, j)
	// y pone en cero el (i, j)
	// La matriz deberia tener esta pinta (lo vacio son ceros, es muy
	// parecida a la identidad) :
	//
	//           j     i
	//  | 1                     |
	//  |   1                   |
	//  |      1                |
	// j|         c     s       |
	//  |           1           |
	//  |             1         |
	// i|        -s     c       |
	//  |                 1     |
	//  |                   1   |

	matrix G_i = matrix::identity(Q.cant_rows());
	double p = R.get(j, j);
	double n = R.get(i, j);
	double norm = sqrt(p*p + n*n);

	double c = p / norm;
	double s = n / norm;
	G_i.set(i, i, c);
	G_i.set(i, j, -s);
	G_i.set(j, i, s);
	G_i.set(j, j, c);

	return G_i;
}

void triang_col(uint j, matrix& Q, matrix& R)
{
	// A y Q tienen la misma cantidad de filas, asique nos fijamos la
	// cantidad de filas de Q que ya la tenemos como parametro
	uint n = Q.cant_rows();

	// Calculamos las matrices de givens para cada (i, j) y las
	// multiplicamos para anular la columna j
	for (uint i = j + 1; i <= n; i++) {
		matrix G_i = getG_i(i, j, Q, R);
		R = G_i.mult(R);
		Q = G_i.mult(Q);
	}

	// Ponemos en cero lo que sabemos que debe dar cero (y chillamos si no
	// es my chico)
//	for (uint i= j + 1; i <= R.cant_rows(); i++) {
//		double val = R.get(i, j);
//		assert(abs(val) < convenciones::tolerancia());
//		R.set(i, j, 0);
//	}
}

// devuelve <Q,R>
void givens(const matrix& A, matrix& Q, matrix& R)
{
	uint m = A.cant_cols();
	R = A;
	Q = matrix::identity(m);

	for (uint j = 1; j < m; j++)
		triang_col(j, Q, R);

	// Calculamos QA = R
	// Q^-1 = Qt ==> A = Q^-1 R = Qt R
	// Por lo que la Q que calculamos es la transpuesta de la que queremos,
	// asique transponemos antes de devolver
	Q = Q.transpose();
	return;
}


bool esDiagonal(const matrix& M, double cota)
{
	for (uint i = 1 ; i <= M.cant_rows(); i++) {
		for (uint j = 1 ; j < M.cant_cols() ; j++ ) {
			if ( i == j)
				continue;

			if (abs(M.get(i, j)) > cota)
				return false;
		}
	}
	return true;
}

bool esTriangularSup(const matrix& M, double cota)
{
	for (uint i = 1 ; i <= M.cant_rows(); i++) {
		for (uint j = 1 ; j < i ; j++ ) {
			if (abs(M.get(i, j)) > cota)
				return false;
		}
	}
	return true;
}

bool cambioV(const matrix& Vant, const matrix& V)
{
	for (uint i = 1; i <= V.cant_rows(); i++) {
		if (abs(V.get(i, i) - Vant.get(i, i)) > 0.001)
			return true;
	}

	return false;
}


void calc_autov(const matrix& A_orig, matrix& Q, matrix& R, matrix& V)
{
	matrix A = A_orig;
	V = matrix::identity(A_orig.cant_rows());
	matrix Vant = matrix::identity(A_orig.cant_rows());
	int rat = 0;

	do {
		givens(A, Q, R);

		Vant = V;
		A = R.mult(Q);
		V = V.mult(Q);
		rat++;

//	} while (rat < 1024);
//	} while (cambioV(Vant, V));
//	} while (!esTriangularSup(A, 1e-17));
	} while (!esDiagonal(A, 1e-7));

	return;
}

void sep()
{
	cout << "=================================================" << endl;
}

int main()
{
	matrix A(3,3);
	matrix B(3,3);
	matrix Q(3,3);
	matrix R(3,3);
	matrix V(3,3);

	A.set(1,1,4);
	A.set(1,2,-1);
	A.set(1,3,1);
	A.set(2,1,-1);
	A.set(2,2,3);
	A.set(2,3,9);
	A.set(3,1,1);
	A.set(3,2,9);
	A.set(3,3,5);
	B = A;
	calc_autov(A, Q, R, V);

	matrix AVal = R.mult(Q);
//	V = V.transpose();
	cout << "A es " << endl << A.print();
	sep();
	cout << "V es " << endl << V.print();
	sep();
	cout << "R es " << endl << R.print();
	sep();
	cout << "AVal es " << endl << AVal.print();
	sep();
	cout << "VVt es " << endl << V.mult(V.transpose()).print();
	sep();
	cout << "VtV es " << endl << (V.transpose()).mult(V).print();
	sep();
	cout << "VtAV es " << endl << (V.transpose()).mult(A).mult(V).print();
	sep();
	cout << "VtA es " << endl << (V.transpose()).mult(A).print();
	sep();
	cout << "RVt es " << endl << R.mult(V.transpose()).print();
	sep();
	cout << "AV es " << endl << (A.mult(V)).print();
	sep();
	cout << "VR es " << endl << (V.mult(R)).print();
	sep();






	for (int i = 1; i <= 3; i++) {
		cout << "autov " << i << "------------------------------------" << endl;
		double lambda = AVal.get(i, i);
		matrix v = V.column(i);
		cout << "v es: " << v.transpose().print() << endl;
		cout << "A * v " << endl << (A.mult(v)).transpose().print() << endl;
		v.scale(lambda);
		cout << "lambda* v" << endl << v.transpose().print() << endl;

		cout << "autov " << i <<
			" finished----------------------------------------" << endl;
	}

//	Cambiar E por V;
//	for (int mati = 1; mati <= 3; mati++) {
//		matrix x(3,1);
//		matrix v_1(3,1);
//		matrix v_2(3,1);
//		double lamb = A.get(mati, mati);
//		v = E.columna(mati);
//
//		y = v;
//		y.mult(lamb);
//
//		x = B.mult(v);
//
//		cout << "E*A*Et es:\n" << ((E.mult(B)).mult((E.transpose()))).print();
//		cout << endl;
//		cout << "E*Et es:\n" << (E.mult(E.transpose())).print();
//		cout << endl;
//		cout << "B es:\n" << B.print();
//		cout << endl;
//		cout << "v es:\n" << v.print();
//		cout << endl;
//		cout << "lambda: " << lamb << endl;
//		cout << endl;
//		cout << "B*v es:\n" << x.print();
//		cout << endl;
//		cout << "lambda*v es:\n" << y.print();
//		cout << endl;
//		cout << "------------------------------------------------\n";
//		cout << "B*E es:\n" << (B.mult(E)).print();
//		cout << "A*E es:\n" << (A.mult(E)).print();
//		cout << "------------------------------------------------\n";
//	}

//	*R = A->mult(*A);
//	cout << "A es: " << endl << A.print();
//	cout << "Q es: " << endl << Q.print();
//	cout << "R es: " << endl << R.print();
//
//	cout << "Q transpuesta es: " << endl << Q.transpose().print();
//
//	cout << "Q transpuesta * Q  es: " << endl << Q.transpose().mult(Q).print();
//
//	cout << "Q * R  es: " << endl << Q.mult(R).print();
//
//	cout << "Q traspuesta * A  es: " << endl << Q.transpose().mult(A).print();

//	cout <<	"================================================================";
//	cout << "Q transpuesta es: " << endl << Q.transpose().print();
//
//	cout << "Q transpuesta * Q  es: " << endl << Q.transpose().mult(Q).print();
//
//	cout << "Q * R  es: " << endl << Q.mult(R).print();
//
//	cout << "Q traspuesta * A  es: " << endl << Q.transpose().mult(A).print();


	return 0;
}
