// ~/c/MyClib/SynchrRad.h
#ifndef SynchrRad_h
#define SynchrRad_h 1

// next class not so useful, most methods more useful outside class
class SynchrRad
{
public:
  Lorentz Part;       // the particle
  double ngam;        // number of photons radiated by one particle per turn, see for example my LEP Note 632
  double U0;          // energy loss per turn in GeV
  double Ecrit;       // critical energy (all energies in GeV)
  double rho;         // bending radius
  double LambdaCrit;  // wavelength at critical energy
  double FreqCrit;    // critical frequency
  double QuantCorr;   // photon recoil quantum correction, reduces power by factor (1-QuantCorr)
  double TauTurn;     // damping time, number of turns it takes to radiate all energy
  // methods
  SynchrRad(const double rho,const Lorentz& thePart); // constructor
  const void PrintU0(ostream& FilOut,const double frev);    // print U0,Ecrit,LambdaCrit,FreqCrit, damping time
};

// SynchrRad with Suppression
class SynchrRadSuppr : public SynchrRad     // derived from class SynchrRad
{
public:
  double Chamberheight;   // full Chamberheight
  double frevMag;         // revolution frequency of the circular machine with bends only
  double FreqCutOff;      // hard cutoff frequency, no synchrotron radiation below
  double LambdaCutOff;    // hard cutoff wavelength, no synchrotron radiation for longer wavelength
  double U0cut;           // energy loss per turn in GeV above hard cutoff
  // methods
  SynchrRadSuppr(const double rho,const Lorentz& thePart):SynchrRad(rho,thePart){} // SynchrRadSuppr constructor calling base class has constructor, needed since base class has constructor
  double CalcCutOff(const double Chamberheight,const double frev);    // calc FreqCutOff,LambdaCutOff and FracCutOff
  const void PrintU0(ostream& FilOut);    // print U0,Ecrit,LambdaCrit,FreqCrit, damping time
  const void PrintU0(ostream& FilOut,double frev);    // overloaded, version with also frev given
  const void PrintCutOff(ostream& FilOut); // Synchr.Rad suppression by fixed distance cutoff
};


//now outside:
double U0FromRho(double rho,const Lorentz& thePart); // Eloss from synchrotron radiation as function of bending radius
double RhoFromU0(double U0 ,const Lorentz& thePart); // calc the effective bending rad from U0
double SR_dWdtheta(const double GamTheta,const double freqOverFreqCrit,const Lorentz& Part,const int mode); // angular power spectrum photon spectrum
double SR_dNdtheta(const double GamTheta,const double Ibeam,const double DeltafreqOverfreq,const double freqOverFreqCrit,const Lorentz& Part,const int mode); // photon spectrum
double SR_dNAsymp(                       const double Ibeam,const double DeltafreqOverfreq,const double freqOverFreqCrit,const Lorentz& Part); // low angle,freq. aymptotic
double SR_Angle(const double xi,const double GamTheta,int mode); // angular distribution, K2/3**2 * F(xi,theta) Wiedemann2 page 255
double inline SR_AngleHiFreq(const double xi,const double GamTheta) // HiFreq (above critical) approx, even for low freq not so bad, see my SynchrRad.tex and Wiedemann II p. 259
{ return M_PI_2 *exp(-2.*xi)/xi *(1.+GamTheta*GamTheta)*(1.+2.*GamTheta*GamTheta); }

#endif
