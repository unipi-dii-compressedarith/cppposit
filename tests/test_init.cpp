#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <posit.h>

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

