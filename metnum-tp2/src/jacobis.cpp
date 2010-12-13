#include <stdlib.h>
#include <iostream>
#include <map>
#include <assert.h>
#include <math.h>
#include "matrix.hpp"
#include "matrix_utils.hpp"
#include "io_utils.hpp"

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
			(*it_fi).second = (*it_fi).second * c_j * -p;
		}
	}

	// ponemos unos en la diagonal
	for (int i = 1; i <= n; i++)
		a[i][i] = 1;
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
			//cout << "fila: " << i << " columna: " << j << " valor: " << it_fi->second << endl;
			//cout << "x es: " << x.transpose().print();
			if (i == j) {
				diagonal.set(i, 1, it_fi->second);
				continue;
			}

			double elemento = (it_fi->second * x.get(j, 1));
			//cout << "elemento : " << elemento << " ";
			//cout << "it_fi->second : " << it_fi->second << " ";
			//cout << "x.get(" << j << ", 1) : " << x.get(j, 1)<< endl;
			x_sig.set(i, 1, (x_sig.get(i, 1) - elemento));
		}
	}

	for (uint i = 1; i <= diagonal.cant_rows(); i++) {
		assert(diagonal.get(i, 1) != 0);
		x_sig.set(i, 1, x_sig.get(i, 1) / diagonal.get(i, 1));
	}

	return x_sig;
}

matrix jacobi(const map<int, map<int, double> > &a, const matrix& b, uint max_iter, double cota)
{
	uint iteracion = 0;
	bool cambia = true;
	
	matrix x = b;
	//matrix x(b.cant_rows(), 1);

	while ((iteracion < max_iter || max_iter == 0) && cambia) {
		cout << iteracion << ":\n" << x.transpose().print();
		iteracion++;

		matrix buff = x;
		x = iterarJacobi(a, x, b);
		cambia = !sonIguales(buff, x, norma2Vectorial, cota);
		//cout << "x_ant: "<< buff.transpose().print();
		//cout << "x_pos: "<< x.transpose().print();
	}
	//if ( iteracion >= max_iter)
	//	cout << "Se llegó a la cantidad máxima de iteraciones: "<< max_iter << endl;
	return x;
}

int main(int argc, char *argv[])
{
	if (argc != 3 && argc != 4 && argc != 5 ) {
		cout << "Uso: <pag-file> <link-file> [cota] [iteraciones_max = 100]" << endl;
		return 1;
	}
	char* pags_path = argv[1];
	char* link_path = argv[2];
	p = 0.85;

	double cota = 1e-8;
	if ( argc >= 4)
		cota = atof(argv[3]);
	
	ifstream f_pag, f_link;

	if (leer(pags_path, &f_pag) != 0 || leer(link_path, &f_link))
		return 1;

	string palabra;
	f_pag >> palabra;
	int n = atoi(palabra.c_str());
	
	int max_iter = pow(n,2);

	if ( argc >= 5 )
		max_iter = atoi(argv[4]);
//	cout << "Paginas: " << n << endl;
	
	map<int, map<int, double> > a;
	

	parsearW(n, f_link, a);

	//imprimir(n, a);

	cerrar(&f_link);
	cerrar(&f_pag);
	matrix b(n, 1);
	for (unsigned int i = 1; i <= b.cant_rows(); i++)
		b.set(i, 1, 1);
	
	matrix x = jacobi(a, b, max_iter, cota);

	//normalizar(n, x);

	//cout << "Respuesta:\n";
	//cout << x.transpose().print();

	//cout << "Respuesta Normalizada:\n";
	//cout << normalizar_vector(x).transpose().print();
	return 0;
}



