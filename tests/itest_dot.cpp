#include "posit.h"
#include <iomanip>
#include <bitset>
#include <cmath>

#ifndef POSIT
using X = Posit<int8_t,8,0, uint16_t, PositSpec::WithNanInf>;
#else
using X = Posit<POSIT>;
#endif
using Y = Posit<int16_t,12,0, uint16_t, PositSpec::WithNanInf>;


template <class T>
std::pair<T,T> maxdot(T* a, T* b, int N)
{
	T r = a[0]*b[0];
	T m = std::fabs(r);
	T M = m;
	for(int i = 0; i < N; i++)
	{		
		T q = a[i]*b[i];
		r += q;
		if(std::fabs(q) > M)
			M = std::fabs(q);
		if(std::fabs(r) > m)
			m = std::fabs(r);
	}
	return {m,M};
}

#define bina(x)  std::bitset<sizeof(decltype(x.v))*8>(x.v)
int itest_dot()
{
	float v1[] = { 1, 12, 3.5, -14,1e-3,  6.3};
	float v2[] = { 4, -2, 2,   1,  1e-5,  4};
	constexpr int N = sizeof(v2)/sizeof(v2[0]);
	X v1p[N];
	X v2p[N];
	float v1pf[N],v2pf[N];

	for(int i = 0; i < N; i++)
	{
		v1pf[i] = v1p[i] = v1[i];
		v2pf[i] = v2p[i] = v2[i];
	}
	// then remake float


	auto w = maxdot(v1,v2,N);
	std::cout << "dot orig maxsingle: " << w.second << " maxint:" << w.first << std::endl;
	 w = maxdot(v1pf,v2pf,N);
	std::cout << "dot conv maxsingle: " << w.second << " maxint:" << w.first << std::endl;
	std::cout << "dot orig float: " << math::dot(v1,v2,N) << std::endl;
	std::cout << "dot conv float: " << math::dot(v1pf,v2pf,N) << std::endl;
	std::cout << "dot_self: " << X::dot(v1p,v2p,N) << std::endl;
	//std::cout << "dot_float: " << X::dot(v1,v2,N,1.0f) << std::endl;
	//std::cout << "dot_double: " << X::dot(v1,v2,N,1.0) << std::endl;
	//std::cout << "dot_fixed: " << X::dot(v1,v2,N,lightfixed<X::UnpackedT::fraction_type,X::UnpackedT::exponent_type,fixedtrait<int32_t,10,3>>()) << std::endl;
	//std::cout << "dot_other: " << X::dot(v1,v2,N,Y()) << std::endl;
	//std::cout << "dot_unpacked" << X::dot(v1,v2,N,X::UnpackedT()) << std::endl;

	std::cout << "dotK_float: " << math::comp_dot(v1pf,v2pf,N,1.0f,1.0f,1.0f) << std::endl;
	std::cout << "dotK_posit: " << math::comp_dot(v1p,v2p,N,X(),X(),X()) << std::endl;
	std::cout << "dotK_ppff: " << math::comp_dot(v1p,v2p,N,X(),1.0f,1.0f) << std::endl;
	//std::cout << "dotK_double: " << X::dotKahan(v1,v2,N,1.0,1.0) << std::endl;
	//std::cout << "dotK_fixed: " << X::dotKahan(v1,v2,N,lightfixed<X::UnpackedT::fraction_type,X::UnpackedT::exponent_type,fixedtrait<int32_t,10,3>>()) << std::endl;
	//std::cout << "dotK_other: " << X::dotKahan(v1,v2,N,Y()) << std::endl;
	//std::cout << "dotK_unpacked" << X::dotKahan(v1,v2,N,X::UnpackedT()) << std::endl;

	return 0;
}