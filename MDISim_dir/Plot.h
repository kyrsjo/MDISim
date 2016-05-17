// ~/c/MDISim/Plot.h
#ifndef Plot_h

void PlotNames(Ntuple* nt,const vector<unsigned int>& elemList,double xymax,unsigned int verbose);
void Plot_optics(Ntuple* nt,unsigned int verbose,double smin=0,double smax=400);
void Plot_survey(Ntuple* nt,unsigned int verbose=0,double xmin=0,double xmax=0,double zmin=0,double zmax=0);
#endif
#define Plot_h 1
