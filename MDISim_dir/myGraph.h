// ~/c/MDISim/myGraph.h
#ifndef myGraph_h
#include <TGraph.h>
TGraph* MyBeamGraph(const unsigned int n,unsigned int linecolor,const valarray<double>& zvec,const valarray<double>& x_2);
TGraph* MyApertureGraph(const unsigned int n,unsigned int linecolor,const vector<double>& zvec,const vector<double>& x_2,const vector<double>& aper);
TGraph* MyTrackGraph(const unsigned int nn,const valarray<double>& svec,const valarray<double>& xy,unsigned int linecolor,unsigned int verbose);
#define myGraph_h 1
#endif

