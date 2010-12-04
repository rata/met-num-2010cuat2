#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include <string>	// str
#include "matrix.hpp"
#include "matrix_utils.hpp"
#include <fstream>
#include <iostream>

#define MENOR_CHAR 32
#define CANT_CARACT 92
#define PIXELES_H 19
#define PIXELES_W 11
#define PIXELES_IMG (PIXELES_H * PIXELES_W)

using namespace std;

typedef unsigned int uint;

void llenarFilaTransformacion(matrix& A , uint fila)
{
	assert(fila <= A.cant_rows());

	ifstream autovector;

	char path[100];
	sprintf(path, "../data/autoVal%u.dat", fila);
	autovector.open(path, ifstream::in);

	if (autovector.fail()) {
		cout << "error al leer autovalor: " << path << endl;
		exit(2);
	}

	uint lineas;
	autovector >> lineas;

	assert(lineas == A.cant_cols());

	double valor;
	// el primero es el valor del autovalor
	autovector >> valor;

	for (uint j = 1; j <= lineas; j++) {
		autovector >> valor;
		A.set(fila, j, valor);
	}

	autovector.close();
}

matrix leerCaracter(uint i, uint k)
{
	ifstream fcaracter;

	char path[100];
	sprintf(path, "../data/img%u.dat", i);
	fcaracter.open(path, ifstream::in);

	if (fcaracter.fail()) {
		cout << "error al leer: " << path << endl;
		exit(2);
	}

	uint lineas;
	fcaracter >> lineas;
	assert(k <= lineas);

	matrix caracter(k, 1);

	double valor;
	for (uint fila = 1; fila <= caracter.cant_rows(); fila++) {
		fcaracter >> valor;
		caracter.set(fila, 1, valor);
	}

	fcaracter.close();

	return caracter;
}

matrix leerImagen(string src, uint w, uint h)
{
	ifstream img;
	matrix res(PIXELES_IMG, w * h);

	img.open(src.c_str(), ifstream::in);
	if (img.fail()) {
		cout << "Error al leer: " << src << endl;
		exit(2);
	}

	for (uint fila = 1; fila <= h; fila++) {
		for (uint i = 1; i <= PIXELES_H; i++) {
			for (uint col = 1; col <= w; col++) {
				for (uint j = 1; j <= PIXELES_W; j++) {
					uint fila_img = j + (i - 1) * PIXELES_W;
					uint col_img = col + (fila - 1) * w;
					int byte = img.get();
					res.set(fila_img, col_img, byte);
				}
			}
		}
	}

	return res;
}

double calcular_distancia(const matrix& cuadrante, const matrix& vect)
{
	assert(cuadrante.cant_cols() == 1);
	assert(vect.cant_cols() == 1);

	double distancia = 0;
	for (uint i = 1; i <= cuadrante.cant_rows(); i++) {
		double penalidad = pow(1000, cuadrante.cant_rows() - i);
		//double penalidad = i;
		distancia += abs(cuadrante.get(i, 1) - vect.get(i, 1)) * penalidad;
	}

	return distancia;
}

char dame_caracter(const matrix& cuadrante, const vector<matrix>& caracteres)
{
	uint numero = 0;
	double distancia = calcular_distancia(cuadrante, caracteres[0]);
	for ( uint i = 1 ; i < caracteres.size() ; i++) {
		double new_dist = calcular_distancia(cuadrante, caracteres[i]);
		if ( new_dist < distancia ) {
			distancia = new_dist;
			numero = i;
		}
	}
	return MENOR_CHAR + numero;
}

void toASCII(const matrix& transformacion, const vector<matrix>& caracteres,
		uint w, uint h, string src, string dst)
{
	ofstream out;
	out.open(dst.c_str());
	if ( ! out.is_open() ) {
		cout << "Path inválido: "<< dst << endl;
                exit(4);
        }

	matrix imagen = leerImagen(src, w, h);
	matrix imgTransformada = transformacion * imagen;

	uint col = 1;
	for (uint i = 1; i <= h; i++) {
		for (uint j = 1; j <= w; j++) {
			assert( (j+(i-1)*w) == col);
			out << dame_caracter(imgTransformada.column(col),
					caracteres);
			col++;
		}
		out << endl;
	}
}

int main(int argc, char** argv)
{
	if (argc != 5 && argc != 6) {
		cout << "Usage: " << argv[0] << " <src> <width> <height> <dst> [k]"
			<< endl;
		return 1;
	}

	if (atoi(argv[2]) < 0 || atoi(argv[3]) < 0) {
		cout << "width y heigth deben ser positivos" << endl;
		return 1;
	}

	string src = argv[1];
	uint w = atoi(argv[2]);
	uint h = atoi(argv[3]);
	string dst = argv[4];
	uint k = (argc == 5)? 3 : atoi(argv[5]);

	// Matriz de los primero k AutoVectores
	matrix transformacion(k, PIXELES_IMG);

	for (uint i = 1 ; i <= transformacion.cant_rows() ; i++) {
		llenarFilaTransformacion(transformacion, i);
	}

	// Le digo caracter a los vectores de las imagenes de los caracteres ya
	// transformadas (contra las que voy a comparar)
	vector<matrix> caracteres;
	for ( int i = 1 ; i <= CANT_CARACT; i++) {
		matrix caracter = leerCaracter(i, k);
		caracteres.push_back(caracter);
	}

	toASCII(transformacion, caracteres, w, h, src, dst);

	return 0;
}
