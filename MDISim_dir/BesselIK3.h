// ~/c/MyClib/BesselIK3.h

#ifndef BesselIK3_h
#define BesselIK3_h 1

double besir3(double x,int nu,bool exp); // calc I nu/3  nu=-2,-1,1,2  if exp true *exp(-x)
double beskr3(double x,int nu,bool lex); // calc K nu/3  nu=-2,-1,1,2  if lex true *exp(x)
double besk53(double x); // K 5/3,   used in synchrotron radiation

// Airy functions ai,ai',bi,bi', derived from Bessel for nu=+/- 1/3
void airy(double x,double *ai, double *bi, double *aip, double *bip);
#endif
