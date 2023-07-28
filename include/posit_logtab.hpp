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

#include <limits>
#include "posit.h"

/**
 * @brief Table holder for the log version of tabulated posit
 */
template <class T,int totalbits, int esbits, class FFT, PositSpec positspec>
struct posit_logtab_helper
{
	static T opinv[];
	static T ophalf[];
	static T opexp2[];
	static T oplog2[];
	static T opsquare[];
	static uint32_t op2float[];
	static T opadd[];
};

/**
 * @brief Log Tabulated version of posit
 */
template <class T,int totalbits, int esbits, class FFT>
class posit_logtab
{
public:
	static constexpr PositSpec positspec = PositSpec::WithNanInf;
	// template <class T,int totalbits, int esbits, class FT, bool withnan>
	using posit_logtabns = posit_logtab_helper<T,totalbits,esbits,FFT,positspec>;
	using value_t=T;
	using fraction_t=FFT;
	static constexpr int tabsize = 1<<totalbits; 
	static constexpr int tabmask = (1<<totalbits)-1;
	static constexpr int vtotalbits = totalbits;
	static constexpr int vesbits = esbits;
	using exponenttype=FFT; 
	using PT=PositTrait<T,vtotalbits,esbits,positspec>;
	using FT=fraction_t;
	using FPT=Posit<T,vtotalbits,esbits,FFT,positspec>;
	using UnpackedLow=typename FPT::UnpackedLow;
	using UnpackedT=typename FPT::UnpackedT;
	using info=typename FPT::info;
	using utype=typename std::make_unsigned<T>::type;

	struct DeepInit{};


	posit_logtab(double a);
	posit_logtab(float a);
	posit_logtab(int a);
	explicit posit_logtab(DeepInit , T a) : v(a) {}
#ifdef POSITMUL
	posit_logtab(FPT::PositMul x) : v(x.asPosit().v) {}
#endif
	posit_logtab() : v(0) {}
	explicit posit_logtab(FPT x) : v(x.v) {}
	explicit posit_logtab(UnpackedLow u) : v(FPT(u).v) {}
	explicit posit_logtab(UnpackedT u) : v(FPT(u).v) {}
	posit_logtab(const posit_logtab & a): v(a.v) {}

	bool is_negative() const { return v < 0; }

	bool is_nan() const { return PT::withnan && v == PT::POSIT_NAN; }
	bool is_infinity() const { return positspec == PositSpec::WithNanInf && (v == PT::POSIT_PINF || v == PT::POSIT_NINF); }
	static posit_logtab nan() { return posit_logtab(DeepInit(),PT::POSIT_NAN); }
	static posit_logtab max() { return posit_logtab(DeepInit(),PT::POSIT_MAXPOS); }
	static posit_logtab min() { return posit_logtab(DeepInit(),1); }
	static posit_logtab one() { return posit_logtab(DeepInit(),PT::POSIT_ONE); }
	static posit_logtab afterone() { return posit_logtab(DeepInit(),PT::POSIT_ONE+1); }
	static posit_logtab zero() { return posit_logtab(DeepInit(),0); }
	static posit_logtab two() { return posit_logtab(DeepInit(),PT::POSIT_TWO); }
	static posit_logtab onehalf() { return posit_logtab(DeepInit(),PT::POSIT_HALF); }
	static posit_logtab pinfinity() { return posit_logtab(DeepInit(),positspec == PositSpec::WithNanInf ? PT::POSIT_PINF : PT::_POSIT_TOP); }
	static posit_logtab ninfinity() { return posit_logtab(DeepInit(),positspec == PositSpec::WithNanInf ? PT::POSIT_NINF : PT::_POSIT_TOP); }
	/*
	bool has_neg_exponent() const { return (v & 0x100) == 0; }
	*/

	friend bool operator<(const posit_logtab &a, const posit_logtab & b) { return a.v < b.v; }
	friend bool operator>(const posit_logtab &a, const posit_logtab & b) { return a.v > b.v; }
	friend bool operator<=(const posit_logtab &a, const posit_logtab & b) { return a.v <= b.v;}
	friend bool operator>=(const posit_logtab &a, const posit_logtab & b) { return a.v >= b.v; }
	friend bool operator==(const posit_logtab &a, const posit_logtab & b) { return a.v == b.v; }
	friend bool operator!=(const posit_logtab &a, const posit_logtab & b) { return a.v !=b.v; }

	posit_logtab inv() const { return posit_logtab(DeepInit(), v < 0 ? -posit_logtabns::opinv[neg_uu()] : posit_logtabns::opinv[uu()]); }
	posit_logtab half() const { return posit_logtab(DeepInit(),v < 0 ? -posit_logtabns::ophalf[neg_uu()] : posit_logtabns::ophalf[uu()]);}
	//posit_logtab twice() const;
	posit_logtab exp2() const { return posit_logtab(DeepInit(),posit_logtabns::opexp2[uu()]); }
	posit_logtab square() const { return posit_logtab(DeepInit(),posit_logtabns::opsquare[abs_uu()]); }
	posit_logtab operator-() const { return posit_logtab(DeepInit(),-v);  }
	posit_logtab log2() const { return posit_logtab(DeepInit(),v < 0 ? nan(): posit_logtabns::oplog2[uu()]); }

	friend posit_logtab operator+(const posit_logtab &a, const posit_logtab & b) { return posit_logtab(DeepInit(),posit_logtabns::opadd[a.uu()*tabsize+b.uu()]); }
	friend posit_logtab operator*(const posit_logtab &a, const posit_logtab & b) { 
		return posit_logtab(DeepInit(),posit_logtabns::opexp2[posit_logtabns::opadd[posit_logtabns::oplog2[a.uu()]*tabsize+posit_logtabns::oplog2[b.uu()]]]); }
	friend posit_logtab operator-(const posit_logtab &a, const posit_logtab & b) { return a+(-b); }
	friend posit_logtab operator/(const posit_logtab &a, const posit_logtab & b) {	
		return posit_logtab(DeepInit(),posit_logtabns::opexp2[posit_logtabns::opadd[posit_logtabns::oplog2[a.uu()]*tabsize-posit_logtabns::oplog2[b.uu()]]]); 
	}

	friend std::ostream & operator << (std::ostream & ons, const posit_logtab & p);

	posit_logtab& operator+=(const posit_logtab &a) { posit_logtab r = *this+a; v = r.v; return *this; }
	posit_logtab& operator*=(const posit_logtab &a) { posit_logtab r = *this*a; v = r.v; return *this; }
	posit_logtab& operator/=(const posit_logtab &a) { posit_logtab r = *this/a; v = r.v; return *this; }

	utype uu() const { return utype(v & tabmask);}
	utype neg_uu() const { return utype((-v) & tabmask);}
	utype abs_uu() const { return utype((v < 0 ? -v : v) & tabmask);}
	UnpackedLow unpack_low() const { return as_posit().unpack_low(); }
	UnpackedT unpack() const { return as_posit().unpack(); }
	info analyze() const { return as_posit().analyze(); }

	FPT as_posit() const { return FPT(typename FPT::DeepInit(), v); }
	operator FPT() const { return as_posit(); }
	operator float() const { return uint32_to_float(posit_logtabns::op2float[uu()]); }
	operator double() const { return (float)*this; }
	operator int() const { return (float)*this; }

	T v;

};

template <class T,int totalbits, int esbits, class FFT>
inline posit_logtab<T,totalbits,esbits,FFT> half(posit_logtab<T,totalbits,esbits,FFT> z) { return z.half(); }

template <class T,int totalbits, int esbits, class FFT>
inline posit_logtab<T,totalbits,esbits,FFT> twice(posit_logtab<T,totalbits,esbits,FFT> z) { return z.twice(); }

template <class T,int totalbits, int esbits, class FFT>
inline posit_logtab<T,totalbits,esbits,FFT> square(posit_logtab<T,totalbits,esbits,FFT> z) { 	return z.square();  }

template <class T,int totalbits, int esbits, class FFT>
inline posit_logtab<T,totalbits,esbits,FFT> inv(posit_logtab<T,totalbits,esbits,FFT> x) { return x.inv(); }

template <class T,int totalbits, int esbits, class FFT>
inline posit_logtab<T,totalbits,esbits,FFT> neg(posit_logtab<T,totalbits,esbits,FFT> z) { return -z; }

template <class T,int totalbits, int esbits, class FFT>
inline posit_logtab<T,totalbits,esbits,FFT> exp2(posit_logtab<T,totalbits,esbits,FFT> z) { return z.exp2(); }

template <class T,int totalbits, int esbits, class FFT>
inline bool is_negative(posit_logtab<T,totalbits,esbits,FFT> a)
{
	return a.v < 0;
}

namespace std {
	template <class T,int totalbits, int esbits, class FFT>
    class numeric_limits<class posit_logtab<T,totalbits,esbits,FFT> > {
    public:
    using qposit_logtab = posit_logtab<T,totalbits,esbits,FFT>;
       static qposit_logtab max() {return qposit_logtab::max(); };
       static qposit_logtab min() {return qposit_logtab::min(); };
       static qposit_logtab lowest() {return qposit_logtab(qposit_logtab::DeepInit(),qposit_logtab::PT::POSIT_MINNEG);}
       static qposit_logtab epsilon() {return qposit_logtab::afterone()-qposit_logtab::one(); };
	   static constexpr int  digits10 = ((qposit_logtab::vtotalbits-3)*30000)/100000;  // *log10(2)	       
        // One can implement other methods if needed
    };
}

namespace std
{
	template <class T,int totalbits, int esbits, class FFT>
	inline posit_logtab<T,totalbits,esbits,FFT> abs(posit_logtab<T,totalbits,esbits,FFT> z) 
	{
		return posit_logtab<T,totalbits,esbits,FFT>(posit_logtab<T,totalbits,esbits,FFT>::DeepInit(),z.v < 0 ? -z.v : z.v);
	}

	template <class T,int totalbits, int esbits, class FFT>
	inline posit_logtab<T,totalbits,esbits,FFT> min(const posit_logtab<T,totalbits,esbits,FFT> &a, const posit_logtab<T,totalbits,esbits,FFT> & b)
	{
		return a <=  b ? a : b;
	}

	template <class T,int totalbits, int esbits, class FFT>
	inline posit_logtab<T,totalbits,esbits,FFT> max(const posit_logtab<T,totalbits,esbits,FFT> &a, const posit_logtab<T,totalbits,esbits,FFT> & b)
	{
		return a >= b ? a : b;
	}
}