// ~/c/MyClib/Vec4.h

#ifndef Vec4_h
#define Vec4_h 1

#include "Vec3.h"

class Spherical; ; // class forward declarations

class Vec4
{
public:
  double p[4];    // four vector components 0 is energy, 1-3 is momentum
  // now the methods
  Vec4() {}; // allow empty init
  Vec4 (const double& E,const double& px,const double& py,const double& pz) {p[0]=E;p[1]=px;p[2]=py;p[3]=pz;} // standard constructor, init like    Vec4 pos(Eb,px,py,pz);
  Vec4 (const double& E,const Vec3& v) {p[0]=E;p[1]=v.r[0];p[2]=v.r[1];p[3]=v.r[2];} // init from E, Vec3
  friend Vec4 operator+ (const Vec4&, const Vec4&); //Stroustrup3 282, add two four vectors
  friend Vec4 operator- (const Vec4&, const Vec4&); // subtraction
  friend Vec4 operator- (const Vec4&);    //Stroustrup3 282, change sign of vector part    ---  change direction, relevant for boost back,   not meant for subtraction
  friend double operator* (const Vec4& v1, const Vec4& v2)  { return v1.p[0]*v2.p[0] -v1.p[1]*v2.p[1]-v1.p[2]*v2.p[2]-v1.p[3]*v2.p[3]; }//Stroustrup3 282, four vector scalar product   P1*P2 = E1*E2  - p1*p2

  inline Vec3 Pvec() { return Vec3(p[1],p[2],p[3]); }

  //old double    abs2() const;   // 3 momentum squared, const because changes nothing in class (const function or inspector) Stroustrup3 229
  void    Boost(const Vec4& qb); // boost p to qb rest system,  call with -qb to boost into qb system
  //old const void PrintObj(ostream& FilOut);
  const string Print() const; // print the 4 vector, does not change the class, so const
};

// next are defined directly inline  outside the class
inline double absPvec2(const Vec4& q) {return      q.p[1]*q.p[1]+q.p[2]*q.p[2]+q.p[3]*q.p[3] ;} // abs squared of 3-mom. part of four vector
inline double absPvec (const Vec4& q) {return sqrt(q.p[1]*q.p[1]+q.p[2]*q.p[2]+q.p[3]*q.p[3]);} // abs         of 3-mom. part of four vector
inline double abs2(const Vec4& q) {return      q.p[0]*q.p[0]-absPvec2(q) ;} // abs squared of four vector = mass squared
inline double abs (const Vec4& q) {return sqrt(q.p[0]*q.p[0]-absPvec2(q));} // abs         of four vector = mass
Vec4    Boost(const Vec4& q,const Vec4& qb); // new vector from p boosted by qb rest system
#endif
