#ifndef _MATRIX_UTILS_HPP
#define _MATRIX_UTILS_HPP

#include "matrix.hpp"
#include <vector>	// vector

matrix tc(const std::vector<matrix>& autoVectores, const matrix& x);

// Deja en Q y R la factorización QR de A
void givens(const matrix& A, matrix& Q, matrix& R);

void calcular_autovalores(const matrix& A_orig, matrix& Vect, matrix& Val, double tol);

bool esDiagonal(const matrix& M, double cota);

bool esTriangularSup(const matrix& M, double cota);

bool esSimetrica(const matrix& A);

bool sonIguales(const matrix& A1, const matrix& A2, double (*norma) (const matrix&), double cota);

bool esVector(const matrix& mat);

double norma2Vectorial(const matrix& vect);

double norma1Vectorial(const matrix& vect);

double normaInfVectorial(const matrix& vect);

matrix back_substitution(matrix &m);

void lu_triang(matrix &m);

matrix normalizar_vector(const matrix& vector);

#endif // ifdef _MATRIX_UTILS_HPP
