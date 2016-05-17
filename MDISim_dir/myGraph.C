/*  ~/c/MDISim/myGraph.C

 */

#ifndef __CLING__
#include "MyCppStdHeaders.H"
#endif
#include "myGraph.h"

TGraph* MyBeamGraph(const unsigned int n,unsigned int linecolor,const valarray<double>& zvec,const valarray<double>& x_2) // just beam, works also when no aperture defined
{
  //--- for plotting use x vs y
  double x[n];
  double y[n];

  // plot beam passage
  TGraph* gr=new TGraph(n,x,y);
  gr->SetLineColor(linecolor);
  for(unsigned int i=0;i<n;++i) x[i]= zvec[i];
  for(unsigned int i=0;i<n;++i) y[i]=x_2[i]; // x_2  is x from survey in global Euclidian coordinates
  gr->DrawGraph(n,x,y,"LSAME");

  return gr;
}

TGraph* MyApertureGraph(const unsigned int n,unsigned int linecolor,const vector<double>& zvec,const vector<double>& x_2,const vector<double>& aper) // with clipping
{
  // x_2 is the x coordinate of the beam from survey    use zero in when plotting y

  //--- for plotting use x vs y
  double x[n];
  double y[n];

  // plot beam passage
  TGraph* gr=new TGraph(n,x,y);
  gr->SetLineColor(linecolor);
  for(unsigned int i=0;i<n;++i) x[i]= zvec[i];
  for(unsigned int i=0;i<n;++i) y[i]=x_2[i];
  gr->DrawGraph(n,x,y,"LSAME");

  // plot positive aperture      beam+aper
  for(unsigned int i=0;i<n;++i) x[i]=zvec[i];
  for(unsigned int i=0;i<n;++i) y[i]=x_2[i]+aper[i];
  gr->DrawGraph(n,x,y,"LSAME");

  // plot negative aperture      beam-aper
  for(unsigned int i=0;i<n;++i) x[i]= zvec[i];
  for(unsigned int i=0;i<n;++i) y[i]=x_2[i]-aper[i];
  gr->DrawGraph(n,x,y,"LSAME");

  return gr;
}

TGraph* MyTrackGraph(const unsigned int nn,const valarray<double>& svec,const valarray<double>& xy,unsigned int linecolor,unsigned int verbose) // with clipping, use when no survey, called from  main_TwissTrack_tfs.C
{

  double xt[nn],yt[nn];

  if(verbose>1) cout << "MyTrackGraph nn=" << nn << " svec.size()=" << svec.size() << " xy.size()=" << xy.size() << '\n';

  for(unsigned int i=0;i<nn;++i) xt[i]=svec[i];
  for(unsigned int i=0;i<nn;++i) yt[i]=xy[i];

  TGraph* grt=new TGraph(nn,xt,yt);
  grt->SetLineColor(linecolor);
  grt->Draw("LSAME");

  return grt;
}
