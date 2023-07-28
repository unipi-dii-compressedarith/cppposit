Tabulated posits
=================================================

The cppPosit class supports both non-tabulated 
and tabulated posits.

Tabulation can only be done for small posits, like
posit 8, 10, 12 and 14 total bits.

In particular, tabulated posit8 are very useful
in k-nearest neighbours, deep neural networks (DNN), 
decision trees, random forests, etc.

An interesting feature of tabulated posits is that
they are fast to be evaluated both on a generic CPU
without the Posit Processing Unit (PPU) and on 
a micro-controller without PPU nor the FPU (Floating Point Unit).

Thus tabulated posit allow to:
- fast train a DNN on existing CPUs;
- deployment of the trained DNN on a micro-controller.

In the latter case, no error is added during the cross-compilation.
Of course, the micro-controller must have a lot of RAM, to store
the table for +/- (* and / are not tabulated, thanks to the use
of tabulated logarithm and exponential functions).
