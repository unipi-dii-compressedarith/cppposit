#pragma once
#include "posit.h"
#include "posit_vect.h"
#include <type_traits>
#include <stdexcept>
#include <posit_rvv_types.h>
#include <chrono>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;

namespace posit {
	namespace vectorized {

		template<class PositT>
		class PositRVVBackend : public PositVectorizedFrontend<PositT> {
		public:
			using PositInt = typename PositT::value_t;
			using PositUInt = typename PositT::PT::POSIT_UTYPE;
			std::vector<PositT> vBuildPosit(PositInt* src,int n) {
                PositT* res = const_cast<PositT*>(reinterpret_cast<const PositT*>(src));
				std::vector<PositT> vec(res,res+n);
				return vec;
			}
            
            PositT* vFloat2Posit(const std::vector<float>& op) {
               int gvl = 0;
				long unsigned int N = op.size();
			   PositInt* posit_vec = new PositInt[N];
				__epi_4xi16 rvvihvec;	
				const float* float_vec = op.data();
				for(int i = 0; i < N; i+=gvl) {
					gvl = __builtin_epi_vsetvl(N-i,__epi_e32,__epi_m2);
					rvvihvec = float_to_posit16(__builtin_epi_vload_4xf32(&float_vec[i],gvl),gvl);
					__builtin_epi_vstore_4xi16(&posit_vec[i],rvvihvec,gvl);
				}
				return reinterpret_cast<PositT*>(posit_vec);
            }
            
            float* vPosit2Float(const std::vector<PositT>& op) {
            	int gvl = 0;
				long unsigned int N = op.size();
				PositInt* posit_vec = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(op.data()));
				float* fl_out = new float[N];
				__epi_4xf32 fint;
				for(int i = 0; i < N; i+=gvl) {
					gvl = __builtin_epi_vsetvl(N-i,__epi_e16,__epi_m1);
					fint = posit16_to_float(__builtin_epi_vload_4xi16(&posit_vec[i],gvl),gvl);
					__builtin_epi_vstore_4xf32(&fl_out[i],fint,gvl);
				}
				return fl_out;
            }
 
            PositT* vFloat2Posit(float* op,long unsigned int N,PositT* dst) {
               int gvl = 0;
			   PositInt* posit_vec = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(dst));
				__epi_4xi16 rvvihvec;	
				float* float_vec =op;
				for(int i = 0; i < N; i+=gvl) {
					gvl = __builtin_epi_vsetvl(N-i,__epi_e32,__epi_m2);
					rvvihvec = float_to_posit16(__builtin_epi_vload_4xf32(&float_vec[i],gvl),gvl);
					__builtin_epi_vstore_4xi16(&posit_vec[i],rvvihvec,gvl);
				}
				return reinterpret_cast<PositT*>(posit_vec);
            }
            
            float* vPosit2Float(const PositT* op,long unsigned int N) {
            	int gvl = 0;
				PositInt* posit_vec = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(op));
				float* fl_out = new float[N];
				__epi_4xf32 fint;
				for(int i = 0; i < N; i+=gvl) {
					gvl = __builtin_epi_vsetvl(N-i,__epi_e16,__epi_m1);
					fint = posit16_to_float(__builtin_epi_vload_4xi16(&posit_vec[i],gvl),gvl);
					__builtin_epi_vstore_4xf32(&fl_out[i],fint,gvl);
				}
				return fl_out;
            }

			void vFastSigmoid(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				PositInt* int_op = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(op.data()));
				PositInt* dst_op = new PositInt[dst.size()];
				unsigned long int N = dst.size();
                unsigned long int gvl = 0;
				for(int i = 0; i < N;i+=gvl) {
				   gvl = __builtin_epi_vsetvl(N-i,__epi_e16,__epi_m1);
				   auto opv = __builtin_epi_vload_4xi16(&int_op[i],gvl);
				   auto one_helper = __builtin_epi_vmv_v_x_4xi16(1,gvl);
				   auto opv_shifted = __builtin_epi_vsra_4xi16(opv,one_helper,gvl);
				   auto invert_bit = __builtin_epi_vmv_v_x_4xi16(PositT::PT::POSIT_INVERTBIT,gvl);
				   auto opv_sum_inv = __builtin_epi_vadd_4xi16(invert_bit,opv_shifted,gvl);
				   auto opv_shifted_final = __builtin_epi_vsra_4xi16(opv_sum_inv,one_helper,gvl);
				   __builtin_epi_vstore_4xi16(&dst_op[i],opv_shifted_final,gvl);
				}
                PositT* dres = const_cast<PositT*>(reinterpret_cast<const PositT*>(dst_op));
				dst.assign(dres,dres+N);
			};

			void vFastTanh(const std::vector<PositT>&  op,std::vector<PositT>& dst) {
                
			};
			void vFastOneComp(const std::vector<PositT>& op,std::vector<PositT>& dst) {
			};


			/* 1-operand L1 Operations */
			void vFastTwice(const std::vector<PositT>& op,std::vector<PositT>& dst) {
			};

			void vFastInv(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				
			};

			void vFastELU(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				PositInt* int_op = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(op.data()));
                PositInt* dst_op = new PositInt[dst.size()];                                                   
				unsigned long int gvl = 0;              
				unsigned long int N = dst.size();
                for(int i = 0; i < N;i+=gvl) {
					gvl = __builtin_epi_vsetvl(N-i,__epi_e16,__epi_m1);                                                
					auto opv = __builtin_epi_vload_4xi16(&int_op[i],gvl);
					
					//Negative values mask
					auto zero_helper = __builtin_epi_vmv_v_x_4xi16(0,gvl);
					__epi_4xi1 neg_mask  =__builtin_epi_vmslt_4xi16(opv,zero_helper,gvl);
					
					//Pseudosigmoid for negative values
					auto one_helper = __builtin_epi_vmv_v_x_4xi16(1,gvl);
					auto opv_shifted = __builtin_epi_vsra_4xi16_mask(opv,opv,one_helper,neg_mask,gvl);
					auto invert_bit = __builtin_epi_vmv_v_x_4xi16(PositT::PT::POSIT_INVERTBIT,gvl);
                	                auto opv_sum_inv = __builtin_epi_vadd_4xi16_mask(opv,invert_bit,opv_shifted,neg_mask,gvl);   
					auto opv_shifted_final = __builtin_epi_vsra_4xi16_mask(opv,opv_sum_inv,one_helper,neg_mask,gvl);

					auto reciprocate_helper =  __builtin_epi_vmv_v_x_4xi16(PositUInt((~PositT::PT::POSIT_SIGNBIT)),gvl);
					auto opv_sf_reciprocate = __builtin_epi_vor_4xi16_mask(opv,opv_shifted_final,reciprocate_helper,neg_mask,gvl);
					
					//Half
					auto opv_sfr_half  =  __builtin_epi_vsra_4xi16_mask(opv,opv_sf_reciprocate,one_helper,neg_mask,gvl);
					
					// 1-x
					__builtin_epi_vnot_4xi16_mask(opv,opv_sfr_half,neg_mask,gvl,opv_sfrh_neg);
					auto opv_sfrhn_inv = __builtin_epi_vadd_4xi16_mask(opv,invert_bit,opv_sfrh_neg,neg_mask,gvl);

					//Twice
					auto opv_sfrhni_twice =  __builtin_epi_vsll_4xi16_mask(opv,opv_sfrhn_inv,one_helper,neg_mask,gvl);
					
					//Not
					__builtin_epi_vnot_4xi16_mask(opv,opv_sfrhni_twice,neg_mask,gvl,res);
					
					//Store back
					__builtin_epi_vstore_4xi16(&dst_op[i],res,gvl);
				}
                PositT* dres = const_cast<PositT*>(reinterpret_cast<const PositT*>(dst_op));
				dst.assign(dres,dres+N);
			};

			#include "posit_rvv_common.h"
		
		};	

	}
}
