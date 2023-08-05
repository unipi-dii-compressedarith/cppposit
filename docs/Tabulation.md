# Tabulation

Posit arithmetic calculations can be pre-computed and used at compile time to avoid full emulation of arithmetic ops. You can generate the tables for the 4 basic arithmetic opearations using the scripts in `<cppposit_root>/scripts`:

```bash
python scripts/generatePositCppTables.py -n <nbits> -e <esbits> -o <output_file>
```

Source in `<cppposit_root>/examples/09_tabulated/09_tabulated.cpp` shows an example use of tabulation. 

```c++
#include <iostream>
#include <posit.h>
using namespace posit;

template <class T,int nbits, int esbits>
struct PositTable;
#include "ptab4_0.h"

int main() {
    using PPEMU = Posit<int32_t, 4, 0, uint32_t, PositSpec::WithInfs>;
    using PP = Posit<int8_t, 4, 0, TabulatedBackend<int8_t,PPEMU,PositTable<int8_t,4,0>>, PositSpec::WithInfs>;
    PP a(0.5f), b(0.25f);
    std::cout << a+b << std::endl;
    std::cout << a*b << std::endl;
    std::cout << a-b << std::endl;
    std::cout << a/b << std::endl;
    return 0;
}
```

Here we declare a posit type `PP` with a total of 4 bits. We declare a `PositTable` templetized struct and in the header file included with `#include "ptab4_0.h"` we specify it for the needed posit configuration, using the python generator script.

```c++
template<> struct PositTable<int8_t,4,0>{
    static constexpr int8_t mul[16][16] = {...};
    static constexpr int8_t add[16][16] = {...};
    static constexpr int8_t sub[16][16] = {...};
    static constexpr int8_t div[16][16] = {...};
};
```

For simplicity you can create a cmake target to automate generation of tables when needed. An example can be found in `<cppposit_root>/examples/09_tabulated/CMakeLists.txt`.

```cmake
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ptab4_0.h
  COMMAND python ${PROJECT_SOURCE_DIR}/scripts/generatePositCppTables.py 
    -n 4 -e 0
    -o ${CMAKE_CURRENT_SOURCE_DIR}/ptab4_0.h
  DEPENDS ${PROJECT_SOURCE_DIR}/scripts/generatePositCppTables.py
)

add_executable(example_09_tabulated ${CMAKE_CURRENT_SOURCE_DIR}/09_tabulated.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ptab4_0.h)
```

When compiling the example_09_tabulated target you will see the python script being executed to generate the target header file:

```bash
➜  cppposit/builddir git:(main) ✗ make example_09_tabulated
[ 33%] Generating ../../../examples/09_tabulated/ptab4_0.h
Generating source header for table: n=4, esbits=0
[ 66%] Building CXX object examples/09_tabulated/CMakeFiles/example_09_tabulated.dir/09_tabulated.cpp.o
[100%] Linking CXX executable example_09_tabulated
[100%] Built target example_09_tabulated
```