//   ~/c/MDISim/TwissTrack_Param.C     H. Burkhardt 5/2013

#ifndef __CLING__
#include "MyCppStdHeaders.H"  // use this to test syntax
#else
#include <iomanip>
#include <cstring>    // older C string routines like   strtok  etc
#include <string>		// Stroustrup3 432,  string of T
#include <sstream>	// string streams, Stroustrup3 p. 640,  or "mysstream.h" for old compilers
using namespace std;
#endif

#include "TwissTrack_Param.h"

TwissTrack_Param::TwissTrack_Param(double emitx,double emity,unsigned int verbose) // constructor
{
  this->emitx=emitx;
  this->emity=emity;
  this->verbose=verbose;
  if(verbose) cout << "TwissTrack_Param constructor end emitx=" << emitx << " emity=" << emity << '\n';
}

string TwissTrack_Param::Print(void)
{
  ostringstream ostr;
  ostr << "emitx=" << emitx << " emitx=" << emity << " IPstr=" << IPstr << " BetaStarX=" << BetaStarX << " BetaStarY=" << BetaStarY << '\n';
  return ostr.str();
}
