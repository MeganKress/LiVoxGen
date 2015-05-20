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
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>


using namespace std;

void thread(string segment, string file, vector<liblas::Bounds<double> > filters, int j)
{

    unsigned int k = 0;
    for(unsigned int i = j; i < filters.size() + j; ++i)
    {



        liblas::Bounds<double> bounds = filters.at(k++);

        int ptCount = 0;

        ifstream ifs;
        ifs.open( file.c_str(), ios::in | ios::binary );

        liblas::Reader reader = liblas::Reader(ifs);

        liblas::Header header = reader.GetHeader();

        std::vector<liblas::FilterPtr> myfilter;

        liblas::FilterPtr b_filter = liblas::FilterPtr(new liblas::BoundsFilter(bounds));

        myfilter.push_back(b_filter);

        vector<double>::iterator  xmin, xmax, ymin, ymax, zmin, zmax;
        std::vector<double> x_v, y_v, z_v;

        reader.SetFilters(myfilter);


//        cout << "read points\n";

        if(reader.ReadNextPoint())
        {
            std::ofstream ofs;

            stringstream outFile;
            outFile << segment << "_" << i << ".las";
            string outFiles = outFile.str();


            ofs.open(outFiles.c_str(), ios::out | ios::binary);

            liblas::Writer writer	(ofs, header);

            liblas::Point p = reader.GetPoint();


            ++ptCount;


            writer.WritePoint(p);


            x_v.push_back(p.GetX());
            y_v.push_back(p.GetY());
            z_v.push_back(p.GetZ());


            while (reader.ReadNextPoint())
            {

                liblas::Point p = reader.GetPoint();


                ++ptCount;


                writer.WritePoint(p);


                x_v.push_back(p.GetX());
                y_v.push_back(p.GetY());
                z_v.push_back(p.GetZ());



            }

            xmin = min_element(x_v.begin(), x_v.end());
            xmax = max_element(x_v.begin(), x_v.end());
            ymin = min_element(y_v.begin(), y_v.end());
            ymax = max_element(y_v.begin(), y_v.end());
            zmin = min_element(z_v.begin(), z_v.end());
            zmax = max_element(z_v.begin(), z_v.end());

            liblas::Bounds<double> extents(*xmin,*ymin,*zmin,*xmax,*ymax,*zmax);

            static liblas::Header header1 ( header );


            header1.SetExtent(extents);

            writer.SetHeader(header1);

            writer.WriteHeader();


            ofs.close();

    }
    }
}


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



     liblas::Reader reader0 = liblas::Reader(ifs);
     liblas::Reader reader1 = liblas::Reader(ifs);
     liblas::Reader reader2 = liblas::Reader(ifs);
     liblas::Reader reader3 = liblas::Reader(ifs);
     liblas::Reader reader4 = liblas::Reader(ifs);
     liblas::Reader reader5 = liblas::Reader(ifs);


    std::vector<liblas::Bounds<double> > filter0, filter1, filter2, filter3, filter4, filter5;



    int j1 = filters.size()/6;
    int j2 = j1 + j1;
    int j3 = j2 + j1;
    int j4 = j3 + j1;
    int j5 = j4 + j1;

    filter0.reserve(j1 + 5);
    filter1.reserve(j1 + 5);
    filter2.reserve(j1 + 5);
    filter3.reserve(j1 + 5);
    filter4.reserve(j1 + 5);
    filter5.reserve(j1 + 5);

//    cout << j1 << '\n';
//    cout << filters.size();

    filter0.insert(filter0.begin(), filters.begin(), filters.begin() + j1 - 1);
    filter1.insert(filter1.begin(), filters.begin() + j1, filters.begin() + j2 - 1);
    filter2.insert(filter2.begin(), filters.begin() + j2, filters.begin() + j3 - 1);
    filter3.insert(filter3.begin(), filters.begin() + j3, filters.begin() + j4 - 1);
    filter4.insert(filter4.begin(), filters.begin() + j4, filters.begin() + j5 - 1);
    filter5.insert(filter5.begin(), filters.begin() + j5, filters.end());




    boost::thread t0{thread, segment, file, filter0, 0};
    boost::thread t1{thread, segment, file, filter1, j1};
    boost::thread t2{thread, segment, file, filter2, j2};
    boost::thread t3{thread, segment, file, filter3, j3};
    boost::thread t4{thread, segment, file, filter4, j4};
    boost::thread t5{thread, segment, file, filter5, j5};

    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();


//    boost::filesystem::path full_path( boost::filesystem::current_path() );
////    cout << full_path << file;
//    file = full_path.string() + "/" + file;
//    remove(file.c_str());

    return 0;

}

