#include "posit.h"
//#include "tabulated/posit8.hpp"
//#include "tabulated/posit10.hpp"
//#include "tabulated/posit12.hpp"
using namespace posit;
#include <chrono>
#include <cstddef>
#include <vector>
#include <stdint.h>
#include <inttypes.h>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock;
using X = Posit<int_fast32_t, 14 , 0, uint_fast32_t, PositSpec::WithNan>;

int_fast16_t tanhTab[256*4*4];
X tabTanh(X a) {
	return X::from_sraw(tanhTab[((unsigned int)(uint16_t)a.v) & 0x0FFF]);
}

int_fast16_t eluTab[256*4*4];
X tabElu(X a) {
	return X::from_sraw(eluTab[((unsigned int)(uint16_t)a.v) & 0x0FFF]);
}

template <typename T>
T elu(T x) {
	return (x > T(0)) ? x: (T(std::exp(float(x)))-T(1));
}


template <class DataType>
std::vector<DataType> genData() {
	std::vector<DataType> data(4096);
	for(int i = 0; i < 4096;++i)
		data[i] = DataType::from_sraw(rand());
	return data;
}

long testTabRange(std::vector<X> data) {
	long elapsed = 0;
	for(int i = 0; i < data.size(); ++i) {
		auto start = get_time::now();
		tabTanh(data[i]);
		auto end = get_time::now();
		elapsed+=std::chrono::duration_cast<ns>(end - start).count();
	}	
	return elapsed;
}

long testL1Range(std::vector<X> data) {
	long elapsed = 0;
	for(int i = 0; i < data.size(); ++i) {
		auto start = get_time::now();
		data[i].fastTanh();
		auto end = get_time::now();
		elapsed+=std::chrono::duration_cast<ns>(end - start).count();
	}	
	return elapsed;	
}

int main(int argc,char* argv[]) {

	for(int i = -512*4; i<512*4; ++i) {
		int32_t j = ((unsigned int)(uint16_t)i) & 0x0FFF;
		tanhTab[j] = X(std::tanh(float(X::from_sraw(i)))).v;
	}

	for(int i = -512*4; i<512*4; ++i) {
		int32_t j = ((unsigned int)(uint16_t)i) & 0x0FFF;
		eluTab[j] = elu(X::from_sraw(i)).v;
	}	


	long tab_elapsed_accum = 0;
	for(int i = 0; i < atoi(argv[1]); ++i) tab_elapsed_accum+=testTabRange(genData<X>());

	long L1_elapsed_accum = 0;
	for(int i = 0; i < atoi(argv[1]); ++i) L1_elapsed_accum+=testL1Range(genData<X>());

	std::cout << 100*(1-(double)tab_elapsed_accum/double(L1_elapsed_accum)) << std::endl;
}
