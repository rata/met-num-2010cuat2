#include <iostream>	// cout
#include <fstream>	// fstream
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include "matrix.hpp"
#include "matrix_utils.hpp"

using namespace std;

/* shortcut */
typedef unsigned int uint;


ifstream fpags, flinks;

double p = 0.85;

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
	// d) No lo hacemos con la ultima columna pues sera "b"

//	cout << m.print();
	for (uint j = 1; j < m.cant_cols(); j++) {
		double cj = 0;

		/* calculo cj */
		for (uint i = 1; i <= m.cant_rows(); i++) {
			if (m.get(i, j) == 1)
				cj += 1;
		}

		if (cj != 0)
			cj = 1 / cj;

		/* Multiplico la columna j por cj * p * -1. Y si es la diagonal lo
		 * pongo en 1. Es decir, dejo esta columna como es en I - pWD */
		for (uint i = 1; i <= m.cant_rows(); i++) {
			if (i == j)
				m.set(j, j, 1);
			else
				m.set(i, j, m.get(i, j) * cj * p * -1);
		}
	}

	// Ponemos b
	uint last_col = m.cant_cols();
	for (uint i = 1; i <= m.cant_rows(); i++)
		m.set(i, last_col, 1);

	//cout << "matriz final:" << endl << m.print();

	return;
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
	fpags.close();
	flinks.close();

	// Triangulamos
	lu_triang(m);
	//cout << "matriz triangulada" << endl << m.print();

	// Y resolvemos!
	matrix res = back_substitution(m);
	cout << "Respuesta: " << endl;;
	cout << res.transpose().print();

	cout << "Respuesta Normalizada:\n";
	cout << normalizar_vector(res).transpose().print();
	
	return 0;
}
