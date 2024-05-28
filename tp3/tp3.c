#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tp3.h"  // Include the tp3.h header file

#define INITIAL_TABLE_SIZE 10000  // Adjust as needed
#define LOAD_FACTOR_THRESHOLD 0.75

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

// Function definitions

dictionary_t *dictionary_create(destroy_f destroy) {
  // Allocate memory for the dictionary
  dictionary_t *dictionary = malloc(sizeof(dictionary_t));
  if (dictionary == NULL) {
    return NULL; // Handle memory allocation error
  }

  // Initialize dictionary members
  dictionary->table_size = INITIAL_TABLE_SIZE;
  dictionary->destroy = destroy;
  dictionary->table = malloc(dictionary->table_size * sizeof(struct node *));
  if (dictionary->table == NULL) {
    free(dictionary); // Free dictionary if table allocation fails
    return NULL;
  }

  // Initialize table entries to NULL (empty slots)
  for (size_t i = 0; i < dictionary->table_size; i++) {
    dictionary->table[i] = NULL;
  }
  dictionary->size = 0;

  return dictionary;
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
  // Allocate memory for the new table
  unsigned int new_size = dictionary->table_size * 2;  // Double the size
  struct node **new_table = malloc(new_size * sizeof(struct node *));
  if (new_table == NULL) {
    return false; // Handle memory allocation error
  }

  // Initialize new table entries to NULL (empty slots)
  for (size_t i = 0; i < new_size; i++) {
    new_table[i] = NULL;
  }

  // Rehash existing key-value pairs into the new table
  for (size_t i = 0; i < dictionary->table_size; i++) {
    struct node *current = dictionary->table[i];
    while (current != NULL) {
      struct node *next = current->next;  // Store next node before rehashing

      // Compute new index based on the new table size
      unsigned int new_index = hash(current->key, dictionary) % new_size;

      // Move node to the new table
      current->next = new_table[new_index];
      new_table[new_index] = current;

      current = next;
    }
  }

  // Free the old table and update dictionary members
  free(dictionary->table);
  dictionary->table = new_table;
  dictionary->table_size = new_size;

  return true;
}


bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  if (key == NULL || strlen(key) == 0) {
    return false; // Invalid key (empty string)
  }

  unsigned int index = hash(key, dictionary) % dictionary->table_size;

  // Check if the key already exists in the list at the given index
  struct node *current = dictionary->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      // Key already exists, update value
      if (dictionary->destroy != NULL) {
        dictionary->destroy(current->value); // Free previous value if destroy function exists
      }
      current->value = value;
      return true;
    }
    current = current->next;
  }

  // Key does not exist, create a new node and add it to the list
  struct node *new_node = malloc(sizeof(struct node));
  if (new_node == NULL) {
    return false; // Memory allocation error
  }
  // Allocate memory for the key (instead of strdup)
  new_node->key = malloc(strlen(key) + 1);
  if (new_node->key == NULL) {
    free(new_node);
    return false; // Memory allocation error
  }
  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->next = dictionary->table[index];
  dictionary->table[index] = new_node;

  // Increment size only when a new element is inserted
  dictionary->size++;

  // Check load factor and rehash if necessary
  dictionary->load_factor = (double)dictionary->size / dictionary->table_size;
  if (dictionary->load_factor >= LOAD_FACTOR_THRESHOLD) {
    return rehash(dictionary); // Rehash the dictionary
  }

  return true;
}


void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  if (key == NULL || strlen(key) == 0) {
    *err = true;
    return NULL; // Invalid key (empty string)
  }

  unsigned int index = hash(key, dictionary) % dictionary->table_size;

  // Search the linked list at the given index
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
  if (key == NULL || strlen(key) == 0) {
    return false; // Invalid key (empty string)
  }

  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *prev = NULL;
  struct node *current = dictionary->table[index];

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (prev == NULL) {
        // Key found at the head of the linked list
        dictionary->table[index] = current->next;
      } else {
        prev->next = current->next;
      }

      // Do not destroy the value here
      free(current->key);
      free(current);
      dictionary->size--;

      return true;
    }
    prev = current;
    current = current->next;
  }

  return false;  // Key not found
}


void *dictionary_pop(dictionary_t* dictionary, const char *key, bool *err) {
  if (key == NULL || strlen(key) == 0) {
    *err = true;
    return NULL; // Invalid key (empty string)
  }

  unsigned int index = hash(key, dictionary) % dictionary->table_size;
  struct node *prev = NULL;
  struct node *current = dictionary->table[index];

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (prev == NULL) {
        // Key found at the head of the linked list
        dictionary->table[index] = current->next;
      } else {
        prev->next = current->next;
      }

      void *value = current->value; // Store value before freeing memory

      // Do not destroy the value here
      free(current->key);
      free(current);
      dictionary->size--;

      *err = false;
      return value;
    }
    prev = current;
    current = current->next;
  }

  // Key not found
  *err = true;
  return NULL;
}




bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  if (key == NULL || strlen(key) == 0) {
    return false; // Invalid key (empty string)
  }

  unsigned int index = hash(key, dictionary) % dictionary->table_size;

  // Search the linked list at the given index
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
  if (dictionary == NULL) {
    return;
  }

  // Free nodes and their contents
  for (size_t i = 0; i < dictionary->table_size; i++) {
    struct node *current = dictionary->table[i];
    while (current != NULL) {
      struct node *next = current->next;
      if (dictionary->destroy != NULL) {
        dictionary->destroy(current->value); // Free value if destroy function exists
      }
      free(current->key);
      free(current);
      current = next;
    }
  }

  // Free the table itself
  free(dictionary->table);
  free(dictionary);
}