/**
 * Emanuele Ruffaldi (C) 2017
 * Templated C++ Posit
 */
#include <typeinfo>
#include <fstream>
#include <iostream>
#include "posit.h"
#include "posit14.hpp"

#define SIGNEX(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

int16_t opinv[16384],opsquare[16384],opexp2[16384];
uint32_t op2float[16384];
int16_t opadd[16384*16384],opmul[16384*16384],opdiv[16384*16384];

int main(int argc, char const *argv[])
{
	// match the posit16 from posit16.hpp
	using X=Posit<typename posit14::PT::POSIT_STYPE,14,posit14::PT::POSIT_ESP_SIZE,typename posit14::FT,posit14::PT::positspec>;
	union Q {
		float f;
		uint32_t i;
	} ;
	for(int16_t s = -8192 ; s < 8192; s++)
	{
		X x;
		x.v = s;
		int32_t i = ((unsigned int)(uint16_t)s) & 0x0FFF;
        float fx = (float)x;
        //std::cout << s << " " << i << " " << fx << std::endl;
        Q fxq;
        fxq.f = fx;
        opinv[i] = x.inv().v;
        opsquare[i] = X(fx*fx).v;
        op2float[i] = fxq.i; 
        opexp2[i] = X(exp2(fx)).v;
        std::cout << "twice " << (X)(fx*2) << " half " << (X)(fx/2) << std::endl;
        
		for(int j = 0; j < 16384; j++)
		{
	        X y(SIGNEX(j,X::vtotalbits-1));
	        float fy = (float)y;
	        opadd[i*16384+j] = X(fx+fy).v;
	        opmul[i*16384+j] = X(fx*fy).v;
	        opdiv[i*16384+j] = X(fx/fy).v;
		}
		
	}

	std::ofstream onf(argc == 1 ? "posit14_tbl.cpp": argv[1]);
	onf << "#include <stdint.h>\nnamespace posit14ns {\n";
	onf << "// type is " << typeid(X).name() << std::endl;
	onf << "int16_t opinv[] = {\n";
	for(int i  = 0; i < 16384; i++)
		onf << (int)(opinv[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int16_t opsquare[] = {\n";
	for(int i  = 0; i < 16384; i++)
		onf << (int)(opsquare[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int16_t opexp2[] = {\n";
	for(int i  = 0; i < 16384; i++)
		onf << (int)(opexp2[i]) << ",";
	onf << "}; " << std::endl;
	onf << "uint32_t op2float[] = {\n";
	for(int i  = 0; i < 16384; i++)
		onf << (op2float[i]) << ",";
	onf << "};} " << std::endl;
	onf << "int16_t opmul[] = {\n";
	for(int i  = 0; i < 16384*16384; i++)
		onf << (int)(opmul[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int16_t opadd[] = {\n";
	for(int i  = 0; i < 16384*16384; i++)
		onf << (int)(opadd[i]) << ",";
	onf << "}; " << std::endl;
	onf << "int16_t opdiv[] = {\n";
	for(int i  = 0; i < 16384*16384; i++)
		onf << (int)(opdiv[i]) << ",";
	onf << "}; " << std::endl;
	return  0;
}
