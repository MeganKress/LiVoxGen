#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>

#include "vgpar.h"
using namespace std;

vgpar::vgpar(string f)
{
  ifstream infile(f.c_str(), ios::in);
  if ( !infile ) {
    cerr << "Parameter file could not be opened." << endl;
    exit(1);
  }

  string temp, key, value;
  temp = "";


  while (getline(infile,temp)){
    istringstream input (temp);

    if (temp[0] != '#') {
      input >> key >> value;
      vgMap.insert(pair<string, string> (key,value));
    }
  }

}

string vgpar::getString(string k)
{
    return vgMap[k];
}

double vgpar::getNum(string k)
{
    string v = vgMap[k];
//    char chars [v.length()];
//    for(unsigned int i = 0; i < v.length(); ++i)
//    {
//        chars[i] = v[i];
//    }

    char *chars=new char[v.size()+1];
    chars[v.size()]=0;
    memcpy(chars,v.c_str(),v.size());

    return strtod(chars, NULL);

}
