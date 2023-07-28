#include "posit.h"
#include <iostream>
#include <chrono>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock;
using namespace posit;

using X32 = Posit<int32_t, 32 , 0, uint32_t, PositSpec::WithNan>;
using X16 = Posit<int16_t, 16 , 0, uint16_t, PositSpec::WithNan>;
using X8 = Posit<int8_t, 8 , 0, uint8_t, PositSpec::WithNan>;
using X4 = Posit<int16_t,4,0,uint16_t,PositSpec::WithNan>;

template <class T>
T sigmoid(T x)
{
    
    return T( 1 / (1 + std::exp(float(-x))));
}

template <typename T>
T elu(T x) {
	return (x > T(0)) ? x: T(std::exp(float(x))-1);
}

template <class T>
T tanh(T x)
{
    return T(std::tanh(float(x)));
}

long exactTime() {
   long elapsed = 0;
   for(int i = -128; i < 128; ++i) {
       X8 a = X8::from_sraw(i);
       auto start = get_time::now();
       tanh<X8>(a);
       auto end = get_time::now();
       elapsed+=std::chrono::duration_cast<ns>(end - start).count();
  }
  return elapsed;
}

long pseudoTime() {
   long elapsed = 0;
   for(int i = -128; i < 128; ++i) {
       X8 a = X8::from_sraw(i);
       auto start = get_time::now();
       a.fastELU();
       auto end = get_time::now();
       elapsed+=std::chrono::duration_cast<ns>(end - start).count();                                                                                                                                                 }
  return elapsed;
}

void exactVals() {
   long elapsed = 0;
   for(int i = -128; i <= 127; i+=1) {
       X8 a = X8::from_sraw(i);
       float v = float(elu(a));
       std::cout << float(a) << "," << v << "\n";
  }
}

void pseudoVals() {
   long elapsed = 0;
   for(int i = -128; i <= 127; i+=1) {
       X8 a = X8::from_sraw(i);
       float v = float(a.fastELU());
       std::cout << float(a) << "," << v << "\n";
    }
}

int main() {
   pseudoVals();
}
