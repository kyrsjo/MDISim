// ~/c/MyClib/Lorentz.h

#ifndef Lorentz_h
#define Lorentz_h 1

// class Lorentz : public Vec4M { public:   // mainly for total energy, momentum, beta , gamma
class Lorentz
{
public: // mainly for total energy, momentum, beta , gamma
  double E;
  double P;
  double m;   // mass, el. or proton mass, for ions m=A*Mamu
  double gamma;
  double beta;
  double Z;   // charge of particle in units of elementary charge, +1 for e+,p  , -1 for e-, 82 for Pb82
  double N;   // number of nucleons, like 208 for Pb 208, only used for some printing
  const char* Name;   // for info, print only

  double Rc; // classical particle radius, proportional to the charge squared, one Echarg here cancelled with GeV units of mass
  // now the methods
  // Lorentz(){};   // constructor, needed if base class has constructors  - here not really needed
  Lorentz() : Z(1),N(1) {}; // constructor, with initialization of Z=1, N = 1
  Lorentz(double E, double m,const char* Name) : Z(1),N(1) { this->E=E; this->m=m; this->Name=Name; FromE(); CalcRc(); }; // constructor, with initialization of Z=1, N = 1
  ~Lorentz() {}; // (empty) destructor
  void    FromP();
  void    FromP(double P); // allow to also set P at the same time
  void    FromE();
  void    CalcRc() { Rc=Echarg*Z*Z/(4.*M_PI*eps0*1.e9*m);} // classical particle radius, proportional to the charge squared, one Echarg here cancelled with GeV units of mass
  double  PowerEfield(double Efield); // power radiated by an accelerated charge in a pure Efield
  double  PowerBfield(double Bfield); // power radiated by an accelerated charge in a pure Bfield
  double  BunchPulseLength(double PipeRad); // rms current pulse length induced by one particle in chamber walls
  double  RFPhaseFromQsU0(double Qs,double alfac,double U0harm); // calc stable phase angle from U0
  double  RFPhaseFromQsRFHV(double Qs,double alfac,double RFHVharm); // calc stable phase angle from the rf-voltage
  double  QsFromRFPhaseU0(double phis,double alfac,double U0harm); // calc Qs from stable phase angle and U0
  double  QsFromRFPhaseRFHV(double phis,double alfac,double RFHVharm); // calc Qs from stable phase angle and rf-voltage
  void    Iwall(double ChamRad,std::ostream& FilOut); // calculate the current induced by a single charge in a cylindrical chamber of radius ChamRad

  const void PrintLorentz(std::ostream& FilOut) const;
  const double inline EmitN(double Emit)  const { return Emit  *beta*gamma;  } // normalized emittance from emittance,  does not change anything in class, therefore constant
  const double inline Emit (double EmitN) const { return EmitN/(beta*gamma); } // emittance from normalized emittance,  does not change anything in class, therefore constant

  void SetVerbose(unsigned int verbose) { this->verbose=verbose; }
  unsigned int GetVerbose(unsigned int verbose) const { return verbose; }

protected:
  unsigned int verbose;

};
// end of Lorentz part

double MultipleScattering(const Lorentz& Part,double x,double X0length); // returns scattering angle
double MultipleScattering(const Lorentz& Part,double xrho,double Z,double A); // without rad.len, more precise

double inline F_Touschek_appr(double x) { double lx=log(x); return -2.07721566490153- lx+ x*(51.0889080074805-29.0734120211816*lx+6*lx*lx)/24; } // hbu 1/2015, Touschek2.nb

double inline SigNoS(double Sbuc,double Eb1,double Eb2)
{ // BeamBeam Bremsstrahung cross section, no screening
  return 16./3.*Re*Re*AlfaQED*( (log( (2.*Eb1/Mele)*(2.*Eb2/Mele) ) -.5)*(-log(Sbuc)-5./8.)
                               +0.5*log(Sbuc)*log(Sbuc) - M_PI*M_PI/6. -3./8. );
}

double inline SigScr(double Sbuc,double Apar)
{ // BeamBeam Bremsstrahlung cross section screened
  return 16./3.*Re*Re*AlfaQED* ((log(Apar)+1./12.)*log(1./Sbuc)-5./8.*log(Apar)-1./12.);
}

#endif
