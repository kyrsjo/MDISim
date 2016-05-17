// ~/c/MyClib/Combined.h

#ifndef Combined_h
#define Combined_h 1

vector<string> stringtok(const string &InpStr,const char* sep);      // split line with words separated by sep to words,  ignores double separators
vector<string> stringtok_fast(const string &InpStr,const char* sep); // split line with words separated by sep to words
int GetParamVector(valarray<double>& ParmVec,string ParamName,const char* InpFname); // same as GetParamValue in StrConv but reading full vector, returns 0 if ok
/*
int GetParamVector(vector<double>& ParmVec,string ParamName,const char* InpFname); // same as GetParamValue in StrConv but reading full vector, returns 0 if ok
template<class GenVec> int GetParamVector(GenVec& ParmVec,string ParamName,const char* InpFname);
void  CompileTemplate();
*/
#endif
