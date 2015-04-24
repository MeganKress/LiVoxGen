/*
SplitLas, 2015

Megan Kress <kressmeg@msu.edu>

This program takes the input of a LAS
file and splits it into 195mx195m LAS files.

*/

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>  // std::ifstream
#include <liblas/liblas.hpp>
#include <liblas/filter.hpp>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>


using namespace std;


int main(int argc, char *argv[])
{

    string file = argv[1];

    double width = 195;

    if(argc > 2)
    {
        width = strtod(argv[2],NULL);
    }

//    cout << "\n" << width << "\n";




    string delimiter = "/";
    string delimiter1 = ".";


    string segment = file.substr(0,file.find(delimiter1));

    cout << segment << "\n";

    ifstream ifs;
    ifs.open( file.c_str(), ios::in | ios::binary );

    liblas::Reader reader = liblas::Reader(ifs);

    liblas::Header header = reader.GetHeader();

    double minx = header.GetMinX();
    double maxx = header.GetMaxX();
    double miny = header.GetMinY();
    double maxy = header.GetMaxY();
    double minz = header.GetMinZ();
    double maxz = header.GetMaxZ();

    double x = minx;
    double y = miny;




    std::vector<liblas::Bounds<double> > filters;


    while(x < maxx)
    {
        while(y < maxy)
        {
            liblas::Bounds<double> bound(x,y,minz,x+width,y+width,maxz);

            filters.push_back(bound);

            y += width;
        }

        y = miny;
        x += width;
    }


    for(unsigned int i = 0; i < filters.size(); ++i)
    {


//        cout << '\n' << i << " out of " << filters.size();
        std::ofstream ofs;

        stringstream outFile;
        outFile << segment << "_" << i << ".las";
        string outFiles = outFile.str();

        ofs.open(outFiles.c_str(), ios::out | ios::binary);

        liblas::Writer writer	(ofs, header);

        liblas::Bounds<double> bounds = filters.at(i);

        int ptCount = 0;

        reader.Reset();

        std::vector<liblas::FilterPtr> myfilter;

        liblas::FilterPtr b_filter = liblas::FilterPtr(new liblas::BoundsFilter(bounds));

        myfilter.push_back(b_filter);

        vector<double>::iterator  xmin, xmax, ymin, ymax, zmin, zmax;
        std::vector<double> x_v, y_v, z_v;

        reader.SetFilters(myfilter);

        while (reader.ReadNextPoint())
        {

            liblas::Point p = reader.GetPoint();
            ++ptCount;

//
//            liblas::Point pp (p);
//            pp.SetData(p.GetData());
//            pp.SetCoordinates(p.GetX(),p.GetY(),p.GetZ());

//            cout << pp.GetX() << ',' << pp.GetY() << ','<< pp.GetZ() << '\n';

            writer.WritePoint(p);


            x_v.push_back(p.GetX());
            y_v.push_back(p.GetY());
            z_v.push_back(p.GetZ());



        }


        if(ptCount == 0)
        {
            remove(outFiles.c_str());
        } else {


            xmin = min_element(x_v.begin(), x_v.end());
            xmax = max_element(x_v.begin(), x_v.end());
            ymin = min_element(y_v.begin(), y_v.end());
            ymax = max_element(y_v.begin(), y_v.end());
            zmin = min_element(z_v.begin(), z_v.end());
            zmax = max_element(z_v.begin(), z_v.end());

            liblas::Bounds<double> extents(*xmin,*ymin,*zmin,*xmax,*ymax,*zmax);

            static liblas::Header header1 ( header );
//
//            header1.SetPointRecordsCount(ptCount);
//
//            header1.SetExtent(extents);
//
//            writer.WriteHeader();
//
//            writer.SetHeader(header1);

            header1.SetExtent(extents);

            writer.SetHeader(header1);

            writer.WriteHeader();

        }

        ofs.close();


    }



}

