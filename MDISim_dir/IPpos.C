//  ~/c/MDISim/IPpos.C     H. Burkhardt 5/2013
//
//  http://www.cplusplus.com/doc/tutorial/control/


#ifndef __CLING__
#include "MyCppStdHeaders.H"
#endif

#include "IPpos.h"

// #include <ctype.h> //   http://www.cplusplus.com/reference/cctype/isdigit/

IPpos::IPpos(const Ntuple* nt,unsigned int verbose) // constructor          did not work well CINT,   ok when compiled
{
  if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " nt->Noent()=" << nt->Noent() << '\n';
  this->verbose=verbose;
  const string MatchStr="IP";
  const string ColumnName="NAME";
  vector<string> nt_StrCol=nt->GetStrCol(ColumnName);
  if(verbose>1)
  {
	cout << "IPpos matching MatchStr=" << MatchStr
	<< " nt_StrCol[0]=" << nt_StrCol[0]
	<< " nt_StrCol.size()=" << nt_StrCol.size()
	<< '\n';
  }
  unsigned int is=nt->ivar("S");
  unsigned int ix=nt->ivar("X_2"); // x_2  is x from survey in global Euclidian coordinates
  unsigned int iy=nt->ivar("Y");
  unsigned int iz=nt->ivar("Z");
  bool has_betx=nt->VarExists("BETX");
  bool has_bety=nt->VarExists("BETY");
  unsigned int nfound=0;
  vector<unsigned int> iElemList;
  for(unsigned int ient=0;ient<nt->Noent();++ient)
  {
    string ElementName=nt_StrCol[ient];
    if(StartsWith(ElementName,MatchStr))
    {
      if(verbose>1) cout << " found " << ElementName << " length()=" << ElementName.length() << '\n';
	  if(StartsWith(ElementName,MatchStr) && ElementName.length()>2)
	  {
        const char cip=ElementName.at(2); // http://www.cplusplus.com/reference/string/string/at/      here read the character corresponding to 1  from IP1
        unsigned int IP=atoi(&cip);  // for "IP" as in case of CLIC< cip will be '"'  resulting in 0
        if(verbose) cout << " cip=" << cip << " IP=" << IP << "   IPVec.size()=" << IPVec.size() << '\n';
        if(IP<20)  // make sure the number of IP is reasonable
        {
          nfound++;
          // unsigned int i=IP-1;
          IPVec.push_back(IP);
          s.push_back(nt->Entry(ient,is));
          x.push_back(nt->Entry(ient,ix));
          y.push_back(nt->Entry(ient,iy));
          z.push_back(nt->Entry(ient,iz));
          if(has_betx) betx.push_back(nt->Entry(ient,nt->ivar("BETX"))); else betx.push_back(0);
          if(has_bety) bety.push_back(nt->Entry(ient,nt->ivar("BETY"))); else bety.push_back(0);
        }
      }
	}
  }
  if(verbose>1)
  {
    if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " end MatchStr=" << MatchStr << " nfound=" << nfound <<  " matches  IPVec.size()=" << IPVec.size() << '\n';
  }
}

void IPpos::Print()
{
  if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " IPVec.size()=" << IPVec.size() << '\n';
  cout << "IP              s              x              y              z        betx        bety" << '\n';
  for(unsigned int i=0;i<IPVec.size();++i)
  {
	setprecision(3);
	cout << IPVec[i] << " "
	<< setw(15) << s[i]
	<< setw(15) << x[i]
	<< setw(15) << y[i]
	<< setw(15) << z[i]
	<< setw(12) << betx[i]
	<< setw(12) << bety[i]
	<< '\n';
  }
  if(verbose) cout << "IPpos::Print() done IPVec.size()=" << IPVec.size() << '\n';
}

unsigned int IPpos::ind(unsigned int IP)  // index of IP
{
  unsigned int ii=0;
  for(unsigned int i=0;i<IPVec.size();++i)
  {
	if(IPVec[i]==IP) return ii;
  }
  return ii;
}

void CalcAveBeam(const Ntuple &nt_b1,const Ntuple &nt_b2,const IPpos* ips_b1,const IPpos* ips_b2, unsigned int verbose) // for the moment only checking if between D1 and D2, called by  FindIPs from main_LHC.C
{
  // special attention for region between D1 and D2   where  x_2  between 0  and  0.0969999
  // D1   MBXW.A4 .. MBXW.F4    in IR1, 5         look for  "MBXW.
  // D1   MBX.4                 in IR2, 8         look for  "MBX.4
  // D2   MBRC.4                in all IPs        look for  "MBRC.4
  vector<string> Name1=nt_b1.GetStrCol("NAME");
  vector<string> Name2=nt_b2.GetStrCol("NAME");

  unsigned int ndim1=nt_b1.Noent(); unsigned int is=nt_b1.ivar("S");
  // unsigned int ndim2=nt_b2.Noent();
  bool BetweenD1andD2=false;
  if(verbose) cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << endl;
  for(unsigned int i1=0;i1<ndim1;++i1)
  {
	unsigned int len=Name1[i1].length();
	bool D11=StartsWith(Name1[i1],"MBXW.");
	bool D12=StartsWith(Name1[i1],"MBX.4");
	bool D2 =StartsWith(Name1[i1],"MBRC.4");
    if( D11 || D12)
	{
      BetweenD1andD2=true;
      string cip=Name1[i1].substr(len-1,1); // get the IP number for the magnet name
      unsigned int IP=atoi(cip.c_str());
	  const char lr =Name1[i1].at(len-2); // L or R
	  double spos=nt_b1.Entry(i1,is);
	  double dist_to_IP=spos-ips_b1->s[IP-1];
	  cout << "Name1[i1]=" << setw(12) << Name1[i1] << " D1 IP=" << IP << " lr=" << lr << " s=" << setw(10) << spos << " dist_to_IP="  << setw(10) << dist_to_IP
      << " aper_1=" << setw(10) << nt_b1.Entry(i1,nt_b1.ivar("APER_1"))
      << " x_2="    << setw(10) << nt_b1.Entry(i1,nt_b1.ivar("X_2"))
      << '\n';
	}
    if( D2 )
	{
      BetweenD1andD2=true;
      string cip=Name1[i1].substr(len-4,1); // get the IP number for the magnet name
      unsigned int IP=atoi(cip.c_str());
	  const char lr =Name1[i1].at(len-5); // L or R
	  double spos=nt_b1.Entry(i1,is);
	  double dist_to_IP=spos-ips_b1->s[IP-1];
	  cout << "Name1[i1]=" << setw(12) << Name1[i1] << " D2 IP=" << IP << " lr=" << lr << " s=" << setw(10) << spos << " dist_to_IP="  << setw(10) << dist_to_IP
      << " aper_1=" << setw(10) << nt_b1.Entry(i1,nt_b1.ivar("APER_1"))
      << " x_2="    << setw(10) << nt_b1.Entry(i1,nt_b1.ivar("X_2"))
      << '\n';
	}
  }
}
