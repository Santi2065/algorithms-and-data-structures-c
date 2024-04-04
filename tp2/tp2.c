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
    return list;

}

size_t list_length(const list_t *list){
    return list->size;
}

bool list_is_empty(const list_t *list){
    return list_lenght(&list)==0;
}

bool list_insert_head(list_t *list, void *value){

    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;
    }

    new_node->value = value;
    new_node->next = list->head;
    new_node->prev = NULL;
    return true;
    
}

bool list_insert_tail(list_t *list, void *value){
    for (node_t* node = list->head; node != NULL; node = node->next) {
        if (node->next == NULL) {
            node_t* new_node = malloc(sizeof(node_t));
            if (new_node == NULL) {
                return false;
            }
            new_node->value = value;
            new_node->next = NULL;
            new_node->prev = node;
            node->next = new_node;
            return true;
        }
    }
}

void *list_peek_head(const list_t *list){
    return list->head->value;
}

void *list_peek_tail(const list_t *list){
    for (node_t* node = list->head; node != NULL; node = node->next) {
        if (node->next == NULL) {
            return node->value;
        }
    }
}

void *list_pop_head(list_t *list){
    void* value = list->head->value;
    void* head = list->head;
    list->head = list->head->next;
    free(head);
    return value;

}

void *list_pop_tail(list_t *list){
    for (node_t* node = list->head; node != NULL; node = node->next) {
        if (node->next->next == NULL) {
            void* value = node->next->value;
            void* tail = node->next;
            node->next = NULL;
            free(tail);
            return value;
        }
    }
}

void list_destroy(list_t *list, void destroy_value(void *)){
    for (node_t* node = list->head; node != NULL; node = node->next) {
        destroy_value(node->value);
        free(node);
    }
    free(list);
    return;
}

list_iter_t *list_iter_create_head(list_t *list){
    return NULL;
}

list_iter_t *list_iter_create_tail(list_t *list){
    return NULL;
}

bool list_iter_forward(list_iter_t *iter){
    return false;
}

bool list_iter_backward(list_iter_t *iter){
    return false;
}

void *list_iter_peek_current(const list_iter_t *iter){
    return NULL;
}

bool list_iter_at_last(const list_iter_t *iter){
    return false;
}

bool list_iter_at_first(const list_iter_t *iter){
    return false;
}

void list_iter_destroy(list_iter_t *iter){
    return;
}

bool list_iter_insert_after(list_iter_t *iter, void *value){
    return false;
}

bool list_iter_insert_before(list_iter_t *iter, void *value){
    return false;
}

void *list_iter_delete(list_iter_t *iter){
    return NULL;
}