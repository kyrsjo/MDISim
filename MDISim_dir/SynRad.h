// ~/c/MyClib/SynRad.h
// fast functions using Chebyshev summation to calculate and generate the standard synchrotron radiation spectra
// H. Burkhardt, Monte Carlo generation of the energy spectrum of synchrotron radiation, 8 June 2007
// CERN-OPEN-2007-018; CLIC-Note-709; EUROTEV-Report-2007-018
// http://cdsweb.cern.ch/record/1038899     local file  file:///Users/hbu/tex/InvSynFracInt/InvSynFracInt_report.pdf
//
// file:///Users/hbu/www/gesynrad.pdf  my 1990 LEP Note 632, local file
// file:///Users/hbu/c/SynRad/ReadMe.txt    and         file:///Users/hbu/tex/bs/Math/Math.pdf    with formulas for the functions below

#ifndef SynRad_h
#define SynRad_h 1

double SynRadC(const double x);       // Photon spectrum              int_x^\infty K5/3(x) dx, normalization is 5 pi /3,  i.e. multiply with 3/(5pi) to get the normalized spectrum
double SynRadInt(const double z);     // integrated photon spectrum   int_z^\infty SynRadC(x) dx, number of photons above
double InvSynFracInt(const double x); // inverse integrated spectrum, direct "ultimate" photon spectrum generator,  also implemented in Geant4
inline double PowSynRadC(const double x) { if(x>0) return x*SynRadC(x); else return 0; } // Power spectrum, normalization is 8 M_PI / (9 sqrt(3))
inline double SynFracInt(const double x) { if(x>0) return (1.-3.*SynRadInt(x)/(5.*M_PI)); else return 0; } // fraction of photons below x
// would also be useful to have a PowSynRadint   , the integrated power spectrum, so far done numerically, see  ~/c/SynRad/main_Examples.C

#endif
