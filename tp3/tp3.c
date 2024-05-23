#include "tp3.h"
#include <stdlib.h>
#include <stdio.h>

struct dictionary {
  struct node *head;
  destroy_f destroy;
  size_t size;
};

struct node {
  struct node *next;
  const char *key;
  void *value;
};

dictionary_t *dictionary_create(destroy_f destroy) { 
  dictionary_t *dictionary = malloc(sizeof(dictionary_t));
  if (dictionary == NULL) {
    return NULL;
  }
  dictionary->head = NULL;
  dictionary->destroy = destroy;
  dictionary->size = 0;
  return dictionary;
};

bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  if (dictionary_contains(dictionary, key)) {
    bool err;
    struct node *node = dictionary_get(dictionary, key, &err);
    node->value = value;
    if(err){
      return false;
    }
    return true;
  }
  struct node *new_node = malloc(sizeof(struct node));
  if (new_node == NULL) {
    return false;
  }
  new_node->key = key;
  new_node->value = value;
  new_node->next = dictionary->head;
  dictionary->head = new_node;
  dictionary->size++;
  return true;
};

void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  struct node *current = dictionary->head;
  *err = true;
  while (current != NULL) {
    if (current->key == key) {
      *err = false;
      return current->value;
    }
    current = current->next;
  }
  return NULL;
};

bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  struct node *current = dictionary->head;
  if(current->key == key) {
    dictionary->head = current->next;
    dictionary->size--;
    return true;
  }
  struct node *previous = current;
  current = current->next;
  while (current->next != NULL) {
    if (current->key == key) {
      previous->next = current->next;
      free(current);
      dictionary->size--;
      return true;
    }
    previous = current;
    current = current->next;
  }
  return false;
};

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  struct node *current = dictionary->head;
  if(current->key == key) {
    void *value = current->value;
    dictionary->head = current->next;
    free(current);
    dictionary->size--;
    *err = false;
    return value;
  }
  struct node *previous = current;
  current = current->next;
  while (current->next != NULL) {
    if (current->key == key) {
      void *value = current->value;
      previous->next = current->next;
      free(current);
      dictionary->size--;
      *err = false;
      return value;
    }
    previous = current;
    current = current->next;
  }
  *err = true;
  return NULL;
};

bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  struct node *current = dictionary->head;
  while (current != NULL) {
    if (current->key == key) {
      return true;
    }
    current = current->next;
  }
  return false;
};

size_t dictionary_size(dictionary_t *dictionary) { return dictionary->size; };

void dictionary_destroy(dictionary_t *dictionary){
  struct node *current = dictionary->head;
  while (current != NULL) {
    struct node *next = current->next;
    if (dictionary->destroy != NULL) {
      dictionary->destroy(current->value);
    }
    free(current);
    current = next;
  }
  free(dictionary);
};
