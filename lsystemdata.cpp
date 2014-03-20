#include "lsystemdata.h"

Lsystemdata& Lsystemdata::getInstance(const std::string& fn) {
  static Lsystemdata instance(fn);
  if ( instance.filename != fn ) {
    instance.filename = fn;
    instance.reset();
  }
  return instance;
}

void Lsystemdata::reset() {
  gameData.clear();
  grammar.clear();
  ParseLsysXML parser(filename);
  parser.parseXML(); 
  gameData = parser.getXmlData();
  grammar = parser.getGrammar();
  //displayData();
}

Lsystemdata::Lsystemdata(const string& fn ) :
  filename(fn),
  gameData(),
  grammar()
{ 
  reset();
}


const std::map<string, string>& Lsystemdata::getXmlData() const { 
  if ( gameData.size() == 0 ) {
    throw std::string("xmlData map is empty. Did you call parse()?");
  }
  return gameData; 
}


bool Lsystemdata::getXmlBool(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find boolean tag ")+tag+string(" in xml");
  else {
    if ( ptr->second == "true" ) return true;
    else return false;
  }
}

int Lsystemdata::getXmlInt(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find integer tag ")+tag+string(" in xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    int data;
    strm >> data;
    return data;
  }
}

// An error in declaring data as an int has been noticed by Yingzia Chen;
// the fix is to change line 45 to:
// float data;
float Lsystemdata::getXmlFloat(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find float tag ")+tag+string(" in xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    float data;
    strm >> data;
    return data;
  }
}

const string& Lsystemdata::getXmlStr(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find string tag ")+tag+string(" in xml");
  else return ptr->second;
}

void Lsystemdata::displayData() const {
  std::map<string, string>::const_iterator ptr = gameData.begin();
  while ( ptr != gameData.end() ) {
    std::cout << ptr->first << ", " << ptr->second << std::endl;
    ++ptr;
  }
}


