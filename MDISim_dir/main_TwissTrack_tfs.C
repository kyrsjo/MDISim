/*   ~/c/MDISim/main_TwissTrack_tfs.C
 5/2013
 started as newer version of TwissTrack_root.C   with direct reading of tfs file using my root with MyClib   works both well within root   and  as command line executable

 rm -rf "/tmp/$LOGNAME/"*

 Unix make -  also works on lxplus, checked 19/12/2014
 proj=MDISim ;                       cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make -j5 ; make install
 proj=MDISim ; targ=TwissTrack_tfs ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h


 see    ~/mad/lhc/lhc_V6_5_survey.madx

 cd /tmp/$LOGNAME ; rm -f * ; Release/TwissTrack_tfs ; open *.pdf

 needs directory with track files in tfs
 /tmp/$LOGNAME/twiss_track     make by running twiss_track
 or
 ~/mad/lhc/twiss_track_ref


 see  http://root.cern.ch/download/doc/7CINT.pdf  Setting the Include Path


      test within root,   ~/mad/lhc/twiss_track_ref  is 90m
 cd /tmp/$LOGNAME ; root
  .I /Users/hbu/myroot
 .L ~/c/MDISim/Twiss_util.C
 .L ~/c/MDISim/Apertures.C
 .L ~/c/MDISim/myGraph.C
 .L ~/c/MDISim/Plot.C
 .L ~/c/MDISim/TwissTrack_Param.C
 .L ~/c/MDISim/main_TwissTrack_tfs.C

 TwissTrack_tfs("~/mad/lhc/twiss_track_ref","_px_",2); // in main_TwissTrack_tfs.C
 TwissTrack_tfs("~/mad/lhc/twiss_track_ref","_py_",2);

 TwissTrack_tfs("/tmp/$LOGNAME/twiss_track","_px_",2);
 TwissTrack_tfs("/tmp/$LOGNAME/twiss_track","_py_",2);


 Full runs with tracking and display
 -------------------------------------
 example as relevant for background tracking from the TCTs left through IP
 rm -rf "/tmp/$LOGNAME/"*
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/lhc/lhc.xcodeproj -target twiss_track -configuration Release;
 cd /tmp/$LOGNAME ; rm -f * ; time /tmp/$LOGNAME/Release/twiss_track -vt 11mIR5  7000 ~/mad/lhc/lhc_V6_5_start.madx db/V6.5.coll.str db/IR5/IP5_beta11.0p.str ; ~/c/pawformat/LHCphases tune_*.dat
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target TwissTrack_tfs -configuration Release; cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C /tmp/$LOGNAME/Release/ ; Release/TwissTrack_tfs -V /tmp/$LOGNAME/twiss_track _py_ ; open AperLHC*.pdf

 nominal  use  options  c,s for crossing / separation   like  -Vcs for verbose with crossing and separation, in physics just crossing angle -Vc
 rm -rf "/tmp/$LOGNAME/"*
 proj=lhc ; targ=twiss_track ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h
 export LHCVERSION=V6.503
 cd /tmp/$LOGNAME ; rm -f * ; time /tmp/$LOGNAME/Release/twiss_track -Vc  Nom_IP5 7000 ~/mad/lhc/lhc_V6_5_start.madx db/V6.5.coll.str db/IR5/IP5_beta_0.55.str ; ~/c/pawformat/LHCphases tune_*.dat ; cat BetaStar_b*.out
 proj=MDISim ; targ=TwissTrack_tfs ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; mv $targ /tmp/$LOGNAME/Release/. ; cd /tmp/$LOGNAME ; Release/$targ -h
 cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C /tmp/$LOGNAME/Release/ ; Release/TwissTrack_tfs -V /tmp/$LOGNAME/twiss_track _px_ ;
 cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C /tmp/$LOGNAME/Release/ ; Release/TwissTrack_tfs -V /tmp/$LOGNAME/twiss_track _py_ ;
 cd /tmp/$LOGNAME ; ln -sf ~/myroot/rootlogon.C /tmp/$LOGNAME/Release/ ; Release/TwissTrack_tfs -V /tmp/$LOGNAME/twiss_track _pt_ ;
 open AperLHC*.pdf


 */

#include "MyCppStdHeaders.H"
#include <TRint.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLegend.h>
#include <TPolyLine.h>

#include "RunFlags.h"
#include "FileInfo.h"  // GetFileListInDir
//#include "Ntuple.h"
//#include "MyRoot.h"
#include "Apertures.h"
#include "TwissTrack_Param.h"
#include "Plot.h"
#include "Lorentz.h"
#include "Twiss_util.h"
#include "myGraph.h"

void MyAperturePolyLine(const unsigned int n,const vector<double>& svec,const vector<double>& aper) // no clipping
{
  // plot +/-svec and +/-aper
  double x[n];
  double y[n];

  for(unsigned int i=0;i<n;++i) x[i]=svec[i];
  for(unsigned int i=0;i<n;++i) y[i]=aper[i];
  TPolyLine* tp=new TPolyLine(n,x,y); tp->SetLineColor(kBlue); tp->Draw();

  for(unsigned int i=0;i<n;++i) x[i]= svec[i];
  for(unsigned int i=0;i<n;++i) y[i]=-aper[i];
  tp->DrawPolyLine(n,x,y);

  for(unsigned int i=0;i<n;++i) x[i]=-svec[i];
  for(unsigned int i=0;i<n;++i) y[i]= aper[i];
  tp->DrawPolyLine(n,x,y);

  for(unsigned int i=0;i<n;++i) x[i]=-svec[i];
  for(unsigned int i=0;i<n;++i) y[i]=-aper[i];
  tp->DrawPolyLine(n,x,y);
}

void Set_Plot_Object(Ntuple* nt,double emit,const vector<unsigned int>& elemList,double nsig,double xymax,const char plane,unsigned int linestyle,unsigned int verbose) // Show lines own to nsig for element in elemList
{
  unsigned int ibeta=nt->ivar("BETX");
  unsigned int linecolor=kRed;
  if(plane=='x')
  {
	if(verbose>2) cout << "plane is x" << '\n';
  }
  else
  {
	if(verbose>2) cout << "plane is y" << '\n';
	ibeta=nt->ivar("BETY");
	linecolor=kGreen;
  }
  unsigned int is=nt->ivar("S");
  vector<string> nt_StrCol=nt->GetStrCol("NAME");
  for(unsigned int i=0;i<elemList.size();++i)
  {
	unsigned int ient=elemList[i];
	if(ient<nt->Noent())
	{
	  double s   =nt->Entry(ient,is);
	  double beta_coll=nt->Entry(ient,ibeta);
	  double sig=sqrt(beta_coll*emit);
	  double aper=nt->Entry(ient,nt->ivar("APER_1"));
	  if(plane!='x') aper=nt->Entry(ient,nt->ivar("APER_2"));
	  if(verbose) cout << setw(4) << ient << "  " << setw(14) << nt_StrCol[ient]
		<< setw(9) << s << " "
		<< nsig << " "
		<< setw(9) << beta_coll << " "
		<< setw(9) << 1.e3*sig << " "
		<< 1.e3*aper << " "  // available aperture at this element in the plane considered
		<< 1.e3*nsig*sig
		<< '\n';
	  TLine* line_coll = new TLine(s,nsig*sig,s,xymax); // define a line
#ifdef __GNUC_PATCHLEVEL__	
	  if(verbose>2) cout << " compiled by GNUC version " << __GNUC__ << '.' << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << '\n';
	  line_coll->SetLineWidth((Width_t)0.1); // 0.1 invisible on screen  - do not use directly in root, ok in pdf
#endif
	  line_coll->SetLineColor(linecolor);
	  line_coll->SetLineStyle(linestyle); // 2 is dashed line,  3 is dashed dot        http://root.cern.ch/root/html/TAttLine.html
	  line_coll->Draw();
	  line_coll->DrawLine(s,-nsig*sig,s,-xymax);
	}
	else
	{
	  cout << "Set_Plot_Object *** error *** ient=" << ient << " > Noent=" << nt->Noent() << '\n';
	}
  }
}

void TwissTrack_tfs(string InpDir, const string SearchString,unsigned int verbose)
{
  if(verbose>1) cout << "start of TwissTrack_tfs(" << InpDir << "," << SearchString << "," << verbose << ")" << '\n';
  InpDir = ResolveHome(InpDir);
  if(verbose) cout << "InpDir=" << InpDir << " SearchString=" << SearchString << " verbose=" << verbose << '\n';
  vector<string> FileList;
  int ierr=GetFileListInDir(InpDir,FileList);
  if(ierr) { cout << " problem  to get file list for " << InpDir << '\n'; return; }
  else if(verbose) cout << " directory " << InpDir << " has " << FileList.size() << " files (or subdirectories, links) " << '\n';
  vector<string> InputFileNameList_b1,InputFileNameList_b2;
  for(unsigned int i=0;i<FileList.size();++i)
  {
	if( EndsWith(FileList[i],"b1.tfs") && FileList[i].find(SearchString) !=string::npos ) InputFileNameList_b1.push_back(string(InpDir)+"/"+FileList[i]);
	if( EndsWith(FileList[i],"b2.tfs") && FileList[i].find(SearchString) !=string::npos ) InputFileNameList_b2.push_back(string(InpDir)+"/"+FileList[i]);
  }
  if(verbose)
  {
	cout << '\n' << "b1 files with " << SearchString << '\n';
	for(unsigned int i=0;i<InputFileNameList_b1.size();++i) cout << InputFileNameList_b1[i] << '\n';
	cout << '\n' << "b2 files with " << SearchString << '\n';
	for(unsigned int i=0;i<InputFileNameList_b2.size();++i) cout << InputFileNameList_b2[i] << '\n';
  }
  // Aperture information from first file
  const char* AperFile = InputFileNameList_b1[0].c_str();
  // beam information from first file
  double Eb =GetParamValue_tfs("ENERGY",AperFile);
  double emitx=GetParamValue_tfs("EX",AperFile); // get emittance from tfs file header, using GetParamValue_tfs from ~/c/MyClib/StrConv.C
  double emity=GetParamValue_tfs("EY",AperFile); // get emittance from tfs file header

  if(verbose) cout << "use apertures from " << AperFile << " Eb=" << Eb << " GeV" << '\n';
  Ntuple* ntaper= new Ntuple(AperFile);
  if(verbose) ntaper->PrintSummary();

  // TCanvas *c1 = new TCanvas("c1","Root Canvas 1");

  Apertures* LHC_aper = new Apertures(ntaper,verbose);
  double xymax=0.05;
  // LHC_aper->PlotFrame(-500,500,-xymax,xymax);
  LHC_aper->PlotFrame(   0,500,-xymax,xymax); // only right of IP
  MyApertureGraph(LHC_aper->svec.size(),kBlue,LHC_aper->svec,LHC_aper->x_2_vec,LHC_aper->aper_1_vec);

  TwissTrack_Param* tp=new TwissTrack_Param(emitx,emity,verbose);
  if(verbose>2) cout << " after new TwissTrack_Param" << '\n';

  vector<unsigned int> i_IP_List   =FindElements(*ntaper,"IP" ,verbose,"NAME",0); // IP       FindElements in Twiss_util.C
  vector<unsigned int> i_XRP_List  =FindElements(*ntaper,"XRP",verbose,"NAME",0); // Roman pots
  vector<unsigned int> i_Coll_List =FindElements(*ntaper,"TC" ,verbose,"NAME",0); // TC collimators
  vector<unsigned int> i_Quad_List =FindElements(*ntaper,"MQ",verbose,"NAME",0);  // Quads

  vector<unsigned int> i_all;
  i_all.reserve( i_IP_List.size()+ i_XRP_List.size()+ i_Coll_List.size() + i_Quad_List.size());
  i_all.insert( i_all.end(), i_IP_List.begin(),  i_IP_List.end() );
  i_all.insert( i_all.end(), i_XRP_List.begin(), i_XRP_List.end() );
  i_all.insert( i_all.end(), i_Coll_List.begin(),i_Coll_List.end() );

  i_all.insert( i_all.end(), i_Quad_List.begin(),i_Quad_List.end() );

  if(verbose) cout << __FILE__ << " TwissTrack_tfs line " << setw(4) << __LINE__  << '\n';

  PlotNames(ntaper,i_all,xymax,verbose); // in Plot.C, plot names for all elements in  i_all  on top

  vector<string> Ntaper_StrCol=ntaper->GetStrCol("NAME");
  if(i_IP_List.size()>0)
  {
	unsigned int ip_entr=i_IP_List[0];
	tp->IPstr=Ntaper_StrCol[ip_entr] ;
	tp->BetaStarX=ntaper->Entry(ip_entr,ntaper->ivar("BETX"));
	tp->BetaStarY=ntaper->Entry(ip_entr,ntaper->ivar("BETY"));
	if(verbose) cout << "TwissTrack_Param Print " << tp->Print() << " i_IP_List.size()=" << i_IP_List.size() << " Ntaper_StrCol[0]=" << Ntaper_StrCol[0] << '\n';
  }

  double nsig=10; // set to fixed number of sigma
  char plane='x';
  double emit=emitx;
  nsig=sqrt(emitx)*LHC_aper->aper_x_n_min; // set to maximum available aperture
  if(SearchString.find("y") !=string::npos)
  {
	plane='y';
	emit=emity;
	nsig=sqrt(emity)*LHC_aper->aper_y_n_min; // set to maximum available aperture
	cout << "nsig=" << nsig << " is the maximum available aperture in y from" << Ntaper_StrCol[LHC_aper->isigy_min] << '\n';
  }
  else
  {
	cout << "nsig=" << nsig << " is the maximum available aperture in x from" << Ntaper_StrCol[LHC_aper->isigx_min] << '\n';
  }
  cout << "Set_Plot_Object plane=" << plane << '\n';
  cout << "iele      Element       s     nsig      beta     sigma    aper    nsig*sigma" << '\n';
  cout << "                        m               m        mm       mm      mm" << '\n';
  Set_Plot_Object(ntaper,emit,i_Coll_List,nsig,xymax,plane,2,verbose);
  Set_Plot_Object(ntaper,emit,i_XRP_List, nsig,xymax,plane,3,verbose);

  double x1=0.8,y1=0.80, x2=x1+0.2, y2=0.90; // normalized coordinates for legend
  TLegend *tl =  new TLegend(x1,y1,x2,y2); // http://root.cern.ch/root/html/TLegend.html

  const unsigned int xcolor=kRed;
  const unsigned int ycolor=kGreen;

  const bool Plot_b1=true;
  const bool Plot_b2=false;

  TGraph *grx,*gry;

  if(Plot_b1)
  {
	// Plot tracks b1
	// for(unsigned int i=0;i<2;++i)
	for(unsigned int i=0;i<InputFileNameList_b1.size();++i)
	{
	  Ntuple NT(InputFileNameList_b1[i].c_str());
	  unsigned int n=NT.Noent();
	  grx=MyTrackGraph(n,NT.GetVar("S"),NT.GetVar("X"),xcolor,verbose);
	  gry=MyTrackGraph(n,NT.GetVar("S"),NT.GetVar("Y"),ycolor,verbose);
	}
	tl->AddEntry(grx,"x","pl");
	tl->AddEntry(gry,"y","pl");
	tl->Draw();
  }

  if(Plot_b2)
  {
	//Plot tracks b2 --- looks same as b1  (not going backward)
	for(unsigned int i=0;i<InputFileNameList_b2.size();++i)
	{
	  Ntuple NT(InputFileNameList_b2[i].c_str());
	  unsigned int n=NT.Noent();
	  MyTrackGraph(n,NT.GetVar("S"),NT.GetVar("X"),xcolor,verbose);
	  MyTrackGraph(n,NT.GetVar("S"),NT.GetVar("Y"),ycolor,verbose);
	}
  }

  TCanvas* c2=LHC_aper->GetCanvas();
  c2->SaveAs(string("/tmp/$LOGNAME/AperLHC"+SearchString+".pdf").c_str());
  c2->SaveAs(string("/tmp/$LOGNAME/AperLHC"+SearchString+".eps").c_str());

}

#ifndef __CLING__

void RunFlags::ShowUsage() const
{
  cout << "Help for " << argv0
  << " which is a unix command line utility. Written in May 2013 by H. Burkhardt" << '\n'
  << BOLD << "SYNOPSIS" << NORMAL << '\n'
  << "    " << argv0 << " [OPTION] [DirName] [MatchStr]" << '\n'
  << BOLD << "OPTIONS" << NORMAL << " " << cflags << '\n'
  << "    -h    show help" << '\n'
  << "    -v    verbose output" << '\n'
  << "    -V    more verbose info" << '\n'
  << " default values for the optional arguments are" << '\n'
  << " DirName=" << ArgvDefault[0]
  << " MatchStr=" << ArgvDefault[1] << '\n'
  << BOLD << "EXAMPLEs" << NORMAL << '\n'
  << ArgvVec[0] << " -V ~/mad/lhc/twiss_track_ref _py_" << '\n'
  ;
}

int main(int argc,char * const argv[])
{
  vector<string> ArgvDefault={"~/mad/lhc/twiss_track_ref","_py_"}; // C++11
  cout << " this is " << __FILE__ << '\n';
  double CPUStart=clock();

  const char* cflags="hvV";
  RunFlags Flag(argc,argv,cflags,ArgvDefault);
  if(Flag.verbose) cout << __FILE__<< " " << __FUNCTION__ << " after RunFlags line " << setw(4) << __LINE__ << '\n';

  if(Flag.verbose) cout << "TwissTrack_tfs main start DirName=" << Flag.ArgvVec[1] << " MatchStr=" << Flag.ArgvVec[2] << endl;
  TwissTrack_tfs(Flag.ArgvVec[1],Flag.ArgvVec[2],Flag.verbose);

  if(Flag.verbose>0) cout << "CPU time " << (clock()-CPUStart)/CLOCKS_PER_SEC << " sec" << '\n';
  return 0;
};
#endif
