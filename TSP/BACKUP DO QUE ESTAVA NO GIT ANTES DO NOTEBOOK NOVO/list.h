#ifndef LIST_H_
#define LIST_H_

typedef struct node_list{
  struct node_list* next;
  int point;
}city;

typedef struct master_list{
  city* cities;
  int amount;
}int_list;

//Funções básicas
int_list* create_list();
void add_list(int_list* list,int x);
int get_point(int_list* list,int i);
int_list* destroy_list(int_list* list);

#endif
