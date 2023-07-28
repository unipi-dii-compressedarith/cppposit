#include <iostream>
#include <vector>
using namespace std;

#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;
int main() {

    cout << "This demo shows the use of deep initialization of a posit" << endl;
	cout << "and the PositTrait class" << endl;

	using real = Posit<int8_t, 8, 0, uint16_t, PositSpec::WithNan>;
	    
	using PT = PositTrait<int8_t,8,0,PositSpec::WithNan>;
    
    // Let us create a posit, initialized to zero (POSIT_ZERO is not defined, being it equal to 0)
	real a(real::DeepInit(), 0);
	cout << dec << a << " -> " << oct << a << endl;

    // Let us create another posit, initialized to 1
	real b(real::DeepInit(), PT::POSIT_ONE);
	cout << dec << b << " -> " << oct << b << endl; 

    // Let us create another posit, initialized to two
	real c(real::DeepInit(), PT::POSIT_TWO);
	cout << dec << c << " -> " << oct << c << endl; 
    
    // Let us create another posit, initialized to MAXPOS
	real d(real::DeepInit(), PT::POSIT_MAXPOS);	
	cout << dec << d << " -> " << oct << d << endl; 

	// Let us create another posit, initialized to -1
	real e(real::DeepInit(), PT::POSIT_MONE);
	cout << dec << e << " -> " << oct << e << endl; 

    /* 
	THIS DOES NOT WORK: PLEASE FIX IT
	// Let us create another posit, initialized to -Inf
	real f(real::DeepInit(), PT::POSIT_NINF);	
	cout << dec << f << " -> " << oct << f << endl; 
	*/

    // Loop for printing all the positive posits
    real counter(real::DeepInit(), 0); // 
	for (int i = 0; i < 128; i++){
		cout << dec << i+1 << ": " << counter << " -> " << oct << counter << endl;
		counter = counter.next();
    }

	return 0;

}