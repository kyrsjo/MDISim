// ~/c/MyClib/MyTGCocoa.h   H. Burkhardt 5/2013

#ifndef MyTGCocoa_h
#define MyTGCocoa_h 1

#ifndef hist_h
#include "hist.h" // class hist1; // defined in ~/c/MyClib/hist.h
#endif


#include "TRint.h"		     // http://root.cern.ch/root/html/TRint.html			http://root.cern.ch/root/html/src/TRint.cxx.html
#include "TCanvas.h"         // http://root.cern.ch/root/html/TCanvas.html          http://root.cern.ch/root/html/src/TCanvas.cxx.html

#include "TVirtualX.h"       // http://root.cern.ch/root/html/TVirtualX.html        http://root.cern.ch/root/html/src/TVirtualX.cxx.html
#include "TSystem.h"         // http://root.cern.ch/root/html/TSystem.html          http://root.cern.ch/root/html/src/TSystem.cxx.html

#include "TPolyLine.h"       // http://root.cern.ch/root/html/TPolyLine.html        http://root.cern.ch/root/html/src/TPolyLine.cxx.html

// #include "TGCocoa.h"     // http://root.cern.ch/root/html/TGCocoa.html


#define Use_TRint


class MyTGCocoa_hist1 : public hist1 // hist1 with plotting using root TGCocoa
{
public:
  bool PlotAsHist; // if true plot as hist, otherwise just connect contents with line at each bin center
#ifdef Use_TRint
  TRint* theApp;     // for session with root commands
#else
  TApplication* theApp; // just for window  -- currently crash,   need to define canvas or pad ?
#endif

  MyTGCocoa_hist1(string Name,unsigned int nbins,double xmin,double xmax); // constructor
  virtual ~MyTGCocoa_hist1(){};    // (empty) destructor
  virtual void Plot();
  void ClosePlot();

private:
  unsigned int verbose;
  // int calls_to_plot;  // not yet used
  double width,height;
};
#endif
