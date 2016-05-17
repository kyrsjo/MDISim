// ~/c/MyClib/Chebyshev.h

#ifndef Chebyshev_h
#define Chebyshev_h 1

#include <complex>  // for root
#include <valarray> // for root

void chebft(double a, double b,double c[], int n, double (*func)(double));  // calculate Chebyshev coefficients for function func
// inline double chebev(double a, double b, double c[], int m, double x);  // use Chebyshev coefficients to approximate a function
void pade(valarray<double> cof_in,valarray<double>& cof_out,double& resid);

// for speed inline
inline double chebev(double a,double b,const double c[],int m,double x) // use the coefficients c to approx f(x)
// used in beschb in ~/c/MyClib/Bessel.C
// Fortran recipes 5.8 p. 187
// Chebyshev evaluation: c[0..m-1] is an array of Chebyshev coefficients,
// the first m elements of c output from chebft (which must have been called with the
// same a and b). The Chebyshev polynomial is evaluated at a point
// y = [x  (b + a)=2]=[(b  a)=2], and the result is returned as the function value.
// speed for n=13:  about 0.34 musec, inline version only 0.03 musec,   on PowerPC G4
{
  const double y=(2.0*x-a-b)/(b-a); // map to interval
  const double y2=2.0*y;
  //debug cout << "start of chebev for x=" << x << " y2=" << y2 << '\n';
  double d=0, dd=0;
  for (int j=m-1; j>0 ; j--) // Clenshaw's recurrence.
  {
    double sv=d;
    d=y2*d-dd+c[j];
    dd=sv;
    //debug cout << "now d=" << setw(10) << d << " j=" << j << '\n';
  }
  return y*d-dd+0.5*c[0]; // Last step is different.
}

// my complex extension 2/2009
valarray< complex<double> > GetComplexChebyshevCoefficients(double a, double b, int n, complex<double> (*func)(const complex<double>& z));
complex<double> ComplexChebyshevApprox(double a,double b,const valarray< complex<double> >& c,    complex<double> x);
complex<double> ComplexChebyshevApprox(double a,double b,const complex<double> c[],unsigned int m,complex<double> x);

#endif
