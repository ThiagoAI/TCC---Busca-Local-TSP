#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "localsearch.h"
#include "point.h"



//Retorna se x é maior que y
int bigger(int x,int y){
  if(x > y) return 1;
  else return 0; 
}

//Para retornar valor final da local search
int add_distances(int* sol,point** points,int size,int** distances){
  int i = 0;
  int total = 0;
  
  //printf("%d l0l!!",distances[19][2]);
  for(i=0;i<size-1;i++){
    //printf("%d %d!\n\n\n",sol[i],sol[i+1]);
    if(sol[i] > sol[i+1])
      total += distances[sol[i]][sol[i+1]];
    else 
      total += distances[sol[i+1]][sol[i]];
  }

  if(sol[0] > sol[size-1])
   total += distances[sol[0]][sol[size-1]];
  else
   total += distances[sol[size-1]][sol[0]];
   
  return total;
}

//Para fazer a sol inicial aleatória
void random_start(int* sol,int size){
  int i = 0;
  
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

//Para fazer a sol inicial gulosa
/*void greedy_start(int* sol,int size,int** distances){
  int i = 0;
  int e = 0;
  int best = -1;
  int best_e = -1;
  int temp = 0;
  
  int* aux = (int*)malloc(sizeof(int)*size);
  for(i=0;i<size;i++){
    aux[i] = i;
  }
  
  //Prendemos o inicial em 0
  sol[0] = 0;
  aux[0] = -1;
  
  for(i=1;i<size;i++){

    for(e=0;e<size;e++){
      
      //Se não forem iguais verificamos qual o melhor
      if(sol[i-1] != aux[e] && aux[e] != -1){

        if(sol[i-1] > aux[e])
          temp = distances[sol[i-1]][aux[e]];
        else 
          temp = distances[aux[e]][sol[i-1]];
       
        if(temp < best || best == -1){
          best_e = e;
          best = temp;
        }
      }
      
    //for e   
    }
    sol[i] = best_e;
    aux[best_e] = -1;
    best = -1;
    
  //for i   
  }
  
  free(aux); 
}*/

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
answer local_search_SD (int* sol,point** points,int size,int** distances){
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
  
  answer ans;
  ans.swaps = 0;
  ans.res_changes = 0;
  
  int* new_sol = (int*)malloc((sizeof(int)*size));
  
  //Para guardarmos qual a melhor que encontramos
  int* best_sol = (int*)malloc((sizeof(int)*size));
  
  for(i=0;i<size;i++){
    new_sol[i] = sol[i];
    best_sol[i] = sol[i];
  }
  
  double best_value = add_distances(sol,points,size,distances);
  double new_value = best_value;

  //Loop do local search
  while(condition == 1){
   // printf("condition: %d\n\n", condition);
    iter++;
    condition = 0;
    
    //Se chega no limite de iterações entra no if
    if(iter > 10000) return ans;
    
    best_value = add_distances(sol,points,size,distances);
    
   // printf("O as ideai q loko kkkkk\n\n\n");
    for(i=0;i<size-1;i++){
      for(e=i+1;e<size;e++){

	  //Fazemos a troca
	  two_opt_swap(sol,new_sol,i,e,size);
	  ans.swaps++;
	  
	  //Calculamos o valor novo
	  new_value = add_distances(new_sol,points,size,distances);

	  //Se for melhor o valor novo entra aqui
	  if(new_value < best_value){
	    best_value = new_value;
	    ans.result = best_value;
	    for(k=0;k<size;k++)best_sol[k] = new_sol[k];
	    condition = 1;
	  }

	
    //2 fechamentos dos for	
      }
    }
    
    //Mudamos de fato aqui
    if(condition == 1){
      for(k=0;k<size;k++)sol[k] = best_sol[k];
      ans.res_changes++;
    }
    
   //Fim do while
  }
  
  //Retornamos o valor total do percurso
  return ans;
}
//Best-First Search
answer local_search_BFS (int* sol,point** points,int size,int** distances){
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
  
  answer ans;
  ans.swaps = 0;
  ans.res_changes = 0;
  ans.original_result = add_distances(sol,points,size,distances);
  
  int* new_sol = (int*)malloc((sizeof(int)*size));
  
  for(i=0;i<size;i++){
    new_sol[i] = sol[i];
  }
  
  double best_value = ans.original_result;
  double new_value = best_value;

  //Loop do local search
  while(condition == 1){
   // printf("condition: %d\n\n", condition);
    iter++;
    condition = 0;
    
    //Se chega no limite de iterações entra no if
    if(iter > 10000) return ans;
    
    best_value = add_distances(sol,points,size,distances);
    
   // printf("O as ideai q loko kkkkk\n\n\n");
    for(i=0;i<size-1;i++){
      for(e=i+1;e<size;e++){

	  //Fazemos a troca
	  two_opt_swap(sol,new_sol,i,e,size);
	  ans.swaps++;
	  
	  //Calculamos o valor novo
	  new_value = add_distances(new_sol,points,size,distances);

	  //Se for melhor o valor novo entra aqui
	  if(new_value < best_value){
	    best_value = new_value;
	    ans.result = best_value;
	    ans.res_changes++;
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
  
  //Retornamos o valor total do percurso
  return ans;
}
//Hill Climbing
answer local_search_HC (int* sol,point** points,int size,int** distances,int n){
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
  
  answer ans;
  ans.swaps = 0;
  ans.res_changes = 0;
  
  int* new_sol = (int*)malloc((sizeof(int)*size));
  
  //Para guardarmos qual a melhor que encontramos
  int* best_sol = (int*)malloc((sizeof(int)*size));
  
  for(i=0;i<size;i++){
    new_sol[i] = sol[i];
    best_sol[i] = sol[i];
  }
  
  double best_value = add_distances(sol,points,size,distances);
  double new_value = best_value;

  //Loop do local search
  while(condition == 1){
   // printf("condition: %d\n\n", condition);
    iter++;
    condition = 0;
    
    //Se chega no limite de iterações entra no if
    if(iter > 10000) return ans;
    
    best_value = add_distances(sol,points,size,distances);
    
   // printf("O as ideai q loko kkkkk\n\n\n");
    for(i=0;i<size-1;i++){
      for(e=i+1;e<size;e++){

	  //Fazemos a troca
	  two_opt_swap(sol,new_sol,i,e,size);
	  ans.swaps++;
	  
	  //Calculamos o valor novo
	  new_value = add_distances(new_sol,points,size,distances);

	  //Se for melhor o valor novo entra aqui
	  if(new_value < best_value){
	    best_value = new_value;
	    ans.result = best_value;
	    for(k=0;k<size;k++)best_sol[k] = new_sol[k];
	    condition = 1;
	  }


      //2 fechamentos dos for	
      }
      if(i+1 >= n){
       //printf("l0l\n");
       i = size;
       }
    }
    
    //Mudamos de fato aqui
    if(condition == 1){
      for(k=0;k<size;k++)sol[k] = best_sol[k];
      ans.res_changes++;
    }
    
   //Fim do while
  }
  
  //Retornamos o valor total do percurso
  return ans;
}
