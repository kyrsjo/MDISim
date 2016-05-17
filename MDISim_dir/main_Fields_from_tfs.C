/* ~/c/MDISim/main_Fields_from_tfs.C      H. Burkhardt 16/12/2014

 Extract fields from tfs files for easy input to Geant4

 Makes sense to keep field information which changes more often, separate from geometry information

 rm -rf "/tmp/$LOGNAME/"*

 Unix make
 proj=MDISim ; targ=Fields_from_tfs ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=Fields_from_tfs ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h

 Xcode
 proj=MDISim ; targ=MyNtuple2Geom ; cmakeXcode $proj

 --- LEIR
 cd /tmp/hbu ; Release/Fields_from_tfs -v ~/mad/makethin/leir/output/leir_optics.tfs LEIR_fields.dat | tee /tmp/hbu/Fields_from_tfs.out ; open /tmp/hbu/Fields_from_tfs.out
 cd /tmp/hbu ; cp -pf LEIR_fields.dat ~/www/Geom/

 -- fcc-ee
 OPTICSNAME="fcc_ee_t_42_6a_cw"
 OPTICSNAME="fcc_ee_t_42_7c_cw"
 cd /tmp/hbu ; Release/Fields_from_tfs -v  ~/mad/fcc-ee/Oide/output/"$OPTICSNAME"_twiss.tfs $OPTICSNAME.dat | tee /tmp/hbu/Fields_from_tfs.out ; open /tmp/hbu/Fields_from_tfs.out
 cd /tmp/hbu ; cp -pf $OPTICSNAME.dat ~/www/Geom/
 ls -lh  ~/www/Geom/fcc_ee*

 -- fcc-hh
 OPTICSNAME="fcc_ring_v4_baseline"
 Release/Fields_from_tfs -v ~/mad/makethin/leir/output/leir_optics.tfs LEIR_fields.dat | tee /tmp/hbu/Fields_from_tfs.out
 cd /tmp/hbu ; Release/Fields_from_tfs -v  ~/mad/fcc_hh/output/"$OPTICSNAME"_b2_twiss.tfs "$OPTICSNAME"_b2.dat | tee /tmp/hbu/Fields_from_tfs.out ; open /tmp/hbu/Fields_from_tfs.out
 cd /tmp/hbu ; cp -pf "$OPTICSNAME"_b2.dat ~/www/Geom/
 ls -lh  ~/www/Geom/fcc_ring_v4_baseline*

 */

#include "MyCppStdHeaders.H"
#include "TGeoManager.h"
#include "Ntuple.h"

#include "RunFlags.h"
#include <getopt.h>

void Fields_from_tfs(const char* tfsname,const char* outfname,unsigned int verbose=0)
{ // still hange
  if(verbose) cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << " tfsname=" << tfsname << " outfname=" << outfname << '\n';
  vector <string> tags(1); // will be resized in read
  Ntuple Nt(tags,&cout,verbose);
  Nt.Read_tfs(tfsname);
  if(verbose) Nt.PrintSummary();
  vector<string>   Name    =Nt.GetStrCol("NAME");
  vector<string>   Keyword =Nt.GetStrCol("KEYWORD");
  valarray<double> L       =Nt.GetVar("L");
  valarray<double> Angle   =Nt.GetVar("ANGLE");
  valarray<double> K1L     =Nt.GetVar("K1L");
  ofstream field_file(outfname);
  double sum_Angles=0;
  for(unsigned int i=0;i<Name.size();++i)
  {
    string Name_with_i=Name[i]+"_"+to_string(i); // as  GeomFromMyNtuple with AddIndexToName
    Name_with_i=CleanName(Name_with_i);// required to be able to export geometry to C++  ,verbose);
    if(Keyword[i].find("BEND")!=std::string::npos) Name_with_i+="_bend";
    else if(Keyword[i].substr(0,8)=="QUADRUPO") Name_with_i+="_quad";
    if(Keyword[i].find("BEND")!=std::string::npos || Keyword[i].find("QUADRUPOLE")!=std::string::npos) field_file << left << setw(16) << Name_with_i<< " " << setw(12) << Keyword[i] << " " << right << setw(13) << L[i] << " " << setw(13) << Angle[i] << " " << setw(13) << K1L[i] << '\n';
    sum_Angles+=Angle[i];
  }
  double diff_from_2_pi=2*M_PI-fabs(sum_Angles);
  if(verbose) cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << " done output in " << outfname << " sum_Angles=" << sum_Angles << " diff from 2pi=" << diff_from_2_pi << '\n';
  field_file.close();
}

void RunFlags::ShowUsage() const
{
  cout << "Help for " << argv0
  << " which is a unix command line utility to extract field information from tfs files. Written in 2014 by H. Burkhardt" << '\n'
  << BOLD << "SYNOPSIS" << NORMAL << '\n'
  << "    " << argv0 << " [OPTION] tfsname outfname" << '\n'
  << BOLD << "OPTIONS" << NORMAL << " " << cflags << '\n'
  << "    -h           just show this help" << '\n'
  << "    -v           verbose (turn debug on)" << '\n'
  << "    -V           more verbose (turn verbose>1 on)" << '\n'
  << BOLD << "DESCRIPTION" << NORMAL << '\n'
  << " read tfs files with Name, Keyword, L, Angle, K1L, K2L,.." << '\n'
  << " and write simple text output file with magnet names and field information in numbers" << '\n'
  << BOLD << "EXAMPLEs" << NORMAL << '\n'
  << argv0 << " -v ~/mad/makethin/leir/output/leir_optics.tfs LEIR_fields.dat 2>&1 | tee /tmp/hbu/Fields_from_tfs.out" << '\n'
  << argv0 << " -V ~/mad/makethin/leir/output/leir_optics.tfs LEIR_fields.dat 2>&1 | tee /tmp/hbu/Fields_from_tfs.out" << '\n'
  ;
}

int main(int argc,char * const argv[])
{
  const char* cflags="hvV";
  RunFlags Flag(argc,argv,cflags);
  if(argc<2) { Flag.ShowUsage(); cout << '\n' << "*** Error ***  more arguments or option -h needed" << '\n';  exit(1); }
  if(Flag.verbose>1 ) cout << __FILE__<< " " << __FUNCTION__ << " after RunFlags line " << setw(4) << __LINE__ << '\n';

  const char* tfsname  =Flag.ArgvVec[1].c_str();
  const char* outfname =Flag.ArgvVec[2].c_str();

  if(Flag.verbose>1)
  {
    cout << "ArgvVec.size()=" << Flag.ArgvVec.size()
    << " tfsname=" << tfsname
    << " outfname=" << outfname
    << '\n';
  }
  Fields_from_tfs(tfsname,outfname,Flag.verbose);
}
