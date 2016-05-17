/* ~/c/MDISim/main_CLIC.C

 
 rm -rf "/tmp/$LOGNAME/"*
 
 Unix make
 proj=MDISim ; targ=CLIC ; cd /tmp/$LOGNAME ; cmakeUnix ~/c/$proj ; cd $proj ; make -j9 ; make install   ;              echo makes /tmp/$LOGNAME/$proj/Release/$targ
 proj=MDISim ; targ=CLIC ; cd /tmp/$LOGNAME ; cmakeUnix ~/c/$proj ; cd $proj ; make $targ -j9 ; mv $targ Release/.
 
 then always
 cd /tmp/$LOGNAME/ ; time $proj/Release/$targ -h          2>&1 | tee /tmp/$LOGNAME/$targ.out ; open /tmp/$LOGNAME/$targ.out
 which is
 cd /tmp/hbu ; MDISim/Release/CLIC -h
 
 --- run in line mode by
 cd /tmp/$LOGNAME  ; rm -f * ; time Release/CLIC ; find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf

 Use  ~/mad/clic/v_10_10_11/job_clic.madx    to get  newbds_survey.tfs


 ---  1. step just setup geometry, raw geometry from tfs    and export to root file,   no plotting
 cd /tmp/$LOGNAME ; time root
 Lorentz dummy; // dummy to get MyClib types and constants, to avoid problems with my  ROOT 6.03/01
 .L ~/c/MDISim/MyNtuple2Geom.C
 gSystem->Load("libGeom");
 unsigned int verbose=1;
 Ntuple nt_b1(ReadAndMerge("~/mad/clic/v_10_10_11/newbds_twiss.tfs","~/mad/clic/v_10_10_11/newbds_survey.tfs",verbose)); if(verbose) nt_b1.PrintSummary();
 double zmin=0,zmax=2800,Scale_xy=1000;
 verbose=1;GeomFromMyNtuple("CLIC","NEWBDS",nt_b1,zmin,zmax,0,Scale_xy,verbose);
 gGeoManager->Export("CLIC_BDS.root");  // export the whole geometry
 gGeoManager->GetTopVolume()->Draw("ogl");
 .q

   or in line mode with element dependend coloring :
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; proj=MDISim ; targ=MyNtuple2Geom ; cmakeUnix $proj ;   cd /tmp/$LOGNAME/$proj ; make $targ -j5 ; cd /tmp/$LOGNAME/
 time /tmp/$LOGNAME/MDISim/MyNtuple2Geom -acsV CLIC_BDS NEWBDS 0 2800 1000 0 ~/mad/clic/v_10_10_11/newbds_twiss.tfs ~/mad/clic/v_10_10_11/newbds_survey.tfs 2>&1 | tee /tmp/$LOGNAME/MyNtuple2Geom_clic.out ; open /tmp/hbu/MyNtuple2Geom_clic.out

 --- 3d geometry plot with ogl ---
 cd /tmp/$LOGNAME ; time root
 TGeoManager::Import("CLIC_BDS.root");
 // TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/CLIC_BDS.root");
 gGeoManager->GetTopVolume()->Draw("ogl");

  --- 2d plot directly in root
 cd /tmp/$LOGNAME ; rm -f "/tmp/$LOGNAME/"* ; time root
 cout << __CLING__ << endl; // should show 1
 Lorentz dummy; // dummy to get MyClib types and constants, to avoid problems with my  ROOT 6.03/01
 .I /Users/hbu/myroot
 .L ~/c/MDISim/Twiss_util.C
 .L ~/c/MDISim/Apertures.C
 .L ~/c/MDISim/IPpos.C
 .L ~/c/MDISim/myGraph.C
 .L ~/c/MDISim/Plot.C
 unsigned int verbose=1;
 const char* Fname_b1_o="~/mad/clic/v_10_10_11/newbds_twiss.tfs";
 const char* Fname_b1_s="~/mad/clic/v_10_10_11/newbds_survey.tfs";
 Ntuple nt_b1(ReadAndMerge(Fname_b1_o,Fname_b1_s)); if(verbose) nt_b1.PrintSummary();
 // nt_b1.WriteAsciiNtFile(NULL,"/tmp/hbu/clic_o_s.out"); // write combined optics and survey
 Beam b1(Fname_b1_o,verbose); // get Energy and synchr integrals from optics twiss header
 b1.Get_Part().PrintLorentz(cout);
 double emitxN=660.e-9,emityN=20.e-9; // normalized emittance,  beginning of BDS
 b1.SetEmitN(emitxN,emityN);
 double nsig=1;
 b1.Print();
 verbose=2;
 CalcBeamSizeDivergence(nt_b1,b1.ex,b1.ey,b1.sige,verbose); if(verbose) nt_b1.PrintSummary();
 CalcSynrad(nt_b1,b1,verbose,nsig);
 Plot_optics(&nt_b1,verbose); c1->SaveAs("/tmp/$LOGNAME/ring_optics.eps");
 Plot_survey(&nt_b1,verbose); // whole BDS, writes /tmp/hbu/ring_o_s.eps
 .q
 find . -name "*.eps" -exec pstopdf {} \;  ; open *.pdf
 open "/tmp/$LOGNAME/"*.pdf


 */

#include "MyCppStdHeaders.H"  // use this to test syntax
#include <TRint.h>
#include <TGeoManager.h>

#include "Lorentz.h"
#include "Plot.h"
#include "Twiss_util.h"
#include "myGraph.h"
#include "myGraph3D.h"
#include "MyNtuple2Geom.h"

// int main(int argc,char * const argv[]) // normally this
int main(int argc,char *  argv[]) // const argv[]   not compatible with TRint
{
#ifndef __CLING__
  TRint* theApp = new TRint("", &argc, argv, NULL, 0); // root in line mode, defined theApp
#endif
  unsigned int verbose=1;
  const char* Fname_b1_o="~/mad/clic/v_10_10_11/newbds_twiss.tfs";
  const char* Fname_b1_s="~/mad/clic/v_10_10_11/newbds_survey.tfs";
  Ntuple nt_b1(ReadAndMerge(Fname_b1_o,Fname_b1_s,verbose));
  Beam b1(Fname_b1_o,verbose); // get Energy and synchr integrals from optics twiss header
  //double Eb=1500;
  //using namespace MyConst;
  //Lorentz Part(Eb,Mele,"e");
  //Part.FromE(); // calculate P,gamma, beta from E
  //Part.PrintLorentz(cout);
  b1.Get_Part().PrintLorentz(cout);
  b1.Print();
  double emitxN=660.e-9,emityN=20.e-9; // normalized emittance,  beginning of BDS
  double emitx=b1.Part->Emit(emitxN);
  double emity=b1.Part->Emit(emityN);
  cout << setprecision(6) << "emitxN=" << emitxN << " emitx=" << emitx << '\n';
  cout << setprecision(6) << "emityN=" << emityN << " emity=" << emity << '\n';
  double nsig=1;
  CalcSynrad(nt_b1,b1,verbose,nsig);
  Plot_optics(&nt_b1,verbose);
  // double xmin=-0.3,xmax=0.15,zmin=-300.,zmax=300.;
  // Plot_survey(&nt_b1,verbose,xmin,xmax,zmin,zmax);
  Plot_survey(&nt_b1,verbose); // whole machine

  double zmin=0,zmax=2800,Scale_xy=1000;
  verbose=1; GeomFromMyNtuple("CLIC","NEWBDS",nt_b1,zmin,zmax,kBlue,Scale_xy,verbose); // 3d plot of raw apertures
  gGeoManager->Export("CLIC_BDS.root");
#ifdef __CLING__
  cout << " running with CLING"<< '\n';
#else
  cout << " running in line mode. When done quit root from the menu bar / File " << '\n';
  theApp->Run();
#endif
};
