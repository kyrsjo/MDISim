//  ~/c/MDISim/Apertures.C     H. Burkhardt 5/2013

#ifndef __CLING__
#include "MyCppStdHeaders.H"
#include <TROOT.h>
#include <TLine.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#endif

#include <climits>
#include "MyRootConst.h"
#include "Apertures.h"

// simple utility routines to move end of vector to beginning

vector<string> my_CombineStr(const vector<string> InpStr,unsigned int ileft,unsigned int iright) // string
{
  unsigned verbose=0;
  vector<string> result;
  if(verbose) cout << "my_CombineStr start InpStr.size()=" << InpStr.size() << " ileft=" << ileft << '\n';
  if(InpStr.size()>ileft)
  {
    for(unsigned int i=ileft;i<InpStr.size();++i) result.push_back(InpStr[i]);
    for(unsigned int i=0;i<iright;++i) result.push_back(InpStr[i]);
  }
  if(verbose) cout << "my_CombineStr end result.size()=" << result.size() << '\n';
  return result;
}

vector<double> my_CombineDou(const vector<double> InpDou,unsigned int ileft,unsigned int iright) // same for double
{
  unsigned verbose=0;
  vector<double> result;
  if(verbose) cout << "my_CombineDou start InpStr.size()=" << InpDou.size() << " ileft=" << ileft << '\n';
  if(InpDou.size()>ileft)
  {
    for(unsigned int i=ileft;i<InpDou.size();++i) result.push_back(InpDou[i]);
    for(unsigned int i=0;i<iright;++i) result.push_back(InpDou[i]);
  }
  if(verbose) cout << "my_CombineDou end result.size()=" << result.size() << '\n';
  return result;
}

Apertures::Apertures(const Ntuple* nt,unsigned int verbose) // constructor
// 1st cleanup the aperture information directly from madx tfs and keep the result as vector<double> svec, aper_1_vec;   --- more cleanup possible later
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " constructor start verbose=" << verbose << " nt->Noent()=" << nt->Noent() << '\n';

  // copy to local const valarray's
  const valarray<double>      s_array=nt->GetVar("S");        // http://www.cplusplus.com/reference/valarray/
  const valarray<double> aper_1_array=nt->GetVar("APER_1");
  const valarray<double> aper_2_array=nt->GetVar("APER_2");
  const valarray<double> aper_3_array=nt->GetVar("APER_3");
  const valarray<double> aper_4_array=nt->GetVar("APER_4");
  bool has_betx=nt->VarExists("BETX");
  bool has_bety=nt->VarExists("BETY");
  valarray<double> betx_array(nt->Nvar()),bety_array(nt->Nvar());
  if(has_betx) betx_array=nt->GetVar("BETX");
  if(has_bety) bety_array=nt->GetVar("BETY");
  const valarray<double>      z_array=nt->GetVar("Z");
  const valarray<double>      y_array=nt->GetVar("Y");   // y directly
  const valarray<double>    x_2_array=nt->GetVar("X_2"); // x_2  is x from survey in global Euclidian coordinates

  const valarray<double> aper_x_n = aper_1_array / sqrt(betx_array); // normalized aperture in x           not working correctly in CINT
  const valarray<double> aper_y_n = aper_1_array / sqrt(bety_array); // normalized aperture in y           not working correctly in CINT

  cout << std::defaultfloat;

  bool HasX2=nt->VarExists("X_2");
  if(verbose>2)
  {
    cout
    << " aper_x_n.size()=" << aper_x_n.size()
    << " aper_1_array[0]=" << aper_1_array[0]
    << " betx_array[0]=" << betx_array[0]
    << " aper_x_n[0]=" << aper_x_n[0]
    << '\n';
    if(HasX2) cout << "X_2 exists" << '\n';
  }

  this->verbose=verbose;
  if(verbose>1)
  {
    cout << "Apertures constructor"
    << " s_array.size()=" << s_array.size()
    << " aper_1_array.size()=" << aper_1_array.size() << '\n';
  }
  double aper_x_n_min=numeric_limits<double>::max();
  double aper_y_n_min=numeric_limits<double>::max();
  isigx_min=0;
  isigy_min=0;
  vector<string> nt_StrCol_Name=nt->GetStrCol("NAME");
  vector<string> nt_StrCol_Keyw=nt->GetStrCol("KEYWORD");
  vector<string> nt_StrCol_Type=nt->GetStrCol("APERTYPE");
  if(verbose) cout
    << " nt_StrCol_Name.size()=" << nt_StrCol_Name.size()
    << " nt_StrCol_Keyw.size()=" << nt_StrCol_Keyw.size()
    << " nt_StrCol_Type.size()=" << nt_StrCol_Name.size()
    << '\n';
  // verbose=3; // CSPE
  for(unsigned int j=0;j<s_array.size();++j)
  {
    bool AperOK=aper_1_array[j]>0 && aper_1_array[j]<5;
    if( AperOK ) // reasonable between 0 and 5 m, aperture information available
    {
      AperName.push_back(nt_StrCol_Name[j]); // The name should always be defined
      if(j<nt_StrCol_Keyw.size()) AperKeyw.push_back(nt_StrCol_Keyw[j]);
      if(j<nt_StrCol_Type.size()) AperType.push_back(nt_StrCol_Type[j]);
      svec.push_back( s_array[j]);
      yvec.push_back( y_array[j]);
      aper_1_vec.push_back( aper_1_array[j]);
      if(aper_2_array.size()>0) aper_2_vec.push_back( aper_2_array[j]);
      if(aper_3_array.size()>0) aper_3_vec.push_back( aper_3_array[j]);
      if(aper_4_array.size()>0) aper_4_vec.push_back( aper_4_array[j]);
      if(HasX2)
      {
        zvec.push_back(z_array[j]);
        x_2_vec.push_back(x_2_array[j]);
      }
      else
      {
        zvec.push_back(0);
        x_2_vec.push_back(0);
      }
      if( aper_x_n[j]<aper_x_n_min )
      {
        aper_x_n_min=aper_x_n[j];
        isigx_min=j;
        if(verbose>2) cout << "new aper_x_n_min=" << aper_x_n_min << " at isigx_min=" << isigx_min << '\n';
      }
      if( aper_y_n[j]<aper_y_n_min )
      {
        aper_y_n_min=aper_y_n[j];
        isigy_min=j;
        if(verbose>2) cout << "new aper_y_n_min=" << aper_y_n_min << " at isigy_min=" << isigy_min << '\n';
      }
    }
  }
  if(verbose)
  {
    cout << "isigx_min=" << isigx_min << " isigy_min=" << isigy_min << '\n';
    cout << "aper_x_n_min=" << aper_x_n_min << "/sqrt(m) at s=" << s_array[isigx_min] << " " << nt_StrCol_Name[isigx_min] << '\n';
    cout << "aper_y_n_min=" << aper_y_n_min << "/sqrt(m) at s=" << s_array[isigy_min] << " " << nt_StrCol_Name[isigy_min] << '\n';
    cout << "Apertures constructor end svec.size()=" << svec.size() << '\n';
  }
}

void Apertures::CheckConsistence()
{
  unsigned int ierr=0;
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " Noent()=" << Noent() << '\n';
  // next must have full size
  if(AperName.size() != Noent())   { ierr ++; cout << " *** error AperName.size()=" << AperName.size() << " is not equal to Noent()=" << Noent() << '\n'; }
  if(zvec.size() != Noent())       { ierr ++; cout << " *** error zvec.size()=" << zvec.size() << " is not equal to Noent()=" << Noent() << '\n'; }
  if(yvec.size() != Noent())       { ierr ++; cout << " *** error yvec.size()=" << yvec.size() << " is not equal to Noent()=" << Noent() << '\n'; }
  if(aper_1_vec.size() != Noent()) { ierr ++; cout << " *** error aper_1_vec.size()=" << aper_1_vec.size() << " is not equal to Noent()=" << Noent() << '\n'; }
  if(x_2_vec.size() != Noent())    { ierr ++; cout << " *** error x_2_vec.size()=" << x_2_vec.size() << " is not equal to Noent()=" << Noent() << '\n'; }
  // next must have full size or 0
  if(AperKeyw.size() > 0 && AperKeyw.size() != Noent())     { ierr ++; cout << " *** error AperKeyw.size()=" << AperKeyw.size() << " is > 0 and not equal to Noent()=" << Noent() << '\n'; }
  if(AperType.size() > 0 && AperType.size() != Noent())     { ierr ++; cout << " *** error AperType.size()=" << AperType.size() << " is > 0 and not equal to Noent()=" << Noent() << '\n'; }
  if(aper_2_vec.size() > 0 && aper_2_vec.size() != Noent()) { ierr ++; cout << " *** error aper_2_vec.size()=" << aper_2_vec.size() << " is > 0 and not equal to Noent()=" << Noent() << '\n'; }
  if(aper_3_vec.size() > 0 && aper_3_vec.size() != Noent()) { ierr ++; cout << " *** error aper_3_vec.size()=" << aper_3_vec.size() << " is > 0 and not equal to Noent()=" << Noent() << '\n'; }
  if(aper_4_vec.size() > 0 && aper_4_vec.size() != Noent()) { ierr ++; cout << " *** error aper_4_vec.size()=" << aper_4_vec.size() << " is > 0 and not equal to Noent()=" << Noent() << '\n'; }
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " end ierr=" << ierr << '\n';
  if(ierr) exit(ierr);
}

void Apertures::RemoveEntry(unsigned int ient)
{
  if(ient<Noent())
  {
    AperName.erase (AperName.begin()+ient);
    svec.erase (svec.begin()+ient);
    zvec.erase (zvec.begin()+ient);
    yvec.erase (yvec.begin()+ient);
    x_2_vec.erase (x_2_vec.begin()+ient);
    aper_1_vec.erase (aper_1_vec.begin()+ient);
    if(AperKeyw.size()>0) AperKeyw.erase (AperKeyw.begin()+ient);
    if(AperType.size()>0) AperType.erase (AperType.begin()+ient);
    if(aper_2_vec.size()>0) aper_2_vec.erase (aper_2_vec.begin()+ient);
    if(aper_3_vec.size()>0) aper_3_vec.erase (aper_3_vec.begin()+ient);
    if(aper_4_vec.size()>0) aper_4_vec.erase (aper_4_vec.begin()+ient);
  }
}

void Apertures::CleanUpApertures()
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " Noent()=" << Noent() << '\n';
  unsigned int Noent_prev=Noent();

  unsigned int iprinted=0;
  unsigned int PriMax=20;
  if(verbose>1) PriMax=100;

  unsigned int nRedundant=0;
  for(unsigned int i=0;i<Noent()-1;++i) // remove redundant values     same s and aper_1
  {
    if(svec[i+1]<=svec[i] && aper_1_vec[i]==aper_1_vec[i+1] )
    {
      if(i<PriMax)
      {
        cout << " s does not increase, and same aperture remove entry i+1=" << i+1 << '\n';
        Print(i,i+1);
      }
      RemoveEntry(i+1);
      nRedundant++;
    }
    iprinted++;
  }
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " after deleting " << Noent_prev-Noent() << " redundant apertures Noent()=" << Noent() << '\n';
  Noent_prev=Noent();

  unsigned int nFixOldIP=0;
  for(unsigned int i=0;i<Noent()-1;++i) // special fix for old beam pipe IP marker sizes
  {
    if(StartsWith(AperName[i],"IP"))
    {
      if(aper_1_vec[i]>aper_1_vec[i+1])
      {
        cout << " work on entry with AperName[" << i << "]=" << AperName[i] << '\n';
        Print(i,i+1);
        cout << " set historic larger value " << aper_1_vec[i] << " to the smaller new value " << aper_1_vec[i+1] << '\n';
        aper_1_vec[i] = aper_1_vec[i+1];
        nFixOldIP++;
      }
    }
  }

  iprinted=0;
  unsigned int nRectEll2Circle=0;
  unsigned int nRectEll2Ell=0;
  if(AperKeyw.size()>0)
  {
    for(unsigned int i=0;i<Noent()-1;++i) // simplify RECTELLIPSE which are in fact CIRCLES
    {
      if(AperType[i]=="RECTELLIPSE")
      {
        if(aper_1_vec[i]==aper_2_vec[i] && aper_1_vec[i]==aper_3_vec[i] && aper_3_vec[i]==aper_4_vec[i])  // all equal, this is a circle
        {
          if(i<PriMax)
          {
            cout << " simplify AperName[" << i << "]=" << AperName[i] << '\n';
            Print(i,i);
          }
          AperType[i]="CIRCLE";
          aper_2_vec[i]=aper_3_vec[i]=aper_4_vec[i]=0;
          nRectEll2Circle++;
          if(i<PriMax)
          {
            cout << "  to circle" << '\n';
            Print(i,i);
          }
          iprinted++;
        }
        // check if there are rectangle which are larger than the ellipse, simplify to ELLIPSE
        else if(aper_1_vec[i]>=aper_3_vec[i] && aper_2_vec[i]>=aper_4_vec[i])
        {
          if(i<PriMax)
          {
            cout << " simplify AperName[" << i << "]=" << AperName[i] << '\n';
            Print(i,i);
          }
          AperType[i]="ELLIPSE";
          aper_1_vec[i]=aper_3_vec[i];
          aper_2_vec[i]=aper_4_vec[i];
          aper_3_vec[i]=aper_4_vec[i]=0;
          nRectEll2Ell++;
          if(i<PriMax)
          {
            cout << "  to ellipse" << '\n';
            Print(i,i);
          }
          iprinted++;
        }
      }
    }
  }
  cout << "done with CleanUpApertures"
  << " nRedundant=" << nRedundant
  << " nFixOldIP=" << nFixOldIP
  << " nRectEll2Circle=" << nRectEll2Circle
  << " nRectEll2Ell=" << nRectEll2Ell
  << " Noent()=" << Noent()
  << '\n';
}

void Apertures::CombineLeftRight(const double smax)
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " Noent()=" << Noent() << '\n';
  unsigned int ileft=Noent()-1;
  double slast=svec[ileft];
  // or maybe better go to +/- smax from IP
  while(ileft>0 && abs(slast-svec[ileft])<smax) ileft--; cout << " ileft=" << ileft << '\n'; Print(ileft,ileft);
  unsigned int iright=0;
  while(iright<Noent() && svec[iright]<smax) iright++; cout << " iright=" << iright << '\n'; Print(iright,iright);
  if(iright<Noent()) iright++; // one more to have one point just outside
  if(verbose)
  {
    cout << " move " << Noent()-ileft << " entries ileft=" << ileft << " to Noent()=" << Noent() << " to the beginning, subtract slast=" << slast << " and go to iright=" << iright << " on the right side" << '\n';
  }
  for(unsigned int i=ileft;i<Noent();++i) svec[i]=svec[i]-slast;
  AperName=my_CombineStr(AperName,ileft,iright);
  AperKeyw=my_CombineStr(AperKeyw,ileft,iright);
  AperType=my_CombineStr(AperType,ileft,iright);
  svec=my_CombineDou(svec,ileft,iright);
  zvec=my_CombineDou(zvec,ileft,iright);
  yvec=my_CombineDou(yvec,ileft,iright);
  x_2_vec=my_CombineDou(x_2_vec,ileft,iright);
  aper_1_vec=my_CombineDou(aper_1_vec,ileft,iright);
  aper_2_vec=my_CombineDou(aper_2_vec,ileft,iright);
  aper_3_vec=my_CombineDou(aper_3_vec,ileft,iright);
  aper_4_vec=my_CombineDou(aper_4_vec,ileft,iright);
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " end verbose=" << verbose << " Noent()=" << Noent() << '\n';
}

void Apertures::PlotFrame(double xmin,double xmax,double ymin,double ymax)
{
  Int_t wtopx=500,wtopy=60; // hbu position on screen, a bit shifted to right
  // unsigned int kSizePt=700;
  unsigned int kSizePt=1000; // size in points   here used for both directions, to get square
  c2=new TCanvas("c2","PlotFrame",wtopx,wtopy,kSizePt,kSizePt); // kSizePt set in rootlogon.C

  Float_t xsize=20,ysize=20;	// standard 20 cm * 20 cm window
  c2->Size(xsize,ysize); // set size for Postscript

  gStyle->SetLabelSize(k12pt,"xyz");  // axis numbers text size

  // keep the frame boundaries for possible later use
  this->xmin=xmin;
  this->xmax=xmax;
  this->ymin=ymin;
  this->ymax=ymax;

  if(verbose>1) cout << "Apertures::Plot_tfs" << '\n';
  double xrange=500;
  TH2F* hpx = new TH2F("s","",10,xmin,xmax,10,ymin,ymax);
  hpx->SetStats(kFALSE); // no statistics
  hpx->GetXaxis()->SetTitleOffset(1.3); // more space between numbers and x-axis title
  hpx->GetYaxis()->SetTitleOffset(1.3); // more space between numbers and y-axis title
  hpx->GetXaxis()->SetTitle("z[m]");
  hpx->GetYaxis()->SetTitle("x,y[m]");  hpx->Draw(); // after this hpx not needed any more
  hpx->Draw();

  TLine* ZeroYLine=new TLine(-xrange,0,xrange,0); ZeroYLine->Draw(); // draw a horizontal line a y = 0
  TLine* ZeroXLine=new TLine(0,-0.01,0,0.01);     ZeroXLine->Draw(); // draw a vertical   line a x = 0
}

Ntuple* Apertures::MakeNtuple() // make a new ntuple with the cleaned re-arranged aperture information
{
#if __cplusplus >= 201103L // C++11 on
  vector <string> tags={"NAME","KEYWORD","APERTYPE","X_2","Y","Z","S","APER_1","APER_2","APER_3","APER_4"}; // C++11
#else
  const char *vinit[] ={"NAME","KEYWORD","APERTYPE","X_2","Y","Z","S","APER_1","APER_2","APER_3","APER_4"};
  vector <string> tags(11);
  for(unsigned int i=0;i<11;++i) tags[i]=vinit[i];
#endif
  unsigned int nvar=tags.size();
  Ntuple* nt=new Ntuple(tags,&cout);
  nt->DefineAsString(0);
  nt->DefineAsString(1);
  nt->DefineAsString(2);
  valarray <double> Dat(nvar);
  vector<string> TextVec(nvar);
  for(unsigned int i=0;i<Noent();++i)
  {
    TextVec[0]=AperName[i];
    TextVec[1]=AperKeyw[i];
    TextVec[2]=AperType[i];
    Dat[3] =x_2_vec[i];
    Dat[4] =yvec[i];
    Dat[5] =zvec[i];
    Dat[6] =svec[i];
    Dat[7] =aper_1_vec[i];
    Dat[8] =aper_2_vec[i];
    Dat[9] =aper_3_vec[i];
    Dat[10]=aper_4_vec[i];
    nt->fill(Dat,TextVec);
  }
  return nt;
}

void Apertures::Print(unsigned int ientr1,unsigned int ientr2,unsigned int PriMax) // print ientr,    or all if ientr not given or outside range
{
  if(verbose) cout << __FILE__ << " " << __PRETTY_FUNCTION__ << " line " << __LINE__ << " start verbose=" << verbose << " Noent()=" << Noent() << " ientr1=" << ientr1 << " ientr2=" << ientr2 << '\n';
  if(ientr1>=Noent())
  {
    cout << " *** warning Apertures::Print ientr1=" << ientr1 << " should be less then Noent()=" << Noent() << '\n';
    return;
  }
  cout << "AperName            AperKeyw            AperType            s           z           x_2         aper_1" << '\n';
  // unsigned int imax=min((unsigned int)AperName.size(),PriMax); // CINT does not like that
  if(ientr2>=Noent()) ientr2=Noent();
  string EmptyString="";
  unsigned int iprinted=0;
  for(unsigned int i=ientr1;;++i) // print at least one entry
  {
    if(i>=Noent()) break;
    if(i>ientr2) break;
    string EmptyString;

    cout << left << setw(20) << AperName[i];  // in CINT may be printed as address, seems just a problem of CINT, ok in line mode
    if(i<AperKeyw.size()) cout << setw(20) << AperKeyw[i]; else cout << setw(20) << EmptyString;
    if(i<AperType.size()) cout << setw(20) << AperType[i]; else cout << setw(20) << EmptyString;
    cout
    << setw(12) << svec[i]
    << setw(12) << zvec[i]
    << setw(13) << x_2_vec[i]
    << setw(8) << aper_1_vec[i];
    if(i<aper_2_vec.size()) cout << setw(8) << aper_2_vec[i] ;
    if(i<aper_3_vec.size()) cout << setw(8) << aper_3_vec[i] ;
    if(i<aper_4_vec.size()) cout << setw(8) << aper_4_vec[i] ;
    cout << '\n';
    iprinted++;
    if(iprinted>PriMax) break;
  }
}
