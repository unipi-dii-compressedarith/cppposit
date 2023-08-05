#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <posit.h>

using P = posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan>;
using P16 = posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithNan>;

double convert32(const double x) {
  P y(x);
  const uint32_t z =  *(uint32_t*)&y;
  return *(P*)&z;  
}

float convert16(const float x) {
  P16 y(x);
  const uint16_t z =  *(uint16_t*)&y;
  return *(P16*)&z;  
}


TEST_CASE("Test 32,2 minposit", "[short]") {
  float a = convert32(1.75911e-33f);
  float b = convert32(-1.15356e-33f);
  REQUIRE(P(a).v == 7);
  REQUIRE(P(b).v == -6);
}

TEST_CASE("Test 16,2 minposit", "[short]") {
  float a = convert32(6.40675e-27f);
  float b = convert32(-8.76183e-37f);
  REQUIRE(P16(a).v == 1);
  REQUIRE(P16(b).v == -1);
}
