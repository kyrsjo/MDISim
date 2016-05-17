// ~/c/MyClib/Numer.h

#ifndef Numer_h
#define Numer_h 1

// function prototypes
double Ranqd2(); // recipies page 275, very fast, quick and dirty random generator
double Ran1();
double Ran2();
double RanNorm();
void RanNorm2(double& r1, double& r2); // as RanNorm but giving two numbers in a single call
double SynGenC(double xmin);    // in SynRad.C
double PoissonGen(double mean);

complex<double> gamma(complex<double> z); // CERNLIB C305 gamma function of complex argument
int     iFactorial(int n);  // n!
double   Factorial(int n);
int     iFactorial2(int n); // n!!
int     iBinomial(int n,int k);
double   Binomial(int n,int k);

double gaussi(double (*func)(double),double xmin,double xmax,int np); // Gauss integration
double SynFacLow(double xmin);      // SynRad fraction of photons below cutoff
double SynFacHigh(double xmin);     // SynRad fraction of photons above cutoff
double PowSynFacLow(double xmin);   // SynRad power below cutoff (in units of ecrit)
double PowSynFacHigh(double xmin);  // SynRad power above cutoff (in units of ecrit)
double Knu(double x,double Nu,int ipow);    // approx for the integral x to infty of Bessel function Knu, for Nu=5/3 this is same as SynRadC
double PowIntKnu(double x,double Nu);   // for Nu=5/3 integrated power spectrum from x to infinity
double Inverse(double (*func)(double),double y,double x1,double x2); // finds x=func^-1(y) the inverse or root of func in the range x1<x<x2 with bisection

#endif
