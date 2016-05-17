// ~/c/MDISim/MyNtuple2Geom.h
#ifndef MyNtuple2Geom_h

class TGeoVolume;class TGeoMedium;

class Aper
{ public:
  string Name,Keyword,Apertype;
  double a_1;double a_2;double a_3;double a_4;
  double amax() const; // maximum of the four aperture numbers
  unsigned int color;
  const string Print() const;
  bool RECTELLIPSE_IsCircle()  const;
  bool RECTELLIPSE_IsEllipse() const;
  bool RECTELLIPSE_IsRectCircle() const;
};  // http://www.cplusplus.com/doc/tutorial/structures/       or better public class

void Plot_BeamScreen_2d(const Aper aper_l); //-- for debug  show directly BeamScreen in 2d

// next called only from inside MyNtuple2Geom.C
// TGeoVolume* MyVolume(TGeoMedium *med,const Aper aper_l,const Aper aper_r,const double len2,const double thick,unsigned int verbose);
void MyMaterialInit();
void InventMagnetAperturesIfZero(valarray<Aper>& aper,unsigned int verbose);
void Set_Aperture_colors(valarray<Aper>& aper,const unsigned int color,unsigned int verbose);
valarray<Aper> SetUp_all_Aper(const Ntuple* nt,const unsigned int color,unsigned int verbose);
void Simplify_Apertures(valarray<Aper>& aper,unsigned int verbose);
void Connect_Apertures(valarray<Aper>& aper,unsigned int verbose);

//-- main steering routine :
void GeomFromMyNtuple(const char* MyGeoName,const string StartElementName,const Ntuple& nt,const double zmin,const double zmax,const unsigned int color,const double Scale_xy,const unsigned int verbose=0,const bool ConnectApertures=true,const bool AddIndexToName=false,const bool SimplifyAper=true);

#endif
#define MyNtuple2Geom_h 1
