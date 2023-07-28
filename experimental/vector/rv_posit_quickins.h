// __fcvt_f32_p8: Float to Posit8,0 conversion
// fcvt.s.p8 fcvt.s.p8 rd rs1 24..20=0x2 31..25=0x60 14..12=0x0 6..2=0x2 1..0=0x3
int __fcvt_f32_p8 (float a) {
	register float p1 asm ("fa0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x0\n"
		".set opf2,0x2\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"f"(p1), "[result]"(result));
	return result;
}



// __fcvt_f32_p16_0: Float to Posit16,0 conversion
// fcvt.s.p16.0 fcvt.s.p16.0 rd rs1 24..20=0x3 31..25=0x60 14..12=0x0 6..2=0x2 1..0=0x3
int __fcvt_f32_p16_0 (float a) {
	register float p1 asm ("fa0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x0\n"
		".set opf2,0x3\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"f"(p1), "[result]"(result));
	return result;
}



// __fcvt_f32_p16_1: Float to Posit16,1 conversion
// fcvt.s.p16.1 fcvt.s.p16.1 rd rs1 24..20=0x3 31..25=0x60 14..12=0x2 6..2=0x2 1..0=0x3
int __fcvt_f32_p16_1 (float a) {
	register float p1 asm ("fa0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x2\n"
		".set opf2,0x3\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"f"(p1), "[result]"(result));
	return result;
}



// __fcvt_p8_f32: Posit8,0 to float conversion
// fcvt.p8.s fcvt.p8.s rd rs1 24..20=0x2 31..25=0x68 14..12=0x0 6..2=0x2 1..0=0x3
float __fcvt_p8_f32 (int a) {
	register int p1 asm ("a0") = a;
	register float result asm ("fa1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x0\n"
		".set opf2,0x2\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=f"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_0_f32: Posit16,0 to float conversion
// fcvt.p16.0.s fcvt.p16.0.s rd rs1 24..20=0x3 31..25=0x68 14..12=0x0 6..2=0x2 1..0=0x3
float __fcvt_p16_0_f32 (int a) {
	register int p1 asm ("a0") = a;
	register float result asm ("fa1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x0\n"
		".set opf2,0x3\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=f"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_1_f32: Posit16,1 to float conversion
// fcvt.p16.1.s fcvt.p16.1.s rd rs1 24..20=0x3 31..25=0x68 14..12=0x2 6..2=0x2 1..0=0x3
float __fcvt_p16_1_f32 (int a) {
	register int p1 asm ("a0") = a;
	register float result asm ("fa1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x2\n"
		".set opf2,0x3\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=f"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_fx16_p8: Fixed16 to Posit8,0 conversion
// fxcvt.h.p8 fxcvt.h.p8 rd rs1 24..20=0x2 31..25=0x60 14..12=0x1 6..2=0x2 1..0=0x3
int __fcvt_fx16_p8 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x1\n"
		".set opf2,0x2\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_fx32_p16_0: Fixed32 to Posit16,0 conversion
// fxcvt.w.p16.0 fxcvt.w.p16.0 rd rs1 24..20=0x3 31..25=0x60 14..12=0x1 6..2=0x2 1..0=0x3
int __fcvt_fx32_p16_0 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x1\n"
		".set opf2,0x3\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_fx64_p16_1: Fixed64 to Posit16,1 conversion
// fxcvt.l.p16.1 fxcvt.l.p16.1 rd rs1 24..20=0x3 31..25=0x60 14..12=0x3 6..2=0x2 1..0=0x3
int __fcvt_fx64_p16_1 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x3\n"
		".set opf2,0x3\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p8_fx16: Posit8,0 to Fixed16 conversion
// fxcvt.p8.h fxcvt.p8.h rd rs1 24..20=0x2 31..25=0x68 14..12=0x1 6..2=0x2 1..0=0x3
int __fcvt_p8_fx16 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x1\n"
		".set opf2,0x2\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_0_fx32: Posit16,0 to Fixed32 conversion
// fxcvt.p16.0.w fxcvt.p16.0.w rd rs1 24..20=0x3 31..25=0x68 14..12=0x1 6..2=0x2 1..0=0x3
int __fcvt_p16_0_fx32 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x1\n"
		".set opf2,0x3\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_1_fx64: Posit16,1 to Fixed64 conversion
// fxcvt.p16.1.l fxcvt.p16.1.l rd rs1 24..20=0x3 31..25=0x68 14..12=0x3 6..2=0x2 1..0=0x3
int __fcvt_p16_1_fx64 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x3\n"
		".set opf2,0x3\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p8_p16_0: Posit8,0 to Posit16,0 conversion
// fcvt.p8.p16.0 fcvt.p8.p16.0 rd rs1 24..20=0x2 31..25=0x60 14..12=0x4 6..2=0x2 1..0=0x3
int __fcvt_p8_p16_0 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x4\n"
		".set opf2,0x2\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_0_p8: Posit16,0 to Posit8,0 conversion
// fcvt.p16.0.p8 fcvt.p16.0.p8 rd rs1 24..20=0x3 31..25=0x60 14..12=0x4 6..2=0x2 1..0=0x3
int __fcvt_p16_0_p8 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x4\n"
		".set opf2,0x3\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p8_p16_1: Posit8,0 to Posit16,1 conversion
// fcvt.p8.p16.1 fcvt.p8.p16.1 rd rs1 24..20=0x3 31..25=0x60 14..12=0x6 6..2=0x2 1..0=0x3
int __fcvt_p8_p16_1 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x6\n"
		".set opf2,0x3\n"
		".set opf3,0x60\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_1_p8: Posit16,1 to Posit8,0 conversion
// fcvt.p16.1.p8 fcvt.p16.1.p8 rd rs1 24..20=0x2 31..25=0x68 14..12=0x5 6..2=0x2 1..0=0x3
int __fcvt_p16_1_p8 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x5\n"
		".set opf2,0x2\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_0_p16_1: Posit16,0 to Posit16,1 conversion
// fcvt.p16.0.p16.1 fcvt.p16.0.p16.1 rd rs1 24..20=0x3 31..25=0x68 14..12=0x5 6..2=0x2 1..0=0x3
int __fcvt_p16_0_p16_1 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x5\n"
		".set opf2,0x3\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



// __fcvt_p16_1_p16_0: Posit16,1 to Posi16,0 conversion
// fcvt.p16.1.p16.0 fcvt.p16.1.p16.0 rd rs1 24..20=0x3 31..25=0x68 14..12=0x7 6..2=0x2 1..0=0x3
int __fcvt_p16_1_p16_0 (int a) {
	register int p1 asm ("a0") = a;
	register int result asm ("a1");
	__asm__ volatile(
		""
		".set rfs0,8\n"
		".set rfs1,9\n"
		".set rfs2,18\n"
		".set rfa0,10\n"
		".set rfa1,11\n"
		".set rfa2,12\n"
		".set rs0,8\n"
		".set rs1,9\n"
		".set rs2,18\n"
		".set ra0,10\n"
		".set ra1,11\n"
		".set ra2,12\n"
		".set op,0xb\n"
		".set opf1,0x7\n"
		".set opf2,0x3\n"
		".set opf3,0x68\n"
		".byte op|((r%[result]&1) <<7),((r%[result]>>1)&0xF)|(opf1<<4)|((r%1&1) <<7),((opf2&0xF) << 4) | ((r%1>>1)&0xF),((opf2>>4)&0x1)|(opf3<<1)"
		: [result] "=r"(result)
		:"r"(p1), "[result]"(result));
	return result;
}



