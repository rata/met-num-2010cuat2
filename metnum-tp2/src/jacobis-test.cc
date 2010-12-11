#include <stdlib.h>
#include <iostream>
#include <map>
#include <assert.h>
#include "matrix.hpp"
#include "io_utils.hpp"
#include "vector_utils.hpp"

using namespace std;

double p;

typedef unsigned int uint;

void imprimir(int n, map<int, map<int,double> > &a)
{
	for ( int i = 1; i <= n; i++) {
		for (int j = 1; j <= n ; j++){
			if ( a.count(j) != 0 ) {
				if ( a[j].count(i) != 0 )
					cout << a[j][i] <<"\t";
				else
					cout << "0\t";
			}
			else 
				cout << "0\t";
		}
		cout << endl;
	}
}

matrix iterarJacobi(const map<int, map<int, double> > &a, matrix x, const matrix& b)
{
	matrix x_sig = b;

	// Aca acumulamos la diagonal para despues dividir al final
	matrix diagonal(b.cant_rows(), b.cant_cols());

	// A x_sig le voy restando los elementos de la matriz de la fila
	// correspondiente
	map<int, map<int, double> >::const_iterator it_col;
	map<int,double>::const_iterator it_fi;

	for (it_col = a.begin() ; it_col != a.end(); it_col++ ) {
		for ( it_fi = it_col->second.begin() ; it_fi != it_col->second.end(); it_fi++ ) {
			int i = it_fi->first;
			int j = it_col->first;
			cout << "fila: " << i << " columna: " << j << " valor: " << it_fi->second << endl;
			//cout << "x es: " << x.transpose().print();
			if (i == j) {
				diagonal.set(i, 1, it_fi->second);
				continue;
			}

			double elemento = (it_fi->second * x.get(j, 1));
			cout << "elemento : " << elemento << " ";
			cout << "it_fi->second : " << it_fi->second << " ";
			cout << "x.get(" << j << ", 1) : " << x.get(j, 1)<< endl;
			x_sig.set(i, 1, (x_sig.get(i, 1) - elemento));
		}
	}

	for (uint i = 1; i <= diagonal.cant_rows(); i++) {
		assert(diagonal.get(i, 1) != 0);
		x_sig.set(i, 1, x_sig.get(i, 1) / diagonal.get(i, 1));
	}

	return x_sig;
}

matrix jacobi(const map<int, map<int, double> > &a, const matrix& b, uint max_iter)
{
	uint iteracion = 0;
	//bool cambia = true;
	
	matrix x = b;
	//matrix x(b.cant_rows(), 1);

	while(iteracion < max_iter)
	{
		cout << iteracion << ":\n" << x.print();
		iteracion++;
	//	matrix buff = x;
		x = iterarJacobi(a, x, b);
	//	normalizar(n, x);
	//	cambia = distintos(n, buff, n, x);
	//	imprimir_vector(n, x);
	}
	//cout << "cambia: " << cambia << endl;
	return x;
}

int main(int argc, char *argv[])
{
	map<int, map<int, double> > a;
	int n = 3;

	// a[j][i]
	a[1][1] = 0.003;	a[2][1] = 0.017;	a[3][1] = 0.004;
	a[1][2] = -0.005;	a[2][2] = 0.002;	a[3][2] = 0.002;
	a[1][3] = 0.001;	a[2][3] = -0.003;	a[3][3] = -0.003;
	imprimir(n, a);

	// Propongo como solucion el vector nulo
	matrix r(n, 1);
	r.set(1, 1, -0.007);
	r.set(2, 1, -0.010);
	r.set(3, 1, 0.002);
	
	matrix x = jacobi(a, r, 200);

//	normalizar(n, x);

	cout << "Respuesta:\n";
	cout << x.print();

	return 0;
}
