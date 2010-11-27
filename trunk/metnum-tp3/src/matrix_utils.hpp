#ifndef _MATRIX_UTILS_HPP
#define _MATRIX_UTILS_HPP

#include "matrix.hpp"

// Deja en Q y R la factorización QR de A
void givens(const matrix& A, matrix& Q, matrix& R);

void calcular_autovalores(const matrix& A_orig, matrix& Vect, matrix& Val);

bool esDiagonal(const matrix& M, double cota);

bool esTriangularSup(const matrix& M, double cota);

bool esSimetrica(const matrix& A);

#endif // ifdef _MATRIX_UTILS_HPP
