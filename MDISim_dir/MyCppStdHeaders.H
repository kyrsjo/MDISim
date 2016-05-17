/* ~/c/MyClib/MyCppStdHeaders.H
 use this to precompile headers to speed up.  Can be made with make or directly:
 clang -v MyCppStdHeaders.H   ; echo makes  MyCppStdHeaders.H.gch 7.1 M
 g++   -v MyCppStdHeaders.H   ; echo makes  MyCppStdHeaders.H.gch 7.1 M
 
 depend on compiler and options, may have to make sure to use the same compiler with the same options
 
 compile source code with -Winvalid-pch -D__NO_INLINE__=1 option to check if precompiled headers included ok

 http://clang.llvm.org/docs/PCHInternals.html
 https://gcc.gnu.org/onlinedocs/gcc/Precompiled-Headers.html
*/

#ifndef MyCppStdHeaders
#define MyCppStdHeaders 1

  // http://www.cplusplus.com/reference/

  #include <iostream>	// for cin,cout,cerr,clog                            http://www.cplusplus.com/reference/istream/iostream
  #include <fstream>	// for definition of classes  ifstream, ofstream     http://www.cplusplus.com/reference/fstream
  #include <iomanip>	// i/o formatting like setprecision, setbase, flags  http://www.cplusplus.com/reference/iomanip

  #include <cstring>    // older C string routines like   strtok  etc        http://www.cplusplus.com/reference/cstring
  #include <string>		// Stroustrup3 432,  string of T                     http://www.cplusplus.com/reference/string
  #include <sstream>	// string streams, Stroustrup3 p. 640                http://www.cplusplus.com/reference/sstream

  #include <complex>	// for iimag in ConstPhys.h  http://www.cplusplus.com/reference/complex/     http://www.cplusplus.com/reference/cmath/
  #include <vector>		// Stroustrup 430, 442 includes, 52 general Containers ,442,469   http://www.cplusplus.com/reference/vector
  #include <valarray>	// Stroustrup 3 p. 662    http://www.cplusplus.com/reference/valarray

  // #include <array>   // http://www.cplusplus.com/reference/array/array/    C++11    used in ~/c/ansiitest/AnsiCpp.C   ~/c/ansiitest/main_Test_C++11.C

  #include <list>		// Stroustrup 431 includes, 452 operations, 470 general     http://www.cplusplus.com/reference/list
  #include <algorithm>  //Stroustrup 64,432, 509                             http://www.cplusplus.com/reference/algorithm

  #include <cstdlib>    // for getenv                                        http://www.cplusplus.com/reference/cstdlib
  #include <cctype>		//Stroustrup3 p. 601,   isalpha, isupper..           http://www.cplusplus.com/reference/cctype
  #include <cfloat>		// for DBL_MAX, MIN                                  http://www.cplusplus.com/reference/cfloat
  #include <ctime>		// for asctime, localtime                            http://www.cplusplus.com/reference/ctime
  #include <cstdint>	// uint32_t, needed for ByteManip.h                  http://www.cplusplus.com/reference/cstdint
  using namespace std; // make std names like cout available without std:: prefix
  // big 12.7 MByte

  // now add to this my main headers, increases slighlyt to 12.99 say 13 MByte
  // declare these also in dependencies in makefile
  #include "ConstPhys.h"	// CLight,eps0,ZFreeSpace...
  #include "Conversion.h"	// CPUTime
  #include "Numer.h" 		// for RanNorm
  #include "StrConv.h"
  #include "ByteManip.h"
  #include "Combined.h"  	// stringtok
  #include "Ntuple.h"
  #include "statis.h"	    // stmean

  #include "Lorentz.h"      // hbu trial 14/08/2015, still needs  Lorentz dummy; for root

#endif
