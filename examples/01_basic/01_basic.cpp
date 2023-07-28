

#include <iostream>
#include <vector>
using namespace std;
#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;

int main() {

    // The Posit contructor requires 4 arguments:
	// - the first is the class for the associated trait (it must be an integer type. It generally has the same number of bits of the posit itself)
	// - the second is the total number of bits for the posit
	// - the third argument is the number maximum number of bits for the exponent (it can be zero)
	// - the fourth is a specifier. It is an enumerate, defined within PositSpec namespace, and
	//   it can assume one of the values { WithNan, WithInf, WithNanInf }

    // A few examples are:
    //using real = Posit<int8_t, 8, 0, uint16_t, PositSpec::WithNan>;
	//using real = Posit<int16_t, 16, 1, uint32_t, PositSpec::WithNan>;
	//using real = Posit<int32_t, 32, 2, uint64_t, PositSpec::WithNan>;
	
	using real = Posit<int8_t, 8, 0, uint32_t, PositSpec::WithInfs>;

	//typedef double real; // uncomment this to use IEEE 754 double precision instead

	real a(1.23f);      // constucts a posit, using a float literal

	cout << a << endl;  // converts the posit a into a float and print it on screen

    a = -1.96f; 
	real d(1.96f);        // assigning a new value to a posit
	cout << a << " " << d << endl;
	
	real b(0.5003f);
	cout << b << endl;

	real c; // allocate memory for a new posit

	c = a + b; // compute the sum between two posits

	cout << c << endl; // print the result on screen
	
	cout << oct << c << endl; // print the posit to screen, using the unpacked representation

	return 0;

}

/*! \example 01_basic.cpp
    \brief A Documented file.
    
    Details.
*/
