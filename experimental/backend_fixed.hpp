	

	template <class B>
	struct UnpackedFixed
	{
		using bt = B;
		struct single_tag{};

		UnpackedF() {}
		UnpackedF(single_tag, uint32_t x): v(uint32_to_float(x)) {}
		UnpackedF(int x) : v(x) {}
		UnpackedF(float x) : v(x) {}
		UnpackedF(double x) : v(x) {}

		constexpr operator float () const {return v;}
		constexpr operator double () const {return v;}

	 	UnpackedF operator + (UnpackedF o) const { return UnpackedF(v+o.v); }
		UnpackedF operator * (UnpackedF o) const { return UnpackedF(v*o.v); }
	 	UnpackedF operator - (UnpackedF o) const { return UnpackedF(v-o.v); }
	 	UnpackedF operator /() (UnpackedF o) const { return UnpackedF(v/o.v); }
		bt v;
	};

	template <class PT, class FT>
	struct PositPacker<PT,UnpackedFixed<FT> >
	{
		using BE = UnpackedFixed<FT>;
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