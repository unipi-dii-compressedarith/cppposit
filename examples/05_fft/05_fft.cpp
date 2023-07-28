#include <iostream>

#include "posit.h"

typedef posit::Posit<int32_t,32,2,uint32_t,posit::PositSpec::WithNan> real;

constexpr unsigned int k = 8; /* for 2^8 = 256 points */
constexpr unsigned N =  (1 << k); /* N-point FFT */

typedef struct{
  real Re;
  real Im;
}
complex;

#ifndef PI
#define PI real(3.14159265358979323846264338327950288)
#endif
void fft(complex * signal, const unsigned int n, complex * tmp) {
  if (n > 1) {
    /* return if n =< 0 */
    unsigned int k, m;
    complex z, w, *vo, *ve;
    ve = tmp;
    vo = tmp + n / 2;
    for (k = 0; k < n / 2; k++) {
      ve[k] = signal[2 * k];
      vo[k] = signal[2 * k + 1];
    }
    fft(ve, n / 2, signal); // FFT on even-indexed elements of signal[]
    fft(vo, n / 2, signal); // FFT on odd-indexed elements of signal[]
    for (m = 0; m < n / 2; m++) {
      w.Re = (std::cos((real(2) * PI * real(m) / real(n))));
      w.Im = -(std::sin((real(2) * PI * real(m) / real(n))));
      z.Re = w.Re * vo[m].Re - w.Im * vo[m].Im; /* Re(w*vo[m]) */
      z.Im = w.Re * vo[m].Im + w.Im * vo[m].Re; /* Im(w*vo[m]) */
      signal[m].Re = ve[m].Re + z.Re;
      signal[m].Im = ve[m].Im + z.Im;
      signal[m + n / 2].Re = ve[m].Re - z.Re;
      signal[m + n / 2].Im = ve[m].Im - z.Im;
    }
  }
  return;
}

// Program entry point.
int main() {
  complex signal[N], tmp[N];
  unsigned int k;
  /* Fill signal[] as the sum of two pure sine signals of known frequency */
  for (k = 0; k < N; k++) {
     signal[k].Re = real(0.125) * std::cos((real(2) * PI * real(k) / real(N))) + real(0.5) * std::cos((real(2) * PI * real(10*k) / real(N)));
     signal[k].Im = real(0.125) * std::sin((real(2) * PI * real(k) / real(N))) + real(0.5) * std::cos((real(2) * PI * real(10*k) / real(N)));
  }
  /* Printing the generated signal (for reading it in Matlab)*/
  std::cout << "signal = [\n";
  for (k = 0; k < N; k++) {
	 std::cout << signal[k].Re << "+" << signal[k].Im <<"i" << std::endl;
  }
  std::cout << "]; \n\n";
  std::cout << "Computing the FFT...\n";
  fft(signal, N, tmp); // Perform FFT, signal will have the result.
  std::cout << "Printing the module of the obtained FFT coefficients:\n\nabs_FFT = [\n";
  for (k = 0; k < N; k++) {
	 std::cout << std::sqrt((signal[k].Re*signal[k].Re+signal[k].Im*signal[k].Im)) << std::endl;
  }
  std::cout << "];\n\nDone. Check whether the second module is 32 and the 11th is 128\n";
  std::cout << "2th: " << std::sqrt((signal[1].Re*signal[1].Re+signal[1].Im*signal[1].Im)) << std::endl;
  std::cout << "11th: " << std::sqrt((signal[10].Re*signal[10].Re+signal[10].Im*signal[10].Im)) << std::endl;
  std::cout << "(all the others must be zero, or close to zero)\n\n";
  
  return 0;
}