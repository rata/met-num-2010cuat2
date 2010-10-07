#include <stdlib.h>
#include <iostream>
#include <map>
#include "io_utils.hpp"

using namespace std;

float p;

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

float cj(int j, map<int, map<int,float> > &a)
{
	uint cant = a.count(j) == 0 ? 0 : a[j].size();
	if ( cant == 0)
		return 0.0;
	else
		return 1.0/cant;
}

void parsearW(int n, ifstream &file, map<int, map<int,float> > &a)
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

	map<int, map<int, float> >::iterator it_col;
	map<int,float>::iterator it_fi;

	// Para cada columna
	for ( it_col = a.begin() ; it_col != a.end(); it_col++ ) {
		for ( it_fi = (*it_col).second.begin() ; it_fi != (*it_col).second.end(); it_fi++ ) {
			float c_j = cj((*it_fi).first, a);
			//Esta linea me suena a que no anda ni en pedo :S
			(*it_fi).second = (*it_fi).second * c_j;
			cout << c_j << endl;
		}
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
	if (argc != 4) {
		cout << "Uso: <prob> <pag-file> <link-file>" << endl;
		return 1;
	}
	p = atof(argv[1]);
	char* pags_path = argv[2];
	char* link_path = argv[3];
	
	ifstream f_pag, f_link;

	if (leer(pags_path, &f_pag) != 0 || leer(link_path, &f_link))
		return 1;

	string palabra;
	f_pag >> palabra;
	int n = atoi(palabra.c_str());

//	cout << "Paginas: " << n << endl;
	
	map<int, map<int, float> > a;
	

	parsearW(n, f_link, a);

	imprimir(n, a);

	cerrar(&f_link);
	cerrar(&f_pag);
	
//	float* x = jacobi(n, a);

//	normalizar(x);

	return 0;
}



