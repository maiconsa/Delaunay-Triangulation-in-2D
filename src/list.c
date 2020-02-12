#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../includes/list.h"

node_t * alloc_node(void * value,int len){
    node_t * node = (node_t * )calloc(1,sizeof(node_t));
    if(!node){
        printf("Error: Memory not allocated!!!\n");
        exit(1);
    }else{
        node->data = value;
        /*
        *Aponta apenas para o valor já alocado
        */
        //malloc(len);
        //memcpy(  node->data ,value,len);
        node->prev = NULL;
        node->next = NULL;
        return node;
    }
}

list_t *  create_list(int data_size){
    list_t * list = (list_t *)calloc(1,sizeof(list_t));
    list->data_size = data_size;
    list->lenght = 0;
    return list;
}

bool append_value(list_t * list,void * value ){
    if(list_contains(list,value)) return false;

    if(list->lenght == 0 ){
            list->first_node = alloc_node(value,list->data_size);
            list->first_node->next = list->first_node;
            list->first_node->prev =  list->first_node;
            list->last_node = list->first_node;
            list->cur_node = list->first_node;
            list->lenght = 1;
    }else{
        node_t * new_node = alloc_node(value,list->data_size);
        new_node->next = list->first_node;
        new_node->prev = list->last_node;

        list->last_node->next = new_node;

        list->last_node = new_node;
        list->first_node->prev = new_node;

        list->lenght++;
    }
    return true;
}

bool list_contains(list_t * list,void *value){
    if(list->lenght <= 0 )return false;
    list->cur_node = list->first_node;
     do{
        if(list->cur_node->data == value){
            return true;
        }
     }
     while(( list->cur_node = list->cur_node->next) !=  list->first_node);
    return false;
}

bool remove_value(list_t * list,void *value){
    list->cur_node = list->first_node;
     do{
        if(list->cur_node->data == value){
            node_t * prev =  list->cur_node->prev;
            node_t * next =  list->cur_node->next;

            prev->next = next;
            next->prev = prev;
            list->lenght--;

            if( list->cur_node == list->first_node){
                list->first_node = next;
            }
            if( list->cur_node == list->last_node){
                list->first_node = prev;
            }
            free(list->cur_node);
            return true;
        }
     }while(( list->cur_node = list->cur_node->next) !=  list->first_node);
    return false;
}

int size_list(list_t *lst){
    return (lst == NULL) ? -1 : lst->lenght;
}

bool clear_list(list_t *lst){
    if(lst == NULL) return false;
    lst->cur_node = lst->first_node;
    void *ptr = NULL;
    do{
        ptr =  lst->cur_node;
        lst->cur_node = lst->cur_node->next;
        free(ptr);
    }while( lst->cur_node != lst->first_node);
    lst->lenght = 0;
    lst->data_size = 0;
    lst->last_node = NULL;
    lst->cur_node = NULL;
    lst->first_node  = NULL;

    return true;
}

bool copy_to(list_t *src,list_t *dst){
    if(size_list(src) <= 0 ) return false;
    src->cur_node = src->first_node;
    do{
        if(!append_value(dst,src->cur_node->data)){
            return false;
        }

    }while(( src->cur_node =  src->cur_node->next) != src->first_node);

   return clear_list(src);

    return true;

}


void display_value_addres(list_t * list){
    list->cur_node = list->first_node;
    printf("################### - Display List - ###################\n");
    printf("List Address:%p - List Size:%d\n",list,size_list(list));
    do{
        printf("Value Addr.:%p\n",list->cur_node->data);
    }while(( list->cur_node =  list->cur_node->next) != list->first_node);
}


