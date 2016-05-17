// ~/c/MyClib/ExpInt.h

#ifndef ExpInt_h
#define ExpInt_h 1

double ExpIntn(int n, double x);   // Evaluates the exponential integral En(x).
double ExpInt(double x);            // ExpInt from CERNLIB C337, special case n=1 ExpInt(x)=E1(x)=-Ei(-x)
std::complex<double> Ein(std::complex<double> z); // complex Ein, from CERNLIB C388
std::complex<double> E1( std::complex<double> z); // complex E1 from complex Ein, for real arg same as ExpInt(x), ExpIntn(1,x)
#endif
