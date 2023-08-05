

#include <iostream>
#include <vector>
using namespace std;
#include <posit.h>
using namespace posit;
#include <iomanip>


template <class T,int nbits, int esbits, bool log>
struct PositTable;

#include "logptab4_0.h"
#include "ptab4_0.h"



int main() {
    using PPEMU = Posit<int32_t, 4, 0, uint32_t, PositSpec::WithInfs>;
    using PTT = PositTableTrait<int8_t,4,0,false>;
    using LogPTT = PositTableTrait<int8_t,4,0,true>;

    using PTable = PositTable<int8_t,4,0,false>;
    using LogPTable = PositTable<int8_t,4,0,true>;

    using PP = Posit<int8_t, 4, 0, TabulatedBackend<PTT,PPEMU,PTable>, PositSpec::WithInfs>;
    using LogPP = Posit<int8_t, 4, 0, TabulatedBackend<LogPTT,PPEMU,LogPTable>, PositSpec::WithInfs>;

    PP a(0.5f), b(0.25f);
    LogPP a2(0.5f), b2(0.25f);
    std::cout << a+b << " " << a2+b2 << std::endl;
    std::cout << a*b << " " << a2*b2 << std::endl;
    std::cout << a-b << " " << a2-b2 << std::endl;
    std::cout << a/b << " " << a2/b2 << std::endl;
    return 0;
}
