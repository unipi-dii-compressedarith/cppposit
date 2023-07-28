/**
 * Emanuele Ruffaldi (C) 2017
 * Templated C++ Posit
 */
#include <typeinfo>
#include <fstream>
#include <iostream>
#include "posit.h"
#include "posit_logtab.hpp"

#define SIGNEX(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

#ifndef TARGETGENTYPE
#define TARGETGENTYPE posit_logtab<int8_t,8,2,uint16_t>
#error "Expected TARGETGENTYPE as full type"
#else
using basetype = TARGETGENTYPE::value_t;
using trait = TARGETGENTYPE::PT;
using basetypeU = std::make_unsigned<basetype>::type;
using helper = TARGETGENTYPE::posit_logtabns;
#define MINVALUE (-TARGETGENTYPE::tabsize/2)
#define MAXVALUE (TARGETGENTYPE::tabsize/2)
#endif
#define xstr(s) str(s)
#define str(s) #s

template <> basetype helper::opinv[TARGETGENTYPE::tabsize/2];
template <> basetype helper::opsquare[TARGETGENTYPE::tabsize/2];
template <> basetype helper::opexp2[TARGETGENTYPE::tabsize];
template <> basetype helper::oplog2[TARGETGENTYPE::tabsize/2];
template <> basetype helper::ophalf[TARGETGENTYPE::tabsize/2];
template<> uint32_t helper::op2float[TARGETGENTYPE::tabsize];
template <> basetype helper::opadd[TARGETGENTYPE::tabsize*TARGETGENTYPE::tabsize]; // TODO: optimize
static const char * classname = str(TARGETGENTYPE);

int main(int argc, char const *argv[])
{
	// match the posit16 from posit16.hpp
	using X=TARGETGENTYPE;
	union Q {
		float f;
		uint32_t i;
	} ;

	// TODO iterate in larger space
	for(basetype s = MINVALUE ; s <= MAXVALUE-1; s++)
	{
		X x;
		x.v = s;
		int32_t i = ((unsigned int)basetypeU(s)) & TARGETGENTYPE::tabmask;
		float fx(x);
		Q fxq;
        fxq.f = fx;
        helper::op2float[i] = fxq.i; 
        helper::opexp2[i] = X(exp2(fx)).v;
        if(s >= 0)	
        {
			helper::ophalf[i] = X(fx/2).v;
			helper::opinv[i] = s == 0 ? trait::POSIT_PINF : (s == trait::POSIT_PINF) ? 0 : x.inv().v;
			helper::opsquare[i] = (s == trait::POSIT_PINF) ? s : X(fx*fx).v;
			helper::oplog2[i] = s == trait::POSIT_PINF ? s : s == 0 ? trait::POSIT_NINF : X(log2(fx)).v;
 	    }
    }

	for(basetype s1 = MINVALUE ; s1 < MAXVALUE; s1++)
	{
		X x1;
		x1.v = s1;
		int32_t i1 = ((unsigned int)basetypeU(s1)) & TARGETGENTYPE::tabmask;
		float f1 = uint32_to_float(helper::op2float[i1]); 

		for(basetype s2 = MINVALUE ; s2 < MAXVALUE; s2++)
		{
			X x2;
			x2.v = s2;
			int32_t i2 = ((unsigned int)basetypeU(s2)) & TARGETGENTYPE::tabmask;
			float f2 = uint32_to_float(helper::op2float[i2]); 

	        helper::opadd[i1*TARGETGENTYPE::tabsize+i2] = X(f1+f2).v;
	        #if HASMUL
	        //opmul[i1*TARGETGENTYPE::tabsize+i2] = X(f1*f2).v;
	        //opdiv[i1*TARGETGENTYPE::tabsize+i2] = X(f1/f2).v;
	        #endif
		}
		
	}

#if 0
#include <stdint.h>
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

posit_logtab_helper<Q>::opadd...
posit_logtab_helper<Q>::
#endif
	std::ofstream onf(argc == 1 ? "posit_logtab_tbl.cpp": argv[1]);

	std::string sbasetype = typeid(X).name();

	onf << "#include <stdint.h>\n {\n";
	onf << "// type is " << typeid(X).name() << " " << classname << std::endl;

	basetype * tables [] = {helper::opinv,helper::opexp2,helper::oplog2,helper::opsquare,helper::ophalf,helper::opadd};
	int tables_size [] = {TARGETGENTYPE::tabsize/2,TARGETGENTYPE::tabsize,TARGETGENTYPE::tabsize/2,TARGETGENTYPE::tabsize/2,TARGETGENTYPE::tabsize/2,TARGETGENTYPE::tabsize*TARGETGENTYPE::tabsize}; // "opmul" "opdiv"
	const char * tables_names [] = {"opinv","opexp2","oplog2","opsquare","ophalf","opadd"};

	// TODO add opmul and opdiv if not exponential

	for (int j = 0; j < sizeof(tables)/sizeof(tables[0]); j++)
	{
		onf << sbasetype << " " << tables_names[j] << "[] = {\n";
		for(int i  = 0; i < tables_size[j]; i++)
			onf << (int)(tables[j][i]) << ",";
		onf << "}; " << std::endl;		
	}

	// special one
	onf << sbasetype << "  op2float[] = {\n";
	for(int i  = 0; i < TARGETGENTYPE::tabsize; i++)
		onf << (helper::op2float[i]) << ",";
	onf << "};} " << std::endl;
	return  0;
}
