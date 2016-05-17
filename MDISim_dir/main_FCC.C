/* ~/c/MDISim/main_FCC.C     used by   ~/mad/fcc-ee/BernhardHolzer_2013_10_11/TLEP_V9_HB.madx

 ----  work on Cones  Plot_Bend_SR_Cones      with   FCC -vP          see also   ~/www/Geom/my_Geom_Eve_Display.C   how to read and plot geom and tracks

 
 Unix make
 proj=MDISim ; targ=FCC ; cd /tmp/$LOGNAME ; cmakeUnix ~/c/$proj ; cd $proj ; make -j9 ; make install   ;              echo makes /tmp/$LOGNAME/$proj/Release/$targ
 proj=MDISim ; targ=FCC ; cd /tmp/$LOGNAME ; cmakeUnix ~/c/$proj ; cd $proj ; make $targ -j9 ; mv $targ Release/.
 
 then always
 cd /tmp/$LOGNAME/ ; time $proj/Release/$targ -h          2>&1 | tee /tmp/$LOGNAME/$targ.out ; open /tmp/$LOGNAME/$targ.out
 which is
 cd /tmp/hbu ; MDISim/Release/FCC -h

 

 Unix make
 proj=MDISim ;            cmakeUnix $proj ; cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=FCC ; cmakeUnix $proj ; cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h


 mkdir /tmp/$LOGNAME/Release
 rm -rf /tmp/$LOGNAME/CMake* ;  cd /tmp/$LOGNAME/Release ; cmake ~/c/MDISim ; make FCC -j5   ;   cd /tmp/$LOGNAME ; Release/FCC -h

 can also check syntax by line command
 cd ~/c/MDISim/ ; clang $MyClib_CFLAG -I$HOME/myroot -I$HOME/c/MDISim/ $MyRoot_CFLAG $ROOT_CFLAGS -c -o /dev/null main_FCC.C


 cd /tmp/$LOGNAME ; rm -f * ; Release/FCC ;  find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf

 cd /tmp/$LOGNAME ; time Release/FCC -h
 cd /tmp/$LOGNAME ; time Release/FCC -v   0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs
 cd /tmp/$LOGNAME ; time Release/FCC -vP  0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs  > /tmp/hbu/temp.out ; open /tmp/hbu/temp.out
 cd /tmp/$LOGNAME ; time Release/FCC -vPR 0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs

 next still need to update geometry
 cd /tmp/$LOGNAME ; time Release/FCC -v   0.001 1 ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_twiss.tfs ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_survey.tfs
 cd /tmp/$LOGNAME ; time Release/FCC -vPR 0.001 1 ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_twiss.tfs ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_survey.tfs
 fcc-hh
 cd /tmp/$LOGNAME ; time Release/FCC -v   0.001 1 ~/mad/fcc_hh/output/fcc_hh_race_v3_twiss.tfs ~/mad/fcc_hh/output/fcc_hh_race_v3_survey.tfs
 cd /tmp/$LOGNAME ; time Release/FCC -vPR 0.001 1 ~/mad/fcc_hh/output/fcc_hh_race_v3_twiss.tfs ~/mad/fcc_hh/output/fcc_hh_race_v3_survey.tfs


 together, with timing
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target TLEP -configuration Release ; time Release/FCC -vP 0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs >& /tmp/hbu/temp.out ; open /tmp/hbu/temp.out ; grep -i time /tmp/hbu/temp.out

 Make
 rm -f "/tmp/$LOGNAME/"* ; cd ~/c/MDISim/ ; make TLEP -j5 ; time TLEP ; cd /tmp/$LOGNAME ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf

 CMAKE
 mkdir /tmp/$LOGNAME/Release ; cd /tmp/$LOGNAME/Release ; cmake ~/c/MDISim ; make TLEP -j5 ; time TLEP > /tmp/$LOGNAME/TLEP.out ; open /tmp/$LOGNAME/TLEP.out ; cd /tmp/$LOGNAME ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf

 or in root, see   also   ~/mad/fcc-ee/BernhardHolzer_2013_10_11/TLEP_V9_HB.madx

 export PYTHONPATH=$ROOTSYS/lib:$ROOTSYS/geom/gdml       ; echo for gdml, seems was not needed

 ---  1. step just setup geometry, raw geometry from tfs    and export to root file,   no plotting, or better in batch see just below
 cd /tmp/$LOGNAME ; time root -b
 .L ~/c/MDISim/MyNtuple2Geom.C
 gSystem->Load("libGeom");
 unsigned int verbose=1;
 Ntuple nt_b1(ReadAndMerge("~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs","~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs",verbose)); if(verbose) nt_b1.PrintSummary();
 double zmin=0,zmax=180.,Scale_xy=100;
 const unsigned int color=kGray+100; //  // light grey, add 100 to get darker http://root.cern.ch/root/html/TColor.html
 verbose=0;GeomFromMyNtuple("TLEP","IP",nt_b1,zmin,zmax,color,Scale_xy,verbose); // in MyNtuple2Geom.C
 // gGeoManager->GetTopVolume()->Draw("ogl"); new TBrowser; // optional plotting/debug - then call without -b
 gGeoManager->CloseGeometry(); // 16 nodes/ 9 volume UID's in My3D
 gGeoManager->CheckGeometry();
 gGeoManager->Export("TLEP_geom.gdml"); // export in gdml format   -- ok when called first   http://root.cern.ch/root/html532/TGDMLWrite.html   GDMLEngine, ok when root made with  -Dgdml=ON
 gGeoManager->Export("TLEP_geom.root"); // export the geometry    http://root.cern.ch/root/html/TGeoManager.html
 gGeoManager->Export("TLEP_geom.C");    // export the geometry in C format
 gGeoManager->Export("TLEP_geom.xml");  // export in xml format
 .q

 or in line mode :
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target MyNtuple2Geom -configuration Release
 Release/MyNtuple2Geom -v TLEP_geom   IP 0 180 100 18 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs
   or
 Release/MyNtuple2Geom -v fcc-ee-geom IP 0 180 100 18 ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_twiss.tfs ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_survey.tfs


 --- 3d geometry plot with ogl ---       in screen   type e   for white background,  w  wireframe mode, r  filled polygons, t  outline mode
 cd /tmp/$LOGNAME ; time root
 gSystem->Load("libGeom");
 // TGeoManager::Import("fcc-ee-geom.root");  // only square box  no apertures defined yet
    TGeoManager::Import("TLEP_geom.root");    // ok
 // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom.root"); // ok, with color
 // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom.gdml"); // ok
 // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom.xml");  // ok
 // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom_ROOT5.root");
 TGeoVolume* top=gGeoManager->GetTopVolume();
 top->Draw("ogl"); // ftp://root.cern.ch/root/doc/ROOTUsersGuideHTML/ch09s12.html


 new TBrowser;  // start the root Browser   and inspect TLEP which shows Materials, Master Volume TGeoVolume::top  and  TGeoNoteMatrix::top_1



 --- 3d geometry plot with ogl ---   from .C   gROOT->LoadMacro   works with root5,  not with root6 which is more strict
 source ~/bin/my_Xcode_env.sh clang 1 root5
 cd /tmp/$LOGNAME ; $ROOTSYS/bin/root
 gROOT->LoadMacro("/Users/hbu/www/Geom/TLEP_geom.C");
 gInterpreter->ProcessLine("TLEP_geom()");
 gGeoManager->GetTopVolume()->Draw("ogl")

 --- read and plot Geometry, standard root
 cd /tmp/$LOGNAME ;  time $ROOTSYS/bin/root
 TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/TLEP_geom.root");
 gGeoManager->GetTopVolume()->Draw("ogl");


 --- read Geometry  in standard root with event display,  type letter e  to switch from black to white
 cd /tmp/$LOGNAME ;  time $ROOTSYS/bin/root
 .L ~/c/MDISim/myEve.C
 // StartEveWithGeomDisplay("TLEP_geom.root");
 StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/TLEP_geom.root");
 // add tracks for cone,  after  running   Release/FCC -vP  which makes the .root files
 .L ~/c/MDISim/PlotAddTrack.C
 PlotAddTrack("B07_0.root");  // or coneset.root,    currently not working,  empty
 PlotAddTrack("BD07_0.root");
 gEve->Redraw3D(kTRUE);

 // add beam divergence
 PlotAddTrack("BD07_div_0.root");
 PlotAddTrack("BD07_div_1.root");
 PlotAddTrack("BD07_div_2.root");
 PlotAddTrack("BD07_div_3.root");

 cd /tmp/$LOGNAME ; time root
 .L ~/c/MyRoot/init.C
 MDISim_init2();
 //.L ~/c/MDISim/Apertures.C .L ~/c/MDISim/IPpos.C .L ~/c/MDISim/Plot.C .L ~/c/MDISim/myGraph.C
 unsigned int verbose=0;
 // string MachName="~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175"; // TLEP
 string MachName="~/mad/fcc-ee/CERN-LM-21May14/FCC_ee"; // TLEP
 string Fname_b1_o=MachName+"_twiss.tfs";
 string Fname_b1_s=MachName+"_survey.tfs";
 Ntuple nt_b1( ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose) ); // Ntuple or  NtupleRoot to test  nt_b1.WriteRoot
 Beam b1(Fname_b1_o.c_str(),verbose); // get Energy and synchr integrals from optics twiss header
 b1.RFHV_from_twiss(nt_b1 );
 double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
 double EmitRatio=0.001;
 b1.EmitFromSynrad(EmitRatio);
 b1.Print();
 CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,verbose); // calculate beam sizes and divergences and add to ntuple
 verbose=1;
 if(verbose) nt_b1.PrintSummary();
 CalcSynrad(nt_b1,b1,verbose,nsig);
 b1.Touschek(nt_b1,verbose);


 Plot_optics(&nt_b1,verbose);
 // Plot_optics(&nt_b1,verbose0,1.e9); // full ring
 nt_b1.WriteAsciiNtFile(NULL,"/tmp/hbu/tlep_175_o_s.tfs"); // write combined optics and survey

 nt_b1.WriteRoot("nt_b1","nt1","/tmp/hbu/tlep_175_o_s.root");  // needs Tree library,  use   gSystem->Load("libTree");  in case of doubt


 --- 2d plot directly in root, here for tlep_v12a_cern optics   survey beam1, beam2 by slight rotation
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
 Lorentz dummy; // dummy to get MyClib types and constants, to avoid problems with my  ROOT 6.03/01
 .I /Users/hbu/myroot
 .L ~/c/MDISim/myEve.C
 .L ~/c/MDISim/Twiss_util.C
 .L ~/c/MDISim/Apertures.C
 .L ~/c/MDISim/IPpos.C
 .L ~/c/MDISim/myGraph.C
 .L ~/c/MDISim/Plot.C
 unsigned int verbose=1;
 const char* Fname_b1_o="~/mad/fcc-ee/tlep_v12a_cern/output/tlep_v12a_cern_twiss.tfs";
 const char* Fname_b1_s="~/mad/fcc-ee/tlep_v12a_cern/output/tlep_v12a_cern_b1_survey.tfs";
 Ntuple nt_b1(ReadAndMerge(Fname_b1_o,Fname_b1_s)); if(verbose) nt_b1.PrintSummary();
 Beam b1(Fname_b1_o,verbose); // get Energy and synchr integrals from optics twiss header
 verbose=2;
 b1.Get_Part().PrintLorentz(cout);
 b1.Print();
 Plot_optics(&nt_b1,verbose); c1->SaveAs("/tmp/$LOGNAME/tlep_optics.eps");
 Plot_survey(&nt_b1,verbose); //  Plot.C  full ring, /tmp/hbu/ring_o_s.eps
 .q
 find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf
 open "/tmp/$LOGNAME/"*.pdf



 --- local geom generation - useful before cone plotting
 rm -rf "/tmp/$LOGNAME/Release"* ; mkdir /tmp/$LOGNAME/Release ; cd /tmp/$LOGNAME/Release ; cmake ~/c/MDISim      ; cd /tmp/$LOGNAME/Release ; make MyNtuple2Geom -j9
 cd /tmp/$LOGNAME ; Release/MyNtuple2Geom -v fcc-ee-geom IP 0 500 100 1020 ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_twiss.tfs ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_survey.tfs


 //-- older 21May2014
 // string MachName="~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175"; // TLEP
 string MachName="~/mad/fcc-ee/CERN-LM-21May14/FCC_ee"; // TLEP
 string Fname_b1_o=MachName+"_twiss.tfs";
 string Fname_b1_s=MachName+"_survey.tfs";
 Ntuple nt_b1(ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose)); // Merge here to NtupleRoot, to be able to make a root tree file out of it with WriteRoot  - not sure that is actually useful
 // StartEveWithGeomDisplay("fcc-ee-geom.root");  // for CERN-LM-21May14     locally generated
 // StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/TLEP_geom.root"); goto_CM_units=false;  // works only with BernhardHolzer_2013_10_11  -- still in meter units
 // StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/fcc-ee-geom.root");  // for CERN-LM-21May14
 StartEveWithGeomDisplay(ResolveHome("~/www/Geom/fcc-ee-geom.root"));


 --- fcc-hh
 cd /tmp/$LOGNAME ; time root
 .L ~/c/MyRoot/init.C
 MDISim_init();
 unsigned int verbose=0;
 string MachName="~/mad/fcc_hh/output/fcc_hh_ring_v3";
 // string MachName="~/mad/fcc_hh/output/fcc_hh_race_v3";
 // string MachName="~/mad/fcc_hh/output/fcc_hh_race_v3_before_cycle";
 string Fname_b1_o=MachName+"_twiss.tfs";
 string Fname_b1_s=MachName+"_survey.tfs";
 Ntuple nt_b1(ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose)); if(verbose) nt_b1.PrintSummary();
 Beam b1(Fname_b1_o.c_str(),verbose); // get Energy and synchr integrals from optics twiss header
 b1.RFHV_from_twiss(nt_b1 );
 b1.Get_Part().PrintLorentz(cout);
 double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
 double emitxN=2.2e-6,emityN=2.2e-6; // normalized emittance
 b1.SetEmitN(emitxN,emityN);
 Lorentz Part=b1.Get_Part();
 b1.Print();
 CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,verbose); // calculate beam sizes and divergences and add to ntuple
 verbose=1; if(verbose) nt_b1.PrintSummary();
 CalcSynrad(nt_b1,b1,verbose,nsig);
    StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/fcc_hh_ring.root");
 // StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/fcc_hh_race.root");
 // StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/fcc_hh_ring.root");
 double Scale_xy=100;
 bool goto_CM_units=true; // normally true
 // double zmin=0,zmax=500; // race, as for geometry generation or less
 double zmin=-600,zmax=600; // ring, as for geometry generation or less
 verbose=1;
 Plot_Bend_SR_Cones(nt_b1,b1,zmin,zmax,Scale_xy,verbose,goto_CM_units,-1); // myGraph3D.C



 ----------------
 Naming conventions and data structures
 b1_o, b1_s, b1    beam 1 optics and survey, and combined
 b2_o, b2_s, b2    beam 2 optics and sruvey,  if missing means same as b1
 The tfs table information is stored in my root ntuples,  optics and survey is joined to a single ntuple
 The header information from the optics files is read by


 -- read back the .root ntuple   using standard root
 cd /tmp/$LOGNAME ;  time $ROOTSYS/bin/root
 TFile* f1=new TFile("/tmp/hbu/tlep_175_o_s.root");
 f1->ls();
 TTree *t1 = (TTree*) f1->Get("nt1");
 // t1->Print();
 t1->StartViewer();
 t1->Draw("BETX:S","","L");
 t1->Draw("DIVX:S","","L");


 --- read back gdml ---
 cp -pf $G4INSTALL/examples/extended/persistency/"gdml/G01/"*.mac /tmp/hbu/ ;  echo really needed, otherwise 'Top' is not unique errors
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml ~/www/Geom/TLEP_geom.gdml   ; echo test to read gdml for geant4, ok
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml ~/www/Geom/LEIR.gdml        ; echo test to read gdml for geant4, ok
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml ~/www/Geom/LHC_IR_5_LR.gdml ; echo test to read gdml for geant4, ok, complex geometry, reading and drawing very fast
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml            TLEP_geom.gdml   ; echo test to read gdml for geant4, ok




 -- test synrad for dafne   run    MRe_twiss.madx  emittance  expect 0.24e-6 m  or rather 0.36e-6 from synchr. rad integrals    here get  465.097 nm = 0.465e-6    roughly same

 cd /tmp/$LOGNAME ; time root
  .I /Users/hbu/myroot
 .L ~/c/MyRoot/init.C
 MDISim_init();
 unsigned int verbose=1;
 Ntuple nt_b1(ReadAndMerge("~/mad/dafne_2014/trunk/outputs/dafne_thick_twiss.tfs","~/mad/dafne_2014/trunk/outputs/dafne_survey.tfs",verbose)); if(verbose) nt_b1.PrintSummary(); // read twiss and survey locally generated on tmp
 Beam b1("~/mad/dafne_2014/trunk/outputs/dafne_thick_twiss.tfs",verbose); // get Energy and synchr integrals from optics twiss header
 // b1.RFHV_from_twiss(nt_b1 ); // seems to set to here  RFHV=0.00036 GV
 b1.SetRF( 0.00018, 120 ); // 180 kV and Harm = 120
 double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
 EmitRatio=0.004; // Phys. Rev. ST Accel. Beams 15, 104201 (2012)  p. 104201-9  0.4% beam coupling
 // b1.SetVerbose(2);
 b1.EmitFromSynrad(EmitRatio);
 b1.Print();
 CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,verbose); // calculate beam sizes and divergences and add to ntuple
 verbose=1; if(verbose) nt_b1.PrintSummary();
 CalcSynrad(nt_b1,b1,verbose,nsig);
 b1.Touschek(nt_b1,verbose); // 14 min about as expected
 nt_b1.WriteAsciiNtFile(NULL,"/tmp/hbu/dafne_o_s.out"); // keep in ~/mad/dafne_2014/trunk/outputs/


 .q


 ---- plot the twiss functions and derived functions
  rm -f "/tmp/$LOGNAME/"*.eps ; rm -f "/tmp/$LOGNAME/"*.pdf
  cd /tmp/$LOGNAME ; root
    unsigned int verbose=1;
  Ntuple nt_b1("~/mad/dafne_2014/trunk/outputs/dafne_o_s.out"); if(verbose) nt_b1.PrintSummary(); // "/tmp/hbu/dafne_o_s.out"

  Plot_x_y1_y2(nt_b1,"S","BETX","BETY");
  Plot_x_y1_y2(nt_b1,"S","SIGX","SIGY");
  Plot_x_y1_y2(nt_b1,"S","SIGX","SIGXD");
  Plot_x_y1_y2(nt_b1,"S","SIGXD","SIGXP");

  Plot_x_y(nt_b1,"S","DIVXP"); // can zoom inside
  Plot_x_y(nt_b1,"S","HS");

  .q
  find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf


  zoom/edit plot in root :




 */

// grep SetMainColor ~/root_git/*/*/inc/*.h

#include "MyCppStdHeaders.H"
#include <TRint.h>
#include <TEveManager.h>

#include "MyRootConst.h"
#include "Lorentz.h"
#include "Twiss_util.h"
#include "myGraph3D.h"
#include "Plot.h"
#include "myEve.h" // StartEveWithGeomDisplay

#include "RunFlags.h"
#include <getopt.h>

void RunFlags::ShowUsage() const
{
  cout << "Help for " << argv0
  << " which is a unix command line utility to check if all unix logical links exist. Written in 2014 by H. Burkhardt" << '\n'
  << BOLD << "SYNOPSIS" << NORMAL << '\n'
  << "    " << argv0 << " [OPTION] EmitRatio nsig Opt1 Surv1" << '\n'
  << BOLD << "OPTIONS" << NORMAL << " " << cflags << '\n'
  << "    -h           just show this help" << '\n'
  << "    -p           plot optics" << '\n'
  << "    -P           plot Cones" << '\n'
  << "    -R           run ROOT interactively - currently crashes" << '\n'
  << "    -v           verbose (turn debug on)" << '\n'
  << "    -V           more verbose (turn verbose>1 on)" << '\n'
  << BOLD << "EXAMPLEs" << NORMAL << '\n'
  << argv0 << " -v" << '\n'
  << argv0 << " -v   0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs" << '\n'
  << argv0 << "      0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs" << '\n'
  << argv0 << " -vP  0.001 1 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs 2>&1 | tee /tmp/$LOGNAME/$targ.out ; open /tmp/$LOGNAME/$targ.out" << '\n'
  ;
}

// int main(int argc,char * const argv[]) // normally this
int main(int argc,char *  argv[]) // const argv[]   not compatible with TRint
{
  const char* cflags="hpPRvV";
  RunFlags Flag(argc,argv,cflags);
  if(Flag.verbose>1 ) cout << __FILE__<< " " << __FUNCTION__ << " after RunFlags line " << setw(4) << __LINE__ << '\n';

  TRint* theApp=NULL;
  if(Flag.R) // Root window
  {
    argc=1;
    theApp = new TRint("", &argc, argv ); // root in line mode, defined theApp
  }
  else theApp = new TRint("", &argc, argv );

  if(Flag.ArgvVec.size()<5) { cout << "*** Error, missing input parameters, ArgvVec.size()=" << Flag.ArgvVec.size() << " should be at least 5" << '\n'; exit(1) ; }

  double EmitRatio=stod(Flag.ArgvVec[1]);   // http://www.cplusplus.com/reference/string/stod/
  double      nsig=stod(Flag.ArgvVec[2]);
  const char* Fname_b1_o= Flag.ArgvVec[3].c_str();
  const char* Fname_b1_s= Flag.ArgvVec[4].c_str();

  if(Flag.verbose>1)
  {
    cout << "ArgvVec.size()=" << Flag.ArgvVec.size()
    << " EmitRatio=" << EmitRatio
    << " Fname_b1_o=" << Fname_b1_o
    << " Fname_b1_s=" << Fname_b1_s
    << '\n';
  }
  if(Flag.ArgvVec.size()<5) exit(1);

  cout << CPUTime() << '\n';
  Ntuple nt_b1(ReadAndMerge(Fname_b1_o,Fname_b1_s,Flag.verbose));
  cout << CPUTime() << '\n';
  nt_b1.RemoveVarList({"E1","E2","GLOBALTILT","SLOT_ID","ASSEMBLY_ID","MECH_SEP","V_POS"}); // C++11
  cout << CPUTime() << '\n';

  Beam b1(Fname_b1_o,Flag.verbose); // get Energy and synchr integrals from optics twiss header
  b1.RFHV_from_twiss(nt_b1 );
  b1.Print();

  // double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
  b1.EmitFromSynrad(EmitRatio);
  CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,Flag.verbose); // calculate beam sizes and divergences and add to ntuple
  CalcSynrad(nt_b1,b1,Flag.verbose,nsig);
  if(Flag.verbose) nt_b1.PrintSummary();
  if(Flag.p) Plot_optics(&nt_b1,Flag.verbose);

  double Scale_xy=100;
  if(Flag.P) // Eve needs root setup with TRint and  TEveManager::Create,   some things work without but danger of segemntation violation
  {
    Flag.verbose=2;
    if(Flag.verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';
    // if(!Flag.R) theApp = new TRint("", &argc, argv );  // Eve needs Root,  dummy root unless Flag.R
    if(Flag.R) StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/TLEP_geom.root"); else TEveManager::Create(kFALSE); // setup Eve       with root selected also show geometry
    double zmin=0,zmax=500;
    Plot_Bend_SR_Cones(nt_b1,b1,zmin,zmax,Scale_xy,Flag.verbose,-1); // myGraph3D.C
  }

  //nt_b1.WriteAsciiNtFile(NULL,"/tmp/hbu/tlep_175_o_s.tfs"); // write combined optics and survey
  // double xmin=-0.3,xmax=0.15,zmin=0,zmax=250.;
  //Plot_survey(&nt_b1,Flag.verbose,xmin,xmax,zmin,zmax);

  if(Flag.R)
  {
    gEve->Redraw3D(kTRUE);
    cout << " running in line mode. When done quit root by the usual .q  or from the menu bar / File" << '\n';
    theApp->Run();
  }
};
