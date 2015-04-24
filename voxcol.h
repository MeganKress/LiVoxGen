#ifndef VOXCOL_H
#define VOXCOL_H

#include <vector>
#include "point.h"
#include "voxel.h"
#include <string>



struct VoxCol
{

public:

  int pic;
  double xC, yC, d0, d1 , d2 ,
    d3 , d4 , d5 , d6 , d7 ,
    d8 , d9 ,
    kurt , mean , p10 , p20 ,
    p30 , p40 , p50 , p60 , p70 ,
    p80 ,
    p90 , qmean , refl_kurt , refl_mean ,
    refl_skew , refl_stdev ,
    skew , stdev , ground_aspect ,
    ground_elev_mean ,
    ground_refl_kurt , ground_refl_mean ,
    ground_refl_skew ,
    ground_refl_stdev , ground_slope ,
    pulse_density ,
    pulse_scan_angle , returns_per_pulse ,
    shrub_mean ,
    shrub_refl_mean , shrub_refl_stdev ,
    shrub_stdev , tree_aad ,
    tree_crr , tree_d0 , tree_d1 ,
    tree_d2 , tree_d3 , tree_d4 ,
    tree_d5 , tree_d6 , tree_d7 ,
    tree_d8 , tree_d9 , tree_fcover ,
    tree_fract_all , tree_iqr , tree_kurt ,
    tree_mad , tree_mean ,
    tree_p10 , tree_p20 , tree_p30 ,
    tree_p40 , tree_p50 , tree_p60 ,
    tree_p70 , tree_p80 , tree_p90 ,
    tree_qmean , tree_refl_kurt,
    tree_reflt_mean , tree_refl_skew ,
    tree_refl_stdev ,
    tree_rugosity , tree_skew , tree_stdev ,
    tree_vdr ,
    refl_max , ground_refl_max , tree_refl_max ,
    p100 ,
    shrub_refl_max , tree_p100 ;


  bool inRaster;


  std::vector<Voxel> voxels;

  void init(double);
  void setDensities();

};

#endif // VOXCOL_H
