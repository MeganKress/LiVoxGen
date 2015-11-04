#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "voxcol.h"
#include "voxel.h"
#include "point.h"
#include "voxdata.h"

VoxData::VoxData() {};



using namespace std;

void VoxData::toFile(std::string outFile)
{
   ofstream myfile;
   myfile.open (outFile.c_str());

   string sig = "0V 1O 2X 2G 7E 3N 9E";
   myfile <<  sig << '\n';

   myfile << numPts << ',' << numCols << ',' << numHi << '\n';

   for(unsigned int i = 0; i < voxcols.size(); ++i)
   {
       //myfile << "VoxCol\n";
       //if(voxcols.at(i).inRaster)
       //{
       vector<Voxel> vc = voxcols.at(i).voxels;

       while(vc.size() > 0)
       {
           Voxel v = vc.back();

           myfile << setprecision(12) << v.cX << ',' <<  v.cY << ',' << v.cZ << ',';
           myfile << setprecision(12) << v.D << ','  << v.d << ','  << v.h << ','  << v.pointNum << '\n';

           vc.pop_back();
       }
       //}
   }

   myfile.close();
};

void VoxData::fromFile(string inFile)
{

    VoxData data;

    ifstream file (inFile.c_str());
    string value;

    if (file)
    {
        //Signature Line
        getline( file, value , '\n');

        //Basic Info Line
        getline(file,value,',');
        char val[value.length()];
        strncpy(val, value.c_str(), value.length());
        data.numPts = strtod(val, NULL);

        getline(file,value,',');
        char val1[value.length()];
        strncpy(val1, value.c_str(), value.length());
        data.numCols = strtod(val1, NULL);

        getline(file,value,'\n');
        char val2[value.length()];
        strncpy(val2, value.c_str(), value.length());
        data.numHi = strtod(val2, NULL);


        vector<VoxCol> vcv;


        for(int j = 0; j < data.numCols; ++j)
        {

            VoxCol vc;

            for(int i = 0; i < data.numHi; ++i)
            {
                Voxel v;

                getline(file,value,',');
                char val[value.length()];
                strncpy(val, value.c_str(), value.length());
                v.cX = strtod(val, NULL);

                vc.xC = strtod(val, NULL);

                getline(file,value,',');
                char val1[value.length()];
                strncpy(val1, value.c_str(), value.length());
                v.cY = strtod(val1, NULL);

                vc.yC = strtod(val1, NULL);

                getline(file,value,',');
                char val2[value.length()];
                strncpy(val2, value.c_str(), value.length());
                v.cZ = strtod(val2, NULL);

                getline(file,value,',');
                char val3[value.length()];
                strncpy(val3, value.c_str(), value.length());
                v.D = strtod(val3, NULL);

                getline(file,value,',');
                char val4[value.length()];
                strncpy(val4, value.c_str(), value.length());
                v.d = strtod(val4, NULL);

                getline(file,value,',');
                char val5[value.length()];
                strncpy(val5, value.c_str(), value.length());
                v.h = strtod(val5, NULL);


                getline(file,value,'\n');
                char val7[value.length()];
                strncpy(val7, value.c_str(), value.length());
                v.pointNum = strtod(val7, NULL);

                vc.voxels.push_back(v);
            }

            vcv.push_back(vc);

        }


        data.voxcols = vcv;
    }

    voxcols = data.voxcols;
    numPts = data.numPts;
    numCols = data.numCols;
    numHi = data.numHi;

    file.close();
};

void VoxData::toHistFF(string outFile, int vh, int vl,int nv,double z1, double z2)
{
    ofstream myfile;
    myfile.open (outFile.c_str());


        myfile << "x,y,ac,seg";

        for (unsigned int j = 1; j <= nv + vh + vl; ++j)
        {
            myfile << ",bin" << j;
        }

        myfile << '\n';

        myfile << "x,y,ac,seg";

         for (double k = z1; k < z2; k+=0.3)
        {
            myfile << setprecision(12) << ',' << k;
        }


        myfile << '\n';


    for(unsigned int i = 0; i < voxcols.size(); ++i)
    {
        VoxCol vc = voxcols.at(i);

        if(vc.inRaster){

            myfile << setprecision(12) <<  vc.xC <<  ',' << vc.yC << ',' << acqid << ',' << segid;
            if(vl > 0){
            for(unsigned int l = 0; l < vl; ++l)
            {
                myfile << ",0";
            }
            }

            if(nv > 0){
            for(unsigned int j = 0; j < nv; ++j)
            {
                Voxel v = vc.voxels.at(j);
                myfile << ',' << v.pointNum;
            }
            }

            if(vh > 0){
            for (unsigned int k = 0; k <  vh; ++k)
            {
                myfile << ",0";
            }
            }
            myfile << '\n';
        }
    }

    myfile.close();
};

void VoxData::toLidarMetrics(string outFile)
{
    ofstream myfile;
    myfile.open (outFile.c_str());

    myfile << "x,y,acq,seg,pulse_density,pulse_scan_angle,returns_per_pulse,mean,qmean,stdev,skew,kurt,p10,p20,p30,p40,p50,p60,p70,p80,p90,p100,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,refl_max,shrub_mean,shrub_stdev,shrub_refl_max,tree_mean,tree_qmean,tree_stdev,tree_rugosity,tree_skew,tree_kurt,tree_fcover,tree_fract_al,tree_p10,tree_p20,tree_p30,tree_p40,tree_p50,tree_p60,tree_p70,tree_p80,tree_p90,tree_p100,tree_d0,tree_d1,tree_d2,tree_d3,tree_d4,tree_d5,tree_d6,tree_d7,tree_d8,tree_d9,tree_iqr,tree_vdr,tree_mad,tree_aad,tree_crr,tree_refl_max,ground_elev_mean,ground_slope,ground_aspect,ground_refl_max" << '\n';


    for(unsigned int i = 0; i < voxcols.size(); ++i)
    {
        VoxCol vc = voxcols.at(i);

        if(vc.inRaster){

        myfile << setprecision(12) <<  vc.xC <<  ',' << vc.yC
            << ',' << acqid << ',' << segid << ','
            << vc.pulse_density << ','
            << vc.pulse_scan_angle << ','
            << vc.returns_per_pulse << ','
            << vc.mean << ','
            << vc.qmean << ','
            << vc.stdev << ','
            << vc.skew << ','
            << vc.kurt << ','
            << vc.p10 << ','
            << vc.p20 << ','
            << vc.p30 << ','
            << vc.p40 << ','
            << vc.p50 << ','
            << vc.p60 << ','
            << vc.p70 << ','
            << vc.p80 << ','
            << vc.p90 << ','
            << vc.p100 << ','
            << vc.d0 << ','
            << vc.d1 << ','
            << vc.d2 << ','
            << vc.d3 << ','
            << vc.d4 << ','
            << vc.d5 << ','
            << vc.d6 << ','
            << vc.d7 << ','
            << vc.d8 << ','
            << vc.d9 << ','
            << vc.refl_max << ','
            << vc.shrub_mean << ','
            << vc.shrub_stdev << ','
            << vc.shrub_refl_max << ','
            << vc.tree_mean<< ','
            << vc.tree_qmean << ','
            << vc.tree_stdev << ','
            << vc.tree_rugosity << ','
            << vc.tree_skew << ','
            << vc.tree_kurt << ','
            << vc.tree_fcover << ','
            << vc.tree_fract_all << ','
            << vc.tree_p10 << ','
            << vc.tree_p20 << ','
            << vc.tree_p30 << ','
            << vc.tree_p40 << ','
            << vc.tree_p50 << ','
            << vc.tree_p60 << ','
             << vc.tree_p70 << ','
            << vc.tree_p80 << ','
            << vc.tree_p90 << ','
            << vc.tree_p100 << ','
            << vc.tree_d0 << ','
            << vc.tree_d1 << ','
            << vc.tree_d2 << ','
            << vc.tree_d3 << ','
            << vc.tree_d4 << ','
            << vc.tree_d5 << ','
            << vc.tree_d6 << ','
            << vc.tree_d7 << ','
            << vc.tree_d8 << ','
            << vc.tree_d9<< ','
            << vc.tree_iqr << ','
            << vc.tree_vdr << ','
            << vc.tree_mad << ','
            << vc.tree_aad << ','
            << vc.tree_crr << ','
            << vc.tree_refl_max << ','
            << vc.ground_elev_mean << ','
            << vc.ground_slope << ','
            << vc.ground_aspect << ','
            << vc.ground_refl_max
            << '\n';
            }
    }

    myfile.close();
}

void VoxData::toPercMetrics(string outFile)
{
    ofstream myfile;
    myfile.open (outFile.c_str());

    myfile << "x,y,acq,seg,p10,p20,p30,p40,p50,p60,p70,p80,p90,p100" << '\n';


    for(unsigned int i = 0; i < voxcols.size(); ++i)
    {
        VoxCol vc = voxcols.at(i);

        if(vc.inRaster){

        myfile << setprecision(12) <<  vc.xC <<  ',' << vc.yC
            << ',' << acqid << ',' << segid << ','
            << vc.p10 << ','
            << vc.p20 << ','
            << vc.p30 << ','
            << vc.p40 << ','
            << vc.p50 << ','
            << vc.p60 << ','
            << vc.p70 << ','
            << vc.p80 << ','
            << vc.p90 << ','
            << vc.p100
            << '\n';
            }
    }

    myfile.close();
}

