#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#include "point.h"
#include "localsearch.h"
#include "grasp.h"

//Atualiza eval_array
void update_prob(double* result_array,int* iter_array,double* eval_array,int best,int worst){
  int i = 0;
  int divide = worst - best;
  for(i=0;i<7;i++){
    //Se ainda não tem média entra no if e poem um valor alto de probabilidade
    if(iter_array[i] == 0){
      eval_array[i] = (worst - best)/(divide);
    }
    else{
      eval_array[i] = (worst - (result_array[i]/iter_array[i]))/(divide);
    }
  }
}

//Por ranking
int pick_alfa(double* eval_array,double* alfa_array){
  int i = 0;
  double total = 0;
  for(i=0;i<7;i++) total += eval_array[i];
  
  double x = (double)rand()/(double)RAND_MAX;
  x = x*total;
 
  //Achamos os tres melhores e criamos o array de probabilidades
  int one = -1;
  int two = -1;
  int three = -1;
  int e = 0;
  int prob_array[7];
  for(i=0;i<7;i++) prob_array[i] = i;
  
  //TODO perdoar pelo bubblesort...
  for(i=0;i<7;i++){
  	for(e=0;e<6;e++){
  		if(eval_array[prob_array[e]] < eval_array[prob_array[e+1]]){
  			int troca = prob_array[e];
  			prob_array[e] = prob_array[e+1];
  			prob_array[e+1] = troca;
  		}
  	}
  }
  
  //for(i=0;i<7;i++) printf("eee| %lf\n",eval_array[i]);
  //for(i=0;i<7;i++) printf("oi| %d\n",prob_array[i]);
  
  //As chances para os 3 melhores e ai para o resto
  double first = (4.0*total)/14.0;
  double second = (3.0*total)/14.0;
  double third = (2.0*total)/14.0;
  double remaining = ((5.0*total)/14.0)/4.0;
  double amount[7] = {first,second,third,remaining,remaining,remaining,remaining};
  
  //printf("teste: %lf | %lf | %lf | %lf ||| %lf | %lf\n",amount[0],amount[1],amount[2],amount[3],x,total);
  
  //printf("x teste %lf\n\n",x);
  double temp = 0;
  for(i=0;i<7;i++){
   temp += amount[i];
   if(temp > x) break;
  }
  
  //Pode acontecer devido a double (chance absurda)...
  if(i > 6) i = 6;
  //printf("AAA %d %d\n",prob_array[i],i);
  return prob_array[i];
}
  
//Escolhe alfa
/*int pick_alfa(double* eval_array,double* alfa_array){
  int i = 0;
  double total = 0;
  for(i=0;i<7;i++) total += eval_array[i];
  
  double x = (double)rand()/(double)RAND_MAX;
  //printf("x inicial %lf\n\n",x);
  x = x*total;
  //printf("x teste %lf\n\n",x);
  double temp = 0;
  for(i=0;i<7;i++){
   temp += eval_array[i];
   if(temp > x) break;
  }
  
  return i;
}*/


//Argumento 1 = Arquivo
//Argumento 2 = Escolha (1 é SD, 2 é BFS , 3 é HC)
//Argumento 3 = Quantas vezes deve executar
//Argumento 4 = Se é pra construir a solução gulosa ou não (1 GRASP, 0 aleatória)
//Argumento 5 = Para HC é o n, seed fornecida para GRASP.
int main (int argc, char** argv){
  
  //Verifica se o número de argumentos está correto
  if(argc < 5){
   printf("ERRO: NÚMERO DE ARGUMENTOS ESTÁ ERRADO!\n");
   return 1;
  }
  
  //Foi fornecida seed
  if(argc == 5){
  }
  
  //Variável contadora
  int i = 0;
  
  //Variáveis que obteremos do arquivo
  char name[1024];
  int size;
  int choice = atoi(argv[2]);
  int n_exec = atoi(argv[3]);
  
  //n_exec tem que ser divisível por 10
  if(n_exec % 10 != 0 ){
    printf("ERRO: Número de execuções tem que ser divisível por 10.");
    return 1;
  } 
  
  
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
  
 //Inicializando seed aleatória
 int seed = time(NULL);
 //Se fornecida usa seed dada
 if(argc == 6){
  printf("Seed foi fornecida.\n");
  seed = atoi(argv[5]);
  }
 srand(seed);
  
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
 
 //Para o reactive grasp
 long sum_ans = 0;
 //Array com todos os possíveis valores de alfa
 double alfa_array[7] = {0.10,0.20,0.30,0.40,0.50,0.60,0.70};
 //Valores de resultado médio
 double result_array[7] = {0,0,0,0,0,0,0};
 //Numero de iterações total de cada alfa
 int iter_array[7] = {0,0,0,0,0,0,0};
 //Para probabilidade
 double eval_array[7] = {0,0,0,0,0,0,0};
 //Iterações do alfa atual
 int alfa_iter = 0;
 //Alfa inicial
 int alfa = 0;
 
 //Para gerar gráfico
 int* results_file = (int*)malloc(sizeof(int)*n_exec);
 int* original_results = (int*)malloc(sizeof(int)*n_exec);
 
 //printf("teste: %d %d %d %d\n\n", atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
 
 //Dependendo da escolha do usuário executa com um dos métodos
  for(i=0;i < n_exec;i++){
    
    //Roda as primeiras 7 para inicializar cada alfa
    //TODO pensar se isso é uma boa idéia
    if(i < 7){
     alfa = i;
    }
     
    //Atualiza 10x ao longo do programa
    if(i % (n_exec/10) == 0 && i != 0){
        printf("Atualizando eval...\n\n");
    	update_prob(result_array,iter_array,eval_array,best_result,worst_result);
    }
    //A cada 10 iterações muda o alfa
    if(alfa_iter == 10){
    	printf("Trocando alfa...\n\n");
    	alfa = pick_alfa(eval_array,alfa_array);
    	alfa_iter = 0;
    }
    
    //Verificamos qual o modo selecionado
    start = clock();
    if(atoi(argv[4]) != 1){
     random_start(sol,size);
    }
    else{
     //printf("\ngreedy\n");
     greedy_random_solution(sol,size,distances,alfa_array[alfa]);
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
    //printf("\nresposta: %d\n",ans.result);
    if(ans.result > worst_result || worst_result == -1) worst_result = ans.result;
    if(ans.result < best_result || best_result == -1) best_result = ans.result;
    
    
    alfa_iter++;
    iter_array[alfa] += 1;
    result_array[alfa] += ans.result;
    results_file[i] = ans.result;
    original_results[i] = ans.original_result;
    
    printf("\nTerminou %d.\n",i + 1);
  }
  
  //Acertando as médias
  average_time = average_time/n_exec;
  average_solution_time = average_solution_time/n_exec;
  average_result = average_result/n_exec;
  average_swaps = average_swaps/n_exec;
  average_changes = average_changes/n_exec;
  
  //PRINTF FINAL MOSTRANDO RESULTADOS
  printf("\n\nSol. Media: %lf\nSol. Melhor: %d\nSol. Pior: %d\nTempo Medio: %lf\nSwaps Medio: %lf\nChanges Medio: %lf\nSol. Tempo Medio: %lf\n",average_result,best_result,worst_result,average_time,average_swaps,average_changes,average_solution_time);
  printf("Seed: %d\n",seed);
  
  //PARTE DE ESCRITA
  char nome_csv[124];
  char nome_log[124]; 
  char nome_original[124];
  strcpy(nome_csv,argv[1]);
  strcat(nome_csv,".csv");
  strcpy(nome_log,argv[1]);
  strcat(nome_log,".log");
  strcpy(nome_original,argv[1]);
  strcat(nome_original,".original.csv");
  
  int exists = 0;
  //Se existir entra no if
  if(access(nome_csv, F_OK) != -1){
  exists = 1;
  }
  
  //Escrevemos o arquivo csv
  FILE* csv = fopen(nome_csv,"a");
  
  if(exists == 1)  fprintf(csv,",");
  for(i = 0; i < n_exec-1; i++){
    fprintf(csv,"%d,",results_file[i]);
  }
  fprintf(csv,"%d",results_file[n_exec-1]);
   
  fclose(csv);
  exists = 0;
  
  if(access(nome_original, F_OK) != -1){
  exists = 1;
  }
  
  //Escrevemos o arquivo original
  FILE* original = fopen(nome_original,"a");
  
  if(exists == 1)  fprintf(original,",");
  for(i = 0; i < n_exec-1; i++){
    fprintf(original,"%d,",original_results[i]);
  }
  fprintf(original,"%d",original_results[n_exec-1]);
   
  fclose(original);
  
  
  //Escrevemos o arquivo log 
 time_t horario = time(NULL);
 struct tm info = *(localtime(&horario));
 int hora = info.tm_hour;
 int minuto = info.tm_min;
 int dia = info.tm_mday;
 int mes = info.tm_mon + 1;
 int ano = 1900 + info.tm_year;
  FILE* log = fopen(nome_log,"a");
  fprintf(log,"%d/%d/%d  %d:%d - Execucao de %s com %d iteracoes - Seed foi %d e o melhor resultado %d.\n",dia,mes,ano,hora,minuto,argv[1],n_exec,seed,best_result);
  
  fclose(log);
  
  
  //Print de teste para visualizar os alfas
  for(i = 0;i < 7;i ++){
  printf("alfa %lf: %lf | %d | %lf \n",alfa_array[i],result_array[i],iter_array[i],eval_array[i]);
  }
  
  
  /*for(i=0;i<size;i++){
     printf("i: %d\n",i); 
     printf("X: %lf ", points[i]->x);
     printf(" Y: %lf\n", points[i]->y);
  }*/
  
  return 0;
  
}
