#include <cstring>
#include <sstream>
#include "parseLsysXML.h"
using std::string;
using std::cout;
using std::endl;

void ParseLsysXML::start(const char *el, const char *attr[]) {
  ParseXML::start(el, attr);
  for (int i = 0; attr[i]; i += 2) {
    if ( string(el)=="rule" && attr[i]==string("nonterminal") ) { 
      lastNonTerminal = attr[i+1];
    }
  }
}

void ParseLsysXML::chars(const char *text, int textlen) {
  ParseXML::chars(text, textlen);
  // The text is not zero terminated; thus we need the  length:
  string str(text, textlen);
  // The text is likely to have trailing white space, e.g. newline, etc
  stripTrailWhiteSpace(str);
  if ( str.size() ) {
    if ( lastElementTag == "rule") {
      grammar.insert(std::pair<char,string>(lastNonTerminal[0], str));
    }
  }
}


