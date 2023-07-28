#include "posit.h"
#include <iomanip>
#include <bitset>
#include <cmath>

#ifndef POSIT
using X = Posit<int8_t,8, 0, uint16_t, PositSpec::WithNanInf>;
#else
using X = Posit<POSIT>;
#endif

#define bina(x)  std::bitset<sizeof(decltype(x))*8>(x)
//#define OP(y) ((y)>>1)
#define OP(y) y
//#define OP(y) ((y) ^ PT::POSIT_SIGNMASK)
#define EOP(y) y
#define testspecial(x) std::cout << std::setw(16) <<#x << ": " << std::setw(16) << (float)EOP(X::from_sraw(OP(PT::x))) << " " << std::setw(16) << bina(OP(PT::x))  << " " << std::setw(15) << (int64_t)EOP(X::from_sraw(OP(PT::x))).v << std::endl;
#define testreg(x) std::cout << std::setw(16) <<#x << ": " << std::setw(16) << (float)EOP(X::from_sraw(OP(x))) << " " << std::setw(16) << bina(OP(x))  << " " << std::setw(15) << (int64_t)EOP(X::from_sraw(OP(x))).v << std::endl;
#define testnormal(x) std::cout <<  std::setw(16) << " "<< std::setw(16) << x << " "  << bina(X(x).v) << std::endl;
int itest_printbits()
{
    using PT = typename X::PT;
    testspecial(_POSIT_TOPLEFT)
    testspecial(_POSIT_TOP)
    testspecial(_POSIT_TOPRIGHT)
    testspecial(POSIT_PINF)
    testspecial(POSIT_NAN)
    testspecial(POSIT_NINF)
    testspecial(POSIT_ONE)
    testspecial(POSIT_MONE)
    testspecial(POSIT_TWO)
    testnormal(2.0)        
    testreg(-PT::POSIT_TWO)
    testnormal(-2.0)        
    testspecial(POSIT_HALF)
    testnormal(0.5)        
    testreg(-PT::POSIT_HALF)
    testnormal(-0.5)        
    testspecial(POSIT_MINNEG)
    testspecial(POSIT_MAXPOS)
    testspecial(POSIT_AFTER0)
    testspecial(POSIT_BEFORE0)
    return 0;
}
