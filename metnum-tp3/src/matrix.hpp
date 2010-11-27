#ifndef _MATRIX_H
#define _MATRIX_H

#include <string>

class matrix
{
public:
	// Crea una matriz de n * m inicializada con ceros
	matrix(unsigned int n, unsigned int m);
	matrix();

	matrix(const matrix &m2);

	// identidad
	static matrix identity(unsigned int n);

	~matrix();

//	double operator[][] (int i, int j);
	matrix mult(const matrix& m) const;
	void mult(double alpha);
	void set(unsigned int i, unsigned int j, double num);
	double get(unsigned int i, unsigned int j) const;
	unsigned int cant_rows() const;
	unsigned int cant_cols() const;
	matrix transpose() const;
	void swap_rows(unsigned int i1, unsigned int i2);
	matrix& operator=(const matrix& m2);
	matrix columna(unsigned int i) const;

	std::string print(void);

private:
	// Tamaño
	unsigned int n;
	unsigned int m;

	// La matriz
	double *mat;

	bool valid_pos(unsigned int i, unsigned int j) const;
};

#endif // _MATRIX_H
