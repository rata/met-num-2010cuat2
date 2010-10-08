#include "vector_utils.hpp"
#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void imprimir_vector(int n, float* x)
{
	cout << "[";
	for ( int i= 0 ; i < n-1 ; i++)
	{
		printf("%g, ", x[i]);
//		cout << x[i] << ", ";
	}
	printf("%g ]\n", x[n-1]);
//	cout << x[n-1] << "]" << endl;
}

void normalizar(int n, float* x)
{
	float norma = 0;

	for ( int i = 0 ; i < n ; i++ ) {
		norma += fabs(x[i]);
//		cout << "valor de x: "<< x[i] << ". Valor de abs de x: "<< fabs(x[i]) << endl;
	}

//	cout << "Norma: " << norma;

	if ( norma != 0)
	{
		for ( int i = 0 ; i < n ; i++ ) {
			x[i] = x[i]/norma;
		}
	}
}

bool distintos(int n1, float* x1, int n2, float* x2)
{
	if ( n1 != n2 )
		return true;

	for ( int i = 0 ; i < n1 ; i++) {
		if ( x1[i] != x2[i] )
			return true;
	}

	return false;
}

