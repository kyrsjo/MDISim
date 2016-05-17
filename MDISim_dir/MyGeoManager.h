//   ~/c/MDISim/MyGeoManagerTest.h

#ifndef MyGeoManager_h
#define MyGeoManager_h 1

inline TGeoRotation geo_rot_y(double theta) { return TGeoRotation("rot",-90, -theta*180./M_PI, 90); } // rotation by theta around y, equivalent to same as rot.RotateY(theta*deg);

void CloseCanvas(TCanvas* c1=NULL);
void CheckFeatures();
void PlotAxesAtOrigin();
TVirtualGeoTrack* MinimalGeom(bool ShowEveWindow);

void my_geo_current_pos_dir_Print(unsigned int verbose=0);
void my_geo_current_node_vol_Print(unsigned int verbose=0);
void my_geo_track(unsigned int verbose=0);

void ShowShape();
void ShowShapes2();
void ShowBelle(bool HER=true, bool right=true, unsigned int verbose=0);
void SynRadGeo();

void my_Pri_ObjArray(const char* ObjType, const TObjArray* ObjList, unsigned int verbose=0);

class TEveGeoShape;

void pri_node_daughters(TGeoNode* node,unsigned int verbose=0);
void my_Geom_Diagnostics(unsigned int verbose=0);

#endif
