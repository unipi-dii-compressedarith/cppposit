/**
 * Copyright (C) 2017-2023 Emanuele Ruffaldi, Federico Rossi
 * 
 * Distributed under the terms of the BSD 3-Clause License.  
 * 
 * (See accompanying file LICENSE)
 * 
 * --
 */



namespace posit {

    template <class T>
	struct is_posit_backend;

	template <class T, class PositEmu, class PositTable>
	struct TabulatedBackend: public HwBaseBackend
	{
		struct single_tag{};
        constexpr static T indexMask = (1<<PositEmu::vtotalbits)-1;
		TabulatedBackend() {}
		TabulatedBackend(single_tag, T x): v(x) {}

		explicit TabulatedBackend(int x) {v=PositEmu(x).v;}
		explicit TabulatedBackend(long x) {v=PositEmu(x).v;}
		explicit TabulatedBackend(float x) {v=PositEmu(x).v;}
		explicit TabulatedBackend(double x) {v=PositEmu(x).v;}

		constexpr operator float () const {return (float)PositEmu::from_sraw(v);}
		constexpr operator double () const {return (double)PositEmu::from_sraw(v);}
		constexpr operator int () const {return (int)PositEmu::from_sraw(v);}
		constexpr operator long () const {return (long)PositEmu::from_sraw(v);}

	 	TabulatedBackend operator + (TabulatedBackend o) const { 
            return TabulatedBackend{{},PositTable::add[v & indexMask][o.v & indexMask]};
        } 
		TabulatedBackend operator * (TabulatedBackend o) const { 
            return TabulatedBackend{{},PositTable::mul[v & indexMask][o.v & indexMask]};
        } 
	 	TabulatedBackend operator / (TabulatedBackend o) const { 
            return TabulatedBackend{{},PositTable::div[v & indexMask][o.v & indexMask]};
        } 
		T v;

	};

	template <class T, class PositEmu, class PositTable>
	struct is_posit_backend<TabulatedBackend<T,PositEmu,PositTable> >: public std::true_type
	{
	};
    
}