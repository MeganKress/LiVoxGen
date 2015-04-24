#ifndef VOXEL_H
#define VOXEL_H

#include <vector>
#include "point.h"

struct Voxel
{
public:

  double cX, cY, cZ, h, d;
  double D;
  int pointNum;

  std::vector<Point> pointsInVox;


  Voxel() {};

  bool operator==(Voxel obj);

  void print();


  int trimVox();

  bool inVox(Point p);

  bool inVox(double x, double y, double z);

};

#endif // VOXEL_H
