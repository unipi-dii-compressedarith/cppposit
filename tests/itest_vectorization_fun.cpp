/*
aarch64-linux-g++ -march=armv8-a+sve  -Ofast -I../include
*/
#include "posit.h"
#include "posit_vect.h"
#ifdef __ARM_FEATURE_SVE
#include "posit_sve.h"
#endif
#include <chrono>
#include <cstddef>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;

using namespace posit;
using X = Posit<int8_t, 8 , 0, uint8_t, PositSpec::WithNan>;

std::vector<X> genData(int n) {
	std::vector<X> data;
	for(int i = 0; i < n; ++i)
		data.push_back(-X(double(rand())/RAND_MAX));
	return data;
}

float plain_dot(std::vector<X>& data1,std::vector<X>& data2) {
	float acc = 0;
	for(int i = 0; i < data1.size(); ++i) 
		acc+=float(data1[i]*data2[i]);
	return acc;
}

void plainMM(std::vector<X>& A,std::vector<X>& B,std::vector<X>& C,int M,int N,int K) {
	for (int i = 0; i < M; i ++)
		for (int j = 0; j < N; j ++) {
			C[i * N + j] = 0;
			for (int k = 0; k < K; k ++)
				C[i * N + j] += A[i * K + k] * B[k * N + j];
			}
}

void plainConv3x3(std::vector<X>& A,std::vector<X>& F,std::vector<X>& O,int R,int C) {
	// Loop on rows
	for(int i = 0; i < R-2;++i) {

		// Loop on columns
		for(int j = 0; j < C-2; ++j) {

			X accum(0);

			// Loop on kernel rows
			for(int k = 0; k < 3; ++k) {
				// Loop on kernel columns
				for(int l = 0; l < 3; ++l) {
					accum = accum + A[(i+k)*C + j + l ]*F[k*3+l];
				}
			}

			O[i*(C-2)+j] = accum;
		}

	}
}


void plainMaxPool3x3(std::vector<X>& A,std::vector<X>& O,int R,int C) {
	// Loop on rows
	for(int i = 0; i < R-2;++i) {

		// Loop on columns
		for(int j = 0; j < C-2; ++j) {

			X max(0);

			// Loop on kernel rows
			for(int k = 0; k < 3; ++k) {
				// Loop on kernel columns
				for(int l = 0; l < 3; ++l) {
					if(A[(i+k)*C + j + l ] > max) max = A[(i+k)*C + j + l ];
				}
			}

			O[i*(C-2)+j] = max;
		}

	}	
}

#ifdef __ARM_FEATURE_SVE
void test_correctness(std::vector<X>& data) {
	std::vector<X> sv;
	std::vector<X> sv1;
	vectorized::PositSVEBackend<X> engine;	
	engine.vFastELU(data,sv);	
	for(X tmp:data) sv1.push_back(tmp.fastELU());	
	double sqrsum = 0;
	for(int i = 0; i < sv.size(); ++i) {
		sqrsum = double(sv[i]-sv1[i])*double(sv[i]-sv1[i]);
	}
	std::cout << "Err: " << sqrsum/sv.size() << "\n";
}

void test_correctness(std::vector<X>& data1,std::vector<X>& data2) {
	std::vector<X> sv;
	std::vector<X> sv1;
	vectorized::PositSVEBackend<X> engine;	
	float a1 = engine.vDot(data1,data2);	
	float a2 = plain_dot(data1,data2);
	std::cout << "Err: " << (a1-a2)*(a1-a2)	 << "\n";
}
#endif

void test_time(std::vector<X>& data,int runs) {
	std::vector<X> sv;
	std::vector<X> sv1;
	auto start = get_time::now();		
	#ifdef __ARM_FEATURE_SVE
	std::cout << "Executing with Posit SVE Backend (bits: " << 32*svcntw() <<")\n" ;
	vectorized::PositSVEBackend<X> engine;
	for(int i = 0; i < runs; ++i) engine.vFastELU(data,sv);
	#else
	std::cout << "Executing with naive Posit backend\n" ;
	#pragma clang loop vectorize(disable)
	for(int i = 0; i < runs; ++i) for(X tmp:data) sv1.push_back(tmp.fastELU());
	#endif
	auto end = get_time::now();			
	auto elapsed = 	std::chrono::duration_cast<ms>(end - start).count();	
	std::cout << "External (ms): " << elapsed << std::endl;
	std::cout << "Mean per computation (ms): " << double(elapsed)/double(runs) << std::endl;
}

void test_time(std::vector<X>& data1,std::vector<X>& data2,int runs) {
	std::vector<X> sv;
	std::vector<X> sv1;
	auto start = get_time::now();		
	#ifdef __ARM_FEATURE_SVE
	std::cout << "Executing with Posit SVE Backend (bits: " << 32*svcntw() <<")\n" ;
	vectorized::PositSVEBackend<X> engine;
	for(int i = 0; i < runs; ++i) engine.vDot(data1,data2);
	#else
	std::cout << "Executing with naive Posit backend\n" ;

	float acc = 0;
	for(int i = 0; i < runs; ++i) plain_dot(data1,data2);
	#endif
	auto end = get_time::now();			
	auto elapsed = 	std::chrono::duration_cast<ms>(end - start).count();	
	std::cout << "External (ms): " << elapsed << std::endl;
	std::cout << "Mean per computation (ms): " << double(elapsed)/double(runs) << std::endl;
}

void test_time(std::vector<X>& mat1,std::vector<X>& mat2,int M,int N,int K,int runs) {
	std::vector<X> res((M/3)*(N/3));
	std::vector<X> res1((M/3)*(N/3));
	auto start = get_time::now();		
	#ifdef __ARM_FEATURE_SVE
	std::cout << "Executing with Posit SVE Backend (bits: " << 32*svcntw() <<")\n" ;
	vectorized::PositSVEBackend<X> engine;
	for(int i = 0; i < runs; ++i) engine.vMaxPool3f(mat1,res,M,N);
	#else
	std::cout << "Executing with naive Posit backend\n" ;

	float acc = 0;
	for(int i = 0; i < runs; ++i) plainMaxPool3x3(mat1,res1,M,N);
	#endif
	auto end = get_time::now();			
	auto elapsed = 	std::chrono::duration_cast<ms>(end - start).count();	
	std::cout << "External (ms): " << elapsed << std::endl;
	std::cout << "Mean per computation (ms): " << double(elapsed)/double(runs) << std::endl;
}



int main(int argc,char* argv[]) {

	if(argc > 4) {
		int M = atoi(argv[1]),N=atoi(argv[2]),K=atoi(argv[3]);
		std::vector<X> data = genData(M*N);
		std::vector<X> data1 = genData(3*3);		
		int runs = atoi(argv[4]);
		test_time(data,data1,M,N,K,runs);
		std::cout << "Done..." << std::endl;
		return 0;
	}
	std::vector<X> data = genData(atoi(argv[1]));
	if(argc > 2) {
		int runs = atoi(argv[2]);
		test_time(data,runs);
	}

	#ifdef __ARM_FEATURE_SVE
	//test_correctness(data);
	#endif	
}
