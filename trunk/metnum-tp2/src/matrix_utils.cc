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
void gi_triang_col(uint j, matrix& Q, matrix& R)
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
		gi_triang_col(j, Q, R);

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



// Pone en cero el elemento (row, row_base) usando la fila row_base como base
// para calcular el coeficiente de gauss
static
void lu_triang_row(matrix &m, uint row_base, uint row)
{
	assert(m.get(row_base, row_base) != 0);

	double coef = m.get(row, row_base) / m.get(row_base, row_base);

	// Ponemos cero donde ya sabemos que la cuenta deberia dar cero
	m.set(row, row_base, 0);

	for (uint j = row_base + 1; j <= m.cant_cols(); j++) {
		double val = m.get(row, j) - coef * m.get(row_base, j);

		// Imprimimos la cuenta en forma "simbolica"
		// XXX: Si el for empezara desde row_base, vemos claramente que
		// nos deberia dar cero, pero por error numerico, no da. Que
		// hacemos ? Ni hacemos la cuenta para este que *debe* dar cero
		// y lo ponemos en cero ? Y si otro de los numeros tambien
		// deberia dar cero pero no da (y no es de los que anulamos
		// directamente) ?
//		cout << "haciendo: (" << row << ", " << j << ")" << endl;
//		cout << "m[" << row << ", " << j << "] " <<  " - ";
//			cout << "m[" << row << ", " << row_base <<"]";
//			cout << " / m[" << row_base << ", " << row_base << "] * ";
//			cout << "m[" << row_base << ", " << j << "] ";
//		cout << " = " << val << endl;
//		cout << endl << m.print();

		m.set(row, j, val);
	}
}

static
void lu_triang_col(matrix &m, uint j)
{
	for (uint i = j + 1; i <= m.cant_rows(); i++) {
		lu_triang_row(m, j, i);
	}
}

void lu_triang(matrix &m)
{
	// Triangulamos todas las columnas menos la ultima porque es "b"
	for (uint j = 1; j < m.cant_cols(); j++) {
//		cout << "col " << j << endl;

		// Pivoteo parcial
//		uint max_row = j;
//		double max_module = 0;
//		uint i;
//		for (i = j; i <= m.cant_rows(); i++) {
//			if (abs(m.get(i, j)) > max_module) {
//				max_module = abs(m.get(i, j));
//				max_row = i;
//			}
//		}
//		if (max_row != j) {
//			cout << "swapeando porque " << max_module << "es mayor que"
//				<< m.get(i, j) << endl;
//			m.swap_rows(max_row, j);
//		}

		lu_triang_col(m, j);
	}
}

matrix back_substitution(matrix &m)
{
	matrix res(m.cant_rows(), 1);

	for (uint i = m.cant_rows(); i > 0; i--) {
		double x_i = m.get(i, i);
		assert(x_i != 0);

		// El invariante de ciclo es que de la columna i+1 hasta el final
		// conocemos el valor de las variables. Asique son un termino
		// independiente
		double ti = 0;
		for (uint j = i + 1; j < m.cant_cols(); j++) {
			ti += m.get(i, j) * res.get(j, 1);
		}

		// Pasamos el termino indep restando y dividimos por el coef
		// para saber el valor de la variable
		x_i = (m.get(i, m.cant_cols()) - ti) / x_i;

		res.set(i, 1, x_i);
	}

	return res;
}

matrix normalizar_vector(const matrix& vector)
{
	assert(vector.cant_cols() == 1);
	matrix res(vector.cant_rows(), 1);

	double norma = 0;

	for ( uint i = 1 ; i <= vector.cant_rows() ; i++ ) {
		norma += fabs(vector.get(i,1));
	}

	if ( norma != 0)
	{
		for ( uint i = 1 ; i <= vector.cant_rows() ; i++ ) {
			res.set(i, 1, vector.get(i, 1)/norma);
		}
	}
	return res;
}

