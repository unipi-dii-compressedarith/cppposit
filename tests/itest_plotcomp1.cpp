// -I../include -I../extern -lboost_iostreams -lboost_system -lboost_filesystem

#include "posit.h"
#include "unpacked.h"
#include <iostream>
#include "gnuplot-iostream.h"
#include <bitset>
#include <cmath>



using X = Posit<int8_t, 8 , 1, uint16_t, PositSpec::WithNan>;
#define bina(x)  std::bitset<sizeof(decltype(x))*8>(x)


void itest_plotcomp1()
{
	Gnuplot gp;
	std::vector<std::pair<double, double> > xy_pts_A,xy_pts_B;

    for(auto x = X::mone().v; x <= X::one().v; x++)
    {
        auto af = (float)X::from_sraw(x);
        auto pf = (float)X::from_sraw(x).one_minus_ur();
        auto rf = 1.0-af;
        xy_pts_A.push_back(std::make_pair(af,pf));
        xy_pts_B.push_back(std::make_pair(af,rf));
        if(x > 0)
        {
        	std::cout << bina(x)  << " " << af << " " << bina(X(rf).v) << std::endl;
        }
    }

    // Don't forget to put "\n" at the end of each line!
	gp << "set xrange []\nset yrange []\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines lt rgb 'red' title 'posit', '-' with lines title 'real' lt rgb 'green'\n";
	gp.send1d(xy_pts_A);
	gp.send1d(xy_pts_B);
	
#ifdef _WIN32
	// For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
	// the gnuplot window doesn't get closed.
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
#endif

}