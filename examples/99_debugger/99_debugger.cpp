//
// Created by Federico Rossi on 29/07/23.
//

#include "posit.h"
#include <cstdint>
int main() {
    uint64_t bigNum = uint64_t(INT32_MAX)*2;
    std::cout << bigNum << " " << INT32_MAX << std::endl;
    posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithNan> x(bigNum);
    std::cout << (uint64_t)x << std::endl;
    return 0;
}
