#include "vector_utils.hpp"
#include <math.h>
#include <iostream>

using namespace std;

void imprimir_vector(int n, float* x)
{
	cout << "[";
	for ( int i= 0 ; i < n-1 ; i++)
	{
		cout << x[i] << ", ";
	}
	cout << x[n-1] << "]" << endl;
}

void normalizar(int n, float* x)
{
	float norma = 0;

	for ( int i = 0 ; i < n ; i++ ) {
		norma += fabs(x[i]);
		cout << "valor de x: "<< x[i] << ". Valor de abs de x: "<< fabs(x[i]) << endl;
	}

	cout << "Norma: " << norma;

	if ( norma != 0)
	{
		for ( int i = 0 ; i < n ; i++ ) {
			x[i] = x[i]/norma;
		}
	}
}


