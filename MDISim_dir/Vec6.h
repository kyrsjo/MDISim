// ~/c/MyClib/Vec6.h

#ifndef Vec6_h
#define Vec6_h 1

namespace Vec
{
  const unsigned int ndim=6; // define a constant variable rather then using the numerical constant 6
}

class Vec6
{
public: // use no virtual to allow initialization like Vec6 vec={0,0,0,0,0,0};
  double r[Vec::ndim]; // like x,x', y',y', t,pt
  // now the methods
  Vec6() { for(unsigned int i=0;i<Vec::ndim;++i) r[i]=0; }; //Stroustrup2 p. 579; constructor, allow for empty init, put all components to 0
  Vec6(const double& r0,const double& r1,const double& r2,const double& r3,const double& r4,const double& r5)
  {r[0]=r0;r[1]=r1;r[2]=r2,r[3]=r3;r[4]=r4;r[5]=r5;} // constructor
  ~Vec6(){} // (empty) destructor

  //  friend Vec6 operator+ (const Vec6&,const Vec6&);    //Stroustrup3 282, add two 6 vectors
  //  friend Vec6 operator- (const Vec6&);    //Stroustrup3 282, change sign of vector part
  //  friend Vec6 operator- (const Vec6&,const Vec6&);    // difference of 2 6 vector
  //  friend Vec6 operator^ (const Vec6&,const Vec6&);    //Stroustrup3 282, 6 vector cross product
  //  friend Vec6 operator* (double, const Vec6&);    //Stroustrup3 282, factor * 6 vector
  //  friend Vec6 operator/ (const Vec6&,double); // 6 vector devided by double
  friend double   operator* (const Vec6& v1,const Vec6& v2)   //Stroustrup3 282, 6 vector scalar product
  { double res=0; for(unsigned int i=0;i<Vec::ndim;++i) res+=v1.r[i]*v2.r[i]; return res;}

  // possible to define abs2 and abs as inline inspector functions, more natural outside, see below
  // double inline abs2() const {return      r[0]*r[0]+r[1]*r[1]+r[2]*r[2] ;} // 3 momentum squared, const because changes nothing in class (const function or inspector) Stroustrup3 229
  // double inline abs () const {return sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);} // abs 3 momentum, const because changes nothing in class (const function or inspector) Stroustrup3 229
  const string Print() const;
};

// rather define outside class, makes calling more natural  abs(vec) instead of vec.abs()
inline double abs2(const Vec6& v) { double res=0; for(unsigned int i=0;i<Vec::ndim;++i) res+=v.r[i]*v.r[i]; return res;} // 6 momentum squared
inline double abs (const Vec6& v) { double res=0; for(unsigned int i=0;i<Vec::ndim;++i) res+=v.r[i]*v.r[i]; return sqrt(res);} // abs of 6 momentum

#endif
