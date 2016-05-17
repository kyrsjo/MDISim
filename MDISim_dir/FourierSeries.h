// ~/c/MyClib/FourierSeries.h
#ifndef FourierSeries_h
#define FourierSeries_h 1

//old #include <iostream> // for rootcint
//old #include <fstream>

class FourierSeries
{
public:
  ostream *theFilOut;   // pointer to the ofstream for output
  int NumPoin;
  int NumHarm;
  int NumCalled;        // counts how many times MakeFourierSeries was called
  const double *Ypoint; // function sampled over one period, pointer to array Ypoint[NumPoin]
  double *FourierA;     // fourier coefficients calculated -
  double *FourierB;     // - here from Ypoint
  double *YpointBack;   // function calculated back from spectrum

  FourierSeries(ostream *theFilOut,const int &NumPoin,const int &NumHarm);    // constructor
  virtual ~FourierSeries(){};   // (empty( destructor
  virtual void MakeFourierSeries(const double *Ypoint);
  virtual void FuncFromSpectrum();
  virtual void WriteSpectrum();
  virtual void WriteFuncBack();
};
#endif
