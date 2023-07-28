#pragma once
#include "posit.h"
#include <vector>
namespace posit {
	namespace vectorized {
		
		template<class PositT>
		class PositVectorizedFrontend {

		public:
			virtual ~PositVectorizedFrontend() {}
			/* 2-operands Non reducing operations */
			virtual void vAdd(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) = 0;
			virtual void vMul(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) = 0;
			virtual void vMuladd(std::vector<PositT> op1,std::vector<PositT> op2,std::vector<PositT>& dst) = 0;			

			/* Gemm operations */
			virtual void vGemm(const std::vector<PositT>& A,const std::vector<PositT>& B, std::vector<PositT>& C,uint32_t M,uint32_t K,uint32_t N) = 0;
			virtual void sv_conv3x3(std::vector<PositT>& op1,std::vector<PositT>& filter,std::vector<PositT>& dst,long unsigned int R,long unsigned int C) = 0;
			/* 2-operands Reducing operations */
			virtual PositT vDot(std::vector<PositT>& op1,std::vector<PositT>& op2) = 0;


			/* 1-operand L1 Operations */
			virtual void vFastOneComp(const std::vector<PositT>& op,std::vector<PositT>& dst) = 0;
			virtual void vFastTwice(const std::vector<PositT>& op,std::vector<PositT>& dst) = 0;
			virtual void vFastInv(const std::vector<PositT>& op,std::vector<PositT>& dst) = 0;
			virtual void vFastELU(const std::vector<PositT>& op,std::vector<PositT>& dst) = 0;
			virtual void vFastSigmoid(const std::vector<PositT>& op,std::vector<PositT>& dst) = 0;
			virtual void vFastTanh(const std::vector<PositT>& op,std::vector<PositT>& dst) = 0;

		};




	}
}
