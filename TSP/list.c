#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int_list* create_list(){
  int_list* list = (int_list*)malloc(sizeof(int_list));
  list->amount = 0;
  list->cities = NULL;
}

void add_list(int_list* list,int x){
  city* new = (city*)malloc(sizeof(city));
  new->point = x;
  new->next = list->cities;
  list->cities = new;
  list->amount++;
}

int get_point(int_list* list,int i){
  if(i >= list->amount) return -10;
  
  city* temp = list->cities;
  int k = 0;
  
  for(k = 0; k < i; k++){
  temp = temp->next;  
  }
  
  return temp->point;
}

int_list* destroy_list(int_list* list){
 city* temp = list->cities;
  while(temp != NULL){
    city* temp2 = temp->next;
    free(temp);
    temp = temp2;
  }
  
  free(list);
  return NULL;
}
