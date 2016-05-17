/* ~/c/MyClib/EscapeSequences.h      H. Burkhardt 2009
 
 
 for tests and references see   Test_terminal_escape_sequences
 as alternative consider also overstriking  http://en.wikipedia.org/wiki/Overstrike
 moved in 12/12/2009 to MyClib   -   maybe a little generic names, also not clear that escape sequences have any future
 */

#ifndef EscapeSequences_h
#define EscapeSequences_h 1

namespace MyESC
{
  
  static const char* ESC="\033"; // static really needed here to avoid duplicate symbols
  
  // (foreground) colors
  static const char* BLACK  ="\033[30m";
  static const char* RED    ="\033[31m";
  static const char* GREEN  ="\033[32m";
  static const char* YELLOW ="\033[33m";
  static const char* BLUE   ="\033[34m";
  static const char* MAGENTA="\033[35m";
  static const char* CYAN   ="\033[36m";
  static const char* WHITE  ="\033[37m";
  
  // background colors
  static const char* BG_BLACK  ="\033[40m";
  static const char* BG_RED    ="\033[41m";
  static const char* BG_GREEN  ="\033[42m";
  static const char* BG_YELLOW ="\033[43m";
  static const char* BG_BLUE   ="\033[44m";
  static const char* BG_MAGENTA="\033[45m";
  static const char* BG_CYAN   ="\033[46m";
  static const char* BG_WHITE  ="\033[47m";
  
  static const char* NORMAL="\033[0m";
  static const char* BOLD="\033[1m";
} // namespace MyESC
using namespace MyESC;
#endif
