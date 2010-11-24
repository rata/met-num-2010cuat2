#include <stdlib.h>	// malloc, free y amigos
#include <assert.h>	// assert
#include <sstream>	// ostringstream
#include <string>	// std::string
#include <string.h>	// memcpy
#include <cmath>	// abs(double)
#include "matrix.hpp"
#include "convenciones.hpp"
#include <iostream>

using namespace std;
// gay!

typedef unsigned int uint;

matrix::matrix()
:n(0), m(0), mat(NULL)
{
	//cout << "cons defecto" << endl;
}

matrix::matrix(uint i, uint j)
:n(i), m(j), mat(NULL)
{
	// Allocamos memoria para n*m elementos de tamaño sizeof(double)
	// Usamos calloc para inicializar con ceros
	mat = (double *) calloc(sizeof(double), n * m);

	// TODO: error mas claro
	if (mat == NULL) {
		//cout << "sin memoria" << endl;
		throw;
	}
}

matrix matrix::identity(uint n)
{
	matrix m(n, n);
	for (uint i = 1; i <= n; i++)
		m.set(i, i, 1);
	return m;
}

matrix::matrix(const matrix &m2)
:n(0), m(0), mat(NULL)
{
	//cout << "copy constructor" << endl;
	*this = m2;
}

matrix::~matrix()
{
	//cout << "destructor" << endl;
	free(this->mat);
	this->mat = NULL;
}

matrix& matrix::operator=(const matrix& m2)
{
	//cout << "operator=!!" << endl;
	this->n = m2.n;
	this->m = m2.m;

	if (this->mat == m2.mat) {
		//cout << "self assignment" << endl;
		return *this;
	}

	free(this->mat);
	this->mat = (double *) malloc(sizeof(double) * n * m);
	memcpy(this->mat, m2.mat, sizeof(double) * n * m);

	return *this;
}

void matrix::set(uint i, uint j, double num)
{
	assert(valid_pos(i, j));
	assert(mat != NULL);
	mat[(i - 1) * m + j - 1] = num;
}

double matrix::get(uint i, uint j) const
{
	assert(valid_pos(i, j));
	assert(mat != NULL);
	return mat[(i - 1) * m + j - 1];
}

uint matrix::cant_rows() const
{
	return n;
}

uint matrix::cant_cols() const
{
	return m;
}


// ""this * A""
matrix matrix::mult(const matrix& a)
{
	assert(a.cant_rows() == this->cant_cols());

	matrix res(this->cant_rows(), a.cant_cols());
//	matrix res(2, 2);

	for (uint i = 1; i <= this->cant_rows(); i++) {
		for (uint j = 1; j <= a.cant_cols(); j++) {
			double val = 0;

			for (uint k = 1; k <= a.cant_rows(); k++) {
				val += this->get(i, k) * a.get(k, j);
			}
			if ( abs(val) < convenciones::tolerancia()) {
				val = 0;
			}
			res.set(i, j, val);
		}
	}

	return res;
}

matrix matrix::transpose() const
{
	// La traspuesta es de mxn
	matrix transp(this->cant_cols(), this->cant_rows());
	for(uint i = 1; i <= cant_rows(); i++) {
		for (uint j = 1; j <= cant_cols(); j++) {
			transp.set(j, i, this->get(i,j));
		}
	}
	return transp;
}

void matrix::swap_rows(uint i1, uint i2)
{
	assert(valid_pos(i1, i2));

	double *tmp = (double *) malloc(sizeof(double) * m);
	if (tmp == NULL) {
		// TODO: error mas claro
		throw;
	}

	double *i1_ptr = mat + (i1 - 1) * m;
	double *i2_ptr = mat + (i2 - 1) * m;

	// Copiamos la fila i1 a tmp
	memcpy(tmp, i1_ptr, m);

	// Copiamos la fila i2 a la fila i1
	memcpy(i1_ptr, i2_ptr, m);

	// Copiamos la fila i1 orig a la fila i2
	memcpy(i2_ptr, tmp, m);

	free(tmp);
}

bool matrix::valid_pos(uint i, uint j) const
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
			// la presicion del double para que al imprimirlo no
			// "mueva" una fila de la matriz por mas largo que sea
			ret << this->get(i, j) << "\t";
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
