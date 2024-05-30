#include "tp3.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_TABLE_SIZE 512
#define LOAD_FACTOR_THRESHOLD 0.85

struct dictionary {
  unsigned int table_size;
  destroy_f destroy;
  struct node **table;
  size_t size;
  double load_factor;
};

struct node {
  char *key;
  void *value;
  struct node *next;
};

dictionary_t *dictionary_create(destroy_f destroy) {
  dictionary_t *dictionary = malloc(sizeof(dictionary_t));
  if (dictionary == NULL) {
    return NULL;
  }
  dictionary->table_size = INITIAL_TABLE_SIZE;
  dictionary->destroy = destroy;
  dictionary->table = calloc(dictionary->table_size, sizeof(struct node *));
  if (dictionary->table == NULL) {
    free(dictionary);
    return NULL;
  }
  dictionary->size = 0;
  return dictionary;
}

struct node *node_create(const char *key, void *value) {
  struct node *new_node = malloc(sizeof(struct node));
  if (new_node == NULL) {
    return NULL; 
  }
  new_node->key = malloc(strlen(key) + 1);
  if (new_node->key == NULL) {
    free(new_node);
    return NULL;
  }
  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->next = NULL;
  return new_node;
}

unsigned int jenkins_one_at_a_time_hash(const char *key, size_t length) {
    unsigned int hash = 0;
    for (size_t i = 0; i < length; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned int hash(const char *key, dictionary_t *dictionary) {
    return jenkins_one_at_a_time_hash(key, strlen(key)) % dictionary->table_size;
}

bool rehash(dictionary_t *dictionary) {
  unsigned int new_size = dictionary->table_size * 2;
  struct node **new_table = calloc(new_size, sizeof(struct node *));
  if (new_table == NULL) {
    return false;
  }
  for (size_t i = 0; i < dictionary->table_size; i++) {
    struct node *current = dictionary->table[i];
    while (current != NULL) {
      struct node *next = current->next;
      unsigned int new_index = jenkins_one_at_a_time_hash(current->key, strlen(current->key)) % new_size;
      current->next = new_table[new_index];
      new_table[new_index] = current;
      current = next;
    }
  }

  free(dictionary->table);
  dictionary->table = new_table;
  dictionary->table_size = new_size;
  return true;
}

bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *current = dictionary->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (dictionary->destroy != NULL) {
        dictionary->destroy(current->value);
      }
      current->value = value;
      return true;
    }
    current = current->next;
  }
  struct node *new_node = node_create(key, value);
  if (new_node == NULL) {
    return false;
  }
  new_node->next = dictionary->table[index];
  dictionary->table[index] = new_node;
  dictionary->size++;

  dictionary->load_factor = (double)dictionary->size / dictionary->table_size;
  if (dictionary->load_factor >= LOAD_FACTOR_THRESHOLD) {
    return rehash(dictionary);
  }
  return true;
}

void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *current = dictionary->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      *err = false;
      return current->value;
    }
    current = current->next;
  }
  *err = true;
  return NULL;
}

bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *prev = NULL;
  struct node *current = dictionary->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (prev == NULL) {
        dictionary->table[index] = current->next;
      } else {
        prev->next = current->next;
      }
      free(current->key);
      if (dictionary->destroy != NULL) {
        dictionary->destroy(current->value);
      }
      free(current);
      dictionary->size--;
      return true;
    }
    prev = current;
    current = current->next;
  }
  return false;
}

void *dictionary_pop(dictionary_t* dictionary, const char *key, bool *err) {
  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *prev = NULL;
  struct node *current = dictionary->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (prev == NULL) {
        dictionary->table[index] = current->next;
      } else {
        prev->next = current->next;
      }
      void *value = current->value;
      free(current->key);
      free(current);
      dictionary->size--;

      *err = false;
      return value;
    }
    prev = current;
    current = current->next;
  }
  *err = true;
  return NULL;
}

bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *current = dictionary->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      return true;
    }
    current = current->next;
  }
  return false;
}

size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->size;
}

void dictionary_destroy(dictionary_t *dictionary) {
  for (size_t i = 0; i < dictionary->table_size; i++) {
    struct node *current = dictionary->table[i];
    while (current != NULL) {
      struct node *next = current->next;
      if (dictionary->destroy != NULL) {
        dictionary->destroy(current->value);
      }
      free(current->key);
      free(current);
      current = next;
    }
  }
  free(dictionary->table);
  free(dictionary);
}