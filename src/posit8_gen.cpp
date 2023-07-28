/**
 * Emanuele Ruffaldi (C) 2017
 * Templated C++ Posit
 */
#include <typeinfo>
#include <fstream>
#include <iostream>
#include "posit.h"
#include "tabulated/posit8.hpp"
using namespace posit;
#define SIGNEX(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

int main(int argc, char const *argv[])
{
	// match the posit8 from posit8.hpp
	using X=posit8::FPT; // Posit<typename posit8::PT::POSIT_STYPE,8,posit8::PT::POSIT_ESP_SIZE,typename posit8::FT,posit8::PT::positspec>;
	union Q {
		float f;
		uint32_t i;
	} ;
	int8_t opadd[256*256],opmul[256*256],opdiv[256*256],opinv[256],opsquare[256],opexp2[256],ophalf[256],optanh[256];
	uint32_t op2float[256];
	int32_t op2int[256];

	for(int16_t s = -128 ; s < 128; s++)
	{
		X x;
		x.v = s;
		int32_t i = ((unsigned int)(uint16_t)s) & 0xFF;
		float fx(x);
		Q fxq;
        fxq.f = fx;

        opinv[i] = x.reciprocate().v;
        opsquare[i] = X(fx*fx).v;
        op2float[i] = fxq.i; 
        op2int[i] = (int)x; 
        ophalf[i] = x.half().v; // ISSUE: x.half() != x/X::two() !!!!!!
        opexp2[i] = X(exp2(fx)).v;
        optanh[i] = X(std::tanh(fx)).v;
        //std::cout << "twice " << (X)(fx*2) << " half " << (X)(fx/2) << std::endl;
    }

	for(int16_t s1 = -128 ; s1 < 128; s1++)
	{
		X x1;// same as DeepInit
		x1.v = s1;
		int32_t i1 = ((unsigned int)(uint8_t)s1) & 0xFF;
		//float f1 = uint32_to_float(op2float[i1]); 

		for(int16_t s2 = -128 ; s2 < 128; s2++)
		{
			X x2; // same as DeepInit
			x2.v = s2;
			int32_t i2 = ((unsigned int)(uint8_t)s2) & 0xFF;
			//float f2 = uint32_to_float(op2float[i2]); 

	        opadd[i1*256+i2] = (x1+x2).v;
	        opmul[i1*256+i2] = (x1*x2).v;
	        opdiv[i1*256+i2] = (x1/x2).v;
	        //opadd[i*256+j] = (x*y).v;
		}
	}

	std::ofstream onf(argc == 1 ? "posit8_tbl.cpp" : argv[1]);
	onf << "#include <stdint.h>\nnamespace posit8ns {\n";
	onf << "// type is " << typeid(X).name() << std::endl;
	onf << "int8_t opadd[] = {\n";
	for(int i  = 0; i < 256*256; i++)
		onf << (int)(opadd[i]) << ",";
	onf << "}; " << std::endl;
	// emit the numbers as C file
	onf << "int8_t opmul[] = {\n";
	for(int i  = 0; i < 256*256; i++)
		onf << (int)(opmul[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int8_t opdiv[] = {\n";
	for(int i  = 0; i < 256*256; i++)
		onf << (int)(opdiv[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int8_t opinv[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (int)(opinv[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int8_t opsquare[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (int)(opsquare[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int8_t opexp2[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (int)(opexp2[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int8_t optanh[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (int)(optanh[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int8_t ophalf[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (int)(ophalf[i]) << ",";
	onf << "}; " << std::endl;

	onf << "uint32_t op2float[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (op2float[i]) << ",";
	onf << "}; " << std::endl;


	onf << "int32_t op2int[] = {\n";
	for(int i  = 0; i < 256; i++)
		onf << (op2int[i]) << ",";
	onf << "}; " << std::endl;
	
	onf << "} " << std::endl;
	return  0;
}
