#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "point.h"
#include "localsearch.h"

//Argumento 1 = Arquivo
//Argumento 2 = Escolha (1 para vinte vezes, qualquer outro valor para uma vez)
int main (int argc, char** argv){
  
  //Verifica se o número de argumentos está correto
  if(argc != 3){
   printf("ERRO: NÚMERO DE ARGUMENTOS ESTÁ ERRADO!\n");
   return 1;
  }
  
  //Variável contadora
  int i = 0;
  
  //Variáveis que obteremos do arquivo
  char name[1024];
  int size;
  int capacity;
  int choice = atoi(argv[2]);
 /*
  * 
  * COMEÇO DA LEITURA !!!
  * 
  */
 
  //Abrindo arquivo cvrp
  FILE* cvrp;
  cvrp = fopen(argv[1],"r");
  
  //Se não achar o arquivo ou falhar entra aqui
  if(cvrp == NULL){
   printf("\nERRO: ARQUIVO NULO, VERIFIQUE SE O CAMINHO ESTÁ CORRETO!\n");
   return 1;
  }
  char buffer[1024];
  
  /*
  int read;
  while(fgets(buffer, 1024, cvrp)){
    
    printf("BUFFER: %s\n",buffer);
  }*/

  //printf("Lendo... %s %d\n",argv[1],fscanf(cvrp,"%s", buffer));
  //printf("aii %s\n",buffer);
  
  //Pegamos o nome
  while( fscanf(cvrp,"%s", buffer)){
   if(strcmp(buffer,"NAME") == 0){
     fscanf(cvrp,"%s", buffer);
     fscanf(cvrp,"%s", name);
     //printf("NOME: %s\n", name);
   }
   if(strcmp(buffer,"DIMENSION") == 0){
     fscanf(cvrp,"%s", buffer);
     fscanf(cvrp,"%d", &size);
     //printf("TAMANHO: %d\n", size);
   }
   if(strcmp(buffer,"CAPACITY") == 0){
     fscanf(cvrp,"%s", buffer);
     fscanf(cvrp,"%d", &capacity);
     //printf("CAPACIDADE: %d\n", capacity);
   }
   if(strcmp(buffer,"NODE_COORD_SECTION") == 0){
     break;
   }
  } 
  
  //Double temporário para leitura
  double temp;
  
  //Criando array de pontos
  point** points = (point**)malloc(sizeof(point*)*(size));
  
  //Lendo os pontos
  for(i=0;i<size;i++){
   point* new_point = (point*)malloc(sizeof(point));
   fscanf(cvrp,"%lf", &temp);
   fscanf(cvrp,"%lf", &temp);
   // printf("X: %lf\n", temp);
   new_point->x = temp;
   fscanf(cvrp,"%lf", &temp);
   // printf("Y: %lf\n", temp);
   new_point->y = temp;
   points[i] = new_point;
  }
  
  //Para ler DEMAND_SECTION
  fscanf(cvrp,"%s", buffer);
  
  //Lendo demanda
  for(i=0;i<size;i++){
    fscanf(cvrp,"%lf", &temp);
    fscanf(cvrp,"%lf", &temp);
    points[i]->demand = temp;
  }
  
  fclose(cvrp);
  
  //for(i=0;i<size;i++)
  //	printf("Oi %d: %lf %lf %lf\n",i,points[i]->x,points[i]->y,points[i]->demand);
  
 // printf("Nome, capacidade e tamanho: %s | %d | %d\n\n",name,capacity,size);
 /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */
  
 //Para número mínimo de veículos
 int total_demand = 0;
 int min_v = 0;
 for(i=0;i<size;i++)
 	total_demand += points[i]->demand;
 if(total_demand%capacity == 0) min_v = total_demand/capacity;
 else min_v = (int)(total_demand/capacity) + 1; 
 
 //printf("Min_v: %d\n",min_v);
  
 clock_t start, end;
 int result = -1;
 
 //Para termos uma ordem aleatória
 srand(time(NULL));
 
 double average_time = 0;
 double average_result = 0;
 int best_result = -1;
 int worst_result = -1;
 
 //Dependendo da escolha do usuário executa com um dos métodos
  if(choice == 1){
    int count = 0;
    for(count = 0; count < 100; count++){
      start = clock();
      result = local_search(points,size,capacity);
      end = clock();
      double time = (double)(end - start)/CLOCKS_PER_SEC;
      average_time += time;
      average_result  += result;
      
      if(result < best_result || best_result == -1) best_result = result;
      if(result > worst_result || worst_result == -1) worst_result = result;
    }
    
    average_time = average_time/100;
    average_result = average_result/100;
    
    printf("\nEm media obtivemos %lf demorando em media %lf.\nO melhor resultado foi %d e o pior resultado foi %d.\n",average_result,average_time,best_result,worst_result);   
  }
  else{
    start = clock();
    result = local_search(points,size,capacity);
    end = clock();
    
    double time = (double)(end - start)/CLOCKS_PER_SEC;
    
    printf("\nMelhor caminho totalizou %d e levou %lf segundos.\n",result,time);
  }
  
  return 0;
  
}
