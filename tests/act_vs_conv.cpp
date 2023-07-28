#include "posit.h"
#include "posit_vect.h"
#ifdef __ARM_FEATURE_SVE
#include "posit_sve.h"
#endif
#include <chrono>
#include <cstddef>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock;

using namespace posit;
using X = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;

std::vector<X> genData(int n) {
	std::vector<X> data;
	for(int i = 0; i < n; ++i)
		data.push_back(-X(double(rand())/RAND_MAX));
	return data;
}

template <typename T>
T elu(T x) {
	return (T(std::exp(float(x)))-T(1));
}

void stdELU(std::vector<X>& img) {
	for(int i = 0; i < img.size(); ++i) elu(img[i]);
}

void fastELU(std::vector<X>& img) {
	for(int i = 0; i < img.size(); ++i) {
		std::cout << i;
		img[i].fastELU();
	}
}


int main() {
	// Get the vectorized backend
	vectorized::PositSVEBackend<X> engine;
		std::vector<X> img = genData(225*225);

		// Generate 3-by-3 random filter
		std::vector<X> fil = genData(3*3);

	long int full_el=0,conv_el=0;
	// Apply 3-by-3 conovlution and profile
	for(int i = 0; i < 10; ++i) {

		// Prepare result container 
		std::vector<X> res(223*223);

		auto start = get_time::now();
		engine.sv_conv3x3(img,fil,res,225,225);
		auto post_conv = get_time::now();

		// Apply fastELU
		fastELU(res);

		auto end = get_time::now();
		conv_el+=	std::chrono::duration_cast<ns>(post_conv - start).count();	
		full_el+=	std::chrono::duration_cast<ns>(end - start).count();	
		
	}

	std::cout << conv_el << " " << full_el << "\n";



}