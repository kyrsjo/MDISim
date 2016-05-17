// ~/c/MyClib/fft.h  by H. Burkhardt
#ifndef Fft_h
#define Fft_h 1

#include <string> // needed for rootcint ?
class FftTransform
  { public:
    std::ostream *theFilOut; // pointer to the ofstream for output
    int nn; // number of points, for pow2=10  nn=1024
    int ip; // index of bin with peak
    double asym;   // asymmetry from left and right to maximum
    double xpeakinter; // interpolated maximum using asymmetry
    double PeakAmp;     // amplitude from peak and +/- 1
    //old complex<double> *data; // data array complex data length is nn
    valarray< complex<double> > data; // used from 12/2003
    int WindowType; // last WindowType applied via ApplyWindowOnData

    // methods:
    FftTransform(std::ostream *theFilOut,const int PowerOfTwo);      // constructor
    virtual ~FftTransform();    // destructor
    virtual double inline xc(const int i) { return ( (0.5+(double)i) / (double) nn); } // centre of bin
    virtual double inline xi(const int i) { return (double) i/ (double) nn; } // x binning, only for x-pos of peak, centre of bin
    virtual void DoFft(const int isign); // replaces data by its discrete Fourier transform, if isign is input as 1, backwards by  isign = -1
    virtual void FindPeak(double skipFrac); // index of peak
    virtual void FindPeakCloseTo(double Qnom,double DelQ);  // look for a peak close to Qnom
    virtual double MultiPeakFinderCloseTo(double Qnom,double DelQ,const char *matchstr,std::ostream *fftout);    // look for several peaks close to Qnom
    virtual void PrintPeak();   // print result of FindPeaks
    virtual void ApplyWindowOnData(int WindowType); // apply WindowType 0-3
    virtual void ApplyHannWindowOnData();
    virtual void ApplyWelchWindowOnData();
    virtual void ApplyBartlettWindowOnData();
    virtual void WriteData(std::ostream *fftout,const char *matchstr); // for paw and my ntuple
    virtual void WriteDataFile(const char *fname); // useful for gnuplot
  private:
    virtual void PeakFrom_ip(); // given ip, set PeakAmp, asym, xpeakinter
  };

const int WindowTypes=4; // currently 4 WindowTypes defined
const string WindowTypeStr[WindowTypes]={"Rect","Bartlett","Welch","Hann"};
int  WindowType(string TypeStr); // WindowType 0-3 from WindowTypeStr Rect, Bartlett..
#endif
