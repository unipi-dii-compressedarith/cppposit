#include "posit.h"
#include <iostream>
#include <vector>
using namespace posit;
using P8a = posit::Posit<int16_t, 8 , 2, uint_fast32_t, posit::PositSpec::WithNan>;


int main() {
	P8a a(.f);
	std::cout << a << std::endl;
}
