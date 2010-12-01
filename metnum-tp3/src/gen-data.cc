#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include <vector>	// vector
#include <fstream>	// ifstream / ofstream
#include <algorithm>	// sort
#include "convenciones.hpp" // dataDirectory
#include "matrix.hpp"
#include "matrix_utils.hpp"

using namespace std;

typedef unsigned int uint;

matrix gen_vector(string img_path, uint n)
{
	ifstream img;
	matrix res(n, 1);

	img.open(img_path.c_str(), ifstream::in);
	if (img.fail()) {
		cout << "Error al leer: " << img_path << endl;
		exit(2);
	}

	for (uint i = 1; i <= n; i++) {
		if (img.eof()) {
			cout << "Error al leer: " << img_path << " Mal el tamaño ?" << endl;
			exit(3);
		}

		int byte = img.get();
		res.set(i, 1, byte);
	}

	return res;
}

matrix vect_promedio(const vector<matrix> l)
{
	assert(l.size() >= 1);

	// Creo la matriz con ceros
	matrix res(l[0].cant_rows(), l[0].cant_cols());
	uint n = l.size();

	for (uint i = 0; i < n; i++) {
		res = res + l[i];
	}

	res.scale(1.0/n);

	return res;
}

matrix covarianza(const vector<matrix> X)
{
	assert(X.size() >= 1);

	// Creo la matriz con ceros
	matrix M(X[0].cant_rows(), X[0].cant_rows());

	uint m = X.size();
	matrix mu = vect_promedio(X);

	for (uint i = 0; i < m; i++) {
		matrix x = (X[i] - mu);
		M = M + (x * x.transpose());
	}

	M.scale(1.0/m);

	return M;
}

void escribirArchivoAutovalor(const char* nombre, double autovalor, matrix autovector)
{
	ofstream myfile;
	myfile.open(nombre);

	if ( ! myfile.is_open() ) {
		cout << "Path inválido: "<< nombre << endl;
                exit(4);
        }

	myfile << autovector.cant_rows() << endl;
	myfile << autovalor << endl;

	for (uint i = 1 ; i < autovector.cant_rows() ; i++)
		myfile << autovector.get(i,1) << endl;

	myfile.close();


	cout << nombre << " ------------------------------------" << endl;
	cout << "autovalor: " << autovalor << endl;
	cout << "v es: " << autovector.transpose().print() << endl;
	cout << nombre << " finished ---------------------------" << endl;
}

void escribirVectorAArchivo(const matrix& vect, char* nombre)
{
	assert(vect.cant_cols() == 1);

	ofstream myfile;
	myfile.open(nombre);

	if ( ! myfile.is_open() ) {
		cout << "Path inválido: "<< nombre << endl;
                exit(4);
        }

	myfile << vect.cant_rows() << endl;

	for ( uint i = 1 ; i < vect.cant_rows() ; i++ ) {
		myfile << vect.get(i,1) << endl;
	}

	myfile.close();
}

bool sort_autovect(pair<double, matrix> i, pair<double, matrix> j)
{
	return i.first <= j.first;
}

int main(int argc, char** argv)
{
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <width> <heigth> <img1> [img2] ... [imgN]" << endl;
		return 1;
	}

	if (atoi(argv[1]) < 0 || atoi(argv[2]) < 0) {
		cout << "width y heigth deben ser positivos" << endl;
		return 1;
	}

	uint w = atoi(argv[1]);
	uint h = atoi(argv[2]);

	// A la cantidad de datos le decimos m
	//uint m = argc - 3;
	uint n = w * h;

	vector<matrix> imgs;
	for (int i = 3; i < argc; i++) {
		string img = argv[i];
		imgs.push_back(gen_vector(img, n));
	}

	matrix M = covarianza(imgs);
//	cout << M.print();
	matrix Vect(M.cant_rows(), M.cant_cols());
	matrix Val(M.cant_rows(), M.cant_cols());
	calcular_autovalores(M, Vect, Val, 1e-7);

	// TODO: Hay que oredenar los autovalores y autocetores!!!

//	cout << "calculado" << endl;

	string nombre = dataDirectory() + "/autoVal";
	vector<pair<double, matrix> > autoval;
	for (uint i = 1; i <= M.cant_rows(); i++) {
		double autovalor = Val.get(i, i);
		matrix autovector = Vect.column(i);

		// TODO: ver el constructor de pair para pasarle directamente a
		// .push_back
		pair<double, matrix> p;
		p.first = autovalor;
		p.second = autovector;
		autoval.push_back(p);

		char path[100];
		sprintf(path, "%s%u.dat", nombre.c_str(), i);

		escribirArchivoAutovalor(path, autovalor, autovector);

//		cout << "M * v " << endl << (M * autovector).transpose().print() << endl;
//		autovector.scale(autovalor);
//		cout << "lambda* v" << endl << autovector.transpose().print() << endl;
	}

	// TODO: probar que efectivamente funcione y que no la cague
	// TODO: capaz que calcular_autovalores podria devolver el par, o el par
	// ordenado. Asi no copiamos tantas cosas (si es que vale la pena
	// ahorrarse las copiadas y cambiarle la signatura)
	sort(autoval.begin(), autoval.end(), sort_autovect);

	nombre = dataDirectory() + "/img";
	// Ojo con esto que pude estar mal!!
	matrix transformacion = Vect.transpose();
	for ( uint i = 0 ; i < imgs.size() ; i++ ) {
		matrix transformada = transformacion * imgs[i];
		char path[100];
		sprintf(path, "%s%u.dat", nombre.c_str(), i);
		escribirVectorAArchivo(transformada, path);
	}

	return 0;
}
