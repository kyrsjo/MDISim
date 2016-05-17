// ~/c/MyClib/Vec4M.h

#ifndef Vec4M_h
#define Vec4M_h 1

class Spherical; class Vec3; class Vec4; // class forward declarations

class Vec4M : public Vec4
{
public: // Vec4 and in addition the mass
  double m;       // sometimes useful to keep mass for precision (electron) and speed
  // now the methods
  Vec4M(){} // allow for empty init
  Vec4M(const Vec4& v,double m);  // construct based on Vec4 and masss
  Vec4M(double E,const Vec3& v,double m); // construct based on energy, Vec3 and mass
  Vec4M(double E,double p1,double p2,double p3,double m); // construct based on energy, momentum and mass
  friend Vec4M operator+ (const Vec4M&, const Vec4M&);    //Stroustrup3 282, add two four vectors
  friend Vec4M operator- (const Vec4M&);  //Stroustrup3 282, change sign of vector part
  // inline void CalcMass() { m=sqrt(p[0]*p[0]-(p[1]*p[1]+p[2]*p[2]+p[3]*p[3])); }
  inline void CalcMass() { m=abs(*this); }
  void    Boost(const Vec4M& qb); // boost to qb rest system,  call with -qb to boost into qb system
  // old const void PrintObj(ostream& FilOut);
  const string Print() const; // print the 4 vector and mass
};

Vec4M Boost(const Vec4M& q,const Vec4M& qb); // new vector from p boosted by qb rest system
#endif
