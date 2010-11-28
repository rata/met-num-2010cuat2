#include <assert.h>	// assert
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include <iostream>
#include "matrix.hpp"
#include "convenciones.hpp"
#include "matrix_utils.hpp"

using namespace std;

typedef unsigned int uint;

static
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

//	matrix G_i = matrix::identity(Q.cant_rows());
	double p = R.get(j, j);
	double n = R.get(i, j);
	matrix res;
	if (n == 0)
		return res;
	double norm = sqrt(p*p + n*n);
	if (norm == 0)
		cout << "NORM ES CERO!!!" << endl;

	double c = p / norm;
	double s = n / norm;
//	G_i.set(i, i, c);
//	G_i.set(i, j, -s);
//	G_i.set(j, i, s);
//	G_i.set(j, j, c);

	for (uint k = 1; k <= Q.cant_cols(); k++) {
		double Rjk = R.get(j,k);
		double Rik = R.get(i,k);
		double Qjk = Q.get(j,k);
		double Qik = Q.get(i,k);

		R.set(j, k, (c * Rjk) + (s * Rik));
		Q.set(j, k, (c * Qjk) + (s * Qik));

		R.set(i, k, (c * Rik) - (s * Rjk));
		Q.set(i, k, ((-1 * s) * Qjk) + (c * Qik));
	}

	return res;
//	return G_i;
}

static
void triang_col(uint j, matrix& Q, matrix& R)
{
	// A y Q tienen la misma cantidad de filas, asique nos fijamos la
	// cantidad de filas de Q que ya la tenemos como parametro
	uint n = Q.cant_rows();

	// Calculamos las matrices de givens para cada (i, j) y las
	// multiplicamos para anular la columna j
	for (uint i = j + 1; i <= n; i++) {
		matrix G_i = getG_i(i, j, Q, R);
	//	R = G_i * R;
	//	Q = G_i * Q;
	}
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

static
bool cambioV(const matrix& Vant, const matrix& V)
{
	for (uint i = 1; i <= V.cant_rows(); i++) {
		if (abs(V.get(i, i) - Vant.get(i, i)) > 0.000000000000001)
			return true;
	}

	return false;
}

bool esSimetrica(const matrix& A)
{
	for (uint i = 1; i <= A.cant_rows(); i++) {
		for (uint j = i + 1; j <= A.cant_cols(); j++) {
			if (A.get(i, j) != A.get(j, i))
				return false;
		}
	}
	return true;
}

void calcular_autovalores(const matrix& A_orig, matrix& Vect, matrix& Val)
{
	assert(esSimetrica(A_orig));
	Val = A_orig;
	Vect = matrix::identity(A_orig.cant_rows());
//	matrix ValAnt = Val;
	matrix R(A_orig.cant_rows(), A_orig.cant_cols());
	matrix Q(A_orig.cant_rows(), A_orig.cant_cols());

	do {
		std::cout << "iniciando QR" << std::endl;
		givens(Val, Q, R);
//		std::cout << "Q es " << endl << Q.print();
//		std::cout << "R es " << endl << R.print();
		std::cout << "QR OK" << std::endl;
//		ValAnt = Val;

		Val = R * Q;
		Vect = Vect * Q;

//	} while (cambioV(ValAnt, Val));
//	} while (!esTriangularSup(A, 1e-17));
	} while (!esDiagonal(Val, 1e-2));

	return;
}


