// ~/c/MyClib/lomb.h

#ifndef lomb_h
#define lomb_h 1

void ApplyHannWindowOnData(double data[],const int nn);
void ApplyWelchWindowOnData(double data[],const int nn);
void ApplyBartlettWindowOnData(double data[],const int nn);
void LombPeriodogram(const double x[],const double y[],int nn,int ofac,double hifac,double px[],double py[],int np,int *nout,int *jmax,double *prob);
#endif
