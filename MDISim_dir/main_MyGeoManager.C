/*   ~/c/MDISim/main_MyGeoManager.C  12/2013       this now mainly for tests and trials of new things and comments


 rm -rf "/tmp/$LOGNAME/"*

 Unix make
 proj=MDISim ;                     cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=MyGeoManager ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ

 Xcode
 proj=MDISim ; cmakeXcode $proj

 cp -pf $G4INSTALL/examples/extended/persistency/"gdml/G01/"*.mac /tmp/hbu/ ;  echo really needed, otherwise 'Top' is not unique errors
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml  ~/www/Geom/Shapes2.gdml    ; echo test to read gdml for geant4
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml  ~/www/Geom/SynRadGeo.gdml  ; echo test to read gdml for geant4

 all 3 also tested ok  root5 or root6  (root_git_build)
 source ~/bin/my_Xcode_env.sh clang 1 root_git
 source ~/bin/my_Xcode_env.sh clang 1 root6   ; ln -sf ~/lib/clang/libMyroot_git.so ~/lib/clang/libMyroot6.so
 source ~/bin/my_Xcode_env.sh clang 1 root5

 or reasonable standard default by :
 unset DYLD_LIBRARY_PATH PYTHONPATH LIBPATH LD_LIBRARY_PATH SHLIB_PATH ; source /opt/local/bin/thisroot.sh ; export PATH=/Users/hbu/bin:/opt/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/local/sbin:/usr/local/cernlib/2006/bin:./

 --- run in line mode by
 cd /tmp/$LOGNAME  ; rm -f * ; time Release/MyGeoManager ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf

 needs directory with track files in tfs
 /tmp/$LOGNAME/twiss_track     make by running twiss_track
 or
 ~/mad/lhc/twiss_track_ref

 see  http://root.cern.ch/download/doc/7CINT.pdf  Setting the Include Path

ftp://root.cern.ch/root/doc/18Geometry.pdf      ROOT manual , geometry


 source ~/bin/my_Xcode_env.sh clang 1 root_git

 ------- run in standard root ---------
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShape();

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShapes2();

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowTorus();
.q

 --- Belle HER, LER  L/R
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 bool HER,Right; unsigned int verbose;
 ShowBelle(HER=true, Right=true, verbose=2);
 gGeoManager->CloseGeometry(); // save volumes, idea is to use this with  MyEveFieldTest(); // nice helix around z-axis
 gGeoManager->Export("Belle_HER_r.gdml"); gGeoManager->Export("Belle_HER_r.C"); gGeoManager->Export("Belle_HER_r.root"); gGeoManager->Export("Belle_HER_r.xml");
 .q

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 bool HER,Right; unsigned int verbose;
 ShowBelle(HER=true, Right=false, verbose=2);
 gGeoManager->CloseGeometry(); // save volumes, idea is to use this with  MyEveFieldTest(); // nice helix around z-axis
 gGeoManager->Export("Belle_HER_l.gdml"); gGeoManager->Export("Belle_HER_l.C"); gGeoManager->Export("Belle_HER_l.root"); gGeoManager->Export("Belle_HER_l.xml");
 .q
 cp -pf /tmp/hbu/Belle_*.* ~/www/Geom/ ; ls -l ~/www/Geom/Belle_*
 --- Belle LER
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 bool HER,Right; unsigned int verbose;
 ShowBelle(HER=false, Right=false, verbose=2);
 gGeoManager->CloseGeometry(); // save volumes, idea is to use this with  MyEveFieldTest(); // nice helix around z-axis
 gGeoManager->Export("Belle_LER_l.gdml"); gGeoManager->Export("Belle_LER_l.C"); gGeoManager->Export("Belle_LER_l.root"); gGeoManager->Export("Belle_LER_l.xml");
 .q
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 bool HER,Right; unsigned int verbose;
 ShowBelle(HER=false, Right=true, verbose=2);
 gGeoManager->CloseGeometry(); // save volumes, idea is to use this with  MyEveFieldTest(); // nice helix around z-axis
 gGeoManager->Export("Belle_LER_r.gdml"); gGeoManager->Export("Belle_LER_r.C"); gGeoManager->Export("Belle_LER_r.root"); gGeoManager->Export("Belle_LER_r.xml");
 .q
 cp -pf /tmp/hbu/Belle_*.* ~/www/Geom/ ; ls -l ~/www/Geom/Belle_*
---  done with all 4 pipes




 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowPolycone();

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowPolygone();

 Shows GLViewer::TGLSAViewer       has Update Scene,  Camera Home,  clicking on them reset zoom/rotation to initial
 click on top volume, try
 RandomPoints, opens extra window, OK,  now shows just the top volume with extra points  -- now only showing this volume alone
   well visible in kWireFrame view, in stead Rays not seen

   after ShowShape();
 TGLViewer * v = (TGLViewer *)gPad->GetViewer3D();
 v->SetStyle(TGLRnrCtx::kOutline);  // shows extra lines on volume surface
 v->SetStyle(TGLRnrCtx::kFill);     // clicking on window restores view with filling
 v->SetStyle(TGLRnrCtx::kWireFrame); // clicking on window turns filling off, shows volumes just by wires

   switch light/shadow :
 after ShowShape();
 TGLViewer * v = (TGLViewer *)gPad->GetViewer3D();
 TGLLightSet* ls = v->GetLightSet();
 ls->SetLight(TGLLightSet::kLightLeft, kFALSE);  // click in window, slightly different light/shadow
 ls->SetLight(TGLLightSet::kLightRight, kFALSE);
 ls->SetLight(TGLLightSet::kLightTop, kFALSE);
 ls->SetLight(TGLLightSet::kLightBottom, kFALSE);

   trial to animate the camera  -- failed here   get more from glViewerExercise.C
 TGLViewer * v = (TGLViewer *)gPad->GetViewer3D();
 TTimer timer(25);
 TGLSAViewer* sav = dynamic_cast<TGLSAViewer*>(v);
 sav->GetFrame()->Connect("CloseWindow()", "TTimer", &timer, "TurnOff()");
 timer.SetCommand("AnimateCamera()");

 Viewer help:
 w      wireframe mode
 e      switch dark/light
 r      filled polygons
 t      outline mode
 double click will show the GUI editor if assigned    -- nothing happened


          now call next several times -  will change the ogl viewer   direction, filled or not ..
 .x /Users/hbu/Downloads/test.C

 -- next works and shows rotating moving balls
 cd ~/root_git_build/tutorials/gl/ ; $ROOTSYS/bin/root
 .x glViewerExercise.C


 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; $ROOTSYS/bin/root
 .x ~/root_git_build/tutorials/gl/nucleus.C


 next only works with  FromAfs,   others fail with  TGLHistPainter does not support gl
  cd ~/root_git_build/tutorials/gl/ ; $ROOTSYS/bin/root
  .x glbox.C


 ----
 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; $ROOTSYS/bin/root
 gStyle->SetCanvasPreferGL(kTRUE); // prefer GL
 Int_t wtopx=800,wtopy=400;
 kSizePt=700; // size in points   here used for both directions, to get square
 TCanvas *c1 = new TCanvas("mycanvas","PlotFrame",wtopx,wtopy,kSizePt,kSizePt); //   canvas visible  with title PlotFrame  -- next step is to get a 3D viewer
 .x /Users/hbu/Downloads/test.C



 // rather minimal run,  shows, canvas, ROOT GL and Eve windows
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 TVirtualGeoTrack *myTrack=MinimalGeom(true); // false without, true with eve window,  with eve window c1 geometry may dissapear but comes back after clicking in window

 gGeoManager->GetTopVolume()->Draw();
 myTrack-Draw(); // does not work
 gGeoManager->GetTopVolume()->Draw("ogl");



 ----  interesting example    Eve GLViewer window with geometry  -------   reeds a geometry,       http://www-ekp.physik.uni-karlsruhe.de/~cpulvermacher/shape_extracts/
 cd ~/Downloads/ ; $ROOTSYS/bin/root
 .x createExtract.C

 std::cout << "a...\n";
 TEveManager::Create();
 std::cout << "b...\n";

 TEveGeoTopNode* eve_top_node = new TEveGeoTopNode(gGeoManager, gGeoManager->GetTopNode());
 std::cout << "c...\n";
 gEve->AddGlobalElement(eve_top_node);
 std::cout << "now saving extract...\n";
 eve_top_node->ExpandIntoListTreesRecursively();
 eve_top_node->SaveExtract("mirror_extract.root", "Extract", false);
 std::cout << "done\n";

 gEve->Redraw3D(kTRUE);



  try to add tracks in Eve window         open $ROOTSYS/tutorials/eve/csgdemo.C
 ----------------------------------
     was useful to see how to set up eve viewer  and to be able to continue on gGeoManager  and draw track inside existing volumes
 cd /tmp/$LOGNAME ;  root
 .x $ROOTSYS/tutorials/eve/csgdemo.C
 // .x $ROOTSYS/tutorials/eve/csgdemo.C
 cout << "gEve=" << gEve << '\n';
 gEve->Redraw3D(kTRUE);
 TGeoVolume* top=gGeoManager->GetTopVolume();
 Int_t PdgCode=2212;
 Int_t itrk=0;
 TObject *particle=NULL;
 Int_t iPrimary = gGeoManager->AddTrack(itrk,PdgCode,particle);
 TVirtualGeoTrack *myTrack = gGeoManager->GetTrack(iPrimary);
 myTrack->AddPoint(0,12.5,0,0);
 myTrack->AddPoint(0, 20,0,0);
 myTrack->Print();
 top->SetVisRaytrace(true); // solid view
 // gGeoManager->SetCurrentTrack(myTrack);
 myTrack->Draw();  //----   track visible in c1 window,  not in Eve

 gEve->Redraw3D(kTRUE);   // click in canvas,  says it is raytracing,   switch c1 to editing   by view editor
 // gEve->AddGlobalElement(en);


 ------- run with my root ---------        source ~/bin/my_Xcode_env.sh clang 1 root_git          also ok with standard root    source ~/bin/my_Xcode_env.sh clang 1 root6
 cd /tmp/$LOGNAME ; time root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShape();



 try with older root  ---  to go without  MyClib  -- just give all coordinates by hand
 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; ~/ROOT/5.34.07/x86_64-mac106-gcc42-opt/root/bin/root
 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; unset DYLD_LIBRARY_PATH PYTHONPATH LIBPATH LD_LIBRARY_PATH SHLIB_PATH ; source ~/ROOT/5.34.14/x86_64-mac108-clang42-opt/bin/thisroot.sh
 ~/ROOT/5.34.14/x86_64-mac108-clang42-opt/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShape();


 http://root.cern.ch/drupal/content/tutorials
 http://root.cern.ch/root/html/tutorials/geom/rootgeom.C.html
 $ROOTSYS/share/root/tutorials/geom/rootgeom.C      /opt/local/share/root/tutorials/geom/rootgeom.C
 open -a Xcode_old /opt/local/etc/root/system.rootrc $ROOTSYS/share/root/tutorials/geom/rootgeom.C
 open -a Xcode_old /opt/local/etc/root/system.rootrc

 open -a Xcode_old /opt/local/share/root/tutorials/README
 /opt/local/share/root/tutorials/gl       root OpenGL tutorials

 run some tutorials as described in ~/tex/plotting/plotting.tex  under root

 here can run geometry with OpenGL
 cd /tmp/$LOGNAME ; root
 .x /opt/local/share/root/tutorials/geom/rootgeom.C

 menu bar of canvas window
 View / View with OpenGL
 File / Save viewer     .eps or .pdf

 ---- test editing geometry ----  see also  open ~/temp/manuals/ROOTUsersGuide.pdf   p.503 ff
 source ~/bin/my_Xcode_env.sh clang 1 root5
 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; $ROOTSYS/bin/root
 TGeoManager *geom = new TGeoManager("MyGeom","Test builder");
 geom->Edit(""); // open c1 Canvas for geometry editing


 --- test transparency
 source ~/bin/my_Xcode_env.sh 4.8 1 1 rootgit     ; echo text transparent ok
 source ~/bin/my_Xcode_env.sh 4.8 1 1 rootgit          ; echo text transparent ok
 source ~/bin/my_Xcode_env.sh clang 1 root5      ; echo no trasparency

 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 TestTransparency();




 --- viewer3DLocal.C
 source ~/bin/my_Xcode_env.sh clang 1 root5
 cd $ROOTSYS/tutorials/gl
 $ROOTSYS/bin/root
 .x viewer3DLocal.C+
    failed with   file not found: @rpath/libGLEW.so for architecture x86_64

 source ~/bin/my_Xcode_env.sh 4.8 1 1 rootgit
 cd /tmp/hbu ; rm -rf "/tmp/$LOGNAME/"* ; cp -prf /opt/local/share/root/tutorials . ; export ROOTSYS=/tmp/hbu/
 cd $ROOTSYS/tutorials/gl ; $ROOTSYS/bin/root
 .x viewer3DLocal.C+
 -- here showing volumes,   not transparency ?     modify     open /tmp/hbu/tutorials/gl/viewer3DLocal.C       fTransparency = 0 --->    fTransparency = 50    no change


 cd $ROOTSYS/tutorials/gl ; $ROOTSYS/bin/root
 .x glvox2.C
 -- here showing volumes,  seems no transparency


 http://root.cern.ch/drupal/content/root-graphics-do-list
 tutorials/geom/geomD0.C      uses     vol->SetTransparency(transp);



 --------- just make and save the line
 cd /tmp/$LOGNAME/ ; ln -sf ~/myroot/rootlogon.C; ln -sf ~/c/MyRoot/MyRootConst.h ;  time root
 TEveManager::Create(kFALSE); // init global gEve, here with false to not show anything    https://root.cern.ch/root/html534/TEveManager.html   http://root.cern.ch/root/html/TGeoManager.html
 TEveLine* line = new TEveLine("mytrack1");
 line->SetMainColor(kBlack);
 line->SetNextPoint(0,0,0); // x,y,z
 line->SetNextPoint(0,0,300);
 line->SaveAs("mytrack1.root"); // save the line
 line->SaveAs("mytrack1.C"); // save the line   in .C   format,   shows that it is just a TPolyMarker3D

 -- now read and show this line
 cd /tmp/$LOGNAME/ ; ln -sf ~/myroot/rootlogon.C; ln -sf ~/c/MyRoot/MyRootConst.h ;  time root
 TEveManager::Create(kTRUE); // type e  to turn to white
 TFile* f=new TFile("mytrack1.root"); // read one track!
 TEveLine* line=(TEveLine*) f->Get("mytrack1");
 gEve->AddElement(line);
 gEve->Redraw3D();


 --- this is the C code generated ---
 TPolyMarker3D *pmarker3D = new TPolyMarker3D(2,20,"");
 pmarker3D->SetName("mytrack1");

 Int_t ci;      // for color index setting
 TColor *color; // for color definition with alpha
 ci = TColor::GetColor("#00ff00");
 pmarker3D->SetMarkerColor(ci);
 pmarker3D->SetMarkerStyle(20);
 pmarker3D->SetPoint(0,0,0,0);
 pmarker3D->SetPoint(1,0,0,300);
 pmarker3D->Draw();


 --- from .root to .C ---   needs filename and which type of object

  cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; $ROOTSYS/bin/root
  .L ~/myroot/my_TFile_info.C
  my_TFile_info("/Users/hbu/www/Geom/002_px_0b1_eu.root");     // object is TEveLine
  my_TFile_info("/Users/hbu/www/Geom/LHC_IR_5.root",2);        // object is TGeoManager
  my_TFile_info("/Users/hbu/c/ansiitest/Ntuple3Example.root"); // object is TTree

 --- read .root and translate to .C
 cd /tmp/$LOGNAME/; ln -sf ~/myroot/rootlogon.C . ; $ROOTSYS/bin/root
 .L ~/myroot/my_root2C.C
 my_root2C("/Users/hbu/www/Geom/LHC_IR_5.root");          // TGeoManager
 my_root2C("/Users/hbu/www/Geom/TLEP_geom.root");         // TGeoManager
 my_root2C("/Users/hbu/www/Geom/002_px_0b1_eu.root");     // TEveLine
 my_root2C("/Users/hbu/c/ansiitest/Ntuple3Example.root"); // TTree, .C conversion not implemented
 my_root2C("/Users/hbu/c/ansiitest/TestNtuple1.root");
 my_root2C("/Users/hbu/c/BeamTrack/All_Part_turn_256_b0.root");
 my_root2C("/Users/hbu/c/BeamTrack/TurnHist_b1.root");

 .q



 new TBrowser; // to inspect: Geometry, Master Volume, Cone1, Cone2, ELTU, COMP;   same also in Top_1

 ------ test import export gdml  ------
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShape();
 // gGeoManager->CloseGeometry(); // not needed here
 gGeoManager->Export("Shape.gdml");
 .q
 cp -pf Shape.gdml Shape_$ROOTVERSION.gdml

 cd /tmp/$LOGNAME/; root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShape();
 gGeoManager->CloseGeometry();
 gGeoManager->Export("Shape.gdml");
 gGeoManager->Export("Shape.C");
 gGeoManager->Export("Shape.root");
 my_Geom_Diagnostics();
 .q
 cp -pf Shape.gdml Shape_myroot.gdml
 cp -pf Shape.gdml Shape_myroot.gdml

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 gGeoManager->Import("Shape_root_git.gdml");
 // gGeoManager->Import("Shape_root5.gdml");
 gGeoManager->GetTopVolume()->Draw("ogl"); // shape ok, color lost - in fact not an essential volume information,   all ok, also with root_git, root6

 new TBrowser; // to inspect:  default, Master Volume, Cone1, Cone2, ELTU, COMP;   same also in Top_1

 diff /tmp/hbu/Shape_root_git.gdml  /tmp/hbu/Shape_myroot.gdml  ;  echo identical
 diff /tmp/hbu/Shape_root_git.gdml  /tmp/hbu/Shape_root5.gdml   ;  echo identical
 open -a FileMerge ; echo compare Shape_root_git.gdml Shape_root5.gdml


      Shapes2

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 ShowShapes2();
 gGeoManager->CloseGeometry(); // not essential here,  Info in <TGeoManager::CloseGeometry>: 9 nodes/ 9 volume UID's in default geometry
 gGeoManager->Export("Shapes2.gdml");
 gGeoManager->Export("Shapes2.C");
 gGeoManager->Export("Shapes2.root");
 my_Geom_Diagnostics();
 new TBrowser; // see Geometry, Top_1
 .q

 cd /tmp/$LOGNAME/; root
 .L ~/c/MyRoot/init.C
  MDISim_init();
 .L ~/c/MDISim/MyGeoManager.C
    gGeoManager->Import("/Users/hbu/www/Geom/Shapes2.root"); // all ok
 // gGeoManager->Import("/Users/hbu/www/Geom/Shapes2.gdml"); // shapes ok, color lost
 // gGeoManager->Import("/Users/hbu/Downloads/alice_mini_geom.root"); // gGeoManager import no error -- but also gGeoManager still 0, needs Eve
 // gEve->GetGeometry("/Users/hbu/Downloads/alice_mini_geom.root"); // segmentation violation -- no geometry
 my_Geom_Diagnostics();
 gGeoManager->GetTopVolume()->Draw("ogl"); // shapes ok, some color lost in gdml


 --- safe for use with geant4 :
 cp -pf /tmp/hbu/Shape.gdml   ~/www/Geom/
 cp -pf /tmp/hbu/Shapes2.gdml ~/www/Geom/


 --- use in geant4, see  ~/mygeant4/HowToUpdateRunGeant4.txt
 cp -pf $G4INSTALL/examples/extended/persistency/"gdml/G01/"*.mac /tmp/hbu/
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml Shape.gdml      ;  echo geom visible in black and white, zoom with touch panel, ..
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml Shapes2.gdml    ;  echo geom visible in black and white      both ok


 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 SynRadGeo(); // creates SynRadGeo and shows in GL viewer
 gGeoManager->CloseGeometry();
 gGeoManager->Export("SynRadGeo.gdml");  // export geometry in gdml,  medium parameters lost, world ref Top and solid Top
 gGeoManager->Export("SynRadGeo.root");  // export the geometry
 gGeoManager->Export("SynRadGeo.C");     // export the geometry in C format     saves also Medium parameters   par[1]  = 1.100000; // ifield   par[2]  = 2.200000; // fieldm
 gGeoManager->Export("SynRadGeo.xml");   // export in xml format, medium parameters kept
 .q
 cp -pf /tmp/hbu/SynRadGeo.* ~/www/Geom/


 cp -pf $G4INSTALL/examples/extended/persistency/"gdml/G01/"*.mac /tmp/hbu/
 cd /tmp/hbu ; ~/mygeant4/bin/load_gdml SynRadGeo.gdml      ;  echo invalid drawable -- more needed


 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 TGeoManager* gGeoManager;
 // gGeoManager->Import("SynRadGeo.gdml");   //   https://root.cern.ch/doc/master/classTGeoManager.html
 gGeoManager->Import("/Users/hbu/www/Geom/SynRadGeo.gdml");
 gGeoManager->GetTopVolume()->Draw("ogl"); // shape ok, color lost




     read geometry produced by geant4
 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 TGeoManager* gGeoManager;
 gGeoManager->Import("/Users/hbu/www/Geom/TestEm16.gdml");
 gGeoManager->Export("TestEm16.C");
 // gGeoManager->Import("/Users/hbu/www/Geom/SynRadGeo.gdml");
 .q

 cd /tmp/$LOGNAME/; $ROOTSYS/bin/root
 .L ~/c/MDISim/MyGeoManager.C
 Show_RaceTrack();

 ------- can draw several together  with or without projections,  in RPhi and RhoZ from curevent see only last geometry
 cd /tmp/$LOGNAME/; rm -f "/tmp/$LOGNAME/"* ; root
 .L ~/c/MyRoot/init.C
 MDISim_init();
 unsigned int verbose;TEveElement* el;bool AllowForProjections;
    el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_HER_r.root",AllowForProjections=true,verbose=0);
    el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_HER_l.root",AllowForProjections=true,verbose=0);
    el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_LER_r.root",AllowForProjections=true,verbose=0);
    el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_LER_l.root",AllowForProjections=true,verbose=0);
 // el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Shape.root",  AllowForProjections=true,verbose=0);
 // el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Shapes2.root",AllowForProjections=true,verbose=0);
 // el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Shape_eve.root",  AllowForProjections=true,verbose=1);
 // el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Shapes2_eve.root",AllowForProjections=true,verbose=1);

 //---   wait a bit,   move cursor,   sleep(1);   not enough
 ShowProjections(el,verbose); // wait a bit before this command, otherwise segmentation violation     sleep(1)
 PlotGuidesAtOrigin(); // in Viewer 1

 bool isRungeKutta =true, LER=true;  MyEveFieldTest(isRungeKutta,LER); // nice helix around z-axis, tracks also visible in Multi View #D view, but not yet projections


 --------------- Eve magnetic field propagation ---------------
 $EDITOR $ROOTSYS/tutorials/eve/track.C    TEveTrackPRopagator with different magnetic field configurations.

 cd /tmp/$LOGNAME ; time root
 .L ~/c/MyRoot/init.C
 MDISim_init();
 .L ~/c/MDISim/MyGeoManager.C
 unsigned int verbose=1;TEveElement* el;MultiView* gMultiView=0;bool AllowForProjections=true;
 bool NoEve=false; // tracking also works without Eve,  but trouble with display
 if(NoEve) { gGeoManager->Import("/Users/hbu/www/Geom/Belle_LER_l.root"); if(gGeoManager) my_geo_track(verbose); gGeoManager->GetTopVolume()->Draw(); gGeoManager->GetTopVolume()->Draw("ogl"); } else { el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_LER_l.root",AllowForProjections,verbose); } // use Eve
 //  if(NoEve) { gGeoManager->Import("/Users/hbu/www/Geom/Belle.xml");  if(gGeoManager) my_geo_track(verbose); gGeoManager->GetTopVolume()->Draw(); gGeoManager->GetTopVolume()->Draw("ogl"); } else {el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Belle_LER_l.root",AllowForProjections,verbose); } // use Eve
 bool isRungeKutta =true, LER=true;
 MyEveFieldTest(isRungeKutta,LER); // nice helix around z-axis
 PlotGuidesAtOrigin();

 my_Geom_Diagnostics(); // with loop over volumes







 before daughter definition
 rc->Print() OBJ: TEveRecTrackT<double>	TEveRecTrackT<double>	Template for reconstructed track (also used in VSD).
 track->Print() TPolyMarker3D N=0, Option=
 track->PrintPathMarks() TEveTrack 'Charge 1', number of path marks 0, label -1
 after  daughter definition
 TEveTrack 'Charge 1', number of path marks 2, label -1
 Daughter   p: 0.000000 0.000000 0.000000 Vertex: 1.479084e+00 -4.370661e+00 3.119761e+00 0 Extra:0.000000 0.000000 0.000000
 Daughter   p: 0.000000 0.000000 0.000000 Vertex: 5.772345e+01 -8.977011e+01 -9.783746e+00 0 Extra:0.000000 0.000000 0.000000




 -- generate my helix files       | tee /tmp/hbu/MDISim.out      ;
 with main : proj=ansiitest ; targ=Test_myHelix ; cmakeUnix $proj ; cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv /tmp/$LOGNAME/$proj/$targ /tmp/$LOGNAME/Release/$targ ;  cd /tmp/$LOGNAME/; rm -f "/tmp/$LOGNAME/"*; Release/Test_myHelix ; echo ls -l ; ls -l *.out
 or with root :
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"*.out ; time root
 .L ~/c/MyRoot/init.C
 MDISim_init();
  .L ~/c/ansiitest/myHelix.C
 unsigned int verbose=2;
 double B=1.5; // 1.5 Tesla
 double theta=-0.0415;
 double zmax=2;
 myHelix hl(B,1,theta,verbose);
 hl.Print();
 for(double p=0.2;p<1.2;p+=0.2) { hl.set_p(p); hl.make_ntuple(zmax); } // p in GeV/c
 hl.set_p(100); hl.make_ntuple(zmax);
 .q
 ls -l *.out ; grep "Tesla" MDISim.out

 --- for plotting with tracks in projections see    main_AddTracksToGeom.C
 cd /tmp/$LOGNAME ; time root
 .L ~/c/MyRoot/init.C
 MDISim_init();
 unsigned int verbose=2;TEveElement* el;bool AllowForProjections=true;
 // const char* fname="/Users/hbu/www/Geom/LEIR.root";
 // const char* fname="http://root.cern.ch/files/alice_ESDgeometry.root"; // ok, has class TEveGeoShapeExtract
 // const char* fname="/Users/hbu/Downloads/alice_mini_geom.root";
 // const char* fname="/Users/hbu/Downloads/AliVSD.root"; // with tracks, key Names, TDirectoryFile  Event0001 ..  Event0032
 // const char* fname="/Users/hbu/Downloads/alice_ESDs.root"; // does not show with  StartEveWithGeomDisplay, has  esdTree, HLTesdTree objets
 // const char* fname="/Users/hbu/www/Geom/LHC_IR_5.root";
    const char* fname="/Users/hbu/www/Geom/Belle_LER_l.root";
 // const char* fname="/Users/hbu/www/Geom/Shape.root";
 el=StartEveWithGeomDisplay(fname,AllowForProjections=true,verbose=2);
 const double zmin=-2,zmax=2,Scale_xy=1;
 verbose=2;PlotTracksInFiles("/tmp/hbu/Helix*.out","",zmin,zmax,kGreen,Scale_xy,verbose);


 */

#include "MyCppStdHeaders.H"
#include <TRint.h>
#include <TGeoManager.h>
#include <TCanvas.h>

//#include "Ntuple.h"
//#include "MyRoot.h"

#include "MyGeoManager.h"

int main(int argc,char * const argv[])
{
#ifndef __CLING__
  TRint* theApp = new TRint("",0,NULL);
#endif
  // CheckFeatures();
  // MinimalGeom(true);
  // ShowShape(); // test beam pipe shapes
  //ShowShapes2(); // test beam pipe shapes
  SynRadGeo();

  gGeoManager->CloseGeometry(); // probably not really needed, changes order as visible in gdml output
  my_Geom_Diagnostics();

  // gGeoManager->Export("Shapes2.gdml");
  gGeoManager->Export("SynRadGeo.gdml");

#ifdef __CLING__
  cout << "running ShowShape with CINT"<< '\n';
#else
  cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << '\n';
  cout << "running ShowShape in line mode. When done quit root from the menu bar / File " << '\n';
  theApp->Run();
#endif
};
