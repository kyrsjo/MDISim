// MDISim_init.C   H. Burkhardt 10/2015      for MDISim distribution version

void my_exec(string cmd,unsigned int verbose=0)
{
  if(verbose) cout << cmd << '\n';
  gROOT->ProcessLine(cmd.c_str());
}

void MDISim_init()
{
  unsigned int verbose=2;
  if(verbose>1) cout << "This is " << __FILE__ << " " << __FUNCTION__ << " line " << __LINE__ << '\n';
  const char* MDISim_dir_charptr=gSystem->Getenv("MDISim_dir");
  if(MDISim_dir_charptr==NULL)
  {
    cout << "environment variable MDISim_dir is not defined. Should be set with   export MDISim_dir=.." << '\n';
    cout << "  automatically done by        cd MDISim_dir ; source setup.sh " << '\n';
    return;
  }
  string MDISim_dir=MDISim_dir_charptr;

  // load MyClib and MyRoot, so far rather general,  ntuple..
  my_exec(".I "+MDISim_dir,verbose);   //    - aggiunge il path $MDISim_dir agli include
  my_exec("#include <MyCppStdHeaders.H>      ",verbose);
  my_exec(".L "+MDISim_dir+"/libMyClib.so    ",verbose);
  my_exec("#include <MyRoot.h>               ",verbose);
  my_exec("#include <NtupleRoot.h>           ",verbose);
  my_exec(".L "+MDISim_dir+"/libMDISim.so    ",verbose);
  my_exec(".L "+MDISim_dir+"/libMyroot6.so   ",verbose);
  my_exec(".L "+MDISim_dir+"/Twiss_util.C   ",verbose); // ADDED BY FRANCESCO .L /Users/$LOGNAME/MDISim_dir/Twiss_util.C
  my_exec("#include <Twiss_util.h>        ",verbose);
  my_exec("#include <myEve.h>             ",verbose);
  my_exec("#include <myGraph.h>           ",verbose);
  my_exec("#include <MyNtuple2Geom.h>     ",verbose);
  my_exec("#include <Apertures.h>         ",verbose);
  my_exec("#include <IPpos.h>             ",verbose);
  my_exec("#include <MultiView.h>         ",verbose);
  my_exec("#include <MyGeoManager.h>      ",verbose);
  my_exec("#include <Plot.h>              ",verbose);
  my_exec("#include <PlotAddTrack.h>      ",verbose);
  my_exec("#include <TwissTrack_Param.h>  ",verbose);
  my_exec("#include <myGraph3D.h>         ",verbose);
  cout << "all done, enjoy root enhanced for MDISim" << '\n';
  // to test type
  // StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5_LR_eve.root");
}
