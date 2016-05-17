// ~/c/MyClib/hist.h

#ifndef hist_h
#define hist_h 1

#include <valarray> // for root
#include <iostream> // cout

class hist1
{
  // histogram is something approximate with finite bin width.  Could also allow int for contents
  // idea is also to interface with plotting with final resolution   - used in BeamTrack
public:
  string Name;
  double xmin,xmax;
  double ymin,ymax; // only used for plotting with Higz, could go there
  valarray <double> axis;
  valarray <double> contents;
  double underflow,overflow;
  hist1(const string& Name,const unsigned int nbins,const double xmin,const double xmax,const unsigned int verbose=0); // constructor
  virtual ~hist1(){}; // (empty) destructor
  virtual void Fill(double x);
  inline virtual void SetYmax(const double ymax){this->ymax=ymax;}
  virtual void Reset();
  virtual void PrintSummary(ostream &FilOut=std::cout);
  virtual void Print(ostream &FilOut=std::cout);
  virtual double Mean();              // calculate mean value from the histogram   and rms and number of entries
  virtual double Rms(){return rms;};  // calculate rms value from histogram, call after mean which does the calculation
  virtual double EntriesTotal() { return entriesTotal; };     // total number of entries, including over and undeflow
  virtual double EntriesInside(){ return entriesInside; };
  virtual double GetBinWid(){ return BinWid; };
protected:
  unsigned int verbose;
  double BinWid,HalfBinWid;
  double entriesTotal,entriesInside,mean,rms;
};

class hist2
{
public:
  // simple 2 dim histogram   similar to matrix
  string Name;
  unsigned int nbin1,nbin2;
  double xmin1,xmax1;
  double xmin2,xmax2;
  valarray <double> axis1,axis2;
  valarray <double> contents;
  unsigned int underflow1,overflow1;
  unsigned int underflow2,overflow2;
  hist2(const string& Name,const unsigned int nbin1,const double xmin1,const double xmax1,const unsigned int nbin2,const double xmin2,const double xmax2,const unsigned int verbose=0); // constructor
  virtual ~hist2(){}; // (empty) destructor
  void Fill(double x1,double x2);
  virtual void Print(ostream &FilOut);
  virtual void PrintSummary(ostream &FilOut);
  // protected:
  unsigned int verbose;
  double BinWid1,HalfBinWid1;
  // double mean1,rms1;
  double BinWid2,HalfBinWid2;
  // double mean2,rms2;
  double entriesTotal,entriesInside;
};
#endif
