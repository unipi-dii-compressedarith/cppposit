

#include <iostream>
#include <vector>
using namespace std;
#include <posit.h>
using namespace posit;
#include <iomanip>


template <class T,int nbits, int esbits>
struct PositTable;

#include "ptab4_0.h"



int main() {
    using PPEMU = Posit<int32_t, 4, 0, uint32_t, PositSpec::WithInfs>;
    using PP = Posit<int8_t, 4, 0, TabulatedBackend<int8_t,PPEMU,PositTable<int8_t,4,0>>, PositSpec::WithInfs>;
    PP a(0.5f), b(0.25f);
    std::cout << a+b << std::endl;
    std::cout << a*b << std::endl;
    std::cout << a-b << std::endl;
    std::cout << a/b << std::endl;
    return 0;
}
