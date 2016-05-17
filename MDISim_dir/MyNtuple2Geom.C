/*  ~/c/MDISim/MyNtuple2Geom.C

 Geometry subject to optional
 Scale_xy                 transverse coordinates scale increase,   to improve visibility - to do all critical calculations without, see comments on "Dirty scaling which appears to work"
 bool goto_CM_units=true; translation to cmto match geant4

 Nutple input coordinates from MAD-X stay always in meter units

 input is MAD-X
 twiss  tfs
 survey tfs

 */

#ifndef __CLING__
#include "MyCppStdHeaders.H"
#include <TROOT.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TMarker.h>
#include <TBox.h>
#include <TArc.h>
#include <TGeoManager.h>
#include <TGeoCompositeShape.h>
#endif

#include "MyRoot.h" // WCS_root
#include "MyNtuple2Geom.h"

Aper operator*(const Aper a,const double f) // multiply all apertures by factor f
{
  Aper res=a; // start from full clone, i.e. copy Name, Keyword,Apertype
  res.a_1=a.a_1*f;
  res.a_2=a.a_2*f;
  res.a_3=a.a_3*f;
  res.a_4=a.a_4*f;
  return res;
}

double Aper::amax() const // maximum of the four aperture numbers
{
  double result=0;
  if(a_1>result) result=a_1;
  if(a_2>result) result=a_2;
  if(a_3>result) result=a_3;
  if(a_4>result) result=a_4;
  return result;
}

const string Aper::Print() const
{
  ostringstream ostr;
  ostr
  << setw(20) << Name
  << setw(20) << Keyword
  << setw(20) << Apertype
  << setw(12) << a_1 << setw(12) << a_2 << setw(12) << a_3 << setw(12) << a_4 ;
  return ostr.str();
}

bool Aper::RECTELLIPSE_IsCircle() const
{
  return Apertype=="RECTELLIPSE" && a_3==a_4 && a_3<=a_1 && a_4 <= a_2; // circle, smaller than rectangular
}

bool Aper::RECTELLIPSE_IsEllipse() const
{
  return Apertype=="RECTELLIPSE" && a_3<=a_1 && a_4<=a_2;
}

bool Aper::RECTELLIPSE_IsRectCircle() const
{
  return Apertype=="RECTELLIPSE" && a_3==a_4;
}

void Plot_BeamScreen_2d(const Aper aper_l) //-- for debug  show directly BeamScreen in 2d
{
  static unsigned int icalled=0;
  double const deg=180./M_PI;
  unsigned int verbose=0;
  double xmax=ceil(aper_l.amax());
  if(verbose) cout << " Plot_BeamScreen_2d start icalled=" << icalled << " aper_l.amax()=" << aper_l.amax() << " xmax=" << xmax << " aper=" << aper_l.Print() << '\n';
  if(icalled<20) // plot only at the 20 first BeamSceen's
  {
    double x0=aper_l.a_1, y0=aper_l.a_2, r=aper_l.a_3;
    bool TestRotated=false;
    if(TestRotated) // checked ok
    { // test  rotated beam screen invert x,y
      double temp=y0;
      y0=x0;
      x0=temp;
    }
    TCanvas* c2 = new TCanvas("c2","BeamScreen_2d",900,10,700,700);  // 900,10  is the position on the screen - adapted to MacPro   and 700, 700 the size in pixels  (square)
    TBox     *b = new TBox(-x0,-y0, x0, y0);
    TEllipse *e = new TEllipse(0,0, r, r);
    double phi,xt,yt;
    if(x0<r) phi=acos(x0/r);
    if(y0<r) phi=asin(y0/r);
    xt=r*cos(phi);
    yt=r*sin(phi);
    if(verbose) cout << "phi=" << phi << " crossing at " << xt << " " << yt << '\n';
    TMarker *m=new TMarker(xt,yt,8); m->Draw(); // dot at crossing
    b->SetFillStyle(0); // hollow, draw contour
    b->SetLineColor(kGray);
    b->SetLineStyle(2); // dashed - here no effect
    b->Draw();
    e->SetLineStyle(2); // dashed
    e->SetLineColor(kGray);
    e->SetFillStyle(0); // hollow
    e->Draw();
    double xmin=-xmax,ymin=-xmax,ymax=xmax;
    TGaxis *xaxis = new TGaxis(xmin,0,xmax,0,xmin,xmax,510,""); xaxis->SetName("x"); xaxis->Draw();
    TGaxis *yaxis = new TGaxis(0,ymin,0,ymax,ymin,ymax,510,""); yaxis->SetName("x"); yaxis->Draw();
    // construct rectellipse
    if(x0<r)
    {
      // cout << "rotated beam screen" << '\n';
      TLine *l1=new TLine( xt,-yt, xt, yt); l1->SetLineColor(kRed); l1->SetLineWidth(2); l1->Draw();
      TLine *l2=new TLine(-xt,-yt,-xt, yt); l2->SetLineColor(kRed); l2->SetLineWidth(2); l2->Draw();
      TArc *a1 =new TArc(0,0, r,    deg*phi,180-deg*phi); a1->SetLineColor(kRed); a1->SetLineWidth(2); a1->SetFillStyle(0) ; a1->Draw("only");
      TArc *a2 =new TArc(0,0, r,180+deg*phi,360-deg*phi); a2->SetLineColor(kRed); a2->SetLineWidth(2); a2->SetFillStyle(0) ; a2->Draw("only");
    }
    else // -- almost always this
    {
      // cout << "flat beam screen" << '\n';
      TLine *l1=new TLine(-xt, yt, xt, yt); l1->SetLineColor(kRed); l1->SetLineWidth(2); l1->Draw();
      TLine *l2=new TLine(-xt,-yt, xt,-yt); l2->SetLineColor(kRed); l2->SetLineWidth(2); l2->Draw();
      TArc *a1 =new TArc(0,0, r,   -deg*phi,    deg*phi); a1->SetLineColor(kRed); a1->SetLineWidth(2); a1->SetFillStyle(0) ; a1->Draw("only");
      TArc *a2 =new TArc(0,0, r,180-deg*phi,180+deg*phi); a2->SetLineColor(kRed); a2->SetLineWidth(2); a2->SetFillStyle(0) ; a2->Draw("only");
    }
    gPad->Range(1.1*xmin,1.1*ymin,1.1*xmax,1.1*ymax);
    //c2->Update();
    //c2->Show();
    c2->SaveAs((string(aper_l.Name)+string(".eps")).c_str());  // as basis for Illustrator
    // c2->SaveAs((string(Name)+string(".pdf")).c_str());
    if(verbose)
    {
      cout << " Type anything to continue" << '\n';
      char dummy;cin.get(dummy);
    }
  }
  icalled++;
}

TGeoVolume* MyVolume(TGeoMedium *med,const Aper aper,const Aper aper_l,const Aper aper_r,const double len2,const double thick,unsigned int verbose) // make the volumes
// aper    for actual element, use for naming and fields
// aper_l, aper_r   left, right aperture, normally same as aper,   except for drifts
//
// circle and ellipse could just be shapes, RECTELLIPSE more complex
// construct Root TGeoVolume MyVol    Tube, Cone or more complex  RECTELLIPSE  as combination of box and Tubs
// http://root.cern.ch/root/html/TGeoManager.html
// Volumes are "solids" with medium and can contain several nodes
// Nodes represent just positioned instances of volumes inside a container volume and they are not directly defined by the user
{ // len2 is the half length of the volume
  bool goto_CM_units=true;
  static unsigned int ipri=0;
  TGeoVolume *MyVol=NULL;
  TGeoMedium *medVac=(TGeoMedium*)gGeoManager->GetListOfMedia()->FindObject("medVac"); // Vacuum relevant for fields inside magnets
  if(med==nullptr)    { cout << "*** error *** " << __FUNCTION__ << " med    undefined for Volume Name=" << aper.Name << "    med=" << med    << '\n'; exit(1); }
  if(medVac==nullptr) { cout << "*** error *** " << __FUNCTION__ << " medVac undefined for Volume Name=" << aper.Name << " medVac=" << medVac << '\n'; exit(1); }
  string Apertype   =aper.Apertype;
  if(Apertype=="NONE") Apertype = aper_r.Apertype;
  string NameField  = aper.Name;
  string NameVac    = NameField+"_v";
  const unsigned int vol_color=aper.color; // color of actual element
  if(verbose && ipri<100)
  {
    cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__
    << " Name=" << setw(12) << aper.Name << " " << Apertype
    << " half len2=" << setw(8) << len2
    << " aper.RECTELLIPSE_IsCircle()="  << setw(8) << aper.RECTELLIPSE_IsCircle()
    << " aper.RECTELLIPSE_IsEllipse()=" << setw(8) << aper.RECTELLIPSE_IsEllipse()
    << " aper.RECTELLIPSE_IsRectCircle()=" << setw(8) << aper.RECTELLIPSE_IsEllipse()
    << " vol_color=" << setw(4) << vol_color
    << " Material=" << med->GetMaterial()
    << " aper.Print()=" << aper.Print();
    if( aper.a_1==0 ) cout << " aper_l.Print()=" << aper_l.Print() << " aper.Print()=" << aper.Print();
    cout << '\n';
    // add magnet type to element name - used later in geant4 to defined magnetic fields
    if(aper.Keyword.find("BEND")!=std::string::npos) NameField=string(aper.Name)+"_bend";
    else if(aper.Keyword.substr(0,8)=="QUADRUPO") NameField=string(aper.Name)+"_quad";
    else if(aper.Keyword.substr(0,8)=="SEXTUPOL") NameField=string(aper.Name)+"_sext";
    else if(aper.Keyword=="OCTUPOLE") NameField=string(aper.Name)+"_oct";
  }

  // for translation to cm
  double al1=aper_l.a_1;
  double ar1=aper_r.a_1;
  double al2=aper_l.a_2;
  double ar2=aper_r.a_2;
  double r =aper_l.a_3;
  double r2=aper_r.a_3;
  double l2=len2;
  double d=thick;
  if(goto_CM_units)
  {
    al1*=100;
    ar1*=100;
    al2*=100;
    ar2*=100;
    l2*=100;
    r*=100;
    r2*=100;
    d*=100;
  }

  if(Apertype=="RECTELLIPSE") // just print RECTELLIPSE types
  {
    cout << " RECTELLIPSE " << setw(20) << "  aper_r=" << aper_r.Print() << " aper_r=" << aper_r.Print();
    if(aper_r.RECTELLIPSE_IsCircle())          cout << " Circle";     // actually circle
    else if(aper_r.RECTELLIPSE_IsEllipse())    cout << " Ellipse";    // actually ellipse
    else if(aper_r.RECTELLIPSE_IsRectCircle()) cout << " RectCircle"; // actually RectCircle
    cout << '\n';
  }

  // http://root.cern.ch/root/html/TColor.html

  bool UseTorus=false;

  if(Apertype=="CIRCLE" || (Apertype=="RECTELLIPSE" && aper_r.RECTELLIPSE_IsCircle()) )
  {
    if( al1>0 && al1 != ar1 && al2 == 0 && ar2 == 0)
    { // drift space going from one circle to different circle, make a cone, only expected to happen for drift spaces
      if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " MakeCone for " << aper.Name
        << " aper_l is " << aper_l.Print() << " aper_r is " << aper_r.Print() << '\n';
      MyVol                =gGeoManager->MakeCone(aper.Name.c_str(),             med,   l2,  0,al1+d,   0,ar1+d); // filled cone
      TGeoVolume* VacInside=gGeoManager->MakeCone(NameVac.c_str(),medVac,l2,  0,al1  ,   0,ar1  ); // daughter volume, inside, vacuum, possible field
      VacInside->SetLineColor(vol_color); VacInside->SetTransparency(70); MyVol->AddNode(VacInside,1);
    }
    else // fixed radius circle
    {
      if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " fixed radius circle UseTorus=" << UseTorus << '\n';
      MyVol=                gGeoManager->MakeTube(aper.Name.c_str(),             med   ,0,ar1+d, l2); // filled tube (cylinder)
      if(UseTorus)
      {
        cout << "Torus for aper_r.Name" << aper_r.Name << '\n';
        Double_t R=10;    // axial radius
        Double_t Rmin=ar1  ; // inner radius
        Double_t Rmax=ar1+d; // outer radius
        Double_t Phi1=0;  // starting phi angle
        Double_t Dphi=90; // beding angle in degress
        MyVol = gGeoManager->MakeTorus(NameVac.c_str(),medVac,R,Rmin,Rmax,Phi1,Dphi); // http://root.cern.ch/root/html/TGeoManager.html   rmin,rmax, dz,  0 to 10 degrees
        // Node, get bending radius, vacuum inside
      }
      else
      {
        MyVol=                gGeoManager->MakeTube(aper.Name.c_str(),             med   ,0,ar1+d, l2); // filled tube (cylinder)
        TGeoVolume* VacInside=gGeoManager->MakeTube(NameVac.c_str(),medVac,0,ar1  , l2); // daughter volume, inside, vacuum, possible field
        VacInside->SetLineColor(vol_color); VacInside->SetTransparency(70); MyVol->AddNode(VacInside,1);
      }
      if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " done with  fixed radius circle" << '\n';
    }
  }
  else if(Apertype=="ELLIPSE" || (Apertype=="RECTELLIPSE" && aper_r.RECTELLIPSE_IsEllipse()) )
  {
    cout << aper_r.Name << " make MakeEltu" << setw(10) << " l2=" << setw(10) << l2 << "  aper_r=" << aper_r.Print() << '\n';
    MyVol                =gGeoManager->MakeEltu(aper.Name.c_str(),             med,      ar1+d,ar2+d, l2); // filled elliptical tube
    TGeoVolume* VacInside=gGeoManager->MakeEltu(NameVac.c_str(),medVac,   ar1  ,ar2  , l2); // daughter volume, inside, vacuum, possible field
    VacInside->SetLineColor(vol_color); VacInside->SetTransparency(70); MyVol->AddNode(VacInside,1);
  }
  else if(Apertype=="RECTANGLE")
  {
    cout << aper_r.Name << " RECTANGLE " << setw(20) << setw(10) << " l2=" << setw(10) << l2 << "  aper_r=" << aper_r.Print() << '\n';
    MyVol                =gGeoManager->MakeBox(aper.Name.c_str(),             med   ,ar1+d,ar2+d,l2);   // solid box
    TGeoVolume* VacInside=gGeoManager->MakeBox(NameVac.c_str(),medVac,ar1  ,ar2  ,l2);   // daughter volume, inside, vacuum, possible field
    VacInside->SetLineColor(vol_color); VacInside->SetTransparency(70); MyVol->AddNode(VacInside,1);
  }
  else if(Apertype=="RACETRACK") // still check/implement for vacuum and field inside
  {
    double g=ar1,s=ar2;
    cout << aper_r.Name << " RACETRACK " << setw(20) << setw(10) << " l2=" << setw(10) << l2 << "  aper_r=" << aper_r.Print() << " g=" << g << " s=" << s << " r2=" << r2 << " l2=" << l2 << '\n';
    TGeoVolume* my_tube_segment = gGeoManager->MakeTubs(aper.Name.c_str(),med,r2,r2+d,l2,0,90); my_tube_segment->SetLineColor(vol_color-1);   //  http://root.cern.ch/root/html/TGeoTube.html  leave as base segment
    MyVol = gGeoManager->MakeBox(aper.Name.c_str(),med,g+r2,s+r2,l2); // mother volume for Racetrack, box that contains RaceTrack
    MyVol->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation(  g, s, 0), TGeoRotation("rot",0,0,  0)) );
    MyVol->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation( -g, s, 0), TGeoRotation("rot",0,0, 90)) );
    MyVol->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation( -g,-s, 0), TGeoRotation("rot",0,0,180)) );
    MyVol->AddNode(my_tube_segment,1,new TGeoCombiTrans( TGeoTranslation(  g,-s, 0), TGeoRotation("rot",0,0,270)) );
    if(s>0)
    {
      TGeoVolume* sbox = gGeoManager->MakeBox((aper_r.Name+string("_s")).c_str(),med,s,d/2,l2); sbox->SetLineColor(vol_color-2);
      MyVol->AddNode(sbox,1,new TGeoCombiTrans( TGeoTranslation(  g+r2+d/2,  0, 0), TGeoRotation("rot",0,0,90)) );
      MyVol->AddNode(sbox,1,new TGeoCombiTrans( TGeoTranslation(-(g+r2+d/2), 0, 0), TGeoRotation("rot",0,0,90)) );
    }
    if(g>0)
    {
      TGeoVolume* gbox = gGeoManager->MakeBox((aper_r.Name+string("_g")).c_str(),med,g,d/2,l2); gbox->SetLineColor(vol_color-3);
      MyVol->AddNode(gbox,1,new TGeoCombiTrans( TGeoTranslation(0,  s+r2+d/2,  0), TGeoRotation("rot",0,0,180)) );
      MyVol->AddNode(gbox,1,new TGeoCombiTrans( TGeoTranslation(0,-(s+r2+d/2), 0), TGeoRotation("rot",0,0,180)) );
    }
    MyVol->SetTransparency(70);
  } // RACETRACK
  else if(Apertype=="RECTELLIPSE" || Apertype=="RECTCIRCLE") // construct as connected subvolumes, still check/implement for vacuum and field inside
  {
    cout << aper_r.Name << " Apertype=" << aper_r.Apertype << " as RECTELLIPSE" << '\n';
    if(verbose>1) Plot_BeamScreen_2d(aper_r);
    double const deg=180./M_PI;
    double x0=ar1, y0=ar2, phi=0, l=0;
    double amax=aper_r.amax(); // should be the maximum of the three numbers
    if(x0<r2) { phi=acos(x0/r2); l=r2*sin(phi); }
    if(y0<r2) { phi=asin(y0/r2); l=r2*cos(phi); }
    double xt=r2*cos(phi);
    double yt=r2*sin(phi);
    if(verbose>1) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " phi=" << phi*deg << " deg xt=" << xt << " yt=" << yt << " vol_color=" << vol_color << '\n';
    MyVol = gGeoManager->MakeBox(aper.Name.c_str(),med,amax,amax,l2); // block containing the whole beamscreen
    TGeoVolume *box =  gGeoManager->MakeBox((aper_r.Name+string("_b")).c_str(),med,l,d/2,l2); box->SetLineColor(vol_color-2);
    if(x0<r2) // rotated beamscreen
    {
      TGeoVolume * arc = gGeoManager->MakeTubs((aper_r.Name+string("_t")).c_str(),med,r2,r2+d, l2,deg*phi,180-deg*phi); arc->SetLineColor(vol_color-2); // arc top
      MyVol->AddNode(arc,1);                                 // arc top
      MyVol->AddNode(arc,1,new TGeoRotation("rot",0,0,180)); // arc bottom
      MyVol->AddNode(box,1,new TGeoCombiTrans( TGeoTranslation(  xt+d/2 ,0,0), TGeoRotation("rot",0,0,90)) );  // box left
      MyVol->AddNode(box,1,new TGeoCombiTrans( TGeoTranslation(-(xt+d/2),0,0), TGeoRotation("rot",0,0,90)) );  // box right
    }
    else // standard flat MyVol beamscreen
    {
      TGeoVolume * arc =  gGeoManager->MakeTubs((aper_r.Name+string("_r")).c_str(),med,r2,r2+d, l2, -deg*phi, deg*phi); arc->SetLineColor(vol_color-2); // arc right
      MyVol->AddNode(arc,1);                                 // arc right
      MyVol->AddNode(arc,1,new TGeoRotation("rot",0,0,180)); // arc left
      MyVol->AddNode(box,1,new TGeoTranslation(0,  yt+d/2,0));   // box top
      MyVol->AddNode(box,1,new TGeoTranslation(0,-(yt+d/2),0));  // box bottem
    }
    MyVol->SetTransparency(70);
  } // RECTELLIPSE
  else
  {
    cout << " Apertype=" << Apertype << " not yet implemented" << '\n';
    exit(1);
  }
  return MyVol;
}

void MyMaterialInit()
{
  // TGeoMaterial *matVac = new TGeoMaterial("vacuum",0,1,0);   // A,Z,rho     Z=0 gives problem with gdml,   use 0,1,0,  leave A=0, to avoid inf rad length, geant4 not crashing but funny results for some processes
  TGeoMaterial *matVac = new TGeoMaterial("vacuum",1.008,1,1.e-6); // trial 28/10/2014
  TGeoMaterial *matFe  = new TGeoMaterial("Fe",55.845,26,7.87);
  TGeoMedium   *medVac = new TGeoMedium("medVac",1, matVac); // not used in root, for interface to Monte Carlo
  TGeoMedium   *medFe  = new TGeoMedium("medFe", 1, matFe);

  if(0>1) cout << medVac << '\n'; // dummy to supress warning of unused medVac
  if(0>1) cout << medFe  << '\n'; // dummy to supress warning of unused medVac
  // use these later by
  // TGeoMaterial *matVac=(TGeoMaterial*)gGeoManager->GetListOfMaterials()->FindObject("Vacuum");
  // TGeoMedium   *medVac=(TGeoMedium*)gGeoManager->GetListOfMedia()->FindObject("medVac");
}

void InventMagnetAperturesIfZero(valarray<Aper>& aper,unsigned int verbose) // in case no apertures were defined, default type is circle, just specify some radii
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start aper.size()=" << aper.size()<< '\n';
  for(unsigned int i=0;i<aper.size();++i)
  {
    if(aper[i].a_1==0)
    {
      if(aper[i].Keyword.find("BEND")!=std::string::npos) aper[i].a_1=0.05;
      else if(aper[i].Keyword.substr(0,8)=="QUADRUPO") aper[i].a_1=0.04;
      else if(aper[i].Keyword.substr(0,8)=="SEXTUPOL") aper[i].a_1=0.03;
      else if(aper[i].Keyword=="OCTUPOLE") aper[i].a_1=0.02;
      else if(aper[i].Keyword=="RFCAVITY") aper[i].a_1=0.06;
    }
  }
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << "  end" << '\n';
}

void Set_Aperture_colors(valarray<Aper>& aper,const unsigned int color,unsigned int verbose)
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " Set_Aperture_colors start aper.size()=" << aper.size() << " color=" << color << '\n';
  unsigned int my_color=color,last_color=0; // in case of color=0, switch my_color dependend on the element class type stored as KEYWORD
  for(unsigned int i=0;i<aper.size();++i)
  {
    if(color==0) // automatic element class dependent coloring
    {
      if(aper[i].Keyword.find("BEND")!=std::string::npos) my_color=kBlue; // http://root.cern.ch/root/html/Rtypes.h
      else if(aper[i].Keyword.substr(0,8)=="QUADRUPO") my_color=kRed;
      else if(aper[i].Keyword.substr(0,8)=="SEXTUPOL") my_color=kGreen;
      else if(aper[i].Keyword.find("COLLIMATOR")!=std::string::npos) my_color=kGray+3;
      else if(aper[i].Keyword=="MARKER") my_color=kOrange;
      else if(aper[i].Keyword=="DRIFT") my_color=kCyan;
      else my_color=last_color;
    }
    last_color=my_color;
    aper[i].color=my_color;
    if(StartsWith(aper[i].Name,"TA"))
    {
      aper[i].color=kYellow; // set color for TAS, TAN
      if(verbose) cout << "set color to Yellow for " << aper[i].Print() << '\n';
    }
  }
}

valarray<Aper> SetUp_all_Aper(const Ntuple* nt,const unsigned int color,unsigned int verbose)
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start color=" << color << '\n';
  vector<string> nt_StrCol_Name    =nt->GetStrCol("NAME");
  vector<string> nt_StrCol_Apertype=nt->GetStrCol("APERTYPE");
  vector<string> nt_StrCol_Keyword =nt->GetStrCol("KEYWORD");
  // for(unsigned int i=0;i<nt->Noent();++i) nt_StrCol_Name[i]   =CleanName(nt_StrCol_Name[i]); // required to be able to export geometry to C++    do before to have same names in all output

  // setup aper for all elements
  valarray<Aper> aper(nt->Noent());
  unsigned int iv;
  for(unsigned int i=0;i<nt->Noent();++i) aper[i].Name    =nt_StrCol_Name[i];
  for(unsigned int i=0;i<nt->Noent();++i) aper[i].Keyword =nt_StrCol_Keyword[i];
  for(unsigned int i=0;i<nt->Noent();++i) aper[i].Apertype=nt_StrCol_Apertype[i];
  iv=nt->ivar("APER_1"); if(iv<nt->Nvar()) for(unsigned int i=0;i<nt->Noent();++i) aper[i].a_1=nt->Entry(i,iv);
  iv=nt->ivar("APER_2"); if(iv<nt->Nvar()) for(unsigned int i=0;i<nt->Noent();++i) aper[i].a_2=nt->Entry(i,iv);
  iv=nt->ivar("APER_3"); if(iv<nt->Nvar()) for(unsigned int i=0;i<nt->Noent();++i) aper[i].a_3=nt->Entry(i,iv);
  iv=nt->ivar("APER_4"); if(iv<nt->Nvar()) for(unsigned int i=0;i<nt->Noent();++i) aper[i].a_4=nt->Entry(i,iv);

  InventMagnetAperturesIfZero(aper,verbose);  // for magents without aperture information invent some

  // if(color==0)
  Set_Aperture_colors(aper,color,verbose);

  if(verbose>2)
  {
    cout << "List of all apertures at the end of SetUp_all_Aper" << '\n';
    for(unsigned int i=0;i<nt->Noent();++i) cout << aper[i].Print() << '\n';
  }
  return aper;
}

void Connect_Apertures(valarray<Aper>& aper,unsigned int verbose) // connect MAD-X aperture information, use last non-zero aperture
{
  // verbose=0; // turn off locally
  Aper lastaper;// http://www.cplusplus.com/reference/valarray/     http://en.cppreference.com/w/cpp/numeric/valarray
  auto endminus1 = end(aper); --endminus1; // end is one too far, step back by one   http://en.cppreference.com/w/cpp/language/auto
  for(auto this_aper = endminus1; this_aper!=begin(aper); --this_aper) // loop backward from end to find last valid aperture, useful for rings
  {
    if(verbose) cout << __FUNCTION__ << " line " << __LINE__ << " backwards loop " << this_aper->Print() << '\n';
    if(this_aper->a_1>0)
    {
      lastaper= *this_aper;
      if(verbose) cout << __FUNCTION__ << " line " << __LINE__ << " backwards loop last valid aperture is " << lastaper.Print() << '\n'; // keep for end right and start left
      break;
    }
  }
  double apermax=5; // 5 meter, safe upper limit, LHC has some dummy 10m aperture, which will be removed using this limit
  for(auto this_aper = begin(aper); this_aper!=end(aper); ++this_aper)
  {
    if(this_aper->Keyword.substr(0,5)=="DRIFT") continue; // leave drifts without aperture
    if(this_aper->Apertype==string("NONE") || this_aper->a_1==0 || this_aper->a_1>apermax) // no valid aperture, keep last valid
    {
      string Name   =this_aper->Name;
      string Keyword=this_aper->Keyword;
      unsigned int mycolor=this_aper->color;
      *this_aper=lastaper; // take last aperture defined, but keep Name, Keyword, color
      this_aper->Name=Name;
      this_aper->Keyword=Keyword;
      this_aper->color=mycolor;
    }
    else lastaper= *this_aper; // keep the last aperture
    if(verbose>2) cout << "after  connect " << this_aper->Print() << " last " << lastaper.Print() << '\n';
  }
  if(verbose>2)
  {
    cout << "List of all apertures at the end of Connect_Apertures" << '\n';
    for(unsigned int i=0;i<aper.size();++i) cout << aper[i].Print() << '\n';
  }
}

void Simplify_Apertures(valarray<Aper>& aper,unsigned int verbose) // http://madx.web.cern.ch/madx/madX/doc/usrguide/Introduction/aperture.html
{
  for(auto this_aper = begin(aper); this_aper!=end(aper); ++this_aper)
  {
    if(this_aper->Apertype=="ELLIPSE")
    {
      if(verbose>1) cout << __FUNCTION__ << " line " << __LINE__ << " " << this_aper->Print();
      if(this_aper->a_1==this_aper->a_2)
      {
        this_aper->Apertype="CIRCLE";
        this_aper->a_2=0;
        this_aper->a_3=0;
        this_aper->a_4=0;
        if(verbose>1) cout << " Apertype changed to CIRCLE     " << this_aper->Print() << '\n';
      }
      else if(verbose>1) cout << " do nothing" << '\n';
    }
    else if(this_aper->Apertype=="RECTCIRCLE" || this_aper->Apertype=="LHCSCREEN")
    {
      if(verbose>1) cout << __FUNCTION__ << " line " << __LINE__ << " " << this_aper->Print();
      if(this_aper->a_1>this_aper->a_3 && this_aper->a_2>this_aper->a_3) // rectangle larger than circle
      {
        this_aper->Apertype="CIRCLE";
        this_aper->a_1=this_aper->a_3; // take radius
        this_aper->a_2=0;
        this_aper->a_3=0;
        this_aper->a_4=0;
        if(verbose>1) cout << " Apertype changed to CIRCLE     " << this_aper->Print() << '\n';
      }
      else if(verbose>1) cout << " do nothing" << '\n';
    }
    else if(this_aper->Apertype=="RECTELLIPSE")
    {
      if(verbose>1) cout << __FUNCTION__ << " line " << __LINE__ << " " << this_aper->Print();
      if(this_aper->RECTELLIPSE_IsCircle())
      {
        this_aper->Apertype="CIRCLE";
        this_aper->a_1=this_aper->a_3; // take radius
        this_aper->a_2=0;
        this_aper->a_3=0;
        this_aper->a_4=0;
        if(verbose>1) cout << " Apertype changed to CIRCLE     " << this_aper->Print() << '\n';
      }
      else if(this_aper->RECTELLIPSE_IsEllipse())
      {
        this_aper->Apertype="ELLIPSE";
        this_aper->a_1=this_aper->a_3;
        this_aper->a_2=this_aper->a_4;
        this_aper->a_3=0;
        this_aper->a_4=0;
        if(verbose>1) cout << " Apertype changed to ELLIPSE    " << this_aper->Print() << '\n';
      }
      else if( this_aper->a_3 == this_aper->a_4)
      {
        this_aper->Apertype="RECTCIRCLE";
         this_aper->a_4=0;
        if(verbose>1) cout << " Apertype changed to RECTCIRCLE " << this_aper->Print() << '\n';
      }
      else if(verbose>1) cout << " do nothing" << '\n';
    }
    else
    {
      if(verbose>1) cout << __FUNCTION__ << " line " << __LINE__ << " " << this_aper->Print() << " do nothing" << '\n';
    }
  }
}

void Get_il_ir(const valarray<Aper>& aper,const vector<int>& iElList,const unsigned int i,unsigned int& il,unsigned int& ir) // for element i, find next valid aperture left and right il, ir
{
  if(aper[i].a_1>0) { il=i;ir=i; } // this element has valid aperture, start with this left and right
  else // no valid aperture, typical for drift, continue right
  {
    ir=i+1;
    while(ir<iElList.size())
    {
      if(aper[ir].a_1>0) break; // next valid aperture found on the right
      ++ir;
    }
  }
}

void GeomFromMyNtuple(const char* MyGeoName,const string StartElementName,const Ntuple& nt,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,const unsigned int verbose,const bool ConnectApertures,const bool AddIndexToName,const bool SimplifyAper)
// set up  Beam Pipe Apertures  in global  gGeoManager   based on combined ntuple with aperture information, calls MyVol
// the Mad-X optics and survey positions are given a the end of the element
{
  // x_2  is x from survey in global Euclidian coordinates
#ifdef __CINT__
  const char* __PRETTY_FUNCTION__ = "GeomFromMyNtuple";
#endif
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start MyGeoName=" << MyGeoName << " nt.Noent()=" << nt.Noent()
    << " zmin=" << zmin
    << " zmax=" << zmax
    << " color=" << color
    << " Scale_xy=" << Scale_xy
    << " verbose=" << verbose
    << " ConnectApertures=" << ConnectApertures
    << " AddIndexToName=" << AddIndexToName
    << " SimplifyAper=" << SimplifyAper
    << '\n';

  bool goto_CM_units=true; // root volumes assume cm, also important  for gdml which writes with cm units -   no standard unit change seems to exist in root
  double cm=1;
  if(goto_CM_units) cm=100; // here as much as possible staying in meter units, only for geom call use cm

  const double eps=1.e-10;
  double thick=0.002;

  if(gGeoManager) // to allow to call for b1, b2 without overwriting
  {
    cout << "TGeoManager Geometry " << MyGeoName << " was already defined" << '\n';
  }
  else
  {
    cout << " initialize the TGeoManager" << '\n';
    gGeoManager=new TGeoManager(MyGeoName, "My3D"); // create new Geometry
    MyMaterialInit();
  }

  // use materials defined
  TGeoMedium *medVac=(TGeoMedium*)gGeoManager->GetListOfMedia()->FindObject("medVac");
  if(medVac==nullptr) { cout << "*** error *** " << __FUNCTION__ << " medVac not defined" << '\n'; exit(1); }
  TGeoMedium *medFe =(TGeoMedium*)gGeoManager->GetListOfMedia()->FindObject("medFe");
  if(medFe==nullptr)  { cout << "*** error *** " << __FUNCTION__ << " medFe  not defined" << '\n'; exit(1); }

  bool Is_LHC=(CopToLower(nt.GetFname()).find("lhc")!=string::npos);
  if(Is_LHC) cout << "nt.GetFname()=" << nt.GetFname() << " Is_LHC=" << Is_LHC << " Working with LHC" <<'\n';

  valarray<Aper> aper=SetUp_all_Aper(&nt,color,verbose); // set up a table of all apertures
  if(SimplifyAper) Simplify_Apertures(aper,verbose);
  if(ConnectApertures) Connect_Apertures(aper,verbose);

  valarray<double>  lvec=nt.GetVar("L");
  valarray<double>  svec=nt.GetVar("S"); double smax=nt.Max("S");
  valarray<double>  zvec=nt.GetVar("Z");
  valarray<double>   x_2=nt.GetVar("X_2");
  valarray<double>   y_2=nt.GetVar("Y_2");
  valarray<double> theta=nt.GetVar("THETA");
  valarray<double>   phi=nt.GetVar("PHI");
  valarray<double>   psi=nt.GetVar("PSI");

  valarray<double> angle=nt.GetVar("ANGLE"); // bending angle, relevant for bends
  if(theta.size()<nt.Noent())  theta.resize(nt.Noent());

  for(unsigned int i=0;i<nt.Noent();++i) if(theta[i] < -M_PI) theta[i]+=2.*M_PI;  // make sure  -M_PI < theta < M_PI       /usr/include/math.h

  vector<int> iElList; // http://www.cplusplus.com/reference/vector/vector/
  for(unsigned int i=0;i<nt.Noent();++i) if(zvec[i] >= zmin && zvec[i] <= zmax && fabs(x_2[i])< zmax ) iElList.push_back(i); // && svec[i]< 2*pi*zmax)  // point to list of all elements within the specified z range; also check s to avoid to jump to end of half rings
  if(verbose>1) cout << "iElist set up in z-range, from i = " << iElList.front() << " to " << iElList.back() << '\n';

  if(verbose>1) // print element list
  {
    cout << '\n';
    cout << "    i                NAME             Keyword           Apertype        aper1       aper2       aper3       aper4           s           z           l         x_2       theta color     smax-s  l/r aper_a1 l/r" << '\n';
    for(unsigned int j=0;j<190;++j) cout << "-"; cout << '\n';
    for(unsigned int j=0;j<iElList.size();++j)
    {
      unsigned int i=iElList[j]; // i of current element
      unsigned int il,ir;
      Get_il_ir(aper,iElList,i,il,ir); // find left/right aperture
      cout
      << setw(5)  << i
      << aper[i].Print()
      << setw(12) << svec[i]
      << setw(12) << zvec[i]
      << setw(12) << lvec[i]
      << setw(12) << x_2[i]
      << setw(12) << theta[i]
      << setw(5)  << aper[i].color
      << setw(12) << smax-svec[i];
      if( aper[i].a_1==0 ) cout << " " << setw(5) << il << " " << setw(5) << ir << " " << setw(5) << aper[il].a_1 << " " << setw(5) << aper[ir].a_1;
      cout << '\n';
    }
    // cout << "CSPE " << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " stop after print elements" << '\n'; exit(1); // CSPE
  }

  double apermax=5;

  // top Volume  ---  Box of Vacuum  -- not visible        The top volume ist not positioned, it specifies the global reference frame.
  // define in two steps to allow for different names for volume and solid, to keep the geant4 G4GDMLParser happy
  TGeoVolume *top=gGeoManager->GetTopVolume(); // use if already created before
  if(top==NULL)
  {
    cout << "make top volume" << '\n';
    // get the largest
    double x_max=(nt.Max("X_2"))+2.*thick;
    double y_max=(nt.Max("Y"))+2.*thick;
    double z_max=(nt.Max("Z"))+2.*thick;
    if(x_max>y_max) y_max=x_max;
    if(y_max>x_max) x_max=y_max;
    if(x_max>z_max) z_max=x_max;
    cout << "top volume size from maximum dimensions of geometry + 10 times thickness in x,y,z=" << x_max << "," << y_max << "," << z_max << " m  size relevant to stop particles as OutOfWorld in geant4" << '\n';
    TGeoBBox *my_box = new TGeoBBox("top_Solid",zmax*cm,zmax*cm,zmax*cm); // in gdml solid name, not shown, make size big enough to hold all - otherwise  outside mother logical volume top
    top = new TGeoVolume("top",my_box,medVac);  // in gdml volume and world name    Vacuum
    // top = new TGeoVolume("top",my_box,medFe);  // trial, top volume of Iron - not a good idea - results in major losses in "empty" transitions
    gGeoManager->SetTopVolume(top);
    top->SetLineColor(kMagenta);
    gGeoManager->SetTopVisible(0); // 0 not visible       for a test can set to 1 or just ()
    if(verbose) { cout << '\n' << "new volume created "; top->Print(); }
  }
  else // useful for example for LHC, second beam
  {
    if(verbose) { cout << " top volume was already defined and will be re-used "; top->Print();}
  }

  // see if we have a ring or line
  unsigned int ilast=nt.Noent()-1;
  const double ztol=1;
  bool Is_Ring = fabs(zvec[0]-zvec[ilast])<ztol; // initial and final z agree within one meter, assume we have a ring
  if(verbose) cout << " zvec[0]-zvec[ilast]=" << zvec[0]-zvec[ilast] << " Is_Ring=" << Is_Ring << '\n';

  unsigned int i0=iElList.front();  // for line i0 at beginning
  if(Is_Ring)  i0=iElList.back();   // for ring i0 at end
  double slast=0; // just for checks

  double z0=-DBL_MAX;     // determine first z0, can be subtracted to centre objects -   useful to look at few elements, but then zpos different from survey
  bool Subtract_z0=false; // normally false,  set only true if really needed to look at first elements

  for(unsigned int j=0;j<iElList.size();++j) // --- main loop over elements and construct volumes
  {
    unsigned int i=iElList[j];   // i of current element
    aper[i].Name=CleanName(aper[i].Name); // required to be able to export geometry to C++
    if(AddIndexToName) aper[i].Name+="_"+to_string(i);
    // const Aper thisAper=aper[i]; // keep the original information
    unsigned int il,ir;
    Get_il_ir(aper,iElList,i,il,ir); // find left/right aperture

    if( lvec[i]==0 ) continue; // skip zero length elements

    if(verbose>1)
    {
      cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << " loop over elements previous i0=" << setw(4) << i0 << " current i=" << setw(4) << i
      << " " << setw(15) << aper[i].Name << " " << setw(10) << aper[i].Keyword;
      if(i!=il) cout << " aper[il].a_1=" << setw(6) << aper[il].a_1;
      if(i!=ir) cout << " aper[ir].a_1=" << setw(6) << aper[ir].a_1;
      cout << '\n';
    }
    if( aper[ir].a_1<=0 || aper[ir].a_1>=apermax) continue; // require reasonable aperture between 0 and 5 m,  already fixed by Connect, still check here when not using connect

    double zpos      =0.5*(zvec[i]  +zvec[i0]);
    double xpos      =0.5*( x_2[i]   +x_2[i0]);
    double ypos      =0.5*( y_2[i]   +y_2[i0]);
    double phi_ave   =0.5*( phi[i]   +phi[i0]);
    double psi_ave   =0.5*( psi[i]   +psi[i0]);
    double theta_ave =0.5*(theta[i]+theta[i0]);
    double theta_diff=0.5*(theta[i]-theta[i0]);
    double ds        =0.5*(svec[i]  -svec[i0]); // distance from last reasonable to next reasonable aperture
    double dz        =0.5*(zvec[i]  -zvec[i0]); // distance from last reasonable to next reasonable aperture
    double dl        =0.5*lvec[i];
    if(theta_diff<-M_PI_2) theta_diff+=M_PI; // where M_PI_2  is  pi/2  defined in math.h
    if(theta_diff> M_PI_2) theta_diff-=M_PI;
    dl*=cos(theta_diff); // avoid overlap from rotation, see   ~/tex/plotting/Geometry.tex
    if(Is_Ring && ds<0)  // for a ring, make sure ds is positive, add circumference if needed
    {
      ds =0.5*(smax+svec[i]-svec[i0]);
      if(verbose>1) cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << " svec[i]-svec[i0]=" << svec[i]-svec[i0] << " close the ring, add the circumference smax=" << smax << " ds=" << ds << '\n';
    }
    if(verbose>1) cout << left << setw(25) << aper[i].Name << " i=" << setw(3) << i << " i0=" << setw(3) << i0
      << " at s=" << setw(10) << svec[i] << setprecision(3) << " ds=" << setw(6) << ds
      << " dl="   << setw(6) << dl
      << " 0.5*lvec[i]=" << setw(6) << 0.5*lvec[i]
      << " at z="  << setw(6) << zvec[i] << " dz=" << setw(6) << dz
      << " theta_ave="  << setw(6) << theta_ave
      << " theta[i0]="  << setw(6) << theta[i0]
      << " theta[i]="   << setw(6) << theta[i]
      << " theta_diff=" << setw(6) << theta_diff
      << " angle/2="    << angle[i]/2 // bending angle relevant for dipoles, should be same as theta_diff
      << " cos theta diff =" << setw(6) << cos(theta_diff) // reduce length by this amount to avoid overlap,  see   ~/tex/plotting/Geometry.tex
      << setprecision(6) << '\n';

    if(z0<-1.e20) z0=zvec[i0]; // store the first z position used

    if(fabs(x_2[i]) > zmax)
    {
      if(verbose>1)
      {
        cout << "x2 large, skip "<< setw(25) << aper[ir].Name << " i=" << setw(3) << i << " i0=" << setw(3) << i0
        << " at s=" << setw(10) << svec[i] << " x_2[i]=" << x_2[i] << '\n';
      }
      continue;
    }

    if(ds>zmax-zmin)
    {
      cout << " *** warning *** big jump ds=" << ds << " looks like jumping to different side of ring -- no volume made here "
      << setw(25) << aper[i].Name << " i=" << setw(3) << i << " i0=" << setw(3) << i0
      << " at s=" << setw(10) << svec[i] << setprecision(3) << " ds=" << setw(6) << ds
      << " dl="   << setw(6) << dl
      << " 0.5*lvec[i]=" << setw(6) << 0.5*lvec[i]
      << " at z="  << setw(6) << zvec[i] << " dz=" << setw(6) << dz
      << " theta_ave=" << setw(6) << theta_ave
      << setprecision(6) << '\n' << '\n';
      i0=i; // last with reasonable aperture
      slast=svec[i];
      continue;
    }

    if(Scale_xy>1)
    {
      dl      *=sqrt(1.+ Scale_xy*theta_ave * Scale_xy*theta_ave); // dirty fix, increase length for Scale_xy
      theta_ave=atan(Scale_xy*theta_ave);
    }

    if(fabs(ds)<0.001) // http://www.cplusplus.com/reference/cmath/fabs/
    {
      if(verbose) cout << " i=" << setw(3) << i << setw(20) << aper[i].Name << "     at s=" << setw(10) << svec[i] << " has half dist to next valid aperture at s=" << setw(10) << svec[i] << " of ds=" << setw(10) << ds << " below 0.001 m and is ignored" << '\n';
    }
    else // enough distance
    {
      if(slast!=svec[i] && verbose>1) cout << "line=" << __LINE__ << " slast=" << setw(10) << slast
        << " svec[i0]=" << setw(10) << svec[i0] << " svec[i]=" << setw(10) << svec[i]
        << " zvec[i0]=" << setw(10) << zvec[i0] << " zvec[i]=" << setw(10) << zvec[i]
        << '\n';

      // IR8
      // two beams start right b1 at  MBX.4R8   (end of D1 right)       s = 67.83300046   to     MBRC.4R8.B1                        131.1280005 m  end of D2 right
      //                              MBX.4L8 s= 26658.8832 - 26600.5002 =  58.383 m,   "MBRC.4L8.B1 s = 26658.8832 -  26537.2052 = 121.678 m
      // still next should also work for IP8, since in both cases z < <143.  together with  fabs(xpos)<0.0798  seems ok,  Drift_921 right  at  s =  82.9255  in b1 looks strange but seems true different TDI acceptance b1

      if(Is_LHC && xpos !=0 ) //--- special fixes for some LHC apertures
      {
        // next is really special for LHC IP1 & IP5 - apertures define with respect to average beam - actually Courant Snyder until TAN
        if(fabs(zvec[i])<143. && fabs(xpos)<0.0798 && fabs(xpos) < aper[ir].a_1 ) // special LHC, between D1 and D2, transition between 1 and 2 beam pipes  in TAN at a separation of about 0.079
        {
          if(verbose) cout << "special LHC b1-b2 transition at zvec[i]=" << setw(12) << zvec[i] << " reset xpos=" << setw(12) << xpos << " " << aper[i].Print() << '\n';
          xpos=0;
          theta_ave=0;
        }
      }

      TGeoVolume* CurVol=MyVolume(medFe,aper[i],aper[il]*Scale_xy,aper[ir]*Scale_xy, dl,thick*Scale_xy,verbose); // Make new volume, with shape depending on Apertype, left/right Apertures
      if(verbose>1) cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << " after CurVol=MyVolume" << '\n';
      
      // now position the new CurVol
      if(verbose)
      {
        cout << " i=" << setw(5) << i << " position "<< setw(20) << aper[i].Name << " from z=" << setw(10) << zvec[i0] << " to " << setw(10) << zvec[i] << " dl=" << setw(10) << dl
        << " a_0=" << setw(10) << aper[i0].a_1
        << " a_1=" << setw(10) << aper[i].a_1
        << " xpos=" << setw(10) << xpos
        << " ypos=" << setw(10) << ypos
        << " x_2[i]=" << setw(10) << x_2[i];
        if(fabs(theta_ave)>eps) cout << " (scaled) theta_ave=" << setw(10) << theta_ave;
        cout << " phi_ave=" << setw(10) << phi_ave
        << " psi_ave=" << setw(10) << psi_ave
        << '\n';
      }

      if(strncmp(aper[i].Name.c_str(),StartElementName.c_str(),StartElementName.length())==0) // Change color for element matching StartElementName http://www.cplusplus.com/reference/cstring/strncmp/
      {
        double z_IP=zvec[i];
        cout << "Element i=" << i << " " << aper[i].Name << " matches StartElementName=" << StartElementName << " z_IP=" << z_IP << " nt.Noent()=" << nt.Noent() << '\n';
        CurVol->SetLineColor(kMagenta); // change color when matching StartElementName
      }

      // now position the volume --- by translation and rotation   using the average (scaled) survey coordinates
      if(Subtract_z0) zpos-=z0;
      TGeoTranslation trans(Scale_xy*xpos*cm,Scale_xy*ypos*cm,zpos*cm); // scale translation
      TGeoHMatrix *CurPos=NULL;
      if(fabs(theta_ave)>eps || fabs(phi_ave)>eps || fabs(psi_ave)>eps ) // non negligible rotation
      {
        TGeoRotation rot(WCS_root(theta_ave,phi_ave,psi_ave));
        CurPos= new TGeoHMatrix(TGeoCombiTrans(trans, rot)); // translation and rotation
      }
      else CurPos = new TGeoHMatrix(trans);    // just translation
      if(verbose>1)
      {
        cout << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << " theta_ave=" << theta_ave << " CurPos=" << CurPos << " CurVol=" << CurVol << '\n';
        cout << "CurVol->Print()="; CurVol->Print();
        cout << "CurPos->Print()="; CurPos->Print();
      }
      // CurVol->SetTransparency(70); // 0 - 100,   0 is no transparency,   100  is completely transparent (invisible)
      top->AddNode(CurVol,1,CurPos);
      if(verbose)   { cout << '\n' << "new volume created and added as node "; CurVol->Print(); }
      if(verbose>1) { cout << " gGeoManager->CheckGeometry() "; gGeoManager->CheckGeometry(); cout << " CheckGeometry() done " << '\n'; };
      if(verbose>1) { double precision=1.e-6; cout << '\n' << " CurVol->CheckOverlaps "; CurVol->CheckOverlaps(precision); cout << " CurVol->CheckOverlaps done " << '\n' << '\n'; };
    }  // enough distance
    i0=i; // last with reasonable aperture
    slast=svec[i];
  } // --- main loop over elements and construct volumes

  //CSPE top->Draw("ogl");  // CSPE debug
  if(verbose) cout << " line " << __LINE__ << " now top->Print() "; top->Print();
  if(verbose>1) { cout << " GeoManager->PrintOverlaps() "; gGeoManager->PrintOverlaps(); cout << " GeoManager->PrintOverlaps() done" << '\n';}

  cout << '\n' << " done with " << __FILE__<< " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << '\n';
}
