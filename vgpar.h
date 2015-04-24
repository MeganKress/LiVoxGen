#ifndef VGPAR_H
#define VGPAR_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

class vgpar
{
 public:
  vgpar(string f);
  string getString(string);
  double getNum(string);

 private:
  map<string, string> vgMap;

};
#endif
