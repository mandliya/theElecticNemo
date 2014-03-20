#ifndef LSYSTEMDATA__H
#define LSYSTEMDATA__H
#include <string>
#include <sstream>
#include <map>
using std::string;
#include "parseLsysXML.h"

class Lsystemdata {
public:
  // if fn != filename, getInstance sets filename to fn and calls reset():
  static Lsystemdata& getInstance(const std::string& fn);

  void displayData() const;
  bool getXmlBool(const string&) const;
  const string& getXmlStr(const string&) const;
  
  float getXmlFloat(const string&) const;
  int getXmlInt(const string&) const;

  const std::map<string, string>& getXmlData() const;
  const std::multimap<char, std::string>& getGrammar() const { 
    return grammar; 
  }
private:
  static Lsystemdata& instance;
  string filename;
  std::map<string, string> gameData;
  std::multimap<char, std::string> grammar;

  Lsystemdata(const string&);
  Lsystemdata(const Lsystemdata&);
  Lsystemdata& operator=(const Lsystemdata&);
  void reset();
};
#endif
