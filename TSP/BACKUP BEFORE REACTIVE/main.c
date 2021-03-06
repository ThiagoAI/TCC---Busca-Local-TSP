#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "point.h"
#include "localsearch.h"
#include "grasp.h"

//Argumento 1 = Arquivo
//Argumento 2 = Escolha (1 é SD, 2 é BFS , 3 é HC)
//Argumento 3 = Quantas vezes deve executar
//Argumento 4 = Se é pra construir a solução gulosa ou não (1 GRASP, 0 aleatória)
//Argumento 5 = Para HC é o n , para GRASP é o tamanho da RCL
int main (int argc, char** argv){
  
  //Verifica se o número de argumentos está correto
  if(argc < 5){
   printf("ERRO: NÚMERO DE ARGUMENTOS ESTÁ ERRADO!\n");
   return 1;
  }
  
  //Variável contadora
  int i = 0;
  
  //Variáveis que obteremos do arquivo
  char name[1024];
  int size;
  int choice = atoi(argv[2]);
  int n_exec = atoi(argv[3]);
  
  printf("Executaremos %d vezes.\n\n",n_exec);
  
  if(choice == 3 && argc < 6){
   printf("ERRO: NÚMERO DE ARGUMENTOS ESTÁ ERRADO! HILL CLIMBING PRECISA DE UM N!\n");
   return 1;
  }
  
  
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
   if(strcmp(buffer,"NAME") == 0){
     fscanf(tsp,"%s", buffer);
     fscanf(tsp,"%s", name);
     printf("NOME: %s\n", name);
   }
   if(strcmp(buffer,"NAME:") == 0){
     fscanf(tsp,"%s", name);
     printf("NOME: %s\n", name);
   }
   if(strcmp(buffer,"DIMENSION") == 0){
     fscanf(tsp,"%s", buffer);
     fscanf(tsp,"%d", &size);
     printf("TAMANHO: %d\n", size);
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
   new_point->id = i;
   points[i] = new_point;
  }
  
  fclose(tsp);
  
   /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */
  
  //Gerando a matriz de distâncias (só armazenamos uma metade já que ela é simétrica) 
  int** distances = (int**)malloc(sizeof(int*)*size);
  int e = size-1;
  for(i=0;i<size;i++){
    distances[i] = (int*)malloc(sizeof(int)*(size - e));
    e--;
    //printf("! %d %d %d !\n",e,size, size - e);
  }
  
  for(i=0;i<size;i++){
    for(e=0;e <= i;e++){
      if(e == i) distances[i][e] = 0;
      else distances[i][e] = euclidian_distance(points[i],points[e]);
    }
  }
  
  /*for(i=0;i<size;i++){
    for(e=0;e <= i;e++){
      printf("%d ",distances[i][e]);
    }
    printf("\n");
  }*/
  
  //Inicializamos a solução
  int* sol = (int*)malloc(sizeof(int)*size);
    for(i=0;i<size;i++){
    sol[i] = i;  
  }
  
  //Gera solução gulosa ou aleatória
  if(atoi(argv[4]) == 1){
    printf("GRASP selecionado\n");
    //greedy_start(sol,size,distances); 
  }
  else random_start(sol,size);

  //teste se repetiu algum ao criar...
  /*for(i=0;i<size;i++)
    for(e=0;e<size;e++){
      if(e != i && sol[i] == sol [e]) printf("ERROU!!!!\n");
    }*/	
  
 //Variáveis auxiliares para encontrarmos os valores 
 clock_t start, end;
 
 //Valores nos quais estamos interessados.
 double average_time = 0;
 double average_solution_time = 0;
 double average_result = 0;
 double average_swaps = 0;
 double average_changes = 0;
 int best_result = -1;
 int worst_result = -1;
 answer ans;
 
 //printf("teste: %d %d %d %d\n\n", atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
 
 //Inicializando seed aleatória
 srand(time(NULL));
 
 //Dependendo da escolha do usuário executa com um dos métodos
  for(i=0;i < n_exec;i++){
    //Verificamos qual o modo selecionado
    start = clock();
    if(atoi(argv[4]) != 1){
     random_start(sol,size);
    }
    else{
     printf("\ngreedy\n");
     greedy_random_solution(sol,size,distances,atoi(argv[5]));
     }
    end = clock();
    
    average_solution_time += (double)(end - start)/CLOCKS_PER_SEC;
    
     //PRINTS DE TESTE
     //int z = 0; 
     //for(z=0;z<size;z++) printf(" E %d",sol[z]);
     //printf("\n\n"); 
     //printf("lol\n");
     
    if(choice == 1){
      start = clock();
      ans = local_search_SD(sol,points,size,distances);
      end = clock();
    }
    if(choice == 2){
      start = clock();
      ans = local_search_BFS(sol,points,size,distances);
      end = clock();
    }
    if(choice == 3){
      start = clock();
      ans = local_search_HC(sol,points,size,distances,atoi(argv[5]));
      end = clock();
    }
    
    double time = (double)(end - start)/CLOCKS_PER_SEC;
    average_time += time;
    average_result += ans.result;
    average_swaps += ans.swaps;
    average_changes += ans.res_changes;
    printf("\nresposta: %d\n",ans.result);
    if(ans.result > worst_result || worst_result == -1) worst_result = ans.result;
    if(ans.result < best_result || best_result == -1) best_result = ans.result;
    printf("\nTerminou %d.\n",i + 1);
  }
  
  //Acertando as médias
  average_time = average_time/n_exec;
  average_solution_time = average_solution_time/n_exec;
  average_result = average_result/n_exec;
  average_swaps = average_swaps/n_exec;
  average_changes = average_changes/n_exec;
  
  printf("\n\nSol. Media: %lf\nSol. Melhor: %d\nSol. Pior: %d\nTempo Medio: %lf\nSwaps Medio: %lf\nChanges Medio: %lf\nSol. Tempo Medio: %lf\n",average_result,best_result,worst_result,average_time,average_swaps,average_changes,average_solution_time);
 
  
  
  /*for(i=0;i<size;i++){
     printf("i: %d\n",i); 
     printf("X: %lf ", points[i]->x);
     printf(" Y: %lf\n", points[i]->y);
  }*/
  
  return 0;
  
}
