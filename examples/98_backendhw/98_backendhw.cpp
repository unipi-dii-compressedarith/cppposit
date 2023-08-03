

#include <iostream>
#include <vector>
using namespace std;
#include <posit.h>
using namespace posit;
#include <iomanip>


int main() {
    using PPEMU = Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs>;
    using PP = Posit<int32_t, 32, 2, BackendXPosit<int32_t,PPEMU>, PositSpec::WithInfs>;
    PP One(0.5f);
    std::cout << One << std::endl;
    return 0;
}
