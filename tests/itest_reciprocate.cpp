#include <iomanip>
#include <bitset>
#include <fstream>
#include <vector>
#include "posit.h"
#ifdef HAS_GNUPLOT
#include "gnuplot-iostream.h"
#endif

#include <locale>

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};



using namespace posit;
#ifndef POSIT
using X = posit::Posit<int8_t,8,0, uint16_t, posit::PositSpec::WithNan>;
#else
using X = posit::Posit<int16_t,16,0, uint16_t, posit::PositSpec::WithNan>;
#endif

#define bina(x)  std::bitset<sizeof(decltype(x.v))*8>(x.v)
int itest_reciprocate()
{
#ifdef HAS_GNUPLOT
    Gnuplot gp;
#endif
    std::vector<std::pair<double, double> > xy_pts_A,xy_pts_B;
    std::vector<std::vector<double> > all_raw;
    std::vector<std::vector<double> > all_value;

    using PT = typename X::PT;

    const char * name []={"nan","pinf","max","two","one","0.5","min","zero","-min","lowest","ninf"};
    X sp [] = { X::nan(),X::pinf(),X::max(),X::two(),X::one(),X::onehalf(),X::min(),X::zero(),-X::min(),X::lowest(),X::ninf()};
    std::cout << "name\tx\tbin(x)\t1/x\tbin(1/x)\t1.0/float(x)\n";
    for(int i = 0; i < sizeof(name)/sizeof(name[0]);i++)
    {
        auto a = sp[i];
        std::cout << std::setw(8)<<name[i]  << std::setw(16) <<a << " " << std::setw(16) <<bina(a) << " inverted as " << std::setw(16) <<a.reciprocate() << " " << std::setw(16) <<bina(a.reciprocate()) << " real " << std::setw(16) << 1.0/(float)a << std::endl; 
    }

    int64_t count = PT::POSIT_MAXPOS-(int64_t)(PT::POSIT_MINNEG+1); 
    int target = 1024;
    int dt = count < target ? 1 : (count+target-1)/target;
    std::cout << "stepping " << dt << std::endl;

    // PT::POSIT_MINNEG ... POSIT_MAXPOS
    int q = 3;
    int runs [5][3] = { { PT::POSIT_MINNEG, PT::POSIT_MINNEG+q } , { PT::POSIT_MAXPOS-q-1, 2, -1 }, {-1, 1, 1}, { 2 , PT::POSIT_MAXPOS-q-1, 1 },{ PT::POSIT_MAXPOS-q, PT::POSIT_MAXPOS,1 }};

    for(int k = 0; k < 5; k++)
    {
        int n = abs(runs[k][1]-runs[k][0]+1);
        int mn = 1024;
        int dt = n > mn ? n/mn : 1;
        int s = runs[k][2] < 0;
        int np = n/dt;
        if(s)
            dt = -dt;
        std::cout << "run " << runs[k][0] << ".." << runs[k][1] << " n " << np << " dt " << dt << std::endl;
        for(int xx = runs[k][0], i = 0; i < np; i++, xx+= dt)
        {
            auto x = s ? -xx : xx;
            X a = X::from_sraw(x);
            double af = a;
            X ipL1 = a.reciprocate(); // L1
            X ipf  = X::one()/a; // explicit
            double id = 1.0/af; // exact
            X ipr = id; // as posit

            xy_pts_A.push_back(std::make_pair(af,ipf)); // regular
            xy_pts_B.push_back(std::make_pair(af,ipL1)); // L1
            std::vector<double> qraw;
            qraw.push_back(x);
            qraw.push_back(ipf.v);
            qraw.push_back(ipL1.v);
            qraw.push_back(ipr.v);
            qraw.push_back(X::PT::posvalue_has_only_regime(x<0? -x:x));
            //qraw.push_back();
            if(x >= 0 && x > 63)
            std::cout << x << " de " << X::PT::posvalue_has_only_regime(x<0? -x:x) << " " << (X::PT::POSIT_UTYPE)X::PT::_POSIT_TOP-(X::PT::POSIT_UTYPE)(x<0?-x:x) << " " << bina(a) << std::endl;

            std::vector<double> qvalue;
            qvalue.push_back(af);
            qvalue.push_back((double)ipf);
            qvalue.push_back((double)ipL1);
            qvalue.push_back((double)ipr); // after casting to posit
            qvalue.push_back((double)id);  // exact

            all_raw.push_back(qraw);
            all_value.push_back(qvalue);
        }
    }
    std::ofstream onf("itest_reciprocate.tsv");
    onf.imbue(std::locale(std::locale::classic(), new Comma));
    const char *names[] = {"x_raw","y_i_raw","y_i1_raw","y_if_raw","is_fraction_0",  "x_val","y_i_val (d(p(1)/x))","y_i1_val (d(recp(x)))","y_if_val (p(1.0/d(x))","y_ie_val (1.0/d(x))","x_bin"};
    for(int i = 0; i < sizeof(names)/sizeof(names[0]); i++)
    {
        if(i != 0)
            onf << "\t";
        onf << names[i];
    }
    onf << std::endl;
    for(int i = 0; i < all_raw.size(); i++)
    {
        for(int j = 0; j < all_raw[i].size();j++)
        {
            if(j != 0)
                onf << "\t";
            onf << all_raw[i][j];
        }
        for(int j = 0; j < all_value[i].size();j++)
        {
            onf << "\t";
            onf << all_value[i][j];
        }
        onf << "\t" << bina(X::from_sraw(all_raw[i][0]));
        onf << std::endl;
    }

#ifdef HAS_GNUPLOT
    gp << "set xrange []\nset yrange []\n";
    // '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
    //gp << "plot '-' with lines lt rgb 'red' title 'delta'\n"; //, '-' with lines title 'quick' lt rgb 'green'\n";
    gp << "plot '-' with lines lt rgb 'red' title 'real', '-' with lines title 'L1' lt rgb 'green'\n";
    gp.send1d(xy_pts_A);
    gp.send1d(xy_pts_B);
#endif
    return 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    itest_reciprocate();
    return 0;
}