## Prerequisites

cppposit is a header-only library, no external libraries are needed for using it. You need a C++ compiler that supports at least the `-std=c++17` option.

To run all the examples you may need to install the eigen library:

```bash
sudo apt install libgeigen3-dev
```

## Compilation

The minimum example to use the cppposit library is the following:

```c++
// file: test.cpp
#include <posit.h>

int main() {
    using Posit = posit::Posit<int32_t, 32, 2, uint32_t, posit::PositSpec::WithInfs>
    Posit a(0.5f);
    Posit b(1.5f);
    Posit c = a + b;
    return 0;
}
```

To compile the example code in the `test.cpp` file:

```bash
g++ -I<path_to_cppposit_root>/include -std=c++17 test.cpp
```