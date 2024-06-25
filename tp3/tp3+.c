#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// User-defined data type (replace with your actual data type)
typedef int data_t;

// Function to hash data elements (replace with your custom hash function if needed)
unsigned int hash_data(const data_t *data, size_t data_size) {
    unsigned int hash = 0;
    for (size_t i = 0; i < data_size; i++) {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    return hash;
}

// Function to compare data elements for equality (replace with your custom comparison function)
bool compare_data(const data_t *data1, const data_t *data2, size_t data_size) {
    for (size_t i = 0; i < data_size; i++) {
        if (data1[i] != data2[i]) {
            return false;
        }
    }
    return true;
}

// Structure for a node in the modified list
struct node {
    data_t *data;
    size_t data_size;
    struct node *next;
};

// Structure for the modified list with counting filter
struct modified_list {
    struct node *head;
    size_t size;

    // Counting filter implementation (adjust based on your needs)
    size_t filter_size;
    unsigned char *filter; // Use a more efficient data structure if necessary

    // User-provided functions
    unsigned int (*hash_func)(const data_t *, size_t);
    bool (*compare_func)(const data_t *, const data_t *, size_t);
};

// Function to create a new modified list
modified_list_t *modified_list_create(size_t filter_size,
                                      unsigned int (*hash_func)(const data_t *, size_t),
                                      bool (*compare_func)(const data_t *, const data_t *, size_t)) {
    modified_list_t *list = (modified_list_t *)malloc(sizeof(modified_list_t));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->size = 0;
    list->filter_size = filter_size;

    list->filter = (unsigned char *)calloc(filter_size, sizeof(unsigned char));
    if (list->filter == NULL) {
        free(list);
        return NULL;
    }

    list->hash_func = hash_func;
    list->compare_func = compare_func;

    return list;
}

// Function to insert an element into the modified list
bool modified_list_insert(modified_list_t *list, const data_t *data, size_t data_size) {
    // Check for duplicates in the counting filter
    for (size_t i = 0; i < list->filter_size; i++) {
        unsigned int index = (hash_data(data, data_size) + i) % list->filter_size;
        if (list->filter[index] == 1) {
            // Potential duplicate, check the list
            struct node *current = list->head;
            while (current != NULL) {
                if (list->compare_func(current->data, data, data_size)) {
                    return false; // Duplicate found, do nothing
                }
                current = current->next;
            }
        }
    }

    // Not a duplicate, insert into the list
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL) {
        return false;
    }

    new_node->data = (data_t *)malloc(data_size);
    if (new_node->data == NULL) {
        free(new_node);
        return false;
    }
    memcpy(new_node->data, data, data_size);
    new_node->data_size = data_size;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;

    // Update counting filter
    for (size_t i = 0; i < list->filter_size; i++) {
        unsigned int index = (hash_data(data, data_size) + i) % list->filter_size;
        list->filter[index] = 1;  // Set the corresponding bits in the filter
    }

    return true;
}
