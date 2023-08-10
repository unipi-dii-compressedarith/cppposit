  
# cppposit

[![CMake](https://github.com/federicorossifr/cppposit/actions/workflows/cmake.yml/badge.svg)](https://github.com/federicorossifr/cppposit/actions/workflows/cmake.yml)

[![codecov](https://codecov.io/gh/federicorossifr/cppposit/branch/main/graph/badge.svg?token=K2GHRUL9XV)](https://codecov.io/gh/federicorossifr/cppposit)

[Emanuele Ruffaldi](https://github.com/eruffaldi)
[Federico Rossi](https://github.com/federicorossifr)

Implementation of John L. Gustafson Unum Type III aka Posits using C++ Templates.
Initial inspiration was the existing C++ https://github.com/libcg/bfp but then we diverged a lot with several features as detailed below.






## Features
Overall:

- Posit's total bits from 4 to 64
- storage in larger holding type (always signed in) e.g 12bit in 16bits
- any valid exponent bits size
- support of variant with NaN and signed Infinity (see below)
- implementation of operations expressed over 4 possible levels (see below)
- constexpr whenever possible
- integration with Eigen
- C++17

The library supports many variants of the Posits as controlled by the template parameters:

```c++
template <class T,int totalbits, int esbits, class BackendType, PositSpec specs>
```

- this is a Posit with `totalbits` size and it is stored in an storage type of class `T` whose size is at least `totalbits`.
- `totalbits` can go from 2 to 63
- `esbits` is the maximum exponent size in bits. Values from 0 to totalbits-1 are supported
- `BackendType` is the data type that holds the fractional part during operations (more info about backends [here](https://federicorossifr.github.io/cppposit/docBackends.html) )
- `PositSpec` is a type used to interpret the value of the represented by the integer $-2^{(nbits-1)}$

```c++
enum PositSpec {
	WithNone, /// never consider special numbers. The top value is never present
	WithNan, /// one top Nan (or Nar using posit spec terminology)
	WithNanInfs, /// one top Nan and two signed Infinites (for more float compatibility)
	WithInfs /// one top Nan and two signed Infinites (for more float compatibility)
}; 
```


## Documentation

API and documentation is available [here](https://federicorossifr.github.io/cppposit/).

## Bibliography

cppposit has been extensively used and expanded by the following works:


1.  Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio  
    **Fast Approximations of Activation Functions in Deep Neural Networks when using Posit Arithmetic**  
    Sensors, vol. 20(5), 2020  
    
2.  Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio; Dupont de Dinechin, Benoit  
    **Novel Arithmetics in Deep Neural Networks Signal Processing for Autonomous Driving: Challenges and Opportunities**  
    IEEE Signal Processing Magazine, vol. 38(1), pp. 97-110, 2021  
    
3.  Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio  
    **A Lightweight Posit Processing Unit for RISC-V Processors in Deep Neural Network Applications**  
    IEEE Transactions on Emerging Topics in Computing, vol. (), pp. 1-1, 2021  

4.  _Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio_  
    **A Fast Approximation of the Hyperbolic Tangent When Using Posit Numbers and Its Application to Deep Neural Networks**  
    In _Applications in Electronics Pervading Industry, Environment and Society_, pp. 213--221, 2020  
    
5.  _Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio_  
    **Novel Arithmetics to Accelerate Machine Learning Classifiers in Autonomous Driving Applications**  
    In _2019 26th IEEE International Conference on Electronics, Circuits and Systems (ICECS)_, pp. 779-782, 2019  
    
6.  _Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio_  
    **A Novel Posit-based Fast Approximation of ELU Activation Function for Deep Neural Networks**  
    In _2020 IEEE International Conference on Smart Computing (SMARTCOMP)_, pp. 244-246, 2020  
    
7.  _Cococcioni, Marco; Rossi, Federico; Ruffaldi, Emanuele; Saponara, Sergio_  
    **Small Reals Representations for Deep Learning at the Edge: A Comparison**  
    In _Next Generation Arithmetic_, pp. 117--133, 2022  
    
8.  _Neves, Nuno; Crespo, Luis; Rossi, Federico; Cococcioni, Marco; Saponara, Sergio; Kuehn, Martin; Krueger, Jens; Tomas, Pedro; Roma, Nuno_  
    **An FPGA-based platform to evaluate Posit arithmetic in next-generation processors**  
    In _ISC High Performance (Poster)_, 2023  
