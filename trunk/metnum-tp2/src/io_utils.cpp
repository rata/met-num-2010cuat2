#include "io_utils.hpp"
#include <iostream>

using namespace std;

int leer(char* pags_path, ifstream* fpag ) 
{
        fpag->open(pags_path , ifstream::in);

        if ( ! fpag->good() ) {
		cout << "Path inválido: "<< pags_path << endl;
                return 1;
        }
        return 0;
}

void cerrar(ifstream* file)
{
	file->close();
}

