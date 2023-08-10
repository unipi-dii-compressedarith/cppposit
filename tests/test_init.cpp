#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <posit.h>
#include <iostream>

TEST_CASE("Initialize with deep initalization", "[short]") {
  using P = posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithInfs>;
  P a = P::from_sraw(1 << 14);
  REQUIRE(a.v == (1<<14));
}

TEST_CASE("Initialize with float conversion", "[short]") {
  using P = posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithInfs>;
  P a(1.0f);
  REQUIRE(a.v == (1<<14));
}

TEST_CASE("Initialize with integer conversion", "[short]") {
  using P = posit::Posit<int16_t, 16, 2, uint32_t, posit::PositSpec::WithInfs>;
  P a(1);
  REQUIRE(a.v == (1<<14));
}


TEST_CASE("Initialize with big integers","[short]") {
  uint64_t bigNum = uint64_t(INT32_MAX)*2;
  posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan> x(bigNum);
  REQUIRE(x.v == 2143289344);

  uint32_t bigNum32 = uint32_t(INT16_MAX)*2;
  posit::Posit<int16_t, 16, 2, uint16_t, posit::PositSpec::WithNan> y(bigNum),z(bigNum32);
  REQUIRE(y.v == 32704);
  REQUIRE(z.v == 31744);

  posit::Posit<int8_t, 8, 2, uint16_t, posit::PositSpec::WithNan> w(bigNum);
  REQUIRE(w.v == 127);
}
