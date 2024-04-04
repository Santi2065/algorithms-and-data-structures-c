#include <stddef.h>

struct nodo {
    int dato;
    struct nodo *sig;
};

struct lista_simple {
    struct nodo *primero;
    size_t cantidad;
    struct nodo *ultimo;
};

void insertar_ultimo(struct lista_simple *plista, int dato) {
    struct nodo *nuevo = malloc(sizeof(struct nodo));
    nuevo->dato = dato;
    nuevo->sig = NULL;
    if (plista->cantidad == 0) {
        plista->primero = nuevo;
    } else {
        for(struct nodo *actual = plista->primero; actual->sig != NULL; actual = actual->sig){
            actual->sig = nuevo;}
    }
    plista->cantidad++;
}

//Lista Circular
struct lista_circular {
    struct nodo *primero;
    size_t cantidad;
};

/*  Crear
    Destruir
    Insertar_Actual
    Obtener_Actual
    Avanzar
*/

//Crear O(1)
struct lista_circular *crear_lista_circular() {
    struct lista_circular *nueva = malloc(sizeof(struct lista_circular));
    nueva->primero = NULL;
    nueva->cantidad = 0;
    return nueva;
}

//Destruir O(n)
void destruir_lista_circular(struct lista_circular *plista) {
    struct nodo *actual = plista->primero;
    for (size_t i = 0; i < plista->cantidad; i++) {
        struct nodo *siguiente = actual->sig;
        free(actual);
        actual = siguiente;
    }
    free(plista);
}

//Insertar_Actual O(1)
void insertar_actual(struct lista_circular *plista, int dato) {
    struct nodo *nuevo = malloc(sizeof(struct nodo));
    nuevo->dato = dato;
    if (plista->cantidad == 0) {
        nuevo->sig = nuevo;
        plista->primero = nuevo;
    } else {
        nuevo->sig = plista->primero->sig;
        plista->primero->sig = nuevo;
    }
    plista->cantidad++;
}

//lista circular doblemente enlazada


