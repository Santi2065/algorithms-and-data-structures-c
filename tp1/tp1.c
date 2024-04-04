#include "tp1.h"
#include <stdlib.h>

bool is_prime(int x){

    if (x <= 1) return false; // 1 o menor a uno no son primos
    else if (x <= 3) return true; // 2 y 3 son primos

    else if (x % 2 == 0|| x % 3 == 0) return false; // si son divisibles por 2 o 3 no son primos

    for (int i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }

    return true;

}

int storage_capacity(float d, float v){return (int) (d / v);}

void swap(int *x, int *y) {

    int temp = *x;
    *x = *y;
    *y = temp;

}

int array_max(const int *array, int length) {

    int max = array[0];
    for (int i = 1; i < length; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;

}

void array_map(int *array, int length, int f(int)) {

    if( f == NULL) {
        return;
    }

    for (int i = 0; i < length; i++) {
        array[i] = f(array[i]);
    }

}

int *copy_array(const int *array, int length) {

    if (array == NULL) {
        return NULL;
    }
    int *copy = malloc(length * sizeof(int));
    if (copy == NULL) {
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        copy[i] = array[i];
    }
    return copy;

}

int **copy_array_of_arrays(const int **array_of_arrays, const int *array_lenghts, int array_amount){

    if (array_of_arrays == NULL) {
        return NULL;
    }
    int **copy = malloc(array_amount * sizeof(int*));
    if (copy == NULL) {
        return NULL;
    }
    for (int i = 0; i < array_amount; i++) {
        copy[i] = copy_array(array_of_arrays[i], array_lenghts[i]);
    }
    return copy;

}

void free_array_of_arrays(int **array_of_arrays, int *array_lenghts, int array_amount){

    if (array_of_arrays == NULL) {
        return;
    }
    for (int i = 0; i < array_amount; i++) {
        free(array_of_arrays[i]);
    }
    free(array_of_arrays);
    free(array_lenghts);

}

void bubble_sort(int *array, int length){

    if (array == NULL) {
        return;
    }
    for (int i = 0; i < length - 1; i++) {
        for (int j = 0; j < length - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }

}

bool array_equal(const int *array1, int length1, const int *array2, int length2){

    //chequeo si son NULL
    if(array1 == NULL || array2 == NULL){
        if (array1 == NULL && array2 == NULL){
            return true;
        }
        else {
            return false;
        }
    }
    
    //Si los largos son distintos no son iguales
    if(length1 != length2){
        return false;
    }
    
    //comparo las arrays
    for(int i = 0; i < length1; i++){
        if(array1[i] != array2[i]){
            return false;
        }
    }
    return true;
}

bool integer_anagrams(const int *array1, int length1,
                      const int *array2, int length2){
    //chequeo si son NULL
    if(array1 == NULL || array2 == NULL || length1 != length2){
        return false;
    }

    //copio las arrays
    int *array1_copy = copy_array(array1, length1);
    int *array2_copy = copy_array(array2, length2);
    if (array1_copy == NULL || array2_copy == NULL) {
        free(array1_copy);
        free(array2_copy);
        return false;
    }

    //ordeno las arrays
    bubble_sort(array1_copy, length1);
    bubble_sort(array2_copy, length2);


    //comparo las arrays
    bool result = array_equal(array1_copy, length1, array2_copy, length2);


    //libero la memoria
    free(array1_copy);
    free(array2_copy);

    return result;

}
