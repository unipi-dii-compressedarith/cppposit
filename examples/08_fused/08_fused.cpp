

#include <iostream>
#include <vector>
using namespace std;
#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;

int main() {
	
	using ptype = Posit<int8_t, 8, 0, uint32_t, PositSpec::WithInfs>;
    using paccum = Posit<int16_t,16,0,uint32_t, PositSpec::WithNan>;
	//typedef double real; // uncomment this to use IEEE 754 double precision instead

	std::vector<ptype> a{ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f)};      // constucts a posit, using a float literal
	std::vector<ptype> b{ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f)};      // constucts a posit, using a float literal
    paccum res(0);

    auto pres = posit::math::fma_dot(a.begin(),b.begin(),a.size(),res);

    std::cout << pres << std::endl;
	return 0;

}

/*! \example 01_basic.cpp
    \brief A Documented file.
    
    Details.
*/
