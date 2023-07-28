	    /**
	     * @brief Square rootc
	     * 
	     * @tparam T 
	     * @tparam totalbits 
	     * @tparam esbits 
	     * @tparam FT 
	     * @tparam positspec 
	     * @param z 
	     * @return CONSTEXPR14 c 
	     */
		template <class T,int totalbits, int esbits, class FT, PositSpec positspec>
		inline CONSTEXPR14 Posit<T,totalbits,esbits,FT,positspec> sqrt(Posit<T,totalbits,esbits,FT,positspec> z) 
		{
			using PP=Posit<T,totalbits,esbits,FT,positspec>;
			return PP::from_sraw(pcabs(z.v));
		}