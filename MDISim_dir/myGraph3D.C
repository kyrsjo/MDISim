/*  ~/c/MDISim/myGraph3D.C

 work on cones,  idea is to add synrad parameters from PrintBends, PrintQuads  to ntuple       which  has also survey and divergence
 cone part from bending angles and divergence also applies to protons

 */

#ifndef __CLING__
#include "MyCppStdHeaders.H"
#include <TROOT.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGeoManager.h>
#include <TEveManager.h>
#include <TEveGeoNode.h>
#include <TVirtualGeoTrack.h>
#include <TEveLine.h>
#include <TEveStraightLineSet.h>
#include <TEveBoxSet.h>
#include <TGLFontManager.h>
#include <TEveText.h>
#endif

#include "Lorentz.h"
#include "Twiss_util.h" // Beam
#include "MyRoot.h"     // to_TEveVector
#include "myEve.h"      // PlotGuidesAtOrigin
#include "Mat3x3.h"
#include "myGraph3D.h"
#include "FileInfo.h"   // ListOfFilesMatching

void MyTrack3D(const Ntuple& nt,Int_t PdgCode ,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,unsigned int verbose) // fname like    /Users/hbu/mad/lhc/twiss_track_ref/002_px_0b1.tfs          event display in canvas,   did not manage to get this in OpenGl with Geometry ---   better got to Eve, see below
{
  bool goto_CM_units=true;
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " nt.Noent()=" << nt.Noent() << " Scale_xy=" << Scale_xy << '\n';

  if(gGeoManager==NULL)
  {
    cout << "No geometry defined to attach tracking" << '\n';
    return;
  }

  unsigned int n=nt.Noent();

  double xt[n],yt[n],zt[n],st[n];

  valarray<double> xvec=nt.GetVar("X");
  valarray<double> yvec=nt.GetVar("Y");
  // valarray<double> zvec=nt.GetVar("Z");
  valarray<double> svec=nt.GetVar("S");

  for(unsigned int i=0; i<n; ++i) st[i]=svec[i];
  for(unsigned int i=0; i<n; ++i) xt[i]=xvec[i];
  for(unsigned int i=0; i<n; ++i) yt[i]=yvec[i];

  for(unsigned int i=0; i<n; ++i) zt[i]=st[i];  // zvec[i];     // for the moment use s instead of z

  if(goto_CM_units)
  {
    for(unsigned int i=0; i<n; ++i) xt[i]*=100;
    for(unsigned int i=0; i<n; ++i) yt[i]*=100;
    for(unsigned int i=0; i<n; ++i) zt[i]*=100;
  }

  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';
  Int_t itrk=0;
  TObject *particle=NULL;
  Int_t iPrimary = gGeoManager->AddTrack(itrk,PdgCode,particle);
  TVirtualGeoTrack *myTrack = gGeoManager->GetTrack(iPrimary); // keep link in class
  for(unsigned int i=0; i<n; ++i)
  {
    if(zt[i]>zmax) break;
    double tt=MyConstPhys::CLight*st[i];
    if(verbose&&i<1000) cout << " i=" << setw(3) << i << " x=" << setw(12) << xt[i] << " y=" << setw(12) << yt[i] << " z=" << setw(10) << zt[i] << " tt=" << setw(10) << tt << '\n';
    if(zt[i]>zmin) myTrack->AddPoint(xt[i],yt[i],zt[i],tt);
  }
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';

  myTrack->Print();
  // myTrack->Draw(); // Draw("ogl")
  gGeoManager->GetTopVolume()->SetVisRaytrace(true);  // switches to more solid view    --- more similar to OpenGL

  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';

  // gGeoManager->GetTopVolume()->Draw(); // shows geometry, but track not visible       Draw("ogl")

  cout << " gGeoManager->GetNtracks()=" << gGeoManager->GetNtracks() << '\n'; //  fNtracks
  // cout << " myTrack->GetNtracks()=" << myTrack->GetNtracks() << '\n'; //  fNtracks


  // gGeoManager->DrawTracks(); // shows track in c1 after click in window, also tried with  "ogl"  which has not effect here

  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';

  if (gROOT->GetListOfCanvases()->FindObject("c1")) cout << " UseGL()=" << ((TCanvas*)gROOT->GetListOfCanvases()->FindObject("c1"))->UseGL() << '\n'; // check if ogl on, still off
  // shows track after click in canvas window   -- track immediately visible when not calling   gGeoManager->GetTopVolume()->Draw();  after myTrack->Draw();

  // gGeoManager->DrawTracks(); // shows track in c1
  // myTrack->Clear();
  // myTrack->Draw(); // track visible in 2 dim  only
}

vector<const char*> GetKoordNames(const Ntuple& nt) // find out which kind of x,y,z  or similar are there
{
  unsigned int verbose=nt.GetVerbose();
  vector<const char*> koord(3);
  koord[0]="x";
  koord[1]="y";
  koord[2]="S";
  if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " select Variables" << endl;

  if(nt.VarExists("X_2")) koord[0]="X_2"; else if(nt.VarExists("X")) koord[0]="X"; // accept X_2, X, or x
  if(nt.VarExists("Y_2")) koord[1]="Y_2"; else if(nt.VarExists("Y")) koord[1]="Y"; // accept Y_2, Y, or y
  if(nt.VarExists("Z_2")) koord[2]="Z_2"; else if(nt.VarExists("Z")) koord[2]="Z"; else if(nt.VarExists("z")) koord[2]="z"; // accept Z_2, Z, z or s
  if(verbose) cout << "GetKoordNames Variables " << koord[0] << ", " << koord[1] << ", " << koord[2] << " will be used" << '\n';
  return koord;
}

void EveSingleTrackDisplay(const Ntuple& nt,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,unsigned int verbose)
// use euclidian coordinates X_2, Y_2, Z_2 if available, x,y, z or s  otherwise
// zmin, zmax  --  tricky for rings, check also x not to jump to other side
//
//  using root global   gEve
//
// with  TEveLine    as in tutorial  compound.C
//
// TEve  has concept of tracks with vertex, momentum and optional reference points, maybe overkill
// en->ExpandIntoListTreesRecursively();
// TEveTrackList *gTrackList = new TEveTrackList("MyTracks");
// cout << " gTrackList=" << gTrackList << " defined" << '\n';
//
{
  bool goto_CM_units=true;
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " nt.Noent()=" << nt.Noent() << " Scale_xy=" << Scale_xy << " goto_CM_units=" << goto_CM_units << '\n';
  bool SaveTrack=true;
  if(gEve==NULL)
  {
    cout << "*** Error *** Eve not setup. Do first   StartEveWithGeomDisplay" << '\n';
    return;
  }
  string dir,fname;
  string full_ntuple_fname=nt.GetFname();
  GetDirFname(full_ntuple_fname,dir,fname);
  string fname_no_ext=WithoutExtension(fname);
  cout << "now plot track file with fname_no_ext=" << fname_no_ext << '\n';

  unsigned int n=nt.Noent();
  double xt[n],yt[n],zt[n];
  const vector<const char*> koord =GetKoordNames(nt);;
  const valarray<double> xvec=nt.GetVar(koord[0]);
  const valarray<double> yvec=nt.GetVar(koord[1]);
  const valarray<double> zvec=nt.GetVar(koord[2]);

  vector<int> iElList; // http://www.cplusplus.com/reference/vector/vector/
  // find elements in allowed z range,  tricky in rings, check also for x not to jumo to other side
  double zstart=0;
  if(zmin<0) // start on negative side, go to zero
  {
    for(unsigned int i=0;i<nt.Noent();++i) if(zvec[i] < zstart && zvec[i] >= zmin && fabs(xvec[i])< zmax ) iElList.push_back(i);
    zstart=1.e-10; // put to some positive small value, to avoid to plot the zero point again on the positive side
  }
  // now the positive side
  for(unsigned int i=0;i<nt.Noent();++i) if(zvec[i] >= zstart && fabs(xvec[i])< zmax ) { iElList.push_back(i); if(zvec[i] >= zmax) break; }

  for(unsigned int i=0; i<n; ++i) xt[i]=Scale_xy*xvec[i];
  for(unsigned int i=0; i<n; ++i) yt[i]=Scale_xy*yvec[i];
  for(unsigned int i=0; i<n; ++i) zt[i]=         zvec[i];
  // order in increasing z

  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " goto_CM_units=" << goto_CM_units << " zmin=" << zmin << " zmax=" << zmax << '\n';

  TEveLine* line = new TEveLine(fname_no_ext.c_str()); // give the line the name of the track file
  line->SetMainColor(kBlack);
  for(unsigned int j=0;j<iElList.size();++j) // --- loop over elements and plot range
  {
    unsigned int i=iElList[j];
    //if(zt[i]>=zmin && zt[i]<=zmax && fabs(xt[i])< zmax) // draw
    //{
    if(verbose>1 && i<1000) cout << " j=" << setw(3) << j << " i=" << setw(3) << i << " x=" << setw(12) << xt[i] << " y=" << setw(12) << yt[i] << " z=" << setw(10) << zt[i] << '\n';
    if(goto_CM_units) { xt[i]*=100; yt[i]*=100; zt[i]*=100; } // unit change only just before drawing with SetNextPoint
    line->SetNextPoint(xt[i],yt[i],zt[i]);
    //}
  }
  if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';

  gEve->AddElement(line);
  if(SaveTrack) // save track in root format -   such that it can be read without need for my root extensions
  { // http://root.cern.ch/drupal/content/how-read-objects-file
    string line_fname=WithoutExtension(fname)+string(".root");  // http://root.cern.ch/root/html/TGObject.html
    if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " line->SaveAs(" << line_fname << ")" << '\n';
    line->SaveAs(line_fname.c_str());  // TEveLine::SaveAs       where is this ?   cd /opt/local/include/root ;  find . -type f -exec cat {} \; | grep -R SaveAs
  }
  if(verbose>2) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " before gEve->Redraw3D()"<< '\n';
  gEve->Redraw3D();
  if(verbose>2) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__  << " after gEve->Redraw3D()"<< '\n';
}

void PlotTracksInFiles(const char* fnames_tracks,const char* fname_survey,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,unsigned int verbose)
{ // loop over files  fnames_tracks, translate to euclidian and ploit

  const bool SaveTranslatedFiles=false;
  bool Transform=true; // set false to avoid segmentation violation seen some times in root6 in CourantSnyder_to_Euclidian

  if(!FileExists(ResolveHome(fname_survey)))
  {
    if(verbose) cout << "survey file " << fname_survey << " was not found. No translation to Euclidian coordinates" << '\n';
    Transform=false;
  }
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " fnames_tracks=" << fnames_tracks
    << " SaveTranslatedFiles=" << SaveTranslatedFiles << " Transform=" << Transform << " verbose=" << verbose << '\n';
  vector<string> FileList=ListOfFilesMatching(fnames_tracks,verbose);
  Ntuple* nt_s=nullptr;
  if(verbose>1) for(unsigned int i=0;i<FileList.size();++i) cout << "FileList[" << setw(2) << i << "]=" << FileList[i] << '\n';
  if(Transform)
  {
    cout << "Survey file fname_survey=" << fname_survey << " will be used if required to translate track Courant Synder track coordinates to Euclidian coordinates" << '\n';
    nt_s=new Ntuple(fname_survey);
  }
  for(unsigned int i=0;i<FileList.size();++i)
  {
    if(verbose>1) cout << "Now track file from FileList[" << setw(2) << i << "]=" << FileList[i] << endl;
    Ntuple nt_trk(FileList[i].c_str());
    if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << "after Ntuple nt_trk(..)" << endl;
    if(verbose) nt_trk.PrintSummary(); // track, starting IP5
    if(Transform)
    {
      Ntuple nt_trk_eu=CourantSnyder_to_Euclidian(*nt_s,nt_trk,zmin,zmax); // at some point crashing in root6, now ok
      if(verbose) { cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " after translation" << '\n'; nt_trk_eu.PrintSummary(); }
      if(SaveTranslatedFiles)
      {
        string Dir,Fname;
        GetDirFname(nt_trk_eu.GetFname(),Dir,Fname);
        Fname="/tmp/hbu/"+Fname;
        if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " before nt_trk_eu.WriteAsciiNtFile" << '\n';
        nt_trk_eu.WriteAsciiNtFile(NULL,Fname.c_str()); // save translated file on /tmp/hbu
      }
      EveSingleTrackDisplay(nt_trk_eu,zmin,zmax,kGreen,Scale_xy,verbose);
    }
    else EveSingleTrackDisplay(nt_trk,zmin,zmax,kGreen,Scale_xy,verbose);
  }
}

void Plot_axis_arrow(const string axis,const double length,const double Scale_xy,const unsigned int verbose,const bool goto_CM_units)
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " axis=" << axis << " length=" << length << " Scale_xy=" << Scale_xy << " verbose=" << verbose << " goto_CM_units=" << goto_CM_units << endl;
  string lunit=to_string((int)length)+"m";
  TEveStraightLineSet* eve_axis_arrow   = new TEveStraightLineSet( (axis+"_"+lunit).c_str()  ); // "xy_1m"
  TEveVectorT<double> xp;
  if(axis=="x")      xp.Set(length, 0, 0);
  else if(axis=="y") xp.Set(0, length, 0);
  else if(axis=="z") xp.Set(0, 0, length);
  else { cout << " warning axis=" << axis << " no known Plot_axis_arrow does nothing" << '\n'; return; }
  TEveVectorT<double> xm(0,0,0); // origin
  // apply Scale_xy to arrows, and optionally convert to CM units
  if(axis!="z") xp*=Scale_xy; // scale factor not used for z
  if(goto_CM_units) xp*=100;
  eve_axis_arrow->AddLine(xm,xp);
  eve_axis_arrow->SetLineWidth(4);
  gEve->AddElement(eve_axis_arrow);

  // text on arrow
  TEveText* t1 = new TEveText(lunit.c_str()); // https://root.cern.ch/root/html/TEveText.html
  t1->SetMainColor(kBlack);
  t1->SetFontSize(18);
  // t1->SetFontMode(TGLFont::kExtrude);
  t1->SetLighting(kTRUE);
  t1->PtrMainTrans()->SetPos(xp);  // https://root.cern.ch/root/html/TEveElement.html  https://root.cern.ch/root/html/TEveTrans.html
  
  gEve->AddElement(t1);
}

void Plot_Bend_SR_Cones(const Ntuple& nt,const Beam &Mach,const double zmin,const double zmax,const double Scale_xy,const unsigned int verbose,const bool goto_CM_units,const double sign) // see also   ~/root_git_build/tutorials/eve/jetcone.C
// --  draw tangential lines and SR cones towards z=0      w/o beam divergence, relevant for neutral tracking  --- lines ok, cones shuld be improved
// done here in TEve. Alternative could be to create each cone as a geometry which could be saved as and reloaded, to allow for individual colors, names ..
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " Scale_xy=" << Scale_xy << endl;

  unsigned int n=nt.Noent();
  vector<string> NameCol   =nt.GetStrCol("NAME");
  vector<string> KeywordCol=nt.GetStrCol("KEYWORD");

  if(gEve==NULL) cout << " *** careful *** global gEve=" << gEve << " is not defined, some general features like setting colors may cause segmentation violation" << endl;

  // use two line sets, one for start and one for end of bend
  TEveStraightLineSet* eve_line_s = new TEveStraightLineSet("BendLines"); // can display lines together as set, but only save one by one seems  http://root.cern.ch/root/html/TEveStraightLineSet.html
  TEveStraightLineSet* eve_line_e = new TEveStraightLineSet("BendLines"); // can display lines together as set, but only save one by one seems  http://root.cern.ch/root/html/TEveStraightLineSet.html
  eve_line_s->SetLineWidth(2); eve_line_s->SetLineColor(kRed);   // draw a   red line from start of bend
  eve_line_e->SetLineWidth(2); eve_line_e->SetLineColor(kGreen); // draw a green line from end   of bend

  double length=1;
  Plot_axis_arrow("x",length,Scale_xy,verbose);
  Plot_axis_arrow("y",length,Scale_xy,verbose);
  if(zmax>20) length=100;
  Plot_axis_arrow("z",length,Scale_xy,verbose);

  // PlotCone
  TEveStraightLineSet* eve_line_m = new TEveStraightLineSet("BendLines"); // middle vector
  eve_line_m->SetLineWidth(1); eve_line_m->SetLineColor(kGray); eve_line_m->SetLineStyle(7); // https://root.cern.ch/root/html/TAttLine.html   use 7 to get dashed

  const vector<const char*> koord =GetKoordNames(nt);;
  const valarray<double> xvec=nt.GetVar(koord[0]);
  const valarray<double> yvec=nt.GetVar(koord[1]);
  const valarray<double> zvec=nt.GetVar(koord[2]);
  const valarray<double> angle=nt.GetVar("ANGLE");
  const valarray<double> theta=nt.GetVar("THETA");
  const valarray<double>   phi=nt.GetVar("PHI");
  const valarray<double>   psi=nt.GetVar("PSI");
  const valarray<double> EcritBend=nt.GetVar("EcritBend");
  const valarray<double> ngamBend=nt.GetVar("ngamBend");

  Vec3 z_unit_vec(0,0,1);
  if(sign<0) z_unit_vec=-1.*z_unit_vec;  // (0,0,-1)

  if(verbose) cout << __FILE__ << " " << __FUNCTION__  << " line " << __LINE__ << setprecision(3) << " n=" << n << " yvec.size()=" << yvec.size() << '\n';
  for(unsigned int i=1; i<n; ++i) // loop over elements
  {
    if(KeywordCol[i].find("BEND") !=string::npos) // sbend or rbend
    {
      Vec3 p0_s(xvec[i-1],yvec[i-1],zvec[i-1]); // Start of beand from previous element, ordered by increasing s
      Vec3 p0_e(  xvec[i],  yvec[i],  zvec[i]); // End   of bend

      if(verbose>1) cout << __FILE__ << " " << __FUNCTION__  << " line " << __LINE__ << " i=" << i << " zvec[i]=" << zvec[i] << '\n';
      
      if(zvec[i] > zmin && zvec[i] < zmax) // in z within range
      {
        // power
        double U0=ngamBend[i]*MeanSyn*EcritBend[i];
        double PowBend=Mach.ibeam*1.e9*U0;

        //---- cone declaration --- here in loop to get separate cones -- change color..
        TEveBoxSet* cones = new TEveBoxSet(NameCol[i].c_str());  // see  http://root.cern.ch/root/html/TEveBoxSet.html    $EDITOR $ROOTSYS/tutorials/eve/boxset_cones.C
        // cones->SetPickable(kTRUE);

        cones->UseSingleColor(); // seems needed for Transparency, do not make shape too flat, then always grey
        cones->SetMainColor(kGreen);    // see  $EDITOR ~/root_git/include/root/TEveDigitSet.h ~/root_git/src/graf3d/eve/src/TEveDigitSet.cxx         void DigitColor(Color_t ci, Char_t transparency);    https://root.cern.ch/root/html/TColor.html
        Char_t MaxTransp=99;  // seen in boxset.C  boxset_single_color   50 is half transparent, 90 is very transparent   https://root.cern.ch/root/html/TEveElement.html
        Char_t DeltaTransp=1;
        cones->SetMainTransparency(MaxTransp);
        if(PowBend>1.e1) { cones->SetMainTransparency(MaxTransp-1*DeltaTransp); }
        if(PowBend>1.e2) { cones->SetMainTransparency(MaxTransp-2*DeltaTransp); }
        if(PowBend>1.e3) { cones->SetMainTransparency(MaxTransp-3*DeltaTransp); cones->SetMainColor(kRed-2); }
        if(PowBend>1.e4) { cones->SetMainTransparency(MaxTransp-4*DeltaTransp); cones->SetMainColor(kRed-3); }
        if(PowBend>1.e5) { cones->SetMainTransparency(MaxTransp-5*DeltaTransp); cones->SetMainColor(kRed-4); }
        cones->Reset(TEveBoxSet::kBT_EllipticCone, kFALSE, 64); // EllipticCone
        //-----

        Mat3x3 WCS_s(WCS_mat3(theta[i-1],phi[i-1],psi[i-1])); // matrix, bend start
        Mat3x3 WCS_e(WCS_mat3(theta[i],    phi[i],  psi[i])); // matrix, bend end
        Vec3 dirvec_s=WCS_s*z_unit_vec; // tanget bend start
        Vec3 dirvec_e=WCS_e*z_unit_vec; // tanget bend end
        if(verbose>1) cout << left << setw(12) << NameCol[i] << setw(6) << " zvec[i]=" << setw(6) << zvec[i] << " dirvec_s " << dirvec_s.Print() << '\n' << WCS_s.Print();
        if(verbose>1) cout << left << setw(12) << NameCol[i] << setw(6) << " zvec[i]=" << setw(6) << zvec[i] << " dirvec_e " << dirvec_e.Print() << '\n' << WCS_e.Print();
        Vec3 p1_s=p0_s+dirvec_s; // start bend vector + direction unit vector
        Vec3 p1_e=p0_e+dirvec_e; // end   bend vector + direction unit vector
        double len_s=1;
        double len_e=1;
        if(fabs(p1_e.r[2])<fabs(p0_e.r[2])) // get length to reach to 0 in z
        {
          len_s=fabs(p0_s.r[2]/(p1_s.r[2]-p0_s.r[2]));
          len_e=fabs(p0_e.r[2]/(p1_e.r[2]-p0_e.r[2]));
          if(verbose>1) cout << " pointing in z to 0  len_s=" << setw(10) << len_s << " p1_s.r[2]-p0_s.r[2]= " << setw(10) << p1_s.r[2]-p0_s.r[2] << '\n';
          if(verbose>1) cout << " pointing in z to 0  len_e=" << setw(10) << len_e << " p1_e.r[2]-p0_e.r[2]= " << setw(10) << p1_e.r[2]-p0_e.r[2] << '\n';
        }
        p1_s=p0_s+len_s*dirvec_s;
        p1_e=p0_e+len_e*dirvec_e;
        if(verbose) cout << setw(5) << i << setw(15) << NameCol[i-1] << " line from " << p0_s.Print() << " to " << p1_s.Print() << " theta=" << setw(12) << theta[i-1] << '\n';
        if(verbose) cout << setw(5) << i << setw(15) << NameCol[i]   << " line from " << p0_e.Print() << " to " << p1_e.Print() << " theta=" << setw(12) << theta[i] << '\n';
        if(fabs(p1_e.r[0])> zmax)
        {
          if(verbose) cout << " x1=p1_e.r[0]=" << p1_e.r[0] << " larger than zmax=" << zmax << " skip" << '\n';
          continue;
        }
        if(abs(p1_s)>zmax || abs(p1_e)>zmax)
        {
          if(verbose) cout << " abs(p1_s)=" << abs(p1_s) << " abs(p1_e)=" << abs(p1_e) << " too large, skip" << '\n';
          continue;
        }

        // now Scale_xy
        p0_s.r[0]*=Scale_xy; p0_s.r[1]*=Scale_xy;  p1_s.r[0]*=Scale_xy; p1_s.r[1]*=Scale_xy;
        p0_e.r[0]*=Scale_xy; p0_e.r[1]*=Scale_xy;  p1_e.r[0]*=Scale_xy; p1_e.r[1]*=Scale_xy;
        Vec3 p0_m = 0.5*(p0_s+p0_e); // middle of magnet
        Vec3 v_s=(p1_s-p0_m);  // from middle of magnet to left  end of cone, same length as v_e
        Vec3 v_e= p1_e-p0_m;   // from middle of magnet to right end of cone
        v_s=(len_e/len_s)*v_s; // same unscaled length as v_e
        if(verbose>1) cout << "p0_m magnet middle        " << p0_m.Print() << '\n';
        if(verbose>1) cout << "v_s  to left  end of cone " << v_s.Print() << " len=" << abs(v_s) << '\n';
        if(verbose>1) cout << "v_e  to right end of cone " << v_e.Print() << " len=" << abs(v_e) << '\n';

        if(abs(v_s)>Scale_xy*zmax || abs(v_e)>Scale_xy*zmax)
        {
          if(verbose) cout << " abs(v_s)=" << abs(v_s) << " abs(v_e)=" << abs(v_e) << " too large, skip" << '\n';
          continue;
        }

        // angle between vectors after scaling
        double cos_angle_cone=(v_s*v_e)/(abs(v_s)*abs(v_e));
        double angle_cone=acos(cos_angle_cone);
        if(verbose) cout << " cos_angle_cone=" << cos_angle_cone << " angle_cone=" << angle_cone << " Scale_xy*angle[i]=" << Scale_xy*angle[i] << " v_s*v_e=" << v_s*v_e << " abs(v_s)=" << abs(v_s) << " abs(v_e)=" << abs(v_e)
          << " PowBend=" << PowerWithUnits(PowBend) << '\n' << '\n';
        TEveVectorT<double> P0_s=to_TEveVector(p0_s), P1_s=to_TEveVector(p1_s);
        TEveVectorT<double> P0_e=to_TEveVector(p0_e), P1_e=to_TEveVector(p1_e);
        if(goto_CM_units) // change to CM just before plotting
        {
          P0_s*=100; P1_s*=100;
          P0_e*=100; P1_e*=100;
        }
        eve_line_s->AddLine( P0_s,P1_s);
        eve_line_e->AddLine( P0_e,P1_e);

        // draw also cones
        double cone_len=abs(v_s+v_e)/2;
        if(goto_CM_units) cone_len*=100;

        // middle vector
        TEveVectorT<double> P0_m=0.5*(P0_s +P0_e);
        TEveVectorT<double> P1_m=0.5*(P1_s +P1_e); // tricky part -- take simple approx
        eve_line_m->AddLine( P0_m,P1_s );
        eve_line_m->AddLine( P0_m,P1_e );
        eve_line_m->AddLine( P0_m,P1_m );

        double r1=angle_cone*cone_len/2;
        double r2=r1/10; // color effects better visible when not too small

        cones->AddEllipticCone(P0_m,P1_m-P0_m,r1,r2,0);

        gEve->AddElement(cones);

      }
    }
  }
  gEve->AddElement(eve_line_s); // show start lines
  gEve->AddElement(eve_line_e); // show end lines

  gEve->AddElement(eve_line_m);     // show middle lines
  gEve->Redraw3D(kTRUE);
    
  PlotGuidesAtOrigin();

}

/*

 source ~/bin/my_Xcode_env.sh 5 1 root5      ; switch to root5


 https://root.cern.ch/root/html/tutorials/eve/index.html  Eve tutorial index

 cd $ROOTSYS/tutorials/eve
 $ROOTSYS/bin/root         ; echo  or also my   root

 .x alice_esd.C            // Alice view with track 3D and RPhy amd RhoZ View,  tracks can be highlighted in 3D and projections, uses TEveTrack
 .x alice_vsd.C            // Alice view with tracks and projections, uses  MultiView.C, looks for  Event...  keys in   http://mtadel.home.cern.ch/mtadel/root/AliVSD.root

 .x calo_detail.C          // draw calorimeter data

 .x geom_atlas.C           // loads  'http://root.cern.ch/files/atlas.root  with several parts, all by    TEveGeoTopNode,  AddGlobalElement;   double click on Scenes folder icon, Geometry scene has Inne 1, CENT 1, OUTE 1    and event scene,   good example, coudl be useful for cones
 .x geom_alice_tpc.C       // alice TPC
 .x geom_alice_its.C       // Alice  central ITS
 .x geom_atlas_playback.C  // loads tracks from web, takes a moment, tried from home, for short term atlas geometry visible,  works better at CERN, no tracks ?

 .x geom_cms.C             // shows CMS
 .x geom_cms_playback.C    // shows cms,   no tracks ?
 .x geom_cms_stereo.C      //

 .x pythia_display.C       // did not work, I do not have libPythia6 in any of my root versions,  I have instead libEGPythia8.so
 .x geom_lhcb.C            // works, shows LHCb geometry
 .x pointset.C             // many points of same color
 .x selection_sigs.C       // nothing ?
 .x track.C                // curved track and coordinate system     ----   interesting ---    tracking with field !!!!!!
 .x lineset.C              // straight lines, works
 .x compound.C             // more complicated lines
 .x box.C                  // draw general box
 .x triangleset.C          // complex torus shapes, made up of many triangles.  eads   broken_torus.tring
 .x annotation.C           // rotatable box and movable text
 .x arrow_standalone.C     // arrows and coordinate system origin shapes
 .x arrow.C                //   -- similar
 .x view3ds.C              // kind of futuristic aeroplane
 .x jetcone.C              // lines and cones
 .x pack.C                 // clickable text fields ?
 .x paramlist.C            // does nothing ?

 .x hierarchical_scene.C   // many spheres of different color

 .x projection.C           // 2-dim projection of Alice

 not got working  -- not much tried  --- not yet root6 ?        fails in  ROOT 6.02/04 and in ROOT 6.03/02
 .x quadset.C              // undeclared quadset_set_callback
 .x quadset_callback.cxx
 .x window_manager.C


 quadset works in root5
 source ~/bin/my_Xcode_env.sh clang 1 root5
 cd $ROOTSYS/share/root/tutorials/eve ;  ~/ROOT/5.34.18/bin/root
 .x quadset.C


 tracking with field :

 cd $ROOTSYS/tutorials/eve
 $ROOTSYS/bin/root
 .L track.C
 track(0, kTRUE) ; // B = 0, no difference btween signed and charge particles
 track(1, kTRUE) ; // constant B field (along z, but could have arbitrary direction)
 track(2, kTRUE) ; // variable B field, sign change at  R = 200 cm
 track(3, kTRUE) ; // magnetic field with a zero-field region
 track(4, kTRUE) ; // CMS magnetic field - simple track;   with 5 yellow dots (marks)
 track(5, kTRUE) ; // CMS magnetic field - track with different path-marks.   with 7 yellow dots (marks)
 track(6, kTRUE) ; // Concpetual ILC detector, problematic track


 good check
 track(4, kTRUE) ; track(4, kFALSE) ; // to compare the 2 field tracking methods


 //-- for tracks with projections see   $EDITOR $ROOTSYS/tutorials/eve/alice_esd.C $ROOTSYS/tutorials/eve/alice_vsd.C
 file  "http://root.cern.ch/files/alice_ESDs.root

 cd $ROOTSYS/tutorials/eve ; root
 .L track.C
 track(1, kTRUE) ;
 MultiView* gMultiView = new MultiView();
 if(gMultiView->f3DView) gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline); else cout << "no f3DView" << '\n';
 gMultiView->SetDepth(-10);

 */

