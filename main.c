#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "point.h"
#include "localsearch.h"

//Argumento 1 = Arquivo
//Argumento 2 = Escolha (1 é SD, 2 é BFS , 3 é HC)
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
  int choice = atoi(argv[2]);
  
 /*
  * 
  * COMEÇO DA LEITURA !!!
  * 
  */
 
  //Abrindo arquivo tsp
  FILE* tsp;
  tsp = fopen(argv[1],"r");
  
  //Se não achar o arquivo ou falhar entra aqui
  if(tsp == NULL){
   printf("\nERRO: ARQUIVO NULO, VERIFIQUE SE O CAMINHO ESTÁ CORRETO!\n");
   return 1;
  }
  char buffer[1024];
  
  /*
  int read;
  while(fgets(buffer, 1024, tsp)){
    
    printf("BUFFER: %s\n",buffer);
  }*/

  //printf("Lendo... %s %d\n",argv[1],fscanf(tsp,"%s", buffer));
  //printf("aii %s\n",buffer);
  
  //Pegamos o nome
  while( fscanf(tsp,"%s", buffer)){
   if(strcmp(buffer,"NAME:") == 0){
     fscanf(tsp,"%s", name);
     printf("NOME: %s\n", name);
   }
   if(strcmp(buffer,"DIMENSION:") == 0){
     fscanf(tsp,"%d", &size);
     printf("TAMANHO: %d\n", size);
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
   fscanf(tsp,"%lf", &temp);
   fscanf(tsp,"%lf", &temp);
   // printf("X: %lf\n", temp);
   new_point->x = temp;
   fscanf(tsp,"%lf", &temp);
   // printf("Y: %lf\n", temp);
   new_point->y = temp;
   points[i] = new_point;
  }
  
  fclose(tsp);
 /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */
  
 clock_t start, end;
 double result = -1;
 
 //Dependendo da escolha do usuário executa com um dos métodos
  if(choice == 1){
    start = clock();
    result = local_search_SD(points,size);
  }
  if(choice == 2){
    start = clock();
    result = local_search_BFS(points,size);
  }
  if(choice == 3){
    start = clock();
    result = local_search_HC(points,size);
  }
  
  //Calculando tempo levado
  end = clock();
  double time = (double)(end - start)/CLOCKS_PER_SEC;
  
  printf("\nMelhor caminho totalizou %lf e levou %lf segundos.\n",result,time);
 
  
  
  /*for(i=0;i<size;i++){
     printf("i: %d\n",i); 
     printf("X: %lf ", points[i]->x);
     printf(" Y: %lf\n", points[i]->y);
  }*/
  
  return 0;
  
}
