#!/bin/bash
#Scrip para executar GRASP 10x com seeds já decididas

#Array de seeds
#Seeds[1]=1481236664
#Seeds[2]=1481236852
#Seeds[3]=1481236940
Seeds[1]=1481237244
Seeds[2]=1481237355
Seeds[3]=1481237421
#Seeds[7]=1481238544
#Seeds[8]=1481238556
#Esses últimos dois não tirei do time(NULL) usei random.org para gerar
#Seeds[9]=550326830
#Seeds[10]=131950446

#Executamos
#4 5 6 7 8 9 10
#PARA OS GRANDES VAMOS EXECUTAR 3X PRIMEIRO SE LEMBRE DE MUDAR SEED DEPOIS
for index in 1 2 3 
do
	for file in TSP_test_files/*.tsp
	do
	./buscalocal $file 2 250 1 ${Seeds[index]}
	done
done

