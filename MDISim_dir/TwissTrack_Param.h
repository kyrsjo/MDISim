//   ~/c/MDISim/TwissTrack_Param.h     H. Burkhardt 6/2013

#ifndef TwissTrack_Param_h

class TwissTrack_Param
{ public:
  unsigned int verbose;
  double emitx,emity;       // geometrical emittances

  // unsigned int IPNumb;  // not yet set, could be read from IPstr
  double BetaStarX,BetaStarY; // beta functions at IP
  string IPstr;

  TwissTrack_Param(double emitx,double emity,unsigned int verbose); // constructor
  string Print(void);
};

#endif
#define TwissTrack_Param_h 1
