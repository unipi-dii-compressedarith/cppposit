#pragma once
#ifdef _GEMMX_
#include <boost/preprocessor/repetition/repeat.hpp>
#endif
#ifdef _GEMMX_NEXT_
#include <preprocessor/repetition/repeat.hpp>
#endif
#define rvcntw() __builtin_epi_vsetvlmax(__epi_e32,__epi_m1)
#define rvcnth() __builtin_epi_vsetvlmax(__epi_e16,__epi_m1)
#define rvcnts() __builtin_epi_vsetvlmax(__epi_e8,__epi_m1)

void __epi_print(__epi_4xi32 v,int n,bool hex=false) {
	int* tmp = new int[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e32,__epi_m2);
		__builtin_epi_vstore_4xi32(&tmp[i],v,gvl);
	}
	for(int i = 0; i < n; ++i) {
		std::cout << std::dec;
		if(hex) std::cout << std::hex;
		std::cout << tmp[i] << " ";
	}
	std::cout << std::endl;
	delete[] tmp;

}

void __epi_print(__epi_2xf32 v,int n) {
	float* tmp = new float[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e32,__epi_m1);
		__builtin_epi_vstore_2xf32(&tmp[i],v,gvl);
	}
	for(int i = 0; i < n; ++i)
		std::cout << tmp[i] << " ";
	std::cout << std::endl;
	delete[] tmp;

}

void __epi_print(__epi_4xf32 v,int n) {
	float* tmp = new float[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e32,__epi_m2);
		__builtin_epi_vstore_4xf32(&tmp[i],v,gvl);
	}
	for(int i = 0; i < n; ++i)
		std::cout << tmp[i] << " ";
	std::cout << std::endl;
	delete[] tmp;

}

void __epi_print(__epi_2xi32 v,int n) {
	int* tmp = new int[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e32,__epi_m1);
		__builtin_epi_vstore_2xi32(&tmp[i],v,gvl);
	}
	for(int i = 0; i < n; ++i)
		std::cout << std::hex << tmp[i] << " ";
	std::cout << std::endl;
	delete[] tmp;
}

void __epi_print(__epi_4xi16 v,int n,bool hex = false) {
	short* tmp = new short[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e16,__epi_m1);
		__builtin_epi_vstore_4xi16(&tmp[i],v,gvl);
	}
	for(int i = 0; i < n; ++i) {
		std::cout << std::dec;
		if(hex) std::cout << std::hex;
		std::cout << tmp[i] << " ";

	}
	std::cout << std::endl;
	delete[] tmp;
}


void __epi_print(__epi_8xi8 v,int n) {
	signed char* tmp = new signed char[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e8,__epi_m1);
		__builtin_epi_vstore_8xi8(&tmp[i],v,gvl);
	}
	for(int i = 0; i < n; ++i)
		std::cout << std::hex << (int)(tmp[i]&0xff) << " ";
	std::cout << std::endl;
	delete[] tmp;
}

void __epi_print(__epi_4xi1 v,int n) {
	unsigned short int* tmp = new unsigned short int[n];
	long unsigned int gvl = 0;
	for(int i = 0; i < n; i+=gvl) {
		gvl = __builtin_epi_vsetvl(n,__epi_e8,__epi_m1);
		__builtin_epi_vstore_4xi1(&tmp[i],v);
	}
	for(int i = 0; i < n; ++i)
		std::cout << std::hex << (int)(tmp[i]&0xff) << " ";
	std::cout << std::endl;
	delete[] tmp;
}
#define vbroadcast16(VAL,GVL) __builtin_epi_vmv_v_x_4xi16(VAL,GVL)
#define vbroadcast16l(VAL,GVL) __builtin_epi_vmv_v_x_8xi16(VAL,GVL)

#define vbroadcast8(VAL,GVL) __builtin_epi_vmv_v_x_8xi8(VAL,GVL)

#define vextend8_16(VAL,GVL) __builtin_epi_vwadd_8xi16(VAL,vbroadcast8(0,GVL),GVL)
#define vextend16_32(VAL,GVL) __builtin_epi_vwadd_4xi32(VAL,vbroadcast16(0,GVL),GVL)
#define vextend16_32l(VAL,GVL) __builtin_epi_vwadd_8xi32(VAL,vbroadcast16l(0,GVL),GVL)

#define vextend8_32(VAL,GVL) vextend16_32l(vextend8_16(VAL,GVL),GVL)


#define vreduce32_16(VAL,GVL) __builtin_epi_vnsrl_4xi16(VAL,vbroadcast16(0,GVL),GVL)
#define vreduce16_8(VAL,GVL) __builtin_epi_vnsrl_8xi8(VAL,vbroadcast8(0,GVL),GVL)
#define vreduce32_16l(VAL,GVL) __builtin_epi_vnsrl_8xi16(VAL,vbroadcast16l(0,GVL),GVL)
#define vreduce32_8(VAL,GVL) vreduce16_8(vreduce32_16l(VAL,GVL),GVL)

#define vbroadcast4xi32(VAL,GVL) __builtin_epi_vmv_v_x_4xi32(VAL,GVL)
#define vbroadcast8xi32(VAL,GVL) __builtin_epi_vmv_v_x_8xi32(VAL,GVL)

#define vbroadcast32(VAL,GVL) __builtin_epi_vmv_v_x_4xi32(VAL,GVL)

#define __builtin_epi_vnot_4xi16_mask(MERGE,A,MASK,GVL,OUT) \
	__epi_4xi16 OUT = __builtin_epi_vxor_4xi16_mask(MERGE,A,__builtin_epi_vmv_v_x_4xi16(0xFFFF,GVL),MASK,GVL); \

#define __builtin_epi_vnot_8xi8_mask(MERGE,A,MASK,GVL,OUT) \
	__epi_8xi8 OUT = __builtin_epi_vxor_8xi8_mask(MERGE,A,__builtin_epi_vmv_v_x_8xi8(0xFF,GVL),MASK,GVL); \

#define __builtin_epi_vnot_4xi32(A,GVL) __builtin_epi_vxor_4xi32(A,__builtin_epi_vmv_v_x_4xi32(0xFFFFFFFF,GVL),GVL)
#define __builtin_epi_vnot_8xi32(A,GVL) __builtin_epi_vxor_8xi32(A,__builtin_epi_vmv_v_x_8xi32(0xFFFFFFFF,GVL),GVL)

#define __builtin_epi_vnot_4xi32_mask(MERGE,A,MASK,GVL,OUT) \
	__epi_4xi32 OUT = __builtin_epi_vxor_4xi32_mask(MERGE,A,__builtin_epi_vmv_v_x_4xi32(0xFFFFFFFF,GVL),MASK,GVL); \

#define __builtin_epi_vnot_8xi32_mask(MERGE,A,MASK,GVL,OUT) \
	__epi_8xi32 OUT = __builtin_epi_vxor_8xi32_mask(MERGE,A,__builtin_epi_vmv_v_x_8xi32(0xFFFFFFFF,GVL),MASK,GVL); \


__epi_4xi16 __builtin_epi_clz_4xi16(__epi_4xi16 X,unsigned long int GVL) {
    __epi_4xi16 clzFF_helper = vbroadcast16(0xFF,GVL); 
    __epi_4xi16 clzF_helper = vbroadcast16(0xF,GVL); 
    __epi_4xi16 clz3_helper = vbroadcast16(0x3,GVL); 

    __epi_4xi16 R = __builtin_epi_vsll_4xi16(__builtin_epi_cast_4xi16_4xi1(__builtin_epi_vmsgtu_4xi16(X,clzFF_helper,GVL)),vbroadcast16(3,GVL),GVL); 
    X = __builtin_epi_vsrl_4xi16(X,R,GVL); 
    __epi_4xi16 Q = __builtin_epi_vsll_4xi16(__builtin_epi_cast_4xi16_4xi1(__builtin_epi_vmsgtu_4xi16(X,clzF_helper,GVL)),vbroadcast16(2,GVL),GVL);  
    X = __builtin_epi_vsrl_4xi16(X,Q,GVL); 
    R = __builtin_epi_vor_4xi16(R,Q,GVL); 
    Q = __builtin_epi_vsll_4xi16(__builtin_epi_cast_4xi16_4xi1(__builtin_epi_vmsgtu_4xi16(X,clz3_helper,GVL)),vbroadcast16(1,GVL),GVL);  
    X = __builtin_epi_vsrl_4xi16(X,Q,GVL); 
    R = __builtin_epi_vor_4xi16(R,Q,GVL); 
    R = __builtin_epi_vor_4xi16(R,__builtin_epi_vsrl_4xi16(X,vbroadcast16(1,GVL),GVL),GVL); 
	return __builtin_epi_vsub_4xi16(vbroadcast16(15,GVL),R,GVL);
}

__epi_8xi8 __builtin_epi_clz_8xi8(__epi_8xi8 X,unsigned long int GVL) {
    __epi_8xi8 clzF_helper = vbroadcast8(0xF,GVL); 
    __epi_8xi8 clz3_helper = vbroadcast8(0x3,GVL); 

    __epi_8xi8 R = __builtin_epi_vsll_8xi8(__builtin_epi_cast_8xi8_8xi1(__builtin_epi_vmsgtu_8xi8(X,clzF_helper,GVL)),vbroadcast8(2,GVL),GVL); 
    X = __builtin_epi_vsrl_8xi8(X,R,GVL); 
    __epi_8xi8 Q = __builtin_epi_vsll_8xi8(__builtin_epi_cast_8xi8_8xi1(__builtin_epi_vmsgtu_8xi8(X,clz3_helper,GVL)),vbroadcast8(1,GVL),GVL);  
    X = __builtin_epi_vsrl_8xi8(X,Q,GVL); 
    R = __builtin_epi_vor_8xi8(R,Q,GVL); 
    R = __builtin_epi_vor_8xi8(R,__builtin_epi_vsrl_8xi8(X,vbroadcast8(1,GVL),GVL),GVL); 
	return __builtin_epi_vsub_8xi8(vbroadcast8(7,GVL),R,GVL);
}

__epi_4xi32 __builtin_epi_4xi32_abs(__epi_4xi32 x,unsigned int sign_mask,unsigned long int gvl) {
	__epi_4xi1 f_sign = __builtin_epi_vmsne_4xi32(__builtin_epi_vand_4xi32(x,vbroadcast4xi32(sign_mask,gvl),gvl),vbroadcast4xi32(0,gvl),gvl);
	__builtin_epi_vnot_4xi32_mask(x,x,f_sign,gvl,x_n); 
	return __builtin_epi_vadd_4xi32_mask(x,x,vbroadcast32(1,gvl),f_sign,gvl);
}

__epi_8xi32 __builtin_epi_8xi32_abs(__epi_8xi32 x,unsigned int sign_mask,unsigned long int gvl) {
	__epi_8xi1 f_sign = __builtin_epi_vmsne_8xi32(__builtin_epi_vand_8xi32(x,vbroadcast8xi32(sign_mask,gvl),gvl),vbroadcast8xi32(0,gvl),gvl);
	__builtin_epi_vnot_8xi32_mask(x,x,f_sign,gvl,x_n); 
	return __builtin_epi_vadd_8xi32_mask(x,x,vbroadcast8xi32(1,gvl),f_sign,gvl);
}

__epi_4xi32 __builtin_epi_4xi32_abs(__epi_4xi32 x,__epi_4xi1 f_sign,unsigned long int gvl) {
	__builtin_epi_vnot_4xi32_mask(x,x,f_sign,gvl,x_n); 
	return __builtin_epi_vadd_4xi32_mask(x,x,vbroadcast32(1,gvl),f_sign,gvl);
}

__epi_8xi32 __builtin_epi_8xi32_abs(__epi_8xi32 x,__epi_8xi1 f_sign,unsigned long int gvl) {
	__builtin_epi_vnot_8xi32_mask(x,x,f_sign,gvl,x_n); 
	return __builtin_epi_vadd_8xi32_mask(x,x,vbroadcast8xi32(1,gvl),f_sign,gvl);
}

__epi_4xi32 __builtin_epi_4xi32_adj_k(__epi_4xi32 x,__epi_4xi1 f_sign,unsigned long int gvl) {
	__builtin_epi_vadd_4xi32_mask(x,x,vbroadcast32(1,gvl),f_sign,gvl);
	__builtin_epi_vnot_4xi32_mask(x,x,f_sign,gvl,x_n); 
	return __builtin_epi_vadd_4xi32_mask(x,x,vbroadcast32(1,gvl),f_sign,gvl);
}

__epi_8xi32 __builtin_epi_8xi32_adj_k(__epi_8xi32 x,__epi_8xi1 f_sign,unsigned long int gvl) {
	__builtin_epi_vadd_8xi32_mask(x,x,vbroadcast8xi32(1,gvl),f_sign,gvl);
	__builtin_epi_vnot_8xi32_mask(x,x,f_sign,gvl,x_n); 
	return __builtin_epi_vadd_8xi32_mask(x,x,vbroadcast8xi32(1,gvl),f_sign,gvl);
}

__epi_4xi16 float_to_posit16(__epi_4xf32 opv,unsigned long gvl) {
	__epi_4xi32 res_;
	__epi_4xi32 f_int = (__epi_4xi32)opv;
	//__epi_4xi32_print(f_int,gvl,true);	
	__epi_4xi1 f_sign = __builtin_epi_vmsne_4xi32(__builtin_epi_vand_4xi32(f_int,vbroadcast4xi32(0x80000000,gvl),gvl),vbroadcast4xi32(0,gvl),gvl);
	__epi_4xi32 f_exp = __builtin_epi_vsrl_4xi32(__builtin_epi_vsll_4xi32(f_int,vbroadcast4xi32(1,gvl),gvl),vbroadcast4xi32(24,gvl),gvl);
	__epi_4xi32 f_exp_norm = __builtin_epi_vsub_4xi32(f_exp,vbroadcast4xi32(127,gvl),gvl); // also k-value
	__epi_4xi32 f_frac = __builtin_epi_vsrl_4xi32(f_int,vbroadcast4xi32(10,gvl),gvl);
	f_frac = __builtin_epi_vand_4xi32(f_frac,vbroadcast4xi32(0x1FFF,gvl),gvl);

	// Exponent absolute value (LOW 8 bit -> 0xFFFFFF80)
	__epi_4xi1 f_exp_norm_sign = __builtin_epi_vmsne_4xi32(__builtin_epi_vand_4xi32(f_exp_norm,vbroadcast4xi32(0xFFFFFF80,gvl),gvl),vbroadcast4xi32(0,gvl),gvl);
	__epi_4xi32 k_value = __builtin_epi_4xi32_abs(f_exp_norm,0xFFFFFF80,gvl);
	//__epi_4xi32_print(k_value,gvl,true);	

	// Adjust k-value x:-x+1
	__epi_4xi32 adj_k_value =__builtin_epi_4xi32_adj_k(f_exp_norm,f_exp_norm_sign,gvl);
	//__epi_4xi32_print(adj_k_value,gvl,true);

	// Low 4 bits of k_value (AND with ____1111)
	__epi_4xi32 k_value_low = __builtin_epi_vand_4xi32(k_value,vbroadcast4xi32(0xF,gvl),gvl);

	// Build regime with arithmetic right shift on signed integer
	//Prepare the content wit [31-15]=1 [15-0]=0   1111 1111 1111 1111 1000 0000 0000 0000
	__epi_4xi32 regime_hold = vbroadcast4xi32(0xFFFF8000,gvl);
	regime_hold = __builtin_epi_vsra_4xi32(regime_hold,k_value_low,gvl);
	__epi_4xi32 regime_pos = __builtin_epi_vsra_4xi32(regime_hold,vbroadcast4xi32(1,gvl),gvl);
	__epi_4xi32 regime = __builtin_epi_vand_4xi32_mask(regime_pos,regime_pos,__builtin_epi_vnot_4xi32(regime_hold,gvl),f_exp_norm_sign,gvl);
	regime = __builtin_epi_vand_4xi32(regime,vbroadcast4xi32(0x00007FFF,gvl),gvl);
	//__epi_4xi32_print(regime,gvl,true);

	// Compute regime length from least 4 significant bits of adjusted k-value x:-x+1
	__epi_4xi32 regime_length = __builtin_epi_vand_4xi32(adj_k_value,vbroadcast4xi32(0xF,gvl),gvl);

	// Use regime length to shift the mantissa right 
	f_frac = __builtin_epi_vsrl_4xi32(f_frac,regime_length,gvl);

	// Pack everything using OR
	res_ = __builtin_epi_vor_4xi32(regime,f_frac,gvl);
	//__epi_4xi32_print(res_,gvl,true);

	// Use sign mask to perform 2's complement of Posit
	res_ = __builtin_epi_4xi32_abs(res_,f_sign,gvl);
	//__epi_4xi32_print(res_,gvl,true);


	return vreduce32_16(res_,gvl);
}


__epi_4xf32 posit16_to_float(__epi_4xi16 opv,unsigned long gvl) {
		__epi_4xi32 res;
		unsigned short POSIT_HOLDER_MSB = (1 << 15), POSIT_SIGNBIT = (1 << 15),POSIT_EXTRA_BITS = 0;
        __epi_4xi16 sign_bit_helper =  __builtin_epi_vmv_v_x_4xi16((unsigned short)((POSIT_SIGNBIT)),gvl); 
        __epi_4xi16 zero_helper =  __builtin_epi_vmv_v_x_4xi16(0,gvl); 
        __epi_4xi1 sign_mask = __builtin_epi_vmsne_4xi16(__builtin_epi_vand_4xi16(opv,sign_bit_helper,gvl),zero_helper,gvl); 
        __epi_4xi16 n_one_helper =  __builtin_epi_vmv_v_x_4xi16(-1,gvl); 
        __epi_4xi16 one_helper =  __builtin_epi_vmv_v_x_4xi16(1,gvl); 

        __epi_4xi16 pa = __builtin_epi_vmul_4xi16_mask(opv,opv,n_one_helper,sign_mask,gvl); 
        __epi_4xi16 extra_bit_helper = __builtin_epi_vmv_v_x_4xi16(POSIT_EXTRA_BITS+1,gvl); 
        __epi_4xi16 pars1 = __builtin_epi_vsll_4xi16(pa,extra_bit_helper,gvl); 

        __epi_4xi16 holder_msb_helper = __builtin_epi_vmv_v_x_4xi16(POSIT_HOLDER_MSB,gvl); 
        __epi_4xi1 one_marker_mask = __builtin_epi_vmsne_4xi16(__builtin_epi_vand_4xi16(pars1,holder_msb_helper,gvl),zero_helper,gvl); 
        __epi_4xi1 neg_one_marker_mask = __builtin_epi_vmseq_4xi16(__builtin_epi_vand_4xi16(pars1,holder_msb_helper,gvl),zero_helper,gvl); 
        __builtin_epi_vnot_4xi16_mask(pars1,pars1,one_marker_mask,gvl,neg_with_mask); 

       	__epi_4xi16 index = __builtin_epi_clz_4xi16(neg_with_mask,gvl); 

        __epi_4xi16 kvalue = __builtin_epi_vadd_4xi16_mask(index,index,n_one_helper,one_marker_mask,gvl); 
        kvalue = __builtin_epi_vmul_4xi16_mask(kvalue,kvalue,n_one_helper,neg_one_marker_mask,gvl); 

        __epi_4xi16 rs = __builtin_epi_vadd_4xi16(index,vbroadcast16(1,gvl),gvl); 

        //__epi_4xi16 rsexp = __builtin_epi_vadd_4xi16(rs,vbroadcast16(0,gvl),gvl);

        __epi_4xi16 pars2 = __builtin_epi_vsll_4xi16(pars1,rs,gvl);

        //__epi_4xi16 fraction_nbits = __builtin_epi_vsub_4xi16(vbroadcast16(15,gvl),rsexp,gvl); 
        __epi_4xi16 fraction = pars2;

		// Adjust float sign in MSB
		__epi_4xi32 f_sign = __builtin_epi_vsll_4xi32(vextend16_32(__builtin_epi_cast_4xi16_4xi1(sign_mask),gvl),vbroadcast32(31,gvl),gvl);

		// Adjust float exponent in MSBs
		__epi_4xi32 f_exp = __builtin_epi_vadd_4xi32(vextend16_32(kvalue,gvl),vbroadcast4xi32(0x7f,gvl),gvl);
		f_exp = __builtin_epi_vsll_4xi32(__builtin_epi_vand_4xi32(f_exp,vbroadcast32(0xFF,gvl),gvl),vbroadcast32(23,gvl),gvl);

		// Adjust fraction in 23 LSB mantissa bits
		__epi_4xi32 f_frac = __builtin_epi_vsll_4xi32(vextend16_32(fraction,gvl),vbroadcast32(7,gvl),gvl);

		// Pack the float
		res = f_sign;
		res = __builtin_epi_vor_4xi32(res,f_exp,gvl);
		res = __builtin_epi_vor_4xi32(res,f_frac,gvl);
		return (__epi_4xf32)res;
}
        
__epi_8xf32 posit8_to_float(__epi_8xi8 opv,unsigned long gvl) {
		__epi_8xi32 res;
		unsigned short POSIT_HOLDER_MSB = (1 << 7), POSIT_SIGNBIT = (1 << 7),POSIT_EXTRA_BITS = 0;
        __epi_8xi8 sign_bit_helper =  __builtin_epi_vmv_v_x_8xi8((unsigned short)((POSIT_SIGNBIT)),gvl); 
        __epi_8xi8 zero_helper =  __builtin_epi_vmv_v_x_8xi8(0,gvl); 
        __epi_8xi1 sign_mask = __builtin_epi_vmsne_8xi8(__builtin_epi_vand_8xi8(opv,sign_bit_helper,gvl),zero_helper,gvl); 
        __epi_8xi8 n_one_helper =  __builtin_epi_vmv_v_x_8xi8(-1,gvl); 
        __epi_8xi8 one_helper =  __builtin_epi_vmv_v_x_8xi8(1,gvl); 

        __epi_8xi8 pa = __builtin_epi_vmul_8xi8_mask(opv,opv,n_one_helper,sign_mask,gvl); 
        __epi_8xi8 extra_bit_helper = __builtin_epi_vmv_v_x_8xi8(POSIT_EXTRA_BITS+1,gvl); 
        __epi_8xi8 pars1 = __builtin_epi_vsll_8xi8(pa,extra_bit_helper,gvl); 

        __epi_8xi8 holder_msb_helper = __builtin_epi_vmv_v_x_8xi8(POSIT_HOLDER_MSB,gvl); 
        __epi_8xi1 one_marker_mask = __builtin_epi_vmsne_8xi8(__builtin_epi_vand_8xi8(pars1,holder_msb_helper,gvl),zero_helper,gvl); 
        __epi_8xi1 neg_one_marker_mask = __builtin_epi_vmseq_8xi8(__builtin_epi_vand_8xi8(pars1,holder_msb_helper,gvl),zero_helper,gvl); 
        __builtin_epi_vnot_8xi8_mask(pars1,pars1,one_marker_mask,gvl,neg_with_mask); 

       	__epi_8xi8 index = __builtin_epi_clz_8xi8(neg_with_mask,gvl); 

        __epi_8xi8 kvalue = __builtin_epi_vadd_8xi8_mask(index,index,n_one_helper,one_marker_mask,gvl); 
        kvalue = __builtin_epi_vmul_8xi8_mask(kvalue,kvalue,n_one_helper,neg_one_marker_mask,gvl); 

        __epi_8xi8 rs = __builtin_epi_vadd_8xi8(index,vbroadcast8(1,gvl),gvl); 

        //__epi_8xi8 rsexp = __builtin_epi_vadd_8xi8(rs,vbroadcast16(0,gvl),gvl);

        __epi_8xi8 pars2 = __builtin_epi_vsll_8xi8(pars1,rs,gvl);

        //__epi_8xi8 fraction_nbits = __builtin_epi_vsub_4xi16(vbroadcast16(15,gvl),rsexp,gvl); 
        __epi_8xi8 fraction = pars2;

		// Adjust float sign in MSB
		__epi_8xi32 f_sign = __builtin_epi_vsll_8xi32(vextend8_32(__builtin_epi_cast_8xi8_8xi1(sign_mask),gvl),vbroadcast8xi32(31,gvl),gvl);

		// Adjust float exponent in MSBs
		__epi_8xi32 f_exp = __builtin_epi_vadd_8xi32(vextend8_32(kvalue,gvl),vbroadcast8xi32(0x7f,gvl),gvl);
		f_exp = __builtin_epi_vsll_8xi32(__builtin_epi_vand_8xi32(f_exp,vbroadcast8xi32(0xFF,gvl),gvl),vbroadcast8xi32(23,gvl),gvl);

		// Adjust fraction in 23 LSB mantissa bits
		__epi_8xi32 f_frac = __builtin_epi_vsll_8xi32(vextend8_32(fraction,gvl),vbroadcast8xi32(7,gvl),gvl);

		// Pack the float
		res = f_sign;
		res = __builtin_epi_vor_8xi32(res,f_exp,gvl);
		res = __builtin_epi_vor_8xi32(res,f_frac,gvl);
		return (__epi_8xf32)res;
}
    

__epi_8xi8 float_to_posit8(__epi_8xf32 opv,unsigned long gvl) {
	__epi_8xi32 res_;
	__epi_8xi32 f_int = (__epi_8xi32)opv;
	//__epi_8xi32_print(f_int,gvl,true);	
	__epi_8xi1 f_sign = __builtin_epi_vmsne_8xi32(__builtin_epi_vand_8xi32(f_int,vbroadcast8xi32(0x80000000,gvl),gvl),vbroadcast8xi32(0,gvl),gvl);
	__epi_8xi32 f_exp = __builtin_epi_vsrl_8xi32(__builtin_epi_vsll_8xi32(f_int,vbroadcast8xi32(1,gvl),gvl),vbroadcast8xi32(24,gvl),gvl);
	__epi_8xi32 f_exp_norm = __builtin_epi_vsub_8xi32(f_exp,vbroadcast8xi32(127,gvl),gvl); // also k-value
	__epi_8xi32 f_frac = __builtin_epi_vsrl_8xi32(f_int,vbroadcast8xi32(10,gvl),gvl);
	f_frac = __builtin_epi_vand_8xi32(f_frac,vbroadcast8xi32(0x1FFF,gvl),gvl);

	// Exponent absolute value (LOW 8 bit -> 0xFFFFFF80)
	__epi_8xi1 f_exp_norm_sign = __builtin_epi_vmsne_8xi32(__builtin_epi_vand_8xi32(f_exp_norm,vbroadcast8xi32(0xFFFFFF80,gvl),gvl),vbroadcast8xi32(0,gvl),gvl);
	__epi_8xi32 k_value = __builtin_epi_8xi32_abs(f_exp_norm,0xFFFFFF80,gvl);
	//__epi_8xi32_print(k_value,gvl,true);	

	// Adjust k-value x:-x+1
	__epi_8xi32 adj_k_value =__builtin_epi_8xi32_adj_k(f_exp_norm,f_exp_norm_sign,gvl);
	//__epi_8xi32_print(adj_k_value,gvl,true);

	// Low 4 bits of k_value (AND with ____1111)
	__epi_8xi32 k_value_low = __builtin_epi_vand_8xi32(k_value,vbroadcast8xi32(0xF,gvl),gvl);

	// Build regime with arithmetic right shift on signed integer
	//Prepare the content wit [31-15]=1 [15-0]=0   1111 1111 1111 1111 1000 0000 0000 0000
	__epi_8xi32 regime_hold = vbroadcast8xi32(0xFFFF8000,gvl);
	regime_hold = __builtin_epi_vsra_8xi32(regime_hold,k_value_low,gvl);
	__epi_8xi32 regime_pos = __builtin_epi_vsra_8xi32(regime_hold,vbroadcast8xi32(1,gvl),gvl);
	__epi_8xi32 regime = __builtin_epi_vand_8xi32_mask(regime_pos,regime_pos,__builtin_epi_vnot_8xi32(regime_hold,gvl),f_exp_norm_sign,gvl);
	regime = __builtin_epi_vand_8xi32(regime,vbroadcast8xi32(0x00007FFF,gvl),gvl);
	//__epi_8xi32_print(regime,gvl,true);

	// Compute regime length from least 4 significant bits of adjusted k-value x:-x+1
	__epi_8xi32 regime_length = __builtin_epi_vand_8xi32(adj_k_value,vbroadcast8xi32(0xF,gvl),gvl);

	// Use regime length to shift the mantissa right 
	f_frac = __builtin_epi_vsrl_8xi32(f_frac,regime_length,gvl);

	// Pack everything using OR
	res_ = __builtin_epi_vor_8xi32(regime,f_frac,gvl);
	//__epi_8xi32_print(res_,gvl,true);

	// Use sign mask to perform 2's complement of Posit
	res_ = __builtin_epi_8xi32_abs(res_,f_sign,gvl);
	//__epi_8xi32_print(res_,gvl,true);


	return vreduce32_8(res_,gvl);
}

template <class Posit8>
__epi_8xf32 __builtin_epi_vfmv_v_P8_8xf32(Posit8 val, unsigned long gvl) {
	float fval = (float)val;
	__epi_8xf32 rvvec = __builtin_epi_vfmv_v_f_8xf32(fval,gvl);
	return rvvec;
}

template <class Posit8>
void __builtin_epi_vstore_8xf32_P8(Posit8* dst, __epi_8xf32 src, unsigned long gvl) {
	__epi_8xi8 p8_vec = float_to_posit8(src,gvl);
	auto i_dst = reinterpret_cast<int8_t*>(dst);
	__builtin_epi_vstore_8xi8(i_dst,p8_vec,gvl);
}

template <class Posit8>
__epi_8xf32 __builtin_epi_vload_P8_8xf32(Posit8* src,unsigned long gvl) {
	int8_t* posit_ivec = const_cast<int8_t*>(reinterpret_cast<const int8_t*>(src));
	__epi_8xi8 p8_vec = __builtin_epi_vload_8xi8(posit_ivec,gvl);
	__epi_8xf32 f_vec  = posit8_to_float(p8_vec,gvl);
	return f_vec;
}



#ifdef _GEMMX_
#define VINIT(z, n, t)  t ## n = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
#define VPFETCH(z, n, t)  t ## n = __builtin_epi_vfmv_v_f_2xf32(Af[(m+n)*K+k],all_gvl);
#define VACCUM(z,n,t) t ## n = __builtin_epi_vfmacc_2xf32(t ## n, A_m_k ## n, B_k_n,gvl);
#define VSTORE(z,X,t) __builtin_epi_vstore_2xf32(&Cf[(m+X) * N + n], accum ## X,gvl);
#define VACCPREFETCH(z,n,t) VPFETCH(z,n,__epi_2xf32 A_m_k) VACCUM(z,n,accum) 
#define VINITBLOCK(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VINIT, __epi_2xf32 accum)
#define VPREFETCHBLOCK(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VPFETCH, __epi_2xf32 A_m_k)
#define VACCUMBLOCK(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VACCUM,accum)
#define VSTOREBLOCK(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VSTORE, a)	
#define VACCUMPREFETCH(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VACCPREFETCH, a)	
#endif


#ifdef _GEMMX_NEXT_

#define VINIT_8(z, n, t)  t ## n = __builtin_epi_vfmv_v_f_8xf32(0.0f,all_gvl);
#define VPFETCH_8(z, n, t)  t ## n = __builtin_epi_vfmv_v_P8_8xf32(aPtr[(m+n)*K+k],all_gvl);
#define VACCUM_8(z,n,t) t ## n = __builtin_epi_vfmacc_8xf32(t ## n, A_m_k ## n, B_k_n,gvl);
#define VSTORE_8(z,X,t) __builtin_epi_vstore_8xf32_P8(&cPtr[(m+X) * N + n], accum ## X,gvl);
#define VACCPREFETCH_8(z,n,t) VPFETCH_8(z,n,__epi_8xf32 A_m_k) VACCUM_8(z,n,accum) 
#define VINITBLOCK_8(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VINIT_8, __epi_8xf32 accum)
#define VPREFETCHBLOCK_8(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VPFETCH_8, __epi_8xf32 A_m_k)
#define VACCUMBLOCK_8(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VACCUM_8,accum)
#define VSTOREBLOCK_8(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VSTORE_8, a)	
#define VACCUMPREFETCH_8(BLOCKN) BOOST_PP_REPEAT(BLOCKN, VACCPREFETCH_8, a)	
#endif