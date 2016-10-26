#ifndef LOCALSEARCH_H_
#define LOCALSEARCH_H_

#include "point.h"

typedef struct answer_struct {
    int res_changes; 
    int result;
    int swaps;
}answer;

//Todos usam vizinhança 2-opt

//Steepest Descent
answer local_search_SD (point** points,int size,int** distances);
//Best-First Search
answer local_search_BFS (point** points,int size,int** distances);
//Hill Climbing
answer local_search_HC (point** points,int size,int** distances,int n);

#endif
