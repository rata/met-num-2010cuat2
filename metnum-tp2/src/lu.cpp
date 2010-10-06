#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "matrix.h"

using namespace std;

/* shortcut */
typedef unsigned int uint;


ifstream fpags, flinks;

/* TODO: tomar p por parametro */
float p = 0.85;

int load_matrix(matrix &m)
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
	// c) todos los elementos multiplicados por p

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
				m.set(k, j, m.get(k, j) * cj * p);
		}
	}

	cout << "matriz final " << endl << m.print();

	return 0;
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

	fpags.close();
	flinks.close();
	return 0;
}
