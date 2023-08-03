# Examples

In the [examples folder](https://github.com/federicorossifr/cppposit/tree/main/examples) you can find some ready-to-try examples that deepen on some of the functionalities of the cppposit library and possible integration with other libraries.

You can compile all the examples as follows:

```c++
cmake -B builddir
cmake --build builddir --target examples
```

You will find the executables as follows:

```bash
➜  cppposit git:(main) ✗ tree -L 1 builddir/examples 
builddir/examples
├── example_01_basic
├── example_02_deepinit
├── example_03_stl
├── example_04_eigen
├── example_05_fft
├── example_05_tabulatedposit
├── example_06_logging
├── example_06_pseudosig
├── example_07_pseudotanh
├── example_08_fused
├── example_98_backendhw
└── example_99_debugger
```



### Table of Contents

- [Basic](#basic)
- [Deep Initialization](#deep-initialization)
- [STL](#standard-template-library)
- [Eigen](#eigen-library)
- [FFT](#fast-fourier-transform)
- [Logging](#logging)
- [Pseudo Sigmoid](#pseudo-sigmoid)
- [Pseudo Tanh](#pseudo-tanh)
- [Fused Operations](#fused-operations)
- [HW Backend](#hardware-backend-example)

## Basic 

```c++
#include <iostream>
#include <vector>
using namespace std;
using namespace posit;
#include <posit.h> // include non-tabulated posits

int main() {
    Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs> One(1);
    Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs> Tiny(0.00000000000000003);
    auto Min = Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs>::from_sraw(1);
    auto ot = One - Tiny;
    auto eps = One - One.prev();
    printf("One val:\t %.23f\n", (double)One);
    printf("Tiny val:\t %.23f\n", (double)Tiny);
    printf("Epsilon:\t %.23f\n", (double)eps);
    printf("Min:\t %.64f\n", (double)Min);

    printf("One - tiny:\t %.23f\n", (double)(ot));
    printf("One - eps:\t %.23f\n", (double)(One - eps));

    printf("One previous:\t %.23f\n", (double)One.prev());

    std::cout << eps.v << " " << Tiny.v << std::endl;
}
```

## Deep initialization

```c++
#include <iostream>
#include <vector>
using namespace std;

#include <posit.h>
using namespace posit;
int main() {

    cout << "This demo shows the use of deep initialization of a posit" << endl;
	cout << "and the PositTrait class" << endl;

	using real = Posit<int8_t, 8, 0, uint16_t, PositSpec::WithNan>;
	    
	using PT = PositTrait<int8_t,8,0,PositSpec::WithNan>;
    
    // Let us create a posit, initialized to zero (POSIT_ZERO is not defined, being it equal to 0)
	real a(real::DeepInit(), 0);
	cout << dec << a << " -> " << oct << a << endl;

    // Let us create another posit, initialized to 1
	real b(real::DeepInit(), PT::POSIT_ONE);
	cout << dec << b << " -> " << oct << b << endl; 

    // Let us create another posit, initialized to two
	real c(real::DeepInit(), PT::POSIT_TWO);
	cout << dec << c << " -> " << oct << c << endl; 
    
    // Let us create another posit, initialized to MAXPOS
	real d(real::DeepInit(), PT::POSIT_MAXPOS);	
	cout << dec << d << " -> " << oct << d << endl; 

	// Let us create another posit, initialized to -1
	real e(real::DeepInit(), PT::POSIT_MONE);
	cout << dec << e << " -> " << oct << e << endl; 

    // Loop for printing all the positive posits
    real counter(real::DeepInit(), 0); // 
	for (int i = 0; i < 128; i++){
		cout << dec << i+1 << ": " << counter << " -> " << oct << counter << endl;
		counter = counter.next();
    }

	return 0;

}
```

## Standard Template Library

```c++
#include <iostream>
#include <vector>
using namespace std;

#include "../../include/posit.h" // include non-tabulated posits
using namespace posit;
int main() {

	//using real = Posit<int8_t, 8, 3, uint16_t, PositSpec::WithNan>;
    // using real = Posit<int16_t, 16, 1, uint32_t, PositSpec::WithNan>;
	using real = Posit<int32_t, 32, 2, uint64_t, PositSpec::WithNan>;
		
	// Let us define a vector of posits of length DIM
	const int DIM = 5;
	vector <real> v(DIM);

	v[0] = int(1);
	v[1] = int(2);
	v[2] = 0.52f;
	v[3] = 0.53f;
	v[4] = 0.50003f;

    // Let us print it on screen
	cout << endl<< endl << "Vector v: " << endl;
	cout << endl << "[ ";
	for (int i = 0; i < DIM; i++)
	   cout << v[i] <<' ';
	cout << "]" << endl;


    // Let us define a rectangular matrix of size DIM x (DIM-1)
	vector<vector<real>> M(DIM, std::vector<real>(DIM - 1));

    // Let us define a rectangular matrix of size DIM x (DIM-1)
	cout << endl<< endl << "Matrix M: " << endl;
	for(int i = 0; i < DIM; i++){
		for (int j = 0; j < DIM - 1; j++) {
			M[i][j] = (i + j) + 0.2;
			cout << float(M[i][j]) << ' ';
		}
		cout << endl;
	}	

	return 0;
}
```

## Eigen Library

```c++
#include<iostream>
#include "../../include/posit.h"
#include<cstdlib>
#include<Eigen/Dense>
using namespace std;
int main() {

	
	// Step1. Create a 3-by-3 random matrix, filled with posits
	using myFloat = posit::Posit<int32_t, 32, 0, uint32_t, posit::PositSpec::WithNan>;
    Eigen::Matrix<myFloat,3,3> m;
    for(int i = 0; i < 3; ++i) {
    	for(int j = 0; j < 3; ++j) {
    		m(i,j) = myFloat((double(rand()%200))/100);
    		std::cout << m(i,j) << " " ;
    	}
    	std::cout << std::endl;
    }
    std::cout << std::endl;
	// Step2. compute its inverse
    Eigen::Matrix<myFloat,3,3> minv = m.inverse();
	// Step3. print it on screen
    for(int i = 0; i < 3; ++i) {
    	for(int j = 0; j < 3; ++j) {
    		std::cout << minv(i,j) << " " ;
    	}
    	std::cout << std::endl;
    }    
    std::cout << std::endl;
	// Step4. check wether the product of the original matrix and its inverse gives the identity matrix
    Eigen::Matrix<myFloat,3,3> prod = m*minv;
    for(int i = 0; i < 3; ++i) {
    	for(int j = 0; j < 3; ++j) {
    		std::cout << prod(i,j) << " " ;
    	}
    	std::cout << std::endl;
    }    
	return 0;
}
```

## Fast Fourier Transform

```c++
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
```

## Logging

```c++
#include <iostream>
#include <cmath>
#include "../../include/posit.h"
using namespace std;
using namespace posit;

using myreal = Posit<int16_t, 8, 0, uint16_t, PositSpec::WithNan>;

/**
 * The posit class has a built-in value logger to trace  
 * the values that are assigned to the class during 
 * the execution of a given portion of code.
**/
int main() {
	myreal a;
	// 1. Enable the logger at the beginning of the portion of code to be traced
	myreal::_logger.setLogLevel(LogLevel::ALL);
	for(int i = 0; i < 100; ++i) {
		a = myreal((double(i)/100));
	}

	// 2. Complete tracing by saving the traced values
	myreal::_logger.save("a_logged.txt");
}
```

## Pseudo-sigmoid

```c++
#include <iostream>
#include <cmath>
#include <posit.h>
using namespace std;
using namespace posit;
using Xa = Posit<int16_t, 16, 0, uint16_t, PositSpec::WithNan>;
typedef Xa myfl;


myfl sigmoid(myfl x) {
	double a = double(x);
	return 1/(1+std::exp(-a));
}


int main() {
	myfl a[] = {myfl(0.90625f)
	 ,myfl(0.2f)
	 ,myfl(0.3f)
	 ,myfl(0.4f)
	 ,myfl(0.43f)};
	cout << "Number" << "   " << "Pseudosigmoid" << "   " << "Sigmoid  " << "   " << "Difference" << "\n";
	for(int i = 0; i < 5; ++i)
		cout << a[i] << "   " << double(a[i].pseudosigmoid()) << "   " << double(sigmoid(a[i])) << "   " << double(sigmoid(a[i]))-double(a[i].pseudosigmoid())<< "\n";
	for(int i = 0; i < 5; ++i)
		cout << -a[i] << "   " << double((-a[i]).pseudosigmoid()) << "   " << double(sigmoid(-a[i])) << "   " << double(sigmoid(-a[i]))-double((-a[i]).pseudosigmoid())<< "\n";

	return 0;
}
```

## Pseudo-tanh

```c++
#include <iostream>
#include <cmath>
#include <posit.h>
using namespace std;
using namespace posit;
using Xa = Posit<int32_t,8,0,uint64_t,PositSpec::WithNan>;
typedef Xa myfl;

template <class T>
T tanh(T x) {
	return std::tanh(double(x));
}


int main() {
	myfl a[] = {0.90625f,
	 			0.32312f,
	 			0.333543f,
	 			0.2132144f,
	 			0.5213f,
	 			-0.3f,
	 			-0.2f,
	 			-0.1f,
	 			0.43f};
	for(auto x: a)
		cout << x << "  " << double(x.pseudotanh()) << "  " << tanh(x) << std::endl;
	return 0;
	
}
```

## Fused Operations

```c++
#include <iostream>
#include <vector>
using namespace std;
#include <posit.h> // include non-tabulated posits
using namespace posit;

int main() {
	
	using ptype = Posit<int8_t, 8, 0, uint32_t, PositSpec::WithInfs>;
    using paccum = Posit<int16_t,16,0,uint32_t, PositSpec::WithNan>;

	std::vector<ptype> a{ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f)};      // constucts a posit, using a float literal
	std::vector<ptype> b{ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f),ptype(1.f)};      // constucts a posit, using a float literal
    paccum res(0);

    auto pres = posit::math::fma_dot(a.begin(),b.begin(),a.size(),res);

    std::cout << pres << std::endl;
	return 0;

}
```

## Hardware backend example

```c++
#include <iostream>
#include <vector>
using namespace std;
#include <posit.h>
using namespace posit;
#include <iomanip>


int main() {
    using PPEMU = Posit<int32_t, 32, 2, uint32_t, PositSpec::WithInfs>;
    using PP = Posit<int32_t, 32, 2, BackendXPosit<int32_t,PPEMU>, PositSpec::WithInfs>;
    PP OneHalf(0.5f);
    std::cout << OneHalf << std::endl;
    return 0;
}
```
