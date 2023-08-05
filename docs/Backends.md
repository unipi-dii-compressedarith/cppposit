# Backends

cppposit supports several different backends for arithmetic computations.
The implemented backends are located in `<cppposit_root>/include/backends/`

## Softback

The softback backend emulates posit arithmetic operations at bit level, resembling an actual hardware implementation in software.

File: `<cppposit_root>/include/backends/softback.hpp`

Instantiation:
```c++
using SoftBack = uint32_t; // soft backend with a 32-bit storage
using P32_2 = Posit<int32_t, 32, 2, SoftBack, PositSpec::WithInfs>;
```

## Fixed

Fixed-point backend uses a `fixedtrait`` specified fixed-point integer backend to perform arithmetic operations between posits. 

File: `<cppposit_root>/include/backends/fixed.hpp`

Instantiation:
```c++
// 32-bit fixed point backend
using FixedBack = BackendFixed<fixedtrait<int32_t,32>,int32_t>; 
using P16_0 = Posit<int16_t, 16, 0, FixedBack, PositSpec::WithInfs>;
```

Note that the size of the fixed trait must be enough to accomodate the dynamic range of the posit configuration.

## Float/Double

Float/Double backend uses the native floating point types (if available) to perform posit arithmetic computations. Double can be used to accomodate larger posit configurations (e.g. 32-bit posits).

File: `<cppposit_root>/include/backends/float.hpp`

Instantiation:
```c++
using FloatBack = float; 
using DoubleBack = double; 
using P16_2 = Posit<int16_t, 16, 2, FloatBack, PositSpec::WithInfs>;
using P32_2 = Posit<int32_t, 32, 2, DoubleBack, PositSpec::WithInfs>;
```

## Generalized hardware backend

The Posit class is also instrumented to bypass emulations if proper hardware support is available. All the backends that extends the base are treated like hardware backends by the library.

File: `<cppposit_root>/include/backends/hwbaseback.hpp`

```c++
struct HwBaseBackend {};
``` 

You can provide specialized hardware interface customising the demo backend in `<cppposit_root>/include/backends/xpositback.hpp`. Since it can happens that not all the functionalities are implemented in hardware, the hardware backend template arguments needs a posit type configured with full emulation.

In the following example we are using the demo backend `BackendXPosit`. In the demo we imagine that we only have instructions for arithmetic operations, while the conversions between posits and other native types are performed via emulation using the `PPEMU` type.

```c++
using PPEMU = Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs>;
using PP = Posit<int32_t, 32, 2, BackendXPosit<int32_t,PPEMU>, PositSpec::WithInfs>;
PP OneHalf(0.5f);
std::cout << OneHalf << std::endl;
return 0;
```

## Tabulated Backend

TabulatedBackend provides look-up table backend for posit computations. More details are available in [Tabulation](docTabulation.html)

