#!/bin/bash
#Scrip para executar GRASP 10x com seeds já decididas

#Array de seeds
Seeds[1]=1481236664
Seeds[2]=1481236852
Seeds[3]=1481236940
Seeds[4]=1481237244
Seeds[5]=1481237355
Seeds[6]=1481237421
Seeds[7]=1481238544
Seeds[8]=1481238556
#Esses últimos dois não tirei do time(NULL) usei random.org para gerar
Seeds[9]=550326830
Seeds[10]=131950446

#Executamos
for index in 1 2 3 4 5 6 7 8 9 10
do
	for file in TSP_test_files/*.tsp
	do
	./buscalocal $file 2 500 1 ${Seeds[index]}
	done
done

