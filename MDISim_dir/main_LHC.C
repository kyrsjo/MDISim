/* ~/c/MDISim/main_LHC.C        ~/mad/lhc/lhc_V6_5_survey.madx

 rm -rf "/tmp/$LOGNAME/"*

 Unix make
 proj=MDISim ; targ=LHC ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=LHC ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h

 Xcode
 proj=MDISim ; targ=LHC ; cmakeXcode $proj


 --- 3 alternatives to make  /tmp/$LOGNAME/Release/LHC    for running in line mode ---
 rm -rf "/tmp/$LOGNAME/Release"* ; mkdir /tmp/$LOGNAME/Release ; cd /tmp/$LOGNAME/Release ; cmake ~/c/MDISim      ; cd /tmp/$LOGNAME/Release ; make LHC -j5 ; /tmp/$LOGNAME/Release/LHC
 rm -rf "/tmp/$LOGNAME/Release"* ; mkdir /tmp/$LOGNAME/Release ; cd ~/c/MDISim/ ; make LHC -j5 ; mv -f LHC /tmp/$LOGNAME/Release/. ; make clean ; /tmp/$LOGNAME/Release/LHC
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target LHC -configuration Release ; Release/LHC -h

 save geometry files in ~/www/Geom  for import from web    TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root");




 --- run in line mode by
 cd /tmp/$LOGNAME  ; rm -f * ; time Release/LHC ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf


 read b1 and b2 optics and twiss files
 combined and plot both beams around ir

 OTHER_CFLAGS = -Wall -fvisibility=default $(MyClib_CFLAG) $(ROOT_CFLAGS) -I$(HOME)/c/MyRoot -I$(HOME)/myroot -DCheckSyntax
 OTHER_LDFLAGS = $(MyClib_LIB) $(MyRoot_LIB) $(ROOT_LIBS)

 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -alltargets clean
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -alltargets -configuration Release; echo good test to make all
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -list
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target LHC -configuration Debug
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target LHC -configuration Release; /tmp/$LOGNAME/Release/LHC
 cd /tmp/$LOGNAME ; rm -f * ; time Release/LHC ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf


 ---  1. step just setup geometry, raw geometry from tfs    and export to root file,   no plotting,  can also be done by just running    cd /tmp/$LOGNAME ; time ~/c/MDISim/MyNtuple2Geom
 cd /tmp/$LOGNAME ; time root -b
 .L ~/c/MDISim/MyNtuple2Geom.C
 unsigned int verbose=1; vector<string> RemoveColumns={"GLOBALTILT","SLOT_ID","ASSEMBLY_ID"}; // C++11
 nt_b1=ReadAndMerge("~/mad/lhc/output/lhcb1_twiss_from5.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",verbose)); nt_b1.RemoveVarList(RemoveColumns); if(verbose) nt_b1.PrintSummary();
 nt_b2=ReadAndMerge("~/mad/lhc/output/lhcb2_twiss_from5.tfs","~/mad/lhc/output/lhcb2_survey_from5.tfs",verbose)); nt_b2.RemoveVarList(RemoveColumns); if(verbose) nt_b2.PrintSummary();
 if(verbose>1) nt_b1.WriteAsciiNtFile(NULL,"lhcb1_merged.tfs");
 // const double zmin=0,zmax=40,Scale_xy=100;
 const double zmin=0,zmax=320,Scale_xy=100; // to beginning of arcs
 // const double zmin=0,zmax=3000,Scale_xy=100;
 // const double zmin=-99999,zmax=99999,Scale_xy=1; // whole ring -- barely visible, use no scalefact, --- not working well at the moment, 1.2MB .root
 gSystem->Load("libGeom");
 verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",nt_b1,zmin,zmax,kBlue-10,Scale_xy,verbose); // kBlue-10 for light blue
 verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",nt_b2,zmin,zmax,kRed -10 ,Scale_xy,verbose); // kRed -10 for light red  use same name  LHC_IR_5   to draw together
 gGeoManager->Export("LHC_IR_5.gdml");
 gGeoManager->Export("LHC_IR_5.root");  // export the whole geometry      to display start root without -b  and type    gGeoManager->GetTopVolume()->Draw("ogl");    gGeoManager->Export("LHC_IR_5.C");
 .q

 or in line mode;   600 is kBlue, 632 is kRed   see   $EDITOR $ROOTSYS/include/Rtypes.h
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target MyNtuple2Geom -configuration Release
 /tmp/hbu/Release/MyNtuple2Geom -v LHC_IR_1 IP 0 320 100 600 ~/mad/lhc/output/lhcb1_twiss_from1.tfs ~/mad/lhc/output/lhcb1_survey_from1.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from1.tfs ~/mad/lhc/output/lhcb2_survey_from1.tfs
 /tmp/hbu/Release/MyNtuple2Geom -v LHC_IR_5 IP 0 320 100 600 ~/mad/lhc/output/lhcb1_twiss_from5.tfs ~/mad/lhc/output/lhcb1_survey_from5.tfs 632 ~/mad/lhc/output/lhcb2_twiss_from5.tfs ~/mad/lhc/output/lhcb2_survey_from5.tfs

 ---   same for IR1
 cd /tmp/$LOGNAME ; time root
  .L ~/c/MDISim/MyNtuple2Geom.C
 unsigned int verbose=0; vector<string> RemoveColumns={"GLOBALTILT","SLOT_ID","ASSEMBLY_ID"}; // C++11
 Ntuple nt_b1(ReadAndMerge("~/mad/lhc/output/lhcb1_twiss_from1.tfs","~/mad/lhc/output/lhcb1_survey_from1.tfs",verbose)); nt_b1.RemoveVarList(RemoveColumns); if(verbose) nt_b1.PrintSummary();
 Ntuple nt_b2(ReadAndMerge("~/mad/lhc/output/lhcb2_twiss_from1.tfs","~/mad/lhc/output/lhcb2_survey_from1.tfs",verbose)); nt_b2.RemoveVarList(RemoveColumns); if(verbose) nt_b2.PrintSummary();
 const double zmin=0,zmax=320,Scale_xy=100; // to beginning of arcs
 gSystem->Load("libGeom");
 verbose=0; GeomFromMyNtuple("LHC_IR_1","IP",nt_b1,zmin,zmax,kBlue-10,Scale_xy,verbose); // kBlue-10 for light blue
 verbose=0; GeomFromMyNtuple("LHC_IR_1","IP",nt_b2,zmin,zmax,kRed -10,Scale_xy,verbose); // kRed -10 for light red  use same name  LHC_IR_5   to draw together
 gGeoManager->Export("LHC_IR_1.root");  // export the whole geometry      to display start root without -b  and type    gGeoManager->GetTopVolume()->Draw("ogl");    gGeoManager->Export("LHC_IR_1.C");
 .q



 --- 3d geometry plot with ogl GL viewer, standard root ---
 cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C; ln -sf ~/c/MyRoot/MyRootConst.h ; time $ROOTSYS/bin/root
 // TGeoManager::Import("LHC_IR_1.root"); // local
 // TGeoManager::Import("LHC_IR_5.root"); // local
    TGeoManager::Import("/Users/hbu/www/Geom/LHC_IR_5.root");
 // TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root"); // or local TGeoManager::Import("LHC_IR_5.root");        http://hbu.web.cern.ch/hbu/Geom/LHC_IR_1.root
 gGeoManager->GetTopVolume()->Draw("ogl"); // "ogl" drawing does not allow for track and interferes with drawing in c1  -- so better keep separate


 --- import to gEve      standard root ---      type e for white
 cd /tmp/$LOGNAME ; $ROOTSYS/bin/root
 TEveManager::Create();
 gGeoManager = gEve->GetGeometry("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root");
 TGeoNode* node = gGeoManager->GetTopNode();
 TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
 gEve->AddGlobalElement(en);
 gEve->Redraw3D(kTRUE);

 --- import to gEve      standard root ---   using StartEveWithGeomDisplay  which switches to white
 cd /tmp/$LOGNAME ; $ROOTSYS/bin/root
 .L ~/c/MyRoot/init.C
 MDISim_init2();
 StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_1_eve.root")
 StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5_eve.root")
 StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5_LR_eve.root")



 --- plot the geometry and tracks MyTrack3D   does not work well without Eve ---
 cd /tmp/$LOGNAME ; $ROOTSYS/bin/root
 .L ~/c/MyRoot/init.C
  MDISim_init2();
 TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root"); // or local TGeoManager::Import("LHC_IR_5.root");
 unsigned int verbose=1;
 Ntuple nt_b1(Ntuple("/Users/hbu/mad/lhc/twiss_track_ref/002_px_0b1.tfs")); if(verbose) nt_b1.PrintSummary(); // tracks IP5
 Int_t PdgCode=2212; // Proton
 const double zmin=0,zmax=320,Scale_xy=100;
 MyTrack3D(nt_b1,PdgCode,zmin,zmax,kGreen,Scale_xy,verbose); // attach tracks to current geometry
 gGeoManager->DrawTracks();  // here "ogl" does not work



 -- 2d plot and cleaning apertures
 rm -rf "/tmp/$LOGNAME/"*
 cd /tmp/$LOGNAME ; time root
  .I /Users/hbu/myroot
 .L ~/c/MDISim/Apertures.C
 .L ~/c/MDISim/IPpos.C
 .L ~/c/MDISim/Twiss_util.C
 .L ~/c/MDISim/myGraph.C
 .L ~/c/MDISim/MyNtuple2Geom.C
 .L ~/c/MDISim/main_LHC.C
 bool CleanUpApertures=true;
 unsigned int verbose=1;
 Ntuple nt_b1(ReadAndMerge("~/mad/lhc/output/lhcb1_twiss_from5.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",verbose)); if(verbose) nt_b1.PrintSummary();
 Ntuple nt_b2(ReadAndMerge("~/mad/lhc/output/lhcb2_twiss_from5.tfs","~/mad/lhc/output/lhcb2_survey_from5.tfs",verbose)); if(verbose) nt_b2.PrintSummary();
 // if(verbose) FindIPs(nt_b1,nt_b2,verbose); // slow, just for info
 Plot_LHC(nt_b1,nt_b2,CleanUpApertures,verbose); // here below in main_LHC.C




 --- directly in root  2d  already cleaned aperture       IP5 up to 22 m  really just circles  which translate to TUBE or CONE
 cd /tmp/$LOGNAME ; time root
 // .I /Users/hbu/c/MyClib
 .I /Users/hbu/myroot
 .I /Users/hbu/c/MyRoot/
 .L ~/c/MDISim/Apertures.C
 .L ~/c/MDISim/IPpos.C
 .L ~/c/MDISim/Twiss_util.C
 .L ~/c/MDISim/myGraph.C
 .L ~/c/MDISim/MyNtuple2Geom.C
 .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/main_LHC.C
 unsigned int verbose=1;
 bool CleanUpApertures=false; // do not clean apertures, already done - read cleaned apertures
 Ntuple nt_b1("~/mad/lhc/output/LHC_aper_cleaned_b1.out"); if(verbose) nt_b1.PrintSummary(); // l,theta, angle missing
 Ntuple nt_b2("~/mad/lhc/output/LHC_aper_cleaned_b2.out"); if(verbose) nt_b2.PrintSummary();
 Plot_LHC(nt_b1,nt_b2,CleanUpApertures,verbose);  // 2dim plot
 const double zmin=-320,zmax=320;
 gSystem->Load("libGeom");
 verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",nt_b1,zmin,zmax,kBlue,100,verbose);
 verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",nt_b2,zmin,zmax,kRed ,100,verbose);
 gGeoManager->Export("LHC_IR_5.root");

 3D volumes:
 translate APERTYPES to volumes        generally based on markers without length -  get from next transition
 TGeoTube *tub = new TGeoTube(rmin, rmax, halfZ);   http://root.cern.ch/root/html/TGeoTube.html
 rmin take APER_1    rmax=rmin+thickness;    thickness maybe start with 1 mm


 turn off 2-d Draw   in saved   MyGeom.C
 cd /tmp/hbu ; sed "s/  gGeoManager->GetTopVolume()->Draw();/\/\/gGeoManager->GetTopVolume()->Draw();/" MyGeom.C > temp.C ; mv temp.C MyGeom.C
 --- read back the saved geometry and plot
 cd /tmp/$LOGNAME ; time root
 .x MyGeom.C()
 gSystem->Load("libGeom");
 gGeoManager->GetTopVolume()->Draw("ogl");
 c1->Close();





 --- read Geometry and tracks written in root format   --- in standard root        use TFile to read local files   or    TWebFile   to read web files
 cd /tmp/$LOGNAME ;  time $ROOTSYS/bin/root
 gSystem->Load("libGeom");
 TEveManager::Create();
 gGeoManager = gEve->GetGeometry("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root");
 TGeoNode* node = gGeoManager->GetTopNode();
 TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
 gEve->AddGlobalElement(en);
 gEve->Redraw3D(kTRUE);

 TGLViewer *v = gEve->GetDefaultGLViewer();   // get access to TGLViewer, seen in geom_lhcb.C
 v->ColorSet().Background().SetColor(kWhite); // change the Background color, by default black, better use white        needs  libRGL   use -lRGL
 v->SetStyle(TGLRnrCtx::kFill);  // choice is kFill, kOutline, kWireFrame;   use default kFill
 TGLLightSet* lightset = v->GetLightSet(); lightset->SetLight(TGLLightSet::kLightBottom, kFALSE);  // by default light from all sides. Here turn off light from bottom. see also glViewerExercise.C,

 TWebFile* f=new TWebFile("http://hbu.web.cern.ch/hbu/Geom/002_px_0b1_eu.root"); // read one track in beam1
 TEveLine* line=(TEveLine*) f->Get("002_px_0b1_eu");
 gEve->AddElement(line);

 TWebFile* f=new TWebFile("http://hbu.web.cern.ch/hbu/Geom/002_px_0b2_eu.root"); // read one track in beam2
 TEveLine* line=(TEveLine*) f->Get("002_px_0b2_eu");
 // line->Print();  cout << " line length=" << line->CalculateLineLength() << '\n';
 gEve->AddElement(line);
 gEve->Redraw3D();



 also tested ok on lxplus
 ssh -X hbu@lxplus.cern.ch
 root
 .x ~/www/Geom/my_Geom_Eve_Display.C


 for info type :   f->ls(); f->Map();  line->Print();  cout << " line length=" << line->CalculateLineLength() << '\n';
 
 
 
 
 
 SynRad LHC
 

 cd /tmp/$LOGNAME ; time root
 .L ~/c/MyRoot/init.C
 MDISim_init();
 unsigned int verbose=1;
 // string Fname_b1_o="~/mad/lhc/output_run2/lhcb1_twiss_from1.tfs";
 string Fname_b1_o="~/mad/lhc/output_run2/lhcb1_twiss_from1_sep1.tfs"; // separation on
 string Fname_b1_s="~/mad/lhc/output_run2/lhcb1_survey_from1.tfs";
 Ntuple nt_b1( ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose) ); if(verbose) nt_b1.PrintSummary(); // Ntuple or  NtupleRoot to test  nt_b1.WriteRoot
 Beam b1(Fname_b1_o.c_str(),verbose); // get Energy and synchr integrals from optics twiss header
 b1.RFHV_from_twiss(nt_b1 );
 cout << " from twiss rfHV=" << b1.RFHV << '\n'; // finds 4.8 GV, not bad
 if(b1.RFHV==0) b1.SetRF( 12.e-3, 35640 ); // if not set, give here
 b1.Get_Part().PrintLorentz(cout);
 double nsig=1; // quad radiation from sawtooth + beam size.  0 is sawtooth only
 //EmitRatio=0.001; // as a start
 //b1.EmitFromSynrad(EmitRatio);
 b1.Print();
 CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,verbose); // calculate beam sizes and divergences and add to ntuple
 verbose=1;
 CalcSynrad(nt_b1,b1,verbose,nsig);

 el=StartEveWithGeomDisplay(ResolveHome("~/www/Geom/LHC_IR_5_LR.root")); // both pipes
 double zmin=-200,zmax=300,Scale_xy=100;
 EveSingleTrackDisplay(nt_b1,zmin,zmax,kGreen,Scale_xy,verbose); // plot design path
 bool goto_CM_units=true; // normally true
 Plot_Bend_SR_Cones(nt_b1,b1,zmin,zmax,Scale_xy,verbose,goto_CM_units,-1); // myGraph3D.C
 
 */

#ifndef __CLING__
#include "MyCppStdHeaders.H"
#include <TRint.h>
#include <TCanvas.h>
#include <TGeoManager.h>
#endif

#include "myEve.h" // StartEveWithGeomDisplay
#include "Apertures.h"
#include "IPpos.h"
//#include "Lorentz.h"
//#include "Twiss_util.h" // Beam
#include "myGraph.h"    // MyApertureGraph
#include "myGraph3D.h"  // PlotTracksInFiles
#include "MyNtuple2Geom.h" // GeomFromMyNtuple

void FindIPs(const Ntuple &nt_b1,const Ntuple &nt_b2,unsigned int verbose) // for the moment only for info
{
  if(verbose) cout << "FindIPs start verbose=" << verbose <<'\n';
  IPpos *ips_b1=new IPpos(&nt_b1,verbose); ips_b1->Print(); // in Apertures.C   CINT  has problems with variable vector size
  IPpos *ips_b2=new IPpos(&nt_b2,verbose); ips_b2->Print();
  CalcAveBeam(nt_b1,nt_b2,ips_b1,ips_b2,verbose); // construct the average beam from b1, b2, in  IPpos.C
}

void Plot_LHC(Ntuple& nt_b1,Ntuple& nt_b2,bool CleanUpApertures,unsigned int verbose)
{
  if(verbose) cout << "Plot_LHC start CleanUpApertures=" << CleanUpApertures << " verbose=" << verbose <<'\n';

  Apertures* LHC_aper_b1 = new Apertures(&nt_b1,verbose);
  Apertures* LHC_aper_b2 = new Apertures(&nt_b2,verbose);

  const double smax=350; // 320 m just start of arc,

  if(CleanUpApertures)
  {
    // work on Apertures of b1
    if(verbose>1) LHC_aper_b1->Print();
    LHC_aper_b1->CheckConsistence();
    LHC_aper_b1->CleanUpApertures();
    if(verbose>1) LHC_aper_b1->Print();
    LHC_aper_b1->CombineLeftRight(smax);
    LHC_aper_b1->CheckConsistence();
    if(verbose) LHC_aper_b1->Print(0,1000,1000);
    Ntuple* nt_cleaned_aper_b1=LHC_aper_b1->MakeNtuple();
    nt_cleaned_aper_b1->PrintSummary();
    nt_cleaned_aper_b1->WriteAsciiNtFile(NULL,"LHC_aper_cleaned_b1.out");

    // work on Apertures of b2
    if(verbose>1) LHC_aper_b2->Print();
    LHC_aper_b2->CheckConsistence();
    LHC_aper_b2->CleanUpApertures();
    if(verbose>1) LHC_aper_b2->Print();
    LHC_aper_b2->CombineLeftRight(smax);
    LHC_aper_b2->CheckConsistence();
    if(verbose) LHC_aper_b2->Print(0,1000,1000);
    Ntuple* nt_cleaned_aper_b2=LHC_aper_b2->MakeNtuple();
    nt_cleaned_aper_b2->PrintSummary();
    nt_cleaned_aper_b2->WriteAsciiNtFile(NULL,"LHC_aper_cleaned_b2.out");
  }

  // double smax= 50, smin=-smax;  double xmin=-0.20,xmax=0.20;
  double smin=-smax;  double xmin=-0.20,xmax=0.20;
  // double smax=6000; double xmin=-8500,xmax=100;
  LHC_aper_b1->PlotFrame(smin,smax,xmin,xmax); // plot frame and keep limits in Apertures class
  // LHC_aper_b2->PlotFrame(smin,smax,xmin,xmax);
  // LHC_aper_b1->PlotFrame(    0,smax,-xymax,xymax); // only right of IP
  cout << "Plot_LHC LHC_aper_b1->zvec.size()=" << LHC_aper_b1->zvec.size() << " call MyApertureGraph" << '\n';
  MyApertureGraph(LHC_aper_b1->zvec.size(),kBlue,LHC_aper_b1->zvec,LHC_aper_b1->x_2_vec,LHC_aper_b1->aper_1_vec);
  MyApertureGraph(LHC_aper_b2->zvec.size(),kRed ,LHC_aper_b2->zvec,LHC_aper_b2->x_2_vec,LHC_aper_b2->aper_1_vec);

  TCanvas* c2=LHC_aper_b1->GetCanvas();
  c2->SaveAs("MyApertureGraph_2d.pdf");

#ifndef __CLING__
  // c2->Close();
#endif
  if(verbose) cout << "Plot_LHC done" <<'\n';
}


#ifndef __CLING__
int main(int argc,char * const argv[]) // normally this
{
  unsigned int verbose=1;
#if __cplusplus >= 201103L // C++11 on
  if(verbose) cout << "main compiled with C++11 on" << '\n';
#else
  if(verbose) cout << "main compiled with C++11 off" << '\n';
#endif

#ifdef __CLING__
  if(verbose) cout << "__CLING__=" << __CLING__ << '\n';
#else
  if(verbose) cout << "__CLING__ not defined" << '\n';
#endif

#ifdef __CINT__
  if(verbose) cout << "__CINT__=" << __CINT__ << '\n';
#else
  if(verbose) cout << "__CINT__ not defined" << '\n';
  TRint* theApp = new TRint("",0,NULL);
#endif


  bool AddTrackToGeometry=true; // CSPE -- better extra program   AddTracksToGeom
  if(AddTrackToGeometry)
  {
    unsigned int verbose=1;
    // StartEveWithGeomDisplay("LHC_IR_5.root");  // local
    StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root");  // or local  local LHC_IR_5.root  or  saved /Users/hbu/www/Geom/LHC_IR_5.root  or  http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root
    const double zmin=0,zmax=330,Scale_xy=100;
    verbose=0;PlotTracksInFiles("~/mad/lhc/twiss_track_ref/0*_px*b1.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",zmin,zmax,kGreen,Scale_xy,verbose); // myGraph3D.C
    verbose=0;PlotTracksInFiles("~/mad/lhc/twiss_track_ref/0*_px*b2.tfs","~/mad/lhc/output/lhcb2_survey_from5.tfs",zmin,zmax,kGreen,Scale_xy,verbose);

    theApp->Run();
    exit(0);
  }

  bool CleanUpApertures=true;
  verbose=0;
  Ntuple *ntptr_b1=NULL,*ntptr_b2=NULL;
  if(CleanUpApertures)
  {
    ntptr_b1=new Ntuple(ReadAndMerge("~/mad/lhc/output/lhcb1_twiss_from5.tfs","~/mad/lhc/output/lhcb1_survey_from5.tfs",verbose));
    ntptr_b1->RemoveVarList({"GLOBALTILT","SLOT_ID","ASSEMBLY_ID"});
    if(verbose) ntptr_b1->PrintSummary();
    ntptr_b2=new Ntuple(ReadAndMerge("~/mad/lhc/output/lhcb2_twiss_from5.tfs","~/mad/lhc/output/lhcb2_survey_from5.tfs",verbose));
    ntptr_b2->RemoveVarList({"GLOBALTILT","SLOT_ID","ASSEMBLY_ID"});
    if(verbose) ntptr_b2->PrintSummary();
  }
  else // use existing cleaned aperture files
  {
    Ntuple* ntptr_b1=new Ntuple("~/mad/lhc/output/LHC_aper_cleaned_b1.out"); if(verbose) ntptr_b1->PrintSummary();
    Ntuple* ntptr_b2=new Ntuple("~/mad/lhc/output/LHC_aper_cleaned_b2.out"); if(verbose) ntptr_b2->PrintSummary();
    if(verbose) FindIPs(*ntptr_b1,*ntptr_b2,verbose);
  }

  bool Plot3D=true;
  if(Plot3D)
  {
    const double zmin=0,zmax=320,Scale_xy=100;
    // const double zmin=-320,zmax=320;
    // const double zmin=-15,zmax=15;
    //const double zmin=-180,zmax=180;
    verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",*ntptr_b1,zmin,zmax,kBlue,Scale_xy,verbose);
    verbose=0; GeomFromMyNtuple("LHC_IR_5","IP",*ntptr_b2,zmin,zmax,kRed ,Scale_xy,verbose); // use same name  LHC_IR_5   to draw together
    gGeoManager->Export("LHC_IR_5.root"); // export the geometry

    if(gGeoManager==NULL) TGeoManager::Import("LHC_IR_5.root");
    gGeoManager->GetTopVolume()->Draw("ogl"); // "ogl" drawing does not allow for track and interferes with drawing in c1  -- so better keep separate

    // Int_t PdgCode=2212; // Proton
    // mygraph3d->verbose=2; mygraph3d->MyTrack3D("LHC_IR_5",ntptr_b1,PdgCode);

    // StartEveWithGeomDisplay();

  }
  else // 2dim plot
  {
    if(verbose) FindIPs(*ntptr_b1,*ntptr_b2,verbose);
    Plot_LHC(*ntptr_b1,*ntptr_b2,CleanUpApertures,verbose); // in main_LHC.C
  }
#ifdef __CINT__
  cout << " running with CINT"<< '\n';
#else
  cout << " running in line mode. When done quit root from the menu bar / File " << '\n';
  theApp->Run();
#endif
};
#endif
