#include <iostream>	// cout
#include <assert.h>	// assert
#include <stdlib.h>	// malloc, free
#include <cmath>	// abs(double)
#include <math.h>	// sqrt
#include <vector>	// vector
#include <fstream>	// ifstream
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

matrix covarianza(vector<matrix> X)
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
	cout << M.print();
	matrix Vect(M.cant_rows(), M.cant_cols());
	matrix Val(M.cant_rows(), M.cant_cols());
	calcular_autovalores(M, Vect, Val, 1e-3);

	cout << "primer autovalor: " << Val.get(1, 1) << endl;
	cout << "calculado" << endl;
	for (uint i = 1; i <= M.cant_rows(); i++) {
		cout << "autov " << i << "------------------------------------" << endl;
		double lambda = Val.get(i, i);
		matrix v = Vect.column(i);
		cout << "v es: " << v.transpose().print() << endl;
		cout << "M * v " << endl << (M * v).transpose().print() << endl;
		v.scale(lambda);
		cout << "lambda* v" << endl << v.transpose().print() << endl;

		cout << "autov " << i <<
			" finished----------------------------------------" << endl;
	}



//	for (uint i = 0; i < imgs.size(); i++) {
//		cout << "Matriz " << i << endl << imgs[i].transpose().print();
//	}
//	cout << src << endl<< w <<endl << h << endl << dst << endl;

	return 0;
}
