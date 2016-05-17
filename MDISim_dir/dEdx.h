/* ~/c/MyClibdEdx.h

 used in
 ~/c/SynchrLead/main_SynchrLead.C
 ~/c/beam/BeamGas.C
 ~/c/beam/HeavyIonStripping.C
 ~/c/const/TestdEdx.C

 */
// the maximum energy transfer in a single collision
// see GEANT PHYS332-2, PDG 94 formula 10.2 page 1251
// double tmax=2.*Mele*beta2*gamma2/(1.+2.*gamma*Mele/Part.m+pow(Mele/Part.m,2));

#ifndef dEdx_h
#define dEdx_h 1

#include "Lorentz.h"

inline double tmax(double b,double g,double m) { return 2.*Mele*b*b*g*g/(1.+2.*g*Mele/m+Mele*Mele/(m*m) ); }

// or directly with the Part as argument
inline double tmax(const Lorentz& Part) { return tmax(Part.beta,Part.gamma,Part.m); }

double dEdx(ostream *theOutFile,Lorentz& Part,const ChemElem& Element);
double dEdxHeavyIon(ostream *theOutFile,Lorentz& Part,const ChemElem& Elem);
#endif
