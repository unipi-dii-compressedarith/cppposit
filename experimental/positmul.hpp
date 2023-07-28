
	#ifdef POSITMUL
		struct PositMul
		{
			constexpr PositMul(Posit av, Posit bv) : a(av),b(bv) {}
			Posit a,b;

			CONSTEXPR14 Posit asPosit() const { return pack_posit<T,totalbits,esbits,FT,positspec>(a.unpack()*b.unpack()); }

			CONSTEXPR14 operator Posit() const { return asPosit(); }

			CONSTEXPR14 operator UnpackedT() const { return asPosit(); }
		#ifndef FPGAHLS
			CONSTEXPR14 operator float() const { return asPosit(); }
			CONSTEXPR14 operator double() const { return asPosit(); }
		#endif
			CONSTEXPR14 operator int() const { return asPosit(); }

			// pa.a*pa.b+pb.a*pb.b => 
			friend CONSTEXPR14 Posit operator+(const PositMul & pa, const PositMul & pb) 
			{
				return pack_posit<T,totalbits,esbits,FT,positspec>(pa.a.unpack()*pa.b.unpack()+pb.a.unpack()*pb.b.unpack()); 
			}

			// pa.a*pa.b-pb.a*pb.b => 
			friend CONSTEXPR14 Posit operator-(const PositMul & pa, const PositMul & pb) 
			{
				return pack_posit<T,totalbits,esbits,FT,positspec>(pa.a.unpack()*pa.b.unpack()-pb.a.unpack()*pb.b.unpack()); 
			}

			// missing operators
			// &
			// -
		};
	#endif
	#ifdef POSITMUL
		/// wraps product to support automatic fusion
		friend CONSTEXPR14 PositMul operator*(const Posit & a, const Posit & b) 
		{
			return PositMul(a,b); 
		}

		/// addition against the fusion type
		friend constexpr Posit operator+(const Posit & a, const PositMul & b)
		{
			return fma(b.a,b.b,a);
		}

		/// addition against the fusion type
		friend constexpr Posit operator+(const PositMul & a, const Posit & b)
		{
			return fma(a.a,a.b,b);
		}

	#else