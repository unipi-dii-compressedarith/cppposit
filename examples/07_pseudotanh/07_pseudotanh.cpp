#include <iostream>
#include <cmath>
#include "posit.h"
using namespace std;
using namespace posit;
using Xa = Posit<int32_t,8,0,uint64_t,PositSpec::WithNan>;
//Posit<int32_t, 16 , 0, uint64_t, PositSpec::WithNan>;
typedef Xa myfl;

template <class T>
T tanh(T x) {
	return std::tanh(double(x));
}


int main() {
	myfl a[] = {0.90625f,
	 			0.32312f,
	 			0.333543f,
	 			0.2132144f,
	 			0.5213f,
	 			-0.3f,
	 			-0.2f,
	 			-0.1f,
	 			0.43f};
	 for(auto x: a)
		cout << x << "  " << double(x.pseudotanh()) << "  " << tanh(x) << std::endl;
	return 0;
	
}
