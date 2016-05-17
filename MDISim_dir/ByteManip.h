/* ~/c/MyClib/ByteManip.h
  with forward declarations and generic code
  to avoid problems with instantaniation, leave generic code in the header

for conversion to bits assumes that 1 Byte = 8 bit  which seems is the general standard

 10/11/2012 updated for gcc-4.7   see also  http://gcc.gnu.org/gcc-4.7/porting_to.html
 change order, first specific types, then template

 rootcint problems with Mavericks,   for the moment left out for rootcint
 __signed,  int8_t   not defined
 already visible with

 rootcint -v -f MyClib_dict.C -c ~/c/MyClib/ByteManip.h

 seems to come from   stdint.h
 maybe not needed any more with C++11 ???

*/

#ifndef ByteManip_h
#define ByteManip_h 1

//old #include <iostream> // for rootcint
//old #include <iomanip>  // for rootcint
//old #include <sstream>  // for rootcint
//old #include <cstdlib>  // for rootcint, get long defined
//old #include <stdint.h> // for rootcint, uint16_t, ..

bool BigEndian(); // test if the machine is BigEndian  (short 1 = 0000 0001  in BigEndian, instead  0001 0000 in LittleEndian)

//old void DumpStruc(const          char* cbuf,int len,ostream& FilOut);
//old void DumpStruc(const unsigned char* cbuf,int len,ostream& FilOut); // allow also unsigned

string DumpStruc(const          char* cbuf,const int len);
string DumpStruc(const unsigned char* cbuf,const int len); // allow also unsigned char

// special case with length given:
string StringBits(const unsigned char* Byte,int len); // write to string rather than file, can be called in output as:  << StringBits((const unsigned char*) &object,sizeof(object))
// in general just call: StringBits(object)

void PrintBits(const unsigned char* word,int len,ostream &FilOut); // rather general, can be called as:     PrintBits((const unsigned char*) &object,sizeof(object),FilOut);


uint16_t GetNbytes_uint16(uint16_t nNumber,const unsigned char* address);
uint32_t GetNbytes_uint32(uint32_t nNumber,const unsigned char* address);
uint64_t GetNbytes_uint64(uint64_t nNumber,const unsigned char* address);

unsigned char ByteFromBits(const string bits); // Input is a string like "10110011", output the corresponding Byte
uint16_t uint16_FromBits(const string bits); // From string like "10110011 101100111", output the corresponding 16 bit number, any characters other than 0 and 1 are skipped
uint32_t uint32_FromBits(const string bits); // From string like "10110011 10110011 10110011 10110011", output the corresponding 32 bit number, any characters other than 0 and 1 are skipped
uint64_t uint64_FromBits(const string bits); // From string like "10110011 10110011 10110011 10110011 10110011 10110011 10110011 10110011", output the corresponding 64 bit number, any characters other than 0 and 1 are skipped
int BytesFromBitString(const string bits,unsigned char* buf,unsigned int bufsiz); // opposite of StringBits, from string like "10110011 10110011" to Bytes

long ByteSwap(const long  WordIn);  // return swapped long, do not modify input
void ByteSwap(short *HalfWord);     // swap short in place
void ByteSwap(long  *Word);         // swap long in place
void ByteSwap2(char *cbuf,const unsigned int len);     // general reverse of groups of 2 Bytes; up to len Bytes
void ByteSwap4(char *cbuf,const unsigned int len);     // general reverse of groups of 4 Bytes; up to len Bytes

string BinReadDump(const char* BinFileName); // general binary dump (print) of a file (first 1024 words)
string CallUnixHexDump(const char* BinFileName); // call unix hexdump, intermediate write to tmp file

int Unix_MacOS_DOS(const char* FileName);   // return 1 for Unixfile, 2 for MacOSfile, 3 for DOSfile, 0 for inconsistent
bool IsTextFile(const char* FileName,int verbose); // return true if text file, false if binary file, give details for verbose > 0
bool IsTextFile(const char* FileName,int verbose,const int nChars); // reads only first nChars

unsigned int BCD2Dec(unsigned int BCD); // from BCD to dec
unsigned int Dec2BCD(unsigned int Dec); // from dec to BCD


// template<class T> string ToHex(T number);
template<class T> inline string ToHex(const T number) // number -> oct string
{
  ostringstream StrOut;
  StrOut.fill(' ');
  StrOut.setf(ios::showbase|ios::right|ios::showpoint); // prefix oct and hex by 0x,  fill up with 000 at end to reach full size
  StrOut << std::setbase(16);
  int siz=sizeof(number);
  if(siz==sizeof(char)          ) StrOut << static_cast<char>      (number);
  else if(siz==sizeof(short)    ) StrOut << static_cast<short>     (number);
  else if(siz==sizeof(uint32_t) ) StrOut << static_cast<uint32_t>  (number); // with 64 bits long may already be 8 byte, use this to make sure there is also a 4 byte type
  else if(siz==sizeof(long)     ) StrOut << static_cast<long>      (number);
  else if(siz==sizeof(long long)) StrOut << static_cast<long long> (number);
  /*
   //  old         *reinterpret_cast<short *>  (&number);
   // rather use         static_cast<short> (number)  etc   ;    see   http://en.wikipedia.org/wiki/Static_cast
   if(siz==sizeof(char)          ) StrOut << *reinterpret_cast<char *>     (&number);
   else if(siz==sizeof(short)    ) StrOut << *reinterpret_cast<short *>    (&number);
   else if(siz==sizeof(uint32_t) ) StrOut << *reinterpret_cast<uint32_t *> (&number); // with 64 bits long may already be 8 byte, use this to make sure there is also a 4 byte type
   else if(siz==sizeof(long)     ) StrOut << *reinterpret_cast<long *>     (&number);
   else if(siz==sizeof(long long)) StrOut << *reinterpret_cast<long long*> (&number);
   */
  else StrOut << " ToOct unexpected size of " << siz << " Bytes ";
  StrOut << std::setbase(10);
  return StrOut.str();
}

// template<class T> string ToOct(T number);
template<class T> inline string ToOct(T number) // number -> hex string
// on templates and instantiation see Stroustrup 3 p. 860 ff
{
  ostringstream StrOut;
  StrOut.fill(' ');
  StrOut.setf(ios::showbase|ios::right|ios::showpoint); // prefix oct and hex by 0x,  fill up with 000 at end to reach full size
  StrOut << std::setbase(8);
  int siz=sizeof(number);
  if(siz==sizeof(char)          ) StrOut << *reinterpret_cast<char *>     (&number);
  else if(siz==sizeof(short)    ) StrOut << *reinterpret_cast<short *>    (&number);
  else if(siz==sizeof(long)     ) StrOut << *reinterpret_cast<long *>     (&number);
  else if(siz==sizeof(long long)) StrOut << *reinterpret_cast<long long*> (&number);
  else StrOut << " ToHex unexpected size of " << siz << " Bytes ";
  StrOut << std::setbase(10);
  return StrOut.str();
}

template<class T> string StringBits(T word) // used to hide the cast
// template, call StringBits in ByteManip.C
{
  ostringstream StrOut;
  StrOut.fill(' ');
  StrOut << StringBits(reinterpret_cast<unsigned char*> (&word),sizeof(word));
#ifdef __mysstream__ // dirty fix for missing sstream
  StrOut << ends; // terminate explicitely with '\0' or ends, at least when using older strstreams
#endif
  return StrOut.str();
}

template<class T> string HexAndBits(const T number)   // print a number as hex and string of bits
{
  ostringstream StrOut;
  StrOut.fill(' ');
  StrOut.setf(ios::showbase|ios::right|ios::showpoint); // prefix oct and hex by 0x,  fill up with 000 at end to reach full size
  StrOut << setw(21) << ToHex(number) << " in bits=" << StringBits(number);
  return StrOut.str();
}

// template<class T> string DecHexAndBits(T number);        // print a number as decimal hex and string of bits
template<class T> string DecHexAndBits(const T number)    // print a number as decimal, hex and string of bits
{
  ostringstream StrOut;
  StrOut.fill(' ');
  StrOut.setf(ios::showbase|ios::right|ios::showpoint); // prefix oct and hex by 0x,  fill up with 000 at end to reach full size
  StrOut << std::setbase(10) << setw(20) << number << setw(21) << ToHex(number)
  << " in bits=" << StringBits(number);
  return StrOut.str();
}

// unsigned char BitsInByte(const unsigned char Byte,unsigned int iup,unsigned int ilow); // old, rather use the generic BitsIn

// template<class T> T BitsIn(T number,unsigned int iup,unsigned int ilow);
template<class T> T BitsIn(const T number,unsigned int iup,const unsigned int ilow) // generic template version, ok for BigEndian. For LittleEndian do Byte swap first
{ unsigned int verbose=0;
  unsigned int NumberOfBits=sizeof(number)*8;
  if(verbose) std::cout << "BitsIn " << DecHexAndBits(number) << " iup=" << iup << " ilow=" << ilow << " NumberOfBits=" << NumberOfBits <<'\n';
  T Result=0;
  // 1) mask for bits iup-ilow
  T BitMask=0;
  T One=1;
  if(iup>NumberOfBits-1) iup=NumberOfBits-1;
  for(unsigned int i=ilow;i<=iup;++i) BitMask=BitMask|(One<<i);
  if(verbose)
  {
    std::cout << "number  " << DumpStruc((char *) &number, sizeof(number))  << '\n';
    std::cout << "BitMask " << DumpStruc((char *) &BitMask,sizeof(BitMask)) << '\n';
  }
  // now do the masking
  Result=number&BitMask;
  if(verbose) { std::cout << "after masking Result " << DumpStruc((char *) &Result,sizeof(Result)) << '\n'; }
  // now shift these bits to the end
  Result= Result>>ilow;
  if(verbose) { std::cout << "after shift to lower bits Result "<< DumpStruc((char *) &Result,sizeof(Result)) << '\n'; }
  return Result;
}


// template<class T> string DecOctHexAndBits(T number); // print a number as decimal, oct, hex and string of bits
template<class T> string DecOctHexAndBits(const T number) // print a number as decimal, oct, hex and string of bits
{
  ostringstream StrOut;
  StrOut.fill(' ');
  StrOut.setf(ios::showbase|ios::right|ios::showpoint); // prefix oct and hex by 0x,  fill up with 000 at end to reach full size
  StrOut << std::setbase(10) << setw(20) << number << " = " << setw(21) << ToHex(number) << " = " << setw(25) << ToOct(number)
    << " in bits=" << StringBits(number);
  return StrOut.str();
}
#endif
#define ByteManip_h 1
