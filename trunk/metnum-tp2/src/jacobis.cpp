#include <stdlib.h>
#include <iostream>
#include <map>
#include "io_utils.hpp"
#include "vector_utils.hpp"

using namespace std;

double p;

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

double cj(int j, map<int, map<int,double> > &a)
{
	uint cant = a.count(j) == 0 ? 0 : a[j].size();
	if ( cant == 0)
		return 0.0;
	else
		return 1.0/cant;
}

void parsearW(int n, ifstream &file, map<int, map<int,double> > &a)
{
	string palabra;
	file >> palabra;
	int cant_links = atoi(palabra.c_str());

//	cout << "Links: " << cant_links << endl;

	string inicial, final;
	for ( int k = 0 ; k < cant_links; k++) {
		file >> inicial >> final;
	
		int i, j;
		j = atoi(inicial.c_str());
		i = atoi(final.c_str());
		if ( i != j) {
			// El map es de columnas a map de filas, valor
			a[j][i] = 1;
		}
	}
	// Como calcular cj no es tan terrible, voy a iterar los mapas
	// y calcular el cj en cada paso

	map<int, map<int, double> >::iterator it_col;
	map<int,double>::iterator it_fi;

	// Para cada columna
	for ( it_col = a.begin() ; it_col != a.end(); it_col++ ) {
		for ( it_fi = (*it_col).second.begin() ; it_fi != (*it_col).second.end(); it_fi++ ) {
			double c_j = cj((*it_col).first, a);
			//Esta linea me suena a que no anda ni en pedo :S
			(*it_fi).second = (*it_fi).second * c_j * p;
		}
	}
}

double* iterarJacobi(int n, map<int, map<int, double> > &a, double* x)
{
	double* x_sig = (double *) malloc(sizeof(double) * n);
	
	// Inicializo con 1
	for ( int i = 0 ; i < n ; i++)
		x_sig[i] = 1;

	// A x_sig le voy restando los elementos de la matriz de la fila
	// correspondiente
	map<int, map<int, double> >::iterator it_col;
	map<int,double>::iterator it_fi;

	for ( it_col = a.begin() ; it_col != a.end(); it_col++ ) {
		for ( it_fi = (*it_col).second.begin() ; it_fi != (*it_col).second.end(); it_fi++ ) {
			int i = (*it_fi).first;
			double elemento = ((*it_fi).second * x[i-1]);
			x_sig[i-1] = (x_sig[i-1] + elemento);
		}
	}

	return x_sig;
}

double* jacobi(int n, map<int, map<int, double> > &a, int max_iter)
{
	// Propongo como solucion el vector nulo
	double* x = (double *) calloc(sizeof(double), n);
	int iteracion = 0;
	bool cambia = true;

	while(iteracion < max_iter && cambia)
	{
		iteracion++;
		double* buff = x;
		x = iterarJacobi(n, a, x);
	//	normalizar(n, x);
		cambia = distintos(n, buff, n, x);
		cout << iteracion <<":\n";
		imprimir_vector(n, x);
		free(buff);
	}
	cout << "cambia: " << cambia << endl;
	return x;
}

int main(int argc, char *argv[])
{
	if (argc != 4 && argc != 5 ) {
		cout << "Uso: <prob> <pag-file> <link-file> [iteraciones_max = 100]" << endl;
		return 1;
	}
	p = atof(argv[1]);
	char* pags_path = argv[2];
	char* link_path = argv[3];
	int max_iter = 100;
	
	if ( argc == 5 )
		max_iter = atoi(argv[4]);

	ifstream f_pag, f_link;

	if (leer(pags_path, &f_pag) != 0 || leer(link_path, &f_link))
		return 1;

	string palabra;
	f_pag >> palabra;
	int n = atoi(palabra.c_str());

//	cout << "Paginas: " << n << endl;
	
	map<int, map<int, double> > a;
	

	parsearW(n, f_link, a);

	imprimir(n, a);

	cerrar(&f_link);
	cerrar(&f_pag);
	
	double* x = jacobi(n, a, max_iter);

	normalizar(n, x);

	cout << "Respuesta:\n";
	imprimir_vector(n, x);
	free(x);

	return 0;
}



