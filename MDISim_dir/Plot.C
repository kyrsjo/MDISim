// ~/c/MDISim/Plot.C

#ifndef __CLING__
#include "MyCppStdHeaders.H"
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TText.h>
#include <TVector.h>
#include <TH2F.h>
#include <TGraph.h>
#endif

#include "NtupleRoot.h" // My_Labels_Graph
#include "MyRoot.h"     // to_TVectorD
#include "MyRootConst.h"
#include "Lorentz.h"
#include "Twiss_util.h"
#include "Apertures.h"
#include "IPpos.h"
#include "myGraph.h"
#include "Plot.h"

void PlotNames(Ntuple* nt,const vector<unsigned int>& elemList,double xymax,unsigned int verbose) // PlotNames of elements in elemList
{
  if(verbose>1) cout << "PlotNames start verbose=" << verbose << " elemList.size()=" << elemList.size() << " xymax=" << xymax << '\n';
  TText* tt0=new TText();
  tt0->SetTextFont(kTimes);
  // tt0->SetTextSize(k12pt);
  tt0->SetTextSize(k7pt);
  tt0->SetTextAlign(12); // left adjusted and vertically centered
  tt0->SetTextAngle(90); // rotate text by 90 degrees

  unsigned int is=nt->ivar("S");
  unsigned int iz=nt->ivar("Z");
  unsigned int ibetx=nt->ivar("BETX");
  unsigned int ibety=nt->ivar("BETY");
  vector<string> nt_StrCol=nt->GetStrCol("NAME");
  unsigned int PriMax=20;
  if(verbose>1) PriMax=100;
  bool HasZ=nt->VarExists("Z");
  for(unsigned int i=0;i<elemList.size();++i)
  {
    unsigned int ient=elemList[i];
    if(ient<nt->Noent())
    {
      double z =nt->Entry(ient,is);
      if(HasZ) z =nt->Entry(ient,iz);
      double betx=nt->Entry(ient,ibetx);
      double bety=nt->Entry(ient,ibety);
      //old string NameWithoutQuotes=nt_StrCol[ient];
      if(verbose && i<PriMax) cout << "PlotNames Element " << setw(7) << ient << " is " << setw(14) << nt_StrCol[ient] << " at z=" << setw(12) << z
        << " BETX=" << setw(10) << betx
        << " BETY=" << setw(10) << bety
        << " m"
        << '\n';
      tt0->DrawText(z,1.02*xymax,nt_StrCol[ient].c_str());
    }
    else
    {
      cout << "PlotNames *** error *** ient=" << ient << " > Noent=" << nt->Noent() << '\n';
    }
  }
}

void Plot_optics(Ntuple* nt,unsigned int verbose,double smin,double smax)
{
//#ifndef __CLING__
//  TCanvas *c1 = new TCanvas("c1","Root Canvas 1");
//#endif

  cout << CPUTime() << '\n';
  if(verbose>1) cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " verbose=" << verbose << '\n';
  cout << CPUTime() << '\n';

  // example how to select and plot vectors
  valarray<double> S   =nt->GetVar("S"); if(smax>S.max()) smax=S.max();    // http://www.cplusplus.com/reference/valarray/     make sure maximum does not exceed the machine size
  valarray<double> BETX   =nt->GetVar("BETX");
  valarray<double> BETY   =nt->GetVar("BETY");
  TGraph* grx = new TGraph(to_TVectorD(S), to_TVectorD(BETX)); grx->SetLineColor(kGreen);
  TGraph* gry = new TGraph(to_TVectorD(S), to_TVectorD(BETY)); gry->SetLineColor(kRed);


  //find the maximum betas in the s-range
  double betamin=0,betamax=0;
  for(unsigned int i=0;i<S.size();++i)
  {
	if(S[i]>=smin && S[i]<=smax)
	{
	  if(BETX[i]<betamin) betamin=BETX[i];
	  if(BETY[i]<betamin) betamin=BETY[i];
	  if(BETX[i]>betamax) betamax=BETX[i];
	  if(BETY[i]>betamax) betamax=BETY[i];
	}
  }
  TH2F* hpx = new TH2F("","",10,smin,smax,10,betamin,betamax); // fixed frame
  hpx->SetTitle("");     // ""  no title
  hpx->SetStats(kFALSE); // no statistics
  hpx->GetXaxis()->SetTitle("s [m]");
  hpx->GetYaxis()->SetTitle("beta x,y [m]");
  hpx->GetYaxis()->SetTitleOffset(1.3); // more space between numbers and y-axis title
  hpx->Draw(); // after this hpx not needed any more
  // gPad->SetLogy(1); ymin=1.e-2 ;// only works after first plot
  // hpx->Draw(); // after this hpx not needed any more
  grx->Draw("LSAME");
  gry->Draw("LSAME");
  // gPad->SetLogy(0);
}

void Plot_survey(Ntuple* nt,unsigned int verbose,double xmin,double xmax,double zmin,double zmax)
{
  // unsigned int verbose=2; // locally increase verbose
  IPpos *ips_b1=new IPpos(nt,verbose); // in Apertures.C
  ips_b1->Print();

  double xip=0,zip=0;

  if(verbose>1) cout << "Plot_survey ips_b1->IPVec.size()=" << ips_b1->IPVec.size() << '\n';

  if(ips_b1->IPVec.size()>0)
  {
	unsigned int IP=1;
    unsigned int IPind=ips_b1->ind(IP);
    xip=ips_b1->x[IPind];
    zip=ips_b1->z[IPind];
    cout << "ips_b1->ind(IP)=" << IPind
    << " xip=" << xip
    << " zip=" << zip
    << '\n';
  }


  if(xmin==0 && xmax==0 && zmin==0 && zmax==00)
  { // all limites zero,  plot the full ring
    xmin=1.1*nt->Min("X_2");
    xmax=1.1*nt->Max("X_2");
    zmin=1.1*nt->Min("Z");
    zmax=1.1*nt->Max("Z");
  }
  else // plot around IP with limits given
  {
    xmin=xip+xmin;
	xmax=xip+xmax;
	zmin=zip+zmin;
	zmax=zip+zmax;
    cout << " zmin=" << zmin << " zmax=" << zmax << '\n';
    cout << " xmin=" << xmin << " xmax=" << xmax << '\n';
  }


  Apertures* RING_aper_b1 = new Apertures(nt,verbose);
  Apertures* RING_aper_b2 = new Apertures(nt,verbose); // for the moment identical

  // RING_aper_b1->Print(); // show non zero apertures
  RING_aper_b1->PlotFrame(zmin,zmax,xmin,xmax); // plot frame and keep limits in Apertures class

  if(RING_aper_b1->zvec.size()==0) // no aperture info
  {
	cout << " no aperture info, just plot beam" << '\n';
	valarray<double> z_beam  =nt->GetVar("Z");
	valarray<double> x_2_beam=nt->GetVar("X_2");
	MyBeamGraph(z_beam.size(),kBlue,z_beam,x_2_beam);
  }
  else MyApertureGraph(RING_aper_b1->zvec.size(),kBlue,RING_aper_b1->zvec,RING_aper_b2->x_2_vec,RING_aper_b2->aper_1_vec); // myGraph.C

  vector<unsigned int> i_IP_List  =FindElements(*nt,"IP" ,verbose,"NAME",0);  // IPs, name starting with IP      FindElements in Twiss_util.C
  vector<unsigned int> i_Bend_List=FindElements(*nt,"B"  ,verbose,"NAME",0);  // Bends
  vector<unsigned int> i_Quad_List=FindElements(*nt,"Q"  ,verbose,"NAME",0);  // Quads
  vector<unsigned int> i_Cav_List =FindElements(*nt,"CAV" ,verbose,"NAME",0); // Cavities

  vector<unsigned int> i_all;
  i_all.reserve( i_IP_List.size()+ i_Bend_List.size()+ i_Quad_List.size()+ i_Cav_List.size());
  i_all.insert( i_all.end(), i_IP_List.begin(),   i_IP_List.end() );
  i_all.insert( i_all.end(), i_Bend_List.begin(), i_Bend_List.end() );
  i_all.insert( i_all.end(), i_Quad_List.begin(), i_Quad_List.end() );
  i_all.insert( i_all.end(), i_Cav_List.begin(),  i_Cav_List.end() );

  PlotNames(nt,i_all,xmax,verbose); // Plot.C

  if(verbose) // write filename, date on plot
  {
	cout << __FILE__<< " " << __FUNCTION__ << " line " << setw(4) << __LINE__ << " verbose=" << verbose << " write extra info on the plot  nt->GetFname()=" << nt->GetFname() << '\n';

    TText* tt0=new TText(); tt0->SetTextFont(kTimes); tt0->SetTextSize(k8pt);
    string DirName=nt->GetFname();
    tt0->SetTextAlign(12); // left adjusted and vertically centered
    tt0->DrawTextNDC(.05,.98,DirName.c_str());

	gStyle->SetOptDate(23); // datefl=23 top right, date + time, use 0 to turn off use datefl=0
	gStyle->GetAttDate()->SetTextSize(k12pt); gStyle->GetAttDate()->SetTextFont(kTimes);
  }

  TCanvas* c2=RING_aper_b1->GetCanvas();
  c2->SaveAs("/tmp/$LOGNAME/ring_o_s.eps");
}
