//   ~/c/MDISim/Twiss_util.h     H. Burkhardt 8/2013
#ifndef Twiss_util_h

class Beam // with particle, energy, emittance
{
public:
  Lorentz* Part;
  double Eb;
  double gamma;
  double cqm; // "const" cq calculated with the actual particle mass  (electron, proton etc).
  double I_1,I_2,I_3,I_4,I_5; // synchrotron radiation integrals "I1" - "I5"
  double J[3]; // damping partition Jx, Jy, Jz
  double alfac;
  double kbunch;
  double length; // length of machine, circumference in case of ring
  double npart;
  double RFHV,Harm; // rf voltage and harmonic number
  double ex,ey; // emittances from tfs header or synrad
  double phis,Qs,sige,sigz,sigb; // synchrotron tune, energy spread and bunch length and bucket half-height

  // derived
  const double Cs;
  double eta; // slip factor  alfac-1./(gamma*gamma);
  double rho,U0,frev,ibun,ibeam;
  double exN,eyN; // normalized emittances calculated from ex, ey from tfs header or synrad
  
  Beam(const char* Fname,unsigned int verbose=0); // constructor
  Beam(const Beam&)=default; // copy constructor
  inline const Lorentz& Get_Part() { return *Part;}
  inline double Get_gamma() const { return gamma;}
  inline double Get_Eb() const { return Eb;}
  void EmitFromSynrad(double kappa); // calculate ex from synchrotron radiation and ey=kappa*ex
  void SetEmitN(double exN,double eyN); // Set normalized emittances, and geometrical emittances, normally from tfs header or synrad. Here set explicitely, maybe useful for linacs and protons
  void Touschek(Ntuple& nt,unsigned int verbose);
  void   Set_Eb(double Eb);
  void SetRF(double RFHV,double Harm) { this->RFHV=RFHV; this->Harm=Harm; };
  void RFHV_from_twiss(const Ntuple& nt); // also sets Harm
  // void RFHV_from_twiss(Ntuple& nt); // sum up RFHV from twiss loaded to Nt
  void CalcDerived();
  void Print();
  virtual void SetVerbose(unsigned int verbose) { this->verbose=verbose; }
private:
  unsigned int verbose;
};

class LinTrk : public Beam
{
public:
  unsigned int nx; double dx; // number of steps and step sizes in units of sigma
  unsigned int ny; double dy; // number of steps and step sizes in units of sigma
  unsigned int np; double dp; // number of steps and step sizes in fraction of beam energy
  Ntuple* the_nt;
  LinTrk(const char* twiss_fname,const char* survey_fname,unsigned int verbose=0); // constructor
  void SetSteps(unsigned int nx, double dx, unsigned int ny, double dy, unsigned int np, double dp);
  void trk_rmatrix(const string& trk_fname);
private:
  unsigned int verbose;
};

void ShiftToElement(Ntuple &nt,string ElementName,unsigned int verbose);

void CalcSynradFr(Ntuple &nt,const Beam &Mach,unsigned int verbose,const double nsig=1);

vector<unsigned int> FindElements(Ntuple &nt,const string& MatchStr,unsigned int verbose,const string& ColumnName=string("NAME"),size_t MatchPos=string::npos); // by default ColumnName="NAME", default arguments seems not to work in CINT, better give expicitely the column name

void PrintBends(Ntuple &nt, const vector<unsigned int>& elemList,const Beam& Mach,unsigned int verbose);
void PrintQuads(Ntuple &nt, const vector<unsigned int>& elemList,const char plane,const Beam& Mach, unsigned int verbose,const double nsig=1);

void CalcBeamSizeDivergence(Ntuple &nt,const double emitx,const double emity=0,const double dp=0,const unsigned int verbose=0); // calculate beam sizes and divergences and add to ntuple

//old void trk_rmatrix(const char* twiss_fname,const char* survey_fname,const string& trk_fname,double emitxN=0,double emityN=0,unsigned int verbose=0);

class LatticeAndBeam : public Beam, public Ntuple // with both lattice and beam information --- not always a good solution, maybe better to keep separate lattice and beam
// in general better to keep Beam, Ntuple seperate, with pointer to the other if information from both needed
{
public:
  LatticeAndBeam(Ntuple &nt,Beam &Mach); // constructor
  void Print_LatticeAndBeam();
  ;
private:
  unsigned int verbose;
};

double F_Touschek_appr(double x);

#endif
#define Twiss_util_h 1
