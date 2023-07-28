#include<iostream>
#include "../../include/posit.h"
#include<cstdlib>
#include<Eigen/Dense>
using namespace std;
int main() {

	
	// Step1. Create a 3-by-3 random matrix, filled with posits
	using myFloat = posit::Posit<int32_t, 32, 0, uint32_t, posit::PositSpec::WithNan>;
    Eigen::Matrix<myFloat,3,3> m;
    for(int i = 0; i < 3; ++i) {
    	for(int j = 0; j < 3; ++j) {
    		m(i,j) = myFloat((double(rand()%200))/100);
    		std::cout << m(i,j) << " " ;
    	}
    	std::cout << std::endl;
    }
    std::cout << std::endl;
	// Step2. compute its inverse
    Eigen::Matrix<myFloat,3,3> minv = m.inverse();
	// Step3. print it on screen
    for(int i = 0; i < 3; ++i) {
    	for(int j = 0; j < 3; ++j) {
    		std::cout << minv(i,j) << " " ;
    	}
    	std::cout << std::endl;
    }    
    std::cout << std::endl;
	// Step4. check wether the product of the original matrix and its inverse gives the identity matrix
    Eigen::Matrix<myFloat,3,3> prod = m*minv;
    for(int i = 0; i < 3; ++i) {
    	for(int j = 0; j < 3; ++j) {
    		std::cout << prod(i,j) << " " ;
    	}
    	std::cout << std::endl;
    }    
	return 0;
}
