Comparing the true sigmoid function with its pseudoversion
===========================================================

For posit numbers with zero bit of exponents, their sigmoid
function can be approximated by manipulating the binary
representation of the posit number.
This avoids the computation of the exponential function
and the inversion of a posit.

The method for computing the pseudo sigmoid function is described
in Gustafson and Yanemoto (2017) "Beating floating point at 
its own game: Posit arithmetic", Supercomputing Frontiers and 
Innovations, Vol. 4, No. 2.

## Test with non-tabulated posits
In file main_notab.cpp we compare the exact sigmoid function
with its pseudo counterpart, using non-tabulated posits.

## Test with tabulated posits
In file main_notab.cpp we compare the exact sigmoid function
with its pseudo counterpart, using non-tabulated posits.

