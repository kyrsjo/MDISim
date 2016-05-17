// Spherical.h

#ifndef Spherical_h
#define Spherical_h 1

class Vec3; // forward declare class Vec3 used in one of the constructors

class Spherical // spherical coordinates
{
public:
  double r;       // length of vector
  double theta;   // polar angle
  double phi;     // azimuthal angle
  Spherical() {}; //Stroustrup2 p. 579; constructor, allow for empty init
  Spherical (const double& r,const double& theta,const double& phi) { this->r=r; this->theta=theta; this->phi=phi;} // constructor
  Spherical (const Vec3& v); // construct from Vec3
  ~Spherical () { } // (empty) destructor
  //old const void PrintObj(ostream& FilOut);
  
  // next would work, but not really used/needed anywhere
  // double get_r() const { return r;};
  // double get_x() const { return r*sin(theta)*cos(phi);};
  // double get_y() const { return r*sin(theta)*sin(phi);};
  // double get_z() const { return r*cos(theta);};

  const string Print() const; // print the 3 vector
};
#endif
