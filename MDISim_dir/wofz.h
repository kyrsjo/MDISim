// ~/c/MyClib/wofz.h

#ifndef wofz_h
#define wofz_h 1

//old #include <complex> // only needed for rootcint

inline int idnint(double x)    // nearest whole integer, here int would be probably also fine       for FORTRAN IDNINT
{
  if(x>=0)  return (int)(x+.5);
  else return (int)(x-.5);
}

std::complex<double> wofz(const std::complex<double>& z);
std::complex<double> wofzSmallZ(std::complex<double> z);
std::complex<double> wofzBigZ(std::complex<double> z);

std::complex<double> wofzBigZ3(std::complex<double> z);

float humlik(const float x, const float y);
void wofz_hum(float x,float y,double& k,double& l);
#endif
