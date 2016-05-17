/* ~/c/MDISim/main_AddTracksToGeom.C

 needs ntuple and MyClib for Courant Snyder to Euclidian transformation,   $ROOTSYS/bin/root not sufficient, needs my root

 next :   compare   rmatrix tracking  and MAD-X tracking  see  TwissTrack_tfs   which uses   lhc.xcodeproj  twiss_track

 rm -rf "/tmp/$LOGNAME/"*

 source ~/bin/my_Xcode_env.sh clang 1 root_git
 source ~/bin/my_Xcode_env.sh clang 1 root6
 source ~/bin/my_Xcode_env.sh clang 1 root5

 check:   cout << __CLING__ << endl;  // should show 1


 Unix make
 proj=MDISim ;                        cmakeUnix $proj ; cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=AddTracksToGeom ; cmakeUnix $proj ; cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h

 ok in line mode
 MDISim/AddTracksToGeom -v  0 330 416 100 http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root ~/mad/lhc/output/lhcb1_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b1.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b2.tfs



 1) nice demo, opens Eve Window,  preferred adding of tracks        tracks in  ~/mad/lhc/twiss_track_ref/0*_px*b1.tfs
 cd /tmp/$LOGNAME ; Release/AddTracksToGeom -V       0 330 416 100 http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root ~/mad/lhc/output/lhcb1_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b1.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b2.tfs
   or both left and right, with tracks only generated right so far
 cd /tmp/$LOGNAME ; Release/AddTracksToGeom -v -- -330 330 416 100 http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5_LR.root ~/mad/lhc/output/lhcb1_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b1.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b2.tfs
    opens browser with possibility to type commands,   .q  to quit..


    debug cling segmentation violation,   next ok, failed without Lorentz dummy;   .L ~/c/MDISim/myGraph3D.C
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .q

 --- read back the saved geometry and use event display --- also works with projections -- but looks funny close to IR and beginning of arc, exceeding buffer sizes TGLRnrCtx::EndSelection
 see  https://root.cern.ch/root/html/src/TGLRnrCtx.cxx.html  warning followed by TGLSelectBuffer grow

 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/MyGeoManager.C
 .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=2;TEveElement* el;MultiView* gMultiView=0;bool AllowForProjections=true;
 //  el=StartEveWithGeomDisplay("LHC_IR_5.root");  // local
     el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5_LR.root"),AllowForProjections=true,verbose=2);
 //  el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5.root")); // ok
 //  el=StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root");
 // PlotGuidesAtOrigin();
 //---- wait a bit before next step
 if(AllowForProjections) gMultiView=ShowProjections(el,verbose); // projections filled, -- zoom in/out, look at details, RhoZ view strange, meaning of sign of R ?
 const double zmin=0,zmax=330,Scale_xy=100;
 verbose=0;PlotTracksInFiles("~/mad/lhc/twiss_track_ref/0*_px*b1.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",zmin,zmax,kGreen,Scale_xy,verbose);
 verbose=0;PlotTracksInFiles("~/mad/lhc/twiss_track_ref/0*_px*b2.tfs","~/mad/lhc/output/lhcb2_survey_from5.tfs",zmin,zmax,kGreen,Scale_xy,verbose);
 if(AllowForProjections) el=gEve->GetCurrentEvent(); //gets track visible in projection
 if(AllowForProjections) { gMultiView->DestroyEventRPhi(); gMultiView->ImportEventRPhi(el); }
 if(AllowForProjections) { gMultiView->DestroyEventRhoZ(); gMultiView->ImportEventRhoZ(el); }
 gEve->Redraw3D(kFALSE, kTRUE);
 .q





 ----- tracking with rmatrix ----   CLIC,  generate the files  /tmp/hbu/clictrack_x_0_BinFile  ...,
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
  .I /Users/hbu/myroot
 .L ~/c/MDISim/myEve.C
 .L ~/c/MDISim/Twiss_util.C
 unsigned int verbose=1;
 const char* Fname_b1_o="~/mad/clic/v_10_10_11/newbds_twiss.tfs";
 const char* Fname_b1_s="~/mad/clic/v_10_10_11/newbds_survey.tfs";
 LinTrk lintrk_b1(Fname_b1_o,Fname_b1_s,verbose); // setup LinTrk machine with parameters from tfs header
 lintrk_b1.Print();
 const string trk_fname="clictrack";
 lintrk_b1.SetSteps(5,20, 5,20, 5,2.e-3); // 0 to 100 sigma in x, 0 to 100 sigma in y   and up to 1% dp
 lintrk_b1.trk_rmatrix(trk_fname);
 .q
 ls  clictrack_*_BinFile

 //---  CLIC plot with rmatrix generated tracks,  strange with projections
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;TEveElement* el;MultiView* gMultiView=0;bool AllowForProjections=false; // colors nicer when false
 double zmin=0,zmax=2800,Scale_xy=1000;
 vector<Ntuple> nt_trks;
 const char* trk_fnames="/tmp/hbu/clictrack_*_BinFile";
 vector<string> FileList=ListOfFilesMatching(trk_fnames);
 for(unsigned int i=0; i<FileList.size(); ++i) { cout << "use " << FileList[i] << '\n'; nt_trks.push_back(Ntuple(FileList[i].c_str()));if(verbose) nt_trks.back().PrintSummary(); }
 const char* Fname_b1_s="~/mad/clic/v_10_10_11/newbds_survey.tfs";
 Ntuple nt_s(Fname_b1_s);
 if(verbose) nt_s.PrintSummary();
 el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/CLIC_BDS.root"),AllowForProjections,verbose);
 if(AllowForProjections) gMultiView=ShowProjections(el,verbose); // projections filled, -- zoom in/out, look at details, RhoZ view strange, meaning of sign of R ?
 cout << "nt_trks.size()=" << nt_trks.size() << endl;
 // verbose=3; nt_s.SetVerbose(verbose); for(unsigned i=0; i<nt_trks.size(); ++i) nt_trks[i].SetVerbose(verbose);
 // i=0;  nt_trks[i].PrintSummary(); Ntuple nt_trk_eu=CourantSnyder_to_Euclidian(nt_s,nt_trks[i],zmin,zmax); // just as check
 for(unsigned i=0; i<nt_trks.size(); ++i) { Ntuple nt_trk_eu=CourantSnyder_to_Euclidian(nt_s,nt_trks[i],zmin,zmax);  cout << "i=" << i << endl; if(verbose) nt_trk_eu.PrintSummary(); EveSingleTrackDisplay(nt_trk_eu,zmin,zmax,kGreen,Scale_xy,verbose); }
 if(AllowForProjections) el=gEve->GetCurrentEvent(); //gets track visible in projection
 if(AllowForProjections) { gMultiView->DestroyEventRPhi(); gMultiView->ImportEventRPhi(el); }
 if(AllowForProjections) { gMultiView->DestroyEventRhoZ(); gMultiView->ImportEventRhoZ(el); }
 gEve->Redraw3D(kFALSE, kTRUE);



 //--- plot geometry with the design path, just using survey coordinates, no track generation needed
 // -- CLIC example
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;TEveElement* el;MultiView* gMultiView=0;bool AllowForProjections=false; // colors nicer when false
 const char* Fname_g="~/www/Geom/CLIC_BDS.root"; // geometry file name
 el=StartEveWithGeomDisplay(ResolveHome(Fname_g),AllowForProjections,verbose);
 double zmin=0,zmax=2800,Scale_xy=1000;
 const char* Fname_s="~/mad/clic/v_10_10_11/newbds_survey.tfs"; // survey file name
 EveSingleTrackDisplay(Ntuple(Fname_s),zmin,zmax,kGreen,Scale_xy,verbose);

 // -- LHC_IR_5_LR example    geometry with design path
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;
 const char* Fname_g="~/www/Geom/LHC_IR_5_LR.root"; // geometry file name
 StartEveWithGeomDisplay(ResolveHome(Fname_g));
 double zmin=-320,zmax=320,Scale_xy=100;
 const char* Fname_b1_s="~/mad/lhc/output/lhcb1_survey_from5.tfs";
 const char* Fname_b2_s="~/mad/lhc/output/lhcb2_survey_from5.tfs";
 Ntuple nt_b1(Fname_b1_s); if(verbose) nt_b1.PrintSummary();
 Ntuple nt_b2(Fname_b2_s);
 verbose=3;
 EveSingleTrackDisplay(nt_b1,zmin,zmax,kGreen,Scale_xy,verbose);
 EveSingleTrackDisplay(nt_b2,zmin,zmax,kGreen,Scale_xy,verbose);



 ----- LHC tracking with rmatrix ----  generate the files lhctrack_*_BinFile,    can look at with      ntuple2ascii lhctrack_x_1_BinFile ; less lhctrack_x_1_BinFile.out
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
  .I /Users/hbu/myroot
 .L ~/c/MDISim/myEve.C
 .L ~/c/MDISim/Twiss_util.C
 unsigned int verbose=1;
 const char* Fname_b1_o="~/mad/lhc/output/lhcb1_twiss_from5.tfs";
 const char* Fname_b1_s="~/mad/lhc/output/lhcb1_survey_from5.tfs";
 LinTrk lintrk_b1(Fname_b1_o,Fname_b1_s,verbose); // setup LinTrk machine with parameters from tfs header
 lintrk_b1.Print();
 const string trk_fname="lhctrack";
 lintrk_b1.SetSteps(5,10, 5,10, 5,5.e-3); // 0-50 sigma  and 0 to 2.5e-2        args ar nx,dx,  ny,dy,  dp,dp
 lintrk_b1.trk_rmatrix(trk_fname);  // tracks going all around ring
 .q
 ls lhctrack_*_BinFile

 --- LHC plot rmatrix tracking  ---
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;
 double zmin=0,zmax=400,Scale_xy=100;
 vector<Ntuple> nt_trks;
 const char* trk_fnames="/tmp/hbu/lhctrack_*_BinFile";
 vector<string> FileList=ListOfFilesMatching(trk_fnames);
 for(unsigned int i=0; i<FileList.size(); ++i) { cout << "use " << FileList[i] << '\n'; nt_trks.push_back(Ntuple(FileList[i].c_str()));if(verbose) nt_trks.back().PrintSummary(); }
 const char* Fname_b1_s="~/mad/lhc/output/lhcb1_survey_from5.tfs";
 Ntuple nt_s(Fname_b1_s);
 if(verbose) nt_s.PrintSummary();
 StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5.root"));
 cout << "nt_trks.size()=" << nt_trks.size() << endl;
 for(unsigned i=0; i<nt_trks.size(); ++i) { Ntuple nt_trk_eu=CourantSnyder_to_Euclidian(nt_s,nt_trks[i],zmin,zmax);  cout << "i=" << i << endl; if(verbose) nt_trk_eu.PrintSummary(); EveSingleTrackDisplay(nt_trk_eu,zmin,zmax,kGreen,Scale_xy,verbose); }
 .q



 ----- LEP tracking with rmatrix ----  generate the files lhctrack_*_BinFile,    can look at with      ntuple2ascii lhctrack_x_1_BinFile ; less lhctrack_x_1_BinFile.out
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
  .I /Users/hbu/myroot
 .L ~/c/MDISim/myEve.C
 .L ~/c/MDISim/Twiss_util.C
 unsigned int verbose=1;
 const char* Fname_b1_o="~/mad/LEP/f0520b98v1/lep_twiss.tfs";
 const char* Fname_b1_s="~/mad/LEP/f0520b98v1/lep_survey.tfs";
 LinTrk lintrk_b1(Fname_b1_o,Fname_b1_s,verbose); // setup LinTrk machine with parameters from tfs header
 lintrk_b1.RFHV_from_twiss(*lintrk_b1.the_nt); // gymnastics to get emittance
 double EmitRatio=0.002;
 lintrk_b1.EmitFromSynrad(EmitRatio);
 lintrk_b1.Print();
 const string trk_fname="leptrack";
 lintrk_b1.SetSteps(5,10, 5,10, 5,5.e-3); // 0-50 sigma  and 0 to 2.5e-2        args ar nx,dx,  ny,dy,  dp,dp
 lintrk_b1.trk_rmatrix(trk_fname);  // tracks going all around ring
 .q
 ls leptrack_*_BinFile

 --- LEP plot rmatrix tracking generated as shown above ---
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;TEveElement* el;MultiView* gMultiView=0;bool AllowForProjections=true; // colors nicer when false
 double zmin=0,zmax=400,Scale_xy=100;
 vector<Ntuple> nt_trks;
 const char* trk_fnames="/tmp/hbu/leptrack_*_BinFile";
 vector<string> FileList=ListOfFilesMatching(trk_fnames);
 for(unsigned int i=0; i<FileList.size(); ++i) { cout << "use " << FileList[i] << '\n'; nt_trks.push_back(Ntuple(FileList[i].c_str()));if(verbose) nt_trks.back().PrintSummary(); }
 const char* Fname_b1_s="~/mad/LEP/f0520b98v1/lep_survey.tfs";
 Ntuple nt_s(Fname_b1_s);
 if(verbose) nt_s.PrintSummary();
 el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/lep_f0520b98v1.root"),AllowForProjections,verbose);
 if(AllowForProjections) gMultiView=ShowProjections(el,verbose);
 cout << "nt_trks.size()=" << nt_trks.size() << endl;
 //-------  wait a bit, click on geom, rotate a bit
 for(unsigned i=0; i<nt_trks.size(); ++i) { Ntuple nt_trk_eu=CourantSnyder_to_Euclidian(nt_s,nt_trks[i],zmin,zmax);  cout << "i=" << i << endl; if(verbose) nt_trk_eu.PrintSummary(); EveSingleTrackDisplay(nt_trk_eu,zmin,zmax,kGreen,Scale_xy,verbose); }
 if(AllowForProjections) el=gEve->GetCurrentEvent(); //gets track visible in projection
 if(AllowForProjections) { gMultiView->DestroyEventRPhi(); gMultiView->ImportEventRPhi(el); }
 if(AllowForProjections) { gMultiView->DestroyEventRhoZ(); gMultiView->ImportEventRhoZ(el); }
 gEve->Redraw3D(kFALSE, kTRUE);




 // -- LEIR example    geometry with design path
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;
 const char* Fname_g="~/www/Geom/LEIR.root"; // geometry file name
 StartEveWithGeomDisplay(ResolveHome(Fname_g));
 double zmin=-10,zmax=22,Scale_xy=1;
 const char* Fname_s="~/mad/makethin/leir/output/leir_survey.tfs"; // survey file name
 EveSingleTrackDisplay(Ntuple(Fname_s),zmin,zmax,kGreen,Scale_xy,verbose);


 // --- Generate Helix files  ( after rm -rf "/tmp/$LOGNAME/"*  )
 proj=ansiitest ; targ=Test_myHelix ; mkdir /tmp/$LOGNAME/Release ; cmakeUnix $proj ; cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv /tmp/$LOGNAME/$proj/$targ /tmp/$LOGNAME/Release/$targ ;  cd /tmp/$LOGNAME/; rm -f "/tmp/$LOGNAME/"*; Release/Test_myHelix ; echo ls -l ; ls -l *.out

 // --- SuperKEKB, Helix  -- make sure the files are there   ls -l "/tmp/$LOGNAME/"*.out       | tee AddTracksToGeom.out
 cd /tmp/$LOGNAME ; time root | tee AddTracksToGeom.out
  .L ~/c/MDISim/MyGeoManager.C
 .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=0;TEveElement* el;MultiView* gMultiView=0;bool AllowForProjections=false;
 el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_HER_r.root",AllowForProjections=true,verbose=0);
 el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_HER_l.root",AllowForProjections=true,verbose=0);
 el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_LER_r.root",AllowForProjections=true,verbose=0);
 el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_LER_l.root",AllowForProjections=true,verbose=0);
 PlotGuidesAtOrigin();
 if(AllowForProjections) gMultiView=ShowProjections(el,verbose);
 const double zmin=-2,zmax=2,Scale_xy=1;
 verbose=2;PlotTracksInFiles("/tmp/hbu/Helix*.out","",zmin,zmax,kGreen,Scale_xy,verbose);
 if(AllowForProjections) el=gEve->GetCurrentEvent(); //gets track visible in projection
 if(AllowForProjections) { gMultiView->DestroyEventRPhi(); gMultiView->ImportEventRPhi(el); }
 if(AllowForProjections) { gMultiView->DestroyEventRhoZ(); gMultiView->ImportEventRhoZ(el); }
 gEve->Redraw3D(kFALSE, kTRUE);
 .q
 ls -l /tmp/$LOGNAME/AddTracksToGeom.out ; open /tmp/$LOGNAME/AddTracksToGeom.out   ; echo works with tee



 ---- special trial tests with ordering
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=1;
 const char* Fname_b1_s="~/mad/lhc/output/lhcb1_survey_from5.tfs";
 Ntuple nt(Fname_b1_s);
 nt.PrintSummary();

 valarray<double> zvar=nt.GetVar("Z");
 for(unsigned int i=0;i<10;++i) cout << " " << zvar[i]; cout << endl;

 nt.SetVerbose(3);
 nt.SortByVar("Z"); // works now,  but jumps to other side of ring, not really wanted, needs also x
 // nt.SortByVar("X");

 nt.SetVerbose(1);
 nt.PrintSummary();
 nt.WriteAsciiNtFile("","ordered.out");

 */

#include "MyCppStdHeaders.H"
#include <TRint.h>

#include "Lorentz.h"
#include "Twiss_util.h" // Beam
#include "myEve.h" // PlotGuidesAtOrigin
#include "myGraph3D.h"

#include "RunFlags.h"
#include <getopt.h>

void RunFlags::ShowUsage() const
{
  cout << "Help for " << argv0
  << " which is a unix command line utility to check if all unix logical links exist. Written in 2014 by H. Burkhardt" << '\n'
  << BOLD << "SYNOPSIS" << NORMAL << '\n'
  << "    " << argv0 << " [OPTION] zmin zmax color Scale_xy GeomFile InputFiles" << '\n'
  << "    for track input files use ?  rather than * to match several files, give both survey and track files" << '\n'
  << BOLD << "OPTIONS" << NORMAL << " " << cflags << '\n'
  << "    -h           just show this help" << '\n'
  << "    -n           show commands, without doing" << '\n'
  << "    -v           verbose (turn debug on)" << '\n'
  << "    -V           more verbose (turn verbose>1 on)" << '\n'
  << BOLD << "EXAMPLEs" << NORMAL << '\n'
  << argv0 << " -Vn 0 330 416 100 http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root ~/mad/lhc/output/lhcb1_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b1.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b2.tfs" << '\n' // kGreen is 416
  << argv0 << " -v  0 330 416 100 http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root ~/mad/lhc/output/lhcb1_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b1.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs ~/mad/lhc/twiss_track_ref/0?_px?b2.tfs" << '\n'; // kGreen is 416
}

int main(int argc,char * const argv[]) // normally this
{

  TRint* theApp = new TRint("",0,NULL);

  const char* cflags="hnvV";
  RunFlags Flag(argc,argv,cflags);
  if(Flag.verbose>1 ) cout << __FILE__<< " " << __FUNCTION__ << " after RunFlags line " << setw(4) << __LINE__ << '\n';

  double zmin=stod(Flag.ArgvVec[1]);   // http://www.cplusplus.com/reference/string/stod/
  double zmax=stod(Flag.ArgvVec[2]);
  double color=stod(Flag.ArgvVec[3]);
  double Scale_xy=stod(Flag.ArgvVec[4]);
  const char* GeomFile=Flag.ArgvVec[5].c_str();

  if(Flag.ArgvVec.size()<6) { cout << "*** Error, missing input parameters, ArgvVec.size()=" << Flag.ArgvVec.size() << " should be at least 5" << '\n'; exit(1) ; }

  if(Flag.verbose>1)
  {
    cout << "ArgvVec.size()=" << Flag.ArgvVec.size()
    << " zmin=" << zmin
    << " zmax=" << zmax
    << " color=" << color
    << " Scale_xy=" << Scale_xy
    << " Geometry file=" << GeomFile
    << '\n';
  }
  if(Flag.ArgvVec.size()<6) exit(1);

  vector<string> SurveyFiles;
  vector<string> TrackFiles;

  for(unsigned int i=6;i<Flag.ArgvVec.size();++i) // input files
  {
    while(Flag.ArgvVec[i].find("?")!=string::npos) Flag.ArgvVec[i].replace(Flag.ArgvVec[i].find("?"),1,"*"); // http://www.cplusplus.com/reference/string/string/  replace any ? by *
    if( Flag.ArgvVec[i].find("survey")!=string::npos ) SurveyFiles.push_back(Flag.ArgvVec[i]); // http://www.cplusplus.com/reference/string/string/find/
    else TrackFiles.push_back(Flag.ArgvVec[i]);
  }

  if(Flag.n) cout << "StartEveWithGeomDisplay(\"" << GeomFile << "\");" << '\n';
  else StartEveWithGeomDisplay(GeomFile);
  for(unsigned int i=0;i<SurveyFiles.size();++i)
  {
    if(i<TrackFiles.size())
    {
      if(Flag.n) cout << "PlotTracksInFiles(\"" << TrackFiles[i] << "\",\"" << SurveyFiles[i] << "\",zmin,zmax,color,Scale_xy,Flag.verbose);" << '\n';
      else PlotTracksInFiles(TrackFiles[i].c_str(),SurveyFiles[i].c_str(),zmin,zmax,color,Scale_xy,Flag.verbose);
    }
  }
  theApp->Run();
};
