#ifndef LOCALSEARCH_H_
#define LOCALSEARCH_H_

#include "point.h"

//Todos usam vizinhança 2-opt

//Steepest Descent
double local_search_SD (point** points,int size);
//Best-First Search
double local_search_BFS (point** points,int size);
//Hill Climbing
double local_search_HC (point** points,int size);

#endif
