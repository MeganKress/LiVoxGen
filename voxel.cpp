#include <stdio.h>
#include "voxel.h"

#include <iostream>

using namespace std;

bool Voxel::operator==(Voxel obj)
  {
      return this->cX == obj.cX && this->cY == obj.cY && this->cZ == obj.cZ;
  };

void Voxel::print()
  {
      cout << "\nX: " << cX << ", Y: " << cY << ", Z: " << cZ;
      cout << "\nNumber of Points: " << pointNum << '\n';
      cout << "Height: " << h << ", Base: " << d;
      cout <<"\nDensity: " << D << '\n';
  };


int Voxel::trimVox()
  {
    vector<Point> newPoints(pointNum);

    int countpt = 0;

    for(unsigned int i = 0; i < pointsInVox.size(); ++i)
    {
      if(inVox(pointsInVox.at(i)))
      {
        newPoints.at(countpt++) = pointsInVox.at(i);
      }

    }

    vector<Point> newPoints2(countpt);
    for(int i = 0; i < countpt; ++i)
    {
      newPoints2.at(i) = newPoints.at(i);
    }

    this->pointsInVox = newPoints2;
    this->pointNum = countpt;

    return countpt;
  };

bool Voxel::inVox(Point p)
  {
    return inVox(p.x, p.y, p.z);
  };

bool Voxel::inVox(double x, double y, double z)
  {
    double xMax = cX + d;
    double xMin = cX - d;
    double yMax = cY + d;
    double yMin = cY - d;
    double zMax = cZ + h;
    double zMin = cZ - h;


    if(x < xMax && x >= xMin && y < yMax && y >= yMin && z < zMax && z >= zMin)
    {
        return true;
    } else {
        return false;
    }


    return true;
  };
