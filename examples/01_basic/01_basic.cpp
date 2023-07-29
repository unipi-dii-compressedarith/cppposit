

#include <iostream>
#include <vector>
using namespace std;
#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;

int main() {
    Posit<int32_t, 32, 2, uint64_t, PositSpec::WithInfs> One(1);
    Posit<int32_t, 32, 2, uint64_t, PositSpec::WithInfs> Tiny(0.000000000000000000000003);
    auto Min = Posit<int32_t, 32, 2, uint64_t, PositSpec::WithInfs>::from_sraw(1);
    auto ot = One - Tiny;
    auto eps = One - One.prev();
    printf("One val:\t %.23f\n", (double)One);
    printf("Tiny val:\t %.23f\n", (double)Tiny);
    printf("Epsilon:\t %.23f\n", (double)eps);
    printf("Min:\t %.64f\n", (double)Min);

    printf("One - tiny:\t %.23f\n", (double)(ot));
    printf("One - eps:\t %.23f\n", (double)(One - eps));

    printf("One previous:\t %.23f\n", (double)One.prev());

    std::cout << eps.v << " " << Tiny.v << std::endl;
}
/*! \example 01_basic.cpp
    \brief A Documented file.
    
    Details.
*/
