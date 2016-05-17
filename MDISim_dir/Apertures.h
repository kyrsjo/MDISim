//   ~/c/MDISim/Apertures.h     H. Burkhardt 5/2013
//
//  class with vectors relevant for Aperture  ---  a bit of duplication of ntuple,  could do most directly with ntuple
//  should all have the same length
// http://www.cplusplus.com/reference/valarray/valarray/
// http://www.cplusplus.com/reference/vector/

#ifndef Apertures_h

class Apertures
{ public:
  unsigned int verbose;
  vector<string> AperName;
  vector<string> AperKeyw; // maybe empty, size=0
  vector<string> AperType; // maybe empty, size=0
  vector<double> x_2_vec; // x_2  is x from survey in global Euclidian coordinates
  vector<double> yvec;
  vector<double> zvec;
  vector<double> svec;
  vector<double> aper_1_vec;
  vector<double> aper_2_vec; // maybe empty, size=0
  vector<double> aper_3_vec; // maybe empty, size=0
  vector<double> aper_4_vec; // maybe empty, size=0

  double aper_x_n_min,aper_y_n_min;
  unsigned int isigx_min,isigy_min;

  Apertures(const Ntuple* nt,unsigned int verbose); // constructor,    filled from tfs table via ntuple,   keeping only element with reasonable aperture information   0 < aper1 < 5
  virtual unsigned int Noent() const { return (unsigned int) svec.size();} // return the number of entries
  virtual void CheckConsistence(); // check all vector sizes are have Noent() or 0
  virtual void RemoveEntry(unsigned int ient);
  virtual void CleanUpApertures(); // remove redundant entries and correct historic IP aperture values
  virtual void CombineLeftRight(const double smax);
  virtual void PlotFrame(double xmin,double xmax,double ymin,double ymax);
  Ntuple* MakeNtuple(); // make a new ntuple with the cleaned re-arranged aperture information
  virtual void Print(unsigned int ientr1 = 0,unsigned int ientr2 = std::numeric_limits<unsigned int>::max(),unsigned int PriMax=20); // Print a range of entries
  TCanvas* GetCanvas(){return c2;};
private:
  double xmin,xmax,ymin,ymax; // as used for PlotFrame
  TCanvas* c2;
};

#define Apertures_h 1
#endif
//   ~/c/MDISim/Apertures.C     H. Burkhardt 5/2013
