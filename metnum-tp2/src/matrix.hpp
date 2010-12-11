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
	matrix operator*(const matrix& m) const;
	matrix operator*(double alpha) const;
	matrix operator+(const matrix& a) const;
	matrix operator-(const matrix& a) const;
	void scale(double alpha);
	void set(unsigned int i, unsigned int j, double num);
	double get(unsigned int i, unsigned int j) const;
	unsigned int cant_rows() const;
	unsigned int cant_cols() const;
	matrix transpose() const;
	matrix diagonal() const;
	void swap_rows(unsigned int i1, unsigned int i2);
	void swap_cols(unsigned int c1, unsigned int c2);
	matrix& operator=(const matrix& m2);
	matrix column(unsigned int i) const;

	std::string print(void) const;

private:
	// Tamaño
	unsigned int n;
	unsigned int m;

	// La matriz
	double *mat;

	bool valid_pos(unsigned int i, unsigned int j) const;
};

#endif // _MATRIX_H
