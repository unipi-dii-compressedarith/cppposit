#include "posit8.hpp"
#include <iostream>
#include <limits>
#include <cmath>
#include "test_any_posit_pair.hpp"


int main(int argc, char const *argv[])
{
    test_any_posit_pair<posit8,posit8::FPT>();
	return 0;
}