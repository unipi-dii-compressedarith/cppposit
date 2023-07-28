			void vAdd(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) {};
			void vMul(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) {};
			void vMuladd(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) {};						
            
            
            void sv_conv3x3(std::vector<PositT>& op1,std::vector<PositT>& filter,std::vector<PositT>& dst,long unsigned int R,long unsigned int C) {
				auto start = get_time::now();
				std::vector<float> O((R-2)*(C-2))/*,F(3*3),A(R*C)*/;
                float* A,*F;
                A = vPosit2Float(op1);
                F = vPosit2Float(filter);
				auto prologue_end = get_time::now();
				//Load & Duplicate filter values
				unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
				__epi_2xf32 f00 = __builtin_epi_vfmv_v_f_2xf32(F[0],all_gvl);
				__epi_2xf32 f01 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[1],all_gvl),0,all_gvl);
				__epi_2xf32 f02 =  __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[2],all_gvl),0,all_gvl),0,all_gvl);
				__epi_2xf32 f10 = __builtin_epi_vfmv_v_f_2xf32(F[3],all_gvl);
				__epi_2xf32 f11 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[4],all_gvl),0,all_gvl);
				__epi_2xf32 f12 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[5],all_gvl),0,all_gvl),0,all_gvl);
				__epi_2xf32 f20 = __builtin_epi_vfmv_v_f_2xf32(F[6],all_gvl);
				__epi_2xf32 f21 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[7],all_gvl),0,all_gvl);
				__epi_2xf32 f22 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[8],all_gvl),0,all_gvl),0,all_gvl);
				
				for(long unsigned int i=0; i < R-2; ++i) {
					for(long unsigned j = 0; j < C-2;) {
						// Load 3 rows batch of matrix A
						long unsigned int gvl = __builtin_epi_vsetvl(C-j,__epi_e32,__epi_m1);
						auto Ai0 =  __builtin_epi_vload_2xf32(&A[i*C+j],gvl);
						auto Ai1 =  __builtin_epi_vload_2xf32(&A[(i+1)*C+j],gvl);
						auto Ai2 =  __builtin_epi_vload_2xf32(&A[(i+2)*C+j],gvl);
						// First row multiplication
						auto p00 = __builtin_epi_vfmul_2xf32(Ai0,f00,gvl);
						auto p01 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f01,gvl),0,gvl);
						auto p02 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f02,gvl),2,gvl);
						// Second row multiplication
                        auto p10 = __builtin_epi_vfmul_2xf32(Ai1,f10,gvl);
						auto p11 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f11,gvl),0,gvl);
						auto p12 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f12,gvl),2,gvl);
						// Third row multiplication
						auto p20 = __builtin_epi_vfmul_2xf32(Ai2,f20,gvl);
						auto p21 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f21,gvl),0,gvl);
						auto p22 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f22,gvl),2,gvl);
						// Sum columns
						auto c0 = __builtin_epi_vfadd_2xf32(p00,__builtin_epi_vfadd_2xf32(p10,p20,gvl),gvl);
						auto c1 = __builtin_epi_vfadd_2xf32(p01,__builtin_epi_vfadd_2xf32(p11,p21,gvl),gvl);
						auto c2 = __builtin_epi_vfadd_2xf32(p02,__builtin_epi_vfadd_2xf32(p12,p22,gvl),gvl);
						// Complete sum
						auto c = __builtin_epi_vfadd_2xf32(c0,__builtin_epi_vfadd_2xf32(c1,c2,gvl),gvl);
						// Store sum result
						__builtin_epi_vstore_2xf32(&O[i*(C-2)+j],c,gvl);
						j+=gvl-2;
					}
				}
				auto body_end = get_time::now();
				//for(int i = 0; i < (C-2)*(R-2); ++i) dst[i] = PositT(O[i]);
                PositT* dstptr = vFloat2Posit(O);
                dst = std::vector<PositT>(dstptr,dstptr+O.size());
				auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				//std::cout << prologue << "," << body << "," << epilogue << "\n";

			}
	      	
			  
			  void vConv3X(std::vector<PositT>& op1,std::vector<PositT>& filter,std::vector<PositT>& dst,long unsigned int R,long unsigned int C) {
				auto start = get_time::now();
				std::vector<float> O((R-2)*(C-2))/*,F(3*3),A(R*C)*/;
                float* A,*F;
                A = vPosit2Float(op1);
                F = vPosit2Float(filter);
				auto prologue_end = get_time::now();
				//Load & Duplicate filter values
				unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
				__epi_2xf32 f00 = __builtin_epi_vfmv_v_f_2xf32(F[0],all_gvl);
				__epi_2xf32 f01 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[1],all_gvl),0,all_gvl);
				__epi_2xf32 f02 =  __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[2],all_gvl),0,all_gvl),0,all_gvl);
				__epi_2xf32 f10 = __builtin_epi_vfmv_v_f_2xf32(F[3],all_gvl);
				__epi_2xf32 f11 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[4],all_gvl),0,all_gvl);
				__epi_2xf32 f12 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[5],all_gvl),0,all_gvl),0,all_gvl);
				__epi_2xf32 f20 = __builtin_epi_vfmv_v_f_2xf32(F[6],all_gvl);
				__epi_2xf32 f21 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[7],all_gvl),0,all_gvl);
				__epi_2xf32 f22 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[8],all_gvl),0,all_gvl),0,all_gvl);
				

				/* First iteration manually - start caching */
				__epi_2xf32 Ai1c,Ai2c;
				long unsigned int i=0;
				for(long unsigned j = 0; j < C-2;) {
						// Load 3 rows batch of matrix A
						long unsigned int gvl = __builtin_epi_vsetvl(C-j,__epi_e32,__epi_m1);
						auto Ai0 =  __builtin_epi_vload_2xf32(&A[i*C+j],gvl);
						Ai1c =  __builtin_epi_vload_2xf32(&A[(i+1)*C+j],gvl);
						Ai2c =  __builtin_epi_vload_2xf32(&A[(i+2)*C+j],gvl);
						// First row multiplication
						auto p00 = __builtin_epi_vfmul_2xf32(Ai0,f00,gvl);
						auto p01 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f01,gvl),0,gvl);
						auto p02 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f02,gvl),2,gvl);
						// Second row multiplication
                        auto p10 = __builtin_epi_vfmul_2xf32(Ai1c,f10,gvl);
						auto p11 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1c,f11,gvl),0,gvl);
						auto p12 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai1c,f12,gvl),2,gvl);
						// Third row multiplication
						auto p20 = __builtin_epi_vfmul_2xf32(Ai2c,f20,gvl);
						auto p21 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai2c,f21,gvl),0,gvl);
						auto p22 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai2c,f22,gvl),2,gvl);
						// Sum columns
						auto c0 = __builtin_epi_vfadd_2xf32(p00,__builtin_epi_vfadd_2xf32(p10,p20,gvl),gvl);
						auto c1 = __builtin_epi_vfadd_2xf32(p01,__builtin_epi_vfadd_2xf32(p11,p21,gvl),gvl);
						auto c2 = __builtin_epi_vfadd_2xf32(p02,__builtin_epi_vfadd_2xf32(p12,p22,gvl),gvl);
						// Complete sum
						auto c = __builtin_epi_vfadd_2xf32(c0,__builtin_epi_vfadd_2xf32(c1,c2,gvl),gvl);
						// Store sum result
						__builtin_epi_vstore_2xf32(&O[i*(C-2)+j],c,gvl);
						j+=gvl-2;
				}
				for(; i < R-2; ++i) {
					for(long unsigned j = 0; j < C-2;) {
						// Load 3 rows batch of matrix A
						long unsigned int gvl = __builtin_epi_vsetvl(C-j,__epi_e32,__epi_m1);
						auto Ai0 =  Ai1c;
						auto Ai1 =  Ai2c;
						auto Ai2 =  __builtin_epi_vload_2xf32(&A[(i+2)*C+j],gvl);
						// First row multiplication
						auto p00 = __builtin_epi_vfmul_2xf32(Ai0,f00,gvl);
						auto p01 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f01,gvl),0,gvl);
						auto p02 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f02,gvl),2,gvl);
						// Second row multiplication
                        auto p10 = __builtin_epi_vfmul_2xf32(Ai1,f10,gvl);
						auto p11 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f11,gvl),0,gvl);
						auto p12 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f12,gvl),2,gvl);
						// Third row multiplication
						auto p20 = __builtin_epi_vfmul_2xf32(Ai2,f20,gvl);
						auto p21 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f21,gvl),0,gvl);
						auto p22 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f22,gvl),2,gvl);
						// Sum columns
						auto c0 = __builtin_epi_vfadd_2xf32(p00,__builtin_epi_vfadd_2xf32(p10,p20,gvl),gvl);
						auto c1 = __builtin_epi_vfadd_2xf32(p01,__builtin_epi_vfadd_2xf32(p11,p21,gvl),gvl);
						auto c2 = __builtin_epi_vfadd_2xf32(p02,__builtin_epi_vfadd_2xf32(p12,p22,gvl),gvl);
						// Complete sum
						auto c = __builtin_epi_vfadd_2xf32(c0,__builtin_epi_vfadd_2xf32(c1,c2,gvl),gvl);
						// Store sum result
						__builtin_epi_vstore_2xf32(&O[i*(C-2)+j],c,gvl);
						j+=gvl-2;
						Ai1c = Ai2c;
						Ai2c = Ai2;
					}
				}
				auto body_end = get_time::now();
				//for(int i = 0; i < (C-2)*(R-2); ++i) dst[i] = PositT(O[i]);
                PositT* dstptr = vFloat2Posit(O);
                dst = std::vector<PositT>(dstptr,dstptr+O.size());
				auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				//std::cout << prologue << "," << body << "," << epilogue << "\n";

			}
			/*
				General matrix multiplication A*B=C.
				in:A is a MxK matrix
				in:B is a KxN matrix
				out:C is a MxN matrix
			*/
			void vGemm(const std::vector<PositT>& A,const std::vector<PositT>& B, std::vector<PositT>& C,uint32_t M,uint32_t K,uint32_t N) {
                //auto start = get_time::now();
				std::vector<float> Cf(M*N);
                float* Af,*Bf;
                Af = vPosit2Float(A);
                Bf = vPosit2Float(B);
				//auto prologue_end = get_time::now();                    
				for (unsigned long int i = 0; i < M; ++i)
				    // Loop on B cols
					for (long unsigned int j = 0; j < N; j +=  rvcntw()) {
					        // Prepare result row for matrix C
						unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
                                                __epi_2xf32 accum = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						unsigned long int gvl = __builtin_epi_vsetvl(N-j,__epi_e32,__epi_m1);
						#pragma clang loop unroll_count(32)
						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							auto A_i_k = __builtin_epi_vfmv_v_f_2xf32(Af[i * K + k],all_gvl);
							auto B_k_j = __builtin_epi_vload_2xf32(&Bf[k * N + j],gvl);

							// Add the contribution to row i-th of C
							accum = __builtin_epi_vfmacc_2xf32(accum, A_i_k, B_k_j,gvl);
						}
						__builtin_epi_vstore_2xf32(&Cf[i * N + j], accum,gvl);
				}
				//auto body_end = get_time::now();                
				//for(int i = 0; i < M*N;++i)
				//	C[i] = PositT(Cf[i]);
                PositT* dstptr = vFloat2Posit(Cf);
                C.assign(dstptr,dstptr+M*N);
                //auto epilogue_end = get_time::now();

				/*auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				std::cout << prologue << "," << body << "," << epilogue << "\n";*/
			}

			void vMaxPool3f(std::vector<PositT>& A,std::vector<PositT>& O,long unsigned int R,long unsigned int C) {
			}

			/* 2-operands Reducing operations */
			PositT vDot(std::vector<PositT>& op1,std::vector<PositT>& op2) {
			/*
				L3Prologue: Building float vector from Posit Vector naively for now
			*/
				auto start = get_time::now();
				/*std::vector<float> op1f(op1.size()),op2f(op2.size());
                for(int i = 0; i < op1.size(); ++i) {
                    op1f[i]=float(op1[i]);
                    op2f[i]=float(op2[i]);

                }*/
                float* op1f,*op2f;
				op1f = vPosit2Float(op1);
                op2f = vPosit2Float(op2);
				auto prologue_end = get_time::now();
				unsigned long i,N=op1.size();
				__epi_2xf32 vaccum = __builtin_epi_vfmv_v_f_2xf32(0.0f,__builtin_epi_vsetvlmax(__epi_e32,__epi_m1));
				for(i=0; i<N;) {
        				unsigned long gvl = __builtin_epi_vsetvl(N - i, __epi_e32, __epi_m1);
			        	__epi_2xf32 vop1 = __builtin_epi_vload_2xf32(&op1f[i],gvl);
			        	__epi_2xf32 vop2 = __builtin_epi_vload_2xf32(&op2f[i],gvl);
			        	__epi_2xf32 vprod = __builtin_epi_vfmul_2xf32(vop1,vop2,gvl);
			        	vaccum = __builtin_epi_vfredosum_2xf32(vprod,vaccum,gvl);
			        	i+=gvl;
				}
				float res =  __builtin_epi_vfmv_f_s_2xf32(vaccum);
				auto body_end = get_time::now();
				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				//std::cout << prologue << "," << body << "\n";

				return PositT(res);
			};

			PositT vDotX(std::vector<PositT>& op1,std::vector<PositT>& op2) {
			/*
				L3Prologue: Building float vector from Posit Vector naively for now
			*/
				auto start = get_time::now();
				/*std::vector<float> op1f(op1.size()),op2f(op2.size());
                for(int i = 0; i < op1.size(); ++i) {
                    op1f[i]=float(op1[i]);
                    op2f[i]=float(op2[i]);

                }*/
                float* op1f,*op2f;
				op1f = vPosit2Float(op1);
                op2f = vPosit2Float(op2);
				auto prologue_end = get_time::now();
				unsigned long i,N=op1.size();
				unsigned long gvl_f = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
				__epi_2xf32 vaccum = __builtin_epi_vfmv_v_f_2xf32(0.0f,gvl_f);
				__epi_2xf32 faccum = __builtin_epi_vfmv_v_f_2xf32(0.0f,gvl_f);
				for(i=0; i<N;) {
        				unsigned long gvl = __builtin_epi_vsetvl(N - i, __epi_e32, __epi_m1);
			        	__epi_2xf32 vop1 = __builtin_epi_vload_2xf32(&op1f[i],gvl);
			        	__epi_2xf32 vop2 = __builtin_epi_vload_2xf32(&op2f[i],gvl);
			        	vaccum = __builtin_epi_vfmacc_2xf32(vaccum,vop1,vop2,gvl);
			        	i+=gvl;
				}
				faccum = __builtin_epi_vfredosum_2xf32(vaccum,faccum,gvl_f);
				float res =  __builtin_epi_vfmv_f_s_2xf32(faccum);
				auto body_end = get_time::now();
				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				//std::cout << prologue << "," << body << "\n";

				return PositT(res);
			};



	void rvv_conv3x3(const PositT* op1,const PositT* filter,PositT* dst,long unsigned int R,long unsigned int C) {
				auto start = get_time::now();
				std::vector<float> O((R-2)*(C-2))/*,F(3*3),A(R*C)*/;
				//for(int i = 0; i < R*C;++i) A[i] = float(op1[i]);
                float* A,*F;
                A = vPosit2Float(op1,R*C);
                F = vPosit2Float(filter,3*3);
                //for(int i = 0; i < 3*3;++i) F[i] = float(filter[i]);
				auto prologue_end = get_time::now();
				//Load & Duplicate filter values
				unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
				__epi_2xf32 f00 = __builtin_epi_vfmv_v_f_2xf32(F[0],all_gvl);
				__epi_2xf32 f01 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[1],all_gvl),0,all_gvl);
				__epi_2xf32 f02 =  __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[2],all_gvl),0,all_gvl),0,all_gvl);
				__epi_2xf32 f10 = __builtin_epi_vfmv_v_f_2xf32(F[3],all_gvl);
				__epi_2xf32 f11 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[4],all_gvl),0,all_gvl);
				__epi_2xf32 f12 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[5],all_gvl),0,all_gvl),0,all_gvl);
				__epi_2xf32 f20 = __builtin_epi_vfmv_v_f_2xf32(F[6],all_gvl);
				__epi_2xf32 f21 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[7],all_gvl),0,all_gvl);
				__epi_2xf32 f22 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[8],all_gvl),0,all_gvl),0,all_gvl);
				
				for(long unsigned int i=0; i < R-2; ++i) {
					for(long unsigned j = 0; j < C-2;) {
						// Load 3 rows batch of matrix A
						long unsigned int gvl = __builtin_epi_vsetvl(C-2-j,__epi_e32,__epi_m1);
						__epi_2xf32 Ai0 =  __builtin_epi_vload_2xf32(&A[i*C+j],gvl);
						__epi_2xf32 Ai1 =  __builtin_epi_vload_2xf32(&A[(i+1)*C+j],gvl);
						__epi_2xf32 Ai2 =  __builtin_epi_vload_2xf32(&A[(i+2)*C+j],gvl);
						// First row multiplication
						__epi_2xf32 p00 = __builtin_epi_vfmul_2xf32(Ai0,f00,gvl);
						__epi_2xf32 p01 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f01,gvl),0,gvl);
						__epi_2xf32 p02 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f02,gvl),2,gvl);
						// Second row multiplication
                        __epi_2xf32 p10 = __builtin_epi_vfmul_2xf32(Ai1,f10,gvl);
						__epi_2xf32 p11 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f11,gvl),0,gvl);
						__epi_2xf32 p12 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f12,gvl),2,gvl);
						// Third row multiplication
						__epi_2xf32 p20 = __builtin_epi_vfmul_2xf32(Ai2,f20,gvl);
						__epi_2xf32 p21 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f21,gvl),0,gvl);
						__epi_2xf32 p22 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f22,gvl),2,gvl);
						// Sum columns
						__epi_2xf32 c0 = __builtin_epi_vfadd_2xf32(p00,__builtin_epi_vfadd_2xf32(p10,p20,gvl),gvl);
						__epi_2xf32 c1 = __builtin_epi_vfadd_2xf32(p01,__builtin_epi_vfadd_2xf32(p11,p21,gvl),gvl);
						__epi_2xf32 c2 = __builtin_epi_vfadd_2xf32(p02,__builtin_epi_vfadd_2xf32(p12,p22,gvl),gvl);
						// Complete sum
						__epi_2xf32 c = __builtin_epi_vfadd_2xf32(c0,__builtin_epi_vfadd_2xf32(c1,c2,gvl),gvl);
						// Store sum result
						__builtin_epi_vstore_2xf32(&O[i*(C-2)+j],c,gvl);
						j+=gvl;
					}
				}
				auto body_end = get_time::now();
				//for(int i = 0; i < (C-2)*(R-2); ++i) dst[i] = PositT(O[i]);
                vFloat2Posit(O.data(),(R-2)*(C-2),dst);
                //dst = std::vector<PositT>(dstptr,dstptr+O.size());
				auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				//std::cout << prologue << "," << body << "," << epilogue << "\n";

			}						


		void rvv_conv5x5(const PositT* op1,const PositT* filter,PositT* dst,long unsigned int R,long unsigned int C) {
				auto start = get_time::now();
				float* O = new float[((R-4)*(C-4))]/*,F(3*3),A(R*C)*/;
				//for(int i = 0; i < R*C;++i) A[i] = float(op1[i]);
                float* A,*F;
                A = vPosit2Float(op1,R*C);
                F = vPosit2Float(filter,5*5);
                //for(int i = 0; i < 3*3;++i) F[i] = float(filter[i]);
				auto prologue_end = get_time::now();
				//Load & Duplicate filter values
				unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
				__epi_2xf32 f00 = __builtin_epi_vfmv_v_f_2xf32(F[0],all_gvl);
				__epi_2xf32 f01 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[1],all_gvl),0,all_gvl);
				__epi_2xf32 f02 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[2],all_gvl),2,all_gvl);
				__epi_2xf32 f03 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[3],all_gvl),3,all_gvl);
				__epi_2xf32 f04 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[4],all_gvl),4,all_gvl);

				__epi_2xf32 f10 = __builtin_epi_vfmv_v_f_2xf32(F[5],all_gvl);
				__epi_2xf32 f11 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[6],all_gvl),0,all_gvl);
				__epi_2xf32 f12 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[7],all_gvl),2,all_gvl);
				__epi_2xf32 f13 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[8],all_gvl),3,all_gvl);
				__epi_2xf32 f14 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[9],all_gvl),4,all_gvl);

				__epi_2xf32 f20 = __builtin_epi_vfmv_v_f_2xf32(F[10],all_gvl);
				__epi_2xf32 f21 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[11],all_gvl),0,all_gvl);
				__epi_2xf32 f22 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[12],all_gvl),2,all_gvl);
				__epi_2xf32 f23 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[13],all_gvl),3,all_gvl);
				__epi_2xf32 f24 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[14],all_gvl),4,all_gvl);		

				__epi_2xf32 f30 = __builtin_epi_vfmv_v_f_2xf32(F[15],all_gvl);
				__epi_2xf32 f31 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[16],all_gvl),0,all_gvl);
				__epi_2xf32 f32 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[17],all_gvl),2,all_gvl);
				__epi_2xf32 f33 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[18],all_gvl),3,all_gvl);
				__epi_2xf32 f34 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[19],all_gvl),4,all_gvl);

				__epi_2xf32 f40 = __builtin_epi_vfmv_v_f_2xf32(F[20],all_gvl);
				__epi_2xf32 f41 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[21],all_gvl),0,all_gvl);
				__epi_2xf32 f42 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[22],all_gvl),2,all_gvl);
				__epi_2xf32 f43 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[23],all_gvl),3,all_gvl);
				__epi_2xf32 f44 =  __builtin_epi_vslideup_2xf32(__builtin_epi_vfmv_v_f_2xf32(F[24],all_gvl),4,all_gvl);	
				
				for(long unsigned int i=0; i < R-4; ++i) {
					for(long unsigned j = 0; j < C-4;) {
						// Load 5 rows batch of matrix A
						long unsigned int gvl = __builtin_epi_vsetvl(C-4-j,__epi_e32,__epi_m1);
						auto Ai0 =  __builtin_epi_vload_2xf32(&A[i*C+j],gvl);
						auto Ai1 =  __builtin_epi_vload_2xf32(&A[(i+1)*C+j],gvl);
						auto Ai2 =  __builtin_epi_vload_2xf32(&A[(i+2)*C+j],gvl);
						auto Ai3 =  __builtin_epi_vload_2xf32(&A[(i+3)*C+j],gvl);
						auto Ai4 =  __builtin_epi_vload_2xf32(&A[(i+4)*C+j],gvl);												
						// First row multiplication
						auto p00 = __builtin_epi_vfmul_2xf32(Ai0,f00,gvl);
						auto p01 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f01,gvl),0,gvl);
						auto p02 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f02,gvl),2,gvl);
						auto p03 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f03,gvl),3,gvl);
						auto p04 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f04,gvl),4,gvl);												
						
						// Second row multiplication
                        auto p10 = __builtin_epi_vfmul_2xf32(Ai1,f10,gvl);
						auto p11 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f11,gvl),0,gvl);
						auto p12 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f12,gvl),2,gvl);
						auto p13 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f13,gvl),3,gvl);
						auto p14 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f14,gvl),4,gvl);						
						
						// Third row multiplication
						auto p20 = __builtin_epi_vfmul_2xf32(Ai2,f20,gvl);
						auto p21 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f21,gvl),0,gvl);
						auto p22 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f22,gvl),2,gvl);
						auto p23 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f23,gvl),3,gvl);
						auto p24 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai2,f24,gvl),4,gvl);

						// Fourth row multiplication
						auto p30 = __builtin_epi_vfmul_2xf32(Ai3,f30,gvl);
						auto p31 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai3,f31,gvl),0,gvl);
						auto p32 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai3,f32,gvl),2,gvl);
						auto p33 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai3,f33,gvl),3,gvl);
						auto p34 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai3,f34,gvl),4,gvl);													

						// Fifth row multiplication
						auto p40 = __builtin_epi_vfmul_2xf32(Ai4,f40,gvl);
						auto p41 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai4,f41,gvl),0,gvl);
						auto p42 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai4,f42,gvl),2,gvl);
						auto p43 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai4,f43,gvl),3,gvl);
						auto p44 = __builtin_epi_vslidedown_2xf32(__builtin_epi_vfmul_2xf32(Ai4,f44,gvl),4,gvl);	

						// Sum columns
						auto c0 = __builtin_epi_vfadd_2xf32(p00,__builtin_epi_vfadd_2xf32(p10,__builtin_epi_vfadd_2xf32(p20,__builtin_epi_vfadd_2xf32(p30,p40,gvl),gvl),gvl),gvl);
						auto c1 = __builtin_epi_vfadd_2xf32(p01,__builtin_epi_vfadd_2xf32(p11,__builtin_epi_vfadd_2xf32(p21,__builtin_epi_vfadd_2xf32(p31,p41,gvl),gvl),gvl),gvl);
						auto c2 = __builtin_epi_vfadd_2xf32(p02,__builtin_epi_vfadd_2xf32(p12,__builtin_epi_vfadd_2xf32(p22,__builtin_epi_vfadd_2xf32(p32,p42,gvl),gvl),gvl),gvl);
						auto c3 = __builtin_epi_vfadd_2xf32(p03,__builtin_epi_vfadd_2xf32(p13,__builtin_epi_vfadd_2xf32(p23,__builtin_epi_vfadd_2xf32(p33,p43,gvl),gvl),gvl),gvl);
						auto c4 = __builtin_epi_vfadd_2xf32(p03,__builtin_epi_vfadd_2xf32(p13,__builtin_epi_vfadd_2xf32(p23,__builtin_epi_vfadd_2xf32(p33,p43,gvl),gvl),gvl),gvl);
						
						// Complete sum
						auto c = __builtin_epi_vfadd_2xf32(c0,__builtin_epi_vfadd_2xf32(c1,__builtin_epi_vfadd_2xf32(c2,__builtin_epi_vfadd_2xf32(c3,c4,gvl),gvl),gvl),gvl);
						// Store sum result

						__builtin_epi_vstore_2xf32(&O[i*(C-4)+j],c,gvl);
						j+=gvl;
					}
				}
				auto body_end = get_time::now();
				//for(int i = 0; i < (C-2)*(R-2); ++i) dst[i] = PositT(O[i]);
                vFloat2Posit(O,(R-4)*(C-4),dst);

                //dst = std::vector<PositT>(dstptr,dstptr+O.size());
				auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				//std::cout << prologue << "," << body << "," << epilogue << "\n";

			}						
	
			void rvvGemm(const PositT* A,const PositT* B,PositT* C,uint32_t M,uint32_t K,uint32_t N) {
                auto start = get_time::now();
				std::vector<float> Cf(M*N);
                float* Af,*Bf;
                Af = vPosit2Float(A,M*K);
                Bf = vPosit2Float(B,K*N);
				auto prologue_end = get_time::now();                    
				for (unsigned long int i = 0; i < M; ++i)
				    // Loop on B cols
					for (long unsigned int j = 0; j < N; j +=  rvcntw()) {
					        // Prepare result row for matrix C
						unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
                        __epi_2xf32 accum = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						unsigned long int gvl = __builtin_epi_vsetvl(N-j,__epi_e32,__epi_m1);
						#pragma clang loop unroll_count(32)
						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							auto A_i_k = __builtin_epi_vfmv_v_f_2xf32(Af[i * K + k],gvl);
							auto B_k_j = __builtin_epi_vload_2xf32(&Bf[k * N + j],gvl);

							// Add the contribution to row i-th of C
							accum = __builtin_epi_vfmacc_2xf32(accum, A_i_k, B_k_j,gvl);
						}
						__builtin_epi_vstore_2xf32(&Cf[i * N + j], accum,gvl);
				}
				auto body_end = get_time::now();                
				//for(int i = 0; i < M*N;++i)
				//	C[i] = PositT(Cf[i]);
                vFloat2Posit(Cf.data(),M*N,C);
                //C = std::vector<PositT>(dstptr,dstptr+Cf.size());
                auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				//std::cout << prologue << "," << body << "," << epilogue << "\n";
			}	

		void rvv_avepool2x2(const PositT* op1,std::vector<PositT>& dst,long unsigned int R,long unsigned int C) {
				auto start = get_time::now();
				std::vector<float> O((R/2)*(C/2))/*,F(3*3),A(R*C)*/;
				//for(int i = 0; i < R*C;++i) A[i] = float(op1[i]);
                float* A,*F;
                A = vPosit2Float(op1,R*C);
                //for(int i = 0; i < 3*3;++i) F[i] = float(filter[i]);
				auto prologue_end = get_time::now();
				//Load & Duplicate filter values
				unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
				__epi_2xf32 f00 = __builtin_epi_vfmv_v_f_2xf32(0.25f,all_gvl);
				__epi_2xf32 f01 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(0.25f,all_gvl),0,all_gvl);
				__epi_2xf32 f10 = __builtin_epi_vfmv_v_f_2xf32(0.25f,all_gvl);
				__epi_2xf32 f11 = __builtin_epi_vslide1up_2xf32(__builtin_epi_vfmv_v_f_2xf32(0.25f,all_gvl),0,all_gvl);

				
				for(long unsigned int i=0; i < R/2; ++i) {
					for(long unsigned j = 0; j < C/2;) {
						// Load 3 rows batch of matrix A
						long unsigned int gvl = __builtin_epi_vsetvl(C/2-j,__epi_e32,__epi_m1);
						auto Ai0 =  __builtin_epi_vload_2xf32(&A[i*C+j],gvl);
						auto Ai1 =  __builtin_epi_vload_2xf32(&A[(i+1)*C+j],gvl);
						// First row multiplication
						auto p00 = __builtin_epi_vfmul_2xf32(Ai0,f00,gvl);
						auto p01 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai0,f01,gvl),0,gvl);
						// Second row multiplication
                        auto p10 = __builtin_epi_vfmul_2xf32(Ai1,f10,gvl);
						auto p11 = __builtin_epi_vslide1down_2xf32(__builtin_epi_vfmul_2xf32(Ai1,f11,gvl),0,gvl);
						// Third row multiplication
					
						auto c0 = __builtin_epi_vfadd_2xf32(p00,p10,gvl);
						auto c1 = __builtin_epi_vfadd_2xf32(p01,p11,gvl);
						// Complete sum 
						auto c = __builtin_epi_vfadd_2xf32(c0,c1,gvl);

						// Reduction of factor 2 with slidedown
						auto c2 = __builtin_epi_vslide1down_2xf32(c,0,gvl);
						__builtin_epi_vfadd_2xf32(c,c2,gvl);	
						auto ind = 	__builtin_epi_vid_2xi32(gvl);
						auto even_mask =  __builtin_epi_cast_2xi1_2xi32(__builtin_epi_vrem_2xi32(ind,__builtin_epi_vmv_v_x_2xi32(2,all_gvl),gvl));
						auto even_index =  __builtin_epi_vdiv_2xi32_mask(ind,__builtin_epi_vid_2xi32(gvl),__builtin_epi_vmv_v_x_2xi32(2,all_gvl),even_mask,gvl);
						// Store sum result scattered with mask.
						__builtin_epi_vstore_indexed_2xf32(&O[i*(C/2)+j],c,even_index,gvl);
						j+=gvl;
					}
				}
				auto body_end = get_time::now();
				//for(int i = 0; i < (C-2)*(R-2); ++i) dst[i] = PositT(O[i]);
                PositT* dstptr = vFloat2Posit(O);
                dst.assign(dstptr,dstptr+R*C);
                //dst = std::vector<PositT>(dstptr,dstptr+O.size());
				auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				//std::cout << prologue << "," << body << "," << epilogue << "\n";

			}						

			/*
				General matrix multiplication A*B=C.
				in:A is a MxK matrix
				in:B is a KxN matrix
				out:C is a MxN matrix
			*/
			void rvvGemm4(const std::vector<PositT>& A,const std::vector<PositT>& B, std::vector<PositT>& C,uint32_t M,uint32_t K,uint32_t N) {
                auto start = get_time::now();
				std::vector<float> Cf(M*N);
                float* Af,*Bf;
                Af = vPosit2Float(A);
                Bf = vPosit2Float(B);
				auto prologue_end = get_time::now();                    
				for (unsigned long int m = 0; m < M-3; m+=4) {
					// Loop on B cols
					for (long unsigned int n = 0; n < N; n +=  rvcntw()) {
							// Prepare result row for matrix C
						unsigned long int gvl = __builtin_epi_vsetvl(N-n,__epi_e32,__epi_m1);
						unsigned long int all_gvl = gvl;
						__epi_2xf32 accum = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum1 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum2 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum3 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);

						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							__epi_2xf32 B_k_n = __builtin_epi_vload_2xf32(&Bf[k * N + n],gvl);

							__epi_2xf32 A_m_k = __builtin_epi_vfmv_v_f_2xf32(Af[m * K + k],all_gvl);
							__epi_2xf32 A_m1_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+1) * K + k],all_gvl);
							__epi_2xf32 A_m2_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+2) * K + k],all_gvl);
							__epi_2xf32 A_m3_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+3) * K + k],all_gvl);


							// Add the contribution to row i-th of C
							accum = __builtin_epi_vfmacc_2xf32(accum, A_m_k, B_k_n,gvl);
							accum1 = __builtin_epi_vfmacc_2xf32(accum1, A_m1_k, B_k_n,gvl);
							accum2 = __builtin_epi_vfmacc_2xf32(accum2, A_m2_k, B_k_n,gvl);
							accum3 = __builtin_epi_vfmacc_2xf32(accum3, A_m3_k, B_k_n,gvl);

						}		
						__builtin_epi_vstore_2xf32(&Cf[m * N + n], accum,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+1) * N + n], accum1,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+2) * N + n], accum2,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+3) * N + n], accum3,gvl);
					}
				}
				auto body_end = get_time::now();                
				//for(int i = 0; i < M*N;++i)
				//	C[i] = PositT(Cf[i]);
                PositT* dstptr = vFloat2Posit(Cf);
                C.assign(dstptr,dstptr+M*N);
                auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
				std::cout << prologue << "," << body << "," << epilogue << "\n";
			}

			void rvvGemm8(const std::vector<PositT>& A,const std::vector<PositT>& B, std::vector<PositT>& C,uint32_t M,uint32_t K,uint32_t N) {
                auto start = get_time::now();
				std::vector<float> Cf(M*N);
                float* Af,*Bf;
                Af = vPosit2Float(A);
                Bf = vPosit2Float(B);
				auto prologue_end = get_time::now();                    
				for (unsigned long int m = 0; m < M-7; m+=8) {
					// Loop on B cols
					for (long unsigned int n = 0; n < N; n +=  rvcntw()) {
							// Prepare result row for matrix C
						unsigned long int gvl = __builtin_epi_vsetvl(N-n,__epi_e32,__epi_m1);
						unsigned long int all_gvl = gvl;
						__epi_2xf32 accum = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum1 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum2 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum3 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum4 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum5 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum6 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						__epi_2xf32 accum7 = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							__epi_2xf32 B_k_n = __builtin_epi_vload_2xf32(&Bf[k * N + n],gvl);

							__epi_2xf32 A_m_k = __builtin_epi_vfmv_v_f_2xf32(Af[m * K + k],all_gvl);
							__epi_2xf32 A_m1_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+1) * K + k],all_gvl);
							__epi_2xf32 A_m2_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+2) * K + k],all_gvl);
							__epi_2xf32 A_m3_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+3) * K + k],all_gvl);
							__epi_2xf32 A_m4_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+4) * K + k],all_gvl);
							__epi_2xf32 A_m5_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+5) * K + k],all_gvl);
							__epi_2xf32 A_m6_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+6) * K + k],all_gvl);
							__epi_2xf32 A_m7_k = __builtin_epi_vfmv_v_f_2xf32(Af[(m+7) * K + k],all_gvl);


							// Add the contribution to row i-th of C
							accum = __builtin_epi_vfmacc_2xf32(accum, A_m_k, B_k_n,gvl);
							accum1 = __builtin_epi_vfmacc_2xf32(accum1, A_m1_k, B_k_n,gvl);
							accum2 = __builtin_epi_vfmacc_2xf32(accum2, A_m2_k, B_k_n,gvl);
							accum3 = __builtin_epi_vfmacc_2xf32(accum3, A_m3_k, B_k_n,gvl);
							accum4 = __builtin_epi_vfmacc_2xf32(accum4, A_m4_k, B_k_n,gvl);
							accum5 = __builtin_epi_vfmacc_2xf32(accum5, A_m5_k, B_k_n,gvl);
							accum6 = __builtin_epi_vfmacc_2xf32(accum6, A_m6_k, B_k_n,gvl);
							accum7 = __builtin_epi_vfmacc_2xf32(accum7, A_m7_k, B_k_n,gvl);


						}		
						__builtin_epi_vstore_2xf32(&Cf[m * N + n], accum,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+1) * N + n], accum1,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+2) * N + n], accum2,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+3) * N + n], accum3,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+4) * N + n], accum4,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+5) * N + n], accum5,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+6) * N + n], accum6,gvl);
						__builtin_epi_vstore_2xf32(&Cf[(m+7) * N + n], accum7,gvl);						
					}
				}
				auto body_end = get_time::now();                
				//for(int i = 0; i < M*N;++i)
				//	C[i] = PositT(Cf[i]);
                PositT* dstptr = vFloat2Posit(Cf);
                C.assign(dstptr,dstptr+M*N);
                auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
			}

			#ifdef _GEMMX_
			#ifndef _GEMM_BLOCK_
				#pragma message "block not set, defaulting to 8"
				#define _GEMM_BLOCK_ 8
			#endif
			void rvvGemmX(const std::vector<PositT>& A,const std::vector<PositT>& B, std::vector<PositT>& C,uint32_t M,uint32_t K,uint32_t N) {
                auto start = get_time::now();
				std::vector<float> Cf(M*N);
                float* Af,*Bf;
                Af = vPosit2Float(A);
                Bf = vPosit2Float(B);
				auto prologue_end = get_time::now();
				unsigned long int m;				                    
				for (m=0; m < M-(_GEMM_BLOCK_-1); m+=_GEMM_BLOCK_) {
					// Loop on B cols
					for (long unsigned int n = 0; n < N; n +=  rvcntw()) {
							// Prepare result row for matrix C
						unsigned long int gvl = __builtin_epi_vsetvl(N-n,__epi_e32,__epi_m1);
						unsigned long int all_gvl = gvl;
						VINITBLOCK(_GEMM_BLOCK_)
						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							__epi_2xf32 B_k_n = __builtin_epi_vload_2xf32(&Bf[k * N + n],gvl);
							VPREFETCHBLOCK(_GEMM_BLOCK_)
							VACCUMBLOCK(_GEMM_BLOCK_)
						}	
							VSTOREBLOCK(_GEMM_BLOCK_)		
					}
				}

				for (unsigned long int i = m; i < M; ++i) {
				    // Loop on B cols
					for (long unsigned int j = 0; j < N; j +=  rvcntw()) {
					        // Prepare result row for matrix C
						unsigned long int all_gvl = __builtin_epi_vsetvlmax(__epi_e32,__epi_m1);
                        __epi_2xf32 accum = __builtin_epi_vfmv_v_f_2xf32(0.0f,all_gvl);
						unsigned long int gvl = __builtin_epi_vsetvl(N-j,__epi_e32,__epi_m1);
						#pragma clang loop unroll_count(32)
						for (uint32_t k = 0; k < K; ++k) {
							// Duplicate element A[i,k] on all sve lane to be multiplied
							// with the correspondent element at k-th row for every column
							// of B
							auto A_i_k = __builtin_epi_vfmv_v_f_2xf32(Af[i * K + k],gvl);
							auto B_k_j = __builtin_epi_vload_2xf32(&Bf[k * N + j],gvl);

							// Add the contribution to row i-th of C
							accum = __builtin_epi_vfmacc_2xf32(accum, A_i_k, B_k_j,gvl);
						}
						__builtin_epi_vstore_2xf32(&Cf[i * N + j], accum,gvl);
					}
				}


				auto body_end = get_time::now();                
                PositT* dstptr = vFloat2Posit(Cf);
                C.assign(dstptr,dstptr+M*N);
                auto epilogue_end = get_time::now();

				auto prologue = std::chrono::duration_cast<ns>(prologue_end - start).count();
				auto body = std::chrono::duration_cast<ns>(body_end - prologue_end).count();
				auto epilogue = std::chrono::duration_cast<ns>(epilogue_end - body_end).count();
			}
			#endif



	

