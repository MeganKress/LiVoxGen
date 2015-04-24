#ifndef VOXDATA_H
#define VOXDATA_H

#include <vector>
#include "voxcol.h"
#include <string>

struct VoxData
{
    std::vector<VoxCol> voxcols;
    int numPts, numCols, numHi;
    std::string acqid, segid, splitid;

    VoxData();

    void toFile(std::string);

    void fromFile(std::string);

    void toHistFF(std::string,int,int,int,double,double);

    void toLidarMetrics(std::string);

    void toPercMetrics(std::string);

};
#endif // VOXDATA_H

