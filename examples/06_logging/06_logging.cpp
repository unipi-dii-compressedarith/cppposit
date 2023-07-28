#include <iostream>
#include <cmath>
#include "../../include/posit.h"
using namespace std;
using namespace posit;

using myreal = Posit<int16_t, 8, 0, uint16_t, PositSpec::WithNan>;

/**
 * The posit class has a built-in value logger to trace  
 * the values that are assigned to the class during 
 * the execution of a given portion of code.
 * 
 * 
 * 
 * 
 * 
**/
int main() {
	myreal a;
	// 1. Enable the logger at the beginning of the portion of code to be traced
	myreal::_logger.setLogLevel(LogLevel::ALL);
	for(int i = 0; i < 100; ++i) {
		a = myreal((double(i)/100));
	}

	// 2. Complete tracing by saving the traced values
	myreal::_logger.save("a_logged.txt");
}
