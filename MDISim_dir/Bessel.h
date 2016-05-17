// ~/c/MyClib/Bessel.h      Bessel functions

#ifndef Bessel_h
#define Bessel_h 1

// various asymptotic formulas, fast and simple
double bessjAsym(double n,double z);    //  Bessel function Jnu(z), approx for very high z >> nu
double bessyAsym(double n,double z);    //  Bessel function Ynu(z), approx for very high z >> nu
double bessjNuAsym(double nu,double z); // for high nu  >  |z|
double bessjXltNu(double nu,double z);  // Jnu asymptotic for x < nu
double bessyXltNu(double nu,double z);  // Ynu asymptotic for x < nu
double bessjZleNu(double nu,double z);  // Jnu  approx for large nu and x close to Nu, uses K1/3
double bessjpZleNu(double nu,double z); // J'nu approx for large nu and x close to Nu, uses K1/3
double bessjMax(int nu,double x); // upper bound Abram 9.1.63 p. 362

//   see also double Knu(double x,double Nu,int ipow)  in Numer.C for Knu function and its integrals
// first the fast (only 6-7 digit precise) functions of integer order

double bessj0(double x);        // Bessel function J0(x) for any real x.
double bessj1(double x);        // Bessel function J1(x) for any real x.
double bessj(int n, double x);  // Bessel function Jn (x) for any real x and n>=2

double bessy0(double x);        // Bessel function Y0(x) for positive x.
double bessy1(double x);        // Bessel function Y1(x) for positive x.
double bessy(int n, double x);  // Bessel function Yn(x) for positive x and n>=2

double bessi0(double x);        // modified Bessel function I0(x) for any real x.
double bessi1(double x);        // modified Bessel function I1(x) for any real x.
double bessi(int n, double x);  // modified Bessel function In(x) for any real x and n>=2

double bessk0(double x);        // modified Bessel function K0(x) for positive real x.
double bessk1(double x);        // modified Bessel function K1(x) for positive real x.
double bessk(int n, double x);  // modified Bessel function Kn(x) for positive x and n>=2

// now the more precise multipurpose functions

// Bessel functions J,J', Y,Y' for fractional nu
void bessjy(double x, double nu, double *besJ, double *besY, double *besJp, double *besYp);
// modified Bessel functions I,I', K,K' for fractional nu
void bessik(double x, double nu, double *besI, double *besK, double *besIp, double *besKp);

// utilities used in calculation of fractional functions
void beschb(double x, double *gam1, double *gam2, double *gampl, double *gammi);
// double chebev(double a, double b, double c[], int m, double x); // now inline in Chebyshev.h

void BesJYzeros(double nu,const int nzeros,int ifunc,double eps,double *res);
// ifunc  1-4:   J, Y, J', Y'
// bessel func order nu
// return nzeros  in array res

// next is to find  Chebyshev coefficients and should better go somewhere else
void chebft(double a, double b, double c[], int n, double (*func)(double));
#endif
