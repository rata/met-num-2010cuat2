#include "matrix.hpp"
#include "matrix_utils.hpp"
#include "vector_utils.hpp"
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{
	matrix A(3, 4);

	A.set(1,1,-5);	A.set(1,2,2);	A.set(1,3,2);	A.set(1,4,-10);

	A.set(2,1,3);	A.set(2,2,17);	A.set(2,3,4);	A.set(2,4,-7);

	A.set(3,1,1);	A.set(3,2,-3);	A.set(3,3,-3);	A.set(3,4,2);

	lu_triang(A);

	int n = 3;
	double *res = (double *) malloc(sizeof(double) * n);
	if (res == NULL)
		throw;

	// Y resolvemos!
	back_substitution(A, res);
	cout << "respuesta: ";
	imprimir_vector(n, res);
	normalizar(n, res);
	cout << "respuesta normalizada: ";
	imprimir_vector(n, res);

	free(res);
	return 0;
}
