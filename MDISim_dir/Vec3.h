// ~/c/MyClib/Vec3.h

// #pragma once

#ifndef Vec3_h
#define Vec3_h 1

class Spherical; class Vec4; class Mat3x3; // class forward declarations

class Vec3 { // use no virtual to allow initialization like Vec3 vec={0,0,0};
public:
  double r[3]; // x,y,z
  // now the methods
  Vec3() {}; //Stroustrup2 p. 579; constructor, allow for empty init
  Vec3 (const double& x,const double& y,const double& z) {r[0]=x;r[1]=y;r[2]=z;} // constructor
  Vec3 (const Spherical& v); // construct from Spherical
  Vec3 (const Vec4& q); // construct from Vec4
  ~Vec3 () { } // (empty) destructor

  friend Vec3 operator+ (const Vec3&,const Vec3&);    //Stroustrup3 282, add two three vectors
  friend Vec3 operator- (const Vec3&);    //Stroustrup3 282, change sign of vector part
  friend Vec3 operator- (const Vec3&,const Vec3&);    // difference of 2 three vector
  friend Vec3 operator^ (const Vec3&,const Vec3&);    //Stroustrup3 282, three vector cross product
  friend Vec3 operator* (double, const Vec3&);    //Stroustrup3 282, factor * three vector
  friend Vec3 operator/ (const Vec3&,double); // three vector devided by double
  friend double   operator* (const Vec3& v1,const Vec3& v2)   //Stroustrup3 282, three vector scalar product
  { return v1.r[0]*v2.r[0]+v1.r[1]*v2.r[1]+v1.r[2]*v2.r[2];}
  friend bool operator == (const Vec3&,const Vec3&); // two vectors equal

  // possible to define abs2 and abs as inline inspector functions, more natural outside, see below
  // double inline abs2() const {return      r[0]*r[0]+r[1]*r[1]+r[2]*r[2] ;} // 3 momentum squared, const because changes nothing in class (const function or inspector) Stroustrup3 229
  // double inline abs () const {return sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);} // abs 3 momentum, const because changes nothing in class (const function or inspector) Stroustrup3 229
  void ParallelPerp(const Vec3& b,Vec3& rParallel,Vec3& rPerp);
  Spherical ToSpherical();   // from Vec3 to Spherical
  Mat3x3    GetRotToZMat();  // returns the matrix which rotates the Vec3 to +z direction
  const string Print(unsigned int verbose=0) const; // print the vector, does not change the class, so const
};

// rather define outside class, makes calling more natural  abs(vec) instead of vec.abs()

inline double abs2(const Vec3& v) {return      v.r[0]*v.r[0]+v.r[1]*v.r[1]+v.r[2]*v.r[2] ;} // 3 momentum (or length) squared
inline double abs (const Vec3& v) {return sqrt(v.r[0]*v.r[0]+v.r[1]*v.r[1]+v.r[2]*v.r[2]);} // abs of three momentum  or length of vector
inline double   phi_from_vec(const Vec3& v) { return atan2(v.r[1],v.r[0]); } // atan2(y/x)   between 0 and 2pi, or -M_PI to +M_PI
inline double theta_from_vec(const Vec3& v) { return acos(v.r[2]/abs(v)); } // theta between 0 and M_PI;  r[2]/r is cos theta = z/r

namespace V3
{
  // next rather general, not using Vec3, but dealing in general way with 3dim vectors using valarray<double>;  useful for checking survey files
  double* centre(valarray<double> x, valarray<double> y, valarray<double> z, unsigned int verbose=0); // find the centre of points, return as double*, general - without Vec3 class, could go somewhere else
  bool  IsClosed(valarray<double> x, valarray<double> y, valarray<double> z, unsigned int verbose=0); // check distance between first and last 3 vector

  const string Print(const double (&x)[3],unsigned int verbose=0);
  
  const Vec3 xdir(1,0,0);
  const Vec3 ydir(0,1,0);
  const Vec3 zdir(0,0,1);
} // namespace V3

#endif
