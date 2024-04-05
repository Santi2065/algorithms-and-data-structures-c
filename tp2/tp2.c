#include "tp2.h"
#include <stdlib.h>
#include <stdbool.h>

struct node;
typedef struct node node_t;

struct node {
    void* value;
    node_t* next;
    node_t* prev;
};

struct list {
    node_t* head;
    node_t* tail;
    size_t size;
};

struct list_iter {
    list_t* list;
    node_t* curr;
};

list_t *list_new(){
    
    list_t* list = malloc(sizeof(list_t));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;

}

size_t list_length(const list_t *list){
    if(list == NULL){
        return 0;
    }
    return list->size;
}

bool list_is_empty(const list_t *list){
    return !list_length(list);
}

bool list_insert_head(list_t *list, void *value){

    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;
    }

    new_node->value = value;
    if(list->head != NULL){
        new_node->next = list->head;
        list->head->prev = new_node;
    }
    else{
        new_node->next = NULL;
        list->tail = new_node;
    }
    new_node->prev = NULL;
    list->head = new_node;
    list->size++;
    return true;
    
}

bool list_insert_tail(list_t *list, void *value){
        
    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;
    }

    new_node->value = value;
    new_node->next = NULL;
    if(list->tail != NULL){
        list->tail->next = new_node;
    }
    new_node->prev = list->tail;
    list->tail = new_node;
    list->size++;
    return true;
}

void *list_peek_head(const list_t *list){
    if(list == NULL || list->head == NULL){
        return NULL;
    }
    return list->head->value;
}

void *list_peek_tail(const list_t *list){
    if(list == NULL || list->tail == NULL){
        return NULL;
    }
    return list->tail->value;
}

void *list_pop_head(list_t *list){
    if(list == NULL || list->head == NULL){
        return NULL;
    }
    void* value = list->head->value;
    node_t* head = list->head;
    list->head = list->head->next;
    if(list->head != NULL){
        list->head->prev = NULL;
    }
    else{
        list->tail = NULL;
    }
    list->size--;
    free(head);
    return value;

}

void *list_pop_tail(list_t *list){
    if(list == NULL || list->head == NULL){
        return NULL;
    }
    void* value = list->tail->value;
    node_t* tail = list->tail;
    list->tail = list->tail->prev;
    if(list->tail != NULL){
        list->tail->next = NULL;
    }
    else{
        list->head = NULL;
    }
    list->size--;
    free(tail);
    return value;
}

void list_destroy(list_t *list, void destroy_value(void *)){
    node_t* node = list->head;
    while (node != NULL) {
        node_t* next_node = node->next;
        if(destroy_value != NULL){
            destroy_value(node->value);
        }
        free(node);
        node = next_node;
    }
    free(list);
    return;
}

list_iter_t *list_iter_create_head(list_t *list){

    list_iter_t* iter = malloc(sizeof(list_iter_t));
    if (iter == NULL) {
        return NULL;
    }
    iter->list = list;
    iter->curr = list->head;
    return iter;
    
}

list_iter_t *list_iter_create_tail(list_t *list){
    list_iter_t* iter = malloc(sizeof(list_iter_t));
    if (iter == NULL) {
        return NULL;
    }
    iter->list = list;
    iter->curr = list->tail;
    return iter;
}

bool list_iter_forward(list_iter_t *iter){
    
    if (iter->curr == NULL) {
        return false;
    }
    iter->curr = iter->curr->next;
    return true;

}

bool list_iter_backward(list_iter_t *iter){

    if (iter->curr == NULL || iter->curr->prev == NULL) {
        return false;
    }
    iter->curr = iter->curr->prev;
    return true;

}

void *list_iter_peek_current(const list_iter_t *iter){
    return iter->curr->value;
}

bool list_iter_at_last(const list_iter_t *iter){
    if (iter->curr == NULL||iter->curr->next == NULL) {
        return true;
    }
    return false;
}

bool list_iter_at_first(const list_iter_t *iter){
    if (iter->curr == NULL||iter->curr->prev == NULL) {
        return true;
    }
    return false;
}

void list_iter_destroy(list_iter_t *iter){
    
    free(iter);
    return;
}

bool list_iter_insert_after(list_iter_t *iter, void *value){
    
    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;
    }
    new_node->value = value;
    new_node->next = iter->curr->next;
    new_node->prev = iter->curr;
    iter->curr->next = new_node;
    iter->list->size++;
    return true;

}

bool list_iter_insert_before(list_iter_t *iter, void *value){
    
    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;
    }
    new_node->value = value;
    new_node->next = iter->curr;
    new_node->prev = iter->curr->prev;
    iter->curr->prev = new_node;
    iter->list->size++;
    return true;
}

void *list_iter_delete(list_iter_t *iter){
    
    void* value = iter->curr->value;
    if (iter->curr->prev == NULL) {
        iter->list->head = iter->curr->next;
    } else {
        iter->curr->prev->next = iter->curr->next;
    }
    if (iter->curr->next == NULL) {
        iter->list->tail = iter->curr->prev;
    } else {
        iter->curr->next->prev = iter->curr->prev;
    }
    free(iter->curr);
    iter->list->size--;
    return value;
}