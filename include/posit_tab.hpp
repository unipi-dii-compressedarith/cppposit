/**
 * Copyright (C) 2017-2019 Emanuele Ruffaldi
 * Distributed under the terms of the BSD 3-Clause License.  
 * 
 * (See accompanying file LICENSE)
 * 
 * --
 */
#pragma once
#include <stdint.h>
#include <iostream>

#ifndef CONSTEXPR14
#define CONSTEXPR14
#endif

#include "posit.h"
using namespace posit;
/**
 * @brief Generalized Tabulated Posit, requires to declare and fill the static tables
 */
template <class T, int totalbits, int esbits, class FT, PositSpec positspec>
class posit_tab {
	static T opinv[];
	static T ophalf[];
	static T opexp2[];
	static T optwice[];
	static T opsquare[];
	static T opmul[];
	static T opadd[];
	static T opdiv[];
	static float op2float[];

public:
	// template <class T,int totalbits, int esbits, class FT, bool withnan>
	using value_t=T;
	using fraction_t=FT;
	static constexpr int vtotalbits = totalbits;
	static constexpr int vesbits = esbits;
	using FPT=Posit<T,totalbits,esbits,FT,positspec>;
	using PT=typename FPT::PT;
	using exponenttype=typename PT::exponenttype;
	using PTU=typename PT::POSIT_UTYPE;
	using UnpackedLow=typename FPT::UnpackedLow;
	using UnpackedT=typename FPT::UnpackedT;
	using info=typename FPT::info;

	struct DeepInit{};

	explicit posit_tab(double a);
	explicit posit_tab(float a);
	explicit posit_tab(int a);
	explicit posit_tab(DeepInit , int8_t a) : v(a) {}
	explicit posit_tab() {}
	explicit posit_tab(FPT x) : v(x.v) {}
	explicit posit_tab(UnpackedLow u) : v(FPT(u).v) {}
	explicit posit_tab(UnpackedT u) : v(FPT(u).v) {}

	bool is_negative() const { return v < 0; }
	bool has_neg_exponent() const { return (v & PT::POSIT_INVERTBIT) == 0; }
	bool is_nan() const { return PT::withnan && v == PT::POSIT_NAN; }
	bool is_infinity() const { return v==PT::POSIT_NINF || v==PT::POSIT_PINF; }
	static posit_tab max() { return posit_tab(DeepInit(),PT::POSIT_MAX); }
	static posit_tab min() { return posit_tab(DeepInit(),PT::POSIT_MIN); }  // <------
	static posit_tab one() { return posit_tab(DeepInit(),PT::POSIT_ONE); }

	//static posit_tab two() { return posit_tab(DeepInit(),72); }  // <------
	//static posit_tab onehalf() { return posit_tab(DeepInit(),0x20); }  // <------

	static posit_tab pinfinity() { return posit_tab(DeepInit(),PT::POSIT_PINF); 
	static posit_tab ninfinity() { return posit_tab(DeepInit(),PT::POSIT_NINF); 
	static posit_tab nan() { return posit_tab(DeepInit(),PT::POSIT_NAN); }

	posit_tab inv() const { return posit_tab(DeepInit(),opinv[uu()]); }
	posit_tab half() const { return posit_tab(DeepInit(),ophalf[uu()]);}
	posit_tab twice() const { return posit_tab(DeepInit(),optwice[uu()]); }
	posit_tab exp2() const { return posit_tab(DeepInit(),opexp2[uu()]); }
	posit_tab square() const { return posit_tab(DeepInit(),opsquare[uu()]); }
	posit_tab operator-() const { return posit_tab(DeepInit(),-v);  }

	friend bool operator<(posit_tab a, posit_tab b) { return a.v < b.v; }
	friend bool operator>(posit_tab a, posit_tab b) { return a.v > b.v; }
	friend bool operator<=(posit_tab a, posit_tab b) { return a.v <= b.v;}
	friend bool operator>=(posit_tab a, posit_tab b) { return a.v >= b.v; }
	friend bool operator==(posit_tab a, posit_tab b) { return a.v == b.v; }
	friend bool operator!=(posit_tab a, posit_tab b) { return a.v !=b.v; }

	friend posit_tab operator+(posit_tab a, posit_tab b) { return posit_tab(DeepInit(),opadd[a.uu()*256+b.uu()]); }  // <------
	friend posit_tab operator*(posit_tab a, posit_tab b) { return posit_tab(DeepInit(),opmul[a.uu()*256+b.uu()]); }  // <------
	friend posit_tab operator-(posit_tab a, posit_tab b) { return a+(-b); }
	friend posit_tab operator/(posit_tab a, posit_tab b) { return posit_tab(DeepInit(),opdiv[a.uu()*256+b.uu()]); }  // <------
	
	posit_tab& operator+=(const posit_tab &a) { posit_tab r = *this+a; v = r.v; return *this; }
	posit_tab& operator*=(const posit_tab &a) { posit_tab r = *this*a; v = r.v; return *this; }

	PTU uu() const { return (PTU)v;}

	FPT as_posit() const { return FPT(typename FPT::DeepInit(), v); }
	operator FPT() const { return as_posit(); }
	operator float() const { return uint32_to_float(op2float[uu()]); }
	operator double() const { return (float)*this; }
	operator int() const { return (float)*this; }

	UnpackedLow unpack_low() const { return as_posit().unpack_low(); }
	UnpackedT unpack() const { return as_posit().unpack(); }
	info analyze() const { return as_posit().analyze(); }





	T v;
};

template <class T, int totalbits, int esbits, class FT, bool withnan>
inline posit_tab<T,totalbits,esbits,FT,withnan> half(posit_tab<T,totalbits,esbits,FT,withnan> z) { return z.half(); }

template <class T, int totalbits, int esbits, class FT, bool withnan>
inline posit_tab<T,totalbits,esbits,FT,withnan> twice(posit_tab<T,totalbits,esbits,FT,withnan> z) { return z.twice(); }

template <class T, int totalbits, int esbits, class FT, bool withnan>
inline posit_tab<T,totalbits,esbits,FT,withnan> square(posit_tab<T,totalbits,esbits,FT,withnan> z) { return z.square(); }

template <class T, int totalbits, int esbits, class FT, bool withnan>
inline posit_tab<T,totalbits,esbits,FT,withnan> inv(posit_tab<T,totalbits,esbits,FT,withnan> x) { return x.inv(); }

template <class T, int totalbits, int esbits, class FT, bool withnan>
CONSTEXPR14 inline posit_tab<T,totalbits,esbits,FT,withnan> neg(posit_tab<T,totalbits,esbits,FT,withnan> z) { return -z; }

template <class T, int totalbits, int esbits, class FT, bool withnan>
inline posit_tab<T,totalbits,esbits,FT,withnan> exp2(posit_tab<T,totalbits,esbits,FT,withnan> z) { return z.exp2(); }

template <class T, int totalbits, int esbits, class FT, bool withnan>
inline bool is_negative(posit_tab<T,totalbits,esbits,FT,withnan> a)
{
	return a.v < 0;
}

template <class T, int totalbits, int esbits, class FT, bool withnan>
posit_tab<T,totalbits,esbits,FT,withnan>::posit_tab(int a)
{
	if(a == 0)
		v = 0;
	else if(a == 1)
		v = PT::POSIT_ONE;
	else 
		v = FPT(a).v;

}

template <class T, int totalbits, int esbits, class FT, bool withnan>
posit_tab<T,totalbits,esbits,FT,withnan>::posit_tab(float a)
{
		if(a == 0)
		v = 0;
	else if(a == 1)
		v = PT::POSIT_ONE;
	else 
		v = FPT(a).v;

}

template <class T, int totalbits, int esbits, class FT, bool withnan>
posit_tab<T,totalbits,esbits,FT,withnan>::posit_tab(double a) 
{
	if(a == 0)
		v = 0;
	else if(a == 1)
		v = PT::POSIT_ONE; 
	else 
		v = FPT(a).v;		
}

namespace std {
    template<class T, int totalbits, int esbits, class FT, bool withnan> class numeric_limits<posit_tab<T,totalbits,esbits,FT,withnan> > {
    public:
       static posit_tab<T,totalbits,esbits,FT,withnan> max() {return posit_tab::max(); };
       static posit_tab<T,totalbits,esbits,FT,withnan> min() {return posit_tab::min(); };
        // One can implement other methods if needed
    };

	template <class T, int totalbits, int esbits, class FT, bool withnan>
	inline CONSTEXPR14 posit_tab<T,totalbits,esbits,FT,withnan> abs(posit_tab<T,totalbits,esbits,FT,withnan> z) 
	{
		return posit_tab(posit_tab::DeepInit(),z.v < 0 ? -z.v : z.v);
	}

	template <class T, int totalbits, int esbits, class FT, bool withnan>
	inline CONSTEXPR14 posit_tab<T,totalbits,esbits,FT,withnan> min(posit_tab<T,totalbits,esbits,FT,withnan> a, posit_tab<T,totalbits,esbits,FT,withnan> b)
	{
		return a <=  b ? a : b;
	}

	template <class T, int totalbits, int esbits, class FT, bool withnan>
	inline CONSTEXPR14 posit_tab <T,totalbits,esbits,FT,withnan> max(posit_tab<T,totalbits,esbits,FT,withnan> a, posit_tab<T,totalbits,esbits,FT,withnan> b)
	{
		return a >= b ? a : b;
	}
}

