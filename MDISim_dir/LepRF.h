// ~/c/MyClib/LepRF.h

#ifndef LepRF_h
#define LepRF_h 1

/* consistent set of LEP RF parameters */
class LepRFCuFreq
{
public:
  /* first the three basic numbers */
  /* old, initialize static constants, gave problems with shared libs
   static const int Harm;         // mean harmonic number Cu RF
   static const int Harmb;        // beating harmonic number
   static const double fRF;        // nominal, mean (central orbit) frequency Cu RF
   */

  /* first the three basic numbers */
  int Harm;      // mean harmonic number Cu RF
  int Harmb;     // beating harmonic number
  double fRF;     // nominal, mean (central orbit) frequency Cu RF
  /* now the drived numbers */
  double frev;        /* revolution frequency */
  int Harm1;
  int Harm2;
  double fb;    /* beating or envelope frequency, positive */
  double f1;            /* lower  storage cavity frequency, 0 mode */
  double f2;            /* higher storage cavity frequency, 9 mode */
  double wavel;   /* mean */
  double wavelb;  /* beating */
  double wavel1;  /* wave length   for f1 */
  double wavel2;  /* wave length   for f2 */
  double Circum;  /* circumference of LEP, defined by RF frequency */
  // methods
          LepRFCuFreq();  // constructor
  virtual ~LepRFCuFreq(){}; // empty destructor
};
#endif
