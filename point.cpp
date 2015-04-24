#include <stdio.h>
#include "point.h"

Point:: Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};

Point::Point() {};

void Point::print()
  {
    printf("\nx = %f, y = %f, z = %f \n", x, y, z);
  };
