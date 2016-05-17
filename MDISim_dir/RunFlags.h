//  ~/c/MyClib/RunFlags.h

#ifndef RunFlags_h
#define RunFlags_h 1

#include "EscapeSequences.h"

//old void ShowUsage(int argc,char* const argv[],const char* cflags,const vector<string>& ExtraInfo=vector<string>()); // provide the code with the main program, ExtraInfo with default value so that can be left out

class RunFlags
{
public:
  unsigned int verbose;
  bool a,A,c,d,f,g,h,i,l,L,m,M,n,o,p,P,r,R,s,S,t,z,errflg,RUN; // option flags, and special option Flags
  vector<string> ArgvVec; // arguments, without option flags
  string optarg_str;      // extra optarg, for the moment only use with --RUN  like   --RUN=1   --RUN=4
  RunFlags(){;}; // defult constructor
  RunFlags(int argc,char* const argv[],const char* cflags,const vector<string>& ArgvDefault=vector<string>()); // creator with initialisation to 0,false, ExtraInfo with default value so that can be left out
  void ShowUsage() const;
  string Show() const;
private: // used in ShowUsage  -  to be provided with each main calling RunFlags
  const char* argv0;  // Name of calling program
  const char* cflags; // like hnrsvV
  vector<string> ArgvDefault;
};
#endif
