

#include <iostream>
#include <vector>
using namespace std;
#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;

int main() {
    Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs> One(1);
    Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs> Tiny(0.00000000000000003);
    auto ot = One - Tiny;
    auto eps = One - One.prev();
    printf("One val:\t %.23f\n", (double)One);
    printf("Tiny val:\t %.23f\n", (double)Tiny);
    printf("Epsilon:\t %.23f\n", (double)eps);

    printf("One - tiny:\t %.23f\n", (double)(ot));
    printf("One - eps:\t %.23f\n", (double)(One - eps));

    printf("One previous:\t %.23f\n", (double)One.prev());


}
/*! \example 01_basic.cpp
    \brief A Documented file.
    
    Details.
*/
