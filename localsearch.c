#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "localsearch.h"
#include "point.h"

//Para retornar valor final da local search
double add_distances(int* sol,point** points,int size){
  int i = 0;
  double total = 0;
  for(i=0;i<size-1;i++){
    total += euclidian_distance(points[sol[i]], points[sol[i+1]]);
  }
    total += euclidian_distance(points[sol[size-1]],points[sol[0]]);
    
  return total;
}

//Para fazer a sol inicial aleatória
void random_start(int* sol,int size){
  int i = 0;
  srand(time(NULL));
  
  //Inicializamos trivialmente
  for(i=0;i<size;i++){
    sol[i] = i;  
  }
  
  //Embaralhamos
  for(i=0;i<size;i++){
    int r = rand() % size;
    if(r != i){
    int t = sol[i];
    sol[i] = sol[r];
    sol[r] = t;
    }
  }
  
}

//Cria vizinhança e calcula novo valor
void two_opt_swap(int* sol,int* new_sol,int i,int e,int size){
  int k = 0;
  
  /*for(k=2;k<size-1;k++)
    if(new_sol[k] == new_sol[k+1]){
      printf("EMERGENCY xxx !!!!\n\n\n");
      printf("I: %d | E: %d | K: %d | %d %d\n",i,e,k,new_sol[i],new_sol[e]);
      exit(0);
    }*/
  
  for(k=0;k<i;k++){
   new_sol[k] = sol[k]; 
  }
  
  //Adicionamos em ordem inversa
  int temp = 0;
  for(k=i;k <= e;k++){
    new_sol[k] = sol[e-temp];
    temp++;
  }
  
  for(k=e+1;k<size;k++){
    new_sol[k] = sol[k];
  }
  
  /*for(k=0;k<size-1;k++)
    if(new_sol[k] == new_sol[k+1]){
	printf("EMERGENCY !!!!\n\n\n");
	printf("I: %d | E: %d | K: %d | %d %d\n",i,e,k,sol[i],sol[e]);
	exit(0);
    }*/
  
   
}


//Steepest Descent
double local_search_SD (point** points,int size){
  /*sol = solução
   * "i" e "e" = contadora
   * condition = condição de parada é mudada para zero se não houver mudança ou atingir o número máximo de iterações
   * values = variáveis temporárias para as guardar valores das distâncias 
   * best_difference = variável temporária para guardar a melhor diferença 
   */
  int i = 0;
  int e = 0;
  int k = 0;
  int condition = 1;
  int iter = 0;
  
  int* sol = (int*)malloc((sizeof(int)*size));
  int* new_sol = (int*)malloc((sizeof(int)*size));
  random_start(sol,size);
  
  for(i=0;i<size;i++){
    new_sol[i] = sol[i];
  }
  
  double best_value = add_distances(sol,points,size);
  double new_value = best_value;

  //Loop do local search
  while(condition == 1){
   // printf("condition: %d\n\n", condition);
    iter++;
    condition = 0;
    
    //Se chega no limite de iterações entra no if
    if(iter > 1000) return best_value;
    
    best_value = add_distances(sol,points,size);
    
   // printf("O as ideai q loko kkkkk\n\n\n");
    for(i=0;i<size-1;i++){
      for(e=i+1;e<size;e++){

	  //Fazemos a troca
	  two_opt_swap(sol,new_sol,i,e,size);
	  
	  //Calculamos o valor novo
	  new_value = add_distances(new_sol,points,size);

	  //Se for melhor o valor novo entra aqui
	  if(new_value < best_value){
	  //printf("TOP KEKE %d %d\n\n",i,e);
	    best_value = new_value;
	    for(k=0;k<size;k++)sol[k] = new_sol[k];
	    condition = 1;
	  }

	
    //2 fechamentos dos for	
      }
    }
   //Fim do while
  }
  
  /*for(k=0;k<20;k++){
    printf("Sol %d: %d\n",k,sol[k]);
  }*/
  
  /*for(k=0;k<size;k++){
    printf("new_sol %d: %d\n",k,sol[k]);
  }*/

  for(k=0;k<size-1;k++)
    if(sol[k] == sol[k+1]){
      printf("EMERGENCY !!!!\n\n\n");
      printf("I: %d | E: %d | K: %d | %d %d\n",i,e,k,sol[i],sol[e]);
      exit(0);
    }
  
  printf("Numero de iteracoes : %d\n", iter);
  
  //Retornamos o valor total do percurso
  return best_value;
}
//Best-First Search
double local_search_BFS (point** points,int size){
  return -1;
}
//Hill Climbing
double local_search_HC (point** points,int size){
  return -1;
}
