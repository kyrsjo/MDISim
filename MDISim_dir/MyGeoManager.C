/*
 ~/c/MDISim/MyGeoManager.C


 Navigation/tracking  --  straight track to boundary, field ignored -- would need vmc,       helix at least visible in Eve

 vmc
 src/etc/vmc
 src/montecarlo/vmc      ~/root_git/src/montecarlo/vmc/src/    uses field



 http://en.wikipedia.org/wiki/Euler_angles
 http://root.cern.ch/root/html/src/TGeoMatrix.cxx.html  most useful for rotation

 http://geant4.web.cern.ch/geant4/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/Detector/geomSolids.html  not much about rotation

 open ~/temp/manuals/ROOTUsersGuide.pdf       file:///Users/hbu/temp/manuals/ROOTUsersGuide.pdf
 Chapter 18 Geometry package  p. 433 ff       http://root.cern.ch/download/doc/ROOTUsersGuideChapters/Geometry.pdf
 18.4.3 Geometrical Transformations p. 474 ff


 January 2014
 ------------
 started to display IRs and beginning of arcs
 Courant Snyder  CS  coordinates from survey
 Merging ntuples with Euclidian Coordimates  (track, optics)    and   Survey
 Cleaner would be :
 Read survey, just to build up conversion from CS to Euclidian
 With extrapolation -  provide conversion for any x,y,z from CS to Euclidian for display



 About scaling x,y :
 -------------------
 ideally this would only be done in display, leaving the geometry untouched.
 It seems this is not foreseen in Root.
 Also not clear if there is a really consistent way to get the wanted effect :
 scale transverse dimensions  (apertures x,y in CS coordinates)
 scale up bending angles

 Dirty scaling which appears to work, see   grep -i scale ~/c/MDISim/myGraph.C
 dz*=sqrt(1.+ Scale_xy*theta_ave * Scale_xy*theta_ave); // dirty fix, increase length for Scale_xy
 theta_ave=atan(Scale_xy*theta_ave);
 TGeoTranslation trans(Scale_xy*xpos,0,zpos);

 double fScale[3]={Scale_xy,Scale_xy,1}; CurPos->SetScale(fScale);   // works for draw, but lost in export



 Scale transformations (TGeoScale class) - represent a scaled shrinking/enlargement, possibly different on all axes.

 cd /opt/local/include/root/ ; grep -i scale *.h

 try with
 RootScaleFac=false;       scale also top volume ?





 3D if nothing vivible :

 Guides / Axes / Origin

 Camera Home        to get back to initial unzoomed

 zoom in   right click drown

 rotate less fast :   command + mouse movement



 Euclidian and Courant-Snyder
 open ~/math/FromJohnJowett/MathematicaExamples/AcceleratorCoordinates/AcceleratorCoordinatesDemo.nb
 open ~/temp/manuals/mad8_user.pdf     page 13 in pdf
 THETA  PHI  PSI       given by Survey,   in simplest case only THETA nonzero, see for example  ~/mad/lhc/output/lhcb1_survey_from5.tfs

 where the theta rotation does

 cost    0    sint
 0       1      0
 -sint   0    cost

 open ~/ROOT/5.34.07/src/root/geom/geom/inc/TGeoMatrix.h
 open ~/ROOT/5.34.07/src/root/geom/geom/src/TGeoMatrix.cxx      and see
 void TGeoRotation::SetAngles(Double_t phi, Double_t theta, Double_t psi)
 use phi,  psi =0  theta=90 so that   sinpsi=0, cosphi=1,  costhe=0, sinthe=1    try SetAngles(theta,90,0)   Set with Euler Angles
 not bad, but still needs swap y,z;   rotate -90 deg around x,  ok

 Consider to use
 SetAngles(Double_t theta1, Double_t phi1, Double_t theta2, Double_t phi2,
 Double_t theta3, Double_t phi3)
 for more flexibility

 fRotationMatrix[0] = TMath::Cos(degrad*phi1)*TMath::Sin(degrad*theta1);    to get Cos(phi1) set   theta1 to 90
 fRotationMatrix[1] = TMath::Cos(degrad*phi2)*TMath::Sin(degrad*theta2);    to get 0         set   theta2 to 0
 fRotationMatrix[2] = TMath::Cos(degrad*phi3)*TMath::Sin(degrad*theta3);    to get



 Root rotation
 ---------------
 theta_deg, 0,0   same by   RotateZ
 cost   -sint    0       Tx =   0.000000
 sint    cost    0       Ty =   0.000000
 0         0     1       Tz =   0.000000


 0, theta_deg, 0    is
 1    0       0       Tx =   0.000000
 0   cost   -sint     Ty =   0.000000
 0   sint    cost     Tz =   0.000000

 0,0,theta_deg    or looks same as  theta_deg, 0,0
 cost   -sint    0       Tx =   0.000000
 sint    cost    0       Ty =   0.000000
 0         0     1       Tz =   0.000000


 print does
 0 1 2  tr0
 3 4 5  tr1
 6 7 8  tr2


 TGeoRotation *rot = new TGeoRotation("rot");     default constructor, makes unity matrix

 TGeoRotation *rot = new TGeoRotation("rot"); rot->SetAngles(-theta_deg,90,0);
 rot->RotateX(-90);


 rot->Print();
 double newphi,newtheta,newpsi;
 rot->GetAngles(newphi,newtheta,newpsi);
 cout  << " newphi=" << newphi << " newtheta=" << newtheta << " newpsi=" << newpsi << '\n';
 exit(1);






 TLEP with my scaling by 100
 theta=-0.00107148 atan(theta)=-0.00107148
 theta_deg=6.13915
 matrix  - tr=1  rot=1  refl=0  scl=0
 0.994265    0.000000   -0.106944    Tx =  -1.141131
 -0.000000    1.000000    0.000000    Ty =   0.000000
 0.106944    0.000000    0.994265    Tz = 110.199996
 i= 26              "BD07" add at z=     147.7 Tube of dz=   24.9999  r=      0.05                   x_2[i]=-0.0496097
 theta=-0.00214297 atan(theta)=-0.00214297
 theta_deg=12.2783

 with  RootScaleFac=true;
 theta=-0.00107148 atan(theta)=-0.00107148
 theta_deg=0.0613915
 matrix rot - tr=0  rot=1  refl=0  scl=0
 0.999999    0.000000   -0.001071    Tx =   0.000000
 -0.000000    1.000000    0.000000    Ty =   0.000000
 0.001071    0.000000    0.999999    Tz =   0.000000
 before ph->Print()
 matrix  - tr=1  rot=1  refl=0  scl=1
 0.999999    0.000000   -0.001071    Tx =  -0.011411
 -0.000000    1.000000    0.000000    Ty =   0.000000
 0.001071    0.000000    0.999999    Tz = 110.199996
 Sx=100.000000Sy=  100.000000Sz=    1.000000







 with  RootScaleFac=true;
 pMatrix300  same with   true/false    including translation
 tr[0]  = -23.812369;    tr[1] = 0.000000;    tr[2] = 315.631245;
 rot[0] =0.617141;    rot[1] = -0.000000;    rot[2] = -0.786853;
 rot[3] =-0.000000;    rot[4] = 1.000000;    rot[5] = -0.000000;
 rot[6] =0.786853;    rot[7] = 0.000000;    rot[8] = 0.617141;



 http://root.cern.ch/root/html/tutorials/eve/index.html    event display tutorials

 ls -l $ROOTSYS/tutorials/eve/   or in my case    ls -l $ROOTSYS/tutorials/eve/
 open $ROOTSYS/tutorials/eve/pythia_display.C

 // show the experiments geometries      http://root.cern.ch/root/html/tutorials/eve/geom_cms_playback.C.html     Eve Man Window     very impressive  --- try to add tracks
 cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C; ln -sf ~/c/MyRoot/MyRootConst.h ; root
 .x $ROOTSYS/tutorials/eve/csgdemo.C
 .x $ROOTSYS/tutorials/eve/geom_atlas.C
 .x $ROOTSYS/tutorials/eve/geom_cms.C
 .x $ROOTSYS/tutorials/eve/geom_cms_playback.C

 cd $ROOTSYS/tutorials/eve/ ; root
 .x geom_cms_playback.C



 Creating and Visualizing Tracks
 -------------------------------
 described in   http://root.cern.ch/download/doc/ROOTUsersGuideChapters/Geometry.pdf      1.5.8 Creating and Visualizing Tracks
 VirtualGeoTrack(Int_t id,Int_t pdg,TVirtualGeoTrack *parent=0, TObject *particle=0);
 cd ~/root_git       ; find . -type f       -exec cat {} \; | grep -R VirtualGeoTrack
 cd ~/root_git_build ; find . -name "*.h"   -exec cat {} \; | grep -R VirtualGeoTrack
 cd ~/root_git       ; find . -name "*.cxx" -exec cat {} \; | grep -R VirtualGeoTrack

 cd ~/root_git/src   ; find . -type f -exec grep -H "g3helx3" {} \;                    with info which file
 cd ~/root_git/src   ; find . -name "*.cxx" -exec cat {} \; | grep -R g3helx3          faster but no giving file name


 open ~/root_git/geom/geom/src/TGeoManager.cxx ~/root_git//geom/geom/src/TVirtualGeoTrack.cxx

 cd $G4INSTALL/source/visualization/OpenGL ; open src/G4OpenGLQtViewer.cc     use OpenGl with Qt dialog managing - not so useful
 cd $G4INSTALL/source/visualization/OpenGL ; open src/G4OpenGLViewer.cc
 http://geant4.cern.ch/support/training.shtml  geometry  http://geant4.web.cern.ch/geant4/collaboration/working_groups/geometry/training/D2-Basics.pdf
 with LHCb imported in root through GDML
 $G4INSTALL/examples/extended/persistency/gdml        http://gdml.web.cern.ch/GDML/       Geometry Description Markup Language (GDML)  used for geometry interchange between ROOT and Geant4 framework.



 if(verbose>1) gROOT->GetListOfBrowsers()->ls();
 if(verbose>1) gROOT->GetListOfGeometries()->ls();
 // TGeoManager* gGeoManager=(TGeoManager*)gROOT->GetListOfGeometries()->FindObject(MyGeoName); // search explicitely,  not really needed,    gGeoManager  is global


 */

#if !defined(__CINT__) && !defined(__CLING__)
#include "MyCppStdHeaders.H"
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TMarker.h>
#include <TBox.h>
#include <TArc.h>
#include <TGeoManager.h>

#include <TSeqCollection.h>
#include <TPluginManager.h>
#include <TVirtualGeoTrack.h>
#include <TGeoCompositeShape.h>

#include <TEveManager.h>
#include <TEveGeoNode.h>
#include <TGLViewer.h>
#include <TGLLightSet.h>
#include <TGeoUniformMagField.h>
#include <TGeoGlobalMagField.h>
#include <TFile.h>

#endif

#include <climits>

#include "MyGeoManager.h"

void CloseCanvas(TCanvas* c1)
{
  if(c1==NULL) // look for default canvas
  {
    gROOT->GetListOfCanvases()->ls();
    cout << "gROOT->GetListOfCanvases()->FindObject(c1)=" << gROOT->GetListOfCanvases()->FindObject("c1") << '\n';
    c1= (TCanvas*) gROOT->GetListOfCanvases()->FindObject("c1");
  }
  c1->Close();
}

void CheckFeatures()
{
  cout << "CheckFeatures() start" << '\n';
  gStyle->SetPalette(0);
  gStyle->SetCanvasPreferGL(kTRUE); // or kFALSE
  TCanvas *c1 = new TCanvas("glc1","Surfaces Drawing Options",200,10,700,900);
  c1->SetFillColor(42);
  cout << " UseGL()=" << c1->UseGL() << '\n';
  cout << " global Pointer gPluginMgr=" << gPluginMgr << '\n';
  gPluginMgr->Print();
  cout << "after gPluginMgr->Print() before gPluginMgr->Print(a)" << '\n';
  gPluginMgr->Print("a");
  cout << "CheckFeatures() end" << '\n';
#ifndef __CLING__
  cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << '\n';
  cout << "running CheckFeatures in line mode. When done quit root from the menu bar / File " << '\n';
#endif
}

void PlotAxesAtOrigin() // show coordinate system
{
  if(gPad->GetViewer3D()==NULL)
  {
    cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " *** warning *** gPad->GetViewer3D()=" << gPad->GetViewer3D() << " no axes will be drawn" << '\n';
  }
  else
  {
    TGLViewer *v = (TGLViewer *)gPad->GetViewer3D();
    v->GetClipSet()->SetClipType(TGLClip::kClipNone);  // defined EType  kClipNone, kClipPlane, kClipBox  see /Users/hbu/root_git_build/include/TGLClip.h  http://root.cern.ch/root/html/TGLClip.html   http://root.cern.ch/root/html/src/TGLClip.cxx.html      can also switch  in ogl display
    // with  kClipNone   axes at volumes
    // with  kClipPlane  axes at origin
    // with  kClipBox    axes at volumes
    v->ColorSet().Background().SetColor(kWhite);
    // v->SetGuideState(TGLUtil::kAxesEdge, kTRUE, kFALSE, 0); // at edge
    v->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0); // https://root.cern.ch/root/html/TGLViewer.html  draw axes at origin
  }
}

TVirtualGeoTrack* MinimalGeom(bool ShowEveWindow) // try to mix Geo drawing and Eve, not managed to get TVirtualGeoTrack visible in Eve
{
  // can also just paste next to CINT      after    cd /tmp/hbu ; $ROOTSYS/bin/root         or   ~/ROOT/5.34.14/x86_64-mac108-clang42-opt/bin/root
  // or    cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C; ln -sf ~/c/MyRoot/MyRootConst.h ; root
  // TGeoManager("world", "the simplest geometry"); // seems to set anyway the global gGeoManager


  TGeoMaterial *mat = new TGeoMaterial("Vacuum",0,0,0);
  TGeoMedium   *med = new   TGeoMedium("Vacuum",1,mat);
  TGeoVolume* top = gGeoManager->MakeTube("Top",med,2,3,10); //  http://root.cern.ch/root/html/TGeoTube.html
  gGeoManager->SetTopVolume(top);
  gStyle->SetCanvasPreferGL(kTRUE); // prefer GL
  // top->Draw();

  // now add a track
  Int_t PdgCode=2212;
  Int_t itrk=0;
  TObject *particle=NULL;
  Int_t iPrimary = gGeoManager->AddTrack(itrk,PdgCode,particle);
  TVirtualGeoTrack *myTrack = gGeoManager->GetTrack(iPrimary);
  myTrack->AddPoint(0,0,-11,0);
  myTrack->AddPoint(1,1, 11,0);
  //myTrack->Draw();  //---------------- already at this point all useful there, may switch to   view / editor
  // myTrack->Print();
  //top->SetVisRaytrace(true); // switches to more solid view    --- similar to OpenGL  --   real OpenGl much faster and shining / shadowing .
  gGeoManager->SetCurrentTrack(myTrack); // no effect ?

  if(ShowEveWindow) // Now eve,  based on gGeoManager
  {
    TEveManager::Create(); // needed, otherwise segmentation violation  in TEveGeoTopNode       shows Eve Window  -- nice but does not show track
    TGeoNode* node = gGeoManager->GetTopNode();
    TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
    en->SetVisLevel(4);
    en->GetNode()->GetVolume()->SetVisibility(kFALSE);
    gEve->AddGlobalElement(en);
    gEve->Redraw3D(kTRUE); // shows Eve Window  -- nice but does not show track,    use   e to turn black background off
  }

  gEve->Redraw3D(kTRUE);

  return myTrack;
}

void my_geo_current_pos_dir_Print(unsigned int verbose)
{
  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " start line " << setw(4) << __LINE__ << " gGeoManager=" << gGeoManager << '\n';
  if(gGeoManager)
  {
    const double* pos = gGeoManager->GetCurrentPoint();
    const double* dir = gGeoManager->GetCurrentDirection();
    cout << "current pos=(" << setw(8) << pos[0] << "," << setw(8) << pos[1] << "," << setw(8) << pos[2] << ")" << '\n';
    cout << "current dir=(" << setw(8) << dir[0] << "," << setw(8) << dir[1] << "," << setw(8) << dir[2] << ")" << '\n';
  }
}

void my_print_field(TVirtualMagField* thefield)
{
  if(thefield)
  {
    double x[3],B_vol[3]={0,0,0};
    thefield->Field(x,B_vol);
    if(thefield) cout << " field=" << B_vol[0] << ", "  << B_vol[1] << ", "  << B_vol[2] << " kGauss  TVirtualMagField" << '\n';
    cout << '\n';
  }
}

void ReDefine_Volume_Field(TGeoVolume *cvol) // using fieldm from par array
{
  TGeoMedium   *cmed = cvol->GetMedium();
  double par[20];
  for(unsigned int i=0;i<8;++i) par[i]=cmed->GetParam(i);
  cout << "par="; for(unsigned int i=0;i<8;++i) cout << par[i] << ", ";
  if(par[1]>0) cout << " magnetic field on ifield=par[1]=" << par[1] << " fieldm=par[2]=" << par[2] << " kGauss" << '\n';
  TGeoUniformMagField* theField=new TGeoUniformMagField(0,0,par[2]); // use here fieldm  to set Bz
  cvol->SetField(theField);  // set field in volume
}

void  my_geo_current_node_vol_Print(unsigned int verbose)
{
  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " start line " << setw(4) << __LINE__ << " gGeoManager=" << gGeoManager << '\n';
  if(gGeoManager)
  {
    const char   *path = gGeoManager->GetPath();           cout << "Current     path is: " << path << '\n';
    TGeoNode    *cnode = gGeoManager->GetCurrentNode();    cout << "Current     node is: " << cnode->GetName() << '\n'; if(verbose>2) cnode->Dump();
    TGeoVolume   *cvol = gGeoManager->GetCurrentVolume();  cout << "Current   volume is: " << cvol->GetName()  << '\n'; if(verbose>2) cvol->Dump();
    TGeoMedium   *cmed = cvol->GetMedium();                cout << "Current   medium is: " << cmed->GetName()  << '\n';
    TGeoMaterial *cmat = cmed->GetMaterial();              cout << "Current material is: " << cmat->GetName()  << '\n';

    ReDefine_Volume_Field(cvol);
    TGeoUniformMagField* medMagField=(TGeoUniformMagField*) cvol->GetField();
    cout << "after ReDefine_Volume_Field  reading back from volume"; my_print_field(medMagField);
    int istate = gGeoManager->GetCurrentNodeId(); cout << " istate=" << istate << '\n';
  }
}

void my_geo_track(unsigned int verbose)
{ // https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuideChapters/Geometry.pdf
  // https://root.cern.ch/root/htmldoc/guides/users-guide/Geometry.html
  // https://root.cern.ch/root/htmldoc/guides/users-guide/Geometry.html#navigation-and-tracking
  // by double [3]  vector pos for position and unit vector dir for direction
  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " start line " << setw(4) << __LINE__ << " gGeoManager=" << gGeoManager << '\n';
  if(gGeoManager)
  {
    // gGeoManager->AddNavigator(); // not actually needed https://root.cern.ch/root/html/TGeoNavigator.html    no more argument navig, see code   https://root.cern.ch/root/html/src/TGeoManager.cxx.html
    cout << "current pos before setting" << '\n'; my_geo_current_pos_dir_Print();

    // next for vis   see    // 1.5.8 Creating and Visualizing Tracks
    Int_t PdgCode=2212;
    Int_t itrk=0;
    // TObject *particle=NULL;
    Int_t iPrimary = gGeoManager->AddTrack(itrk,PdgCode,NULL);
    TVirtualGeoTrack *myTrack = gGeoManager->GetTrack(iPrimary);

    double pos[]{0.1,0.1,2},dir[]{0,sin(.1),cos(.1)};      // position and direction        inside
    // double pos[]{1.e9,1.e9,1.e9},dir[]{0,sin(.1),cos(.1)}; // position and direction        outside
    gGeoManager->InitTrack(pos,dir);  // https://root.cern.ch/root/html/TGeoManager.html
    cout << "current pos  after setting" << '\n'; my_geo_current_pos_dir_Print();

    myTrack->AddPoint(itrk, gGeoManager->GetCurrentPoint()[0], gGeoManager->GetCurrentPoint()[1], gGeoManager->GetCurrentPoint()[2]); // use current point for track

    // find current path, node, volume and material
    if(verbose>1) my_geo_current_node_vol_Print();

    if (gGeoManager->IsOutside())
    {
      cout << " current point is outside" << '\n';
      return;
    }

    if(verbose) cout << " current point is inside" << '\n';

    gGeoManager->FindNextBoundaryAndStep(); // updates current position to where hitting      ---  with or without field ??  -- seems ignores field       last : (0, 0.1, 1.40628) at tof=15.0192
    // seems to ginore filed,  no field seen in   https://root.cern.ch/root/html/src/TGeoNavigator.cxx.html
    cout << "after FindNextBoundaryAndStep" << '\n'; my_geo_current_pos_dir_Print();
    if(verbose>1) my_geo_current_node_vol_Print();

    myTrack->AddPoint(itrk, gGeoManager->GetCurrentPoint()[0], gGeoManager->GetCurrentPoint()[1], gGeoManager->GetCurrentPoint()[2]); // use current point
    myTrack->Print();

    gGeoManager->SetCurrentTrack(myTrack); // can then be used later, also with Eve

    //gGeoManager->GetTopVolume()->SetVisRaytrace(true); // solid view

    // myTrack->Draw();  // visible in canvas

    if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " end line " << setw(4) << __LINE__ << " gGeoManager=" << gGeoManager << '\n';
  }
}

void Show_RaceTrack()
{
  TGeoMaterial *mat = new TGeoMaterial("Vacuum",0,1,0);  // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMedium   *med = new TGeoMedium("Vacuum",1,mat);

  double g=0.04,s=0.01,r=0.03;

  TGeoVolume* top = gGeoManager->MakeBox("top",med,0.1,0.1,0.1); // not shown, size does not matter

  // 0.  Tube   like beam tube around IP
  double thick=0.002;
  double len=0.1;
  TGeoVolume* my_tube_segment = gGeoManager->MakeTubs("TubeSeg",med,r,r+thick,len/2,0,90); my_tube_segment->SetLineColor(kYellow);   //  http://root.cern.ch/root/html/TGeoTube.html  leave as base segment
  TGeoVolume*  RaceTrack = gGeoManager->MakeBox("RaceTrack",med,g+r,s+r,len/2); // mother volume for Racetrack, box that contains RaceTrack
  RaceTrack->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation(  g, s, 0), TGeoRotation("rot",0,0,  0)) );
  RaceTrack->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation( -g, s, 0), TGeoRotation("rot",0,0, 90)) );
  RaceTrack->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation( -g,-s, 0), TGeoRotation("rot",0,0,180)) );
  RaceTrack->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation(  g,-s, 0), TGeoRotation("rot",0,0,270)) );

  if(s>0)
  {
    TGeoVolume* sbox = gGeoManager->MakeBox("sbox",med,s,thick/2,len/2); sbox->SetLineColor(kRed);
    RaceTrack->AddNode(sbox,1,new TGeoCombiTrans( TGeoTranslation(  g+r+thick/2,  0, 0), TGeoRotation("rot",0,0,90)) );
    RaceTrack->AddNode(sbox,1,new TGeoCombiTrans( TGeoTranslation(-(g+r+thick/2), 0, 0), TGeoRotation("rot",0,0,90)) );
  }
  if(g>0)
  {
    TGeoVolume* gbox = gGeoManager->MakeBox("gbox",med,g,thick/2,len/2); gbox->SetLineColor(kBlue);
    RaceTrack->AddNode(gbox,1,new TGeoCombiTrans( TGeoTranslation(0,  s+r+thick/2,  0), TGeoRotation("rot",0,0,180)) );
    RaceTrack->AddNode(gbox,1,new TGeoCombiTrans( TGeoTranslation(0,-(s+r+thick/2), 0), TGeoRotation("rot",0,0,180)) );
  }

  top->AddNode(RaceTrack,1, new TGeoTranslation(0,0,0) );
  top->SetLineColor(kMagenta);
  // gGeoManager->SetTopVisible();   //   or not to make only inside visible

  top->Draw("ogl");
  PlotAxesAtOrigin();
}

void ShowShape()
{

  bool ShowCanvas=false;
  TCanvas *c1=NULL;
  unsigned int verbose=1;

  if(ShowCanvas)
  {
    gStyle->SetCanvasPreferGL(kTRUE); // prefer GL
    Int_t wtopx=800,wtopy=400; // hbu position on screen, a bit shifted to right   http://root.cern.ch/root/html/TCanvas.html#TCanvas:TCanvas@4    wtopy must be over 200 to see an effect, for x11 even more, 400 ok to make movable
    unsigned int kSizePt=700; // size in points   here used for both directions, to get square
    c1 = new TCanvas("mycanvas","PlotFrame",wtopx,wtopy,kSizePt,kSizePt); // kSizePt set in rootlogon.C
    // the default viewer is set by   ~/root_git/graf3d/gl/src/TGLSAViewer.cxx:   fFrame->SetWindowName("ROOT's GL viewer");
    //  TVirtualViewer3D *viewer3D = fCA->GetViewer3D("ogl");   see    test/RootShower/RootShower.cxx

    Double_t xlow,ylow,xup,yup;
    // xlow=0;ylow=0;xup=1;yup=1; // full page pad
    // TVirtualPad* pad=gPad; // also possible
    xlow=0;ylow=.5;xup=1;yup=1; // upper half pad
    TPad* pad1 = new TPad("pad1","pad1 title",xlow,ylow,xup,yup,kWhite); // upper half page white pad

    /*
     TVirtualPad* tvpad=gPad;
     cout << "before new TGLViewer  tvpad=" << tvpad << '\n';
     wtopx=000,wtopy=400;
     TGLViewer *tglv=new TGLViewer(tvpad,wtopx,wtopy,kSizePt,kSizePt);  // something wrong in arguments, link not found
     cout << "after new TGLViewer  tvpad=" << tvpad << '\n';
     */


    TGLViewer *tglv=(TGLViewer *)pad1->GetViewer3D(); //      ~/root_git_build/tutorials/gl/glViewerExercise.C   see  TGeoPainter.cxx
    // TGLViewer *tglv=(TGLViewer *)gPad->GetViewer3D(); //      ~/root_git_build/tutorials/gl/glViewerExercise.C   see  also     ~/root_git_build/tutorials/gl/nucleus.C    there also draw("ogl") used to set up viewer
    cout << "after new TGLViewer  tglv=" << tglv << '\n';

  }

  const double m_2_cm=100; // from m as used by MAD to cm used by root.

  /*
   z   rmin rmax

   tube
   cone

   */

  // open ~/temp/manuals/ROOTUsersGuide.pdf       http://root.cern.ch/root/html/tutorials/geom/rootgeom.C.html
  // TGeoManager("world", "the simplest geometry");  //  <------  do not use this, overwrites  gGeoManager
  TGeoMaterial *mat = new TGeoMaterial("Vacuum",0,1,0);  // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMedium   *med = new TGeoMedium("Vacuum",1,mat);

  // 0.  Tube   like beam tube around IP
  double thick=0.002;
  double fRmin0=0.0217,fRmax0=fRmin0+thick,fDZ0=1.; // min/max radius and half lenth   new CMS beam pipe central cylindrical part
  TGeoVolume* top = gGeoManager->MakeTube("Top",med,fRmin0*m_2_cm,fRmax0*m_2_cm,0.5*fDZ0); //  http://root.cern.ch/root/html/TGeoTube.html

  gGeoManager->SetTopVolume(top);
  // gGeoManager->CloseGeometry(); // seems not needed
  top->SetLineColor(kMagenta);
  gGeoManager->SetTopVisible();   //   or not to make only inside visible
  if(verbose) top->Print();
  cout << " top volume defines origin, fDZ0 =" << fDZ0 << " extending in z up to " << 0.5*fDZ0 << '\n';

  // 1. Cone1   from 0.5 to 1.5 m
  double zstart=0.5*fDZ0;
  double zend  = zstart+1;
  double fRmin1=fRmin0,fRmax1=fRmin0+thick,fRmin2=0.0273,fRmax2=fRmin2+thick,fDZ=zend-zstart; // min/max radius and half lenth      CMSPIPE6.R5
  TGeoVolume* myvol = gGeoManager->MakeCone("Cone1",med,0.5*fDZ,fRmin1*m_2_cm,fRmax1*m_2_cm,fRmin2*m_2_cm,fRmax2*m_2_cm); //  http://root.cern.ch/root/html/TCONE.html

  myvol->SetTransparency(50);

  top->AddNode(myvol,1, new TGeoTranslation(0,0,zstart+0.5*fDZ) );  // shift to end of tube0
  cout << " after Cone1 from zstart=" << zstart << " to zend=" << zend << endl;
  if(verbose) myvol->Print();

  // 2. Cone2  from 1.5 to 2.5 m
  zstart=zend;
  zend  = zstart+1;
  fRmin1=fRmin2;fRmax1=fRmin1+thick; // start from last
  fRmin2=0.029 ;fRmax2=fRmin2+thick;fDZ=zend-zstart;
  myvol = gGeoManager->MakeCone("Cone2",med,0.5*fDZ,fRmin1*m_2_cm,fRmax1*m_2_cm,fRmin2*m_2_cm,fRmax2*m_2_cm); //  http://root.cern.ch/root/html/TCONE.html
  top->AddNode(myvol,1,new TGeoTranslation(0,0,zstart+0.5*fDZ)); // shift to end of tube1
  cout << " after Cone2 from zstart=" << zstart << " to zend=" << zend << endl;
  if(verbose) myvol->Print();

  // 3. elliptical tube
  gGeoManager->MakeEltu("ELTU",med, 30,10,40);
  zstart=zend;
  zend  = zstart+1;
  fRmin1=fRmin2;fRmax1=fRmin1+thick; // start from last
  fRmin2=0.029 ;fRmax2=fRmin2+thick;fDZ=zend-zstart;
  myvol = gGeoManager->MakeEltu("ELTU",med,4,2,0.5*fDZ); // http://root.cern.ch/root/html/TGeoManager.html   http://root.cern.ch/root/html/TGeoEltu.html
  myvol->SetLineColor(kRed);
  top->AddNode(myvol,1,new TGeoTranslation(0,0,zstart+0.5*fDZ)); // shift to end of tube1);
  cout << " after ELTU from zstart=" << zstart << " to zend=" << zend << endl;
  if(verbose) myvol->Print();

  // 4. RECTELLIPSE beam-screen  VSSL.1R1.A.B1 at 22.554408\,m from IP with apertures
  // 0.0192, 0.0241, 0.0241, 0.0241  for  half width, half height (of rectangle), horizontal half axis, vertical half axis (of ellipse).
  zstart=zend;
  fDZ=2.;
  zend  = zstart+fDZ;
  double dx=1.92,dy=2.41;
  gGeoManager->MakeBox("box1",med,dx    ,dy,     0.5*fDZ);
  gGeoManager->MakeBox("box2",med,1.01*dx,1.01*dy,0.499*fDZ);  // only visible if length smaller of larger Box
  gGeoManager->MakeTube("tubeBS",med,2.41,2.51,0.5*fDZ);

  // manual 18.3 shapes  union +, intersection *  and subtraction -     http://root.cern.ch/root/html/TGeoCompositeShape.html
  // TGeoVolume *comp = new TGeoVolume("COMP",new TGeoCompositeShape("cs","box2-box1"));  // not so bad
  //  make sure to use unique names !!!
  TGeoVolume *comp = new TGeoVolume("COMP",new TGeoCompositeShape("cs","(box2-box1)+tubeBS"),med);  // not so bad
  comp->SetLineColor(kBlue);

  top->AddNode(comp,1,new TGeoTranslation(0,0,zstart+0.5*fDZ));
  cout << " after top->AddNode(box2,1,tr4) from zstart=" << zstart << " to zend=" << zend << endl;

  top->Draw("ogl");  // or just call draw with ogl    track not visible       shows both ROOT's GL viewer and c1 Canvas
  PlotAxesAtOrigin();

  if(ShowCanvas)
  {

    // check on ogl and plugins
    // cout << "gROOT->GetListOfCanvases()->FindObject(c1)=" << gROOT->GetListOfCanvases()->FindObject("c1") << '\n';
    if(c1) cout << "c1->UseGL()="<< c1->UseGL() << '\n';
    else if (gROOT->GetListOfCanvases()->FindObject("c1")) cout << " UseGL()=" << ((TCanvas*)gROOT->GetListOfCanvases()->FindObject("c1"))->UseGL() << '\n'; // check if ogl on   c1->UseGL() --- here still off
    gPluginMgr->Print();
  }

  // gGeoManager->Export("ShowShapeGeom.C");  // export the whole geometry

  // CloseCanvas();
}

void my_Pri_ObjArray(const char* ObjType, const TObjArray* ObjList, unsigned int verbose)  // https://root.cern.ch/root/html/TGeoManager.html   https://root.cern.ch/root/html/TObjArray.html  https://root.cern.ch/root/html/TGeoNodeMatrix.html
{
  if(ObjList)
  {
    if(ObjList->First())
    {
      cout << ObjType << " first is " << setw(15) << ObjList->First()->GetName() << " title=" << ObjList->First()->GetTitle();
      cout << " #objects=" << ObjList->GetEntries(); // https://root.cern.ch/root/html/TObjArray.html
      if(ObjList->GetEntries()>1)
      {
        if(ObjList->Last()) cout << " last is " << ObjList->Last()->GetName();
        unsigned int i=0;
        if(verbose) // loop through list
        {
          cout << " full list:" << '\n';
          TObject* itr=ObjList->First();
          while(itr)
          {
            ++i;
            if(itr->GetName()) cout << setw(2) << i << ": " << left << setw(15) << itr->GetName() << " title=" << itr->GetTitle() << '\n';
            itr=ObjList->After(itr);
          }
        }
      }
      else cout << '\n';
    }
    else cout << ObjType << " is empty";
  }
  else cout << "has no " << ObjType << '\n';
  cout << '\n';
}

#include "TGeoTube.h"
#include "TEveGeoShape.h"
#include "TEveGeoShapeExtract.h"


void my_pri_tlist(TList* tl,unsigned int verbose)
{
  if(!tl) { cout << "*** warning *** " << __FUNCTION__ << " tl is 0" << '\n'; return; }
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << " TList is " << tl->IsA() << '\n';
  TIter next(tl);
  unsigned int i=0;
  TObject* fi;
  while( ( fi=next() ) )
  {
    ++i;
    cout << setw(2) << i << " is "; fi->Print();
  }
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << " done with my_pri_tlist" << '\n';
}

#include <TKey.h>

TEveGeoShape* egse_from_file(const char* fname,unsigned int verbose)
{
  TFile* geom_file =  TFile::Open(fname); // read back
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';
  geom_file->ls(); //   KEY: TEveGeoShapeExtract	Top;1

  TIter next(gDirectory->GetListOfKeys());
  TKey* key;
  TString xxx("TEveGeoShapeExtract");
  TEveGeoShapeExtract* gse;
  TEveGeoShape* gs;
  while ((key = (TKey*) next()))
  {
    if (xxx == key->GetClassName())
    {
      gse = (TEveGeoShapeExtract*) key->ReadObj();
      gs = TEveGeoShape::ImportShapeExtract(gse, 0);
      // gEve->AddGlobalElement(gs); // trial
    }
  }
  return gs;
}

TEveGeoShape* my_Shape_to_EveShape(unsigned int verbose) //----  not needed any more --  better translate with   SaveExtract
{ // cd $ROOTSYS/src/graf3d/eve/src
  // $ROOTSYS/tutorials/eve/pythia_display.C   /Users/hbu/root_git/tutorials/eve/pythia_display.C
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';

  TList* test_list= new TList( gGeoManager->GetListOfShapes() ); // this does not do the job  --- so copy myself
  cout << " test, new list from GetListOfShapes" << '\n';
  my_pri_tlist(test_list,verbose);

  // TList* esd_objs= new TList(); // gGeoManager->GetListOfShapes() ); // no first -- make recursive ?

  TObjArray* ObjList=gGeoManager->GetListOfShapes();
  unsigned int i=1;
  TObject* itr=ObjList->First();

  // TEveGeoShape* myresults=new TEveGeoShape();

  TGeoShape* geo_shape=(TGeoShape*) itr;

  // TEveGeoShape* eve_geo_shape=(TEveGeoShape*) itr; // results in segmentation violation

  TEveGeoShape* eve_geo_shape = new TEveGeoShape(geo_shape->GetName(),geo_shape->GetTitle());
  eve_geo_shape->SetShape(geo_shape);

  // TEveGeoShape* myresults=eve_geo_shape; // RhoZ view visible, but segmentation violation when clicking on

  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';
  const char* fname= (string(geo_shape->GetName())+".root").c_str();
  eve_geo_shape->SaveExtract(fname,geo_shape->GetName()); // works

  TEveGeoShape* egse=egse_from_file(fname,verbose); // read back from file
  TEveGeoShape* myresults=egse; // actually helps

  // TEveGeoShapeExtract* egse = new TEveGeoShapeExtract(eve_geo_shape->GetName(),eve_geo_shape->GetTitle());

  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';

  cout << " first_shape name=" << eve_geo_shape->GetName() << " title=" << eve_geo_shape->GetTitle() << '\n';


  // esd_objs->AddFirst( eve_geo_shape );
  if(itr->GetName()) cout << " adding " << setw(2) << i << ": " << setw(15) << itr->GetName() << " title=" << itr->GetTitle()<< '\n';
  while(itr)
  {
    ++i;
    itr=ObjList->After(itr);
    if(itr)
    {
      geo_shape=(TGeoShape*) itr;

      TEveGeoShape* eve_geo_shape = new TEveGeoShape(geo_shape->GetName(),geo_shape->GetTitle());
      eve_geo_shape->SetShape(geo_shape);
      myresults->AddElement( eve_geo_shape ); // this helped a bit -- childs now there

      const char* fname= (string(geo_shape->GetName())+".root").c_str();
      eve_geo_shape->SaveExtract( fname,geo_shape->GetName()); // works
      TEveGeoShape* egse=new TEveGeoShape();
      egse=egse_from_file(fname,verbose); // read back from file
      // esd_objs->Add( eve_geo_shape );
      // myresults->AddElement( egse ); //RhoZ view shows something

      //if(itr->GetName()) cout << " adding itr           " << setw(2) << i << ": " << setw(15) << itr->GetName() << " title=" << itr->GetTitle()<< '\n';
      //if(itr->GetName()) cout << " adding geo_shape     " << setw(2) << i << ": " << setw(15) << geo_shape->GetName() << " title=" << geo_shape->GetTitle()<< '\n';
      if(itr->GetName()) cout << " adding eve_geo_shape " << setw(2) << i << ": " << setw(15) << eve_geo_shape->GetName() << " title=" << eve_geo_shape->GetTitle()<< '\n';
    }
  }
  // my_pri_tlist(esd_objs,verbose);

  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';

  //egse->SetElements(esd_objs); // is this right ??
  //if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';

  //TEveGeoShape* gentle_geom = TEveGeoShape::ImportShapeExtract(egse, 0); // segmentation violation
  //if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';
  // if(1>0) return gentle_geom;
  // return egse;
  // return eve_geo_shape;
  return myresults;
}

void pri_node_daughters(TGeoNode* node,unsigned int verbose)
{
  unsigned int nd = node->GetNdaughters();
  if(verbose>1)cout << __FUNCTION__ << " line=" << __LINE__ << " nd=" << nd << '\n';

  for (unsigned int i = 0; i < nd; ++i)
  {
    // next hbu
    TGeoNode* daughter=node->GetDaughter(i);
    if(daughter) cout << "   daughter(" << setw(2) << i << ") is " << setw(10) << daughter->GetName() ;
    if(daughter->GetNdaughters()) cout << " has further " << daughter->GetNdaughters() << " daughters -- could make recursice call";
    cout << '\n';
  }
}

void my_Geom_Diagnostics(unsigned int verbose) // use also  new TBrowser;   for inspection      http://root.cern.ch/root/html/TGeoManager.html
{
  if(verbose) cout << __FUNCTION__ << " start " << __FILE__ << " line " << __LINE__ << " gGeoManager=" << gGeoManager << endl;
  if(!gGeoManager) { cout << "*** warning *** " << __FUNCTION__ << " no gGeoManager, will do nothing" << '\n'; return; }
  gGeoManager->CheckGeometry();  // see    void TGeoManager::CheckGeometry(Option_t * /*option*/)

  TList* MatList=gGeoManager->GetListOfMaterials(); // http://root.cern.ch/root/html/TList.html
  if(MatList)
  {
    cout << "ListOfMaterials:"<< '\n';
    TIter next (gGeoManager->GetListOfMaterials());
    TGeoMaterial *mat; // http://root.cern.ch/root/html/TGeoMaterial.html
    while ((mat = (TGeoMaterial*)next()))
    {
      mat->Print();
    }
  }
  else if(verbose) cout << "has no ListOfMaterials" << '\n';

  TGeoNode* topnode = gGeoManager->GetTopNode();
  if(topnode)
  {
    if(verbose) cout << "gGeoManager has topnode name=" << topnode->GetName() << '\n';
    if(verbose) pri_node_daughters(topnode,verbose);
  }
  else if(verbose) cout << "has no topnode" << '\n';

  TGeoMedium *dummy = TGeoVolume::DummyMedium();
  TIter nextv(gGeoManager->GetListOfVolumes());
  TGeoVolume *vol;
  unsigned int nvol=0;
  while ((vol = (TGeoVolume*)nextv()))
  {
    if(verbose && nvol==0) cout << "List of volumes:" << '\n';
    ++nvol;
    if (vol->IsAssembly()) vol->GetShape()->ComputeBBox();
    else if (vol->GetMedium() == dummy)
    {
      cout << "my_Geom_Diagnostics Warning Volume" << vol->GetName() << " has no medium: assigned dummy medium and material" << '\n';
      vol->SetMedium(dummy);
    }
    if(verbose)
    {
      cout << nvol << setw(2) << ": " << vol->GetName() << '\n';
      if(verbose>1) vol->Dump();
    }
  }
  if(verbose) cout << " nvol=" << nvol << '\n';

  TObjArray* GShapesList=gGeoManager->GetListOfGShapes();
  my_Pri_ObjArray("GShapesList",GShapesList,verbose);

  TObjArray* ShapesList=gGeoManager->GetListOfShapes();
  my_Pri_ObjArray("ShapesList",ShapesList,verbose);

  TObjArray* NodeList=gGeoManager->GetListOfNodes();
  my_Pri_ObjArray("NodeList",NodeList,verbose);
}

void ShowShapes2()  // minimal subset of  ~/ROOT/5.34.14/x86_64-mac108-clang42-opt/tutorials/geom/building.C
{

  // Materials
  TGeoMaterial *Vacuum = new TGeoMaterial("Vacuum",0,1,0);  // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMaterial *Fe     = new TGeoMaterial("Fe",55.845,26,7.87);

  // Media
  TGeoMedium *Vac  = new TGeoMedium("Vac",0,Vacuum);
  TGeoMedium *Iron = new TGeoMedium("Iron",0,Fe);

  // 1. top Volume  ---  Box of Vacuum  -- not visible
  double Dummy=1.e6; // not shown, size does not really matter
  TGeoVolume *my_TopVol = gGeoManager->MakeBox("Top",Vac,Dummy,Dummy,Dummy);  // "top"
  gGeoManager->SetTopVolume(my_TopVol);
  gGeoManager->SetTopVisible(0); // 0 not visible       for a test can set to 1

  TGeoVolume *my_Vols;

  // 2. red tube segment
  my_Vols = gGeoManager->MakeTubs("tube_seg1",Iron,10,20, 5, 0,10); // http://root.cern.ch/root/html/TGeoManager.html   rmin,rmax, dz,  0 to 10 degrees
  my_Vols->SetLineColor(kRed);
  my_TopVol->AddNodeOverlap(my_Vols,1,new TGeoTranslation(0,0,0)); // http://root.cern.ch/root/html/TGeoVolume.html    AddNodeOverlap allows for overlaps
  // my_TopVol->AddNode(my_Vols,1,new TGeoTranslation(0,0,0)); // http://root.cern.ch/root/html/TGeoVolume.html   just AddNode   --   result looks the same


  // 3. blue larger tube segment
  double dz=2;
  my_Vols = gGeoManager->MakeTubs("tube_seg2",Iron,10,20, dz, 0,110); // rmin,rmax, dz,  0 to 110 degrees
  my_Vols->SetLineColor(kBlue); my_Vols->SetTransparency(50); // 0 - 100      material property   so will apply to all iron, 0  is no transparency  and 100 completely transparent, here not effec ?
  my_TopVol->AddNode(my_Vols,1,new TGeoTranslation(0,0,3));


  // 4. green elliptic tube    subtract larger - smaller to  get elliptic beam pipe
  gGeoManager->MakeEltu("el1",Iron,12,5,1      );
  gGeoManager->MakeEltu("el2",Iron,10,4,1.00001); // increase dz otherwise subtraction not visible
  TGeoVolume *comp = new TGeoVolume("COMP",new TGeoCompositeShape("cs","el1-el2"));
  comp->SetMedium(Iron); // seems the medium information is not transmitted, so set explicitely here
  comp->SetLineColor(kGreen);
  my_TopVol->AddNode(comp,1,new TGeoTranslation(0,0,6));

  // 5. construct a RECTELLIPSE or rather RECTCIRCLE  beam screen, yellow          see also  Plot_BeamScreen_2d
  const double deg=180/3.141592653589793;
  double x0=2.41, y0=1.92, r=2.41, thick=0.2, phi=0, l=0;
  // double x0=1.92, y0=2.41, r=2.41, thick=0.2, phi=0, l=0;
  double amax=r; // should be the maximum of the three numbers
  TGeoVolume *beamscreen = gGeoManager->MakeBox("beamscreen",Vac,amax,amax,dz); // block containing the whole beamscreen
  if(x0<r) { phi=acos(x0/r); l=r*sin(phi); }
  if(y0<r) { phi=asin(y0/r); l=r*cos(phi); }
  double xt=r*cos(phi);
  double yt=r*sin(phi);
  TGeoVolume *box,*arc;
  box =  gGeoManager->MakeBox("box",Iron,l,thick/2,dz); box->SetLineColor(kYellow);
  double phi2=1.1*phi; // increase to get overlap -- looks better, but is that ok for geant ?
  if(x0<r) // rotated beamscreen
  {
    arc = gGeoManager->MakeTubs("arctop",Iron,r,r+thick, dz,deg*phi2,180-deg*phi2); arc->SetLineColor(kYellow); // arc top
    beamscreen->AddNode(arc,1);                                 // arc top
    beamscreen->AddNode(arc,1,new TGeoRotation("rot",0,0,180)); // arc bottom
    beamscreen->AddNode(box,1,new TGeoCombiTrans( TGeoTranslation(  xt+thick/2 ,0,0), TGeoRotation("rot",0,0,90)) );  // box left
    beamscreen->AddNode(box,1,new TGeoCombiTrans( TGeoTranslation(-(xt+thick/2),0,0), TGeoRotation("rot",0,0,90)) );  // box right
  }
  else // standard flat beamscreen
  {
    arc =  gGeoManager->MakeTubs("arcright",Iron,r,r+thick, dz, -deg*phi2, deg*phi2); arc->SetLineColor(kYellow);
    beamscreen->AddNode(arc,1);                                 // arc right
    beamscreen->AddNode(arc,1,new TGeoRotation("rot",0,0,180)); // arc left
    beamscreen->AddNode(box,1,new TGeoTranslation(0,  yt+thick/2,0));   // box top
    beamscreen->AddNode(box,1,new TGeoTranslation(0,-(yt+thick/2),0));  // box bottem
  }
  my_TopVol->AddNode(beamscreen,1,new TGeoTranslation(0,0,8));
  //my_TopVol->AddNode(beamscreen,1, new TGeoCombiTrans(*trans,*rot));

  my_TopVol->Draw("ogl");

  // show axes  http://root.cern.ch/download/doc/ROOTUsersGuideChapters/Graphics.pdf

  PlotAxesAtOrigin();
}

void ShowTorus() // torus http://root.cern.ch/root/htmldoc/guides/users-guide/Geometry.html#torus-tgeotorus-class
{

  // Materials
  TGeoMaterial *Vacuum = new TGeoMaterial("Vacuum",0,1,0);  // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMaterial *Fe     = new TGeoMaterial("Fe",55.845,26,7.87);

  // Media
  TGeoMedium *Vac  = new TGeoMedium("Vac",0,Vacuum);
  TGeoMedium *Iron = new TGeoMedium("Iron",0,Fe);

  // 1. top Volume  ---  Box of Vacuum  -- not visible
  double Dummy=1.e6; // not shown, size does not really matter
  TGeoVolume *my_TopVol = gGeoManager->MakeBox("Top",Vac,Dummy,Dummy,Dummy);  // "top"
  gGeoManager->SetTopVolume(my_TopVol);
  gGeoManager->SetTopVisible(0); // 0 not visible       for a test can set to 1

  TGeoVolume *my_Vols;

  // 2. torus
  Double_t R=10;    // axial radius
  Double_t Rmin=4; // inner radius
  Double_t Rmax=6; // outer radius
  Double_t Phi1=0; // starting phi angle
  Double_t Dphi=90;// phi range
  my_Vols = gGeoManager->MakeTorus("torus_1",Iron,R,Rmin,Rmax,Phi1,Dphi); // http://root.cern.ch/root/html/TGeoManager.html   rmin,rmax, dz,  0 to 10 degrees
  my_Vols->SetLineColor(kRed);
  my_TopVol->AddNodeOverlap(my_Vols,1,new TGeoTranslation(0,0,0)); // http://root.cern.ch/root/html/TGeoVolume.html    AddNodeOverlap allows for overlaps
  my_TopVol->Draw("ogl");

  PlotAxesAtOrigin();
}

#include "TGeoPcon.h"
void ShowPolycone()
// https://root.cern.ch/root/htmldoc/guides/users-guide/Geometry.html#polycone-tgeopcon-class
// https://root.cern.ch/root/html/TGeoManager.html#makePcon       $ROOTSYS/tutorials/geom/geodemo.C
{
  // Materials
  TGeoMaterial *Vacuum = new TGeoMaterial("Vacuum",0,1,0);  // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMaterial *Fe     = new TGeoMaterial("Fe",55.845,26,7.87);

  TGeoMedium *Vac  = new TGeoMedium("Vac", 0,Vacuum);
  TGeoMedium *Iron = new TGeoMedium("Iron",0,Fe);

  // 1. top Volume  ---  Box of Vacuum  -- not visible
  double Dummy=1.e6; // not shown, size does not really matter
  TGeoVolume *my_TopVol = gGeoManager->MakeBox("Top",Vac,Dummy,Dummy,Dummy);  // "top"
  gGeoManager->SetTopVolume(my_TopVol);
  gGeoManager->SetTopVisible(0); // 0 not visible       for a test can set to 1

  TGeoVolume *my_Vols;

  // 2. polycone
  Double_t phi=-30;
  Double_t dphi=90;
  Int_t nz=4;
  my_Vols = gGeoManager->MakePcon("pcon_1",Iron,phi,dphi,nz);
  TGeoPcon *pcon = (TGeoPcon*)(my_Vols->GetShape());
  pcon->DefineSection(0, 0, 5,20); // i, z, rmin, rmax
  pcon->DefineSection(1,20,15,20);
  pcon->DefineSection(2,25,15,25);
  pcon->DefineSection(3,50,15,20);

  my_Vols->SetLineColor(kRed);
  my_TopVol->AddNodeOverlap(my_Vols,1,new TGeoTranslation(0,0,0)); // http://root.cern.ch/root/html/TGeoVolume.html    AddNodeOverlap allows for overlaps
  my_TopVol->Draw("ogl");

  PlotAxesAtOrigin();
}

#include "TGeoPgon.h"
void ShowPolygone()
// https://root.cern.ch/root/htmldoc/guides/users-guide/Geometry.html#polycone-tgeopgon-class
// https://root.cern.ch/root/html/TGeoManager.html#makePgon       $ROOTSYS/tutorials/geom/geodemo.C
{
  // Materials
  TGeoMaterial *Vacuum = new TGeoMaterial("Vacuum",0,1,0);  // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMaterial *Fe     = new TGeoMaterial("Fe",55.845,26,7.87);

  // Media
  TGeoMedium *Vac  = new TGeoMedium("Vac",0,Vacuum);
  TGeoMedium *Iron = new TGeoMedium("Iron",0,Fe);

  // 1. top Volume  ---  Box of Vacuum  -- not visible
  double Dummy=1.e6; // not shown, size does not really matter
  TGeoVolume *my_TopVol = gGeoManager->MakeBox("Top",Vac,Dummy,Dummy,Dummy);  // "top"
  gGeoManager->SetTopVolume(my_TopVol);
  gGeoManager->SetTopVisible(0); // 0 not visible       for a test can set to 1

  TGeoVolume *my_Vols;

  // 2. polygone
  Double_t phi=-30;
  Double_t dphi=90;
  Int_t nedges=4;
  Int_t nz=4;
  my_Vols = gGeoManager->MakePgon("pgon_1",Iron,phi,dphi,nedges,nz);
  TGeoPgon *pgon = (TGeoPgon*)(my_Vols->GetShape());
  pgon->DefineSection(0,-50,45, 50); // i, z, rmin, rmax
  pgon->DefineSection(1,  0,35, 40);
  pgon->DefineSection(2, 10,30, 35);
  pgon->DefineSection(3, 50,90,100);

  my_Vols->SetLineColor(kRed);
  my_TopVol->AddNodeOverlap(my_Vols,1,new TGeoTranslation(0,0,0)); // http://root.cern.ch/root/html/TGeoVolume.html    AddNodeOverlap allows for overlaps
  my_TopVol->Draw("ogl");

  PlotAxesAtOrigin();
}

TGeoCombiTrans Position_by_z_and_theta(double z,double theta,unsigned int verbose)
{
  double v[3]={0,0,0},r[3]; // 3 vectors, v as "LOCAL" volume center and r "MASTER"  after transformation  "r=T*v"
  v[2]=z;
  TGeoRotation rot;
  rot.RotateY(-theta); // set the rotation matrix
  rot.LocalToMasterVect(v,r); // r = rot * v
  TGeoCombiTrans ct;
  ct.SetTranslation(r);
  ct.SetRotation(rot);
  if(verbose)
  {
    cout << "Position_by_z_and_theta z=" << z  << " theta=" << theta << '\n'
    << " Local position v=(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << '\n'
    << " after rotation r=(" << r[0] << ", " << r[1] << ", " << r[2] << ")" << '\n'
    << "Transformation local to master ct="; ct.Print();
  }
  return ct;
}

void ShowBelle(bool HER,bool right,unsigned int verbose) // inspired by Belle II  SuperKEKB  https://indico.cern.ch/event/47961/session/9/contribution/25/attachments/956118/1356921/SuperKEKB.pdf
{ // very simple approximate beam pipe     http://belle2.kek.jp      http://indico.ific.uv.es/indico/getFile.py/access?contribId=545&sessionId=32&resId=0&materialId=slides&confId=2025   software lib Belle II
  // open ~/temp/HL-LHC/KEK_2014/For_Burkhardt-san.pdf      reading very roughly from picture
  // LER e+ 4 GeV
  // HER e- 7 GeV
  // 1.5 T solenoid   -100 to +200 cm
  // 83 mrad full crossing angle
  // beam pipe geometry looks rather L/R symmetric

  double const deg=180./M_PI;
  double halfangle=-0.0415; // - 41.5 mrad

  // Materials
  TGeoMaterial *matVac = new TGeoMaterial("Vac",0,1,0);       // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length
  TGeoMaterial *matBe  = new TGeoMaterial("Be",9.0121831,4,1.85);  // https://en.wikipedia.org/wiki/Beryllium A,Z,rho

  int numed=0;  // user defined medium index,  if 0  same as mother

  // extra parameter array of dimension 20, only 8 (0-7) used, exported in .C,  .root ?, .xml,  not exported in gdml,   geant3 style          fParams in root in medium
  double par[]{0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0}; //    http://en.cppreference.com/w/cpp/language/aggregate_initialization
  par[0]  = 0.000000;   // isvol
  par[1]  = 3.000000;   // ifield  ifield = 0 if no magnetic field; ifield = -1 if user decision in guswim;  ifield = 1 if tracking performed with g3rkuta; ifield = 2 if tracking  performed with g3helix; ifield = 3 if tracking performed with g3helx3.
  par[2]  = 15.000000;  // fieldm  kGauss  1.5 Tesla
  par[3]  = 1.000000;   // tmaxfd
  par[4]  = 1.000000;   // stemax
  par[5]  = 0.100000;   // deemax
  par[6]  = 0.100000;   // epsil
  par[7]  = 0.100000;   // stmin

  double Bx=0,By=0,Bz=1.23456; // unit kGauss ?
  TGeoUniformMagField* theField=new TGeoUniformMagField(Bx,By,Bz);
  // can be made global with   TGeoGlobalMagField::Instance()->Field(x,B)    x,B double[3],   can be position dependend
  TGeoGlobalMagField::Instance()->SetField( theField ); // make also global

  // Media without field   --- params vector = 0
  // TGeoMedium(const char* name, Int_t numed, const TGeoMaterial* mat, Double_t* params = 0)
  TGeoMedium *Vac  = new TGeoMedium("Vac",numed,matVac,par); ++numed;
  TGeoMedium *Be   = new TGeoMedium("Be" ,numed,matBe, par);

  const double m_2_cm=100; // from m as used by MAD to cm used by root.

  // take always the same beam pipe. Just change the angle
  double theta;
  string TopVolName;
  if(right)
  {
    if(HER)
    {
      TopVolName="HER_r";
      theta=halfangle;
    }
    else
    {
      TopVolName="LER_r";
      theta=-halfangle;
    }
  }
  else // left
  {
    if(HER)
    {
      TopVolName="HER_l";
      theta=M_PI+halfangle;
    }
    else
    {
      TopVolName="LER_l";
      theta=M_PI-halfangle;
    }
  }
  if(verbose) cout << "TopVolName=" << TopVolName << " theta=" << TopVolName << '\n';

  // 0. top Volume  ---  Box of Vacuum  -- not visible
  double Dummy=10*m_2_cm; // not shown, size does not really matter --  except when tracking
  TGeoVolume *top = gGeoManager->MakeBox(TopVolName.c_str(),Vac,Dummy,Dummy,Dummy);
  gGeoManager->SetTopVolume(top);
  gGeoManager->SetTopVisible(0); // 0 not visible       for a test can set to 1
  if(verbose) top->Print();

  // 1.  Tube   like beam tube around IP
  const char* Name1="Tube1";
  double zstart=0;
  double zend=1.1;
  double thick=0.002;
  double fRmin1=0.015,fRmax1=fRmin1+thick,fDZ=zend-zstart;
  TGeoVolume* CurVol = gGeoManager->MakeTube(Name1,Be ,fRmin1*m_2_cm,fRmax1*m_2_cm,0.5*fDZ*m_2_cm); //  http://root.cern.ch/root/html/TGeoTube.html
  CurVol->SetLineColor(kRed);  CurVol->SetTransparency(80);
  CurVol->SetField(theField); // sets fField in Volume
  double z=(zstart+0.5*fDZ); // middle
  top->AddNode(CurVol,1,new TGeoCombiTrans( Position_by_z_and_theta(z*m_2_cm,theta*deg,verbose) ));

  // 2. Cone to larger tube
  const char* Name2="Cone2";
  zstart=zend;
  zend  = zstart+0.5;
  double fRmin2=0.020,fRmax2=fRmin2+thick; fDZ=zend-zstart; // min/max radius and half lenth
  CurVol = gGeoManager->MakeCone(Name2,Be,0.5*fDZ*m_2_cm,fRmin1*m_2_cm,fRmax1*m_2_cm,fRmin2*m_2_cm,fRmax2*m_2_cm); //  http://root.cern.ch/root/html/TCONE.html
  CurVol->SetLineColor(kGreen);  CurVol->SetTransparency(80);
  CurVol->SetField(theField);
  z=(zstart+0.5*fDZ); // middle
  top->AddNode(CurVol,1,new TGeoCombiTrans( Position_by_z_and_theta(z*m_2_cm,theta*deg,verbose) ));
  cout << " after " << Name2 << " from zstart=" << zstart << " to zend=" << zend << '\n';
  if(verbose) CurVol->Print();

  // 3.  Tube   like beam tube around IP
  const char* Name3="Tube3";
  zstart=zend;
  zend  = zstart+0.5;
  double fRmin3=fRmin2,fRmax3=fRmin3+thick; fDZ=zend-zstart;
  CurVol = gGeoManager->MakeTube(Name3,Be,fRmin3*m_2_cm,fRmax3*m_2_cm,0.5*fDZ*m_2_cm); //  http://root.cern.ch/root/html/TGeoTube.html
  CurVol->SetLineColor(kMagenta);  CurVol->SetTransparency(80);
  CurVol->SetField(theField);
  z=(zstart+0.5*fDZ); // middle
  top->AddNode(CurVol,1,new TGeoCombiTrans( Position_by_z_and_theta(z*m_2_cm,theta*deg,verbose) ));
  cout << " after " << Name3 << " from zstart=" << zstart << " to zend=" << zend << '\n';
  if(verbose) CurVol->Print();

  top->SetField(theField);
  top->Draw("ogl");
  PlotAxesAtOrigin();
}

void PriFieldVec(const double FieldVec[3])
{
  cout << "PriFieldVec "
  << " (Bx, By, Bz)=" << "(" << FieldVec[0]  << ", "  << FieldVec[1] << ", "  << FieldVec[2] << ")"
  << " kGauss" << '\n';
}

void SynRadGeo() // simple geometry, for synchrotron radiation test with geant4,
// The Box is 500 m   of Vacuum   as TestEm16   ~/mygeant4/TestEm16/src/DetectorConstruction.cc
//
// magnetic field, in root just stored in fField, not directly used, not exported
//----------------
// drawing can be done with  THelix  https://root.cern.ch/root/html/THelix.html
// cd $ROOTSYS ; find . -type f -exec grep -H "fField" {} \;
// cd $ROOTSYS/tutorials ; find . -type f -exec grep -H "field" {} \;
// cd $ROOTSYS ; find . -type f -name "*.cxx" -exec grep -iH "field" {} \;
// see   $ROOTSYS/tutorials/eve/track.C
//
//
// cd $G4INSTALL ; find . -type f -exec grep -H "GDMLProcessor" {} \;        never called directly,  seems init when using  G4GDMLParser fParser;  fParser.Read(fReadFile);
// cd $G4INSTALL ; find . -type f -exec grep -H "G4GDMLWrite" {} \;
//  ~/root_git_build/test/RootShower/MyEvent.cxx
//  ~/root_git_build/test/RootShower/RootShower.cxx       http://root.bnl.gov/QtRoot/RootShower/README     by  Bertrand Bellenot   ftp://root.cern.ch/root/doc/20TutorialsandTests.pdf
//  cd $ROOTSYS/test/RootShower
//  rm -rf "/tmp/$LOGNAME/Release"* ; mkdir /tmp/$LOGNAME/Release ; cd /tmp/$LOGNAME/Release ; cmake $ROOTSYS/test ; make RootShower -j5    ; not working
{
  // if(gGeoManager==NULL)
  TGeoManager* gGeoManager=new TGeoManager("World","SynRadGeo"); // create new Geometry
  cout << "SynRadGeo start gGeoManager=" << gGeoManager << '\n';
  double a, z, density;
  const double meter=100; // meter units in cm
  TGeoMaterial *Vacuum  = new TGeoMaterial("Vacuum", a=14.01, z=7., density= 1.e-6);  //  density= 1.e-10  becomes 0.000000 in gdml
  // Vacuum->SetTemperature(30.);    // test  http://root.cern.ch/root/html/TGeoMaterial.html
  // Vacuum->SetPressure(1.e-3);     // test  http://root.cern.ch/root/html/TGeoMaterial.html
  Vacuum->Print();

  //TGeoElementTable *table      = gGeoManager->GetElementTable();
  //TGeoElement *elem_nitrogen   = table->GetElement(z=7.);
  //TGeoMaterial *Vacuum = new TGeoMaterial("Vacuum",elem_nitrogen,density= 1.516784e-11);
  //TGeoMaterial *Fe     = new TGeoMaterial("Fe",55.845,26,7.87);

  if(Vacuum) cout << "Vacuum defined" << '\n';
  int numed;
  TGeoMedium   *VacMed = new TGeoMedium("Vacuum",numed=1,Vacuum);
  if(VacMed) cout << "VacMed defined" << '\n';

  VacMed->SetParam(1,1.1); // Test, set param[1] ifield=1.1
  VacMed->SetParam(2,2.2); // Test, set param[2] fieldm=2.2

  // 1. top Volume  ---  Box of Vacuum
  double BoxLen=500*meter/2; // 500m box, give half length
  // TGeoVolume *my_TopVol = gGeoManager->MakeBox("myBox",VacMed,BoxLen,BoxLen,BoxLen); //ok but same name myBox for solid and volume

  // define the top volume in two steps with different names for solid and volume, for gdml import in geant4
  TGeoBBox      *my_box = new TGeoBBox("myBox_Solid",BoxLen,BoxLen,BoxLen);  // in gdml solid name
  TGeoVolume *my_TopVol = new TGeoVolume("myBox_Vol",my_box,VacMed);         // in gdml volume and world name

  bool FieldTests = true; //  better use Eve, see   myEve.C

  if(FieldTests)
  {
    double Bx=0,By=0,Bz=1; // unit here kGauss ?

    { // some definining/reading/printing TGeoGlobalMagField  https://root.cern.ch/root/html/TGeoGlobalMagField.html https://root.cern.ch/root/html/src/TGeoGlobalMagField.cxx.html
      // seems not much used  -- except by ALICE  who define a derived class with field map
      // http://aliroot-docs.web.cern.ch/aliroot-docs/macros/ITSQArecoparam.C.html  http://personalpages.to.infn.it/~puccio/htmldoc/src/AliMUONTrackExtrap.cxx.html

      cout << "global field before any setting" << '\n'
      << "TGeoGlobalMagField::Instance()->IsLocked()=" << TGeoGlobalMagField::Instance()->IsLocked() << '\n'
      << "TGeoGlobalMagField::Instance()   =" << TGeoGlobalMagField::Instance() << '\n'
      << "TGeoGlobalMagField::GetInstance()=" << TGeoGlobalMagField::GetInstance() << '\n'
      << "TGeoGlobalMagField::Instance()->GetField()=" << TGeoGlobalMagField::Instance()->GetField() << '\n';
      ;

      //--   test global field  (derived from uniform field,   could also be own field map derived from TVirtualMagField )
      const double x[3] = {50.,50.,50.}; // position
      double B[3]  = {0.,0.,1.};

      cout << "line=" << __LINE__ << '\n';

      TGeoGlobalMagField::Instance()->SetField( new TGeoUniformMagField(B[0],B[1],B[2])  );  // set the global field from a uniform field, works
      cout << "line=" << __LINE__ << " TGeoGlobalMagField::Instance()->GetField()=" << TGeoGlobalMagField::Instance()->GetField() << '\n';

      // could define myself field map derived from TVirtualMagField

      TVirtualMagField* theMagField=TGeoGlobalMagField::Instance()->GetField();
      cout << "line=" << __LINE__ << " theMagField=" << theMagField << '\n';
      double B_global[3]={0,0,0};
      if(theMagField) // read back the global field
      {
        TGeoGlobalMagField::Instance()->Field(x,B_global);  // read back using the derived Field class,  since derived here from  TGeoUniformMagField   x dummy
        cout << "from TGeoGlobalMagField B_global=" << B_global[0] << ", "  << B_global[1] << ", "  << B_global[2] << '\n'; // works, global field was read
      }

      TGeoGlobalMagField::Instance()->SetField(0); // turn off
      cout << "Now TGeoGlobalMagField::Instance()->GetField()=" << TGeoGlobalMagField::Instance()->GetField() << '\n';
      //   TGeoGlobalMagField::Instance()->Field(x,B_global);    now does not fail, does nothing, B_global remains
    }

    // some trivial definining/reading/printing TGeoUniformMagField
    TGeoUniformMagField* theField=new TGeoUniformMagField(Bx,By,Bz);
    if(theField) cout << "theField defined as TGeoUniformMagField with "
      << " (Bx, By, Bz)=" << "(" << Bx  << ", "  << By << ", "  << Bz << ")"
      << " kGauss" << '\n'; // defined, but so far not used and not exported
    // Double_t FieldVec[3];
    const Double_t *FieldVecPtr=theField->GetFieldValue();
    cout << "using &FieldVecPtr[0] " ; PriFieldVec(&FieldVecPtr[0]);
    cout << "FieldVecPtr[2]=" << FieldVecPtr[2] << '\n';
    Double_t FieldVec[3]{FieldVecPtr[0],FieldVecPtr[1],FieldVecPtr[2]};  // http://en.cppreference.com/w/cpp/language/list_initialization
    cout << "using FieldVec "; PriFieldVec(FieldVec);
    my_TopVol->SetField(theField); // sets root internal fField
  }

  gGeoManager->SetTopVolume(my_TopVol); // define as top volume
  gGeoManager->SetTopVisible(1); // 0 not visible       for a test can set to 1

  cout << "SynRadGeo BoxLen=" << BoxLen << '\n';

  my_TopVol->Draw("ogl");
}
