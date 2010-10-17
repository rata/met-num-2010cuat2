#ifndef _MATRIX_H
#define _MATRIX_H

#include <string>

class matrix
{
public:
	// Crea una matriz de n * m inicializada con ceros
	matrix(unsigned int n, unsigned int m);
	~matrix();

//	double operator[][] (int i, int j);
	void set(unsigned int i, unsigned int j, double num);
	double get(unsigned int i, unsigned int j);
	unsigned int cant_rows();
	unsigned int cant_cols();
	void swap_rows(unsigned int i1, unsigned int i2);
	matrix operator=(const matrix m2);

	std::string print(void);

private:
	// Tamaño
	unsigned int n;
	unsigned int m;

	// La matriz
	double *mat;

	bool valid_pos(unsigned int i, unsigned int j);
};

#endif // _MATRIX_H
