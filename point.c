#include <math.h>
#include "point.h"

double euclidian_distance(point* a, point* b){
  double dx = a->x - b->x;
  double dy = a->y - b->y;
  
  return sqrt((dx*dx) + (dy*dy));
}