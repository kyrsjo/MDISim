/* ~/c/MyClib/ConstElem.h

 3/2007  gcc 4.2 is stricter on char and const char
 name and symbol were char*,  now use const char*

 6/2011 const char* Name, Symbol ---> string

 data for elements
 on www, webelements: http://www.webelements.com/

 data on the web:
 there is also a  C-header using defines (ugly) on the web: http://www.umich.edu/~sanders/constants.h


 tungsten thermal properties, see
 http://www.webelements.com/webelements/elements/text/W/heat.html


 specific heat capacity
 http://en.wikipedia.org/wiki/Specific_heat
 Copper   cp = 0.385 J/(g K)     Cp = 24.47  J/(Mol K)
 Uranium  cp = 0.116 J/(g K)     Cp = 27.7   J/(Mol K)

 */

#ifndef ChemElem_h
#define ChemElem_h 1

#ifndef ConstPhys_h

#include "ConstPhys.h"
#endif

// struct ChemElem { // struct s{  is the same as    class s{ public:   Stroustrup2 167, Stroustrup3 p. 101,      so struct can also have constructor which is useful to define the initialization
class ChemElem { public:  // and see   http://en.wikipedia.org/wiki/Composite_type    on structure, class etc   http://en.wikipedia.org/wiki/Class_%28computer_science%29
  string Name;
  string Symbol;     // Chemical symbol like  Pb for lead
  double Z;           // atomic number
  double A;           // atomic weight
  double X0;          // radiation length in kg/m**2    = 0.1 g/cm**2, here from table, see also functions X0 and X0Tsai below
  double Density;     // in kg/m**3 =  g/liter
  double Melting;     // in deg Celsius, from webelements
  double SpecHeat;    // in cal/(g K), old specific heat definition of elements at 25 deg Celsius, D-218
  /* specific heat capacity or specific entropy, given in J / (K mol)
   sometimes also in  Joule Kelvin / mol   on web called C_p for example  lead   26.4 J / (K mol)
   or copper 24.43 J / (K mol)
   1 mol: A * g
   1 Joule = 1./4.1840 1 cal =
   convert to cal/(g K) by: * cal=1./( 4.1840*A)     (/4184 * A)   see Const.C, ChemElemDer.C PriElem
   lead   conversion:  26.4 / (4.184 * 207.2) = 0.03045 cal/(g K)    while PDG has .038 cal/(g K)
   copper conversion:  24.43 / (4.184 * 63.546) = 0.09188 cal/(g K)    while PDG has .0092 cal/(g K)
   indium conversion:  26.7  / (4.184 * 114.818 ) = 0.055579 cal/(g K)
   gold   conversion:  25.42 / (4.184 * 196.96655) = 0.0308 cal/(g K)    while PDG has .0092 cal/(g K)
   uranium conversion  27.7  / (4.184 * 196.96655) = 0.0336121 cal/(g K)
   on specific heat 59th handbook F-120   number of calories needed to raise 1 gramm by temp of 1 K
   heat capacity see 59th handbook F-106
   */
  double Resistivity; // in Ohmm       resistance between faces of 1 meter cube  10**-8 Ohmm = 1 muOhm m
  double EIon;        // mean Ionization energy in GeV from http://physics.nist.gov/PhysRefData/XrayMassCoef/tab1.html   or EGS4 manual p. 69
  double ColLen; // Nuclear collision   length in km/m**2 PDG 2004 6.  p. 98,   no yet found in online versions, only printed table
  double IntLen; // Nuclear interaction length in km/m**2
  // now the methods
  // ChemElem(){cout << "constructor called" << '\n';}; // constructor not allowed in Scalar objects defined below with static
  void PriElem(ostream& FilOut) const;  // print contents, not necessarily provided, if used compile also ConstElem.C
  ChemElem(string Name,string Symbol,const double& Z,const double& A,const double& X0,const double& Density,const double& Melting,const double& SpecHeat,const double& Resistivity,const double& EIon,const double& ColLen,const double& IntLen); // constructor
  ChemElem(){}; // empty constuctor, for ChemElemDer
  virtual ~ChemElem() {};   // (empty) destructor()
  double rhom() const { return Density/(MamuKg * A);} // atoms / m^3
};

class Isotope { public:
  string Name;
  string Symbol;     // Chemical symbol like  Pb for lead
  double N;           // isotope number (Number of nucleons)
  double A;           // atomic weight
  double Z;           // atomic number
  double Zion;        // Ion charge, = Z if fully stripped, = 0 for neutral (default)
  double Abundance;   // Natural abundance
  Isotope(string Name,string Symbol,const double& N,const double& A,const double& Z,const double& Zion,const double& Abundance);
  virtual ~Isotope() {};   // (empty) destructor
  void Strip(double Zion);   // strip to Zion
  void Print(ostream& FilOut) const;    // print contents, not necessarily provided, if used compile also ConstElem.C
};

namespace ConstElem {
  // http://pdg.lbl.gov/2008/AtomicNuclearProperties/
  // http://pdg.lbl.gov/2010/AtomicNuclearProperties/
  // http://pdg.lbl.gov/2012/AtomicNuclearProperties/       so far only Cu updated
  // see PDG table 4.1  page 69 in 1996 edition, page 77 1998 edition
  // for more exotic materials like Au (Gold) see Tsai Rev.Mod.Phys. 46 (1974) Historical/Tsai folder
  // a web reference for ionization energies   http://physics.nist.gov/PhysRefData/IonEnergy/ionEnergy.html
  // http://en.wikipedia.org/wiki/Hydrogen     http://en.wikipedia.org/wiki/Deuterium
  //                               Name      Symbol Z     A         X0  Density Melting   SpecHeat    Resistivity  Eion ColLen IntLen  0 if not known
  //                                                                    kg/m^3
  static  const ChemElem Beryllium("Beryllium","Be", 4.,9.012182, 651.9, 1848., 1287.   ,  0.436    , 5.885e-8,  63.7e-9,  553., 778.);
  static  const ChemElem Carbon   ("Carbon"   ,"C" , 6., 12.011 , 427.0, 2265., 3800.   ,  0.165    , 1375.e-8,   78.e-9,  602., 863.); // density is for graphite
  static  const ChemElem Aluminium("Aluminium","Al",13., 26.98154,240.1, 2700., 660.37  ,  0.215    , 2.6548e-8, 166.e-9,  706.,1064.);
  static  const ChemElem Iron     ("Iron"     ,"Fe",26., 55.845 , 138.4, 7874., 1535.   ,  0.1075   , 9.71e-8,   286.e-9,  828.,1319.);
  static  const ChemElem Stainless("Stainless","Fe",26., 55.845 , 138.4, 7874., 1420.   ,  0.1075   , 7.14e-7,   286.e-9,    0.,   0.); // stainless steel with 18 % chrom, 8 % nickel, resistivity from 1/conductivity Chao page 43, rest copied from Fe  (very approx);  web http://hcrosscompany.com/metals/stainless.htm 7.2 - 7.4 e-8 or 10 * better !!!

//static  const ChemElem Copper   ("Copper"   ,"Cu",29., 63.546 , 128.6, 8920., 1083.4  ,  0.0924   , 1.673e-8,  322.e-9,  856.,1349.);
  static  const ChemElem Copper   ("Copper"   ,"Cu",29., 63.546 , 128.6, 8960., 1085.   ,  0.0924   , 1.673e-8,  322.e-9,  842.,1373.); // updated 2012

  static  const ChemElem Indium   ("Indium"   ,"In",49.,114.818 , 88.491,7310.,  156.6  ,  0.05558  , 8.e-8,     488.e-9,    0.,   0.);
  static  const ChemElem Tungsten ("Tungsten" ,"W" ,74.,183.84  ,  67.6,19250., 3422.   ,  0.0322   , 4.9e-8,    727.e-9, 1104.,1919.);
  static  const ChemElem Gold     ("Gold"     ,"Au",79.,196.966543,64.608,19300.,1064.18 ,  0.0308455, 2.2e-8,   790.e-9, 1125.,196.3);
  static  const ChemElem Lead     ("Lead"     ,"Pb",82.,207.2   ,  63.7,11340., 327.502 ,  0.0305   , 7.14e-7,   823.e-9, 1162.,1940.);
  static  const ChemElem Uranium  ("Uranium"  ,"U", 92.,238.02891, 60.0,19000., 1135.   ,  0.0336121, 28.e-8,    890.e-9, 1186.,2090.); // http://pdg.lbl.gov/2010/AtomicNuclearProperties/HTML_PAGES/092.html
  // density for next is for Gas
  static  const ChemElem Hydrogen ("Hydrogen" ,"H", 1., 1.00794, 630.4,.0899,  14.01    ,   0   ,       0   ,   19.2e-9,   433., 508.); // radlength was ealier 612.8  (pre 2005 ?)
  static  const ChemElem Deuterium("Deuterium","D", 1., 2.01410,1259.8,.168,   23.65    ,   0   ,       0   ,   19.2e-9,   513., 718.); // verify, in particular Eion
  static  const ChemElem Helium   ("Helium"   ,"He",2., 4.002602,943.2,.1786,   0.95    ,   0   ,       0   ,   41.8e-9,   499., 651.);
  static  const ChemElem Nitrogen ("Nitrogen" ,"N", 7., 14.00674,379.9,1.25,   63.05    ,   0   ,       0   ,    82.e-9,   614., 878.);
  static  const ChemElem Oxygen   ("Oxygen"   ,"O", 8., 15.9994, 342.4,1.428,   54.8    ,   0   ,       0   ,    95.e-9,   632., 910.);
  static  const ChemElem Argon    ("Argon"    ,"Ar",18., 39.948, 195.5,1.782,   83.8    ,   0   ,       0   ,   188.e-9,   764.,1172.);
  // allows to directly access data like   Aluminium.SpecHeat

// natural (stable) Lead Isotopes
// CRC book 59th edition
// lead p. B-32 Z = 82, A = 207.2   four stable isotopes, p. B-336
// 1.48 %  Pb 204,  A = 203.973
// 23.6 %  Pb 206,  A = 205.9745
// 22.6 %  Pb 207,  A = 206.9759
// 52.3 %  Pb 208,  A = 207.9766
// webelements 3/2003:
// 204Pb 203.973020(5)  1.4 %
// 206Pb 205.974440(4) 24.1
// 207Pb 206.975872(4) 22.1
// 208Pb 207.976627(4) 52.4
//                                       N    A          Z   Zion Abundance
static const Isotope Pb204("Lead",  "Pb",206.,203.973020,82.,0,0.014);
static const Isotope Pb206("Lead",  "Pb",206.,205.974440,82.,0,0.241);
static const Isotope Pb207("Lead",  "Pb",207.,206.975872,82.,0,0.221);
static const Isotope Pb208("Lead",  "Pb",208.,207.976627,82.,0,0.524);

// Indium CRC book 59th edition p. B-29, isotopes, p. B-304
//  4.28 %  In 113, A = 112.9043
// 95.72 %  In 115, A = 114.9041
// webelements 3/2003
// 113In 112.904061(4)  4.29 %
// 115In 114.903882(4) 95.71 %
//                                       N    A          Z   Zion Abundance
static const Isotope In113("Indium","In",113.,112.904061,49.,0,0.0429);
static const Isotope In115("Indium","In",115.,114.903882,49.,0,0.9571);

// 197Au 196.966543 100 %
//                                       N    A          Z   Zion Abundance
static const Isotope Au197("Gold","Au",197.,Gold.A,79.,0,1.0);

} // namespace ConstElem
using namespace ConstElem;

double AverX0(const ChemElem& theElement1,const ChemElem& theElement2);     // rad length of mixture of two elements
double AverX0(const vector<ChemElem>& Molecule); // rad length of molecule

// simple Dahl inline fit formula to get radiation length, tested in ~/c/CrossSec/RadLen.C
// Dahl formula given in PDG98 page 147, PDG2000 p. 167, here *10 for kg/m**2, (ref is priv comm.)
// still same in 2008     http://pdg.lbl.gov/2008/reviews/passagerpp.pdf   local copy ~/temp/PDG_2008/passagerpp.pdf
inline double X0(const ChemElem& Elm)
{ return 7164.*Elm.A/(Elm.Z*(Elm.Z+1.)*log(287./sqrt(Elm.Z))); } // Dahl formula, good to 2.5 % except for Helium where this is 5 % low

// rest was previously RadLen.h

inline double fTsai(double Z) // TSAI (3.3) p. 820, see also Table II.6 p. 828
{ double a=(AlfaQED*Z)*(AlfaQED*Z); // evalute always  f( (aZ)**2 )   then also directly comparable with fBethe
  return (1.202*a - 1.0369*a*a + 1.008*a*a*a/(1.+a));// TSAI (3.3) P. 820  corrected for factor a, see also Tsai Table II.6 p. 828
}

inline double fBethe(double Z) // PDG 2000 p.166  DaviesBethe1954  Eq. 36, 38
// Coulomb correction
// this seems to be the correct formula which agress also best with Tsai's numerical values
// it is also used in EGS, see EGS4 manual Eq. 2.7.17 p. 31
{ double a=AlfaQED*Z;
  double Sum=1./(1+a*a)+0.20206-0.0369*a*a+0.0083*pow(a,4)-0.002*pow(a,6); // DaviesBethe1954  Eq. 38
  return a*a*Sum; // DaviesBethe1954  Eq. 36
}
double X0Tsai(const ChemElem& Elm);
#endif
