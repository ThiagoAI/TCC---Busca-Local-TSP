#ifndef POINT_H_
#define POINT_H_

typedef struct point_struct { 
    double x; 
    double y;
}point;

double euclidian_distance(point* a, point* b);

#endif