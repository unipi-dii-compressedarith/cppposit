#include <iostream>
#include <vector>
using namespace std;

#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;
int main() {

	//using real = Posit<int8_t, 8, 3, uint16_t, PositSpec::WithNan>;
    // using real = Posit<int16_t, 16, 1, uint32_t, PositSpec::WithNan>;
	using real = Posit<int32_t, 32, 2, uint64_t, PositSpec::WithNan>;
		
	// Let us define a vector of posits of length DIM
	const int DIM = 5;
	vector <real> v(DIM);

	v[0] = int(1);
	v[1] = int(2);
	v[2] = 0.52f;
	v[3] = 0.53f;
	v[4] = 0.50003f;

    // Let us print it on screen
	cout << endl<< endl << "Vector v: " << endl;
	cout << endl << "[ ";
	for (int i = 0; i < DIM; i++)
	   cout << v[i] <<' ';
	cout << "]" << endl;


    // Let us define a rectangular matrix of size DIM x (DIM-1)
	vector<vector<real>> M(DIM, std::vector<real>(DIM - 1));

    // Let us define a rectangular matrix of size DIM x (DIM-1)
	cout << endl<< endl << "Matrix M: " << endl;
	for(int i = 0; i < DIM; i++){
		for (int j = 0; j < DIM - 1; j++) {
			M[i][j] = (i + j) + 0.2;
			cout << float(M[i][j]) << ' ';
		}
		cout << endl;
	}	

	return 0;
}