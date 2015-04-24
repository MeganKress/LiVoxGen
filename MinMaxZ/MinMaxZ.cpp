/*
VoxGen, 2015

Megan Kress <kressmeg@msu.edu>

This program takes the input file VoxGenPar and its
user-defined categories.

The program may output multiple files:
    - R File to be examined with VoxGenR library
    - VoxData file
    - Lidar Metrics Flat File
    - Histogram Flat File
            (see VoxGenFlatFileFormats text file)


*/

#include <stdio.h>     /* time */
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <string>
#include <fstream>  // std::ifstream
#include <liblas/liblas.hpp>
#include <iomanip>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>


using namespace std;

namespace fs = boost::filesystem;



bool exists(string file)
{
    ifstream infile (file.c_str());
    return infile.good();
};


//Suppress output: https://bbs.archlinux.org/viewtopic.php?id=79378

int main(int argc, char *argv[])
{
    time_t timerI;

    time(&timerI);

    string vgfile;
    bool  fileEntry = false;
    string enteredFile;

    std::streambuf* cout_sbuf;

    vector<string> filesInDir;

    string inDir;

    if (argc > 1)
    {
        inDir = argv[1];
    } else {
      cout << "\nMust have file argument\n";
      return 0;
    }

    //**********************************
    // Boost Filesystem Stuff
    // Adapted from
    // http://www.boost.org/doc/libs/1_31_0/libs/filesystem/example/simple_ls.cpp
    //***********************************

    fs::path full_path( inDir );

    unsigned long file_count = 0;
    unsigned long dir_count = 0;

    if ( !fs::exists( full_path ) )
  {
    std::cout << "\nDirectory not found";
    return 1;
  }

    if ( fs::is_directory( full_path ) )
  {
    std::cout << "\nIn directory: "
              << full_path << "\n\n";
    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( full_path );
          dir_itr != end_iter;
          ++dir_itr )
    {
      try
      {
        if ( fs::is_directory( *dir_itr ) )
        {
          ++dir_count;
        }
        else
        {
          filesInDir.push_back(dir_itr->path().c_str());
          ++file_count;
        }
      }
      catch ( const std::exception & ex )
      {
        cout << "Error";
      }
    }
    std::cout << "\n" << file_count << " files\n"
              << dir_count << " directories\n";
  }
  else // must be a file
  {
    std::cout << "\nFound File (not directory): " << full_path << "\n";
  }

    //*******************************
    // End Boost Filesystem Stuff
    //*******************************

    double minZ = 10000000;
    double maxZ = 0;
    double minX = 10000000;
    double maxX = 0;
    double minY = 10000000;
    double maxY = 0;

    for(unsigned int ii = 0; ii < filesInDir.size(); ++ii) {

    string inFile = filesInDir.at(ii);



    ifstream ifs;
    ifs.open( inFile.c_str(), ios::in | ios::binary );

    liblas::Reader reader = liblas::Reader(ifs);

    liblas::Header const& header = reader.GetHeader();


    if(header.GetMinZ() < minZ)
    {
        minZ = header.GetMinZ();
    }
    if(header.GetMaxZ() > maxZ)
    {
        maxZ = header.GetMaxZ();
    }

    if(header.GetMinY() < minY)
    {
        minY = header.GetMinY();
    }
    if(header.GetMaxY() > maxY)
    {
        maxY = header.GetMaxY();
    }

    if(header.GetMinX() < minX)
    {
        minX = header.GetMinX();
    }
    if(header.GetMaxX() > maxX)
    {
        maxX = header.GetMaxX();
    }

}


    if(inDir[inDir.length()] != '/') inDir = inDir + "/";

    ofstream myfile;
    string outfile = inDir + "range.txt";
    cout << outfile;
    myfile.open (outfile.c_str());
    myfile << setprecision(12) << "MinX = " << minX;
    myfile << setprecision(12) << "\nMaxX = " << maxX;

    myfile << setprecision(12) << "\n\nMinY = " << minY;
    myfile << setprecision(12) << "\nMaxY = " << maxY;

    myfile << setprecision(12) << "\n\nMinZ = " << minZ;
    myfile << setprecision(12) << "\nMaxZ = " << maxZ;


};
