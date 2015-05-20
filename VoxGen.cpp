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
#include <liblas/liblas.hpp>
#include "kdtree.h"
#include <iomanip>
#include <map>
#include <math.h>
#include "vgpar.h"
#include "point.h"
#include "voxel.h"
#include "voxcol.h"
#include "voxdata.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

#include <gdal/cpl_conv.h> // for CPLMalloc()
#include <gdal/gdal_priv.h>
//#include <gdal/gdal_alg.h>
//#include <gdal/gdalwarper.h>
//#include <gdal/ogr_spatialref.h>
//#include <gdal/cpl_conv.h>
//#include <gdal/cpl_minixml.h>

using namespace std;

namespace fs = boost::filesystem;

namespace std
{
    template<> struct less<Point>
    {
       bool operator() (const Point& lhs, const Point& rhs)
       {
           return lhs.x < rhs.x || lhs.y < rhs.y || lhs.z < rhs.z;
       }
    };

}

bool exists(string file)
{
    ifstream infile (file.c_str());
    return infile.good();
};



void toR(vector<VoxCol> voxColumnsVector, int numVoxCols, int n, int numInCol, string outFile)
{

    ofstream myfile;



    myfile.open (outFile.c_str());
    cout << "\nWriting data to a file...\n";

    myfile << "\nVoxCol <- matrix(nrow = " << numVoxCols << ", ncol = 3)";
    myfile << "\nVoxels <- matrix(nrow = " << n << ", ncol = 4)";
    myfile << "\nVoxList <- list()";

    if(numVoxCols == 0) return;

    int voxIndex = 1;

    for(int i = 1; i <= numVoxCols; ++i)
  {

     VoxCol voxcol = voxColumnsVector.back();



     myfile << "\nVoxCol[" << i << ", 1] <- " << setprecision(20) << voxcol.xC;
     myfile << "\nVoxCol[" << i << ", 2] <- " << setprecision(20) << voxcol.yC;


    myfile << "\nVoxColList <- matrix(ncol = 4, nrow = " << numInCol << ")";

     int index = numInCol;

    while(voxcol.voxels.size() > 0)
    {


      Voxel voxel = voxcol.voxels.back();

      myfile << "\nVoxels[" << voxIndex << ", 1] <- " << setprecision(20) << voxel.cX;
      myfile << "\nVoxels[" << voxIndex << ", 2] <- " << setprecision(20) << voxel.cY;
      myfile << "\nVoxels[" << voxIndex << ", 3] <- " << setprecision(20) << voxel.cZ;
      myfile << "\nVoxels[" << voxIndex << ", 4] <- " << setprecision(20) << voxel.pointNum;

      myfile << "\nVoxColList[" << index << ", 1] <-" << setprecision(20) << voxel.cX;
      myfile << "\nVoxColList[" << index << ", 2] <-" << setprecision(20) << voxel.cY;
      myfile << "\nVoxColList[" << index << ", 3] <-" << setprecision(20) << voxel.cZ;
      myfile << "\nVoxColList[" << index << ", 4] <-" << setprecision(20) << voxel.pointNum;

      voxcol.voxels.pop_back();
      voxIndex++;
      index--;

    }

    myfile << "\nVoxList[[" << i << "]] <- VoxColList";


    voxColumnsVector.pop_back();

  }

   cout << "Done.";

   myfile.close();
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

    bool quiet = false;

    if (argc > 3)
    {
        cout_sbuf = std::cout.rdbuf(); // save original sbuf
        std::ofstream   fout("/dev/null");
        std::cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'

        fileEntry = true;
        vgfile = argv[1];
        enteredFile = argv[2];
        quiet = true;
    }
    else if (argc > 2)
    {
      fileEntry = true;
      vgfile = argv[1];
      enteredFile = argv[2];
    }
    else if (argc > 1)
    {
        vgfile = argv[1];
    }else {
        cout << "\nEnter /path/to/VoxGenPar: ";
        cin >> vgfile;
        vgfile = vgfile.c_str();
    }

    cout << '\n' << vgfile << '\n';
    vgpar par (vgfile);

    vector<string> filesInDir;

    string fORd = par.getString("fileORdir");

    if(fileEntry)
    {
      filesInDir.push_back(enteredFile);

    }else if(fORd[0] == 'd'){
    //**********************************
    // Boost Filesystem Stuff
    // Adapted from
    // http://www.boost.org/doc/libs/1_31_0/libs/filesystem/example/simple_ls.cpp
    //***********************************

    string inDir = par.getString("inDir");

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
    }else
    {
        string selectedFile = par.getString("inFile");
        filesInDir.push_back(selectedFile);
    }

    for(unsigned int ii = 0; ii < filesInDir.size(); ++ii) {

    cout << "\n***********************************\n";

    string inFile = filesInDir.at(ii);

    if(par.getString("routput")[0] == 'y') if(!exists(par.getString("outDirectory"))){
        cout << "\nR output directory does not exist. Exiting...\n";
        return 0;
    }
    if(par.getString("hff")[0] == 'y') if(!exists(par.getString("hffDir"))){
        cout << "\nHistogram flat file output directory does not exist. Exiting...\n";
        return 0;
    }
    if(par.getString("lmff")[0] == 'y') if(!exists(par.getString("lmffDir"))){
        cout << "\nLidar metrics flat file output directory does not exist. Exiting...\n";
        return 0;
    }

    ifstream ifs;
    ifs.open( inFile.c_str(), ios::in | ios::binary );

    liblas::Reader reader = liblas::Reader(ifs);

    liblas::Header const& header = reader.GetHeader();

//    cout << '\n' << "Signature: " << header.GetFileSignature() << '\n';
//    cout << "Points count: " << header.GetPointRecordsCount() << '\n';

    liblas::SpatialReference srs = header.GetSRS();

//    cout << "Spatial Reference: " << srs.GetWKT() << '\n';

    cout << "\nMin X: " << setprecision(12) << header.GetMinX() << '\n';
    cout << "Max X: " << setprecision(12) << header.GetMaxX() << '\n';
    cout << "Min Y: " << setprecision(12) << header.GetMinY() << '\n';
    cout << "Max Y: " << setprecision(12) << header.GetMaxY() << '\n';
    cout << "Min Z: " << setprecision(12) << header.GetMinZ() << '\n';
    cout << "Max Z: " << setprecision(12) << header.GetMaxZ() << '\n' << '\n';




    string filter;

    filter = par.getString("filter");

    if(filter[0] == 'y' || filter[0] == 'Y')
    {

    // *************************************
    // Filter by Classification
    // *************************************

    string classChoice;

  std::vector<liblas::Classification> classes;

  classChoice = par.getString("classes");

  for(unsigned int i = 0; i < classChoice.length(); ++i)
    {
        if(classChoice[i] == 'A') break;
        if(classChoice[i] == 'U') classes.push_back(liblas::Classification(1));
        if(classChoice[i] == 'G') classes.push_back(liblas::Classification(2));
        if(classChoice[i] == 'L') classes.push_back(liblas::Classification(3));
        if(classChoice[i] == 'M') classes.push_back(liblas::Classification(4));
        if(classChoice[i] == 'H') classes.push_back(liblas::Classification(5));
        if(classChoice[i] == 'B') classes.push_back(liblas::Classification(6));
        if(classChoice[i] == 'W') classes.push_back(liblas::Classification(9));
    }

    std::vector<liblas::FilterPtr> filters;
    liblas::FilterPtr class_filter = liblas::FilterPtr(new liblas::ClassificationFilter(classes));

    // eInclusion means to keep the classes that match.  eExclusion would
    // throw out those that matched
    class_filter->SetType(liblas::FilterI::eInclusion);
    filters.push_back(class_filter);

    // ************************************
    // End Filter by Classification
    // ************************************

    // ************************************
    // Filter by return
    // ************************************

    string returnChoice;
    liblas::ReturnFilter::return_list_type returns;


    returnChoice = par.getString("returns");

    for(unsigned int i = 0; i < returnChoice.length(); ++i)
    {
        if(classChoice[i] == 'A') break;
        if(classChoice[i] == '1') returns.push_back(1);
        if(classChoice[i] == '2') returns.push_back(2);
        if(classChoice[i] == '3') returns.push_back(3);
        if(classChoice[i] == '4') returns.push_back(4);
        if(classChoice[i] == '5') returns.push_back(5);
    }


    liblas::FilterPtr return_filter = liblas::FilterPtr(new liblas::ReturnFilter(returns, false));
    return_filter->SetType(liblas::FilterI::eInclusion);
    filters.push_back(return_filter);

    // *************************************
    // End filter by return
    // *************************************

//
//     *************************************
//     Filter by Scan Angle
//     *************************************

    double angleChoice;
    liblas::ContinuousValueFilter<double>::filter_func f;
    liblas::ContinuousValueFilter<double>::compare_func c;
//    cout << "\n\nSelect Maximum Scan Angle.";
//    cout << "\nAngle: ";

    //cin >> angleChoice;

    angleChoice = par.getNum("angle");

    f = &liblas::Point::GetScanAngleRank;
    c = std::less_equal<double>();

    liblas::FilterPtr angle_filterptr = liblas::FilterPtr(new liblas::ContinuousValueFilter<double>(f, angleChoice, c));

    angle_filterptr->SetType(liblas::FilterI::eInclusion);

    filters.push_back(angle_filterptr);

    c = std::greater_equal<double>();
    angleChoice *= -1;

    liblas::FilterPtr angle_filterptr2 = liblas::FilterPtr(new liblas::ContinuousValueFilter<double>(f, angleChoice, c));

    angle_filterptr2->SetType(liblas::FilterI::eInclusion);

    filters.push_back(angle_filterptr2);


    // ***************************************
    // End Filter by Scan Angle
    // ***************************************


   reader.SetFilters(filters);
    }


    kdtree *kd = kd_create(3);
    struct kdres *results;
    cout << "\nAdding points to tree..." << '\n';

   int totalPoints = 0;

   vector<Point> allPoints;



   if(par.getString("hff")[0] == 'y')
    {
      while (reader.ReadNextPoint())
      {
        liblas::Point const& p = reader.GetPoint();

        double point[3];

        point[0] = p.GetX();
        point[1] = p.GetY();
        point[2] = p.GetZ();

//        Point pt (p.GetX(), p.Ge)
//        allPoints.push_back(Point (p.GetX(), p.GetY(), p.GetZ()));

//        cout << p.GetX() << ',' << p.GetY() << ','<< p.GetZ() << '\n';

        ++totalPoints;

        kd_insert3(kd, p.GetX(), p.GetY(), p.GetZ(), NULL );
      };

    }

  if(totalPoints == 0)
  {
    ifs.close();
    if (quiet) cout.rdbuf(cout_sbuf);
    return 0;
  }

  cout << "\nPoints in kdtree: " << totalPoints <<'\n';
  //Set min and max values for each dimension
  double x1 = header.GetMinX();
  double x2 = header.GetMaxX();

  double y1 = header.GetMinY();
  double y2 = header.GetMaxY();

  double z1 = header.GetMinZ();
  double z2 = header.GetMaxZ();

  if(par.getNum("z1") != 0) z1 = par.getNum("z1");

  if(par.getNum("z2") != 0) z2 = par.getNum("z2");

  double zStart;

  double voxelsLo = 0;
  if(par.getNum("zmin") != 0) zStart = par.getNum("zmin");
  else zStart = z1;

  double ht = par.getNum("height");

//  zStart += ht/2.0;
  double z0 = zStart;

  while(zStart <= z1)
  {
    zStart += ht;
    ++voxelsLo;
  }

  zStart -= 2.0*ht;
  --voxelsLo;
  z1 = zStart;

  cout << "\n...Done creating tree.\n";



  double b;
  double h;
  double xcoord;
  double ycoord;
  double xcoord2;
  double ycoord2;


  b = par.getNum("base");


  h = par.getNum("height");


  xcoord = par.getNum("x1");

  if(xcoord == 0) xcoord = x1;

  xcoord2 = par.getNum("x2");

  if(xcoord2 == 0) xcoord2 = x2;

  ycoord = par.getNum("y1");

  if(ycoord == 0) ycoord = y1;


  ycoord2 = par.getNum("y2");

  if(ycoord2 == 0) ycoord2 = y2;


  int numVoxX = ceil((xcoord2 - xcoord) / b);
  ++numVoxX;
  int numVoxY = ceil((ycoord2 - ycoord) / b);
  ++numVoxY;

  int numVoxCols = numVoxX * numVoxY;



  vector<VoxCol> voxColumnsVector;

  int numVox = 0;
  double z = z1;
  while(z <= z2)
  {
    z += ht;
    ++numVox;
  }

  z += ht;
  ++numVox;


  z2 = z;

  b = b/2.0;
  h = h/2.0;


  //Set Radius of Sphere
  double r = sqrt(2*b*b + h*h);

  double x = xcoord;
  double y = ycoord;
  double zcoord = z1;




  int totalnumVox = numVoxCols * numVox;
//
//  vector<Voxel> allVoxels;
//
//  map<Point,Voxel> voxMap;


  int kdPointCount = 0;




  if(par.getString("hff")[0] == 'y')
  {
      cout << "\nNumber of Voxels: " << totalnumVox << '\n';


      cout << "Creating Voxels...\n";

        int hundVox = totalnumVox/100.0;
        int percent = 0;
        int voxCount = 0;

      int voxCt = 0;



      time_t timer;

      time(&timer);

      for(int i = 0; i < numVoxY; ++i)
      {
//        if(i == 0) y += b;

        for(int j = 0; j < numVoxX; ++j)
        {

//          if(j == 0) x += b;

          VoxCol voxelColumn;
          voxelColumn.pic = 0;

          for(int k = 0; k < numVox; ++k)
          {
//            if(k == 0) zcoord += h;


            results = kd_nearest_range3(kd, x, y, zcoord, r);

            int pointsNum = 0;

            Voxel voxel;
//            cout << "Points from nearest range: " << pointsNum << '\n';

            voxel.cX = x;
            voxel.cY = y;
            voxel.cZ = zcoord;
            voxel.d = b;
            voxel.h = h;

//            cout << x << ',' << y << ',' << zcoord << ',' << pointsNum << '\n';

//            double * posx = (double*) malloc(sizeof(double));
//            double * posy = (double*) malloc(sizeof(double));
//            double * posz = (double*) malloc(sizeof(double));

            double pos [3] ;

//            kdPointCount += kd_res_size(results);

            while( !kd_res_end( results ) ) {

                /* get the position of the current result item */
                kd_res_item( results, pos );

                Point point (pos[0], pos[1], pos[2]);

                if(voxel.inVox(pos[0], pos[1], pos[2])) {
                    voxel.pointsInVox.push_back(point);
                    ++pointsNum;
//                    allPoints.resize(std::remove(allPoints.begin(), allPoints.end(), point) - allPoints.begin());
                }


                /* go to the next entry */
                kd_res_next( results );


          }

            kd_res_free( results );
//            free(posx);
//            free(posy);
//            free(posz);

            voxel.pointNum = voxel.pointsInVox.size();



            voxelColumn.voxels.push_back(voxel);
            voxelColumn.pic += voxel.pointNum;
            kdPointCount += voxel.pointNum;

//            ++ voxCount;
//            if(hundVox > 0){
//           if(voxCount%hundVox == 0)
//            {
//                percent++;
//                cout << percent << "...\n";
//            }
//            }

           zcoord += 2*h;
          }


           voxelColumn.xC = x;
           voxelColumn.yC = y;


           voxelColumn.init(-99);
           if(voxelColumn.pic > 0) voxColumnsVector.push_back(voxelColumn);




          zcoord = z1;
          x = x + 2*b;

        }
          x = xcoord;
          y = y + 2*b;
      }




      cout << "\nDone creating voxels.\n";

       time_t timer2;

      time(&timer2);

        cout << "\nTime Elapsed Creating Voxels: " << timer2 - timer;

        cout << "\nPoints added: " << kdPointCount <<'\n';
        cout << "AllPoints: " << allPoints.size() << '\n';

  } else {

      for(int i = 0; i < numVoxY; ++i)
      {
        if(i == 0) y += b;

        for(int j = 0; j < numVoxX; ++j)
        {

          if(j == 0) x += b;

          VoxCol voxelColumn;

           voxelColumn.xC = x;
           voxelColumn.yC = y;

           voxelColumn.init(-99);
           voxColumnsVector.push_back(voxelColumn);


          x = x + 2*b;

        }
          x = xcoord;
          y = y + 2*b;
      }


    kd_free( kd );

  }




    // ***************************************
    // Parse file name to obtain acquisition
    // and segment identifications.
    // ***************************************



    string fileName = inFile;

    string delimiter = "/";
    string delimiter1 = "_";
    string delimiter2 = ".";
    string delimiter3 = "-";

    string ac, seg, splitNum;


    size_t pos = 0;
    size_t pos1 = 0;
    size_t pos2 = 0;

    while ((pos = fileName.find(delimiter)) != string::npos) {
         fileName.erase(0, pos + delimiter.length());
    }

    ac = par.getString("acquisition");

    if(par.getString("split")[0] == 'y')
    {
        pos = fileName.find(delimiter1);
        seg = fileName.substr(0, pos);
        fileName.erase(0, pos + delimiter1.length());
        pos = fileName.find(delimiter2);
        splitNum = fileName.substr(0, pos);

    } else {

        pos = 0;

        int word = 0;
        while ((pos = fileName.find(delimiter1)) != string::npos) {
            pos = fileName.find(delimiter1);
            if(word != 0) ac += "_";
            ac += fileName.substr(0, pos);
            fileName.erase(0, pos + delimiter1.length());
            ++word;
        }

        pos = fileName.find(delimiter2);
        seg = fileName.substr(0, pos);
        splitNum = "";
    }


    // ***************************************
    // GDAL http://www.gdal.org/gdal_tutorial.html
    //
    // This section is run if the user wishes
    // to create a lidar metrics flat file.
    // ***************************************


    if(par.getString("lmff")[0] == 'y' || par.getString("lmff")[0] == 'Y')
    {
        cout << "\nReading metrics...\n";




        vector<string> filevect;
        vector<int> metricvect;
        vector<string> keyvgpar;

        keyvgpar.push_back("all_d0");
        keyvgpar.push_back("all_d1");
        keyvgpar.push_back("all_d2");
        keyvgpar.push_back("all_d3");
        keyvgpar.push_back("all_d4");
        keyvgpar.push_back("all_d5");
        keyvgpar.push_back("all_d6");
        keyvgpar.push_back("all_d7");
        keyvgpar.push_back("all_d8");
        keyvgpar.push_back("all_d9");
        keyvgpar.push_back("all_kurt");
        keyvgpar.push_back("all_mean");
        keyvgpar.push_back("all_p10");
        keyvgpar.push_back("all_p20");
        keyvgpar.push_back("all_p30");
        keyvgpar.push_back("all_p40");
        keyvgpar.push_back("all_p50");
        keyvgpar.push_back("all_p60");
        keyvgpar.push_back("all_p70");
        keyvgpar.push_back("all_p80");
        keyvgpar.push_back("all_p90");
        keyvgpar.push_back("all_qmean");
        keyvgpar.push_back("all_refl_kurt");
        keyvgpar.push_back("all_refl_mean");
        keyvgpar.push_back("all_refl_skew");
        keyvgpar.push_back("all_refl_stdev");
        keyvgpar.push_back("all_skew");
        keyvgpar.push_back("all_stdev");
        keyvgpar.push_back("ground_aspect");
        keyvgpar.push_back("ground_elev_mean");
        keyvgpar.push_back("ground_refl_kurt");
        keyvgpar.push_back("ground_refl_mean");
        keyvgpar.push_back("ground_refl_skew");
        keyvgpar.push_back("ground_refl_stdev");
        keyvgpar.push_back("ground_slope");
        keyvgpar.push_back("pulse_density");
        keyvgpar.push_back("pulse_scan_angle");
        keyvgpar.push_back("returns_per_pulse");
        keyvgpar.push_back("shrub_mean");
        keyvgpar.push_back("shrub_refl_mean");
        keyvgpar.push_back("shrub_refl_stdev");
        keyvgpar.push_back("shrub_stdev");
        keyvgpar.push_back("tree_aad");
        keyvgpar.push_back("tree_crr");
        keyvgpar.push_back("tree_d0");
        keyvgpar.push_back("tree_d1");
        keyvgpar.push_back("tree_d2");
        keyvgpar.push_back("tree_d3");
        keyvgpar.push_back("tree_d4");
        keyvgpar.push_back("tree_d5");
        keyvgpar.push_back("tree_d6");
        keyvgpar.push_back("tree_d7");
        keyvgpar.push_back("tree_d8");
        keyvgpar.push_back("tree_d9");
        keyvgpar.push_back("tree_fcover");
        keyvgpar.push_back("tree_fract_all");
        keyvgpar.push_back("tree_iqr");
        keyvgpar.push_back("tree_kurt");
        keyvgpar.push_back("tree_mad");
        keyvgpar.push_back("tree_mean");
        keyvgpar.push_back("tree_p10");
        keyvgpar.push_back("tree_p20");
        keyvgpar.push_back("tree_p30");
        keyvgpar.push_back("tree_p40");
        keyvgpar.push_back("tree_p50");
        keyvgpar.push_back("tree_p60");
        keyvgpar.push_back("tree_p70");
        keyvgpar.push_back("tree_p80");
        keyvgpar.push_back("tree_p90");
        keyvgpar.push_back("tree_qmean");
        keyvgpar.push_back("tree_mean");
        keyvgpar.push_back("tree_refl_kurt");
        keyvgpar.push_back("tree_refl_mean");
        keyvgpar.push_back("tree_refl_skew");
        keyvgpar.push_back("tree_refl_stdev");
        keyvgpar.push_back("tree_rugosity");
        keyvgpar.push_back("tree_skew");
        keyvgpar.push_back("tree_stdev");
        keyvgpar.push_back("tree_vdr");
        keyvgpar.push_back("all_refl_max");
        keyvgpar.push_back("ground_refl_max");
        keyvgpar.push_back("tree_refl_max");
        keyvgpar.push_back("all_p100");
        keyvgpar.push_back("shrub_refl_max");
        keyvgpar.push_back("tree_p100");



        if(par.getString("namconv")[0]!='y' && par.getString("namconv")[0]!='Y')
        {

            for (unsigned int vg = 0; vg < keyvgpar.size(); ++vg)
            {
                string key = keyvgpar.at(vg);
                if(par.getString(key)[0] != '0')
                {
                    filevect.push_back(par.getString(key));
                    metricvect.push_back(vg);
                }
            }
        } else {
            if (par.getString("AKnam")[0]=='y' || par.getString("AKname")[0]=='Y')
        {


            if(par.getString("mapNam")[0] == 'y')
            {
                string metDirect = par.getString("mnDir");
                if(metDirect[metDirect.length() - 1] != '/') metDirect += '/';
                for (unsigned int vg = 0; vg < keyvgpar.size(); ++vg)
                {
                    if(par.getNum(keyvgpar.at(vg)))
                    {
                        string metFile = metDirect + keyvgpar.at(vg) + '/' + "mosaic.tif";

                        filevect.push_back(metFile);
                        metricvect.push_back(vg);

                    }
                }
            } else {
            string metDirect = par.getString("AKmetricsDir");
            if(metDirect[metDirect.length() - 1] != '/') metDirect += '/';
            for (unsigned int vg = 0; vg < keyvgpar.size(); ++vg)
            {
                if(par.getNum(keyvgpar.at(vg)))
                {
                    string metFile = metDirect + keyvgpar.at(vg) + '/' + seg + '_' + keyvgpar.at(vg) + ".tif";

                    filevect.push_back(metFile);
                    metricvect.push_back(vg);

                }
            }
            }
        } else {
            string metDirect = par.getString("metricsDir");
            if(metDirect[metDirect.length() - 1] != '/') metDirect += '/';
            for (unsigned int vg = 0; vg < keyvgpar.size(); ++vg)
            {
                if(par.getNum(keyvgpar.at(vg)))
                {
                    string metFile = metDirect + ac + '_' + keyvgpar.at(vg) + ".tif";

                    filevect.push_back(metFile);
                    metricvect.push_back(vg);

                }

            }
        }
        }

        GDALAllRegister();

        cout << "Reading metrics\n";

        for(unsigned int ss = 0; ss < filevect.size(); ++ss)
        {

            string tifFile = filevect.at(ss);

            int metricNum = metricvect.at(ss);

            GDALDataset  *mDataset;



            mDataset = (GDALDataset *) GDALOpen( tifFile.c_str(), GA_ReadOnly );

            double        adfGeoTransform[6];
            int        nXBlockSize, nYBlockSize;

            GDALRasterBand  *mBand;
            mBand = mDataset->GetRasterBand( 1 );


            float      *mData;

            mBand->GetBlockSize( &nXBlockSize, &nYBlockSize );

            mData = (float*) CPLMalloc(sizeof(float)*nXBlockSize * nYBlockSize);

            mDataset->GetGeoTransform( adfGeoTransform ) ;



            for(unsigned int cc = 0; cc < voxColumnsVector.size(); ++cc)
            {

                double xx = voxColumnsVector.at(cc).xC;
                double yy = voxColumnsVector.at(cc).yC;

                int xOffset, yOffset;

                xOffset = static_cast <int> ((xx - adfGeoTransform[0]) / adfGeoTransform[1]);
                yOffset = static_cast <int> ((yy - adfGeoTransform[3]) / adfGeoTransform[5]);

                mBand->RasterIO( GF_Read, xOffset, yOffset, 1, 1,
                                  mData, 1, 1, GDT_Float32,
                                  0, 0 );


           if(!isnan(*mData)) {
                voxColumnsVector.at(cc).inRaster = true;
                }

                switch (metricNum)
                {
                    case 0: voxColumnsVector.at(cc).d0 = *mData;
                        break;
                    case 1: voxColumnsVector.at(cc).d1 = *mData;
                        break;
                    case 2: voxColumnsVector.at(cc).d2 = *mData;
                        break;
                    case 3: voxColumnsVector.at(cc).d3 = *mData;
                        break;
                    case 4: voxColumnsVector.at(cc).d4 = *mData;
                        break;
                    case 5: voxColumnsVector.at(cc).d5 = *mData;
                        break;
                    case 6: voxColumnsVector.at(cc).d6 = *mData;
                        break;
                    case 7: voxColumnsVector.at(cc).d7 = *mData;
                        break;
                    case 8: voxColumnsVector.at(cc).d8 = *mData;
                        break;
                    case 9: voxColumnsVector.at(cc).d9 = *mData;
                        break;
                    case 10: voxColumnsVector.at(cc).kurt = *mData;
                        break;
                    case 11: voxColumnsVector.at(cc).mean = *mData;
                        break;
                    case 12: voxColumnsVector.at(cc).p10 = *mData;
                        break;
                    case 13: voxColumnsVector.at(cc).p20 = *mData;
                        break;
                    case 14: voxColumnsVector.at(cc).p30 = *mData;
                        break;
                    case 15: voxColumnsVector.at(cc).p40 = *mData;
                        break;
                    case 16: voxColumnsVector.at(cc).p50 = *mData;
                        break;
                    case 17: voxColumnsVector.at(cc).p60 = *mData;
                        break;
                    case 18: voxColumnsVector.at(cc).p70 = *mData;
                        break;
                    case 19: voxColumnsVector.at(cc).p80 = *mData;
                        break;
                    case 20: voxColumnsVector.at(cc).p90 = *mData;
                        break;
                    case 21: voxColumnsVector.at(cc).qmean = *mData;
                        break;
                    case 22: voxColumnsVector.at(cc).refl_kurt = *mData;
                        break;
                    case 23: voxColumnsVector.at(cc).refl_mean = *mData;
                        break;
                    case 24: voxColumnsVector.at(cc).refl_skew = *mData;
                        break;
                    case 25: voxColumnsVector.at(cc).refl_stdev = *mData;
                        break;
                    case 26: voxColumnsVector.at(cc).skew = *mData;
                        break;
                    case 27: voxColumnsVector.at(cc).stdev = *mData;
                        break;
                    case 28: voxColumnsVector.at(cc).ground_aspect = *mData;
                        break;
                    case 29: voxColumnsVector.at(cc).ground_elev_mean = *mData;
                        break;
                    case 30: voxColumnsVector.at(cc).ground_refl_kurt = *mData;
                        break;
                    case 31: voxColumnsVector.at(cc).ground_refl_mean = *mData;
                        break;
                    case 32: voxColumnsVector.at(cc).ground_refl_skew = *mData;
                        break;
                    case 33: voxColumnsVector.at(cc).ground_refl_stdev = *mData;
                        break;
                    case 34: voxColumnsVector.at(cc).ground_slope = *mData;
                        break;
                    case 35: voxColumnsVector.at(cc).pulse_density = *mData;
                        break;
                    case 36: voxColumnsVector.at(cc).pulse_scan_angle = *mData;
                        break;
                    case 37: voxColumnsVector.at(cc).returns_per_pulse = *mData;
                        break;
                    case 38: voxColumnsVector.at(cc).shrub_mean = *mData;
                        break;
                    case 39: voxColumnsVector.at(cc).shrub_refl_mean = *mData;
                        break;
                    case 40: voxColumnsVector.at(cc).shrub_refl_stdev = *mData;
                        break;
                    case 41: voxColumnsVector.at(cc).shrub_stdev = *mData;
                        break;
                    case 42: voxColumnsVector.at(cc).tree_aad = *mData;
                        break;
                    case 43: voxColumnsVector.at(cc).tree_crr = *mData;
                        break;
                    case 44: voxColumnsVector.at(cc).tree_d0 = *mData;
                        break;
                    case 45: voxColumnsVector.at(cc).tree_d1 = *mData;
                        break;
                    case 46: voxColumnsVector.at(cc).tree_d2 = *mData;
                        break;
                    case 47: voxColumnsVector.at(cc).tree_d3 = *mData;
                        break;
                    case 48: voxColumnsVector.at(cc).tree_d4 = *mData;
                        break;
                    case 49: voxColumnsVector.at(cc).tree_d5 = *mData;
                        break;
                    case 50: voxColumnsVector.at(cc).tree_d6 = *mData;
                        break;
                    case 51: voxColumnsVector.at(cc).tree_d7 = *mData;
                        break;
                    case 52: voxColumnsVector.at(cc).tree_d8 = *mData;
                        break;
                    case 53: voxColumnsVector.at(cc).tree_d9 = *mData;
                        break;
                    case 54: voxColumnsVector.at(cc).tree_fcover = *mData;
                        break;
                    case 55: voxColumnsVector.at(cc).tree_fract_all = *mData;
                        break;
                    case 56: voxColumnsVector.at(cc).tree_iqr = *mData;
                        break;
                    case 57: voxColumnsVector.at(cc).tree_kurt = *mData;
                        break;
                    case 58: voxColumnsVector.at(cc).tree_mad = *mData;
                        break;
                    case 59: voxColumnsVector.at(cc).tree_mean = *mData;
                        break;
                    case 60: voxColumnsVector.at(cc).tree_p10 = *mData;
                        break;
                    case 61: voxColumnsVector.at(cc).tree_p20 = *mData;
                        break;
                    case 62: voxColumnsVector.at(cc).tree_p30 = *mData;
                        break;
                    case 63: voxColumnsVector.at(cc).tree_p40 = *mData;
                        break;
                    case 64: voxColumnsVector.at(cc).tree_p50 = *mData;
                        break;
                    case 65: voxColumnsVector.at(cc).tree_p60 = *mData;
                        break;
                    case 66: voxColumnsVector.at(cc).tree_p70 = *mData;
                        break;
                    case 67: voxColumnsVector.at(cc).tree_p80 = *mData;
                        break;
                    case 68: voxColumnsVector.at(cc).tree_p90 = *mData;
                        break;
                    case 69: voxColumnsVector.at(cc).tree_qmean = *mData;
                        break;
                    case 70: voxColumnsVector.at(cc).tree_mean = *mData;
                        break;
                    case 71: voxColumnsVector.at(cc).tree_refl_kurt = *mData;
                        break;
                    case 72: voxColumnsVector.at(cc).tree_reflt_mean = *mData;
                        break;
                    case 73: voxColumnsVector.at(cc).tree_refl_skew = *mData;
                        break;
                    case 74: voxColumnsVector.at(cc).tree_refl_stdev = *mData;
                        break;
                    case 75: voxColumnsVector.at(cc).tree_rugosity = *mData;
                        break;
                    case 76: voxColumnsVector.at(cc).tree_skew = *mData;
                        break;
                    case 77: voxColumnsVector.at(cc).tree_stdev = *mData;
                        break;
                    case 78: voxColumnsVector.at(cc).tree_vdr = *mData;
                        break;
                    case 79: voxColumnsVector.at(cc).refl_max = *mData;
                        break;
                    case 80: voxColumnsVector.at(cc).ground_refl_max = *mData;
                        break;
                    case 81: voxColumnsVector.at(cc).tree_refl_max = *mData;
                        break;
                    case 82: voxColumnsVector.at(cc).p100 = *mData;
                        break;
                    case 83: voxColumnsVector.at(cc).shrub_refl_max = *mData;
                        break;
                    case 84: voxColumnsVector.at(cc).tree_p100 = *mData;
                        break;


                    default:
                        break;
                }

            }




            CPLFree(mData);

            GDALClose(mDataset);
        }

        cout << "Done reading metrics.\n";
    }


    // ***************************************
    // End GDAL Stuff
    // ***************************************



    string outFile;



    // ***************************************
    // Set up end of file names (based on id)
    // ***************************************


    VoxData data;
    data.numCols = numVoxCols;
    data.numHi = numVox;
    data.numPts = kdPointCount;
    data.voxcols = voxColumnsVector;



    data.acqid = ac;

    data.segid = seg;

    if(splitNum.compare("") != 0)
    {
        data.splitid = splitNum;
        splitNum = "-" + splitNum;
    }else {
        data.splitid = "1";
    }


    string id = "_" + ac + "_" + seg + splitNum;

    // ***************************************
    // VoxData file creation
    // ***************************************

//    string vDataFile = par.getString("voxdata");
//    if(vDataFile[0] == 'y'){
//        cout << "\n\nWriting VoxData to file...\n";
//        string vdatadir = par.getString("vdataDir") + "/" + par.getString("vdataTitle") + id + ".voxdata";
//        data.toFile(vdatadir.c_str());
//        cout << "Done writing VoxData to file.\n";
//    }


    // ***************************************
    // R file creation
    // ***************************************



    string outDir = par.getString("outDirectory");

    string outName = par.getString("outName");

     outFile = outDir + "/" + outName + id + ".R";

//    string response = par.getString("routput");
//
//    if (response[0] == 'y') {
//        cout << "\n\nWriting data to R file...\n";
//        toR(voxColumnsVector, numVoxCols, totalnumVox, numVox, outFile);
//        cout << "Done writing data to R file.\n";
//    }


    // ***************************************
    // Histogram flat file creation
    // ***************************************

    double zmax;

    if(par.getNum("zmax") != 0) zmax = par.getNum("zmax");
    else zmax = z2;

    int voxelsHi = 0;
    while(z2 <= zmax)
    {
        z2 += ht;
        ++voxelsHi;
    }

    cout << "Voxels Above: " << voxelsHi << '\n';
    cout << "Voxels Below: " << voxelsLo << '\n';
    cout << "Filled Voxels: " << numVox << '\n';



    string hff = par.getString("hff");
    if(hff[0] == 'y')
    {
        cout << "\n\nWriting data to histogram flat file...\n";
        string hffdir = par.getString("hffDir") + "/" + par.getString("hffTitle") + id;



        data.toHistFF(hffdir, voxelsHi, voxelsLo - 1, numVox, z0, z2);
        cout << "Done writing data to histogram flat file.\n";
    }



    // ***************************************
    // Lidar Metrics flat file creation
    // ***************************************


    string lmff = par.getString("lmff");
    if(lmff[0] == 'y')
    {
        cout << "\n\nWriting data to lidar metrics flat file...\n";
        string lmffdir = par.getString("lmffDir") + "/" + par.getString("lmffTitle") + id;


        data.toLidarMetrics(lmffdir);
        cout << "Done writing data to lidar metrics flat file.\n";
    }


    // ***************************************
    // Percentiles flat file creation
    // ***************************************

//    string pff = par.getString("pff");
//    if(pff[0] == 'y')
//    {
//        string pffdir = par.getString("pffDir") + "/" + par.getString("pffTitle") + id;
//        data.toPercMetrics(pffdir.c_str());
//    }



    ifs.close();
    }


//    time_t timerF;
//
//    time(&timerF);
//
//    cout << "\n\nTotal Time Elapsed: " << timerF - timerI;
//    cout << '\n' << '\n';




  if (quiet) std::cout.rdbuf(cout_sbuf); // restore the original stream buffer

  cout << "Buffer restored\n";

  return(0);
};

