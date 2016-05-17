/*  ~/c/MDISim/myEve.C
 for code on root, without need   MyClib Ntuple.h  extension

 TEve powerful but also tricky to use ---  good idea is to start from examples
 Visulalization -   Event display  --   can show and calculate tracks with magnetic fields but no direct access to tracks  and tracking

 Overview of EVE – the event visualization environment of ROOT :
 open ~/temp/IOP/1742-6596_219_4_042055.pdf  ;   <-------------   by  Matevˇz Tadel

 or on web :  http://iopscience.iop.org/1742-6596/219/4/042055/pdf/1742-6596_219_4_042055.pdf

 
 // next as in ~/c/MyClib/Conversion.C - here directly so that no need to link to MyClib --- not so useful any more with .L ~/c/MyRoot/init.C  MDISim_init();
 #include <sys/stat.h>   // test issue MacBookProi7
 bool FileExists(const char *filename ) // returns 1 if file exists
 {
 if(filename==NULL) return false;
 struct stat stat_buffer;
 if ( stat( filename, &stat_buffer ) )
 return false ;  // not ok to get file attributes
 else
 return true ; // it was possible to get the file attributes, implies the files exists
 }


 */

#include <TEveManager.h>

//-- next done to not see errors in Xcode
#ifndef __CLING__
#include "MyCppStdHeaders.H"
#include <TGeoManager.h>
#include <TEveGeoNode.h>
#include <TGLViewer.h>
#include <TGLLightSet.h>
#include <TKey.h>
#include <TFile.h>
#include <TEveTrackProjected.h>

// #include "/Users/hbu/root_git/src/graf3d/gl/inc/TGLLightSet.h"
#endif

#include "myEve.h"

#ifdef __CLING__
  #include "MultiView.C" // CLING  needs MultiView  code  (not just header)
#else
 #include "MultiView.h"
#endif

#include "MyRoot.h"

void PlotGuidesAtOrigin() // show coordinate system
{
  if(gEve==NULL)
  {
    cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " *** warning *** gEve=" << gEve << " no Guides will be drawn" << '\n';
  }
  else
  {
    // manual 9.13.2.7 Guides,   show coordinate system     https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#guides
    TGLViewer *v = gEve->GetDefaultGLViewer();   // get access to TGLViewer, seen in geom_lhcb.C
    v->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0); // https://root.cern.ch/root/html/TGLViewer.html  draw axes at origin
    v->RefreshPadEditor(v);
  }
}

void show_extract(const char* fname, unsigned int verbose) // works when TEveGeoShapeExtract present fname can be file name or url like http://root.cern.ch/files/alice_ESDgeometry.root
{ // $ROOTSYS/tutorials/eve/show_extract.C
  if(!gEve) TEveManager::Create();
  TFile* geom_file =  TFile::Open(fname);
  if (!geom_file)  { cout << "*** Error *** in " << __FUNCTION__ << " could not open " << fname << '\n'; exit(3); }
  TIter next(gDirectory->GetListOfKeys());
  TKey* key;
  while ((key = (TKey*) next()))
  {
    if(verbose>1) cout << __FUNCTION__ << " " << fname << " key->GetClassName()=" << key->GetClassName() << " key->GetName()=" << key->GetName() << '\n';
    if(key->GetClassName() == TString("TEveGeoShapeExtract") ) // only does something if TEveGeoShapeExtract present
    {
      TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) key->ReadObj();
      TEveGeoShape* gs = TEveGeoShape::ImportShapeExtract(gse, 0);
      gEve->AddGlobalElement(gs);
    }
  }
  gEve->Redraw3D(kTRUE);
}

#include "TCLass.h"
#include "TEveScene.h" // defines  TEveSceneList
#include "MyGeoManager.h"

void my_Eve_GeoShape_Diagnostics( TEveGeoShape* theshape, unsigned int verbose) // use also  new TBrowser;   for inspection      http://root.cern.ch/root/html/TGeoManager.html
{
  if(verbose) cout << __FUNCTION__ << " start " << __FILE__ << " line " << __LINE__ << " gGeoManager=" << gGeoManager << endl;
  if(!gEve) { cout << "*** warning *** " << __FUNCTION__ << " no gEve, will do nothing" << '\n'; return; }
  if(!theshape) { cout << "*** warning *** " << __FUNCTION__ << " no shape" << '\n'; return; }
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';
  theshape->Print();
  cout << " theshape class is " << theshape->IsA()->GetName() << '\n';  // TEveGeoShape

  // TGListTree
   // TGListTreeItem* i = theshape->FirstChild();

  cout << " theshape->FirstChild()=" << theshape->FirstChild() << '\n'; // should be there
  if(theshape->FirstChild()) // TEveElement
  {
    cout << " theshape->FirstChild()->GetElementName()=" << theshape->FirstChild()->GetElementName() << '\n';
    cout << " theshape->FirstChild()->GetElementTitle()=" << theshape->FirstChild()->GetElementTitle() << '\n';
  }
  if(theshape->LastChild())
  {
    cout << " theshape->LastChild()->GetElementName()=" << theshape->LastChild()->GetElementName() << '\n';
    cout << " theshape->LastChild()->GetElementTitle()=" << theshape->LastChild()->GetElementTitle() << '\n';
  }

  /*

  cout << " theshape->GetNSegments()=" << theshape->GetNSegments() << '\n';
  static const TEveException eh("SplitGLView::ItemClicked "); // what does this ?   here just to give argument to GetObject
  cout << " theshape->GetObject(()=" << theshape->GetObject(eh) << '\n';
  cout << " theshape->GetNSegments()=" << theshape->GetNSegments() << '\n';    // 0
  cout << " theshape->GetChildClass()=" << theshape->GetChildClass() << '\n';  // no child
  cout << " theshape->GetShape()=" << theshape->GetShape() << '\n'; // 0

  static TGeoManager* gm=theshape->GetGeoMangeur(); // defined but all empty
  cout << " theshape->GetGeoMangeur()=" << theshape->GetGeoMangeur() << '\n';   gm->Print();
  cout << " gm->GetTopNode()=" << gm->GetTopNode() << '\n'; // 0

  TObjArray* GShapesList=gm->GetListOfGShapes();
  my_Pri_ObjArray("GShapesList",GShapesList,verbose);

  TObjArray* ShapesList=gm->GetListOfShapes();
  my_Pri_ObjArray("ShapesList",ShapesList,verbose);

  TObjArray* NodeList=gm->GetListOfNodes();
  my_Pri_ObjArray("NodeList",NodeList,verbose);

   */

}

void my_Eve_Diagnostics(unsigned int verbose) // use also  new TBrowser;   for inspection      http://root.cern.ch/root/html/TGeoManager.html
{
  if(verbose) cout << __FUNCTION__ << " start " << __FILE__ << " line " << __LINE__ << " gGeoManager=" << gGeoManager << endl;
  if(!gEve) { cout << "*** warning *** " << __FUNCTION__ << " no gEve, will do nothing" << '\n'; return; }
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';

  TEveSceneList* sl=gEve->GetScenes(); // class in
  if(sl)
  {
    cout << "SceneList is there" << '\n';
    cout << "SceneList is " << sl->IsA()->GetName() << '\n';
  }
  else cout << "no SceneList " << '\n';

  TEveScene* globscene=gEve->GetGlobalScene();
  if(globscene )
  {
    cout << "Global Scene is there" << '\n';
  }
  else cout << "no Global Scene " << '\n';

}

void my_default_view() // with white background
{
  if(!gEve) { cout << "**** warning *** my_default_view, no gEve, do nothing" << '\n'; return; }
  TGLViewer *v = gEve->GetDefaultGLViewer();   // get access to TGLViewer, seen in geom_lhcb.C
  if(!v) cout << "**** warning *** my_default_view, no v" << '\n';
  v->ColorSet().Background().SetColor(kWhite); // change the Background color, by default black, better use white        needs  libRGL   use -lRGL
  v->SetStyle(TGLRnrCtx::kFill);  // choice is kFill, kOutline, kWireFrame;   use default kFill
  TGLLightSet* lightset = v->GetLightSet(); lightset->SetLight(TGLLightSet::kLightBottom, kFALSE);  // by default light from all sides. Here turn off light from bottom. see also glViewerExercise.C, further changes can be done from the Browser
}

#include <TEveViewer.h>
MultiView* ShowProjections(TEveElement* el,unsigned int verbose)
{
  MultiView* gMultiView = new MultiView();
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << " gEve->GetCurrentEvent()=" <<gEve->GetCurrentEvent() << " el=" << el << '\n';
  if(gMultiView->f3DView) gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline); else cout << "no f3DView" << '\n';
  gMultiView->SetDepth(-10);
  gMultiView->ImportGeomRPhi(el);
  gMultiView->ImportGeomRhoZ(el);

  TEveEventManager* theevt = gEve->GetCurrentEvent();
  cout << "GetCurrentEvent=" << theevt << '\n';
  //gMultiView->ImportGeomRPhi(theevt);
  //gMultiView->ImportGeomRhoZ(theevt);

  gMultiView->SetDepth(0);
  if(verbose) cout << __FUNCTION__ << " line=" << __LINE__ << '\n';
  return gMultiView;
}

TEveElement* StartEveWithGeomDisplay(const char* fname, bool AllowForProjections, unsigned int verbose) // set up Event display.  Start to show Geometry. Later add tracks
// uses EVE - the Event Visualization Environment of ROOT
// TEveManager   not in ROOT manual, see instead  http://root.cern.ch/drupal/content/eve
// http://root.cern.ch/root/html/TEveManager.html  and
// file:///Users/hbu/root_git/graf3d/doc/v520/index.html  doc on OpenGL
// https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookFireworks
//    open   ~/temp/IOP/1742-6596_219_4_042055.pdf
// $ROOTSYS/tutorials/eve/csgdemo.C
// $ROOTSYS/tutorials/eve/pythia_display.C
{

  if(!fname) { cout << "*** Error *** in " << __FUNCTION__ << " fname not given" << '\n'; exit(1); }
  bool IsFile = !StartsWith(string(fname),"http://");
  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " IsFile=" << IsFile << '\n';
  if(IsFile && !FileExists(fname)) { cout << "*** Error *** in " << __FUNCTION__ << " " << fname << " does not exist" << '\n'; exit(2); }
  if(!IsFile)
  {
    // http location, better defined cache, as required for   http://mtadel.home.cern.ch/mtadel/root/alice_mini_geom.root"   https://root.cern.ch/root/html/TDCacheFile.html
    cout << "trying to get file from web via http. Define cache with TFile::SetCacheFileDir(\"/tmp/hbu\");" << '\n';
    TFile::SetCacheFileDir("/tmp/hbu");
  }

  string Fname=fname; // make it a string which can be nodifieds

  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << '\n';

  bool use_TGeoManager        = Has_Class(Fname,"TGeoManager",verbose); // Has_Class defined in ~/c/MyRoot/MyRoot.h, C
  bool use_TEveGeoShapeExtract= Has_Class(Fname,"TEveGeoShapeExtract",verbose);
  if(use_TGeoManager || use_TEveGeoShapeExtract)
  {
    if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << "use_TGeoManager=" << use_TGeoManager << " use_TEveGeoShapeExtract=" << use_TEveGeoShapeExtract << '\n';
  }
  else
  {
    cout << "*** Error *** in " << __FUNCTION__ << " " << Fname << " no TGeoManager and no TEveGeoShapeExtract" << '\n'; return nullptr;
  }

  if(AllowForProjections && !use_TEveGeoShapeExtract)
  {
    string fname2=WithoutExtension(Fname)+"_eve.root";
    if(verbose) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " AllowForProjections=" << AllowForProjections
      << " no TEveGeoShapeExtract in " << Fname << " will look for " << fname2 << " and if not existing create a temporary one"<< '\n';
    if(FileExists(fname2))
    {
      if(verbose) cout << fname2 << " exists ";
      use_TEveGeoShapeExtract= Has_Class(fname2,"TEveGeoShapeExtract",verbose);
      if(verbose)
      {
        if(use_TEveGeoShapeExtract) cout << " and has TEveGeoShapeExtract, will be used";
        else cout << " but has no TEveGeoShapeExtract";
        cout << '\n';
      }
    }
    if(use_TEveGeoShapeExtract) Fname=fname2;
    else // make new temporary file with TEveGeoShapeExtract
    {
      string Dir,LocalFname;
      GetDirFname(Fname,Dir,LocalFname);
      string outdir=string("/tmp/")+getenv("LOGNAME")+string("/"); // so that now outdir = /tmp/hbu/
      string fout=outdir+WithoutExtension(LocalFname)+"_eve.root";
      if(verbose) cout << "make new temporary " << fout << " with class TEveGeoShapeExtract" << '\n';
      // Make_GeoShapeExtract_version(Fname,fout,verbose); // makes file, and inits gEve without window    problem with global gEve
      string theAction=string("geo2shape ");
      if(verbose==1) theAction+="-v ";
      if(verbose>1)  theAction+="-V ";
      theAction+=string(Fname)+" "+fout;
      if(verbose==0) theAction+=string(" > /dev/null 2>&1"); // suppress output from geo2shape
      if(verbose) cout << theAction << '\n';
      int ierr=system(theAction.c_str()); // execute theAction
      if(ierr) cout << "****** " << theAction << " returns error=" << ierr << '\n';
      Fname=fout.c_str();
      use_TEveGeoShapeExtract= Has_Class(Fname,"TEveGeoShapeExtract",verbose);
    }
  }
  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " now use file with Fname=" << Fname << '\n';
  TFile* geom_file=nullptr;
  if(IsFile) geom_file = TFile::Open(Fname.c_str());
  else geom_file =  TFile::Open(Fname.c_str(),  "CACHEREAD");

  if (!geom_file)  { cout << "*** Error *** in " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " could not open " << Fname << '\n'; exit(3); }
  TEveElement* el=nullptr; // useful both for TGeoManager and TEveGeoShapeExtract

  if(!gEve) TEveManager::Create();

  if(use_TEveGeoShapeExtract) // if available use the TEveGeoShapeExtract version
  {
    if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " use_TEveGeoShapeExtract=" << use_TEveGeoShapeExtract << '\n';
    if(verbose>1) geom_file->ls(); // see what is in there

    TIter nextit(geom_file->GetListOfKeys());
    TKey* key=(TKey*) nextit(); // needed to automatically get the keyName, Assembly  if generated by Make_GeoShapeExtract_version
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom_file->Get(key->GetName());
    TEveGeoShape* gse_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    gEve->AddGlobalElement(gse_geom);
    el=gse_geom; // shape can be made into element, but not into node
  }
  else
  {
    if(verbose) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " use_TEveGeoShapeExtract=" << use_TEveGeoShapeExtract << '\n';
    gGeoManager = gEve->GetGeometry(Fname);  // set the global gGeoManager --- may fail
    if(!gGeoManager) { cout << "*** Error *** in " << __FUNCTION__ << " no gGeoManager" << '\n'; exit(3); }
    if(!gGeoManager->GetTopNode()) { cout << "*** Error *** in " << __FUNCTION__ << " no TopNode" << '\n'; exit(3); }

    TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, gGeoManager->GetTopNode());  // https://root.cern.ch/root/html/TEveGeoTopNode.html
    if(verbose>2)
    {
      cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " TEveGeoTopNode en: ";
      en->Dump();
      if(gGeoManager)
      {
        cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " TGeoManager geoman: ";
        gGeoManager->Dump();
        cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " TGeoManager PrintOverlaps: ";
        gGeoManager->PrintOverlaps(); cout << '\n';
      }
    }
    el=en;
  }

  gEve->AddGlobalElement(el);
  my_default_view();
  gEve->Redraw3D(kTRUE);
  return el;

  /* could get from global, better return here directly
  TEveElement* el;  TEveGeoTopNode* en;
  if(gGeoManager && gGeoManager->GetTopNode()) en= new TEveGeoTopNode(gGeoManager, gGeoManager->GetTopNode());
  TEveScene* sc = gEve->GetGlobalScene();
  cout << " en=" << en << " sc=" << sc << '\n';
  if(en) el=en; else if(sc) el=sc->FirstChild();
  cout << el->GetElementName() << '\n';
  */
}

//______________________________________________________________________________
// $ROOTSYS/tutorials/eve/track.C

#include <TEveTrack.h> // TEveTrackList
#include <TEveVSDStructs.h> // TEveRecTrackD
#include <TEveTrackPropagator.h> // TEveTrackPropagator     https://root.cern.ch/root/html/TDecompSVD.html
#include <TSystem.h> // gSystem

TEveTrack* my_make_track(TEveTrackPropagator* prop, bool LER,double pz,unsigned int verbose) // starts a new  TEveTrack  at vertex 0,0,0   with momentum vactor
{
  // Make track with given propagator.
  // Add to math-marks to test fit.

  double halfangle=-0.0415; // - 41.5 mrad
  int sign=1; // +1, LER

  TEveRecTrackD rc; // https://root.cern.ch/root/html/TEveRecTrackT_double_.html   template for reconstructed track
  rc.fV.Set(0,0,0);  // set vertex

  if(LER)
  {
    // double Eb=4; // 4 GeV
    pz*=-1; // left
    double px=sign*halfangle*pz;
    rc.fP.Set(px,0,pz);      // set momentum   LER goes left
    // rc.fP.Set(0,pz,0);  // beding radius check
  }
  else
  {
    sign=-1;
    halfangle=-0.0415; // 41.5 mrad
    double px=sign*halfangle*pz;
    rc.fP.Set(px,0,pz);      // set momentum   HER goes right
    // rc.fP.Set(0,pz,0);  // beding radius check
  }
  rc.fSign = sign;
  TEveTrack* track = new TEveTrack(&rc, prop);
  track->SetName(Form("p %f Charge %d", pz, sign));

  if(verbose)
  {
    //cout << "rc.Print() "; rc.Print();          // just  OBJ: TEveRecTrackT<double>
    //cout << "track->Print() "; track->Print();  // just  track->Print() TPolyMarker3D N=0, Option=
    cout << "track->PrintPathMarks() "; track->PrintPathMarks();
  } //hbu

  /*
   // daughter 0
   TEvePathMarkD pm1(TEvePathMarkD::kDaughter); // https://root.cern.ch/root/html/TEvePathMarkT_double_.html
   pm1.fV.Set(0.01, -0.02, .03); // daughter 0 vertex
   pm1.fP.Set(0.1,0,0.1); // daughter momentum
   track->AddPathMark(pm1); // AddPathMark
   // daughter 1
   TEvePathMarkD pm2(TEvePathMarkD::kDaughter);
   pm2.fV.Set(-0.01,  0.02, .03); // daughter 1 vertex
   pm2.fP.Set(-0.1,0,-0.1); // daughter momentum
   track->AddPathMark(pm2);
   if(verbose)
   {
   cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " after  daughter definition" << '\n';  track->PrintPathMarks();
   } //hbu
   */

  return track;
}

#include "MyGeoManager.h" // for my_geo_track only
#include <TVirtualGeoTrack.h>

void add_trks(TEveTrackPropagator* prop,TEveTrack* track,unsigned int verbose)
{
  if(verbose && 1>2)  // just test, creating by hand, or from gGeoManager tracks
  {
    if(gGeoManager) my_geo_track(verbose);
    TVirtualGeoTrack* myTrack=gGeoManager->GetCurrentTrack();

    if(myTrack)
    {
      cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " current track already defined on gGeoManager level" << '\n';
      myTrack->Print();

      TEveLine* line = new TEveLine("mytrack1");
      line->SetMainColor(kBlack);
      const double* pos0=myTrack->GetFirstPoint();
      const double* pos1=myTrack->GetLastPoint();
      cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " pos0=" << pos0[0] << ", " << pos0[1] << ", " << pos0[2] << '\n';
      cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " pos1=" << pos1[0] << ", " << pos1[1] << ", " << pos1[2] << '\n';
      line->SetNextPoint(pos0[0],pos0[1],pos0[2]);
      line->SetNextPoint(pos1[0],pos1[1],pos1[2]);
      line->SaveAs("mytrack1"); // save the line
      gEve->AddElement(line); // mytrack1 visible in Eve        production vertex : (0, 0.1, 0.1)   last : (0, 0.1, 1.40628)
    }
    else cout << " no track defined on gGeoManager level" << '\n';

    // at start point -1
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " track length=" <<  prop->GetTrackLength() << " GetCurrentPoint=" << prop->GetCurrentPoint() << '\n'; // https://root.cern.ch/root/html/src/TEveTrackPropagator.html
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " curVertex:" ; TEveVectorD curVertex=track->GetVertex();   //curVertex.Dump();
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " curMoment:" ; TEveVectorD curMoment=track->GetMomentum(); //curMoment.Dump();
    // cout << "list->Print: "; list->Print("all");
    // TEveTrackList https://root.cern.ch/root/html/TEveTrackList.html   https://root.cern.ch/root/html/src/TEveTrackList.cxx.html  descending to  https://root.cern.ch/root/html/TNamed.html
    // not https://root.cern.ch/root/html/TEventList.html   http://root.cern.ch/root/html/src/TEventList.cxx.html
    double x,y,z;
    x=0;y=0;z=0;
    bool NewTrk=true;
    if(NewTrk)
    {
      // prop->ResetTrack(); // ?  had no effect
      TEveVectorD part_v(0,0,0); // vertex
      TEveVectorD part_p(0,0.1,.1); // https://root.cern.ch/root/html/TEveVectorD.html
      prop->GoToVertex(part_v,part_p);
      int iFirstPoint=prop->GetCurrentPoint();
      cout << "before  GoToBounds part_p:" ; part_p.Dump(); cout << " part_v:" ; part_v.Dump();
      cout << "iFirstPoint=" << iFirstPoint
      << " ElementName=" << prop->GetElementName()
      << " ElementTitle=" << prop->GetElementTitle()
      << '\n'; // TEveTrackPropagator=TEveTrackPropagator ElementTitle=""
      prop->GoToBounds(part_p);  // calls Update, LoopToBounds..  -- chnges part_p   -- and position ?
      // use  https://root.cern.ch/root/html/TEvePointSet.html ?
      int iLastPoint=prop->GetCurrentPoint();
      cout << "before  GoToBounds part_p:" ; part_p.Dump();
      cout << " part_v:" ; part_v.Dump();
      cout << " iLastPoint=" << iLastPoint
      << " ElementName=" << prop->GetElementName()
      << " ElementTitle=" << prop->GetElementTitle()
      << " Name=" << prop->GetName()
      << " Title=" << prop->GetTitle()
      << '\n'; // TEveTrackPropagator=TEveTrackPropagator ElementTitle=""        use   TEveTrackList     list->AddElement(track);

      for(int i=iFirstPoint;i<iLastPoint;++i)
      {
        /// how to access helix ??
      }
      // prop->GetProjTrackBreaking();
      //  prop->GetTrackLength()   loops over all track points in   vector<TEveVector4D> fPoints  and sums up the length


      cout << "after   GoToBounds part_p:" ; part_p.Dump(); cout << " part_v:" ; part_v.Dump(); // vector changes   and length now non zero  !
    }
    // else prop->GoToBounds(curMoment); // goes to bounds of picture -- not of geometry
    // LER helix        length=190.18  GetCurrentPoint=10
    // LER runge kutta  length=198.244 GetCurrentPoint=17
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " track length=" <<  prop->GetTrackLength() << " GetCurrentPoint=" << prop->GetCurrentPoint() << '\n'; // https://root.cern.ch/root/html/src/TEveTrackPropagator.html
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " curVertex:" ; curVertex=track->GetVertex();   //curVertex.Dump();
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " curMoment:" ; curMoment=track->GetMomentum(); //curMoment.Dump();
    cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " PrintPathMarks " << '\n';  track->PrintPathMarks();
  }

}

void MyEveFieldTest(bool isRungeKutta,bool LER,double zmax) // after this  gEve->GetCurrentEvent() defined
{ // https://root.cern.ch/download/doc/ROOTUsersGuide.html   https://root.cern.ch/download/doc/ROOTUsersGuide.html#creating-and-visualizing-tracks
  // TEveTrackPropagator* g_prop = 0;  // global
  // cd ~/root_git/src/graf3d/eve/src ; grep TVirtualGeoTrack *.cxx       no TVirtualGeoTrack in eve
  // gGeoManager  only used in   TEveGeoNode.cxx, TEveGeoShape.cxx, TEveManager.cxx, TEveUtil.cxx

  bool NewFrame=true;
  unsigned int verbose=2;

  if(gEve)
  {
    if(verbose) cout << __FILE__ << " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " gEve=" << " TEveManager was already initialized"
      << " global gGeoManager=" << gGeoManager << '\n';
    NewFrame=false;
  }
  else TEveManager::Create();

  TEveTrackList *trk_list = new TEveTrackList();
  // TEveTrackPropagator* prop = g_prop = trk_list->GetPropagator();
  TEveTrackPropagator* prop = trk_list->GetPropagator();

  // prop->SetFitDaughters(false);  // true, false, no difference seen
  prop->SetMaxZ(zmax);

  if (isRungeKutta)
  {
    prop->SetStepper(TEveTrackPropagator::kRungeKutta);
    trk_list->SetName("RK Propagator");
  }
  else
  {
    trk_list->SetName("Helix Propagator");
  }

  if (isRungeKutta) trk_list->SetLineColor(kMagenta);
  else trk_list->SetLineColor(kCyan);

  // constant B field, const angle
  double Bfield = 1.5;  // Tesla Belle II soleonoid
  prop->SetMagFieldObj(new TEveMagFieldConst(0., 0., Bfield) ); // https://root.cern.ch/root/html/TEveMagFieldConst.html   https://root.cern.ch/root/html/src/TEveTrackPropagator.cxx.html https://root.cern.ch/phpBB3/viewtopic.php?f=10&t=14072 units cm,Gev/c and Tesla (checked beding radius)
  trk_list->SetElementName(Form("%s, constB", trk_list->GetElementName()));

  for(double pz=0.2;pz<1.2;pz+=0.2)
  {
    TEveTrack* track = my_make_track(prop, LER,pz,verbose);
    if(verbose) cout << "track make for pz=" << pz << " index=" << track->GetIndex() << '\n';
    track->SetLineColor(trk_list->GetLineColor());
    track->MakeTrack(); // https://root.cern.ch/root/html/TEveTrack.html  https://root.cern.ch/root/html/src/TEveTrack.cxx.html   needed to make track visible

    if(verbose)
    {
      cout << "after MakeTrack" << '\n';
      cout << "track->Print() ";          track->Print();
      cout << "track->PrintPathMarks() "; track->PrintPathMarks();
    } //hbu

    trk_list->AddElement(track);
  }
  gEve->AddElement(trk_list);

  // try to find when  cross a boundary       https://root.cern.ch/download/doc/ROOTUsersGuide.html#creating-and-visualizing-tracks  ??
  // TEveTrackPropagator GoToBounds     https://root.cern.ch/root/html/TEveTrackPropagator.html   https://root.cern.ch/root/html/src/TEveTrackPropagator.cxx.html
  // void GoToBounds(TEveVectorD& p)  //

  // add_trks(prop,track,verbose=0);


  if(NewFrame)
  {
    TGLViewer  *gv = gEve->GetDefaultGLViewer(); // https://root.cern.ch/root/html/TEveManager.html   https://root.cern.ch/root/html/TEveViewer.html
    gv->ColorSet().Background().SetColor(kWhite); // change the Background color, by default black, better use white
  }

  gEve->Redraw3D(kTRUE);
  // gSystem->ProcessEvents(); // https://root.cern.ch/root/html/TSystem.html

}

// 2D projections, TEveProjection
// cd $ROOTSYS/tutorials/eve/ ; $ROOTSYS/bin/root
// .x MultiView.C         // expects already an Eve window, otherwise segmentation violation, could provide one with  .x view3ds.C
// .x SplitGLView.C       //  error: no member named 'Update' in 'TFormula'
//
/*
 cd $ROOTSYS/tutorials/eve/ ; $ROOTSYS/bin/root
 .L alice_esd_split.C
 .L MultiView.C
 alice_esd_split()
 MultiView()
 */
//
// clean up
// cd $ROOTSYS/tutorials/eve/; rm -f *.d MultiView_C.d MultiView_C.so MultiView_C_ACLiC_dict_rdict.pcm ; rm -rf $ROOTSYS/tutorials/eve/aliesd $ROOTSYS/tutorials/eve/files


/*

 rm -rf "/tmp/$LOGNAME/"* ; cd /tmp/$LOGNAME/; root
 .L ~/c/MDISim/myEve.C
 unsigned int verbose=2;TEveElement* el;
 // el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Shape.root");
    el=StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/Shape.root",AllowForProjections,verbose);
 // el=StartEveWithGeomDisplay("/Users/hbu/Downloads/alice_mini_geom.root",AllowForProjections,verbose);
 // el=StartEveWithGeomDisplay("http://mtadel.home.cern.ch/mtadel/root/alice_mini_geom.root",AllowForProjections,verbose);
 // el=StartEveWithGeomDisplay("/Users/hbu/www/Geom/Shapes2_eve.root");
 PlotGuidesAtOrigin();
 //---   wait a bit,   move cursor,   sleep(1);   not enough
 ShowProjections(el,verbose); // wait a bit before this command, otherwise segmentation violation     sleep(1)





 Look at
 https://root.cern.ch/root/html/tutorials/eve/projection.C.html       Demonstates usage of automatic 2D projections
 cd $ROOTSYS/tutorials/eve ; root
 .x projection.C

 modify for me
 cp -pf $ROOTSYS/tutorials/eve/projection.C ~/Downloads/

  rm -rf "/tmp/$LOGNAME/"* ; cd /tmp/$LOGNAME/; root
  .x ~/Downloads/projection.C      // looks for gentle which does not exist,  try for my shape  --- or make gentle



 //-------  translate from gGeoManager to TEveGeoShapeExtract by writing file ---   can also be done with my geo2shape  tool,  ~/c/MyRoot/main_geo2shape.C
 cd /tmp/$LOGNAME/; rm -f "/tmp/$LOGNAME/"*; root
 TEveManager::Create();
 gGeoManager = gEve->GetGeometry("/Users/hbu/www/Geom/Shapes2.root"); // ok
 TGeoNode* node = gGeoManager->GetTopNode();
 TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
 en->SetVisLevel(4);
 en->GetNode()->GetVolume()->SetVisibility(kFALSE);
 gEve->AddGlobalElement(en);
 //gEve->Redraw3D(kTRUE);

 en->ExpandIntoListTreesRecursively();
 en->SaveExtract("Shapes2_eve.root","Assembly",false); // this is now   TEveGeoShapeExtract  logical name node !!
 .q


 //-------  translate also track files
 ls -l *.out ; grep "Tesla" MDISim.out ;   echo make sure track files there
 cd /tmp/$LOGNAME/; root
 .L ~/c/MDISim/MyGeoManager.C
 .L ~/c/MDISim/myGraph3D.C
 .L ~/c/MDISim/myEve.C
 TEveManager::Create();
 gGeoManager = gEve->GetGeometry("/Users/hbu/www/Geom/Belle_LER_l.root");
 TGeoNode* node = gGeoManager->GetTopNode();
 TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
 en->SetVisLevel(4);
 en->GetNode()->GetVolume()->SetVisibility(kFALSE);
 gEve->AddGlobalElement(en);
 gEve->Redraw3D(kTRUE);
 const double zmin=-2,zmax=2,Scale_xy=1;
 verbose=2;PlotTracksInFiles("/tmp/hbu/Helix*.out","",zmin,zmax,kGreen,Scale_xy,verbose);

 */
