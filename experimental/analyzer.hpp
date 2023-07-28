		/// diagnostics with full details
		struct info
		{
			bool infinity = false;
			bool nan = false;
			int k = 0; // regime exponent
			bool sign = false; // true if negative
			double regime = 0;
			int rs = 0;
			int es = 0;
			int fs = 0;
			int exp = 0;
			int exponent = 0;
			FT ifraction = 0;
			double fraction = 0;
			double value = 0;
		};

		info analyze();

	#if 0
	/// Level 1: -exponent of unpacked
	/// Level 0: flip bits of rs
	template <class T, int totalbits, int esbits, class FT, PositSpec positspec>
	CONSTEXPR14 auto Posit<T,totalbits,esbits,FT,positspec>::reciprocate() const -> Posit
	{
		auto u = unpack_low();
		if(u.fraction == 0)
		{
			// 2^(R scale + E) with E unsigned
			// becomes -R scale + (scale-E)
			if(u.exp == 0)
				u.regime = -u.regime;
			else
			{
				u.regime = -(u.regime+1);
				u.exp = PT::POSIT_REG_SCALE-u.exp; // NOP if esbits == 0
			}
			return pack_low(u);
		}
		else
		{
			return pack_posit< T,totalbits,esbits,FT,positspec> (unpacked_low2full(u).reciprocate());
		}
	}
	#endif

			/**
	 * @brief Returns a data structure decomposing the posit
	 * 
	 * @tparam T 
	 * @tparam totalbits 
	 * @tparam esbits 
	 * @tparam FT 
	 * @tparam positspec 
	 * @return info 
	 */
	template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
	auto Posit<T,totalbits,esbits,FT,positspec>::analyze() -> info
	{
		using UT=BackendT;
	    using POSIT_UTYPE = typename PT::POSIT_UTYPE;
	    //using POSIT_STYPE = typename PT::POSIT_STYPE;

	    
	    	auto pa = v;
		info i;	
		if(is_infinity())
	    {
	    	if(positspec == PositSpec::WithNanInfs)
				i.sign = (pa & PT::POSIT_SIGNBIT) != 0;
	    	i.infinity = true;
	    	return i;
	    }
	    else if(is_nan())
	    {
	    	i.nan = true;
	    	return i;
	    }
	    else if(v == 0)
	    {
	    	return i;
	    }
	    else
	    {
	        //constexpr int POSIT_RS_MAX = PT::POSIT_SIZE-1-esbits;

			i.sign = (pa & PT::POSIT_SIGNBIT) != 0;
	        pa = pcabs(pa);
	        POSIT_UTYPE pars = pa << (PT::POSIT_EXTRA_BITS+1); // output MSB: RS ES FS 
	        auto q = PT::decode_posit_rs(pars);	
	        int reg = q.first;
	        int rs = q.second;
	        pars <<= rs; // MSB: ES FS
	        POSIT_UTYPE exp = bitset_leftmost_getT(pars,esbits);
	        pars <<= esbits; // output MSB: FS left aligned in T
	        //std::cout << std::bitset<PT::POSIT_HOLDER_SIZE>(pars) << std::endl;
	        i.ifraction = sizeof(FT) >= sizeof(T) ? pars << (UT::FT_bits-PT::POSIT_HOLDER_SIZE) : pars >> (PT::POSIT_HOLDER_SIZE-UT::FT_bits); // output: FS left aligned in FT (larger or equal to T)
	    	i.exponent = PT::join_reg_exp(reg,exp);
	    	i.exp = exp;
	    	i.rs = rs;
	    	i.k = reg;
	    	i.es = totalbits-rs-1 < esbits ? totalbits-rs-1 : esbits; // std::min((int)(totalbits-rs-1),(int)esbits); MSVC issue
	    	i.fs = totalbits-rs-i.es-1; 
	    	return i;
	    }

	}

