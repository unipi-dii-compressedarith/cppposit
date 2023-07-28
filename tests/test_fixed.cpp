#include <posit.h>
#include <iostream>
//using namespace posit;
using FX16 = posit::BackendFixed<fixedtrait<int16_t,16>,int16_t >;
using FX32 = posit::BackendFixed<fixedtrait<int32_t,32>,int32_t >;
using P8fx =    posit::Posit<int8_t,   8 , 0, FX16, posit::PositSpec::WithNan>;
using P16_0fx = posit::Posit<int16_t, 16 , 1, FX32, posit::PositSpec::WithNan>;


int main() {
	posit::Unpacked<uint32_t,int16_t> up(0);
    P16_0fx b(0.5f);    
    FX32 b_raw = P16_0fx::unpacked_low2full(b.unpack_low());
    P8fx a(0.5f);
    FX16 a_raw = P8fx::unpacked_low2full(a.unpack_low());

    std::cout << b << " " << b_raw << std::endl;
    std::cout << a << " " << a_raw << std::endl;
}
