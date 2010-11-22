#include <iostream>	// cout
#include <fstream>	// fstream
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include "matrix.h"
#include "vector_utils.hpp"

using namespace std;

/* shortcut */
typedef unsigned int uint;


ifstream fpags, flinks;

double p = 0.85;

// Pone en cero el elemento (row, row_base) usando la fila row_base como base
// para calcular el coeficiente de gauss
void triang_row(matrix &m, uint row_base, uint row)
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

void triang_col(matrix &m, uint j)
{
	for (uint i = j + 1; i <= m.cant_rows(); i++) {
		triang_row(m, j, i);
	}
}

void triang(matrix &m)
{
	// Triangulamos todas las columnas
	for (uint j = 1; j <= m.cant_cols(); j++) {
//		cout << "col " << j << endl;

		triang_col(m, j);
	}
}

void back_substitution(matrix &m, double *res)
{
	// Asumimos que la longitud de res es m.cant_rows()

	for (uint i = m.cant_rows(); i > 0; i--) {
		double x_i = m.get(i, i);
		assert(x_i != 0);

		// El invariante de ciclo es que de la columna i+1 hasta el final
		// conocemos el valor de las variables. Asique son un termino
		// independiente
		double ti = 0;
		for (uint j = i + 1; j < m.cant_cols(); j++) {
			// res es un array, empieza en 0. Asique la posicion
			// j-esima es j - 1
			ti += m.get(i, j) * res[j - 1];
		}

		// Pasamos el termino indep restando y dividimos por el coef
		// para saber el valor de la variable
		x_i = (m.get(i, m.cant_cols()) - ti) / x_i;

		// Es un array, empieza en 0. En cambio la matriz va de 1 a n
		res[i - 1] = x_i;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Uso: " << argv[0] << " <pags-file> <links-file>" << endl;
		return 1;
	}

	char *pags = argv[1];
	char *links = argv[2];

	fpags.open(pags, ifstream::in);
	flinks.open(links, ifstream::in);

	if (fpags.fail() || flinks.fail()) {
		cout << "Error al leer los archivos" << endl;
		return 2;
	}

	// La primer linea es el tamaño de la matriz
	int n;
	fpags >> n;
	matrix m(n, n + 1);

	// calculamos I - pWD
	load_matrix(m);

	// Triangulamos
	triang(m);
	cout << "matriz triangulada" << endl << m.print();

	double *res = (double *) malloc(sizeof(double) * n);
	if (res == NULL)
		throw;

	// Y resolvemos!
	back_substitution(m, res);
	cout << "respuesta: ";
	imprimir_vector(n, res);
	normalizar(n, res);
	cout << "respuesta normalizada: ";
	imprimir_vector(n, res);

	free(res);
	fpags.close();
	flinks.close();
	return 0;
}
