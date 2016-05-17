//   ~/c/MDISim/IPpos.h     H. Burkhardt 11/2013

#ifndef IPpos_h
#define IPpos_h 1

class IPpos
{
  public:
  unsigned int verbose;
  vector<unsigned int> IPVec;
  vector<double> s;
  vector<double> x;
  vector<double> y;
  vector<double> z;
  vector<double> betx;
  vector<double> bety;
  IPpos(const Ntuple* nt,unsigned int verbose); // constructor, filled from tfs table via ntuple
  void Print();
  unsigned int ind(unsigned int IP); // index of IP
};

void CalcAveBeam(const Ntuple &nt_b1,const Ntuple &nt_b2,const IPpos* ips_b1,const IPpos* ips_b2, unsigned int verbose);

#endif
