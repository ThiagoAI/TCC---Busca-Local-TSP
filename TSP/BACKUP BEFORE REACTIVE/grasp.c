#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "point.h"
#include "grasp.h"

//Para fazer a lista de candidatas
int* make_rcl(int* sol,int* aux,int point,int** distances,int size_rcl,int size){
  int* rcl = (int*)malloc(sizeof(int)*size_rcl);
  int* dists = (int*)malloc(sizeof(int)*size_rcl);
  int i = 0;
  int e = 0;
  int temp = 0;
  
  //Inicializamos a rcl 
  for(i=0;i<size_rcl;i++) rcl[i] = -1;

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
    
     //printf("temp - %d - %d\n",temp,sol[i]);
     //Para vermos se vamos incluir na rcl
     for(e=0;e<size_rcl;e++){
       
      // printf("lel: %d",temp);
       if(rcl[e] != -1){
      
         //Saimos do for se for aceito
         if(temp < dists[e]){
           rcl[e] = sol[i];
           dists[e] = temp;
           break; 
         }
         
       //If != -1  
       }
       else{
       
         //Se for -1 colocamos e saimos 
         rcl[e] = sol[i];
         dists[e] = temp;
         break;
       }
     //for e  
     }
     
   //If superior  
   }

  //for i  
  }
  
    free(dists);
  return rcl;
}

//Solução gulosa aleatória
void greedy_random_solution(int* sol,int size,int** distances,int size_rcl){
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
    int* rcl = make_rcl(sol,aux,point,distances,size_rcl,size);
    int random = rand() % size_rcl;
     //for(e = 0;e<size_rcl;e++){
     
     //}

    //TODO algo melhor que isso
    //Se estivermos no final pode ocorrer de um ser -1...
    if(rcl[random] == -1){
      for(e = 0;e<size_rcl;e++){
        if(rcl[e] != -1){
          random = e;
          break;
        }
      }
    }
    
   // for(e=0;e<size_rcl;e++) printf("- %d -",rcl[e]);
    //printf("\n");
    
    //Damos valor
    copy[i+1] = rcl[random];
    aux[rcl[random]] = -1;
    point = rcl[random]; 
    
    //printf("%d|\n",point);
    
    //Damos free na rcl para evitar perder o espaço
    free(rcl);
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
