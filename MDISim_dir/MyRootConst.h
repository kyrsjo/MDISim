// ~/c/MyRoot/MyRootConst.h   put here definitions and classes that do not depend on other classes

// search for types in root include  for example by     cd $ROOTSYS/include ; grep "Font_t" *.h | grep "typedef"
// avoid calling things double
// colors in global   (look at with .g) and Gtypes.h (there with enum)
// http://root.cern.ch/root/html/Rtypes.h 
// http://root.cern.ch/root/html/TColor.html

#ifndef MyRootConst_h
#define MyRootConst_h 1

#include "TColor.h"

namespace MyRootConst // provide special 6x6 matrices    can be used as   M6x6::Identity,  see  ~/c/ansiitest/TestMatrix.C
{


// find $ROOTSYS -name "TColor.h"     ;  grep kRed $ROOTSYS/include/*.h       defined in Rtypes.h  by enum EColor
// kWhite = 0,
// kRed   = 632
// kBlue  = 600
// kGreen = 416
// kGray  = 920

// now my definitions, not there in any root header files, include on any machine with myroot

// getting good default settings like say 18pt char size everywhere and
// in root is very difficult
// there are many objects linked in complicated ways and often generated on the
// fly   (some text pad, char size from pad size which depends on other thins ..)

// ------------------- define useful globals

// fonts see http://root.cern.ch/root/html/TAttText.html
// see also old Paw manual page 129  (what was neg. there is here positiv*10)
// font  : Text font code = 10*fontnumber + precision
//	Font numbers must be between 1 and 14
//	precision = 1 fast hardware fonts (steps in the size)
//	precision = 2 scalable and rotatable hardware fonts

// hardware, scalable fonts
const Style_t kTimes					=132; // times
const Style_t kTimesBold				= 22; // times bold
const Style_t kHelvetica				= 42; // Helvetica regular
const Style_t kHelveticaBoldOblique		= 72; // Helvetica bold oblique
const Style_t kSymbol					=122; // symbol bold

const Style_t kFstyle=0; // use generally 0 rather than default 1001
const Width_t kLStyle=1; // default line style, 1 is solid line 2,3,4 are dashed, dotted and dashed-dotted
const Width_t kLwidth=2; // default line width

// on various sizes and transformations see class TPad http://root.cern.ch/root/html/TPad.html
// and in particular    http://root.cern.ch/root/html/TPad.html#TPad:ResizePad
// for postscript size: http://root.cern.ch/root/html/src/TCanvas.cxx.html#TCanvas:Size
//

const Float_t kMaxSize=20.;	// maximum real size seems is 20 cm
//old const Float_t kSizePt=700.;	// same in points
const int kSizePt=700;	// same in points

const Float_t k7pt = 7./600.;
const Float_t k8pt = 8./600.;
const Float_t k10pt=10./600.;
const Float_t k12pt=12./600.;
const Float_t k14pt=14./600.;
const Float_t k16pt=16./600.;
const Float_t k18pt=18./600.; // .03
const Float_t k21pt=21./600.; // .035
const Float_t k24pt=24./600.; // .04
const Float_t k32pt=32./600.;
const Float_t k36pt=36./600.;
const Float_t k48pt=48./600.;

// see my comments in the introduction of plotting.pdf
// const int kDarkGreen=kGreen+100;  old
// const int kDarkGreen=TColor::GetColor(0,191,0); //   this will load TColor too early and result in problems, also when static      TColor::GetColor(0,191,0) = 925
const int kDarkGreen=925; // ok no clash with TColor
} // namespace MyRootConst
using namespace MyRootConst;

#endif
