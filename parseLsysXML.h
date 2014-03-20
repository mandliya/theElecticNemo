#ifndef PARSE_LSYS_XML
#define PARSE_LSYS_XML

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <expat.h>
#include "parseXML.h"

class ParseLsysXML : public ParseXML {
public:
  ParseLsysXML(const std::string& fn) : 
    ParseXML(fn),
    grammar()
  { 
    std::cout << "\tMaking ParseLsysXML for: " << filename << std::endl; 
  }

  ~ParseLsysXML() {
    std::cout << "\tNuking ParseLsysXML for: " << filename << std::endl; 
  }

  const std::multimap<char, std::string> getGrammar() const { 
    return grammar; 
  }

private:
  std::multimap<char, std::string> grammar;

  virtual void start(const char *el, const char *attr[]);
  virtual void chars(const char *text, int textlen);

  ParseLsysXML& operator=(const ParseLsysXML&);
  ParseLsysXML(const ParseLsysXML&);
};
#endif
