// ~/c/MyClib/OrthoPoly.h    Generalized Laguerre Polynomials

#ifndef OrthoPoly_h
#define OrthoPoly_h 1

double Lna(double x,int n,int alpha); // low order explicit formulas
double Ln0(double x,int n); // low order explicit formulas, alpha=0
double Ln1(double x,int n); // low order explicit formulas, alpha=1
double Ln2(double x,int n); // low order explicit formulas, alpha=2
// double Ln3(double x,int n); // low order explicit formulas, alpha=3  not yet implemented

#endif
