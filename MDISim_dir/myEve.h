//  ~/c/MDISim/myEve.h
#ifndef myEve_h
#define myEve_h 1
class TEveElement; class TEveGeoTopNode; class TEveTrack; class TEveTrackPropagator;class MultiView;

// bool FileExists(const char *filename ); // in MyClib
void PlotGuidesAtOrigin(); // show coordinate system for gEve
void show_extract(const char* fname, unsigned int verbose=0);
void my_default_view(); // with white background
MultiView* ShowProjections(TEveElement* el,unsigned int verbose);

TEveElement* StartEveWithGeomDisplay(const char* fname=NULL, bool AllowForProjections=true, unsigned int verbose=0);

// for tests:
TEveTrack* my_make_track(TEveTrackPropagator* prop, bool LER,double pz, unsigned int verbose=0);
void add_trks(TEveTrackPropagator* prop,TEveTrack* track, unsigned int verbose=0);
void MyEveFieldTest(bool isRungeKutta = true,bool LER = true,double zmax=2.1*100); //zmax default +/-2.1 m is for Belle.root
#endif
