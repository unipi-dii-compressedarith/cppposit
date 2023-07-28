#pragma once
#include "stdint.h" 
#ifdef __cplusplus
#include "posit.h"
extern "C" {
#endif
struct _posit8;
struct _posit16;
typedef struct _posit8 posit8_t;
typedef struct _posit16 posit16_t;

posit8_t* posit8_fromFloat(float f);
posit8_t* posit8_fromSRaw(uint8_t u8);
posit16_t* posit16_fromFloat(float f);
posit16_t* posit16_fromSRaw(uint16_t u16);

float posit8_toFloat(posit8_t* _p8);
float posit16_toFloat(posit16_t* _p16); 
uint8_t posit8_toRaw(posit8_t* _p8);
uint16_t posit16_toRaw(posit16_t* _p16);

#ifdef __cplusplus
}
#endif
