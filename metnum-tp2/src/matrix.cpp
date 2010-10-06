#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <string>
#include "matrix.h"

using namespace std;

typedef unsigned int uint;

matrix::matrix(uint i, uint j)
{
	this->n = i;
	this->m = j;

	// Allocamos memoria para n*m elementos de tamaño sizeof(float)
	// Usamos calloc para inicializar con ceros
	mat = (float *) calloc(sizeof(float), n * m);

	// TODO: error mas claro
	if (mat == NULL)
		throw;
}

matrix::~matrix()
{
	free(mat);
	mat = NULL;
}

void matrix::set(uint i, uint j, float num)
{
	assert(valid_pos(i, j));
	assert(mat != NULL);
	mat[(i - 1) * n + j - 1] = num;
}

float matrix::get(uint i, uint j)
{
	assert(valid_pos(i, j));
	assert(mat != NULL);
	return mat[(i - 1) * n + j - 1];
}

uint matrix::cant_rows()
{
	return n;
}

uint matrix::cant_cols()
{
	return m;
}

bool matrix::valid_pos(uint i, uint j)
{
	return (i > 0 && j > 0 && i <= n && j <= m);
}

// TODO: operator<< en vez de print (?)
string matrix::print(void)
{
	ostringstream ret;

	for (uint i = 1; i <= n; i++) {
		for (uint j = 1; j <= m; j++)
			ret << this->get(i, j) << "\t";

		/* Termino la fila, ponemos una nueva linea */
		ret << endl;
	}

	return ret.str();
}

/*
int main()
{
	matrix m(1, 2);
	m.set(1, 1, 3);
	std::cout << m.get(1, 1) << std::endl;
	std::cout << m.get(1, 2) << std::endl;
	return 0;
}
*/
