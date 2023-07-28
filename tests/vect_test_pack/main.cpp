#include "posit.h"
#include "posit_vect.h"
#ifdef __ARM_FEATURE_SVE
#include "posit_sve.h"
#endif
#ifdef _EPI_
#include "posit_rvv8.h"
//#include "vehave-control.h"
#else
#define vehave_trace(A,B) ((void)0)
#endif
#include <chrono>
#include <cstddef>
#include <random>
#include <cstddef>
#include <string>
#include <fstream>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;

using namespace posit;
using X = Posit<int8_t, 8 , 0, uint8_t, PositSpec::WithNan>;
//using X = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;

std::mt19937 getRNG() {
    std::random_device dev;
    std::mt19937 rng(dev());
    return rng;
}

template <class T>
std::vector<T> genVector(int vectorSize) {
	std::vector<T> out(vectorSize);
	std::uniform_int_distribution<std::mt19937::result_type> dist(-32768,32768);
	std::mt19937 rng = getRNG();
	for(int i = 0; i < vectorSize; ++i) {
		out[i] = T::from_sraw(dist(rng));
	}
	return out;
}

void dumpTimes(std::vector<unsigned long int>& data,std::string name) {
	std::ofstream outf;
	outf.open(name);
	for(unsigned long int d:data) outf << d << "\n";
	outf.close();
}

template <class T>
std::vector<T> genMatrix(int rows,int cols) {
	return genVector<T>(rows*cols);
}

/* BENCHMARK SECTION */

#include "vconv.h"
#include "vdot.h"
#include "velu.h"
#include "vgemm.h"
#include "vsig.h"
#include "vtanh.h"




/*=======================*/






int main(int argc,char** argv) {
	vectorized::PositVectorizedFrontend<X>* fv = nullptr;
	std::string suffix("naive");
	#ifdef __ARM_FEATURE_SVE
	fv = new vectorized::PositSVEBackend<X>();
	suffix = std::string("sve");
	#endif
	#ifdef _EPI_
	fv = new vectorized::PositRVVBackend<X>();
	vectorized::PositRVVBackend<X> pp_rvv;
	suffix = std::string("rvv");
	#endif
    if(argc == 2) {
        #ifdef _TEST_SIG
        testSig(fv,atoi(argv[1]),8192);
        #endif
        #ifdef _TEST_TAN
        testTanh(fv,atoi(argv[1]),8192);
        #endif
        #ifdef _TEST_ELU
        testELU(fv,atoi(argv[1]),8192);    
        #endif
    }
    else if(argc == 3)
        testDot<X>(fv,atoi(argv[1]),atoi(argv[2]));
        //testDotX<X>(pp_rvv,atoi(argv[1]),atoi(argv[2]));
    else if(argc == 4) {}
        //testConv<X>(fv,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
		//testConv3X<X>(pp_rvv,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
    else if(argc == 5) {
        //testGemm4<X>(pp_rvv,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
        testGemm<X>(fv,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
	}
	else
		std::cout << "Invalid argument number\n";
}
