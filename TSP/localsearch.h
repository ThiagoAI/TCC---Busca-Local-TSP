#ifndef LOCALSEARCH_H_
#define LOCALSEARCH_H_

#include "point.h"

typedef struct answer_struct {
    int res_changes; 
    int result;
    int original_result;
    int swaps;
}answer;

//Todos usam vizinhança 2-opt

//Para inicializar
void random_start(int* sol,int size);
void greedy_start(int* sol,int size,int** distances);

//Steepest Descent
answer local_search_SD (int* sol,point** points,int size,int** distances);
//Best-First Search
answer local_search_BFS (int* sol,point** points,int size,int** distances);
//Hill Climbing
answer local_search_HC (int* sol,point** points,int size,int** distances,int n);

#endif
