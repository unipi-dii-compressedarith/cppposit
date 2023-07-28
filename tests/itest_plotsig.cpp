// -I../include -I../extern -lboost_iostreams -lboost_system -lboost_filesystem

#include "posit.h"
#include <iostream>
#include "gnuplot-iostream.h"

using namespace posit;

using X = Posit<int16_t, 16 , 2, uint16_t, PositSpec::WithNan>;

template <class T>
T sigmoid(T x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

template <class T>
T tanh(T x)
{
    return 2*sigmoid(2*T(x)) - T(1);
}
/*
	TWICE NOT WORKING
	NEG WORKING
	PSEUDOSIG WORKING
	ONE_MINUS_UR WORKING
*/


void itest_plotsig()
{
	Gnuplot gp;
	std::vector<std::pair<double, double> > xy_pts_A,xy_pts_B;

    for(int x = 0; x < 20000; x+=255)
    {
        auto a = X::from_sraw(x);
        float af = a;
        float sf = 2*af;
        auto p = a.twice();
        float pf = p;
        xy_pts_A.push_back(std::make_pair(af,sf));
        xy_pts_B.push_back(std::make_pair(af,pf));
    }

    // Don't forget to put "\n" at the end of each line!
	gp << "set xrange []\nset yrange []\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines lt rgb 'red' title 'real', '-' with lines title 'pseudo' lt rgb 'green'\n";
	gp.send1d(xy_pts_A);
	gp.send1d(xy_pts_B);
	
#ifdef _WIN32
	// For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
	// the gnuplot window doesn't get closed.
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
#endif

}

int main() {itest_plotsig();}