#include <stdio.h>
#include <time.h>       /* time */
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <string>
#include <set>
#include <cmath>
#include <exception>
#include <fstream>  // std::ifstream
#include "liblas.hpp"
#include <string>



using namespace std;


int main(int argc, char *argv[])
{

    string inFile = argv[1];

    ifstream ifs;

    ifs.open( inFile.c_str(), ios::in | ios::binary );

    liblas::Reader reader = liblas::Reader(ifs);

    liblas::Header const& header = reader.GetHeader();

    liblas::SpatialReference srs = header.GetSRS();

    cout << "Spatial Reference: " << srs.GetWKT() << '\n';


}
