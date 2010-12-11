#include <assert.h>	// assert
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include <iostream>
#include "matrix_utils.hpp"

using namespace std;

typedef unsigned int uint;

matrix tc(const vector<matrix>& autoVectores, const matrix& x)
{
	assert(x.cant_cols() == 1);

	uint k = autoVectores.size();

	matrix tcx(k, 1);

	for (uint i = 0 ; i < k ; i++) {
		matrix vi = autoVectores[i];
		assert(vi.cant_cols() == 1);
		// Este otro assert es necesario pero lo hace la multiplicación
//		assert(v1.cant_rows() == x.cant_cols());
		matrix value = (vi.transpose() * x);
		// Asserts para debug
		assert(value.cant_rows() == 1);
		assert(value.cant_cols() == 1);

		tcx.set(i, 1, value.get(1,1) );
	}

	return tcx;
}

static
void mult_por_G_i(uint i, uint j, matrix& Q, matrix& R)
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

	double p = R.get(j, j);
	double n = R.get(i, j);
	double norm = sqrt(p*p + n*n);

	// Si ya es cero, no hace falta multiplicar por nada para dejarlo en
	// cero :)
	if (n == 0)
		return;

	double c = p / norm;
	double s = n / norm;

	// G_i * R es igual a R salvo en la fila i y en la fila j. Asique solo
	// calculamos el resultado de multiplicar esas dos filas y lo ponemos en
	// R, para ahorrar pasos.
	// Y hacemos lo mismo para calcular G_i * Q, ya que es multiplicar por
	// G_i de nuevo, que tiene esta forma particular
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

	return;
}

static
void triang_col(uint j, matrix& Q, matrix& R)
{
	// A y Q tienen la misma cantidad de filas, asique nos fijamos la
	// cantidad de filas de Q que ya la tenemos como parametro
	uint n = Q.cant_rows();

	// Calculamos las matrices de givens para cada (i, j) y las
	// multiplicamos para anular la columna j
	for (uint i = j + 1; i <= n; i++)
		mult_por_G_i(i, j, Q, R);
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
bool tieneDiferenteDiag(const matrix& A1, const matrix& A2, double cota)
{
	assert(A1.cant_rows() == A2.cant_rows());
	assert(A1.cant_cols() == A2.cant_cols());
	
	for (uint i = 1; i <= A1.cant_rows() && i <= A1.cant_cols(); i++) {
		if (abs(A1.get(i, i) - A2.get(i, i)) > cota)
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

void calcular_autovalores(const matrix& A_orig, matrix& Vect, matrix& Val, double tol)
{
	assert(tol > 0);
	assert(esSimetrica(A_orig));
	Val = A_orig;
	Vect = matrix::identity(A_orig.cant_rows());

	matrix R(A_orig.cant_rows(), A_orig.cant_cols());
	matrix Q(A_orig.cant_rows(), A_orig.cant_cols());

	matrix DiagAnt = Val.diagonal();

	do {
		std::cout << "iniciando QR" << std::endl;
		givens(Val, Q, R);
//		std::cout << "Q es " << endl << Q.print();
//		std::cout << "R es " << endl << R.print();
		std::cout << "QR OK" << std::endl;
		DiagAnt = Val.diagonal();

		Val = R * Q;
		Vect = Vect * Q;

//	} while (cambioV(ValAnt, Val));
//	} while (!esTriangularSup(A, 1e-17));
//	} while (!esDiagonal(Val, 1e-2));
	} while (!esDiagonal(Val, tol) || tieneDiferenteDiag(DiagAnt, Val, tol));

	return;
}


