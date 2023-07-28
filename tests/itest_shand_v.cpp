#include "posit.h"
#include <iostream>
using namespace posit;
using X32 = Posit<int32_t, 32 , 0, uint32_t, PositSpec::WithNan>;
using X16 = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;
using X8 = Posit<int8_t, 8 , 0, uint8_t, PositSpec::WithNan>;

int main(int argc, char const *argv[])
{
	float x = atof(argv[1]);
	std::cout << "8)  "<< x << " -> " << int((X8(x)).v) << std::endl;
	std::cout << "16) "<< x << " -> " << int((X16(x)).v) << std::endl;
	std::cout << "32) "<< x << " -> " << int((X32(x)).v) << std::endl;
	return 0;
}