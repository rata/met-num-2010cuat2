#include <stdlib.h>	// malloc, free y amigos
#include <assert.h>	// assert
#include <sstream>	// ostringstream
#include <string>	// std::string
#include <string.h>	// memcpy
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
	mat[(i - 1) * m + j - 1] = num;
}

float matrix::get(uint i, uint j)
{
	assert(valid_pos(i, j));
	assert(mat != NULL);
	return mat[(i - 1) * m + j - 1];
}

uint matrix::cant_rows()
{
	return n;
}

uint matrix::cant_cols()
{
	return m;
}

void matrix::swap_rows(uint i1, uint i2)
{
	assert(valid_pos(i1, i2));

	float *tmp = (float *) malloc(sizeof(float) * m);
	if (tmp == NULL)
		throw;

	float *i1_ptr = mat + (i1 - 1) * m;
	float *i2_ptr = mat + (i2 - 1) * m;

	// Copiamos la fila i1 a tmp
	void *ret = memcpy(tmp, i1_ptr, m);
	if (ret == NULL)
		goto error;

	// Copiamos la fila i2 a la fila i1
	ret = memcpy(i1_ptr, i2_ptr, m);
	if (ret == NULL)
		goto error;

	// Copiamos la fila i1 orig a la fila i2
	ret = memcpy(i2_ptr, tmp, m);
	if (ret == NULL)
		goto error;

	free(tmp);

error:
	free(tmp);
	// TODO: error mas claro
	throw;
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
		for (uint j = 1; j <= m; j++) {
			// TODO: En vez de agregar mas tabs deberiamos recortar
			// la presicion del float para que al imprimirlo no
			// "mueva" una fila de la matriz por mas largo que sea
			ret << this->get(i, j) << "\t\t\t";
		}

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
