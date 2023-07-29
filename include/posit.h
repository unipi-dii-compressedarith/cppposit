/**
 * Copyright (C) 2017-2023 Emanuele Ruffaldi, Federico Rossi
 * 
 * Distributed under the terms of the BSD 3-Clause License.  
 * 
 * (See accompanying file LICENSE)
 * 
 * --
 */
#pragma once
#include"softback.hpp"
#include <complex>
#ifdef POSIT_VALUE_LOGGER
#include "interface/logger/logger.hpp"
#endif

#ifdef RISCV_PPU
#include "rv_posit_quickins.h"
#pragma message ( "Compiling with RISC-V PPU Support" )
#define PPU_INS(INS16_1,INS16_0,INS8,FBACK) \
if constexpr(vtotalbits == 16 && vesbits == 1) { INS16_1; } \
if constexpr(vtotalbits == 16 && vesbits == 0) { INS16_0; } \
if constexpr(vtotalbits == 8 && vesbits == 0) { INS8; } \
else {FBACK;}
#else
#define PPU_INS(INS16_1,INS16_0,INS8,FBACK) FBACK;
#endif




/**
 * @brief The posit namespace provies wrapped access to cppPosit
 */
namespace posit
{

	/**
	 * @brief The posit::math namespace provides general math functionalities 
	 */
	namespace math
	{

		/**
		 * @brief computes the dot product between posit values vectors	 
		 * The accumulating type is anything that can be converted to/from the posit:
		 *   - floating point: float, double
		 *	 - another posit
		 *	 - fixed point
		 * 	 - unpacked posit (AKA soft float)
		 *
		 * @todo decide if resulting type is Posit or another one
		 *
		 * @tparam It1 type of first iterator
		 * @tparam It2 type of second iterator
		 * @tparam A is the product type, default is self
		 * @tparam B is the accumulating type, default is self
		 * @param begin1 first iterator
		 * @param begin2 second iterator
		 * @param N length of vectors
		 */
	    template <class It1, class It2, class A, class B>
	    auto dot(It1 begin1, It2 begin2, int N, A tprod, B tsum) -> typename std::remove_reference<decltype(*begin1)>::type
	    {
	    	using PP = typename std::remove_reference<decltype(*begin1)>::type;
	    	if(N < 1)
	    		return PP(0);
	    	else
	    	{
		    	B v0 = static_cast<A>((*begin1++))*static_cast<A>((*begin2++)); // first
		    	for(int i = 1; i < N; i++)
			    	v0 += static_cast<A>((*begin1++))*static_cast<A>((*begin2++));
		    	return static_cast<PP>(v0);
		    }
	    }

	    template <class It1, class It2>
	    auto dot(It1 begin1, It2 begin2, int N) -> typename std::remove_reference<decltype(*begin1)>::type
	    {
	    	using PP = typename std::remove_reference<decltype(*begin1)>::type;
	    	return dot<It1,It2,PP,PP>(begin1,begin2,N,PP(),PP());
	    }


		/**
		 * @brief computes the dot product between posit values vectors using Kahan summation algorithm	 
		 *
		 * @tparam It1 type of first iterator
		 * @tparam It2 type of second iterator
		 * @tparam A is the product type
		 * @tparam B is the accumulating type
		 * @tparam C is the running compensation type
		 * @param begin1 first iterator
		 * @param begin2 second iterator
		 * @param N length of vectors
		 */
	    template <class It1, class It2, class A, class B, class C>
	    auto comp_dot(It1 begin1, It2 begin2, int N,A tprod, B tsum, C tcomp) -> typename std::remove_reference<decltype(*begin1)>::type
	    {
	    	using PP = typename std::remove_reference<decltype(*begin1)>::type;
	    	if(N < 1)
	    		return PP(0); 
	    	else
	    	{
		    	B vsum(static_cast<A>(*begin1++)*static_cast<A>(*begin2++)); // first
		    	C c(0);
		    	for(int i = 1; i < N; i++)
		    	{
		    		B iv = static_cast<A>(*begin1++)*static_cast<A>(*begin2++);
		    		B t = vsum + iv; // large
		    		if (math::abs(vsum) >= math::abs(iv))
		    			c += (vsum-t)+iv;
		    		else
		    			c += (iv-t)+vsum;
		    		vsum = t;
		    	}
		    	return static_cast<PP>(vsum);
		    }
	    }




	}
	



	/**
	 * @enum PositSpec
	 * @brief Specifies how a family of posit numbers behaves with respect to special numbers
	 * The fundamental special number is the opposite of zero in the posit wheel.
	 *
	 * Signless infinity has been removed
	 */
	enum PositSpec { 
		WithNone, /// never consider special numbers. The top value is never present
		WithNan, /// one top Nan (or Nar using posit spec terminology)
		WithNanInfs, /// one top Nan and two signed Infinites (for more float compatibility)
		WithInfs /// one top Nan and two signed Infinites (for more float compatibility)
	}; 

	/**
	 * @brief Trait for describing the posit specifier based on the family 
	 */
	template <PositSpec x>
	struct PositSpecTrait
	{

	};

	/**
	 * @brief Specialization without special numbers
	 */
	template <>
	struct PositSpecTrait<WithNone>
	{
		static constexpr bool withnan = false;
		static constexpr bool withinf = false;
		static constexpr bool withsignedinf = false;
	};

	/**
	 * @brief Specialization with NaN
	 */
	template <>
	struct PositSpecTrait<WithNan>
	{
		static constexpr bool withnan = true;
		static constexpr bool withinf = false;
		static constexpr bool withsignedinf = false;
	};

	/**
	 * @brief Specialization with Nan on top and infinity on side
	 */
	template <>
	struct PositSpecTrait<WithNanInfs>
	{
		static constexpr bool withnan = true;
		static constexpr bool withinf = true;
		static constexpr bool withsignedinf = true;
	};

	/**
	 * @brief Specialization with Nan on top and infinity on side
	 */
	template <>
	struct PositSpecTrait<WithInfs>
	{
		static constexpr bool withnan = false;
		static constexpr bool withinf = true;
		static constexpr bool withsignedinf = true;
	};

	template<int x>
	struct PositZero
	{

	};

	template<>
	struct PositZero<0>
	{
		using type = bool;
	};

	template<>
	struct PositZero<1>
	{
		using type = int;
	};

	/**
	 * @brief Trait used to describe the families of posit numbers
	 * The trait provides constants for constructing the posit representation and several constants of the posit expressed in the signed integer.
	 * In addition some fundamental constexpr functions are provided
	 * 
	 * @tparam T the signed integer holding the posit
	 * @tparam totalbits the number of bits of the posit less or equal then the bitsize of T. Data will be RIGHT aligned
	 * @tparam esbits the maximum number of bits for the exponent
	 * @tparam positspec_ the enumeration that controls the special numbers in a Posit. @see PositSpec
	 */
	template <class T, int totalbits, int esbits, PositSpec positspec_ >
	struct PositTrait
	{
		static_assert(std::is_signed<T>::value,"required signed T");
		static_assert(sizeof(T)*8 >= totalbits,"required enough storage T for provided bits  SREF");
		static_assert(esbits <= totalbits-3,"esbits should be at most N-3 for the cases [x01,E] and [x10,E]");

		using POSIT_STYPE = T;
		using POSIT_UTYPE = typename std::make_unsigned<T>::type; /// unsigned version of T
		static constexpr PositSpec positspec = positspec_;
		static constexpr bool withnan = PositSpecTrait<positspec_>::withnan;
		static constexpr bool withinf = PositSpecTrait<positspec_>::withinf;
		static constexpr bool withsignedinf = PositSpecTrait<positspec_>::withsignedinf;

		/// type necessary to hold the unpacked exponent
		using exponenttype = typename std::conditional<(totalbits+esbits >= sizeof(T)*8),typename  nextinttype<T>::type,T>::type;

		///@{ 
		/// @name Helper Sizes	
		static constexpr POSIT_UTYPE POSIT_MAXREGIME_BITS = totalbits-1; /// maximum number of regime bits (all except sign)
		static constexpr POSIT_UTYPE POSIT_HOLDER_SIZE = sizeof(T)*8;
		static constexpr POSIT_UTYPE POSIT_SIZE = totalbits;
		static constexpr POSIT_UTYPE POSIT_ESP_SIZE = esbits;
		static constexpr POSIT_UTYPE POSIT_EXTRA_BITS = POSIT_HOLDER_SIZE-totalbits; /// non posit bits in holder 
		///@}

		///@{ 
		/// @name Specific Bits
		static constexpr POSIT_UTYPE POSIT_ONEHELPER = 1;
		static constexpr POSIT_UTYPE POSIT_INVERTBIT = (POSIT_ONEHELPER<<(totalbits-2)); /// bit for 1/x
		static constexpr POSIT_UTYPE POSIT_MSB = POSIT_ONEHELPER<<(totalbits-1); /// most significant bit of posit: (0[*] 1 0[totalbits-1])
		static constexpr POSIT_UTYPE POSIT_SIGNBIT = POSIT_MSB; // sign bit in posit is MSB
		static constexpr POSIT_UTYPE POSIT_HOLDER_MSB = POSIT_ONEHELPER<<(POSIT_HOLDER_SIZE-1); /// most significant bit of holding type: (1 0[*])
		static constexpr POSIT_STYPE POSIT_REG_SCALE = 1<<esbits;
		///@}

		///@{ 
		/// @name Masks
	    static constexpr POSIT_UTYPE POSIT_ESP_MASK = (POSIT_ONEHELPER<< esbits)-1; /// mask for exponent: (0[*] 1[esbits])
	    static constexpr POSIT_UTYPE POSIT_MASK = ((POSIT_MSB-1)|(POSIT_MSB)); /// all posit bits to one on the right: (0[*] 1[totalbits])
		static constexpr POSIT_STYPE POSIT_MASK_NOSIGN = (POSIT_MASK >> 1); /// (0[*] 1[totalbits-1])
		static constexpr POSIT_UTYPE POSIT_SIGNMASK = ~POSIT_MASK_NOSIGN; /// mask for bits of signed part (1[*] 0[totalbits-1])
		///@}
        static constexpr POSIT_UTYPE POSIT_TWICEMASK = (POSIT_INVERTBIT | (POSIT_ONEHELPER << totalbits - 3) ) << 1;
		///@{ 
		/// @name Special Values	
		static constexpr POSIT_STYPE _POSIT_TOP = (POSIT_STYPE)POSIT_HOLDER_MSB; /// 1 0[*]
		static constexpr POSIT_STYPE _POSIT_TOPLEFT = (POSIT_STYPE)(POSIT_SIGNMASK+1); /// 1[*] 0[totlabits-2] 1
		static constexpr POSIT_STYPE _POSIT_TOPRIGHT = -_POSIT_TOPLEFT; /// 0[*] 1[totalbits-1]

		static constexpr POSIT_STYPE POSIT_INF =  withinf ? _POSIT_TOP : 0; /// right of top if present
		static constexpr POSIT_STYPE POSIT_PINF =  withsignedinf ? _POSIT_TOPRIGHT: POSIT_INF; /// right of top if present
		static constexpr POSIT_STYPE POSIT_NINF =  withsignedinf ? _POSIT_TOPLEFT : 0; /// top or left if present
		static constexpr POSIT_STYPE POSIT_NAN  = withnan ? _POSIT_TOP : 0;  /// infinity in withnan=false otherwise it is truly nan
		static constexpr POSIT_STYPE POSIT_ONE = POSIT_INVERTBIT; /// invert bit IS positive one
		static constexpr POSIT_STYPE POSIT_MONE = -POSIT_ONE ; /// trivially minus one
		static constexpr POSIT_STYPE POSIT_TWO = (POSIT_INVERTBIT | (POSIT_INVERTBIT>>(1+esbits))); /// 2.0
		static constexpr POSIT_STYPE POSIT_HALF = -(POSIT_STYPE)(POSIT_TWO ^ POSIT_SIGNMASK); /// 0.5
		static constexpr POSIT_STYPE POSIT_MAXPOS = _POSIT_TOPRIGHT - (withsignedinf ? 1 : 0); /// max value below Infinity: 1[holder-total] 1 0[total-1]
		static constexpr POSIT_STYPE POSIT_MINNEG = _POSIT_TOPLEFT - (withsignedinf ? 1 : 0); 	/// min value above -Infinity // 0[holder-total] 0 1[total-1]
		static constexpr POSIT_STYPE POSIT_AFTER0 = 1; /// right to 0:  minimal number above zero
		static constexpr POSIT_STYPE POSIT_BEFORE0 = -POSIT_AFTER0; /// left to 0: smallest number before zero

		using positzero = typename PositZero<esbits == 0>::type;
		///@}

		/// maxexponent that can be constructed
		static constexpr exponenttype maxexponent() { return withsignedinf ? POSIT_REG_SCALE * (totalbits - 3) : POSIT_REG_SCALE * (totalbits - 2); }
		/// minimumexponent
		static constexpr exponenttype minexponent() { return (-((exponenttype)POSIT_REG_SCALE) * (totalbits - 2)) ; }

		static constexpr bool posvalue_has_only_regime(POSIT_STYPE x)
		{
			return (x <= POSIT_ONE ? is_power_of_two(x) : is_power_of_two((POSIT_UTYPE)_POSIT_TOP-(POSIT_UTYPE)x));
		}
		/**
		 * @brief decoding of posit into Regime balue and number of remaining bits
		 * 
		 * @param pars input posit
		 * @return decode_posit_rs 
		 */
	    static CONSTEXPR14 std::pair<int,int> decode_posit_rs(T pars)
	    {  
	        const bool x = (pars & POSIT_HOLDER_MSB) != 0; // marker bit for > 1
	        int aindex = x ? (~pars == 0 ? POSIT_MAXREGIME_BITS : findbitleftmostC((POSIT_UTYPE)~pars)) : (pars == 0 ? POSIT_MAXREGIME_BITS : findbitleftmostC((POSIT_UTYPE)pars)); // index is LAST with !x
	        int index = aindex; // aindex > POSIT_SIZE  ? POSIT_SIZE : aindex;
	        int reg = x ? index-1 : -index;
	        int rs =  (int)POSIT_MAXREGIME_BITS < index+1 ? POSIT_MAXREGIME_BITS : index+1; //std::min((int)POSIT_MAXREGIME_BITS,index+1);
	        return {reg,rs};
	    }

		/**
		 * @brief Given a full exponent decomposes it into regime part and base exponent
		 * 
		 * @param eexponent 
		 * @return constexpr std::pair<POSIT_STYPE,POSIT_UTYPE> 
		 */
	    static constexpr std::pair<POSIT_STYPE,POSIT_UTYPE> split_reg_exp(exponenttype eexponent)
	    {
	        return {eexponent >> POSIT_ESP_SIZE, eexponent & POSIT_ESP_MASK };
	    }

	    /**
	     * @brief Computes the inverted position of the givem posit using a binary property of the wheel.
	     * Correct for non special values. Requires check for: zero, nan, and the adjacent values if signed infinities
	     *
		 * @param value
		 * @return 1/x for most values except: 
	     */
	    static CONSTEXPR14 POSIT_STYPE reciprocate(POSIT_STYPE x)
	    {
	    	bool s = false;
	    	POSIT_STYPE r;
	    	if(x < 0)
	    	{
	    		s = true;
	    		x = -x;
	    	}
	    	if(posvalue_has_only_regime(x))
	    	{
	    		r = (POSIT_STYPE)((POSIT_UTYPE)(POSIT_SIGNBIT)-(POSIT_UTYPE)x) ;
	    	}
	    	else
	    	{
	    		r = x ^ (~POSIT_SIGNBIT);
	    	}

	    	return s ? -r : r;
	    }

		/**
		 * @brief Merges regime and exponent
		 */
	    static constexpr exponenttype join_reg_exp(POSIT_STYPE reg, POSIT_UTYPE exp)
	    {
	    	return (((exponenttype)reg) * (1<<POSIT_ESP_SIZE))|exp;
	    }

        
	    /**
	     * @brief fast twice for E=0
	     */	    
	    //template <typename std::enable_if<std::is_same<positzero, PositZero<1>::type >::value, int>::type = 0>
	    static constexpr POSIT_STYPE fast_twice(POSIT_STYPE x)
	    {
            POSIT_STYPE s = -POSIT_STYPE(x < 0);
            POSIT_STYPE av = pabs(x);
            POSIT_STYPE X_invbit = av & POSIT_INVERTBIT;
            POSIT_STYPE Xs = av << 1;
            POSIT_STYPE Xs_invbit = Xs & POSIT_INVERTBIT;
            POSIT_STYPE x_ge1 = Xs >> 1;
            POSIT_STYPE x_lthalf = Xs << 1;
            POSIT_STYPE x_lt1 = bitwise_ite<POSIT_STYPE>(Xs_invbit == 0,x_lthalf,Xs ^ POSIT_TWICEMASK);
            POSIT_STYPE Y1 = bitwise_ite<POSIT_STYPE>(X_invbit == 0,x_lt1,x_ge1);
            Y1 = (POSIT_STYPE)((POSIT_UTYPE)Y1 >> 1);
            return (Y1 ^ s) - s;
			//return pcabs(x) < POSIT_MAXPOS ? x << 1 : x;
	    }

	    /**
	     * @brief fast half for E=0
	     */	    
	    //template <typename std::enable_if<POSIT_ESP_SIZE == 0, int>::type = 0>
	    static constexpr POSIT_STYPE fast_half(POSIT_STYPE x)
	    {
            POSIT_STYPE s = -POSIT_STYPE(x < 0);
            POSIT_STYPE av = pabs(x);
            POSIT_STYPE X_invbit = av & POSIT_INVERTBIT;
            POSIT_STYPE Xs = av << 1;
            POSIT_STYPE Xs_invbit = Xs & POSIT_INVERTBIT;
            POSIT_STYPE x_ge2 = Xs << 1;
            POSIT_STYPE x_lt1 = (POSIT_STYPE)((POSIT_UTYPE)Xs >> 1);
            POSIT_STYPE x_ge1 = bitwise_ite<POSIT_STYPE>(Xs_invbit == 0,Xs ^ POSIT_TWICEMASK,x_ge2);
            POSIT_STYPE Y1 = bitwise_ite<POSIT_STYPE>(X_invbit == 0,x_lt1,x_ge1);
            Y1 = (POSIT_STYPE)((POSIT_UTYPE)Y1 >> 1);
            return (Y1 ^ s) - s;
			//return pcabs(x) < POSIT_MAXPOS ? x >> 1 : x;
	    }

	    /**
	     * Complement of number in unitary range
	     */
	    //template <typename std::enable_if<POSIT_ESP_SIZE == 0, int>::type = 0>
		static constexpr POSIT_STYPE fast_one_minus_ur(POSIT_STYPE v) 
		{ 
			return POSIT_INVERTBIT-v;
		}
		/**
		 * \brief returns true if in the unit interval [0,1]
		 */
		static constexpr bool is_unitary_range(POSIT_STYPE v) { return (v & (POSIT_SIGNBIT|POSIT_INVERTBIT)) == POSIT_INVERTBIT; }

		/**
		 * \brief returns true if in the sphere interval [-1,1]
		 */
		static constexpr bool is_sphere_range(POSIT_STYPE v) { return (v & POSIT_INVERTBIT) == POSIT_INVERTBIT; };

	};

	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	class Posit;

	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto unpack_posit(const Posit<T,totalbits,esbits,FT,positspec> & p) -> typename Posit<T,totalbits,esbits,FT,positspec>::BackendT ;

	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 Posit<T,totalbits,esbits,FT,positspec> pack_posit(const typename Posit<T,totalbits,esbits,FT,positspec>::BackendT & x);


	/**
	 * @brief Unpacked representaiton of the Posit with (sign,regime,exponent,fraction)
	 * 
	 * @tparam PT is the posit trait
	 */
	template <class PT>
	struct UnpackedPosit
	{
		constexpr UnpackedPosit(NumberType t): type(t), negativeSign(false), regime(0),exp(0),fraction(0) {}
		constexpr UnpackedPosit(NumberType t, bool anegativeSign): type(t), negativeSign(anegativeSign), regime(0),exp(0),fraction(0) {}
		constexpr UnpackedPosit(bool n, typename PT::POSIT_STYPE r, typename PT::POSIT_UTYPE e, typename PT::POSIT_UTYPE f):
			 type(NumberType::Regular),negativeSign(n), regime(r), exp(e), fraction(f) {}

		NumberType type; /// type of posit special number
		bool negativeSign; // for Regular and Infinity
		typename PT::POSIT_STYPE regime; /// decoded regime value
		typename PT::POSIT_UTYPE exp;    /// decoded exponent value (positive)
		typename PT::POSIT_UTYPE fraction; /// decoded fraciton left aligned with the leading 1
	};

	template <class T>
	struct is_posit_backend: public std::false_type
	{
	};

	template <class FT, class ET>
	struct is_posit_backend<Unpacked<FT,ET> >: public std::true_type
	{
	};

	template <class PT, class Target>
	struct PositPacker
	{

	};

	template <class PT, class FT>
	struct PositPacker<PT,Unpacked<FT,typename PT::exponenttype> >
	{
		using BE = Unpacked<FT,typename PT::exponenttype>;
		using UP = UnpackedPosit<PT>;

		static CONSTEXPR14 BE unpacked_to_backend(UP up)
		{
			using BE = Unpacked<FT,typename PT::exponenttype>;
		    BE r;
		    r.type = up.type;
		    r.negativeSign = up.negativeSign;

		    if(up.type == NumberType::Regular)
		    {
		        r.fraction = cast_msb<typename PT::POSIT_UTYPE,PT::POSIT_HOLDER_SIZE,FT,BE::FT_bits>()(up.fraction);
		        r.exponent = PT::join_reg_exp(up.regime,up.exp);
		    }
			return r;
		}

		static CONSTEXPR14 UP backend_to_unpacked(BE b)
		{
			if(b.type == NumberType::Regular)
			{
				auto eexponent = clamp<decltype(b.exponent)>(b.exponent,PT::minexponent(),PT::maxexponent()); // no overflow
				auto rr = PT::split_reg_exp(eexponent);
				auto frac = cast_msb<FT,sizeof(FT)*8,typename PT::POSIT_UTYPE,sizeof(typename PT::POSIT_UTYPE)*8>()(b.fraction);
				return UP(b.negativeSign,rr.first,rr.second,frac);
			}
			else
			{
				return UP(b.type,b.negativeSign);
			}		
		}		
	};

	template <class B>
	struct BackendFloat
	{
		using bt = B;
		using OBE = Unpacked<uint64_t,int>;
		struct single_tag{};

		BackendFloat() {}
		BackendFloat(single_tag, uint32_t x): v(uint32_to_float(x)) {}
		BackendFloat(int x) : v(x) {}
		BackendFloat(long x) : v(x) {}
		BackendFloat(float x) : v(x) {}
		BackendFloat(double x) : v(x) {}

		template <class FT>
		static BackendFloat make_floati(typename FT::holder_t x) { return BackendFloat(OBE::make_floati<FT>(x)); }

		constexpr operator float () const {return (float)v;}
		constexpr operator double () const {return (double)v;}
		constexpr operator int () const {return (int)v;}
		constexpr operator long () const {return (long)v;}

	 	BackendFloat operator + (BackendFloat o) const { return BackendFloat(v+o.v); }
		BackendFloat operator * (BackendFloat o) const { return BackendFloat(v*o.v); }
	 	BackendFloat operator - (BackendFloat o) const { return BackendFloat(v-o.v); }
	 	BackendFloat operator / (BackendFloat o) const { return BackendFloat(v/o.v); }
		bt v;
	};


	template <class B>
	struct is_posit_backend<BackendFloat<B> >: public std::true_type
	{
	};

	template <class PT, class FT>
	struct PositPacker<PT,BackendFloat<FT> >
	{
		using BE = BackendFloat<FT>;
		using OBE = Unpacked<uint64_t,typename PT::exponenttype>;
		using UP = UnpackedPosit<PT>;

		static CONSTEXPR14 BE unpacked_to_backend(UP up)
		{
			return BE((FT)PositPacker<PT,OBE>::unpacked_to_backend(up));
		}

		static CONSTEXPR14 UP backend_to_unpacked(BE b)
		{
			return PositPacker<PT,OBE>::backend_to_unpacked(OBE(b.v));
		}
	};

	template <class B>
	std::ostream & operator<<(std::ostream &ons, BackendFloat<B> const &o) 
    {
    	ons << "float(" << o.v << ")";
    	return ons;
    }

    /**
     * @tparam FT is the Fixed Trait <T,N,F>
     */
	template <class FT, class ET>
	struct BackendFixed
	{
		using ft = FT;
		using value_t = typename ft::value_t;
		using OBE = Unpacked<uint32_t,ET>;
		struct single_tag{};
		struct raw_tag{};

		BackendFixed() : v(0) {}
		BackendFixed(raw_tag, value_t x) : v(x) {}
		BackendFixed(single_tag, uint32_t x): BackendFixed(OBE::template make_floati<single_trait>(x)) {}
		BackendFixed(int x) : BackendFixed(OBE(x))  {}
		BackendFixed(float x) : BackendFixed(OBE(x)) {}
		BackendFixed(double x) : BackendFixed(OBE(x))  {}
		BackendFixed(OBE u): v(u.template pack_xfixed<FT>()) {}

		template <class XFT>
		static BackendFixed make_floati(typename XFT::holder_t x) { return BackendFixed(OBE::template make_floati<XFT>(x)); }

		static BackendFixed fromraw(value_t v) { return BackendFixed(raw_tag(),v); }

		operator OBE () const { return OBE::template make_fixed<FT>(v); }
		constexpr operator float () const { return OBE::template make_fixed<FT>(v);}
		constexpr operator double () const { return OBE::template make_fixed<FT>(v);}
		constexpr operator int () const { return OBE::template make_fixed<FT>(v);}

	 	BackendFixed operator + (BackendFixed o) const { return fromraw(v+o.v); }
		BackendFixed operator * (BackendFixed o) const { return fromraw(v*o.v); }
	 	BackendFixed operator - (BackendFixed o) const { return fromraw(v-o.v); }
	 	BackendFixed operator /(BackendFixed o) const { return fromraw(v/o.v); }

		value_t v;
	};

	template <class B, class ET>
	struct is_posit_backend<BackendFixed<B,ET> >: public std::true_type
	{
	};

	template <class FT, class ET>
	std::ostream & operator<<(std::ostream &ons, BackendFixed<FT,ET> const &o) 
    {
    	ons << "fix(" << o.v << " = " << (float)o << ")";
    	return ons;
    }

	template <class PT, class FT>
	struct PositPacker<PT,BackendFixed<FT,typename PT::exponenttype> >
	{
		using BE = BackendFixed<FT,typename PT::exponenttype>;
		using OBE = typename BE::OBE;
		using UP = UnpackedPosit<PT>;

		static CONSTEXPR14 BE unpacked_to_backend(UP up)
		{
			return BE::fromraw(PositPacker<PT,OBE>::unpacked_to_backend(up).template pack_xfixed<FT>());
		}

		static CONSTEXPR14 UP backend_to_unpacked(BE b)
		{
			return PositPacker<PT,OBE>::backend_to_unpacked(OBE::template make_fixed<FT>(b.v));
		}
	};


	/**
	 * @brief Posit value type
	 * Stores the posit in the LSB part of the holding type T as signed integer
	 *
	 * Operations are implemented at 4 levels:
	 * - Level 1: direct manipulation of bit representation 
	 * - Level 2: decoding of the posit in its componentds (s,k,E,F)
	 * - Level 3: decoding and manipulation of the posit in full exponent form (s,X,f)
	 * - Level 4: using the posit backend (softfloat by Unpacked, FPU by BackendFloat, fixed point by BackendFixed)
	 *
	 * \tparam T the holding type that has to be signed due to complement 2 sign method
	 * \tparam totalbits the significant bits of posit stored in T right aligned. Due to the 2 complement scheme the MSB bits are extension of the sign
	 * \tparam esbits the size of the exponent 
	 * \tparam FT the backend type that can be: unsigned integer for softposit, float/double for emulation, generic Backend, e.g. fixed point
	 * \tparam positspec specifies the number capabilities
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	class Posit
	{
	public:

	#ifdef POSIT_VALUE_LOGGER
		static Logger<double> _logger;
	#endif

		template <class VV>
		void logValue(VV v) 
		{
			#ifdef POSIT_VALUE_LOGGER
				_logger.logValue(v);
			#endif
		}

		using PT=PositTrait<T,totalbits,esbits,positspec>;
		struct DeepInit{}; /// special constant used internally for safely constructing the posit from raw

	    enum { vtotalbits = totalbits, vesbits = esbits};

	    using value_t = T;

	    /// FT can be a Backend otherwise select it from float or integral
	    using BackendT=typename std::conditional<is_posit_backend<FT>::value,FT,typename std::conditional<std::is_floating_point<FT>::value,BackendFloat<FT>,Unpacked<FT,typename PT::exponenttype> >::type >::type;
	    using UnpackedPosit = posit::UnpackedPosit<PT>;
		using exponenttype = typename PT::exponenttype;
		using single_tag = typename BackendT::single_tag;
		T v; // index in the N2 space

#if 0
		/// known limit: 
		CONSTEXPR14 auto twice_bits() const -> Posit<int_least_bits<int_least_bits<totalbits*2>::signed_type ,totalbits*2,esbits,FT,positspec>
		{
			using target_type = int_least_bits<totalbits*2>::signed_type;
			return Posit<int_least_bits<target_type,totalbits*2,esbits,FT,positspec>::from_sraw(((target_type)v)<<totalbits);
		}
#endif

		/// returns half (Level 1)
		CONSTEXPR14 Posit half() const;

		/// returns half (Level 1)
		CONSTEXPR14 Posit twice() const;

		/// transform from bit representation (Level 1) to unpacked low (Level 2)
		CONSTEXPR14 UnpackedPosit unpack_low() const;

		/// transform from unpacled low (Level 2) to bit representation (Level 1)
		static CONSTEXPR14 Posit pack_low(UnpackedPosit);

		/// transform from Level 2 to Level 4 
		static CONSTEXPR14 BackendT unpacked_low2full(UnpackedPosit x) { return PositPacker<PT,BackendT>::unpacked_to_backend(x); } 

		/// transform from Level 2 to Level 4
		static CONSTEXPR14 UnpackedPosit unpacked_full2low(BackendT x) { return PositPacker<PT,BackendT>::backend_to_unpacked(x); }

		#ifdef RISCV_PPU
		template <class PT2>
		PT2 to_posit() {
			// Destination Posit type has 0 exponent bits and it is hw supported
			if constexpr(PT2::vesbits == 0 && (PT2::vtotalbits == 16 || PT2::vtotalbits == 8)) {
						// Posit16,1->PT2 (can either be 16,0 or 8,0 [TODO: fix this])	 						   		                     
				PPU_INS(return PT2(float(*this)),
						// Posit16,0->P8,0 (can only be P8,0)
						return PT2::from_sraw(__fcvt_p16_0_p8(v)),
						// Posit8,0->P16,0 
						return PT2::from_sraw(__fcvt_p8_p16_0(v)),
						// PositX,Y->PT2 PositX,Y should be P8,1 here					
						return PT2(float(*this));
					)
			} // Support hw conversion for P16,1 as destination type
			else if constexpr(PT2::vtotalbits == 16) {
				PPU_INS(return *this,
						return PT2::from_sraw(__fcvt_p16_0_p16_1(v)),
						return PT2::from_sraw(__fcvt_p8_p16_1(v)),
						return PT2(float(*this));
					)
			}
			// Other cases fallback
			return PT2(float(*this)); // TBD
		}
		#else
		/**
		 * @brief Convert the posit to another posit described by PT2
		 * \tparam PT2 is a posit type declaration 
		 * (e.g. posit::Posit<int8_t, 8 , 0, uint_fast32_t, posit::PositSpec::WithNan>)
		**/ 
		template <class PT2>
		PT2 to_posit() {
			if constexpr(PT2::vesbits == 0 && esbits == 0) {
					using NT = typename PT2::value_t;
					NT pt;
					if constexpr(PT2::vtotalbits > totalbits)
						pt = ((NT)v) << (PT2::vtotalbits - totalbits);
					else
						pt = NT(v >> (totalbits - PT2::vtotalbits));
					return PT2::from_sraw(pt);
			}
			return PT2(float(*this));
		}
		#endif

		/**
		 * @brief Convert the posit to a backend fixed point for quire-like accumulation
		 * \tparam QuireT is the quire type (e.g. uint64_t)
		 */
		template <class QuireT>
		QuireT 	to_quire() {
			BackendT be = this->to_backend();
			QuireT q = (QuireT)(be.v);
			bool sign = (be.v < 0);
			q = (sign)? -q:q; // start with abs value
			q << (sizeof(q)-sizeof(be.v))*8;
			return (sign)?-q:q; // restore sign
		}
		///@{ 
		/// @name Comparison Functions

		/// returns true if the two numbers are comparable
	 	constexpr bool comparable(const Posit & u) const { return !(PT::withnan && (is_nan()||u.is_nan())); }
	    friend constexpr bool operator == (const Posit & a, const Posit & u)  { return a.comparable(u) && a.v == u.v; }
	    friend constexpr bool operator == (const Posit & a, const double & d)  { return d == (double)a; }
	    friend constexpr bool operator == (const Posit & a, const float & f)  { return f == (float)a; }
	    friend constexpr bool operator != (const Posit & a, const Posit & u)  { return a.comparable(u) && a.v != u.v;  }
	    friend constexpr bool operator < (const Posit & a, const Posit & u)   { return a.comparable(u) && a.v < u.v;; }
	    friend constexpr bool operator <= (const Posit & a, const Posit & u)  { return a.comparable(u) && a.v <= u.v; }
	    friend constexpr bool operator > (const Posit & a, const Posit & u)  { return a.comparable(u) && a.v > u.v; }
	    friend constexpr bool operator >= (const Posit & a, const Posit & u)  { return a.comparable(u) && a.v >= u.v; }

	    ///@}

	    static constexpr Posit ldexp(const Posit & u, int exp); // exponent product

	    /// empty constructor
		constexpr Posit() : v(0) {}

		/// 
		CONSTEXPR14 explicit Posit(single_tag t, uint32_t p) { v = pack_posit<T,totalbits,esbits,FT,positspec>(BackendT(t,p)).v; }

	    /// construct passing the holding type x
		CONSTEXPR14 explicit Posit(DeepInit, T x) : v(x) {} 


		/// construct from decomposed (s, R,E,F)
		CONSTEXPR14 explicit Posit(UnpackedPosit u) : v(pack_low(u).v) {} 

		/// construct from fully unpacked floating (s,e,F)
		CONSTEXPR14 explicit Posit(BackendT u) : v(pack_posit<T,totalbits,esbits,FT,positspec>(u).v) {} 

		///@{ 
		/// @name Conversion constructors
			CONSTEXPR14 Posit(int i): Posit(BackendT(i)) {}
			CONSTEXPR14 Posit(long int i): Posit(BackendT(i)) {}
			CONSTEXPR14 Posit(long long int i): Posit(BackendT(i)) {}
			CONSTEXPR14 Posit(unsigned int i): Posit(BackendT(i)) {}
			CONSTEXPR14 Posit(long unsigned int i): Posit(BackendT(i)) {}
			CONSTEXPR14 Posit(long long unsigned int i): Posit(BackendT(i)) {}
			CONSTEXPR14 Posit(std::complex<float> c): Posit(BackendT(c.real())) {}
			CONSTEXPR14 Posit(std::complex<double> c): Posit(BackendT(c.real())) {}
		 ///@}

	#ifndef FPGAHLS
		#ifdef RISCV_PPU
			#ifndef RISCV_PPU_FX
			CONSTEXPR14 Posit(float f) {
				PPU_INS(v=T(__fcvt_f32_p16_1(f)),v=T(__fcvt_f32_p16_0(f)),v=T(__fcvt_f32_p8(f)),v=Posit(BackendT(f)).v);
			}
			#else 
	    	CONSTEXPR14 Posit(float f): Posit(BackendT(f)) {}
			#endif
		#else
	    	CONSTEXPR14 Posit(float f): Posit(BackendT(f)) {}
		#endif
		CONSTEXPR14 Posit(double d): Posit(BackendT(d)) {}

		CONSTEXPR14 Posit & operator= (float f) {
			PPU_INS(v= __fcvt_f32_p16_1(f),v= __fcvt_f32_p16_0(f),v= __fcvt_f32_p8(f),v=Posit(f).v);
			return *this;		
		}

		CONSTEXPR14 Posit & operator= (double f) {
			v = Posit(f).v;
			return *this;		
		}

		CONSTEXPR14 Posit & operator= (int f) {
			v = Posit(f).v;
			return *this;		
		}
	#endif
		/// builds a Posit from the given floating point expressed as the associated integer
		/// e.g. from_floatint<single_trait>(0x123456)
		template <class XFT>
		static CONSTEXPR14 Posit from_floatval(typename XFT::holder_t x) { return BackendT::template make_floati<XFT>(x); }

		/// builds a Posit from the signed number
		static CONSTEXPR14 Posit from_sraw(T x) { return Posit(DeepInit(), x); }

		/// builds a Posit from the unsigned number
		static CONSTEXPR14 Posit from_uraw(typename PT::POSIT_UTYPE x) { return Posit(DeepInit(), x); }

		/// returns the underlying value
		constexpr T to_sraw() const { return v;}

		/// returns the unsigned value
		constexpr typename PT::POSIT_UTYPE to_uraw() const { return (typename PT::POSIT_UTYPE)v;}

		/// Level 1 to Level 4
		#ifdef RISCV_PPU
			#ifdef RISCV_PPU_FX
			constexpr BackendT to_backend() const { 
				BackendT b;
				PPU_INS(b.v = __fcvt_p16_1_fx64(v),b.v = __fcvt_p16_0_fx32(v),b.v=  __fcvt_p8_fx16(v),return (unpack_posit<T,totalbits,esbits,FT,positspec>(*this)); );
				return b;
			}		
			#else		
			constexpr BackendT to_backend() const { 
				return unpack_posit<T,totalbits,esbits,FT,positspec>(*this);
			}
			#endif
		#else
			constexpr BackendT to_backend() const { 
				return unpack_posit<T,totalbits,esbits,FT,positspec>(*this);
			}
		#endif

		/// absolute value (Level 1) 
		constexpr Posit abs()  const { return from_sraw(pcabs(v));  }

		/// negation (Level 1)
		constexpr Posit neg()  const { return from_sraw(-v); }; 

		constexpr Posit operator-() const { return neg(); } 

		/// reciprocate using digital negation
		constexpr Posit operator~() const { return reciprocate(); } 

	    /// returns true if the value is NaN
	    constexpr bool is_nan() const { return PT::withnan && v == PT::POSIT_NAN; } 
	    /// returns true if it is any infinity
		constexpr bool is_infinity() const { return (PT::withsignedinf && (v == PT::POSIT_PINF || v == PT::POSIT_NINF))||(PT::withsignedinf && v == PT::POSIT_INF); }
		/// returns true for zero
		constexpr bool is_zero() const { return v == 0; }
		/// returns true for one
		constexpr bool is_one() const { return v == PT::POSIT_ONE; }
		/// moves clockwise in the regular numbers. Any special number is stationary
		constexpr Posit prev() const { return from_sraw(v > PT::POSIT_MAXPOS || v <= PT::POSIT_MINNEG ? v : v-1); }
		/// moves anterclockwise in the regular numbers. Any special number is stationary
		constexpr Posit next() const { return from_sraw(v < PT::POSIT_MINNEG || v >= PT::POSIT_MAXPOS ? v : v+1); }
		
		constexpr Posit normalize() const { 
			if(abs().v <= PT::POSIT_ONE) return from_sraw(v);
			return (v & PT::POSIT_SIGNBIT) != 0 ? from_sraw(v + PT::POSIT_INVERTBIT):from_sraw(v - PT::POSIT_INVERTBIT);
		}

		/**
		 * \brief returns true if in the unit interval [0,1]  (Level 1)
		 */
		constexpr bool is_unitary_range() const { return PT::is_sphere_range(v); }

		/**
		 * \brief returns true if in the sphere interval [-1,1] (Level 1)
		 */
		constexpr bool is_sphere_range() const { return PT::is_sphere_range(v); }

		///@{ 
		/// @name Arithmetic operations

		Posit& operator+=(const Posit &a) { Posit r = *this+a; v = r.v; return *this; }
		Posit& operator-=(const Posit &a) { Posit r = *this-a; v = r.v; return *this; }

		friend CONSTEXPR14 Posit operator-(const Posit & a, const Posit & b)  { return a + (-b); }

		/// level 4 product 
		friend CONSTEXPR14 Posit operator*(const Posit & a, const Posit & b) 
		{
			return pack_posit<T,totalbits,esbits,FT,positspec>(a.to_backend()*b.to_backend());
		}

		/// fused multiply and addition 
		/// @todo improve
		friend CONSTEXPR14 Posit fma(const Posit & a, const Posit & b, const Posit & c)
		{
			return pack_posit<T,totalbits,esbits,FT,positspec>(a.to_backend()*b.to_backend()+c.to_backend());
		}

		/// multiply and assign operation 
		CONSTEXPR14 Posit & operator*= (const Posit & b)
		{
			*this = pack_posit<T,totalbits,esbits,FT,positspec>(to_backend()*b.to_backend());
			return *this;
		}

		/// dive and assign operation
		friend CONSTEXPR14 Posit operator/(const Posit & a, const Posit & b)  { return pack_posit< T,totalbits,esbits,FT,positspec> (a.to_backend()/b.to_backend()); }
	    Posit & operator/= (const Posit & a) { auto x = *this / a; v = x.v; return *this; }

	    friend CONSTEXPR14 Posit operator+(const Posit & a, const Posit & b)
	    {
            auto ab = a.to_backend();
            auto bb = b.to_backend();
            auto rb = ab+bb;
            auto res = pack_posit<T,totalbits,esbits,FT,positspec>(rb);
	        return a.is_zero() ? b : b.is_zero() ? a: res;
	    }


		/**
		 * @brief returns 1/x as Level 1
		 * Special Cases:
		 *	 zero should give positive infinity, if present, nan otherwise
		 *	 nan  should be nan, if present
		 *	 +inf/-inf should give zero, if present
		 *	 before0,after0 should give infinity, if present
		 *
		 * @return constexpr Posit 
		 */
		constexpr Posit reciprocate() const 
		{
			return is_infinity() ? zero() : (v == PT::POSIT_NAN ? nan() : (v == 0 ? pinf() : from_sraw(PT::reciprocate(v))));
		}	    


		///@{ 
		/// @name Special Constants

		static constexpr Posit zero() { return from_sraw(0); }
		static constexpr Posit inf() { return from_sraw(PT::POSIT_PINF); }
		static constexpr Posit pinf() { return from_sraw(PT::POSIT_PINF); }
		static constexpr Posit ninf() { return from_sraw(PT::POSIT_NINF); }
		static constexpr Posit max() { return from_sraw(PT::POSIT_MAXPOS); }
		static constexpr Posit min() { return from_sraw(PT::POSIT_AFTER0); }
		static constexpr Posit lowest() { return from_sraw(PT::POSIT_MINNEG); }

		static constexpr Posit nan() { return from_sraw(PT::POSIT_NAN); }
		static constexpr Posit infinity() { return from_sraw(PT::POSIT_PINF); }
		static constexpr Posit one() { return from_sraw(PT::POSIT_ONE); }
		static constexpr Posit two() { return from_sraw(PT::POSIT_TWO); }
		static constexpr Posit mone() { return from_sraw(PT::POSIT_MONE); }
		static constexpr Posit onehalf() { return from_sraw(PT::POSIT_HALF); }

		///@}

		/**
		 * @brief Relu Activation function (Level 1)
		 * Returns 0 for negative and not nan, otherwise identity
		 */
		friend Posit relu(const Posit & a) { return a.is_nan() || a.v >= 0 ? a : zero(); }

		/**
		 * @brief computes the dot product between posit values vectors	 
		 *
		 * @todo decide if resulting type is Posit or another on
		 * @tparam It1 type of first iterator
		 * @tparam It2 type of second iterator
		 * @param begin1 first iterator
		 * @param begin2 second iterator
		 * @param N length of vectors
		 */
	    template <class It1, class It2>
	    static CONSTEXPR14 Posit dot(It1 begin1, It2 begin2, int N)
	    {
	    	return math::dot(begin1,begin2,N,Posit(),Posit());
	    }

		/// @todo only for non float backend
		constexpr uint32_t as_float_bin() const { return to_backend().template pack_xfloati<single_trait>(); }


		///@{ 
		/// @name Type casts

		constexpr operator BackendT() const { return to_backend(); }

	#ifndef FPGAHLS
		#ifdef RISCV_PPU
		constexpr operator float() const { 
			PPU_INS(return __fcvt_p16_1_f32(v),return __fcvt_p16_0_f32(v),return __fcvt_p8_f32(v),return to_backend());
		}
		#else
		constexpr operator float() const { return to_backend(); }
		#endif
		constexpr operator double() const { return to_backend(); }
	#endif
		constexpr operator int() const { return (int)to_backend(); }
		constexpr operator long() const { return (int)to_backend(); }
		constexpr operator bool() const { return !is_zero();}
		constexpr operator std::complex<float>() const {return std::complex<float>((float)to_backend());}
		constexpr operator std::complex<double>() const {return std::complex<float>((float)to_backend());}
		constexpr operator unsigned char() const { return (int)to_backend();}
		constexpr operator signed char() const { return (int)to_backend();}
		constexpr operator short unsigned int() const { return (int)to_backend();}
		constexpr operator short int() const { return (int)to_backend();}
		constexpr operator unsigned int() const { return (int)to_backend();}
		constexpr operator long unsigned int() const { return (int)to_backend();}		
		constexpr operator long long unsigned int() const { return (int)to_backend();}		
		constexpr operator long long int() const { return (int)to_backend();}		

		
		constexpr bool operator==(double d) {
			return d == (double)*this;
		}

		constexpr bool operator==(float f) {
			return f == (float)*this;
		}	

		constexpr bool operator==(Posit p) {
			return v == p.v;
		}			
					
		///@}

		///@{ 
		/// @name Special Functions

		/// log_e(x) = log2(x)/log2(e) 
		constexpr Posit log() const { return Posit(std::log((double)*this)); }

		//constexpr Posit twice_pseudosigmoid() const { return PT::POSIT_ESP_SIZE == 0 ? from_sraw((PT::POSIT_INVERTBIT+(v >> 1))): nan(); };


		/// 1/(exp(-x)+1)
		///		div2(addinvbit(div2(x)))
        // q = bitshift((cast(self.invbit,'like',tena)),1)+1;
        //        Y = cast(bitshift(cast(X,'like',tena)+q,-2),'like',X);

	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit pseudosigmoid() const { 
            //return from_sraw((v + 127 +2) *  64/256);
            return from_sraw((v+(PT::POSIT_INVERTBIT << 1) + 2) >> 2);
            //return from_sraw((PT::POSIT_INVERTBIT+(v >> 1))>>1); 
        };

    // (X xor signbit) >> 2
	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit pseudosigmoidalt() const { 
            using POSIT_UTYPE = typename PT::POSIT_UTYPE;
            POSIT_UTYPE uv = (POSIT_UTYPE)v;
            return from_sraw( (T)((uv ^ PT::POSIT_SIGNMASK) >> 2));
        };
		/// (exp(z)-1)
		///		-mul2(c1(inv(div2(addinvbit(div2(-x))))))
	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit elu() const { return from_sraw((PT::POSIT_INVERTBIT+(v >> 1))>>1); };

		/// ln(1+exp(x))
		/// x > .... = x
		/// x = 0 == ??? 
	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		Posit pseudosoftplus() const { return *this > Posit(3) ? *this : (-*this).pseudosigmoid().reciprocate().log(); };

		/// k*sigm(k*x)-k/2
	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit pseudotanh(int k) const { return Posit(k)*((Posit(k)*from_sraw(v)).pseudosigmoid())-Posit(k/2);}

		/// 2 sigm(2x)-1 = 1-(2 sigm(2x))
		/// 2sigm(2x) is [-2,2] ==> [-1,1]
		/// 2(0.5-sigm(2x))
	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit pseudotanh() const { return (twice().pseudosigmoid()).twice()-one();}


	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit fastTanh() const { 
		   //auto y = ((v < 0)? this->twice():this->twice().neg()).pseudosigmoid().twice().one_minus_ur();
		   //return (v < 0  ? -y : y);

		   return (v < 0)? this->twice().pseudosigmoid().twice().one_minus_ur().neg():this->twice().neg().pseudosigmoid().twice().one_minus_ur();
		}

	    //template <typename std::enable_if<esbits == 0, int>::type = 0>
		constexpr Posit fastELU() const {
			// Holder type to increase precision
			//using HP = Posit<int16_t,16,0,uint16_t,positspec>;
			//HP h(this->v);

			return (v >= 0)? *this: this->neg().pseudosigmoid().reciprocate().half().one_minus_ur().twice().neg();
		}
		/**
		 * \breif If the value is in the unit interval [0,1] it returns 1-x if esbits=0
		 */ 
		constexpr Posit one_minus_ur() const { return PT::POSIT_ESP_SIZE == 0 ? from_sraw(PT::POSIT_INVERTBIT-v) : one()-*this; }

		///@}

	};


	#ifdef POSIT_VALUE_LOGGER
		template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
		Logger<double> Posit<T,totalbits,esbits,FT,positspec>::_logger;
	#endif


	#ifndef FPGAHLS
	/**
	 * Plot Posits using manipulator for expresssing differently
	 */
	template <class T, int totalbits, int esbits, class FT, PositSpec positspec>
	std::ostream & operator << (std::ostream & ons, Posit<T,totalbits,esbits,FT,positspec> const & o)
	{
		// mapping std iostream flags as Posit printing
		// - as float
		// - as raw
		// - as parsed
		auto af = ons.flags();
		auto f = af & std::ios::basefield;
		ons.setf(std::ios::dec, std::ios::basefield);
		if(f == std::ios::hex)
		{
			ons << o.v;
		}
		else if(f == std::ios::dec)
		{
			ons << (float)o;
		}
		else if(f == std::ios::oct)
		{
			ons << o.to_backend();
		}	
		// reset
		ons.setf(af & std::ios::basefield, std::ios::basefield);
		return ons;
	}
	#endif

	template <class T, int totalbits, int esbits, class FT, PositSpec positspec>
	constexpr Posit<T,totalbits,esbits,FT,positspec> neg(Posit<T,totalbits,esbits,FT,positspec> x) { return -x; }

	template <class T, int totalbits, int esbits, class FT, PositSpec positspec>
	constexpr Posit<T,totalbits,esbits,FT,positspec> reciprocate(Posit<T,totalbits,esbits,FT,positspec> x) { return ~x; }

	/// helper class
	template <class T, int hbits,int ebits, bool zeroes>
	struct msb_exp
	{

	};

	/// helper class
	template <class T, int hbits,int ebits>
	struct msb_exp<T,hbits,ebits,true>
	{
		 constexpr T operator()(T) const
		{
			return 0;
		}
	};

	/// helper class
	template <class T, int hbits,int ebits>
	struct msb_exp<T,hbits,ebits,false>
	{
	 constexpr T operator()(T exp) const
		{
			return exp << (hbits-ebits);
		}

	};

	/**
	 * @brief Unpacks Posit
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @return UnpackedPosit 
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto Posit<T,totalbits,esbits,FT,positspec>::unpack_low() const -> UnpackedPosit
	{
	    using PT=PositTrait<T,totalbits,esbits,positspec>;
	    using POSIT_UTYPE = typename PT::POSIT_UTYPE;
	    //using POSIT_STYPE = typename PT::POSIT_STYPE;

		if(is_infinity()) // infinity
	    {
	    	return UnpackedPosit(NumberType::Infinity, v < 0);
	    }
	    else if(is_nan())
	   	{	
	    	return UnpackedPosit(NumberType::NaN);
	   	}	
	    else if(v == 0)
	    	return UnpackedPosit(NumberType::Zero);
		else
		{
	        //constexpr int POSIT_RS_MAX = PT::POSIT_SIZE-1-esbits;

			//r.type = BackendT::Regular;
			bool negativeSign = (v & PT::POSIT_SIGNBIT) != 0;
			//std::cout << "unpacking " << std::bitset<sizeof(T)*8>(pa) << " abs " << std::bitset<sizeof(T)*8>(pcabs(pa)) << " r.negativeSign? " << r.negativeSign << std::endl;
	        T pa = negativeSign ? -v : v;
			//	std::cout << "after " << std::hex << pa << std::endl;

	        POSIT_UTYPE pars1 = pa << (PT::POSIT_EXTRA_BITS+1); // MSB: RS ES FS MSB
	        auto q = PT::decode_posit_rs(pars1);
	        int reg = q.first;
	        int rs = q.second;
	        POSIT_UTYPE pars2 = pars1 << rs; // MSB: ES FS
	        POSIT_UTYPE exp = bitset_leftmost_get_const<POSIT_UTYPE,esbits>()(pars2); //        bitset_leftmost_getT(pars,esbits);
	        POSIT_UTYPE pars = pars2 << esbits; // MSB: FS left aligned in T

	        return UnpackedPosit(negativeSign,reg,exp,pars);
		}
	}

	/**
	 * @brief Packs into Posit
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @param x 
	 * @return Posit 
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto Posit<T,totalbits,esbits,FT,positspec>::pack_low(UnpackedPosit x) -> Posit
	{
		using PP=Posit<T,totalbits,esbits,FT,positspec>;
	    using PT=typename Posit<T,totalbits,esbits,FT,positspec>::PT;
	    using POSIT_UTYPE = typename PT::POSIT_UTYPE;
	    using POSIT_STYPE = typename PT::POSIT_STYPE;

	    switch(x.type)
		{
			case NumberType::Infinity:
				// if infinity is missing return nan
				return positspec != PositSpec::WithNan ? (x.negativeSign ? PP::ninf(): PP::pinf()): PP::nan();
			case NumberType::Zero:
				return PP(typename PP::DeepInit(),0);
			case NumberType::NaN:
				// if nan is missing return infinity
				return PT::withsignedinf ? PP::pinf() : PP::nan();
			default:
				break;
		}

		auto exp = x.exp;
		auto reg = x.regime;

	    // for reg>=0: 1 0[reg+1] => size is reg+2 
	    // for reg <0: 0[-reg] 0  => size is reg+1
	    auto rs = -reg+1 > reg+2 ? -reg+1:reg+2; //std::max(-reg + 1, reg + 2);  MSVC issue
	    auto es = (totalbits-rs-1) < esbits ? (totalbits-rs-1): esbits; //std::min((int)(totalbits-rs-1),(int)esbits);  MSVC issue
	    

	    POSIT_UTYPE regbits = reg < 0 ? (PT::POSIT_HOLDER_MSB >> -reg) : (PT::POSIT_MASK << (PT::POSIT_HOLDER_SIZE-(reg+1))); // reg+1 bits on the left
		POSIT_UTYPE eexp = msb_exp<POSIT_UTYPE,PT::POSIT_HOLDER_SIZE,esbits,(esbits == 00)>()(exp);
		POSIT_UTYPE fraction =  x.fraction;
		POSIT_STYPE p = ((fraction >> (rs+es+1)) | (eexp >> (rs+1)) | (regbits>>1)) >> (sizeof(T)*8-totalbits);
		//std::cout << "incoming " << x << std::endl;
		//std::cout << "fraction before " << std::bitset<sizeof(FT)*8>(x.fraction) << " and " << " after " << std::bitset<sizeof(POSIT_UTYPE)*8>(fraction) << " residual exponent " << exp << " from " << eexponent <<  " and regime " << reg << std::endl;
		//std::cout << "output sign " << std::bitset<sizeof(T)*8>(p) << " then " << std::bitset<sizeof(T)*8>(-p) << std::endl; 
	    return PP(typename PP::DeepInit(),x.negativeSign ? -p : p);
	}

	/**
	 * @brief Computes half as Level 1 for es=0, Level 2 for es != 0
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @return Posit<T,totalbits,esbits,FT,positspec> 
		verify for posit8 half() == X/X::two()
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto Posit<T,totalbits,esbits,FT,positspec>::half() const -> Posit<T,totalbits,esbits,FT,positspec>
	{
		if(esbits == 0)
		{
			return from_sraw(PT::fast_half(v));
		}
		else
		{
			UnpackedPosit q = unpack_low();
			if(q.type == NumberType::Regular)
			{
				// +- 2^(R expmax + E) 1.xyz  == +- 2^(exp) 1.xyz
				// where xyz are decimal digits
				// 1.xyz / 2     => 0.1xyz ==> just exp--
				//
				// exp-- mean E-- if E s not null
				// otherwise R-- and exp 
				if(q.exp == 0)
				{
					q.regime--; // will it undrflow?
					q.exp = PT::POSIT_REG_SCALE-1; // maximum exponent
				}
				else
				{
					q.exp--;
				}
				return pack_low(q);
			}
			else
			{
				return *this;
			}
		}
	}

	/**
	 * @brief Computes twice as Level 1 for es=0, Level 2 for es != 0
	 * 
	 * Level 1 for zero esponentbits
	 *
	 * +- 2^(R) 1.xyz  == +- 2^(exp) 1.xyz 
	 * 1.xyz * 2     => 1.0xyz ==> just R++ meaning shift R left up to overflow
	 *
	 * Level 2 otherwise
	 *
	 * +- 2^(R expmax + E) 1.xyz  == +- 2^(exp) 1.xyz 
	 * 1.xyz * 2     => 1.0xyz ==> just exp++ 
	 * exp++ ==> E++ if E not maximal
	 * otherwise R++ and exp zero
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @return Posit<T,totalbits,esbits,FT,positspec> 
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto Posit<T,totalbits,esbits,FT,positspec>::twice() const -> Posit<T,totalbits,esbits,FT,positspec>
	{
		if(esbits == 0)
		{
			return from_sraw(PT::fast_twice(v));
		}
		else
		{
			UnpackedPosit q = unpack_low();
			if(q.type == NumberType::Regular)
			{

				if(q.exp == PT::POSIT_REG_SCALE-1)
				{
					q.regime++; // will it overflo??
					q.exp = 0; // maximum exponent
				}
				else
				{
					q.exp++;
				}
				return pack_low(q);
			}
			else
			{
				return *this;
			}
		}
	}


	/**
	 * @brief Packs from Backend to Posit passing throught the Unpacked form
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @param x 
	 * @return CONSTEXPR14 pack_posit 
	 */
	#ifdef RISCV_PPU_FX
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 Posit<T,totalbits,esbits,FT,positspec> pack_posit(const typename Posit<T,totalbits,esbits,FT,positspec>::BackendT & x)
	{
		using BackendT = typename Posit<T,totalbits,esbits,FT,positspec>::BackendT;
		using PP=Posit<T,totalbits,esbits,FT,positspec>;
		constexpr int vtotalbits = totalbits,vesbits=esbits;
		PP p;
		PPU_INS(p.v = T(__fcvt_fx64_p16_1(x.v)),p.v = T(__fcvt_fx32_p16_0(x.v)),p.v = T(__fcvt_fx16_p8(x.v)),return PP::pack_low(PP::unpacked_full2low(x)););
		return p;
	}	
	#else
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 Posit<T,totalbits,esbits,FT,positspec> pack_posit(const typename Posit<T,totalbits,esbits,FT,positspec>::BackendT & x)
	{
		using PP=Posit<T,totalbits,esbits,FT,positspec>;
		return PP::pack_low(PP::unpacked_full2low(x));
	}
	#endif
	/**
	 * @brief Unpack from Backend to Posit passing throught the Unpacked form
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @param x 
	 * @return CONSTEXPR14 pack_posit 
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto unpack_posit(const Posit<T,totalbits,esbits,FT,positspec> & p) -> typename Posit<T,totalbits,esbits,FT,positspec>::BackendT 
	{
		using PP=Posit<T,totalbits,esbits,FT,positspec>;
		return PP::unpacked_low2full(p.unpack_low());
	}

	/**
	 * @brief Absolute value of Posit
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @param z 
	 * @return CONSTEXPR14 abs 
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	inline CONSTEXPR14 Posit<T,totalbits,esbits,FT,positspec> abs(const Posit<T,totalbits,esbits,FT,positspec> & z) 
	{
		return Posit<T,totalbits,esbits,FT,positspec>::from_sraw(pcabs(z.v));
	}
}


///@{ 
/// @name std member function overloads
namespace std
{

	/*template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec,class QuireT>
	inline CONSTEXPR14 QuireT fma(posit::Posit<T,totalbits,esbits,FT,positspec> x,
								  posit::Posit<T,totalbits,esbits,FT,positspec> y,
								  QuireT z) 
	{
		return z + x.template to_quire<QuireT>() + y.template to_quire<QuireT>();
	}*/

	template <class PType,class PAccumType>
	inline CONSTEXPR14 PAccumType fma(PType x,
								  PType y,
								  PAccumType z) 
	{
		return z + x.template to_posit<PAccumType>() * y.template to_posit<PAccumType>();
	}	


	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	inline CONSTEXPR14 posit::Posit<T,totalbits,esbits,FT,positspec> min(posit::Posit<T,totalbits,esbits,FT,positspec> a, posit::Posit<T,totalbits,esbits,FT,positspec> b)
	{
		return a <=  b ? a : b;
	}

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	inline CONSTEXPR14 posit::Posit<T,totalbits,esbits,FT,positspec> max(posit::Posit<T,totalbits,esbits,FT,positspec> a, posit::Posit<T,totalbits,esbits,FT,positspec> b)
	{
		return a >= b ? a : b;
	}

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	inline CONSTEXPR14 posit::Posit<T,totalbits,esbits,FT,positspec> abs(posit::Posit<T,totalbits,esbits,FT,positspec> a)
	{
		return posit::math::abs(a);
	}

	/*template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr bool isnan(posit::Posit<T,totalbits,esbits,FT,positspec> a)
	{
		return a.is_nan();
	}	*/

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> exp(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::exp((float)a);
	}

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> sqrt(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::sqrt((float)a);
	}	

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> log(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::log((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> log1p(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::log1p((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> log10(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::log1p((float)a);
	}	


	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> log2(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::log1p((float)a);
	}					

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> sin(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::sin((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> cos(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::cos((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> tan(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::tan((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> asin(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::asin((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> acos(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::acos((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> atan(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::atan((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> atan2(const posit::Posit<T,totalbits,esbits,FT,positspec>& a,
																  const posit::Posit<T,totalbits,esbits,FT,positspec>& b)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::atan2((float)a,(float)b);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> sinh(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::sinh((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> cosh(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::cosh((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> tanh(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::tanh((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> asinh(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::asinh((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> acosh(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::acosh((float)a);
	}		

	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> atanh(const posit::Posit<T,totalbits,esbits,FT,positspec>& a)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::atanh((float)a);
	}		



	template <class T,int totalbits, int esbits, class FT, posit::PositSpec positspec>
	constexpr posit::Posit<T,totalbits,esbits,FT,positspec> pow(const posit::Posit<T,totalbits,esbits,FT,positspec>& a,
																const posit::Posit<T,totalbits,esbits,FT,positspec>& b)
	{
		using PP = posit::Posit<T,totalbits,esbits,FT,positspec>;
		return (PP)std::pow((float)a,(float)b);
	}				




	/**
	 * @brief Specialization of std::numerical_limits for Posit
	 * 
	 * @tparam B as T of Posit
	 * @tparam totalbits as all bits of Posit
	 * @tparam esbits as exponent bits of Posit
	 * @tparam FT as backend type
	 * @tparam positspec as detailed specification
	 */
	template <class B,int totalbits, int esbits, class FT, posit::PositSpec positspec> class numeric_limits<posit::Posit<B,totalbits,esbits,FT,positspec> > {
	public:
	  using T=posit::Posit<B,totalbits,esbits,FT,positspec>;
	  using PT=typename T::PT;
	  static constexpr bool is_specialized = true;
	  static constexpr T min() noexcept { return T::min(); }
	  static constexpr T max() noexcept { return T::max(); }
	  static constexpr T lowest() noexcept { return T::lowest	(); }
	  static constexpr int  digits10 = ((totalbits-3)*30000)/100000;  // *log10(2)
	  static constexpr bool is_signed = true;
	  static constexpr bool is_integer = false;
	  static constexpr bool is_exact = false;
	  static constexpr int radix = 2;
	  static constexpr T epsilon() noexcept { return T::one().next()-T::one(); }
	
	  // this is also the maximum integer
	  static constexpr int  min_exponent = PT::minexponent();
	  static constexpr int  max_exponent = PT::maxexponent();

	  static constexpr bool has_infinity = true;
	  static constexpr bool has_quiet_NaN = true;
	  static constexpr bool has_signaling_NaN = false;
	  static constexpr bool has_denorm_loss = false;
	  static constexpr T infinity() noexcept { return T::infinity(); }
	  static constexpr T quiet_NaN() noexcept { return T::nan(); }
	  static constexpr T denorm_min() noexcept { return T::min(); }

	  static constexpr bool is_iec559 = false;
	  static constexpr bool is_bounded = false;
	  static constexpr bool is_modulo = false;

	  static constexpr bool traps = false;
	  static constexpr bool tinyness_before = false;
	};

	/* FOR SET-LIKE */
	template <class B,int totalbits, int esbits, class FT, posit::PositSpec positspec>	
	struct hash<posit::Posit<B,totalbits,esbits,FT,positspec>>
    {
        std::size_t operator()(const posit::Posit<B,totalbits,esbits,FT,positspec>& p) const noexcept
        {
            return p.v;
        }
    };

}
///@}

namespace posit {
	namespace math {
		/**
		 * @brief Fused multiply and accumulation using a QuireT type accumulator
		 */
	    template <class It1, class It2, class AccumT>
	    auto fma_dot(It1 begin1, It2 begin2, int N, AccumT& accum) -> typename std::remove_reference<decltype(*begin1)>::type
	    {
	    	using PP = typename std::remove_reference<decltype(*begin1)>::type;
	    	if(N < 1)
	    		return PP(0);
	    	else
	    	{
		    	accum = std::fma((*begin1++), (*begin2++), accum); // first
		    	for(int i = 1; i < N; i++)
			    	accum = std::fma((*begin1++), (*begin2++), accum);
		    	return PP(double(accum));
		    }
	    }		
	}
}


/// Standardized Posit configurations in the RISC-V RV64Xposit extension
using P8 = posit::Posit<int8_t, 8 , 0, uint_fast32_t, posit::PositSpec::WithNan>;
using P16_0 = posit::Posit<int16_t, 16 , 0, uint_fast32_t, posit::PositSpec::WithNan>;
using P16_1 = posit::Posit<int16_t, 16 , 1, uint_fast32_t, posit::PositSpec::WithNan>;
using P8fx = posit::Posit<int8_t, 8 , 0, posit::BackendFixed<fixedtrait<int16_t,16>,int16_t>, posit::PositSpec::WithNan>;
using P16_0fx = posit::Posit<int16_t, 16 , 0, posit::BackendFixed<fixedtrait<int32_t,32>,int32_t>, posit::PositSpec::WithNan>;
using P16_0fxq = posit::Posit<int16_t, 16 , 0, posit::BackendFixed<fixedtrait<int64_t,64>,int64_t>, posit::PositSpec::WithNan>; // 16,0 with doubled backend, for quire support
using P16_1fx = posit::Posit<int16_t, 16 , 1, posit::BackendFixed<fixedtrait<int64_t,64>,int32_t >, posit::PositSpec::WithNan>;


