#include <stdbool.h>

#ifndef LIST_H
#define LIST_H



struct node;

 typedef struct node{
    void * data;
    struct node * prev;
    struct node *next;
}node_t;

typedef struct lst{
    node_t * first_node;
    node_t * last_node;
    node_t * cur_node;
    int lenght;
    int data_size;
}list_t;


node_t * alloc_node(void * value,int len);
list_t *  create_list(int data_size);
bool append_value(list_t * lst,void * value);
bool remove_value(list_t * lst,void *value);
bool list_contains(list_t * lst,void *value);
int size_list(list_t *lst);
bool copy_to(list_t *src,list_t *dst);
bool clear_list(list_t *lst);

void display_list(list_t * root);


#endif // LIST_H
