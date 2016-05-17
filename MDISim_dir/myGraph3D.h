// ~/c/MDISim/myGraph3D.h
#ifndef myGraph3D_h
class Ntuple;
class Beam;
void MyTrack3D(const Ntuple& nt,Int_t PdgCode,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,unsigned int verbose);
void EveSingleTrackDisplay(const Ntuple& nt,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,unsigned int verbose);
void PlotTracksInFiles(const char* fnames_tracks,const char* fname_survey,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,unsigned int verbose); // with translation from CS to Euclidian

void Plot_axis_arrow(const string axis,const double length,const double Scale_xy,const unsigned int verbose=0,const bool goto_CM_units=true);
void Plot_Bend_SR_Cones(const Ntuple& nt,const Beam &Mach,const double zmin,const double zmax,const double Scale_xy,const unsigned int verbose,const bool goto_CM_units=true,const double sign=1);

#endif
#define myGraph3D_h 1
