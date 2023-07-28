#pragma once
#include <immintrin.h>

// if has AVX2
// _mm256_abs_epi16 _mm256_abs_epi32 _mm256_abs_epi8

inline __m256i relu_epi32(__m256i a)
{
return _mm256_max_epi32(a, _mm256_setzero_si256());
}

inline __m256i relu_epi16(__m256i a)
{
return _mm256_max_epi16(a, _mm256_setzero_si256());
}

inline __m256i relu_epi8(__m256i a)
{
return _mm256_max_epi8(a, _mm256_setzero_si256());
}
