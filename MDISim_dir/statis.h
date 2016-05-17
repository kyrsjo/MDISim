// ~/c/MyClib/statis.h
#ifndef statis_h
#define statis_h 1

//old #include <cfloat> // DBL_MAX for rootcint

class StMean
{
public:
  bool prifl; // by default on, can be turned off to save speed in time critical applications
  double mu,sigx,sigm; // mean and rms of individual and mean
  long N;
  StMean() : prifl(true),N(0),xs(0),x2s(0),ws(0),w2s(0),xmin(DBL_MAX),xmax(-DBL_MAX),wmin(DBL_MAX),wmax(-DBL_MAX) {} //Stroustrup3 p. 270    virtual ~StMean(){} // (empty) destructor;   use initializers in same sequence as declarations
  virtual ~StMean(){} // (empty) destructor
  virtual void Reset();
  //old virtual void FillStMean(const double& xx);
  inline void FillStMean(const double& xx)   // see also PDG 28.2
  {
	N++;
    xs+=xx;       // linear sum       for mean
    x2s+=xx*xx;   // quadratic sum    for variance
	if(xx<xmin) xmin=xx;
	if(xx>xmax) xmax=xx;
  }
  virtual void FillStWeight(const double& xx,const double& weight);
  string GetStMean(); // do the calculation and return a string for optional printing
  double GetxMin(){ return xmin; } // get the minimum, internally updated at filling
  double GetxMax(){ return xmax; } // get the maximum, internally updated at filling
  double GetwMin(){ return wmin; } // get the minimum, internally updated at filling
  double GetwMax(){ return wmax; } // get the maximum, internally updated at filling
  virtual void PrintStMean(std::ostream *PriFile); // old, phase out,  rather use   << GetStMean()
protected:    // for quantities used internally in several methods
  double xs,x2s;  // linear and quadratic sums
  double ws,w2s;  // for weighted mean
private:
  double xd[100],wd[100]; // to store the first hundred data points with weights
  double xmin,xmax; // minimum and maximum value of x
  double wmin,wmax; // minimum and maximum value of w
};

class StLine
{
public:
  bool prifl; // by default on, can be turned off to save speed in time critical applications
  int N;
  int NStore; // use a constant like 1000, must be same as xi,yi dimension
  double aline,sigAli,bline,sigBli; // results a,b and error;  a is the slope
  StLine(): prifl(true),N(0),NStore(1000),sum(0),sumx(0),sumy(0),sumxx(0),sumxy(0) {}//Stroustrup3 p. 27
  virtual ~StLine(){} // (empty) destructor
  virtual void Reset();
  virtual void FillStLine(const double& x,const double& y);
  string GetStLine();  // do the calculation and return a string for optional printing
  virtual void PrintStLine(std::ostream *PriFile); // calc results and print,  old, phase out, replace by   << GetStLine()
private: // do not encourage direct access data directly
  double sum,sumx,sumy,sumxx,sumxy;
  double xi[1000],yi[1000];   // to store the first NStore data points
};
#endif
