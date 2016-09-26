#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "point.h"

int main (int argc, char** argv){
  
  //Variável contadora
  int i = 0;
  
  //Variáveis que obteremos do arquivo
  char name[1024];
  int size;
  
 /*
  * 
  * COMEÇO DA LEITURA !!!
  * 
  */
 
  //Abridno arquivo tsp
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
  fscanf(tsp,"%s", buffer);
   if(strcmp(buffer,"NAME:") == 0){
     fscanf(tsp,"%s", name);
     printf("NOME: %s\n", name);
   }
   else{
    printf("ERRO: ARQUIVO COM FORMATO INVÁLIDO!\n");
    return 1;
   }
   
  //Pegando o tamanho 
  while(fscanf(tsp,"%s", buffer) != EOF){
  if(strcmp(buffer,"DIMENSION:") == 0){
     fscanf(tsp,"%d", &size);
     printf("Tamanho: %d\n", size);
     break;
   }
  }
  
  //Lendo até chegar nos pontos
  while(fscanf(tsp,"%s", buffer) != EOF){
  if(strcmp(buffer,"NODE_COORD_SECTION") == 0){
     break;
   }
  }
  
  //Double temporário para leitura
  double temp;
  
  //Criando array de pontos
  point** points = (point**)malloc(sizeof(point*)*size);
  
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
  
  /*for(i=0;i<size;i++){
     printf("i: %d\n",i); 
     printf("X: %lf ", points[i]->x);
     printf(" Y: %lf\n", points[i]->y);
  }*/
  
  return 0;
  
}