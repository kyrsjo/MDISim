/* ~/c/MDISim/main_MyNtuple2Geom.C        raw geometry from tfs    and export to root file,   no plotting

http://root.cern.ch/drupal/content/root-users-guide-6001020
http://root.cern.ch/root/htmldoc/guides/users-guide/Geometry.html
http://root.cern.ch/root/html/TGeoManager.html

600 kBlue b1  CMYK 100  50   0      clockwise
632 kRed  b2  CMYK   0 100 100  anticlockwise

proj=MDISim ; targ=MyNtuple2Geom ; cmakeXcode ~/c/$proj ;   echo build MyNtuple2Geom      makes    ./MDISim/Release/MDISim ;  find /tmp/$LOGNAME -name $targ

rm -rf "/tmp/$LOGNAME/"*

Unix make
proj=MDISim ; targ=MyNtuple2Geom ; cd /tmp/$LOGNAME ; cmakeUnix $proj ; cd $proj ; make -j5 ; make install   ;              echo makes /tmp/$LOGNAME/$proj/Release/$targ
proj=MDISim ; targ=MyNtuple2Geom ; cd /tmp/$LOGNAME ; cmakeUnix $proj ; cd $proj ; make $targ -j5 ; mv $targ Release/.

-- then always
cd /tmp/$LOGNAME/ ; time $proj/Release/$targ -h
-- which is
cd /tmp/hbu ; time MDISim/Release/MyNtuple2Geom -h



rm -f "/tmp/$LOGNAME/"* ;      echo color, 600 kBlue,   632 is kRed,  0  is automatic coloring
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; proj=MDISim ; targ=MyNtuple2Geom ;cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME
cd /tmp/$LOGNAME ; MDISim/Release/MyNtuple2Geom -cv LHC_IR_5 IP 0 340 100 600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs 2>&1 | tee /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; MDISim/Release/MyNtuple2Geom -cv LHC_IR_5 IP 0 340 100   0 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs   0 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs 2>&1 | tee /tmp/hbu/MDISim.out


just b1
cd /tmp/$LOGNAME ; MDISim/Release/MyNtuple2Geom -cv LHC_IR_5 IP 0 320 100 600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 2>&1 | tee /tmp/hbu/MDISim.out


cd /tmp/$LOGNAME ; time root ; echo ok both my root and standard root6
.L ~/c/MDISim/myEve.C
StartEveWithGeomDisplay("LHC_IR_5.root"); // local
// StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5.root"));


compile and test, single beam LHC
cd /tmp/$LOGNAME ; rm -rf "/tmp/$LOGNAME/"* ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target MyNtuple2Geom -configuration Release ; Release/MyNtuple2Geom -cV LHC_IR_5 IP 0 320 100 600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 2>&1 | tee /tmp/hbu/MDISim.out


--- equivalent root in batch mode
cd /tmp/$LOGNAME ; root -b
// .I /Users/hbu/myroot
.L ~/c/MDISim/MyNtuple2Geom.C
// gSystem->Load("libGeom");
unsigned int verbose=1;
Ntuple nt_b1(ReadAndMerge("~/mad/lhc/output/lhcb1_twiss_from5.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",verbose)); if(verbose) nt_b1.PrintSummary();
Ntuple nt_b2(ReadAndMerge("~/mad/lhc/output/lhcb2_twiss_from5.tfs","~/mad/lhc/output/lhcb2_survey_from5.tfs",verbose)); if(verbose) nt_b2.PrintSummary();
// const double zmin=0,zmax=40,Scale_xy=100;
// const double zmin=0,zmax=320,Scale_xy=100; // to beginning of arcs
const double zmin=-320,zmax=320,Scale_xy=100; // left and right to beginning of arcs, ----------- not working at the moment, start only at IP going right
// const double zmin=0,zmax=3000,Scale_xy=100;
// const double zmin=-99999,zmax=99999,Scale_xy=1; // whole ring -- barely visible, use no scalefact, --- not working well at the moment, 1.2MB .root
verbose=1; GeomFromMyNtuple("LHC_IR_5","IP",nt_b1,zmin,zmax,kBlue,Scale_xy,true,false,verbose);
verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",nt_b2,zmin,zmax,kRed ,Scale_xy,true,false,verbose); // use same name  LHC_IR_5   to draw together
gGeoManager->Export("LHC_IR_5.gdml");  // export the whole geometry in gdml,  do first to avoid  <volumeref ref=""/>
gGeoManager->Export("LHC_IR_5.root");  // export the whole geometry
gGeoManager->Export("LHC_IR_5.C");     // export the whole geometry in C format
.q




--- as test read the geometry and plot with standard root          to understand geometry in .C  look at    last  // HMatrix    translation    tr[0]  = -43.211096;    tr[1] = 0.000000;    tr[2] = 315.629018;
cd /tmp/$LOGNAME ; time $ROOTSYS/bin/root

// TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5_LR.root"); gGeoManager->GetTopVolume()->Draw("ogl"); // nice as demo

// TGeoManager::Import("LHC_IR_1_ring.root"); // not crashing, but also nothing really visible
// TGeoManager::Import("/Users/hbu/www/geom/LHC_IR_5_400m.root");
// TGeoManager::Import("LHC_IR_5_400m.root");

// TGeoManager::Import("LHC_IR_5.root"); // local

// TGeoManager::Import("LHC_IR_5.root"); // ok
TGeoManager::Import("/Users/hbu/www/geom/LHC_IR_5.root"); // ok
// TGeoManager::Import("/Users/hbu/www/geom/LHC_IR_5.gdml"); // Volume is NULL        https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=17214     ok when done first
// TGeoManager::Import("/Users/hbu/www/geom/LHC_IR_5.xml");  // ok

// TGeoManager::Import("TLEP_geom.root");
// TGeoManager::Import("TLEP_geom.xml");  // ok
// TGeoManager::Import("TLEP_geom.gdml"); // Volume is NULL
// TGeoManager::Import("/Users/hbu/www/geom/TLEP_geom_root5.gdml"); // ok, just topvolume color lost

if(gGeoManager) gGeoManager->GetTopVolume()->Draw("ogl"); // Edge at x=-3550, Y=1585, Z=0, seems the minimum of the coordinates

//--   now change the view   http://root.cern.ch/root/html/TGLViewer.html    this view works also well without scalefactor      see  glViewerExercise.C
TGLViewer* v = (TGLViewer *)gPad->GetViewer3D();
TGLOrthoCamera& o = static_cast<TGLOrthoCamera &>(v->CurrentCamera()); o.SetEnableRotate(kTRUE); // allow rotate,
v->SetCurrentCamera(TGLViewer::kCameraOrthoXnOY); //--- look down in beam direction --  display x/y -- very useful in some cases
v->SetCurrentCamera(TGLViewer::kCameraPerspXOZ);  //--- default view  "x=z", "y=y"
//-- next two less useful,  all options see  TGLViewer.h
v->SetCurrentCamera(TGLViewer::kCameraOrthoXOZ);  //--- "x=x", "y=z"   beam upwadrs
v->SetCurrentCamera(TGLViewer::kCameraPerspYOZ);  //--- "z=-z", "y=x"



new TBrowser; // to see what is going on


// rest attempts - better check out display options
TGLViewer::ECameraType camera = TGLViewer::kCameraOrthoXOY; cout << "camera=" << camera << '\n'; // maximum camera type here is 6
v->SetCurrentCamera(camera);
double center[3] = {0,0,0};
double fov = 1, dollyStep=0,hRotateStep=0,vRotateStep=0; // fov is Zoom, dolly  is distance form center
v->SetOrthoCamera(camera, fov, dollyStep, center, hRotateStep, vRotateStep); // puts camera center at 0
v->CurrentCamera().SetExternalCenter(kTRUE);

// read geom and plot with  Eve
cd /tmp/$LOGNAME ; time root ; echo ok both my root and standard root6
.L ~/c/MDISim/myEve.C
// StartEveWithGeomDisplay("LHC_IR_5.root");  // local
StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5_LR.root"); // both sides, nice
// StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root");
PlotGuidesAtOrigin();


-- for tests, draw directly online           ~/lib/4.8/root_1a
cd /tmp/$LOGNAME ; time root
.L ~/c/MDISim/MyNtuple2Geom.C
unsigned int verbose=0;
Ntuple nt_b1(ReadAndMerge("~/mad/lhc/output/lhcb1_twiss_from5.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",verbose)); if(verbose) nt_b1.PrintSummary();
gSystem->Load("libGeom");
const double zmin=-320,zmax=320,Scale_xy=100; verbose=1; GeomFromMyNtuple("LHC_IR_5","IP",nt_b1,zmin,zmax,kBlue,Scale_xy,verbose);
gGeoManager->GetTopVolume()->Draw("ogl");



--- read back geometry written in .C format     works in root5,  and write in gdml
source ~/bin/my_Xcode_env.sh clang 1 root5
cd /tmp/$LOGNAME ; time $ROOTSYS/bin/root
gROOT->LoadMacro("/Users/hbu/www/Geom/TLEP_geom.C"); // TLEP_geom.C made with root6
gInterpreter->ProcessLine("TLEP_geom()");
gGeoManager->GetTopVolume()->Draw("ogl");
gGeoManager->Export("TLEP_geom_root5.xml");
gGeoManager->Export("TLEP_geom_root5.gdml");

source ~/bin/my_Xcode_env.sh clang 1 root5
cd /tmp/$LOGNAME ; time $ROOTSYS/bin/root
gROOT->LoadMacro("/Users/hbu/www/Geom/LHC_IR_5.C");
gInterpreter->ProcessLine("LHC_IR_5()");
gGeoManager->GetTopVolume()->Draw("ogl");
gGeoManager->Export("LHC_IR_5_root5.xml");
gGeoManager->Export("LHC_IR_5_root5.gdml");






fcc-ee IR geometry
------------------
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; proj=MDISim ; targ=MyNtuple2Geom ;cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME

cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -cv fcc-ee-geom IP 0 500 100 1020 ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_twiss.tfs ~/mad/fcc-ee/CERN-LM-21May14/FCC_ee_survey.tfs

cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
StartEveWithGeomDisplay("fcc-ee-geom.root");  // local
// StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/fcc-ee-geom.root")

fcc-ee Oide
-----------
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_t_26_cw_nosol    IP     0 1000 100 1020 ~/mad/fcc-ee/Oide/output/fcc_ee_t_26_cw_nosol_twiss.tfs       ~/mad/fcc-ee/Oide/output/fcc_ee_t_26_cw_nosol_survey.tfs    | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_t_42_6a_cw       IP     0 1000 100 1020 ~/mad/fcc-ee/Oide/output/fcc_ee_t_42_6a_cw_twiss.tfs          ~/mad/fcc-ee/Oide/output/fcc_ee_t_42_6a_cw_survey.tfs       | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_t_42_7c_cw       IP     0 1000 100 1020 ~/mad/fcc-ee/Oide/output/fcc_ee_t_26_cw_nosol_twiss.tfs       ~/mad/fcc-ee/Oide/output/fcc_ee_t_26_cw_nosol_survey.tfs    | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_t_45_16_cw_nosol IP -1500 1500 100 600 ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b1_twiss.tfs ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b1_survey.tfs 632 ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b2_twiss.tfs ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b2_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_t_74_11_nosol    IP -1500 1500 100 600 ~/mad/fcc-ee/Oide/output/fcc_ee_t_74_11_nosol_b1_twiss.tfs    ~/mad/fcc-ee/Oide/output/fcc_ee_t_74_11_nosol_b1_survey.tfs 632    ~/mad/fcc-ee/Oide/output/fcc_ee_t_74_11_nosol_b2_twiss.tfs ~/mad/fcc-ee/Oide/output/fcc_ee_t_74_11_nosol_b2_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out
// cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_t_45_16_cw_nosol IP  -300  300 100 600 ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b1_twiss.tfs ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b1_survey.tfs 632 ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b2_twiss.tfs ~/mad/fcc-ee/Oide/output/fcc_ee_t_45_16_cw_nosol_b2_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out

cp -pf "/tmp/$LOGNAME/"*.root ~/www/Geom/

--- look at this
cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
unsigned int verbose;TEveElement* el;bool AllowForProjections;
// el=StartEveWithGeomDisplay("fcc_ee_t_74_11_nosol.root",AllowForProjections=false,verbose=0); // both beams
el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/fcc_ee_t_74_11_nosol.root"),AllowForProjections=false,verbose=0);
// Plot_axis_arrow("x",  1,100,verbose=1); Plot_axis_arrow("y",  1,100,verbose=1); Plot_axis_arrow("z",100,100,verbose=1); // 1m, 1m, 100m
Plot_axis_arrow("x", -10,100,verbose=1); Plot_axis_arrow("y",  1,100,verbose=1); Plot_axis_arrow("z",500,100,verbose=1); // -10m x, 1m y,  500m z
PlotGuidesAtOrigin();

fcc-ee Anton
------------
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- fcc_ee_9_3 IP -1500 1500 100 600 ~/mad/fcc-ee/Anton/output/fcc_ee_9_3_b1_twiss.tfs ~/mad/fcc-ee/Anton/output/fcc_ee_9_3_b1_survey.tfs 632 ~/mad/fcc-ee/Anton/output/fcc_ee_9_3_b2_twiss.tfs ~/mad/fcc-ee/Anton/output/fcc_ee_9_3_b2_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out

cp -pf "/tmp/$LOGNAME/"*.root ~/www/Geom/

--- look at this
cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
unsigned int verbose;TEveElement* el;bool AllowForProjections;
el=StartEveWithGeomDisplay("fcc_ee_9_3.root",AllowForProjections=false,verbose=0);
Plot_axis_arrow("x", -10,100,verbose=1); Plot_axis_arrow("y",  1,100,verbose=1); Plot_axis_arrow("z",500,100,verbose=1);
PlotGuidesAtOrigin();


FCC-hh IR geometry     arc starts about 500m from IP, plot until 800m, scalefactor 100,  do not go negative, ring not closed,  without cycle starts at first IP, then long straight, plot to over 7000 m
------------------
useful colors :   1020   (grey),   0  for automatic coloring        bend blue, quad red, sext green, coll grey, marker orange
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; proj=MDISim ; targ=MyNtuple2Geom ;cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME

--- racetrack, make fcc_hh_race.root
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"*
time MDISim/Release/MyNtuple2Geom -caV -- fcc_hh_race IP    0  800 100 1020 ~/mad/fcc_hh/output/fcc_hh_race_v3_twiss.tfs              ~/mad/fcc_hh/output/fcc_hh_race_v3_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out
time MDISim/Release/MyNtuple2Geom -cv  -- fcc_hh_race IP    0 7500 100 1020 ~/mad/fcc_hh/output/fcc_hh_race_v3_before_cycle_twiss.tfs ~/mad/fcc_hh/output/fcc_hh_race_v3_before_cycle_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out

cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
// StartEveWithGeomDisplay("fcc_hh_race.root");
unsigned int verbose=2;TEveElement* el;bool AllowForProjections;
el=StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/fcc_hh_race.root",AllowForProjections=false,verbose=2);


--- ring, make fcc_hh_ring.root,   +/- around IP
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"*
time MDISim/Release/MyNtuple2Geom -cv -- fcc_hh_ring IP -800 800 100    0 ~/mad/fcc_hh/output/fcc_hh_ring_v3_twiss.tfs ~/mad/fcc_hh/output/fcc_hh_ring_v3_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out

cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
unsigned int verbose=2;TEveElement* el;bool AllowForProjections;
// StartEveWithGeomDisplay("fcc_hh_ring.root");
el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/fcc_hh_ring.root",AllowForProjections=false,verbose=2);

IP8
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; proj=MDISim ; targ=MyNtuple2Geom ;cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -cV -- LHC_IR_8_LR IP -320 320 100   0 ~/mad/lhc/output/lhcb1_twiss_from8.tfs ~/mad/lhc/output/lhcb1_survey_from8.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from8.tfs ~/mad/lhc/output/lhcb2_survey_from8.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out

cp -pf /tmp/hbu/LHC_IR_8_LR.* ~/www/Geom

+/- around IP :    0 nicer colored, 600grey
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; proj=MDISim ; targ=MyNtuple2Geom ;cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"*
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- LHC_IR_5_LR IP -320 320 100 600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- LHC_IR_5_LR IP -320 320 100   0 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out

/tmp/hbu/Release/MyNtuple2Geom -v LHC_IR_1 IP 0 320 100 600 ~/mad/lhc/output/lhcb1_twiss_from1.tfs ~/mad/lhc/output/lhcb1_survey_from1.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from1.tfs ~/mad/lhc/output/lhcb2_survey_from1.tfs

runII/2015, IP1 with sep
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- LHC_IR_1_LR IP -320 320 100   0 ~/mad/lhc/output_run2/lhcb1_twiss_from1_sep1.tfs ~/mad/lhc/output_run2/lhcb1_survey_from1.tfs 632 ~/mad/lhc/output_run2/lhcb2_twiss_from1.tfs ~/mad/lhc/output_run2/lhcb2_survey_from1.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out



grep "special" /tmp/hbu/MDISim.out

cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
unsigned int verbose;TEveElement* el;bool AllowForProjections;
/// el=StartEveWithGeomDisplay("LHC_IR_5_LR.root",AllowForProjections=false,verbose=0);  // IR5 local
// el=StartEveWithGeomDisplay("LHC_IR_8_LR.root",AllowForProjections=false,verbose=0);  // IR8 local
el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5_LR.root")); // saved version



geant4, see also ~/tex/plotting/Geometry.tex
cd /tmp/hbu ; cp -pf $G4INSTALL/examples/extended/persistency/"gdml/G01/"*.mac /tmp/hbu/ ; ~/mygeant4/bin/load_gdml ~/www/Geom/LHC_IR_5_LR.gdml


LEIR       see   ~/mad/makethin/leir/LEIR_job.madx         open ~/mad/makethin/leir/output/leir.pdf  ~/mad/makethin/leir/output/leir_optics_o_s.tfs
----
proj=MDISim ; targ=MyNtuple2Geom ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME/
time /tmp/$LOGNAME/Release/MyNtuple2Geom -acsV -- LEIR IP -10 22 1 630 ~/mad/makethin/leir/output/leir_optics.tfs ~/mad/makethin/leir/output/leir_survey.tfs 2>&1 | tee /tmp/hbu/MyNtuple2Geom_leir.out ; open /tmp/hbu/MyNtuple2Geom_leir.out
time /tmp/$LOGNAME/Release/MyNtuple2Geom -acsV -- LEIR IP -10 22 1   0 ~/mad/makethin/leir/output/leir_optics.tfs ~/mad/makethin/leir/output/leir_survey.tfs 2>&1 | tee /tmp/hbu/MyNtuple2Geom_leir.out ; open /tmp/hbu/MyNtuple2Geom_leir.out
to find BI1PO  overlap  z 13m to 15m
time /tmp/$LOGNAME/Release/MyNtuple2Geom -acsV -- LEIR IP   0 16 1 0 ~/mad/makethin/leir/output/leir_optics.tfs ~/mad/makethin/leir/output/leir_survey.tfs 2>&1 | tee /tmp/hbu/MyNtuple2Geom_leir.out ; open /tmp/hbu/MyNtuple2Geom_leir.out
time /tmp/$LOGNAME/Release/MyNtuple2Geom -acsV -- LEIR IP  12 16 1 0 ~/mad/makethin/leir/output/leir_optics.tfs ~/mad/makethin/leir/output/leir_survey.tfs 2>&1 | tee /tmp/hbu/MyNtuple2Geom_leir.out ; open /tmp/hbu/MyNtuple2Geom_leir.out


cp -pf /tmp/hbu/LEIR* ~/www/Geom/

echo grep "from" /tmp/hbu/MyNtuple2Geom.out ; echo grep "tr\[2\]" LEIR.C


cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
unsigned int verbose;TEveElement* el;bool AllowForProjections;
// el=StartEveWithGeomDisplay("LEIR.root",AllowForProjections=false,verbose=0);  // local
// el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LEIR.root"));  PlotGuidesAtOrigin();
// el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LEIR1.root")); PlotGuidesAtOrigin(); // rotated, phi, nonzero y
el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LEIR2.root")); PlotGuidesAtOrigin(); // rotated, psi, nonzero y
PlotGuidesAtOrigin();
.q


//--- LEIR using GeomFromMyNtuple  in root    | tee /tmp/hbu/MDISim.out
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
.L ~/c/MyRoot/init.C
// MDISim_init();
MDISim_init2();
// .L ~/c/MDISim/MyNtuple2Geom.C  should be in library, still needed
.L ~/c/MDISim/MyNtuple2Geom.C
unsigned int verbose=2;
string MachName="~/mad/makethin/leir/output/leir";
string Fname_b1_o=MachName+"_optics.tfs";
string Fname_b1_s=MachName+"_survey.tfs";
// string Fname_b1_s=MachName+"_survey1.tfs"; // test with rotation phi0=.1
// string Fname_b1_s=MachName+"_survey2.tfs"; // test with rotation psi0=.2
Ntuple nt_b1(ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose)); if(verbose) nt_b1.PrintSummary();
double zmin=-10,zmax=22,Scale_xy=1; //-10 22 m which is all
bool ConnectApertures, AddIndexToName, SimplifyAper;
GeomFromMyNtuple("LEIR","IP",nt_b1,zmin=-10,zmax=22,630,Scale_xy=1,verbose=2, ConnectApertures=true, AddIndexToName=false, SimplifyAper=true); // all
// GeomFromMyNtuple("LEIR","IP",nt_b1,zmin=  0,zmax= 3,630,Scale_xy=1,verbose=2); // 0 - 3m
gGeoManager->CheckGeometry();
gGeoManager->GetTopVolume()->Draw("ogl");
.q
open /tmp/hbu/MDISim.out


//--- check if LEIR is in a plane,  using   IsFlat  and showing vector normal to plane in ring centre
cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
unsigned int verbose;
string MachName="~/mad/makethin/leir/output/leir";
string Fname_b1_o=MachName+"_optics.tfs";
// string Fname_b1_s=MachName+"_survey.tfs";
string Fname_b1_s=MachName+"_survey1.tfs"; // test with rotation phi0=.1
// string Fname_b1_s=MachName+"_survey2.tfs"; // test with rotation psi0=.2
Ntuple nt_b1(ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose=2)); if(verbose) nt_b1.PrintSummary();
valarray<double> x,y,z;
x=nt_b1.GetVar("X_2");
if(nt_b1.VarExists("Y_2")) y=nt_b1.GetVar("Y_2"); else y.resize( x.size() );
z=nt_b1.GetVar("Z");
double centre[3], NormalVec[3]; IsFlat(x,y,z,centre,NormalVec,verbose=2); //-----------
To_CM_units(centre); To_CM_units(NormalVec);
// StartEveWithGeomDisplay( ResolveHome("~/www/Geom/LEIR.root") );  // geometry file for survey1
StartEveWithGeomDisplay( ResolveHome("~/www/Geom/LEIR1.root") ); // geometry file for survey1   normal pointing up
// StartEveWithGeomDisplay( ResolveHome("~/www/Geom/LEIR2.root") ); // geometry file for survey1   normal pointing down
PlotGuidesAtOrigin();
TEveArrow myArrow(NormalVec[0],NormalVec[1],NormalVec[2], centre[0],centre[1],centre[2]); // tutorial eve/arrow.C  https://root.cern.ch/root/html/TEveArrow.html
myArrow.SetMainColor(kBlue);
myArrow.SetTubeR(0.02);
myArrow.SetPickable(kTRUE);
gEve->AddElement(&myArrow);
gEve->Redraw3D(kTRUE);






overlap check with geant4
--------------------------
open ~/mygeant4/TestEm16/src/DetectorConstruction.cc       modify input file if needed
cd /tmp/hbu ; cmake -DGeant4_DIR=$G4BUILD  ~/mygeant4/TestEm16 ; time make -j5 ; cp -pf /tmp/hbu/TestEm16 ~/mygeant4/TestEm16/.
cd /tmp/hbu ; ~/mygeant4/TestEm16/TestEm16 ~/mygeant4/TestEm16/run01_gdml.mac       ; echo geant4 overlap test

with geant4 display:
cd /tmp/hbu ; ~/mygeant4/TestEm16/TestEm16
/control/execute /Users/hbu/mygeant4/TestEm16/vis_gdml.mac

LEIR
/gun/position 0 0 -1600


dafne_2005     survey not well closed  x = -0.00217396633   z = 0.09182190788
----------
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- dafne IP -30 30 1 0 ~/mad/dafne_2014/trunk/outputs/dafne_thick_twiss.tfs ~/mad/dafne_2014/trunk/outputs/dafne_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out


dafne_2014   survey not closed   X = -0.2081147188  z = -0.006661269436    see  ~/mad/dafne_2014/MRe_twiss.madx
----------
--trial old ptc version
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- dafne IP -10 10 1 0 ~/mad/daphne/p_kloe_10_10_05_lowBy_thick_initial_twiss.tfs ~/mad/daphne/p_kloe_10_10_05_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out
-- close to IP, 10x scaling       step seen at IP,   see survey, starts at x=0,  emds at x=-0.2081147188   ring not closed
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- dafne IP -5 5 10 0 ~/mad/dafne_2014/trunk/outputs/dafne_thick_twiss.tfs ~/mad/dafne_2014/trunk/outputs/dafne_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out
-- complete machine, no scaling
cd /tmp/$LOGNAME ; time MDISim/Release/MyNtuple2Geom -acsV -- dafne IP -30 30 1 0 ~/mad/dafne_2014/trunk/outputs/dafne_thick_twiss.tfs ~/mad/dafne_2014/trunk/outputs/dafne_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out

cd /tmp/$LOGNAME ; time root
.L ~/c/MyRoot/init.C
MDISim_init();
StartEveWithGeomDisplay("dafne.root"); // local
StartEveWithGeomDisplay(ResolveHome("~/www/Geom/dafne.root"));


-- 2d plot survey files   on my ntuple level
cd /tmp/hbu ; rm -f *.eps *.pdf; time root
string Fname_b1_s="~/mad/dafne_2014/trunk/outputs/dafne_survey.tfs";
Ntuple nt_b1(Fname_b1_s.c_str());
nt_b1.PrintSummary();
unsigned int verbose=0;bool square=true;
Plot_x_y(nt_b1,"Z","X",verbose,square);

// check planar
valarray<double> x,y,z;
x=nt_b1.GetVar("X");
y=nt_b1.GetVar("Y");
z=nt_b1.GetVar("Z");
TEveVectorT<double> centre, NormalVec; IsFlat(x,y,z,centre,NormalVec,verbose=1);
V3::IsClosed(x,y,z,verbose=2); // check closed

.q
find . -name "*.eps" -exec pstopdf {} \;
open Plot_Z_X.pdf



*/

#include "MyCppStdHeaders.H"
#include "TGeoManager.h"
//#include "Ntuple.h"
#include "MyNtuple2Geom.h"

#include "RunFlags.h"
#include <getopt.h>

void RunFlags::ShowUsage() const
{
cout << "Help for " << argv0
<< " which is a unix command line utility to check if all unix logical links exist. Written in 2014 by H. Burkhardt" << '\n'
<< BOLD << "SYNOPSIS" << NORMAL << '\n'
<< "    " << argv0 << " [OPTION] MyGeoName StartElementName zmin zmax Scale_xy color1 Opt1 Surv1  [ color2 Opt2 Surv2 ]" << '\n'
<< BOLD << "OPTIONS" << NORMAL << " " << cflags << '\n'
<< "    -a           add index to element name" << '\n'
<< "    -c           connect apertures" << '\n'
<< "    -h           just show this help" << '\n'
<< "    -s           simplify apertures" << '\n'
<< "    -v           verbose (turn debug on)" << '\n'
<< "    -V           more verbose (turn verbose>1 on)" << '\n'
<< BOLD << "DESCRIPTION" << NORMAL << '\n'
<< " color1, color2 if a second beam is present specify the color of the beam pipe " << '\n'
<< " 0-50 are basic colors 2=red, 3=green, 300-1000 from the color wheel, 600 means blue, 3d add +100 for darker, +150 brighter" << '\n'
<< " see http://root.cern.ch/root/html/TColor.html" << '\n'
<< " use 0 to get element class dependend coloring blue, red, green, orange, black for bend, quad, sext, marker, collimator" << '\n'
<< BOLD << "EXAMPLEs" << NORMAL << '\n'
<< argv0 << " -csv -- TLEP_geom     IP 0 180 100 1020 ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_twiss.tfs ~/mad/fcc-ee/BernhardHolzer_2013_10_11/tlep_175_survey.tfs" << '\n'
<< argv0 << " -csv -- LHC_IR_5      IP 0 320 100  600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs ; echo LHC beam1" << '\n'
<< argv0 << " -csv -- LHC_IR_5_400m IP 0 400 1    600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs; echo LHC beam1, 2 longer part, scalefactor=1" << '\n'
<< argv0 << " -csv -- LHC_IR_5      IP 0 320 100  600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs; echo LHC beam1 and 2" << '\n'
<< argv0 << " -csv -- LHC_IR_1_ring IP 0 1.e6  1  600 ~/mad/lhc/output/lhcb1_twiss_from1.tfs ~/mad/lhc/output/lhcb1_survey_from1.tfs ; echo full LHC beam1" << '\n'
<< argv0 << " -csv -- LEIR IP -10 30 1 0 ~/mad/makethin/leir/output/leir_optics.tfs ~/mad/makethin/leir/output/leir_survey.tfs 2>&1 | tee /tmp/hbu/MDISim.out ; open /tmp/hbu/MDISim.out" << '\n'
;
}

int main(int argc,char * const argv[])
{
const char* cflags="achsvV";
RunFlags Flag(argc,argv,cflags);
if(argc<2) { Flag.ShowUsage(); cout << '\n' << "*** Error ***  more arguments or option -h needed" << '\n';  exit(1); }
if(Flag.verbose>1 ) cout << __FILE__<< " " << __FUNCTION__ << " after RunFlags line " << setw(4) << __LINE__ << '\n';

if( Flag.ArgvVec.size()!=9 && Flag.ArgvVec.size()!=12 ) Flag.verbose=2; // wrong number of arguments

const char* MyGeoName =Flag.ArgvVec[1].c_str();
const char* StartElementName=Flag.ArgvVec[2].c_str();
const double zmin=stod(Flag.ArgvVec[3]);   // http://www.cplusplus.com/reference/string/stod/
const double zmax=stod(Flag.ArgvVec[4]);
const double Scale_xy=stod(Flag.ArgvVec[5]);
double color1=stod(Flag.ArgvVec[6]);
const char* Opt1 =Flag.ArgvVec[7].c_str();
const char* Surv1=Flag.ArgvVec[8].c_str();

if(Flag.verbose>1)
{
cout << "ArgvVec.size()=" << Flag.ArgvVec.size()
<< " MyGeoName=" << MyGeoName
<< " StartElementName=" << StartElementName
<< " zmin=" << zmin
<< " zmax=" << zmax
<< " Scale_xy=" << Scale_xy
<< " color1=" << color1
<< " Opt1=" << Opt1
<< " Surv1=" << Surv1
<< '\n';
}

if( Flag.ArgvVec.size()!=9 && Flag.ArgvVec.size()!=12 && Flag.ArgvVec.size()!=15 ) // 1 or 2 or 3 beams
{
cout << "*** Error wrong number of arguments" << '\n';
}

if(Flag.ArgvVec.size()<7)
{
cout << "*** Error, no enough input parameters ArgvVec.size()=" << Flag.ArgvVec.size() << '\n';
exit(1);
}

Ntuple nt_b1(ReadAndMerge(Opt1,Surv1,0)); // 0  for no debug, use  Flag.verbose  if debug of Merge needed
if(Flag.verbose) nt_b1.PrintSummary();
GeomFromMyNtuple(MyGeoName,StartElementName,nt_b1,zmin,zmax,color1,Scale_xy,Flag.verbose,Flag.c,Flag.a,Flag.s);

if(Flag.ArgvVec.size()==12 || Flag.ArgvVec.size()==15) // also beam2 there
{
unsigned int i0=12-3;
double color=stod(Flag.ArgvVec[i0]);
const char* Opt =Flag.ArgvVec[i0+1].c_str();
const char* Surv=Flag.ArgvVec[i0+2].c_str();
Ntuple nt_b(ReadAndMerge(Opt,Surv,Flag.verbose)); if(Flag.verbose) nt_b.PrintSummary();
if(Flag.verbose>1) cout << "next beam color=" << color << " Opt=" << Opt << " Surv=" << Surv << '\n';
GeomFromMyNtuple(MyGeoName,StartElementName,nt_b,zmin,zmax,color,Scale_xy,Flag.verbose,Flag.c,Flag.a,Flag.s);
}

if(Flag.ArgvVec.size()==15) // also beam3 there ---   could actually become a loop over further beams
{
unsigned int i0=15-3;
double color=stod(Flag.ArgvVec[i0]);
const char* Opt =Flag.ArgvVec[i0+1].c_str();
const char* Surv=Flag.ArgvVec[i0+2].c_str();
Ntuple nt_b(ReadAndMerge(Opt,Surv,Flag.verbose)); if(Flag.verbose) nt_b.PrintSummary();
if(Flag.verbose>1)
{
cout << "next beam color=" << color << " Opt=" << Opt << " Surv=" << Surv
<< '\n';
}
GeomFromMyNtuple(MyGeoName,StartElementName,nt_b,zmin,zmax,color,Scale_xy,Flag.verbose,Flag.a,Flag.s);
}

if(Flag.verbose) cout << "after GeomFromMyNtuple. Now gGeoManager->CloseGeometry()" << '\n';
gGeoManager->CloseGeometry();
// CSPE return 0;
//string OutFname=string(MyGeoName)+".root";
//gGeoManager->Export(OutFname.c_str());  // export the geometry as root file
const char* OutFname;
OutFname=string(string(MyGeoName)+".gdml").c_str(); gGeoManager->Export(OutFname); cout << OutFname << " written" << '\n'; // export in gdml format     http://root.cern.ch/root/html532/TGDMLWrite.html   GDMLEngine, ok when root made with  -Dgdml=ON,  do this first to avoid <volumeref ref=""/>   problems
OutFname=string(string(MyGeoName)+".root").c_str(); gGeoManager->Export(OutFname); cout << OutFname << " written" << '\n'; // export the geometry    http://root.cern.ch/root/html/TGeoManager.html
OutFname=string(string(MyGeoName)+".C").c_str();    gGeoManager->Export(OutFname); cout << OutFname << " written" << '\n';
OutFname=string(string(MyGeoName)+".xml").c_str();  gGeoManager->Export(OutFname); cout << OutFname << " written" << '\n';
};
