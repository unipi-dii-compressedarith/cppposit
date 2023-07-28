#include "stdio.h"
#include "cposit.h"
int main() {
    posit8_t* p = posit8_fromFloat(0.125);
    posit16_t* p1 = posit16_fromFloat(0.5f);

    printf("Hello %f\n",posit16_toFloat(p1));
}