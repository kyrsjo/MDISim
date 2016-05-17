// ~/c/MyRoot/MyRoot.h

#ifndef MyRoot_h
#define MyRoot_h 1

#include <TVector.h>
#include <TTimeStamp.h>      // http://root.cern.ch/root/html/TTimeStamp.html       http://root.cern.ch/root/html/src/TTimeStamp.cxx.html
#include <TH2F.h>
#include <TGraph.h>
#include <TGeoMatrix.h>
#include <TEveStraightLineSet.h>

#include "Vec3.h"

#include "hist.h"  // hist1

inline string MyDaTime(double time) { TTimeStamp StartTime(time,0); return string(StartTime.AsString("cl")).substr(0,19); }  // 2011-08-23 22:59:00
string MyDaTime2(double time);                                                                                               // 2011-08-23 22:59:00.123
inline string MyTime(double time)   { TTimeStamp StartTime(time,0); return string(StartTime.AsString("cl")).substr(11,8); }  //            22:59:00
string MyTime2(double time);                                                                   // time with fractions of seconds   like    22:59:00.123

// from vector and valarray  to TVectorD,  useful for drawing in root
inline TVectorD to_TVectorD(const vector<double>& inp)   { const unsigned int n=inp.size();          TVectorD out(n);         for(unsigned int i=0;i<n;++i) out[i]=inp[i]; return out; }
inline TVectorD to_TVectorD(const valarray<double>& inp) { const unsigned int n=inp.size();          TVectorD out(n);         for(unsigned int i=0;i<n;++i) out[i]=inp[i]; return out; }

// from TVectorD to valarray or vector
inline valarray<double> to_valarray(const TVectorD& inp) { const unsigned int n=inp.GetNoElements(); valarray<double> out(n); for(unsigned int i=0;i<n;++i) out[i]=inp[i]; return out; }
inline vector<double>     to_vector(const TVectorD& inp) { const unsigned int n=inp.GetNoElements(); vector<double>   out(n); for(unsigned int i=0;i<n;++i) out[i]=inp[i]; return out; }

void     Plot_x_y(const Ntuple& nt, const char* Xvar, const char* Yvar, unsigned int verbose=0, bool square=false, const string& FnameEnd="", bool same=false);
void Plot_x_y1_y2(const Ntuple& nt, const char* Xvar, const char* Yvar1, const char* Yvar2, bool same_y_scale=false, unsigned int verbose=0, const string& FnameEnd="", bool logx=false, bool logy=false);
void Plot_x_ys(const Ntuple& nt, const char* Xvar, const vector<std::string>& YVar, const vector<double>& xyMinMax={0,0,0,0}, unsigned int verbose=0, const string& FnameEnd="", bool logx=false, bool logy=false); // Xvar vs  list of YVar

TGeoRotation WCS_root(double theta,double phi,double psi); // Courant Snyder to Euclidian rotation matrix

inline TEveVectorT<double> to_TEveVector(const double vec[3]) { TEveVectorT<double> t(  vec[0],  vec[1],  vec[2]) ; return t; } // from double [3] to_TEveVector
inline TEveVectorT<double> to_TEveVector(const     Vec3& vec) { TEveVectorT<double> t(vec.r[0],vec.r[1],vec.r[2]) ; return t; } // from my Vec3 to to_TEveVector

bool Has_Class(string rootfile, string classname, unsigned int verbose=0); // where classname something like   TGeoManager or TEveGeoShapeExtract

void Make_GeoShapeExtract_version(string fin, string fout="/tmp/hbu/temp.root",unsigned int verbose=0); // from geomanager to GeoShape, used for projections

hist1 to_hist1(const TH1D&); // from root TH1D to my hist1

#endif
