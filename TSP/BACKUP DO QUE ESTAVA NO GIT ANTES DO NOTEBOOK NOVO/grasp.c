#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "point.h"
#include "grasp.h"
#include "list.h"

//Para fazer a lista de candidatas
int_list* make_rcl(int* sol,int* aux,int point,int** distances,double alfa,int size){

  //TODO por lista aqui depois

  int_list* rcl = create_list();
  int i = 0;
  int e = 0;
  int temp = 0;
  
  //Inicializando best
  int best = -1;
  int best_point = 0;
  int worst = -1;
  int worst_point = 0;
  
  if(point != 0){
    if(sol[i] > point){
      best = distances[sol[0]][point];
      best_point = 0;
      worst = distances[sol[0]][point];
      worst_point = 0;
    }
    else{
      best = distances[point][sol[0]];
      best_point = 0;
      worst = distances[point][sol[0]];
      worst_point = 0;
    }
   }
   else{
     if(sol[i] > point){
       best = distances[sol[1]][point];
       best_point = 1;
       worst = distances[sol[1]][point];
       worst_point = 0;
     }
     else{
       best = distances[point][sol[1]];
       best_point = 1;
       worst = distances[point][sol[1]];
       worst_point = 0;
     }
   }
  
  //Pegamos o menor e o maior
  for(i=0;i<size;i++){
    if(i != point){
    
      if(sol[i] > best_point){
         temp = distances[sol[i]][point];
      }
      else{
         temp = distances[point][sol[i]];
      }
 
      if(temp < best){
        best = temp;
        best_point = i;
      }
      if(temp > worst){
        worst = temp;
        worst_point = i;
      }
    }
  }

  //For principal parar achar os membros da rcl
  for(i=0;i<size;i++){
  
   //Verifica
    if(sol[i] != point && aux[sol[i]] != -1){

      //Calcular distancia
      if(sol[i] > point){
        temp = distances[sol[i]][point];
        }
      else{
        temp = distances[point][sol[i]];
        }
 
        //Critério para entrar na RCL
 	if(temp <= best + alfa*(worst - best)){
 	  add_list(rcl,i);
 	}   
 	
   //printf("temp - %d - %d\n",temp,sol[i]); 
   //If superior  
   }

  //for i  
  }
 
  return rcl;
}

//Solução gulosa aleatória
void greedy_random_solution(int* sol,int size,int** distances,double alfa){
  int i = 0;
  int e = 0;

  //ISSO RESOLVE, TEM QUE DESCOBRIR O PORQUÊ
  //CONSERTADO MAS DEIXEI ESSES COMENTÁRIOS AQUI
  //for(i=0;i<size;i++) sol[i] = i;

  //Onde copiaremos os resultados
  int* copy = (int*)malloc(sizeof(int)*size);
  for(i=0;i<size;i++){
    copy[i] = sol[i];
  }
  
  //Auxiliar para marcar o que já foi usadoo
  int* aux = (int*)malloc(sizeof(int)*size);
  for(i=0;i<size;i++){
    aux[i] = 1;
  }	
  
  //Ponto inicial 100% aleatório
  int point = rand() % size;
  copy[0] = point; 
  aux[point] = -1;
  
  //printf("ponto inicial e %d\n",point);
  
  //For principal 
  for(i=0;i<size-1;i++){
    //Criamosa rcl e escolhemos um para ser o próximo
    int_list* rcl = make_rcl(sol,aux,point,distances,alfa,size);
    int random = rand() % rcl->amount;
    
    //Damos valor
    copy[i+1] = get_point(rcl,random);;
    aux[copy[i+1]] = -1;
    point = copy[i+1]; 
    
    //printf("%d|\n",point);
    
    //Livramos espaço da rcl
    destroy_list(rcl);
  }
  
  for(i=0;i<size;i++) sol[i] = copy[i];
  
  //TODO remover aqui
  int z = 0;
  for(z=0;z<size;z++){
    for(i=0;i<size;i++){
      if(sol[i] == sol[z] && i != z){
       printf("\nERROR %d %d\n", i, sol[i]);
       exit(1);
       }
    }
  }
  //frees dos arrays auxiliares
  free(aux);
  free(copy);
}
