#include <stdlib.h>
#include <iostream>
#include <map>
#include "io_utils.hpp"

using namespace std;

void imprimir(int n, map<int, map<int,float> > &a)
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

void parsearW(int n, ifstream &file, map<int, map<int,float> > &a)
{
	string palabra;
	file >> palabra;
	int cant_links = atoi(palabra.c_str());

	cout << "links: " << cant_links << endl;

	string inicial, final;
	for ( int k = 0 ; k < cant_links; k++) {
		file >> inicial >> final;
	
		int i, j;
		j = atoi(inicial.c_str());
		i = atoi(final.c_str());
		if ( i != j)
			// El map es de columnas a map de filas, valor
			a[j][i] = 1;
	}
}
/*
float* jacobi(int n, map<int, map<int, float> > &a)
{
}

void normailzar(float* x)
{
}
*/
int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Uso: <pag-file> <link-file>" << endl;
		return 1;
	}

	char* pags_path = argv[1];
	char* link_path = argv[2];
	
	ifstream f_pag, f_link;

	if (leer(pags_path, &f_pag) != 0 || leer(link_path, &f_link))
		return 1;

	string palabra;
	f_pag >> palabra;
	int n = atoi(palabra.c_str());

	cout << "Paginas: " << n << endl;
	
	map<int, map<int, float> > a;
	

	parsearW(n, f_link, a);

//	imprimir(n, a);

	cerrar(&f_link);
	cerrar(&f_pag);
	
//	float[] x = jacobi(n, a);

//	normalizar(x);

	return 0;
}



