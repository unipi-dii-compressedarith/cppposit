#pragma once
#include <arm_sve.h>


svint16_t float_to_posit16(svfloat32_t opv,svbool_t pg) {
	svint32_t res_;
	svuint32_t f_int = svreinterpret_u32(opv);
	//__epi_4xi32_print(f_int,gvl,true);	
	svbool_t f_sign = svcmpne(pg,svand_z(pg,f_int,0x80000000),0);
	svuint32_t f_exp = svlsr_z(pg,svlsl_z(pg,f_int,1),24);
	svint32_t f_exp_norm = svsub_z(pg,svreinterpret_s32(f_exp),127); // also k-value
	svuint32_t f_frac = svlsr_z(pg,f_int,10);
	f_frac = svand_z(pg,f_frac,0x1FFF);

	// Exponent absolute value (LOW 8 bit -> 0xFFFFFF80)
	svbool_t f_exp_norm_sign = svcmpne(pg,svand_z(pg,f_exp_norm,0xFFFFFF80),0);
	svint32_t k_value = svadd_m(f_exp_norm_sign,svnot_m(f_exp_norm,f_exp_norm_sign,f_exp_norm),1);

	// Adjust k-value x:-x+1
	svint32_t adj_k_value = svadd_m(f_exp_norm_sign,svneg_m(k_value,f_exp_norm_sign,k_value),1);


	// Low 4 bits of k_value (AND with ____1111)
	svuint32_t k_value_low = svreinterpret_u32(svand_z(pg,k_value,0xF));

	// Build regime with arithmetic right shift on signed integer
	//Prepare the content wit [31-15]=1 [15-0]=0   1111 1111 1111 1111 1000 0000 0000 0000
	svint32_t regime_hold = svdup_s32(0xFFFF8000);
	regime_hold = svasr_z(pg,regime_hold,k_value_low);
	
	svint32_t regime_pos = svasr_z(pg,regime_hold,1);
	svint32_t regime = svand_m(f_exp_norm_sign,regime_pos,svnot_z(pg,regime_hold));
	regime = svand_z(pg,regime,0x00007FFF);

	// Compute regime length from least 4 significant bits of adjusted k-value x:-x+1
	svint32_t regime_length = svand_z(pg,adj_k_value,0xF);

	// Use regime length to shift the mantissa right 
	f_frac = svlsr_z(pg,f_frac,svreinterpret_u32(regime_length));

	// Pack everything using OR
	res_ = svorr_z(pg,regime,svreinterpret_s32(f_frac));

	// Use sign mask to perform 2's complement of Posit
	res_ = svneg_m(res_,f_sign,res_);


	return svreinterpret_s16(res_);
}


 svfloat32_t posit_to_float(svint16_t opv,svbool_t pg) {
		svint32_t res;
		unsigned short POSIT_HOLDER_MSB = (1 << 15), POSIT_SIGNBIT = (1 << 15),POSIT_EXTRA_BITS = 0;
        svbool_t sign_mask = svcmpne(pg,svand_z(pg,opv,POSIT_SIGNBIT),0); 
        //svint16_t n_one_helper =  __builtin_epi_vmv_v_x_4xi16(-1,gvl); 
        //svint16_t one_helper =  __builtin_epi_vmv_v_x_4xi16(1,gvl); 

        svint16_t pa = svneg_m(opv,sign_mask,opv); 
        svint16_t pars1 = svlsl_z(pg,pa,POSIT_EXTRA_BITS+1);

        svbool_t one_marker_mask = svcmpne(pg,svand_z(pg,pars1,POSIT_HOLDER_MSB),0); 
        svbool_t neg_one_marker_mask = svcmpeq(pg,svand_z(pg,pars1,POSIT_HOLDER_MSB),0); 
        svint16_t neg_with_mask = svnot_m(pars1,one_marker_mask,pars1); 

       	svint16_t index = svsub_z(pg,svdup_s16(15),svreinterpret_s16(svclz_z(pg,neg_with_mask))); 

        svint16_t kvalue = svadd_m(one_marker_mask,index,-1); 
        kvalue = svneg_m(kvalue,neg_one_marker_mask,kvalue); 

        svint16_t rs = svadd_z(pg,index,1); 

        //svint16_t rsexp = __builtin_epi_vadd_4xi16(rs,vbroadcast16(0,gvl),gvl);

        svint16_t pars2 = svlsl_z(pg,pars1,svreinterpret_u16(rs));

        //svint16_t fraction_nbits = __builtin_epi_vsub_4xi16(vbroadcast16(15,gvl),rsexp,gvl); 
        svint16_t fraction = pars2;

		// Adjust float sign in MSB
		res = svdup_s32_z(sign_mask,0x80000000);

		// Adjust float exponent in MSBs
		svint32_t f_exp = svadd_z(pg,svreinterpret_s32(kvalue),0x7f); 
		f_exp = svlsl_z(pg,svand_z(pg,f_exp,0xFF),23);

		// Adjust fraction in 23 LSB mantissa bits
		svint32_t f_frac = svlsl_z(pg,svreinterpret_s32(fraction),7);

		// Pack the float
		res = svorr_z(pg,res,f_exp);
		res = svorr_z(pg,res,f_frac);
		return svreinterpret_f32(res);
}

 svfloat32_t posit_to_float(svint8_t opv,svbool_t pg) {
		svint32_t res;
		unsigned short POSIT_HOLDER_MSB = (1 << 7), POSIT_SIGNBIT = (1 << 7),POSIT_EXTRA_BITS = 0;
        svbool_t sign_mask = svcmpne(pg,svand_z(pg,opv,POSIT_SIGNBIT),0); 
        //svint16_t n_one_helper =  __builtin_epi_vmv_v_x_4xi16(-1,gvl); 
        //svint16_t one_helper =  __builtin_epi_vmv_v_x_4xi16(1,gvl); 

        svint8_t pa = svneg_m(opv,sign_mask,opv); 
        svint8_t pars1 = svlsl_z(pg,pa,POSIT_EXTRA_BITS+1);

        svbool_t one_marker_mask = svcmpne(pg,svand_z(pg,pars1,POSIT_HOLDER_MSB),0); 
        svbool_t neg_one_marker_mask = svcmpeq(pg,svand_z(pg,pars1,POSIT_HOLDER_MSB),0); 
        svint8_t neg_with_mask = svnot_m(pars1,one_marker_mask,pars1); 

       	svint8_t index = svsub_z(pg,svdup_s8(7),svreinterpret_s8(svclz_z(pg,neg_with_mask))); 

        svint8_t kvalue = svadd_m(one_marker_mask,index,-1); 
        kvalue = svneg_m(kvalue,neg_one_marker_mask,kvalue); 

        svint8_t rs = svadd_z(pg,index,1); 

        //svint8_t rsexp = __builtin_epi_vadd_4xi16(rs,vbroadcast16(0,gvl),gvl);

        svint8_t pars2 = svlsl_z(pg,pars1,svreinterpret_u8(rs));

        //svint8_t fraction_nbits = __builtin_epi_vsub_4xi16(vbroadcast16(15,gvl),rsexp,gvl); 
        svint8_t fraction = pars2;

		// Adjust float sign in MSB
		res = svdup_s32_z(sign_mask,0x80000000);

		// Adjust float exponent in MSBs
		svint32_t f_exp = svadd_z(pg,svreinterpret_s32(kvalue),0x7f); 
		f_exp = svlsl_z(pg,svand_z(pg,f_exp,0xFF),23);

		// Adjust fraction in 23 LSB mantissa bits
		svint32_t f_frac = svlsl_z(pg,svreinterpret_s32(fraction),7);

		// Pack the float
		res = svorr_z(pg,res,f_exp);
		res = svorr_z(pg,res,f_frac);
		return svreinterpret_f32(res);
}
        

svint8_t float_to_posit8(svfloat32_t opv,svbool_t pg) {
	svint32_t res_;
	svuint32_t f_int = svreinterpret_u32(opv);
	//__epi_4xi32_print(f_int,gvl,true);	
	svbool_t f_sign = svcmpne(pg,svand_z(pg,f_int,0x80000000),0);
	svuint32_t f_exp = svlsr_z(pg,svlsl_z(pg,f_int,1),24);
	svint32_t f_exp_norm = svsub_z(pg,svreinterpret_s32(f_exp),127); // also k-value
	svuint32_t f_frac = svlsr_z(pg,f_int,10);
	f_frac = svand_z(pg,f_frac,0x1FFF);

	// Exponent absolute value (LOW 8 bit -> 0xFFFFFF80)
	svbool_t f_exp_norm_sign = svcmpne(pg,svand_z(pg,f_exp_norm,0xFFFFFF80),0);
	svint32_t k_value = svadd_m(f_exp_norm_sign,svnot_m(f_exp_norm,f_exp_norm_sign,f_exp_norm),1);

	// Adjust k-value x:-x+1
	svint32_t adj_k_value = svadd_m(f_exp_norm_sign,svneg_m(k_value,f_exp_norm_sign,k_value),1);


	// Low 3 bits of k_value (AND with ____1111)
	svuint32_t k_value_low = svreinterpret_u32(svand_z(pg,k_value,0xF));

	// Build regime with arithmetic right shift on signed integer
	//Prepare the content wit [31-15]=1 [15-0]=0   1111 1111 1111 1111 1000 0000 0000 0000
	svint32_t regime_hold = svdup_s32(0xFFFF8000);
	regime_hold = svasr_z(pg,regime_hold,k_value_low);
	
	svint32_t regime_pos = svasr_z(pg,regime_hold,1);
	svint32_t regime = svand_m(f_exp_norm_sign,regime_pos,svnot_z(pg,regime_hold));
	regime = svand_z(pg,regime,0x00007FFF);

	// Compute regime length from least 4 significant bits of adjusted k-value x:-x+1
	svint32_t regime_length = svand_z(pg,adj_k_value,0xF);

	// Use regime length to shift the mantissa right 
	f_frac = svlsr_z(pg,f_frac,svreinterpret_u32(regime_length));

	// Pack everything using OR
	res_ = svorr_z(pg,regime,svreinterpret_s32(f_frac));

	// Use sign mask to perform 2's complement of Posit
	res_ = svneg_m(res_,f_sign,res_);


	return svreinterpret_s8(res_);
}


typedef svbool_t (*svptrue_ptr)();
typedef svbool_t (*svwhielt_ptr)(int32_t,int32_t);


#define SVL0PROLOGUE(IN_VEC,OUT_VEC) \
PositInt* OUT_VEC = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(IN_VEC.data())); \
int32_t n = IN_VEC.size(), i =0; \
svbool_t all = svptrue(); \
svbool_t Pg; \
uint64_t cntw = svcntw();

#define SVSIGMOID(IN_VEC,OUT_VEC) \
svint sigm__a = svasr_x(Pg,IN_VEC,(uint64_t)1); \
svint sigm__b = svadd_x(Pg,sigm__a,PositT::PT::POSIT_INVERTBIT); \
svint OUT_VEC = svasr_x(Pg,sigm__b,(uint64_t)1);

#define SVONECOMP(IN_VEC,OUT_VEC) \
svint one_comp__a = svneg_x(Pg,IN_VEC); \
svint OUT_VEC = svadd_x(Pg,one_comp__a,PositT::PT::POSIT_INVERTBIT);



#define D(NAME,...) NAME<__VA_ARGS__>

#define PICK_SVE_FUN_SAME_RET(FPTR_NAME,FUN8_NAME,FUN16_NAME,FUN32_NAME) \
switch(sizeof(typename PositT::value_t)) { \
	case 1: FPTR_NAME = &FUN8_NAME; break; \
	case 2: FPTR_NAME = &FUN16_NAME; break; \
	case 4: FPTR_NAME = &FUN32_NAME; break; \
	default: throw std::invalid_argument("Unsupported PositT size"); exit(1); \
} 

#define PICK_SVE_FUN_SIZE_DEP(FPTR_NAME,FUN8_NAME,FUN16_NAME,FUN32_NAME) \
		using FPTR_NAME ## FPTR_NAME = \
				typename std::conditional< \
				sizeof(typename PositT::value_t) == 1,FUN8_NAME, \
					typename std::conditional< \
						sizeof(typename PositT::value_t) == 2,FUN16_NAME, \
				 		typename std::conditional< \
				 			sizeof(typename PositT::value_t) == 4,FUN32_NAME,FUN32_NAME \
				 		>::type \
				 	>::type \
				 >::type; \
			FPTR_NAME ## FPTR_NAME FPTR_NAME;

template <class PositT>
	using _svint = 
		typename std::conditional<
			sizeof(typename PositT::value_t) == 1,svint8_t,
			typename std::conditional<
				sizeof(typename PositT::value_t) == 2,svint16_t,
		 		typename std::conditional<
		 			sizeof(typename PositT::value_t) == 4,svint32_t,svint32_t
		 		>::type
		 	>::type
		 >::type;

template <class PositT>
using _svuint = 
	typename std::conditional<
		sizeof(typename PositT::value_t) == 1,svuint8_t,
		typename std::conditional<
			sizeof(typename PositT::value_t) == 2,svuint16_t,
	 		typename std::conditional<
	 			sizeof(typename PositT::value_t) == 4,svuint32_t,svuint32_t
	 		>::type
	 	>::type
	 >::type;				 


template<typename Utype,typename SType>
class _svreinterpret_u8_s8   { public:Utype operator()(SType s)const{return svreinterpret_u8_s8(s);}};
template<typename Utype,typename SType>			
class _svreinterpret_u16_s16 { public:Utype operator()(SType s)const{return svreinterpret_u16_s16(s);}};
template<typename Utype,typename SType>			
class _svreinterpret_u32_s32 { public:Utype operator()(SType s)const{return svreinterpret_u32_s32(s);}};
template<typename Utype,typename SType>			
class _svreinterpret_s8_u8   { public:SType operator()(Utype s)const{return svreinterpret_s8_u8(s);}};
template<typename Utype,typename SType>			
class _svreinterpret_s16_u16 { public:SType operator()(Utype s)const{return svreinterpret_s16_u16(s);}};
template<typename Utype,typename SType>			
class _svreinterpret_s32_u32 { public:SType operator()(Utype s)const{return svreinterpret_s32_u32(s) ;}};
class _svwhilelt_b8  { public:svbool_t operator()(int32_t a,int32_t b)const{return svwhilelt_b8(a,b) ;}};
class _svwhilelt_b16 { public:svbool_t operator()(int32_t a,int32_t b)const{return svwhilelt_b16(a,b);}};
class _svwhilelt_b32 { public:svbool_t operator()(int32_t a,int32_t b)const{return svwhilelt_b32(a,b);}};
class _svptrue_b8  { public:svbool_t operator()()const{return svptrue_b8() ;}};
class _svptrue_b16 { public:svbool_t operator()()const{return svptrue_b16();}};
class _svptrue_b32 { public:svbool_t operator()()const{return svptrue_b32();}};
class _svcntw  { public:uint64_t operator()()const{return svcntw() ;}};
class _svcnth { public:uint64_t operator()()const{return svcnth();}};
class _svcntb { public:uint64_t operator()()const{return svcntb();}};
class _f2p8 { public:svint8_t operator()(svfloat32_t a,svbool_t b)const{return float_to_posit8(a,b) ;}};
class _f2p16 { public:svint16_t operator()(svfloat32_t a,svbool_t b)const{return float_to_posit16(a,b) ;}};

//class _svreinterpret_s32_u32 { public:SType operator()(Utype s)const{return svreinterpret_s32_u32(s);}};




