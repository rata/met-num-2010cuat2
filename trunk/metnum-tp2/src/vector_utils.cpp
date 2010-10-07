#include "vector_utils.hpp"
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
}


