#include "posit.h"
#include <iostream>
#include "gnuplot-iostream.h"


// -I../include -I../extern -lboost_iostreams -lboost_system -lboost_filesystem

#include "posit.h"
#include <iostream>
#include "gnuplot-iostream.h"

using namespace posit;

using X32 = Posit<int32_t, 32 , 0, uint32_t, PositSpec::WithNan>;
using X16 = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;
using X8 = Posit<int8_t, 8 , 0, uint8_t, PositSpec::WithNan>;

template <typename T>
T elu(T x) {
	return (x > T(0)) ? x: (T(std::exp(float(x)))-T(1));
}

template <typename T>
void plotELU()
{
	Gnuplot gp;
	std::vector<std::pair<double, double> > xy_pts_A,xy_pts_B;
    float min=-5,max=5,step=0.1,mse=0;
    float count = (max-min)/step;
    for(float x = -5; x < 5; x+=0.1)
    {
        T a(x);
        auto fast = elu(a);
        auto real = a.fastELU();
        float pf = fast;
        float pr = real;
        std::cout << "(" << x << "," << pf << "," << pr << ")" << std::endl;
        mse += (pf-pr)*(pf-pr)/(count);
        xy_pts_A.push_back(std::make_pair(x,pf));
        xy_pts_B.push_back(std::make_pair(x,pr));
    }
    std::cout << mse << std::endl;
    // Don't forget to put "\n" at the end of each line!
	gp << "set xrange []\nset yrange []\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines lt rgb 'red' title 'real', '-' with lines title 'fast' lt rgb 'green'\n";
	gp.send1d(xy_pts_A);
	gp.send1d(xy_pts_B);
	

}


int main() {
	plotELU<X8>();
}