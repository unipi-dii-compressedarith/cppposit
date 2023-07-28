#include "posit.h"
#include "cposit.h"

struct _posit8 {
    void* _p8;
};

posit8_t* posit8_fromFloat(float f) {
    posit8_t* cp;
    P8* p;

    cp = (typeof(cp))malloc(sizeof(*cp));
    p  = new P8(f);
    cp->_p8 = p;
    return cp;
}

posit8_t* posit8_fromSRaw(uint8_t u8) {
    posit8_t* cp;
    P8* p;

    cp = (typeof(cp))malloc(sizeof(*cp));
    p  = new P8();
    p->v = P8::from_sraw(u8).v;
    cp->_p8 = p;
    return cp;    
}

float posit8_toFloat(posit8_t* _p8) {
    P8* p;
    if(_p8 == NULL) return nanf("");
    p = static_cast<P8*>(_p8->_p8);
    return (float)(*p);
}

uint8_t posit8_toRaw(posit8_t* _p8) {
    P8* p;
    if(_p8 == NULL) return 0xFF;
    p = static_cast<P8*>(_p8->_p8);
    return p->v;
}









struct _posit16 {
    void* _p16;
};

posit16_t* posit16_fromFloat(float f) {
    posit16_t* cp;
    P16_0* p;

    cp = (typeof(cp))malloc(sizeof(*cp));
    p  = new P16_0(f);
    cp->_p16 = p;
    return cp;
}

posit16_t* posit16_fromSRaw(uint16_t u16) {
    posit16_t* cp;
    P16_0* p;

    cp = (typeof(cp))malloc(sizeof(*cp));
    p  = new P16_0();
    p->v = P16_0::from_sraw(u16).v;
    cp->_p16 = p;
    return cp;    
}

float posit16_toFloat(posit16_t* _p16) {
    P16_0* p;
    if(_p16 == NULL) return nanf("");
    p = static_cast<P16_0*>(_p16->_p16);
    return (float)(*p);
}

uint16_t posit16_toRaw(posit16_t* _p16) {
    P16_0* p;
    if(_p16 == NULL) return 0xFF;
    p = static_cast<P16_0*>(_p16->_p16);
    return p->v;
}
