/*
 *  ~/c/MyClib/IdealGas.h
 *
 *  Created by Helmut Burkhardt on 14/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef IdealGas_h
#define IdealGas_h 1

#include "ConstElem.h"

class IdealGas
{ public:
  vector<ChemElem> ElemList;
  double GasPressure,GasTemp; // these are usually input

  //  derived
  double GasMolDensity; // molecules or atoms per m**3
  double GasDensity;    // kg/m**3
  double Atot;
  // methods:
  IdealGas(const vector<ChemElem>& ElemList,double GasPressure,double GasTemp); // constructor
  virtual ~IdealGas() {}; // (empty) destructor

  string Info();
};
#endif
