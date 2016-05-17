// ~/c/MyClib/NtupleRoot.h    started 29/09/2007.

#ifndef NtupleRoot_h
#define NtupleRoot_h 1

#include <TVector.h>

class TTree;

class NtupleRoot : public Ntuple
{
public:
  // methods:
  NtupleRoot(){}; // constructor 0, empty
  NtupleRoot(const vector <string>& tags,ostream *theFilOut); // constructor 1
  NtupleRoot(const char* fname,const char* treename,unsigned int verbose=0);   // constructor 2
  NtupleRoot(const char* fname); // constructor 3
  NtupleRoot(const NtupleRoot&)=default; // copy constructor
  //NtupleRoot(const Ntuple&)=default; // copy constructor from Ntuple -- not allowed
  // NtupleRoot(Ntuple &nt); // "manual" copy constructor 4 from Ntuple to NtupleRoot
  NtupleRoot(const Ntuple &nt); // "manual" copy constructor 4 from Ntuple to NtupleRoot
  NtupleRoot(TTree *t1,unsigned int verbose=0); // constructor 5 from TTree
  virtual void FromTree(TTree *t1); // fill ntuple from tree
  virtual void Read_root(const char *fname,const char* treename="nt1"); // read root ntuple, calls FromTree
  virtual ~NtupleRoot() {};   // (empty) destructor
  virtual void WriteRoot(const char* NtupleName,const char* TreeName,const char* fname); // converts my ntuple to root tree and writes .root file
  TVectorD GetTVectorD(unsigned int ivar) const;   // get the TVectorD of variable ivar,    alternative is   to_TVectorD( vector or valarray), see ~/c/MyRoot/MyRoot.h
  TVectorD GetTVectorD(string VarNam) const;       // get the TVectorD of variable VarNam, corresponding to a column in the printed table
  // see ~/c/MyRoot/MyRoot.h  for conversion between valarray<double>, vector<double>, TVectorD
  // friend NtupleRoot NtupleMerge2(const NtupleRoot& Nt1,NtupleRoot& Nt2); // called NtupleMerge2  rather than NtupleMerge to avoid problems with CINT
};

// NtupleRoot* ReadAndMerge(const char* f1,const char* f2,unsigned int verbose=0);

// general utility, plotting
#include <TH2F.h>
#include <TGraph.h>
void My_Labels_TH1F(   TH1F* pl,const char* cx,const char* cy);
void My_Labels_TH2F(   TH2F* gr,const char* cx,const char* cy);
void My_Labels_Graph(TGraph* gr,const char* cx,const char* cy);

#endif
