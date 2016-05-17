// several classes, originally all here, now split in different files
// 1) Spherical     spherical coordinates r, theta, phi
// 2) Vec3      general vector r = (x,y,z), now in extra Vec3
// 4) Mat3x3        3*3 matrices
// 5) Vec4      just E,p1,p2,p3
// 6) Vec4M     in addition mass
// 7) Lorentz       beta, gamma, name  , could be based on Vec4M
// 8) Vec6      for machine tracking, like x,x',y,y',t,pt   - now in extra Mat6x6
// 9) Mat6x6        6*6 matrix for machine tracking

// general considerations:
// the size of and array arguments is not available to the called function (Stroustrup2 p.128)
// Vec4 are not just Vec3 with an additional entry
// it therefore seems logical of creating separate classes for Vec3 and Vec4
// initialization:
// without constructors, initialization is like:
// Vec3 vec={1,0,0}; Mat3x3 UnityMat={1,0,0, 0,1,0, 0,0,1};
// for more flexibility (init kartesian from polar, Vec4 from Vec3 is possible via
// using overloading with several constructors. Initialization is then always like:
// Vec3 vec(1,0,0);Mat3x3 UnityMat(1,0,0, 0,1,0, 0,0,1);
// which is not more complicated with in addition any other initialization defined with
// constructors. Done for vectors, but not (yet, was not needed so far) for Mat3x3
// 17/1/2003: also done for Mat3x3
//
// see <complex> as examples what is defined as methods in and outside the class
// for example abs, polar are defined outside the class, see "T abs (const complex<T>& cx)"

// define already as class, details later

// note that some of the conversions like RadiusFromBfield, BfieldFromRadius are
// in ConstPhys.h and test of these in const/const.C


// struct s{  is the same as    class s{ public:   Stroustrup 167

#ifndef Conversion_h
#define Conversion_h 1

// global utility routines
ostream& MyOpen(const char* filename,const unsigned int verbose=0);
string FileToString(const char* InpFileFname,const unsigned int verbose);
void PrintVersionFlags(ostream& FilOut);
void SysInfo(const unsigned int verbose=0);
string EnvironmentInfo(const unsigned int verbose=0);
void AccessRight();
bool OnMacOs();
double* TimeAndResources(ostream& FilOut); // using getrusage, ok with GNU 3.n       returns  user, system, real time in seconds
string CPUTime(void); // CPU consumed since last call
int julday(int date);   // Julian Date of 23.5.1968 or iDate=19680523  is 24440000
int caldat(int julian); // inverse of julday. Here  julian is input
unsigned int ConstructUnixTime(int idate,int hour,int min,int sec);
char* DayTime();    // return char string with current DayTime nicely formatted
void ShowOfstreamStatus(ofstream& FilOut);
void ShowOstreamStatus(ostream& FilOut);
void ShowStreamStatus(ios& IOS);
void ShowOstreamFlags(ostream& ForOstream,ostream& FilOut);

bool FileExists(const char*   filename); // true if file (or directory) exists
bool FileExists(const string& filename); // true if file (or directory) exists
bool FilesExist(const vector<string>& filename,const unsigned int verbose=0); // true if all files in vector filename exist

#endif
