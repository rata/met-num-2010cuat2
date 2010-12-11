#include <stdlib.h>	// malloc, free y amigos
#include <assert.h>	// assert
#include <sstream>	// ostringstream
#include <string>	// std::string
#include <string.h>	// memcpy
#include <cmath>	// abs(double)
#include <iostream>
#include "matrix.hpp"
#include "convenciones.hpp"

typedef unsigned int uint;

matrix::matrix()
:n(0), m(0), mat(NULL)
{

}

matrix::matrix(uint i, uint j)
:n(i), m(j), mat(NULL)
{
	// Allocamos memoria para n*m elementos de tamaño sizeof(double)
	// Usamos calloc para inicializar con ceros
	mat = (double *) calloc(sizeof(double), n * m);

	// TODO: error mas claro
	if (mat == NULL)
		throw;
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
	*this = m2;
}

matrix::~matrix()
{
	free(this->mat);
	this->mat = NULL;
}

matrix& matrix::operator=(const matrix& m2)
{
	this->n = m2.n;
	this->m = m2.m;

	if (this->mat == m2.mat)
		return *this;

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

matrix matrix::operator-(const matrix& a) const
{
	// Duplicamos el codigo para evitar copias de a (que es const)
	assert(a.cant_rows() == this->cant_rows());
	assert(a.cant_cols() == this->cant_cols());

	matrix res(this->cant_rows(), this->cant_cols());

	for (uint i = 1; i <= this->cant_rows(); i++) {
		for (uint j = 1; j <= this->cant_cols(); j++) {
			double val = this->get(i, j) - a.get(i, j);
			res.set(i, j, val);
		}
	}

	return res;
}

matrix matrix::operator+(const matrix& a) const
{
	assert(a.cant_rows() == this->cant_rows());
	assert(a.cant_cols() == this->cant_cols());

	matrix res(this->cant_rows(), this->cant_cols());

	for (uint i = 1; i <= this->cant_rows(); i++) {
		for (uint j = 1; j <= this->cant_cols(); j++) {
			double val = this->get(i, j) + a.get(i, j);
			res.set(i, j, val);
		}
	}

	return res;
}

// ""this * A""
matrix matrix::operator*(const matrix& a) const
{
	assert(a.cant_rows() == this->cant_cols());

	matrix res(this->cant_rows(), a.cant_cols());

	for (uint i = 1; i <= this->cant_rows(); i++) {
		for (uint j = 1; j <= a.cant_cols(); j++) {
			double val = 0;

			for (uint k = 1; k <= a.cant_rows(); k++) {
				val += this->get(i, k) * a.get(k, j);
			}

			//if (abs(val) < tolerancia())
			//	val = 0;

			res.set(i, j, val);
		}
	}

	return res;
}
matrix matrix::operator*(double alpha) const
{
	matrix scaled = *this;
	scaled.scale(alpha);
	return scaled;
}

void matrix::scale(double alpha)
{
	std::cout << "escalando con " << alpha << std::endl;
	for ( uint i = 1 ; i <= this->cant_rows() ; i++) {
		for ( uint j = 1 ; j <= this->cant_cols() ; j++) {
			double val = this->get(i,j);
			val *= alpha;
			this->set(i,j,val);
		}
	}
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

matrix matrix::diagonal() const
{
	// La traspuesta es de mxn
	matrix diag(this->cant_rows(), this->cant_cols());
	for(uint i = 1; i <= cant_rows() && i <= cant_cols(); i++) {
		diag.set(i, i, this->get(i,i));
	}
	return diag;
}

matrix matrix::column(uint j) const
{
	assert(valid_pos(1, j));

	matrix columna(this->cant_rows(), 1);

	for (uint i = 1 ; i <= this->cant_rows() ; i++ ) {
		columna.set(i, 1, this->get(i,j));
	}

	return columna;
}

void matrix::swap_cols(uint c1, uint c2)
{
	assert(valid_pos(1, c1));
	assert(valid_pos(1, c2));

	for ( uint i = 1 ; i <= this->cant_rows() ; i++ ) {
		double tmp = this->get(i, c1);
		this->set(i, c1, this->get(i, c2));
		this->set(i, c2, tmp);
	}
}

void matrix::swap_rows(uint i1, uint i2)
{
	assert(valid_pos(i1, 1));
	assert(valid_pos(i2, 1));

	double *tmp = (double *) malloc(sizeof(double) * m);
	if (tmp == NULL) {
		// TODO: error mas claro
		throw;
	}

	double *i1_ptr = mat + (i1 - 1) * m;
	double *i2_ptr = mat + (i2 - 1) * m;

	// Copiamos la fila i1 a tmp
	memcpy(tmp, i1_ptr, sizeof(double) * m);

	// Copiamos la fila i2 a la fila i1
	memcpy(i1_ptr, i2_ptr, sizeof(double) * m);

	// Copiamos la fila i1 orig a la fila i2
	memcpy(i2_ptr, tmp, sizeof(double) * m );

	free(tmp);
}

bool matrix::valid_pos(uint i, uint j) const
{
	return (i > 0 && j > 0 && i <= n && j <= m);
}

// TODO: operator<< en vez de print (?)
std::string matrix::print(void) const
{
	using namespace std;
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
