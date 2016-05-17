// ~/c/MyClib/ConstPhys.h
// math constants

#ifndef ConstPhys_h
#define ConstPhys_h 1

#include <complex>

// see <cfloat> if long double exists and really precision larger than DBL_MANT_DIG
// see also "/Applications/Metrowerks CodeWarrior 8.0 CodeWarrior Manuals/PDF/Targeting_Mac_OS.pdf"
// Mac OS 8 and 9  was more precise than Mac OS X, see Carbon help on Mathematical and Logical Utilities
// only  the functions num2decl, dec2numl, x80told, and ldtox80  have more precision
#if defined(LDBL_MANT_DIG) && LDBL_MANT_DIG>DBL_MANT_DIG
// the lxplus  2.95.2  compiler pretends 18 digits for LDBL_MANT_DIG compared to DBL_MANT_DIG
// seems not really true, see TestPrecision  in TestNum.C
// checked again on lxplus032 in June 2004 with gcc (GCC) 3.2.3, now seems really true long long and long double  with 18 digits
//  true long double also available on mac with gcc4  (Tiger) with 31 digits
typedef   long double my_long_double; // use this when long double available, for test print see ~/c/ansiitest/TestByteManip.C
#else
typedef   double my_long_double; // use standard double if true long double not available
#endif

namespace MyConstPhys // provide special 6x6 matrices    can be used as   M6x6::Identity,  see  ~/c/ansiitest/TestMatrix.C
{

  // Mathematical constants see http://www.gnu.org/software/libc/manual/html_node/Mathematical-Constants.html
  
  // add letter L  for long double constant,  up to 35 and at least 31 significant digits, give 40 to be sure.   See also Precision.nb
  //                                   123456789 123456789 123456789  123456789
  const my_long_double sqrt3  =1.7320508075688772935274463415058723669428L;

//  keep pi for backwards compatibility and for precision tests, ---  in general better use standard M_PI   defined in   /usr/include/math.h
#ifndef __FP__  // avoid clash with CarbonCore/fp.h which defines pi
  const my_long_double pi =3.141592653589793238462643383279502884197L;  // was in old math.h, not in new ANSI, also possible: M_PI=2.*acos(0.);
#endif

  const my_long_double MeanSyn=8./(15.*sqrt3);        // mean synchroton phot energy in units of Ecrit.
  const my_long_double SyNorm =3./(5.*M_PI);            // 1./ int 0 to infty of  x* K 5/3,  synchr.rad phot spectrum
  const my_long_double SyPowNorm= 9.*sqrt3/(8.*M_PI);   // 1./ int 0 to infty of  x*x K 5/3,  synchr.rad pow spectrum
  const double GamEuler       =0.5772156649015328606065120900824024310422L; //  Euler's constant

  // some values of the gamma function (frequently used in Impedance calculations, special functions) see also Gamma.nb
  const double Gamma12=sqrt(M_PI); // gamma(1/2)
  const double Gamma32=Gamma12/2;  // gamma(3/2)
  const double Gamma23=1.3541179394264004169452880281545137855193L;   // gamma(2/3)
  const double Gamma14=3.6256099082219083119306851558676720029952L;   // gamma(1/4) = sqrt(2) M_PI / gamma(3/4)
  const double Gamma34=1.225416702465177645129098303362890526851L;    // gamma(3/4)

  const std::complex<double> iimag(0.,1.); // maybe this is better anyway

  /*
   physics constants, PDG2000 p.73, CODATA 2000
   PDG general http://pdg.lbl.gov/     http://pdg.lbl.gov/2013/reviews/rpp2013-rev-phys-constants.pdf
   PDG 2006 http://pdg.lbl.gov/2006/reviews/contents_sports.html#constantsetc
   http://pdg.lbl.gov/2006/reviews/contents_sports.html#partpropetc
   http://pdg.lbl.gov/2006/html/computer_read.html
   PDG 2010 not yet updated to that :   http://pdg.lbl.gov/2010/reviews/rpp2010-rev-phys-constants.pdf  local file:///Users/hbu/temp/PDG_2010/rpp2010-rev-phys-constants.pdf
   Particle definitions, old geant3 numbering (never used by me) : http://wwwasdoc.web.cern.ch/wwwasdoc/geant_html3/node72.html#SECTION024000000000000000000000
   PDG encoding used in Geant4   http://pdg.lbl.gov/2007/mcdata/mc_particle_id_contents.html
   */

  // using by default high energy physics units with GeV
  const double Mele   = .510998928E-03;    //  electron mass in GeV /c^2  http://en.wikipedia.org/wiki/Electron
  const double Mproton= .938272046;        //  proton mass in GeV /c^2    http://en.wikipedia.org/wiki/Proton
  const double Mneutron=.939565378;        //  neutron mass in GeV /c^2   http://en.wikipedia.org/wiki/Neutron
  const double Mdeuteron=1.99900750108*Mproton; //  neutron mass in GeV /c^2  http://en.wikipedia.org/wiki/Deuteron   http://physics.nist.gov/cgi-bin/cuu/Value?md   3.343 583 20(17) x 10-27 kg    http://physics.nist.gov/cgi-bin/cuu/CCValue?mdsmp|ShowSecond=proton+mass&First=md

  // from
  const double Mmuon  = .1056583715;      //  muon mass in GeV /c^2       http://en.wikipedia.org/wiki/Muon
  const double Mamu   = .931494061;       //  unified atomic mass unit C12/12    http://en.wikipedia.org/wiki/Unified_atomic_mass_unit
  const double Hbarc  = 197.3269718E-18;  //  in GeV meter          http://pdg.lbl.gov/2013/reviews/rpp2013-rev-phys-constants.pdf
  const double AlfInv = 137.035999074;     //  fine structure constant
  const double AlfaQED= 1./AlfInv;        // QED fine structure constant in Thomson limit
  const double gMin2  = .00115965218076;   // g-2     anomalous magnetic moment of the electron  https://pdg.web.cern.ch/pdg/2013/listings/rpp2013-list-electron.pdf
  // of muon http://pdg.lbl.gov/2013/reviews/rpp2013-rev-g-2-muon-anom-mag-moment.pdf
  const double GFermi=1.16637e-5;         // Fermi coupling constant GF/(hbar c)**3  GeV**-2  http://en.wikipedia.org/wiki/Fermi%27s_interaction

  //      SI units
  const double CLight =299792458.;        //  speed of light in m/s  http://en.wikipedia.org/wiki/Speed_of_light
  const double Echarg =1.602176565E-19;   //  unit charge  in Coulomb or Amperesecond   http://en.wikipedia.org/wiki/Elementary_charge
  const double Re     =2.8179403267E-15;  //  classical electron radius = Echarg / (4 M_PI eps0) 1.e9 Mele)  http://en.wikipedia.org/wiki/Classical_electron_radius
  const double ElLamda=3.8615926800E-13;  //  electron compton wavelength in m = Re/AlfaQED = Hbarc/(Mele c**2)  http://en.wikipedia.org/wiki/Compton_wavelength
  const double HPlanck=6.62606957E-34;    // J s       http://en.wikipedia.org/wiki/Planck_constant
  const double Hbar   =1.054571726E-34;    // = HPlanck/2pi,  J s  http://pdg.lbl.gov/2013/reviews/rpp2013-rev-phys-constants.pdf

  // ideal gas, SI
  const double Navo   =6.0221415E23;      // molecules per mole
  const double kBoltz =1.3806505E-23;     // Joule / Kelvin
  const double Celsius0=273.15;           // 0 deg Celsius in Kelvin
  const double MamuKg =1.66053886E-27;    // atomic mass unit, in kg
  const double cal=1./4.1840;             // 1 Joule in calories, 1 cal : water +1 Kelvin, value is definition, exact  1 cal = 4.1840 Joule, see 59th handbook F-315*/

  // unit conversion
  const double OneAtmos=101325.;          // One atmosphere in Pascal
  const double OneTorr =OneAtmos/760.;    // old pressure unit one Torr
  const double mbar=100.; // milli bar Pascal http://en.wikipedia.org/wiki/Pascal http://en.wikipedia.org/wiki/Bar_%28unit%29 1 bar = 1e5 Pa, 1 mbar = 1e2 Pa, milibar is hectopascal

  const double Barn=1.e28;                // 1 squaremeter in barn
  const double NanoBarn=1.e9*1.e28;       // 1 squaremeter in nanobarn
  const double Inch=0.0254;               // 1 in = 0.0254 m = 2.54 cm = 25.4 mm,    avoid instead   inch  which exists in  include/curses.h

  // now some derived constants:
  const double mu0=4.E-7*M_PI;                      //  N A**-2 = Vs/Am = Ohm s/m
  const double eps0=1./(mu0*CLight*CLight);       //  F/m = As/Vm = s /(Ohm m)
  const double ZFreeSpace=mu0*CLight;     // = 1/ eps0 c = sqrt(mu0/eps0) = 120 M_PI  impedance of free space in Ohm
  const double MeleinKg=Mele*Echarg*1.e9/(CLight*CLight); //  electron mass in kg
  const double kBoltzeV=kBoltz/Echarg;    // Boltzmann constant in eV / Kelvin

  // derived Gas constants
  const double GasConst=Navo*kBoltz;                  // mole**-1 Kelvin**-1
  const double MolVol=GasConst*Celsius0/OneAtmos;     // Volume 1 mole at 0 Celius, 1 atmosphere
  const double NLoschmidt=OneAtmos/(kBoltz*Celsius0); // =Navo/MolVol

  // derived constants for Synchrotron radiation
  const double Cgamma=Echarg/(3.e0*eps0)/(Mele*Mele*Mele*Mele) * 1.e-9;//  meter / GeV**3 units tricky see SynchrRad.tex
  const double cq=55.*Hbarc/(32.*sqrt3*Mele); // meter, for electrons
  const double kgam=5.*M_PI/(sqrt3*AlfInv); // when multiplied with gamma number of photons per turn
  const double LambdaSyn=2.*sqrt3/5. *MeleinKg*CLight/(AlfaQED*Echarg); // Tesla Meter, for mean free path between photon emissions

  // Radioactivity and Radiation Protection
  // gray   1 Gy = 1 Joule /kg = 100 rad = 6.24e12 MeV/kg

  // for examples of use and tests of constants and conversions see ~/c/const/main_const.C

  // for inline //Stroustrup1 page 124 as hint to the compiler

  // now some global inline conversion routines, previously in Conversion.h,C

  inline double   PartFromCur(double current,double frev) { return current/(Echarg*frev); } // number of particles from the beam current and revolution frequency for particle with single elementary chare

  // inline double    RadiusFromBfield(double Bfield,double pmom);

  // inline double    MomFromBfield(double Bfield,double Radius);

  // inline double    BfieldFromRadius(double Radius,double pmom);

  // inline double    RigidityFromAngle(double alpha,double pmom);

  // inline double   NgamBend(double alpha,double gamma);

  // conversions functions like from field in Tesla to bending radius
  // for single charged particles,  for ions call with P/Z

  // from Tesla to 1/meter
  // rho = p / ( B*e )
  // with p in GeV/c and B in Tesla or Vs/m**2
  // rho = 10**9/c  * 1/B
  // a bending magnet of length l bends by alpha = l/rho
  inline double RadiusFromBfield(double Bfield,double pmom)   { return pmom*1.e9/(CLight*Bfield); }   // Radius
  inline double MomFromBfield(double Bfield,double Radius)    { return CLight*Bfield*Radius/1.e9; }   // momentum
  inline double BfieldFromRadius(double Radius,double pmom)   { return pmom*1.e9/(CLight*Radius); }   // Bfield
  inline double RigidityFromAngle(double alpha,double pmom)   { return 1.e9/CLight *alpha*pmom; }     // magnetic rigidity in T m

  inline double NgamBend(double alpha,double gamma) { return 5.*gamma*alpha/(2.*AlfInv*sqrt3); } // see Working_Notes/SynchrRad.tex eq:NgammaL

  // time conversion constants, calculated in TestTime:
  const double       ADto1900      =59926780800.0; // too much for unsigned int, take double
  const unsigned int From1900To1970=0x83aa7e80; // difference in seconds, hexadecimal
  // const unsigned int From1900To1970=2208988800; // gives warning on gnu, ok (how to get rid of warning ?)
} // namespace MyConstPhys
using namespace MyConstPhys;

//CSPE hack for lxplus to compile   even if std::defaultfloat  is not defined
#ifndef __APPLE__
#define defaultfloat left
#endif

#endif
