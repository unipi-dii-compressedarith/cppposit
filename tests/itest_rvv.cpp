#include "posit.h"
#include "posit_vect.h"
#ifdef _EPI_
#include "posit_rvv.h"
#endif
#include <chrono>
#include <cstddef>
using namespace posit;
using X = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;

std::vector<X> genData(int n) {
        std::vector<X> data;
        for(int i = 0; i < n; ++i)
                data.push_back(X::from_sraw(i));
        return data;
}

int main(int argc,char** argv) {
	std::cout << "Can contain: " << rvcntw() << " floats\n";
	int len = atoi(argv[1]);
	vectorized::PositRVVBackend<X> fv;
        std::vector<X> a {X(1),X(2),X(3),X(1),X(2),X(3),X(1),X(2),X(3)};
        std::vector<X> b {X(1),X(2),X(3),X(1),X(2),X(3),X(1),X(2),X(3)};
	std::vector<X> c(9);
	fv.vGemm(a,b,c,3,3,3);
	std::cout << "===============================================================\n";
	for(int i = 0; i < 9; ++i) {
	   if(i%3 == 0) std::cout << std::endl;	
	   std::cout << c[i] << " ";
	

	}
	std::cout << std::endl;
}
