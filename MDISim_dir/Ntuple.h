/* ~/c/MyClib/Ntuple.h

In C++ use Nvar() and not   Nvar(void) etc  as in C  see http://en.wikipedia.org/wiki/Void_type

 http://www.cplusplus.com/reference/vector/vector/     http://www.cplusplus.com/reference/valarray/

 Not really speed optimized,   what about   http://www.cplusplus.com/reference/cstring/memcpy/


 number of entries is   data.size()    and increases in filling
 the number of variables is more the same as the tags, dmin, dmax size   and fixed,   except for special operations like   RemoveVar

 data[ient][ivar]       size is = Noent and also equal to Capacity needed. if the number of entries is known before, use reserve
    stored and filled in rows,   access mostly by variable or in columns
 StrCol[ient][ivar]    same for StrCol         rather inefficient when removing variables -  i.e. columns

 number of varibales "nvar" = tags.size()
 also =  data[0].size()    which could be changed if text is there

*/

#ifndef Ntuple_h
#define Ntuple_h 1

#include <iostream> // for root
//old #include <valarray> // for root

class Mat6x6;

class Ntuple
{
protected:
  unsigned int verbose;
  ostream *theFilOut;  // mostly for messages
  vector<string> tags; // pointer to vector of (nvar) strings
  vector<valarray <double> > data; //pointer to vector of tags.size() vectors, each a valarray  of  noent=data.size()  // was <float> until  10/03/2010
  // both IsTim or IsStr  have either size 0 or tags.size()
  vector<bool> IsTim; // To mark columns with time strings like "16:19:43"  or  "2012-10-11 12:03:06.043"  which are converted to double, identified by ":" in data in first data line
  vector<bool> IsStr; // true for columns with strings,   data will be zero for these,  strings stored in StrCol,  IsStr NULL or
  vector<vector<string> > StrCol; // allow for columns with names

  unsigned int WordLength; // introduced on 05/08/2010; default sizeof(double)=8    or for backward compatibility sizeof(float)=4;
  bool CalcMinMax;
  void init(const vector <string>& tags,ostream *theFilOut); // init for all constructors
public:
  // methods:
  Ntuple(){} // constructor 0, empty
  Ntuple(const unsigned int nvar,const char *ctags[],ostream *theFilOut=&cout,unsigned int verbose=0);    // constructor 1 with cstrings, could calculate nvar=sizeof(ctags)/sizeof(*ctags);
  Ntuple(const vector <string>& tags,ostream *theFilOut=&cout,unsigned int verbose=0);                    // constructor 2 from vector of strings
  // next - filename can't be string, would take as tags vector with one entry
  Ntuple(const char*   fname,const char    *matchstr=NULL,bool ReadRoot=false,unsigned int verbose=0);    // constructor 3 from ntuple from an input file,  generic ReadNtuple, in NtupleRead.C, with default arguments, allowing for just Ntuple(fname), in NtupleRead.C
  // second argument has default and can be left out
  // third argument is only used by the derived NtupleRoot::NtupleRoot to signal that .root files can be read
  // fourth argument to allow to turn debug on
  Ntuple(const Ntuple&)=default; // copy constructor http://en.cppreference.com/w/cpp/language/copy_constructor  http://en.wikipedia.org/wiki/Copy_constructor to allow for  Ntuple Ntuple2(Ntuple1); = default needed otherwise missing code
  virtual ~Ntuple() {}   // (empty) destructor
  virtual unsigned int Nvar()  const { return (unsigned int) tags.size();} // return the number of variables, Stroustrup3 p. 229
  virtual unsigned int Noent() const { return (unsigned int) data.size();} // return the number of entries
  virtual void reserve(const unsigned int noent) { data.reserve(noent); if(IsStr.size()>0) StrCol.reserve(noent); } // reserve space for noent to speed up filling, Stroustrup 3, p. 455, make room, without initializing
  virtual vector<string> Get_tags() const { return tags;}   // return the tag names
  vector<bool> GetIsStr(void) const { return IsStr;} // to  set  use  DefineAsString
  virtual void fill(const valarray <double>& Dat);
  virtual void fill(const double Dat[]);
  virtual void fill(const valarray <double>& Dat,const vector<string>& TextVec); // fill ntuple with both data and text
  virtual void ResetData(); // clear all data, keep tags
  virtual double Min(const unsigned int i); // return the minimum value of variable i   and set position in iMin
  virtual double Max(const unsigned int i); // return the maximum value of variable i   and set position in iMax
  //virtual double Min(string VarNam) const {return Min(ivar(VarNam));}  // return the minimum value of variable VarNam  with recalculation
  //virtual double Max(string VarNam) const {return Max(ivar(VarNam));}  // return the maximum value of variable VarNam  with recalculation
  virtual double Min(string VarNam) const {return dmin[ivar(VarNam)];}  // return the minimum value of variable VarNam
  virtual double Max(string VarNam) const {return dmax[ivar(VarNam)];}  // return the maximum value of variable VarNam
  virtual unsigned int imin(string VarNam) const { return (unsigned int) iMin[ivar(VarNam)];}  // return the minimum position
  virtual unsigned int imax(string VarNam) const { return (unsigned int) iMax[ivar(VarNam)];}  // return the maximum position
  valarray<double> GetVar(const unsigned int ivar) const;   // get the (column) vector of variable ivar
  void SetVar(const string& VarName,const valarray<double>& DatVec);  // set a whole data column. Allows to modify ntuple data
  void SetVar(const string& VarName,const vector<string>&   StrCol);
  valarray<double> GetVar(const string& VarNam)     const;    // get the vector of variable VarNam, corresponding to a column in the printed table
  valarray<double> GetData(const unsigned int ient) const;    // get the data of the i-th line (row) in the printed table - covering the i-th element or row of all tags
  vector<string>   GetStrData(const unsigned int Data) const; // get the data of the i-th line (row) Str

  void AddToVar(const string& VarNam,double AddValue); // AddValue  to variable of   VarNam,   used for example to shift all all positions of a position vector

  virtual void AddVar(const string& VarNam,const valarray<double>& NewVar); // add a variable

  virtual void AddSqrt(const string& VarNam); // add new variable which is sqrt of variable with VarNam,  Example : get sqrt_betx  from betx

  vector<string> GetStrCol(const string& VarNam) const;
  vector<string> GetStrCol(const unsigned int ivar) const;

  vector<Mat6x6> GetRmatrices(); // to get all rmatrices, useful for ntuples read from tfs files with rmatrix

  string GetVarNam(const unsigned int ivar) const; // get the name of variable ivar
  unsigned int ivar(const string VarNam) const;    // get the number of the variable which for VarNam, check existance by result < Nvar
  bool VarExists(const string VarNam) const;

  string GetFname() const { return Fname; }
  virtual void SetFname(string fname) { this->Fname=fname; }

  virtual void RemoveVar(const unsigned int i); // remove the tag and data of variable i
  virtual void RemoveVar(const string VarNam);  // remove the tag and data of VarNam. Removes only the first in case several tags have the same name
  virtual void RemoveVarList(const vector<string>& VarList); // remove variables listed in VarList,   example   RemoveVarList({"GLOBALTILT","SLOT_ID"});  // C++11
  virtual void ZeroSuppression(); // remove variables which have only 0 data

  virtual void RemoveEntry(unsigned int ient);  // remove ient for all variables

  virtual void PrintSummary();    // to *theFilOut
  virtual void PrintSummary(ostream *theSumOut);  // to *theSumOut
  virtual void WriteAsciiNtFile(const char *matchstr="",const char *fname="Ntuple.out",const streamsize prec=0);  // file given by filename, a new file is opened, precision matched to name length can be increased with blanks -  or given here
  virtual void WriteAsciiNtFile(const string& matchstr,ostream *theAsciiNtFile,const streamsize prec=0); // with ostream, can be open, even standard output

  virtual void StrCol_to_double(); // store StrCol as 64 bit data, keeps only the first 8 characters
  virtual void double_to_StrCol(); // and back
  virtual void WriteBinaryNtFile(const char *fname); // in ~/c/MyClib/Ntuple.C
  virtual void ReadBinaryNtFile(const char *fname);  // in ~/c/MyClib/NtupleRead.C

  string ReadTags(istringstream *theistr); // decode the line with blank separated tags
  virtual void CleanTagName(); // make sure the tag names contain only Alphanumeric characters or _
  virtual void Read_cformats(istringstream *theistr);      // decode the line with c formats
  virtual void ReadAsciiNtFile(const char *matchstr,const char *fname); // file given by filename
  virtual void ReadAsciiNtFile(const char *matchstr,ifstream *theInpFile); // file given by stream
  virtual void Read_csv(const char *fname); // read a csv (excel..) file and make from it an ntuple
  virtual void Read_tfs(const char *fname); // read tfs files as generated by mad,  header variables can be read using    GetParamValue_tfs   defined in  ~/c/MyClib/StrConv.h
  virtual void Fast_Read_tfs(const char *fname); // read tfs files as generated by mad

  virtual void ReadTags_tab_separated(string HeaderLine,bool EmptyMatchString); // decode the line with tab separated tags
  virtual void Read_tab_separated(const char *fname);      // read tab separated tables to my ntuple, used for LHC lumiscans
  virtual void Read_tab_separated(ifstream *theInpStream); // read tab separated tables to my ntuple, used for LHC lumiscans

  virtual double Entry(const unsigned int ient,const unsigned int ivar) const {return data[ient][ivar];} // get value of variable iv in entry ip, was first float

  // -- next should be remove in future, just use SetVerbose instead       Nt.SetVerbose(Flag.verbose);   Nt.SetVerbose(verbose);
  // virtual void TurnDebugOn() {verbose=1;}      // TurnDebugOn()       --> SetVerbose(1)
  // virtual void TurnDebugOff() {verbose=0;}     // TurnDebugOff()      --> SetVerbose(0)
  // virtual void TurnMoreDebugOn() {verbose=2;}  // TurnMoreDebugOn()   --> SetVerbose(2)
  // virtual void TurnMoreDebugOff() {verbose=1;} // TurnMoreDebugOff()  --> SetVerbose(1)

  virtual void SetVerbose(unsigned int verbose) { this->verbose=verbose; }
  unsigned int GetVerbose() const { return verbose; }

  virtual void UseFloat() { WordLength=4;} // for backward compatibility. Useful to read old binary files like ~/temp/0.333_TuneV_SepY/All_Part_turn_0_b0.MyBinNtuple

  // in special cases allow for access to data from outside  -   useful to allow for strings in ntuples
  virtual void DefineAsString(unsigned int ivar); // define variable ivar as string
  bool TranslateDots; // normally true,  if true translates any "." in tags to "_"   turn off if this is not wanted, used in ~/c/lhc/main_BetaBeat_in_squeeze.C
  bool IgnoreReadErrors; // Set true to try to carry on in spite of read errors

  void friend NtupleRemoveDuplicates(const Ntuple& Nt1,Ntuple& Nt2); // look for identical variable in Nt1, Nt2, and remove them from Nt2, useful before merge
  friend Ntuple NtupleMerge(const Ntuple& Nt1,Ntuple Nt2); // Merge two ntuples with same number of entries. "nvar = nvar1 + nvar2", declared as friend to allow access to private, removes duplicates in Nt2 if required -- on copy of Nt2
  friend Ntuple CourantSnyder_to_Euclidian(const Ntuple& nts,const Ntuple& nt,const double zmin,const double zmax); // declared as friend to allow access to private

  virtual void SortByVar(const string& VarNam);
  
protected:
  string Fname; // keep filename from which the ntuple was read for information, protected so also available to derived NtupleRoot

private:
  unsigned int ReadFill(const vector<string>& datastr);
  
  void CalcMinMax_at_fill(const valarray <double>& Dat);
  void CalcMinMax_at_fill(const double Dat[]);
  void ReCalcMinMax(const unsigned int ivar=999999); // for all, or just ivar
  
  string PrintIsTimeStamp();
  string PrintIsStr();
  void debug_PrintData(unsigned int nvar_max,unsigned int noent_max); // debug print contents of data
  void SetSizeForNoent(const unsigned int noent,const bool WithStrCol=false);
  void ShowAllSizes();
  bool SizesConsistent();
  unsigned int nTimeStampColumns() {unsigned int n=0; for(unsigned int i=0;i<IsTim.size();++i) if(IsTim[i]) n++; return n;}
  unsigned int nTextColumns()      {unsigned int n=0; for(unsigned int i=0;i<IsStr.size()      ;++i) if(IsStr[i])       n++; return n;}

  vector<double> dmin,dmax; // was <float> until  10/03/2010
  vector<unsigned int> iMin,iMax; // entry with Min or Max
};

Ntuple ReadAndMerge(const char* f1,const char* f2,unsigned int verbose=0);

//new -  somewhat specialized
int ReadStrengthNamesAndValues(const char* StrengthFileName,const string& MatchStart,vector<string>& VarNam,vector<double>& Value,const unsigned int verbose=0);
bool CompareStrengthFiles(const char* fname1,const char* fname2,double tolerance=1.e-6,unsigned int verbose=2);

#endif
