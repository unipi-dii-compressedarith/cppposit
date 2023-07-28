#include "posit.h"
#include <iomanip>
#include <bitset>
#if
#include "gnuplot-iostream.h"
#endif
#ifndef POSIT
using X = Posit<int8_t,8,0, uint16_t, PositSpec::WithNanInf>;
#else
using X = Posit<POSIT>;
#endif

#define bina(x)  std::bitset<sizeof(decltype(x.v))*8>(x.v)
int itest_inver()
{
    Gnuplot gp;
    std::vector<std::pair<double, double> > xy_pts_A,xy_pts_B;

    using PT = typename X::PT;

    // special ones orderd rightwise along the wheel
    const char * name []={"nan","pinf","max","two","one","0.5","min","zero","-min","lowest","ninf"};
    X sp [] = { X::nan(),X::pinf(),X::max(),X::two(),X::one(),X::onehalf(),X::min(),X::zero(),-X::min(),X::lowest(),X::ninf()};
    for(int i = 0; i < sizeof(name)/sizeof(name[0]);i++)
    {
        auto a = sp[i];
        std::cout << std::setw(8)<<name[i]  << std::setw(16) <<a << " " << std::setw(16) <<bina(a) << " inverted as " << std::setw(16) <<a.invert() << " " << std::setw(16) <<bina(a.invert()) << " real " << std::setw(16) << 1.0/(float)a << std::endl; 
    }

    for(int x = PT::POSIT_MINNEG; x <= PT::POSIT_MAXPOS; x++)
    {
        X a(X::DeepInit(),x);
        float af = a;
        float ip = a.invert();
        float ir = 1.0/af;
        xy_pts_A.push_back(std::make_pair(af,ip-ir));
//        xy_pts_B.push_back(std::make_pair(af,ip));
        //std::cout << "x:" << bina(a) << " " << a << " 1/x fast: "  << a.invert() << " 1/x float: " << ir << " " << (ip == ir ? "OK" : "BAD") << std::endl;
    }

    gp << "set xrange []\nset yrange []\n";
    // '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
    gp << "plot '-' with lines lt rgb 'red' title 'delta'\n"; //, '-' with lines title 'quick' lt rgb 'green'\n";
    gp.send1d(xy_pts_A);
    //gp.send1d(xy_pts_B);

    return 0;
}
