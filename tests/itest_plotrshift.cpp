// -I../include -I../extern -lboost_iostreams -lboost_system -lboost_filesystem

#include "posit.h"
#include "unpacked.h"
#include <iostream>
#include "gnuplot-iostream.h"



using X = Posit<int16_t, 8 , 0, uint16_t, PositSpec::WithNan>;

template <class T>
T sigmoid(T x)
{
    return 1.0 / (1.0 + std::exp(-x));
}


void itest_plotrshift()
{
	Gnuplot gp;
	std::vector<std::pair<double, double> > xy_pts_A,xy_pts_B,xy_pts_C;

    for(int x = -125; x <=125; x++)
    //for(int x = -32000; x <=32000; x+=255)
    {
    	auto a = X::from_sraw(x);
        auto af = (float)a;
        float sf = sigmoid(af);
		auto pa = X::from_sraw((X::PT::POSIT_INVERTBIT+(x >> 1))>>1);
        xy_pts_A.push_back(std::make_pair(af,sf));
        xy_pts_B.push_back(std::make_pair(af,(float)pa));
    }

    // Don't forget to put "\n" at the end of each line!
	gp << "set xrange []\nset yrange []\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines lt rgb 'red' title 'real', '-' with lines title 'pseudo2nooff' lt rgb 'green'\n";
	gp.send1d(xy_pts_A);
	gp.send1d(xy_pts_B);
	
#ifdef _WIN32
	// For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
	// the gnuplot window doesn't get closed.
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
#endif

}