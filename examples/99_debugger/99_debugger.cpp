//
// Created by Federico Rossi on 29/07/23.
//

#include "posit.h"

float convert32(const float x) {
    posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan> y(x);
    const uint32_t z =  *(uint32_t*)&y;
    return *(posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan>*)&z;
}

float convert16(const float x) {
    posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithNan> y(x);
    const uint16_t z =  *(uint16_t*)&y;
    return *(posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithNan>*)&z;
}

typedef float (*F)(const float);

template <float (*F)(float)>
void calc(float x, int bits) {
    std::cout << x << "\t" << F(x) << std::endl;
}

int main() {
    std::cout << "Using cppposit library" << std::endl << std::endl;

    calc<&convert32>(1.75911e-33, 32);
    calc<&convert32>(-1.15356e-33f, 32);
    calc<&convert16>(6.40675e-27, 16);
    calc<&convert16>(-8.76183e-37, 16);
    calc<&convert16>(1.4013e-45, 16);
}
