#pragma once
#include "posit.h"
#include "posit_vect.h"
#include <arm_sve.h>
#include <type_traits>
#include <stdexcept>
#include <posit_sve_types.h>
#include <chrono>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;


namespace posit {
	namespace vectorized {

		template<class PositT>
		class PositSVEBackend : public PositVectorizedFrontend<PositT> {
		public:
			using svint = _svint<PositT>;
			using svuint = _svuint<PositT>;
			using PositInt = typename PositT::value_t;
			using PositUInt = typename PositT::PT::POSIT_UTYPE;

	


			PICK_SVE_FUN_SIZE_DEP(svreinterpret_u_s,D(_svreinterpret_u8_s8,svuint,svint),D(_svreinterpret_u16_s16,svuint,svint),D(_svreinterpret_u32_s32,svuint,svint));
			PICK_SVE_FUN_SIZE_DEP(svreinterpret_s_u,D(_svreinterpret_s8_u8,svuint,svint),D(_svreinterpret_s16_u16,svuint,svint),D(_svreinterpret_s32_u32,svuint,svint));
			PICK_SVE_FUN_SIZE_DEP(svwhilelt,_svwhilelt_b8,_svwhilelt_b16,_svwhilelt_b32);
			PICK_SVE_FUN_SIZE_DEP(svptrue,_svptrue_b8,_svptrue_b16,_svptrue_b32);
			PICK_SVE_FUN_SIZE_DEP(svcnt,_svcntb,_svcnth,_svcntw);
			PICK_SVE_FUN_SIZE_DEP(float_to_posit,_f2p8,_f2p16,_f2p16);

			float32_t* posit2float(std::vector<PositT>& op) {
			   	long unsigned int N = op.size();
			   	float32_t* float_vec = new float32_t[N];
				svbool_t all = svptrue(); 
				svbool_t Pg; 
				PositInt* posit_vec = const_cast<PositInt*>(reinterpret_cast<const PositInt*>(op.data()));
				for(long unsigned int i = 0;svptest_first(all,Pg=svwhilelt(i, N));i+=svcnt()) {
					svint op_vec = svld1(Pg,&posit_vec[i]);
					svfloat32_t res = posit_to_float(op_vec,Pg);
					svbool_t Pg_w=svwhilelt_b32(i,N);
					svst1(Pg_w,&float_vec[i],res);
				}
				return float_vec;
			}

			PositT* float2posit(std::vector<float32_t>& op) {
			   	long unsigned int N = op.size();
			   	PositInt* posit_vec = new PositInt[N];
				svbool_t all = svptrue(); 
				svbool_t Pg; 
				const float* float_vec = op.data();
				for(long unsigned int i = 0;svptest_first(all,Pg=svwhilelt_b32(i, N));i+=svcntw()) {
					svfloat32_t op_vec = svld1(Pg,&float_vec[i]);
					svint res = float_to_posit(op_vec,Pg);
					svbool_t Pg_h=svwhilelt(i, N);
					svst1(Pg_h,&posit_vec[i],res);
				}
				return reinterpret_cast<PositT*>(posit_vec);
			}		

			void vAdd(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) {};
			void vMul(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) {};
			void vMuladd(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) {};			

			void sv_conv3x3(std::vector<PositT>& op1,std::vector<PositT>& filter,std::vector<PositT>& dst,long unsigned int R,long unsigned int C) {
				auto start = get_time::now();
				// L3 Prologue *************************************
				float32_t* F = posit2float(filter);
				float32_t* A = posit2float(op1);
				/**************************************************/
				
				auto prologue_end = get_time::now();
				std::vector<float32_t> O((R-2)*(C-2));

				// Load&Duplicate filter values 	
				svfloat32_t f00 = svdup_f32(F[0]); 
				svfloat32_t f01 = svdup_f32(F[1]);
				svfloat32_t f02 = svdup_f32(F[2]);
				svfloat32_t f10 = svdup_f32(F[3]);
				svfloat32_t f11 = svdup_f32(F[4]);
				svfloat32_t f12 = svdup_f32(F[5]);
				svfloat32_t f20 = svdup_f32(F[6]);
				svfloat32_t f21 = svdup_f32(F[7]);
				svfloat32_t f22 = svdup_f32(F[8]);
				// Perform element-wise product between filter lanes and matrix rows
				for(long unsigned int i = 0; i < R-2; ++i) {
					// Result for current row iteration

					// Loop for the internal element-wise product
					for(long unsigned int j = 0; j < C - 2; j+=svcntw()-2) {

						// Predicate for the different rows of the matrix
						svbool_t pg = svwhilelt_b32(j,C);

						// Predictate for elements of first column: 0 <= j < L-2
						svbool_t pgx0 = svand_z(pg,pg,svwhilelt_b32(j, std::min(C-2,j+svcntw()-2)));

						// Predictate for elements of second column: 0 < j < L-1
						svbool_t pgx1lt = svwhilelt_b32(j, std::min(j+svcntw()-1,C-1));
						svbool_t pgx1gt = svnot_z(pg,svwhilelt_b32(j, j+(long unsigned int)1));
						svbool_t pgx1   = svand_z(pg, pgx1lt, pgx1gt);

						// Predictate for elements of third column: 1 < j < L
						svbool_t pgx2lt = svwhilelt_b32(j, std::min(C,j+svcntw()));
						svbool_t pgx2gt = svnot_z(pg,svwhilelt_b32(j, j+(long unsigned int)2));
						svbool_t pgx2   = svand_z(pg, pgx2lt, pgx2gt);

						// Load the current 3-row batch of matrix A
						auto Ai0 = svld1(pg,&A[i*C+j]);
						auto Ai1 = svld1(pg,&A[(i+1)*C+j]);
						auto Ai2 = svld1(pg,&A[(i+2)*C+j]);

						// Perform all the 9 element-wise products

						// First row multiplication
						auto p00 = svmul_z(pgx0,f00,Ai0);
						auto p01 = svrev(svinsr(svrev(svmul_z(pgx1,f01,Ai0)),0));
						auto p02 = svrev(svinsr(svinsr(svrev(svmul_z(pgx2,f02,Ai0)),0),0));

						// Second row multiplication
						auto p10 = svmul_z(pgx0,f10,Ai1);
						auto p11 = svrev(svinsr(svrev(svmul_z(pgx1,f11,Ai1)),0));
						auto p12 = svrev(svinsr(svinsr(svrev(svmul_z(pgx2,f12,Ai1)),0),0));

						// Third row multiplication
						auto p20 = svmul_z(pgx0,f20,Ai2);
						auto p21 = svrev(svinsr(svrev(svmul_z(pgx1,f21,Ai2)),0));
						auto p22 = svrev(svinsr(svinsr(svrev(svmul_z(pgx2,f22,Ai2)),0),0));

						// Sum columns
						auto c0 = svadd_x(pg,p00,svadd_x(pg,p10,p20));
						auto c1 = svadd_x(pg,p01,svadd_x(pg,p11,p21));
						auto c2 = svadd_x(pg,p02,svadd_x(pg,p12,p22));

						// Complete sum
						auto res = svadd_x(pg,c0,svadd_x(pg,c1,c2));

						// Store sum result in the O matrix
						svst1(pg,&O[i*(C-2)+j],res);
					}
				}
				auto body_end = get_time::now();
				PositT* dstptr = float2posit(O);
				//for(int i = 0; i < (C-2)*(R-2); ++i) dst[i] = PositT(O[i]);
				auto epilogue_end = get_time::now();
                dst.assign(dstptr,dstptr+(R-2)*(C-2));

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				std::cout  << prologue << "," << body << "," << epilogue << "\n";
			}
			/*
				General matrix multiplication A*B=C.
				in:A is a MxK matrix
				in:B is a KxN matrix
				out:C is a MxN matrix
			*/
			void vGemm(const std::vector<PositT>& A,const std::vector<PositT>& B, std::vector<PositT>& C,uint32_t M,uint32_t K,uint32_t N) {
                auto start = get_time::now();            
				// L3 Prologue: naive for now
				std::vector<float32_t> Af(M*K),Bf(K*N),Cf(M*N);
				for(int i = 0; i < M*K; ++i)
					Af[i]=float32_t(A[i]);
				for(int i = 0; i < K*N; ++i)
					Bf[i]=float32_t(B[i]);
				auto prologue_end = get_time::now();                
				// Loop on A rows
				for (uint32_t i = 0; i < M; ++i)	
				    // Loop on B cols
					for (uint32_t j = 0; j < N; j += svcntw()) {
					    // Prepare result row for matrix C
						svfloat32_t accum = svdup_f32(0);
						
						const svbool_t pred_j = svwhilelt_b32(j, N);
						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							auto A_i_k = svdup_f32(Af[i * K + k]);
							auto B_k_j = svld1(pred_j, &Bf[k * N + j]);

							// Add the contribution to row i-th of C 
							accum = svmla_x(pred_j, accum, A_i_k, B_k_j);
						}
						svst1(pred_j, &Cf[i * N + j], accum);
				}
				auto body_end = get_time::now();

				// L3 Epilogue
				for(int i = 0; i < M*N;++i)
					C[i] = PositT(Cf[i]);
                auto epilogue_end = get_time::now();
				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				std::cout  << prologue << "," << body << "," << epilogue << "\n";                    
			}

			void vMaxPool3f(std::vector<PositT>& A,std::vector<PositT>& O,long unsigned int R,long unsigned int C) {
				// L3 Prologue *************************************
				std::vector<float32_t> Af(R*C),Of(R/3*C/3); // ToDo check if multiple of 3
				for(int i = 0; i < R*C;++i) Af[i] = float32_t(A[i]);
				/**************************************************/
				svbool_t all = svptrue_b32();
				svbool_t Pg;	
				float32_t* mRow = new float32_t[C];
                
				for(long unsigned int i = 0; i < R - 2; i+=3) {
					for(long unsigned int j = 0;svptest_first(all,Pg=svwhilelt_b32(j, C)); j+=svcntw()) {
						
						// Load the three A rows targeted by current i value
						auto Ai_1 = svld1(Pg,&Af[i*C+j]);
						auto Ai_2 = svld1(Pg,&Af[(i+1)*C+j]);
						auto Ai_3 = svld1(Pg,&Af[(i+2)*C+j]);

						// Compute element-wise maximum between the three rows
						auto Ai_max = svmax_z(Pg,Ai_1,svmax_z(Pg,Ai_2,Ai_3));

						// Store back the maxima in the maxima row
						svst1(Pg,&mRow[j],Ai_max);
					}

					// Auto-unrolled loop: compute 3by3 maxima (force vectorization using pragmas)
					#pragma clang loop interleave_count(3)
					for(long unsigned int k = 0; k < C - 2; k+=3) {
						 auto tmp = std::max(mRow[k],mRow[k+1]);
						 Of[i/3*C/3 + k/3] = std::max(mRow[k+2],tmp);
					}
				}

				for(int i = 0; i < (C*R/9); ++i) O[i] = PositT(Of[i]);
				delete[](mRow);
			}

			/* 2-operands Reducing operations */
			PositT vDot(std::vector<PositT>& op1,std::vector<PositT>& op2) {
			/*
				L3Prologue: Building float vector from Posit Vector naively for now
			*/
            auto start = get_time::now();
			std::vector<float32_t> op1f(op1.size()),op2f(op2.size());
			for(int i = 0; i < op1.size(); ++i) {
				op1f[i]=float32_t(op1[i]);
				op2f[i]=float32_t(op2[i]);
			}
            auto prologue_end = get_time::now();
			int32_t n = op1.size(), i =0;
			svbool_t all = svptrue_b32();
			svbool_t Pg;	
			/***** SVFLOAT DOT PRODUCT *******/
			float32_t accum(0);
			
			for(int i = 0;
			    svptest_first(all,Pg=svwhilelt_b32(i, n));
			    i+=svcntw()) {
				svfloat32_t op1_vec = svld1(Pg,&op1f[i]);
				svfloat32_t op2_vec = svld1(Pg,&op2f[i]);
				svfloat32_t res = svmul_f32_m(Pg,op1_vec,op2_vec);
				accum = svadda_f32(Pg,accum,res);
			}			
            auto body_end = get_time::now();
				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				std::cout << prologue << "," << body <<  "\n";
			/*********************************/
			
			return PositT(accum);};


			/* 1-operand L1 Operations */
			void vFastTwice(const std::vector<PositT>& op,std::vector<PositT>& dst) {

			};
			void vFastInv(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				SVL0PROLOGUE(op,cOp);
				for(int i = 0;svptest_first(all,Pg=svwhilelt(i, n));i+=svcnt()) {
					svint op_vec = svld1(Pg,&cOp[i]);
					svbool_t neg_vals = svcmplt(Pg,op_vec,0);
					svint res = svneg_x(neg_vals,op_vec);					
					res = sveor_x(Pg,res,PositUInt(~(PositT::PT::POSIT_SIGNBIT)));
					res = svneg_x(neg_vals,op_vec);							
					svst1(Pg,&cOp[i],res);
				}				
				dst = vBuildPosit(cOp,n);				
			};
			void vFastELU(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				SVL0PROLOGUE(op,cOp);
				for(int i = 0;svptest_first(all,Pg=svwhilelt(i, n));i+=svcnt()) {
					svint op_vec = svld1(Pg,&cOp[i]);
					svbool_t neg_vals = svcmplt(Pg,op_vec,0);
					
					// Neg
					auto res = svneg_x(neg_vals,op_vec);

					// Pesudosigmoid					
					res = svasr_x(neg_vals,res,(uint64_t)1); 
					res = svadd_x(neg_vals,res,PositT::PT::POSIT_INVERTBIT); 
					res = svasr_x(neg_vals,res,(uint64_t)1);
					
					// Reciprocate
					res = sveor_x(neg_vals,res,PositUInt(~(PositT::PT::POSIT_SIGNBIT)));
					
					// Half
					res = svasr_x(neg_vals,res,1);

					// 1-x
					res = svneg_x(neg_vals,res); 
					res = svadd_x(neg_vals,res,PositT::PT::POSIT_INVERTBIT);

					// Twice
					res = svlsl_x(neg_vals,res,1);

					// Neg
					res = svneg_x(neg_vals,res);

					svst1(Pg,&cOp[i],res);
				}
				dst = vBuildPosit(cOp,n);				
			};
			
			void vFastSigmoid(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				SVL0PROLOGUE(op,cOp);
				for(int i = 0;svptest_first(all,Pg=svwhilelt(i, n));i+=svcnt()) {
					svint op_vec = svld1(Pg,&cOp[i]);
					SVSIGMOID(op_vec,res);
					svst1(Pg,&cOp[i],res);
				}
				dst = vBuildPosit(cOp,n);
			};

			void vFastTanh(const std::vector<PositT>&  op,std::vector<PositT>& dst) {
				SVL0PROLOGUE(op,cOp);
				for(int i = 0;svptest_first(all,Pg=svwhilelt(i, n));i+=svcnt()) {
					svint op_vec = svld1(Pg,&cOp[i]);
					svbool_t neg_vals = svcmplt(Pg,op_vec,0);
					svbool_t pos_vals = svnot_z(Pg,neg_vals);
					op_vec = svneg_x(pos_vals,op_vec);
					auto res_s = svlsl_x(Pg,op_vec,1);
					SVSIGMOID(res_s,sig_res);
					auto tsig_res = svlsl_x(Pg,sig_res,1);
					SVONECOMP(tsig_res,res_one_m);
					auto res = svneg_x(neg_vals,res_one_m);
					svst1(Pg,&cOp[i],res);
				}				
				dst = vBuildPosit(cOp,n);
			};
			void vFastOneComp(const std::vector<PositT>& op,std::vector<PositT>& dst) {
				SVL0PROLOGUE(op,cOp);
				for(int i = 0;svptest_first(all,Pg=svwhilelt(i, n));i+=svcnt()) {
					svint op_vec = svld1(Pg,&cOp[i]);
					SVONECOMP(op_vec,res);
					svst1(Pg,&cOp[i],res);
				}
				dst = vBuildPosit(cOp,n);				
			};
		};
	}
}
