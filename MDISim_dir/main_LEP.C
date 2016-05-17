/* ~/c/MDISim/main_LEP.C           similar to Plot_TLEP.C

 rm -rf "/tmp/$LOGNAME/"*

 Unix make
 proj=MDISim ;            cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=LEP ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h

 Xcode
 proj=MDISim ; cmakeXcode $proj

 --- run in line mode by
 cd /tmp/$LOGNAME  ; rm -f * ; time Release/LEP ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf

 ---  1. step just setup geometry, raw geometry from tfs    and export to root file,   no plotting --  no apertures, not really meaningful, empty plot
 cd /tmp/$LOGNAME ; time ~/lib/4.8/root_1a -b         ; echo use static version with all libs, gSystem->Load libGeom not enough
 .L ~/c/MDISim/MyNtuple2Geom.C
 unsigned int verbose=1;
 Ntuple nt_b1(ReadAndMerge("~/mad/LEP/lep.tfs","~/mad/LEP/lep_survey.tfs",verbose)); if(verbose) nt_b1.PrintSummary();
 double zmin=0,zmax=320,Scale_xy=100;
 gSystem->Load("libGeom");
 verbose=1;GeomFromMyNtuple("LEP","IP",nt_b1,zmin,zmax,kBlue,Scale_xy,verbose);
 gGeoManager->Export("LEP.root");  // export the whole geometry
 .q


 --- 3d geometry plot with ogl ---   empty plot, no apertures defined
 cd /tmp/$LOGNAME ; time root
 gSystem->Load("libGeom");
 TGeoManager::Import("LEP.root");
 gGeoManager->GetTopVolume()->Draw("ogl");



 --- directly in root
 cd /tmp/$LOGNAME ; time root
  .I /Users/hbu/myroot
 .L ~/c/MDISim/Twiss_util.C
 .L ~/c/MDISim/Apertures.C
 .L ~/c/MDISim/IPpos.C
 .L ~/c/MDISim/myGraph.C
 .L ~/c/MDISim/Plot.C
 unsigned int verbose=1;
 const char* Fname_b1_o="~/mad/LEP/f0520b98v1/lep_twiss.tfs";
 const char* Fname_b1_s="~/mad/LEP/f0520b98v1/lep_survey.tfs";
 Ntuple nt_b1(ReadAndMerge(Fname_b1_o,Fname_b1_s,verbose));
 verbose=2;
 Beam b1(Fname_b1_o,verbose); // get Energy and synchr integrals from optics twiss header
 b1.RFHV_from_twiss(nt_b1);
 if(b1.RFHV==0) b1.SetRF( 4, 31320 ); // if not set, then do here
 EmitRatio=0.002;
 b1.EmitFromSynrad(EmitRatio);
 b1.Print();
 CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,verbose); // calculate beam sizes and divergences and add to ntuple
 verbose=1;
 double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
 CalcSynrad(nt_b1,b1,verbose,nsig);
 b1.Touschek(nt_b1,verbose); //  1069.67h  rather, but maybe not completely negligible

 Plot_optics(&nt_b1,verbose);
 Plot_optics(&nt_b1,verbose,0,1.e9); // full ring
 double xmin=-0.3,xmax=0.15,zmin=-300.,zmax=300.;
 Plot_survey(&nt_b1,verbose,xmin,xmax,zmin,zmax);
 // Plot_survey(&nt_b1,verbose); // full ring


 with results
 i_Rbend_List.size=1783
 i_quads.size()=844
 iele            NAME        KEYWORD          S          L       Angle      Emean   ngamBend      rho         B        BETX      SIGX     divx
 m          m                  MeV                     m         T          m        mm      mrad
 34   WIG4M.QL4A.R1          RBEND    60.1490     0.4300   0.000000  0.000000  0.000000    0.000000    0.000000   60.9697    1.6514    0.0271
 38   WIG3M.QL4A.R1          RBEND    66.9005     0.4300   0.000000  0.000000  0.000000    0.000000    0.000000   46.6906    1.4451    0.0310
 40   WIG2M.QL4A.R1          RBEND    68.3005     0.4300   0.000000  0.000000  0.000000    0.000000    0.000000   44.1822    1.4057    0.0318
 50   WIG1M.QL4B.R1          RBEND    75.8350     0.4300   0.000000  0.000000  0.000000    0.000000    0.000000   50.9772    1.5100    0.0296
 110     BW1.QL11.R1          RBEND   259.7414    11.5500   0.000377  0.022284  0.77668930652.046544    0.010882   80.1141    1.8929    0.0236
 112     BW2.QL12.R1          RBEND   271.6314    11.5500   0.000377  0.022284  0.77668930652.046544    0.010882   42.3861    1.3769    0.0325
 120     B2L.QL12.R1          RBEND   286.8114    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   59.6013    1.6327    0.0274
 122     B2R.QL13.R1          RBEND   298.7014    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   90.3823    2.0106    0.0222
 128    B2LT.QL13.R1          RBEND   313.8814    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   30.2747    1.1637    0.0384
 130    B2RT.QL14.R1          RBEND   325.7714    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823    8.0392    0.5996    0.0746
 138     B2L.QL14.R1          RBEND   340.9514    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   44.7118    1.4141    0.0316
 140     B2R.QL15.R1          RBEND   352.8414    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823  122.6693    2.3423    0.0191
 146     B2L.QL15.R1          RBEND   368.0214    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   80.2263    1.8943    0.0236
 148     B2R.QL16.R1          RBEND   379.9114    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   42.8372    1.3842    0.0323
 156     B2L.QL16.R1          RBEND   395.0914    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   70.6752    1.7779    0.0252
 158     B2R.QL17.R1          RBEND   406.9814    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823  114.7851    2.2658    0.0197
 166     B2S.QL18.R1          RBEND   431.0594    11.5500   0.003766  0.222717  7.762564 3066.913358    0.108762   34.9882    1.2510    0.0358
 176     B2L.QL18.R1          RBEND   455.5314    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823   42.4676    1.3782    0.0325
 178     B2M.QF19.R1          RBEND   467.4214    11.5500   0.003770  0.222966  7.771219 3063.497853    0.108883   85.0036    1.9499    0.0229
 180     B2R.QF19.R1          RBEND   479.3114    11.5500   0.003768  0.222842  7.766891 3065.204654    0.108823  146.8956    2.5632    0.0174
 from summing up energy loss U0=2.9234 GeV  elemList.size()=1783
 Start of PrintQuads plane=x
 Part e+  P=   100 GeV E=      100 GeV m=0.000510999 GeV gamma= 195695 beta=0.999999999986944 1-beta=1.306e-11 beta*gamma=195695 Ekin=100 GeV
 with synchrotron radiation in quads at offset of nsig=0.0000 sigma  emit=44.7266nm
 iele         Element          s      beta     sigma           K1L            k0       L            x        Angle      Emean   ngamBend
 m         m        mm           m-2           m-1       m           mm                     MeV
 10         QL1A.R1     23.816    90.600     2.013      -0.02588       0.00000 2.00000      0.04305     0.000002   0.000761   0.004592
 12         QL1B.R1     26.316   110.405     2.222      -0.02588       0.00000 2.00000      0.04572     0.000002   0.000808   0.004877
 22         QL2A.R1     30.316   153.020     2.616       0.03013       0.00000 2.00000      0.05126     0.000003   0.001055   0.006366
 24         QL2B.R1     32.816   155.722     2.639       0.03013       0.00000 2.00000      0.05031     0.000003   0.001035   0.006249
 42         QL4A.R1     71.820    40.384     1.344      -0.02851       0.00000 1.60000     -0.00028     0.000000   0.000005   0.000026
 44         QL4B.R1     73.920    44.136     1.405      -0.02851       0.00000 1.60000     -0.00299     0.000000   0.000058   0.000281
 56          QL5.R1     96.992   152.104     2.608       0.05524       0.00000 2.00000     -0.03197     0.000004   0.001206   0.007281
 62          QL6.R1    121.250    11.860     0.728      -0.06216       0.00000 2.00000     -0.02309     0.000003   0.000980   0.005917
 72          QL7.R1    150.595    70.234     1.772       0.04280       0.00000 2.00000     -0.04967     0.000004   0.001452   0.008763
 86          QL8.R1    174.854    53.128     1.542      -0.04964       0.00000 2.00000     -0.02257     0.000002   0.000765   0.004618
 94          QL9.R1    199.112   220.609     3.141       0.04280       0.00000 2.00000     -0.02102     0.000002   0.000614   0.003708
 100         QL10.R1    223.371    69.169     1.759      -0.04964       0.00000 2.00000      0.00212     0.000000   0.000072   0.000433
 108         QL11.R1    247.629   135.820     2.465       0.03340       0.00000 2.00000      0.02573     0.000002   0.000587   0.003543
 118         QL12.R1    274.699    38.790     1.317      -0.04500       0.00000 2.00000      0.04032     0.000004   0.001239   0.007480
 126         QL13.R1    301.769    90.131     2.008       0.05218       0.00001 2.00000      0.18055     0.000019   0.006435   0.038837
 136         QL14.R1    328.839     8.855     0.629      -0.04196       0.00001 2.00000      0.23408     0.000020   0.006709   0.040488
 144         QL15.R1    355.909   135.274     2.460       0.05187       0.00003 2.00000      0.63905     0.000066   0.022642   0.136653
 154         QL16.R1    382.979    39.921     1.336      -0.05250       0.00002 2.00000      0.32516     0.000034   0.011661   0.070379
 164         QL17.R1    410.049   118.312     2.300       0.04139       0.00002 2.00000      0.55687     0.000046   0.015744   0.095021
 172         QL18.R1    442.479    18.065     0.899      -0.05168       0.00001 2.00000      0.22496     0.000023   0.007942   0.047930
 from summing up energy loss in the 844 quads U0=0.005908 GeV plane=x
 FindElement Element     1 is                IP1 at s=0.000000 m BETX=71.525517 BETY=35.833329 m




 */

#include "MyCppStdHeaders.H"
#include <TRint.h>

#include "MyRootConst.h"
//#include "Ntuple.h"
#include "Lorentz.h"
#include "Twiss_util.h"
#include "myGraph.h"
#include "myGraph3D.h"
#include "MyNtuple2Geom.h"
#include "Plot.h"

#include "RunFlags.h"
#include <getopt.h>

void RunFlags::ShowUsage() const
{
  cout << "Help for " << argv0
  << " which is a unix command line utility to check if all unix logical links exist. Written in 2014 by H. Burkhardt" << '\n'
  << BOLD << "SYNOPSIS" << NORMAL << '\n'
  << "    " << argv0 << " [OPTION] DirName" << '\n'
  << BOLD << "OPTIONS" << NORMAL << " " << cflags << '\n'
  << "    -h           just show this help" << '\n'
  << "    -p           plot optics" << '\n'
  << "    -R           run ROOT" << '\n'
  << "    -v           verbose (turn debug on)" << '\n'
  << "    -V           more verbose (turn verbose>1 on)" << '\n'
  << BOLD << "EXAMPLEs" << NORMAL << '\n'
  << argv0 << " -v" << '\n'
  << argv0 << " -vpR 2>&1 | tee /tmp/$LOGNAME/$targ.out ; open /tmp/$LOGNAME/$targ.out" << '\n'
  ;
}

// int main(int argc,char * const argv[]) // normally this
int main(int argc,char *  argv[]) // const argv[]   not compatible with TRint
{
  const char* cflags="hpRvV";
  RunFlags Flag(argc,argv,cflags);
  if(Flag.verbose>1 ) cout << __FILE__<< " " << __FUNCTION__ << " after RunFlags line " << setw(4) << __LINE__ << '\n';
  const char* Fname_b1_o="~/mad/LEP/lep_twiss.tfs";
  const char* Fname_b1_s="~/mad/LEP/lep_survey.tfs";
  Ntuple nt_b1(ReadAndMerge(Fname_b1_o,Fname_b1_s,Flag.verbose));

  TRint* theApp;
  if(Flag.R) theApp = new TRint("", &argc, argv, NULL, 0); // root in line mode, defined theApp

  Beam b1(Fname_b1_o,Flag.verbose); // get Energy and synchr integrals from optics twiss header
  b1.RFHV_from_twiss(nt_b1 );
  b1.Print();

  double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
  double EmitRatio=0.002;
  b1.EmitFromSynrad(EmitRatio);
  CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,Flag.verbose); // calculate beam sizes and divergences and add to ntupl
  CalcSynrad(nt_b1,b1,Flag.verbose,nsig);
  nt_b1.PrintSummary();
  if(Flag.p) Plot_optics(&nt_b1,Flag.verbose);

  //nt_b1.WriteAsciiNtFile(NULL,"/tmp/hbu/tlep_175_o_s.tfs"); // write combined optics and survey
  // double xmin=-0.3,xmax=0.15,zmin=0,zmax=250.;
  //Plot_survey(&nt_b1,Flag.verbose,xmin,xmax,zmin,zmax);

  if(Flag.R)
  {
    cout << " running in line mode. When done quit root from the menu bar / File " << '\n';
    theApp->Run();
  }
};
