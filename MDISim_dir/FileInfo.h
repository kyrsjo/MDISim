// ~/c/MyClib/FileInfo.h

#ifndef FileInfo_h
#define FileInfo_h 1

#include <unistd.h>   // gid_t,   for root
#include <sys/stat.h> // for root


namespace FileInfoConst
{
  const int LinkToNonExisting=-123; // my error flag, for link to non-existing file. Used in my CheckLinks
}

class NameAndNumber
{
public:
  string Name;
  int Number;
};

bool  IsRegular(const char*  fname); // true if regular file  (and not a directory or a link)
string gid2Name(const gid_t    gid); // from group id like gid=20   to string "staff"
string uid2Name(const uid_t    uid); // from user  id like uid=501  to string "hbu"

const string StatMode(const mode_t& mode); // return a string of the file type and permissions similar to ls -l   typical results is "drwx------"

vector<string> ListOfFilesMatching(const char* Fname,unsigned int verbose=0); // get the list of all files matching name Fname   as visible with   ls -1
int GetFileListInDir(const string& Dir_name,vector<string> &Files,unsigned int verbose=0); // Get file list in directory as vector of strings

class FileInfo
{
public:
  unsigned int gid,uid;
  int SetwPrintFname; // used for setw before filename
  // methods:
  FileInfo(const string& dirname,const unsigned int verbose); // constructor
  virtual ~FileInfo() {}; // (empty) destructor
  virtual int GetInfo(const string& fname); // get info for file fname in directory dirname
  //old virtual int Get_gid_Names(); // read /etc/group to get gid group name correspondance
  //old virtual string GroupName(); // set the group name if available
  string Print() const;
  virtual bool IsRelativeLink();
  virtual bool IsLink();
  virtual bool IsAlias(); // Mac OS link or alias - not clear how to treat
  virtual bool IsRegular();
  virtual bool IsDirectory();
  virtual bool IsPackage();  // special MacOS Packages .mpkg, .pkg, .app
  virtual int Size(); // return size in Bytes
  time_t access_time,modif_time,creation_time;    // acces, modification, creation times, see stat.h
  string FullFileName;
  string OrigFname; // original file name, filled in case of a link
  struct stat info; // struct stat see sys/stat.h
protected:
  unsigned int verbose;
  bool RelativeLink; // true if link and original file starting with ../
  string dirname,fname;
  bool Package;
  mode_t mode;    // mode  dir or file, access rights, see sys/stat.h
  struct stat Originfo; // info for original if link
  //old static vector<NameAndNumber> GroupIDTable;
  //old string groupname; // not always set, call GroupName to to so
  int last_errno; // store errors
};
#endif
