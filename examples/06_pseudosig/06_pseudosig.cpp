#include <iostream>
#include <cmath>
#include "../../include/posit.h" // include non-tabulated posits
using namespace std;
using namespace posit;
using Xa = Posit<int16_t, 16, 0, uint16_t, PositSpec::WithNan>;
typedef Xa myfl;


myfl sigmoid(myfl x) {
	double a = double(x);
	return 1/(1+std::exp(-a));
}


int main() {
	myfl a[] = {myfl(0.90625f)
	 ,myfl(0.2f)
	 ,myfl(0.3f)
	 ,myfl(0.4f)
	 ,myfl(0.43f)};
	cout << "Number" << "   " << "Pseudosigmoid" << "   " << "Sigmoid  " << "   " << "Difference" << "\n";
	for(int i = 0; i < 5; ++i)
		cout << a[i] << "   " << double(a[i].pseudosigmoid()) << "   " << double(sigmoid(a[i])) << "   " << double(sigmoid(a[i]))-double(a[i].pseudosigmoid())<< "\n";
	for(int i = 0; i < 5; ++i)
		cout << -a[i] << "   " << double((-a[i]).pseudosigmoid()) << "   " << double(sigmoid(-a[i])) << "   " << double(sigmoid(-a[i]))-double((-a[i]).pseudosigmoid())<< "\n";

	return 0;
}
