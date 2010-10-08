#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <assert.h>
#include "matrix.h"

using namespace std;

/* shortcut */
typedef unsigned int uint;


ifstream fpags, flinks;

/* TODO: tomar p por parametro */
float p = 0.85;

void load_matrix(matrix &m)
{
	// Hacemos que m sea igual W
	int cant_links;
	flinks >> cant_links;

	for (int k = 0; k < cant_links; k++) {
		int i, j;
		flinks >> j >> i;

		// Ignoramos autolinks
		if (i == j)
			continue;

		// Ponemos un 1 porque hay un link
		m.set(i, j, 1);
	}

	// Ahora tenemos W, pero queremos (I - pWD)
	// Entonces queremos:
	// a) la diagonal con 1s (por I, ya que en W es cero)
	// b) cada columna multiplicada por su Cj
	// c) todos los elementos multiplicados por -1 * p

	cout << m.print();
	for (uint j = 1; j <= m.cant_rows(); j++) {
		uint i = 1;
		float cj = 0;

		/* calculo cj */
		for (i = 1; i <= m.cant_rows(); i++) {
			if (m.get(i, j) != 0)
				cj += 1;
		}

		if (cj != 0)
			cj = 1 / cj;

		/* Multiplico la columna j por cj * p. Y si es la diagonal lo
		 * pongo en 1. Es decir, dejo esta columna como es en I - pWD */
		for (uint k = 1; k <= m.cant_rows(); k++) {
			if (k == j)
				m.set(j, j, 1);
			else
				m.set(k, j, m.get(k, j) * cj * p * -1);
		}
	}

	cout << "matriz final:" << endl << m.print();

	return;
}

// Pone en cero el elemento (row, row_base) usando la fila row_base como base
// para calcular el coeficiente de gauss
void triang_row(matrix &m, uint row_base, uint row)
{
	assert(m.get(row_base, row_base) != 0);

	float coef = m.get(row, row_base) / m.get(row_base, row_base);

	for (uint j = row_base; j <= m.cant_cols(); j++) {
		float val = m.get(row, j) - coef * m.get(row_base, j);

		// Imprimimos la cuenta en forma "simbolica"
		// XXX: Asi vemos claramente que nos deberia dar cero, pero por
		// error numerico, no da. Que hacemos ? Ni hacemos la cuenta
		// para este que *debe* dar cero y lo ponemos en cero ? Y si
		// otro de los numeros tambien deberia dar cero pero no da (y no
		// es de los que anulamos directamente) ?
		cout << "haciendo: (" << row << ", " << j << ")" << endl;
		cout << "m[" << row << ", " << j << "] " <<  " - ";
			cout << "m[" << row << ", " << row_base <<"]";
			cout << " / m[" << row_base << ", " << row_base << "] * ";
			cout << "m[" << row_base << ", " << j << "] ";
		cout << " = " << val << endl;
		cout << endl << m.print();

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
	// Triangulamos todas las columnas menos la ultima (no hay nada debajo
	// de la diagonal de la ultima columna)
	for (uint j = 1; j < m.cant_cols(); j++) {
		cout << "col " << j << endl;
		triang_col(m, j);
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
	matrix m(n, n);

	load_matrix(m);
	triang(m);
	cout << "matriz triangulada" << endl << m.print();

	fpags.close();
	flinks.close();
	return 0;
}
