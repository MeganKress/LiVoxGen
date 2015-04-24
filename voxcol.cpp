#include "voxcol.h"
#include <iostream>
#include <stdio.h>
#include <cmath>

void VoxCol::init(double defaultValue)
{
    d0 = defaultValue;
    d1 = defaultValue;
    d2 = defaultValue;
    d3 = defaultValue;
    d4 = defaultValue;
    d5 = defaultValue;
    d6 = defaultValue;
    d7 = defaultValue;
    d8 = defaultValue;
    d9 = defaultValue;
    kurt = defaultValue;
    mean = defaultValue;
    p10 = defaultValue;
    p20 = defaultValue;
    p30 = defaultValue;
    p40 = defaultValue;
    p50 = defaultValue;
    p60 = defaultValue;
    p70 = defaultValue;
    p80 = defaultValue;
    p90  = defaultValue;
    qmean = defaultValue;
    refl_kurt = defaultValue;
    refl_mean = defaultValue;
    refl_skew  = defaultValue;
    refl_stdev = defaultValue;
    skew  = defaultValue;
    stdev  = defaultValue;
    ground_aspect = defaultValue;
    ground_elev_mean  = defaultValue;
    ground_refl_kurt = defaultValue;
    ground_refl_mean  = defaultValue;
    ground_refl_skew  = defaultValue;
    ground_refl_stdev = defaultValue;
    ground_slope = defaultValue;
    pulse_density  = defaultValue;
    pulse_scan_angle  = defaultValue;
    returns_per_pulse  = defaultValue;
    shrub_mean  = defaultValue;
    shrub_refl_mean  = defaultValue;
    shrub_refl_stdev  = defaultValue;
    shrub_stdev = defaultValue;
    tree_aad  = defaultValue;
    tree_crr = defaultValue;
    tree_d0 = defaultValue;
    tree_d1  = defaultValue;
    tree_d2  = defaultValue;
    tree_d3  = defaultValue;
    tree_d4  = defaultValue;
    tree_d5  = defaultValue;
    tree_d6  = defaultValue;
    tree_d7  = defaultValue;
    tree_d8  = defaultValue;
    tree_d9  = defaultValue;
    tree_fcover  = defaultValue;
    tree_fract_all  = defaultValue;
    tree_iqr = defaultValue;
    tree_kurt  = defaultValue;
    tree_mad  = defaultValue;
    tree_mean  = defaultValue;
    tree_p10 = defaultValue;
    tree_p20  = defaultValue;
    tree_p30 = defaultValue;
    tree_p40  = defaultValue;
    tree_p50  = defaultValue;
    tree_p60 = defaultValue;
    tree_p70 = defaultValue;
    tree_p80  = defaultValue;
    tree_p90  = defaultValue;
    tree_qmean = defaultValue;
    tree_refl_kurt = defaultValue;
    tree_reflt_mean  = defaultValue;
    tree_refl_skew  = defaultValue;
    tree_refl_stdev  = defaultValue;
    tree_rugosity = defaultValue;
    tree_skew  = defaultValue;
    tree_stdev = defaultValue;
    tree_vdr = defaultValue;
    refl_max = defaultValue;
    ground_refl_max = defaultValue;
    tree_refl_max = defaultValue;
    p100 = defaultValue;
    shrub_refl_max = defaultValue;
    tree_p100 = defaultValue;
    inRaster = false;
}

void VoxCol::setDensities()
  {
      double totalPts = 0;

      for(unsigned int i = 0; i < voxels.size(); ++i)
      {
          totalPts += voxels.at(i).pointNum;
      }

      if(totalPts > 0){
      for(unsigned int i = 0; i < voxels.size(); ++i)
      {

          double abovePts1 = 0;
          double abovePts2 = 0;

          for(unsigned int j = i; j < voxels.size(); ++j)
          {
              abovePts1 += voxels.at(j).pointNum;
          }

          for(unsigned int k = i + 1; k < voxels.size(); ++k)
          {
              abovePts2 += voxels.at(k).pointNum;
          }

          if(abovePts2 > 0)
          {
              double x = (abovePts1 / abovePts2);
              voxels.at(i).D = log(x);
              //std::cout << "\nLog Taken.";
          } else
            {
                voxels.at(i).D = 0;
            }

      }

      }
      else
      {
         for(unsigned int i = 0; i < voxels.size(); ++i)
         {
             voxels.at(i).D = 0;
         }
      }
  };

