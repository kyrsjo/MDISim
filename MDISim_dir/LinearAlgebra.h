/*  ~/c/MyClib/LinearAlgebra.h

There is a lot on general matrix packages and linear algebra  (LAPACK, EIGEN)
 only a simple subset implemented here MyClib

*/


#ifndef LinearAlgebra_h
#define LinearAlgebra_h 1

//old #include <valarray> // for rootcint

// version with second index calculated to allow for variable array size
void PrintMatrix(const char* name,const double a[],const unsigned int nrow,const unsigned int ncol); // print an nxm matrix, see Stroustrup3 p. 839
void PrintMatrix(const char* name,const double* a,const unsigned int ndim); // print an nxn matrix
void PrintLUMatrix(const char* name,double a[],const unsigned int ndim,unsigned int verbose);
void ludcmp(const double a[],      double LU[],const unsigned int ndim,      unsigned int indx[],double& d,unsigned int verbose=0);
void lubksb(                 const double LU[],const unsigned int ndim,const unsigned int indx[],double b[]);
void mprove(const double a[],const double alud[],const unsigned int ndim,const unsigned int indx[],const double b[],double x[]);
void InvertMatrix(double a[],double ainv[],const unsigned int ndim);
void CopyMatrix(const double a[],double b[],const unsigned int nrow,const unsigned int ncol);
void MatrixMultiplication(const double a[],const double b[],double c[],const unsigned int ndim);
double det_from_ludcmp(const double a[],const unsigned int ndim,double d); // determinant of lu decomposed simply product of diagonal

//  for tests with  dynamic with valarray of valarray   see ~/c/ansiitest/TestLinearAlgebra.C

/*
 namespace MyMatrix // hide the global constant in a namespace
 {
 const unsigned int ndim=3; // global fixed dimension, to avoid problems with variable array as argument
 }
 using namespace MyMatrix;

 void PrintMatrix(const char* name,const double a[ndim][ndim]);
 void PrintLUMatrix(const char* name,const double a[ndim][ndim]);
 void ludcmp(double a[ndim][ndim],const unsigned int ndim,unsigned int indx[],double& d);
 void lubksb(const double a[ndim][ndim],const unsigned int ndim,const unsigned int indx[],double b[]);
 void InvertMatrix(double a[ndim][ndim],double ainv[ndim][ndim]);
 void CopyMatrix(const double a[ndim][ndim],double b[ndim][ndim]);
 void MatrixMultiplication(const double a[ndim][ndim],const double b[ndim][ndim],double c[ndim][ndim]);
 double det_from_ludcmp(const double a[ndim][ndim],double d);
 */


// earlier class Matrix was based on onedimensional valarray m
// 2 dim access [irow][icol] calculated as [irow*ncol+icol]

class Matrix
{
public:
  unsigned int nrow,ncol;
  valarray<double> m;
  // valarray< valarray<double> > m; // ValVal.mm.size()=nrow,   ValVal.mm[i].size()=ncol

  double d; // +/-1   for even or odd number of permutations to get LU. Needed for determinante
  class Matrix* LU;   // pointer to the LU decomposition version of m
  valarray<unsigned int> indx;

  Matrix(const unsigned int nrow,const unsigned int ncol); // construct new matrix, all entries zero
  Matrix(const double *m,const unsigned int nrow,const unsigned int ncol); // constructor with init from existing c-matrix
  Matrix(const Matrix&); // copy constructor
  virtual ~Matrix(); // destructor
  void Set_prmax(unsigned int prmax); // set the maximum number of rows and columns to be printed
  virtual void Print() const; // print the matrix
  virtual void calcLU();  //  calculate the LU decomposition and
  Matrix Inv(); //  calculate the Inverse Matrix of m
  valarray<double> solve(const valarray<double> &b); // solve a system of linear equations for b
  double   det(); // return the determinant of the matrix
  double trace(); // return the trace of the matrix
  void transpose(); // tranpose the matrix, rows to columns
  void addColumn(const double column[]);
  valarray<double> getColumn(unsigned int icol) const;
  valarray<double> getSumColumns() const; // sum up all columns, return sum as column
  valarray<double> getSumColumns(unsigned int icol1,unsigned int icol2) const; // sum up columns icol1 - icol2,  returns a "column" vector
  double GetMax() const; // value of largest entry
  void   inline set(unsigned int irow,unsigned int icol,double value) { m[irow*ncol+icol]=value; }
  double inline get(unsigned int irow,unsigned int icol) const { return m[irow*ncol+icol]; }

  virtual void SetVerbose(unsigned int verbose) { this->verbose=verbose; }

  friend Matrix operator* (Matrix&,Matrix&); // multiply two matrices, Stroustup3 p. 282
  friend valarray<double> operator* (const Matrix&,valarray<double> v); // multiply matrix and vector
  friend Matrix operator* (const Matrix&,const double fac); // multiply all matrix elements by fac
  friend Matrix operator/ (const Matrix&,const double fac); // divide   all matrix elements by fac
  
private:
  unsigned int verbose;
  unsigned int prmax; // limit the number of rows and columns to print to this
};

// used with valarray<double> as vectors

double abs2(valarray<double> x); // length^2 of vector

#endif