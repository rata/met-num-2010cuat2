#ifndef _MATRIX_H
#define _MATRIX_H

#include <string>

class matrix
{
public:
	matrix(unsigned int n, unsigned int m);
	~matrix();
//	float operator[][] (int i, int j);
	void set(unsigned int i, unsigned int j, float num);
	float get(unsigned int i, unsigned int j);
	unsigned int cant_rows();
	unsigned int cant_cols();
	std::string print(void);

private:
	// Tamaño
	unsigned int n;
	unsigned int m;

	// La matriz
	float *mat;

	bool valid_pos(unsigned int i, unsigned int j);
};

#endif // _MATRIX_H
