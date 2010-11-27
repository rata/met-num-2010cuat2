#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	ifstream img;

	img.open("../imgs/120.gray", ifstream::in);

	if (img.fail()) {
		cout << "error al leer" << endl;
		return 2;
	}

//	while (! img.eof()) {
		cout << endl << "iter" << endl;
		for (int i=1; i <= 19; i++) {
			for (int j=1; j<= 11; j++) {
				if (img.eof()) {
					cout << "Fin de archivo!" << endl;
					return 0;
				}
				int byte = img.get();
				cout << byte << "\t";
			}
			cout << endl;
		}
//	}

	double test = 1e-17;
	cin >> test;

	cout << "test es: " << test << endl;
	if (test > 0)
		cout << "y es mayor a cero!" << endl;
	return 0;
}
