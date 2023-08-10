#include "posit.h"

namespace posit {
template class Posit<int32_t, 32, 2, uint32_t, PositSpec::WithNan>;
}

float convert32(const float x) {
  posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan> y(x);
  const int32_t z =  *(int32_t*)&y;
  std::cout << z << std::endl;
  return *(posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan>*)&z;  
}

float convert16(const float x) {
  posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithNan> y(x);
  const int16_t z =  *(int16_t*)&y;
  std::cout << z << std::endl;
  return *(posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithNan>*)&z;  
}

typedef float (*F)(const float);

template <float (*F)(float)>
void calc(float x, int bits) {
  std::cout << "input = " << x << ", converted back and forth with " << bits
            << " bits = " << F(x)
            << ", ratio = " << std::abs(F(x)/double(x)) << std::endl;
}

int main() {
  std::cout << "Using cppposit library" << std::endl << std::endl;

  calc<&convert32>(1.75911e-33, 32);
  calc<&convert32>(-1.15356e-33f, 32);
  calc<&convert16>(6.40675e-27, 16);
  calc<&convert16>(-8.76183e-37, 16);
  calc<&convert16>(1.4013e-45, 16);  
}
