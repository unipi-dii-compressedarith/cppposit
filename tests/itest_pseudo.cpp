#include "posit.h"
#include <iomanip>
#include <bitset>
#include <cmath>
using namespace posit;
using X = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;

template <class T>
T sigmoid(T x)
{
    return 1 / (1 + std::exp(-x));
}

template <class T>
T tanh(T x)
{
    return 2*sigmoid(2*T(x)) - T(1);
}

#define bina(x)  std::bitset<sizeof(decltype(x.v))*8>(x.v)
int itest_pseudo()
{

    #if 0
    for(int x = -128; x < 128; x++)
    {
        X a(X::DeepInit(),x);
        X a1(X::DeepInit(),x>>1);
        auto au = a.unpack_low();
        auto au1 = a1.unpack_low();
        std::cout 
            //<< bina(a) << " " << (float)a << " " << (int)au.regime << " -> " 
            //<< bina(a1) << " " << (float)a1 << " " << (int)au1.regime 
          // << (int)au.regime << " -> " << (int)au1.regime
           << (float)a << " -> " << (float)a1
            << std::endl;
    }

    return -1;

    for(int x = -128; x < 128; x++)
        X a(X::DeepInit(),x);
        X ax(X::DeepInit(),x>>2);
        X af(X::DeepInit(),x ^ X::PT::POSIT_SIGNMASK);
        X bf(X::DeepInit(),(x ^ X::PT::POSIT_SIGNMAS    K)>>2);
        std::cout << bina(a) << " " << a << " " << bina(ax) << " " << ax << " " << bina(af) << " " << af << " " <<  bina(bf) << " " << bf << std::endl;
        // a.invert() << " vs " << (float)a << " " << 1.0/(float)a << std::endl;
    }
    return 0;
    #endif

    // span all space
    std::cout << "x\ttanh(x)\tfastTanh(x)\tdelta\n";
    for(int x = -128; x < 128; x++)
    {
        X a(X::DeepInit(),x);
        float af = a;
        float sf = tanh(af);
        auto p = a.fastTanh();
        float pf = p;
        std::cout << af << "\t" << p << "\t" << sf << "\t" << (sf-pf) << std::endl;
    }
    return 0;
}

int main() {
    itest_pseudo();
}