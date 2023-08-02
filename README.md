  
# cppposit

[Emanuele Ruffaldi](https://github.com/eruffaldi)
[Federico Rossi](https://github.com/federicorossifr)

Implementation of John L. Gustafson Unum Type III aka Posits using C++ Templates.

Initial inspiration was the existing C++ [](Implementation of Gustafoson Unum Type III aka Posits using C++ Templates.

Initial inspiration was the existing C++ https://github.com/libcg/bfp but then we diverged a lot with several features as detailed below.






## Features
Overall:

- Posit's total bits from 4 to 64
- storage in larger holfing typre (always signed in) e.g 12bit in 16bits
- any valid exponent bits size
- support of variant with NaN and signed Infinity (see below)
- implementation of operations expressed over 4 possible levels (see below)
- constexpr whenever possible
- integration with Eigen

The library supports many variants of the Posits as controlled by the template parameters:

```c++
template <class T,int totalbits, int esbits, class BackendType, PositSpec specs>
```

- this is a Posit with `totalbits` size and it is stored in an storage type of class `T` whose size is at least `totalbits`.
- `totalbits` can go from 2 to 63
- `esbits` is the maximum exponent size in bits. Values from 0 to totalbits-1 are supported
- `BackendType` is the data type that holds the fractional part during operations
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
