// ~/c/MyClib/Mat3x3.h

#ifndef MMat3x3_h
#define MMat3x3_h 1

// #include "Vec3.h"
class Vec3;

class Mat3x3 {
public: // class for 3*3 matrices
  double m[3][3]; // the first index is the row (line) and the second the column number, see colletive/Math.tex with Matrix.tex
  unsigned int verbose;
  // now the methods
  Mat3x3() { m[0][0]=m[0][1]=m[0][2]=  m[1][0]=m[1][1]=m[1][2]=  m[2][0]=m[2][1]=m[2][2] =0; }; //Stroustrup2 p. 579; constructor, allow for empty init, all elements 0
  Mat3x3(double mat3[],unsigned int verbose=0)
  { // a(irow,icol) flattened to irow*ndim+icol
    for(unsigned int irow=0;irow<3;++irow)
      for(unsigned int icol=0;icol<3;++icol)
        m[irow][icol]=mat3[irow*3+icol];
    this->verbose=verbose;
  };
  Mat3x3(const double& m00,const double& m01,const double& m02,
         const double& m10,const double& m11,const double& m12,
         const double& m20,const double& m21,const double& m22,unsigned int verbose=0) // constructor with definition of all matrix elements
  {
    m[0][0]=m00;m[0][1]=m01;m[0][2]=m02;
    m[1][0]=m10;m[1][1]=m11;m[1][2]=m12;
    m[2][0]=m20;m[2][1]=m21;m[2][2]=m22;
    this->verbose=verbose;
  };
  
  friend Mat3x3 operator+ (const Mat3x3&,const Mat3x3&); // add two matrices
  friend Mat3x3 operator- (const Mat3x3&,const Mat3x3&); // diff of two matrices
  friend Mat3x3 operator* (const Mat3x3&,const Mat3x3&); // multiply two matrices
  friend Vec3 operator*   (const Mat3x3&,const   Vec3&); // multiply vector and matrix
  
  void SetVerbose(unsigned int verbose) { this->verbose=verbose; }
  
  //old const void PrintObj(std::ostream& FilOut); // print the matrix
  bool IsSymmetric() const;
  const string Print() const; // print the matrix
};

Mat3x3 Transpose(const Mat3x3&); // return the transposed Matrix (without changing the original)
Mat3x3 Rot_x(double phi);
Mat3x3 Rot_y(double theta);
Mat3x3 Rot_z(double psi);
Mat3x3 WCS_mat3(double theta,double phi,double psi); // Courant Snyder to Euclidian rotation matrix, see also WCS_root in Myroot.h
Mat3x3 SymInv(const Mat3x3&); // calculate the Inverse Matrix of a symmetric Mat3x3

namespace M3x3 // provide special 3x3 matrices    can be used as   M3x3::Identity,  see  ~/c/ansiitest/TestMatrix.C
{
  double inline trace(const Mat3x3& M) {return M.m[0][0]+M.m[1][1]+M.m[2][2];}
  double inline   det(const Mat3x3& M) // determinante, see Schaums lin.alg. p. 173
  {return
    M.m[0][0]*(M.m[1][1]*M.m[2][2]-M.m[1][2]*M.m[2][1]) -
    M.m[0][1]*(M.m[1][0]*M.m[2][2]-M.m[1][2]*M.m[2][0]) +
    M.m[0][2]*(M.m[1][0]*M.m[2][1]-M.m[1][1]*M.m[2][0]);
  }
  const Mat3x3 Identity(
                        1, 0, 0,
                        0, 1, 0,
                        0, 0, 1); // Identity Matrix http://en.wikipedia.org/wiki/Identity_matrix
} // namespace M3x3

#endif
