//Listas con puntero a ultimo

    //insertar primero
        //O(1)

    //insertar ultimo
        //O(1)

    //borrar primero
        //O(1)

    //borrar ultimo
        //O(n)

//Listas sin puntero a ultimo

    //insertar primero
        //O(1)

    //insertar ultimo
        //O(n)

    //borrar primero
        //O(1)

    //borrar ultimo
        //O(n)

//Pila (Stack)
    //Voy apilando datos

    //push (meto arriba)
    //pop (saco de arriba)

    #include <stddef.h>
    struct pila {
        struct nodo *tope;
        size_t cantidad;
    };

    struct nodo {
        int dato;
        struct nodo *sig;
    };

    //Interfaz
        //CrearPila() O(1) (si fuera un vector de TamDinamico, O(1))
    

            void CrearPila(){
                struct pila *nueva = malloc(sizeof(struct pila));
                nueva->tope = NULL;
                nueva->cantidad = 0;
                return nueva;
            }
        

        //DestruirPila()    O(n) (si fuera un vector de TamDinamico, O(1))

            void DestruirPila(struct pila *pila){
                struct nodo *actual = pila->tope;
                for (size_t i = 0; i < pila->cantidad; i++) {
                    struct nodo; // Forward declaration

                    struct nodo *siguiente = actual->sig;
                    free(actual);
                    actual = siguiente;
                }
                free(pila);
            }

        //Push(struct pila *pila, int dato) O(1) (si fuera un vector de TamDinamico, O(1))
            
            void Push(struct pila *pila, int dato){
                struct nodo *nuevo = malloc(sizeof(struct nodo));
                nuevo->dato = dato;
                nuevo->sig = pila->tope;
                pila->tope = nuevo;
                pila->cantidad++;
            }

        //Int Pop(struct pila *pila) O(1)  (si fuera un vector de TamDinamico, O(1))

            int Pop(struct pila *pila){
                int dato = pila->tope->dato;
                struct nodo *aux = pila->tope;
                pila->tope = pila->tope->sig;
                free(aux);
                pila->cantidad--;
                return dato;
            }

        //int Top(struct pila *pila) O(1) (si fuera un vector de TamDinamico, O(1))
            
            int Top(struct pila *pila){
                return pila->tope->dato;
            }

// Cola (Queue FIFO)

    struct cola
    {
        struct nodo *primero;
        struct nodo *ultimo;
        size_t cantidad;
    };
    
    //void CrearCola(struct cola *cola) O(1)

        void CrearCola(struct cola *cola){
            struct cola *nueva = malloc(sizeof(struct cola));
            nueva->primero = NULL;
            nueva->ultimo = NULL;
            nueva->cantidad = 0;
            return nueva;
        }

    //void DestruirCola(struct cola *cola) O(n)

        void DestruirCola(struct cola *cola){
            struct nodo *actual = cola->primero;
            for (size_t i = 0; i < cola->cantidad; i++) {
                struct nodo *siguiente = actual->sig;
                free(actual);
                actual = siguiente;
            }
            free(cola);
        }

    //void Encolar(struct cola *cola, int dato) O(1)

        void Encolar(struct cola *cola, int dato){
            struct nodo *nuevo = malloc(sizeof(struct nodo));
            nuevo->dato = dato;
            nuevo->sig = NULL;
            if (cola->cantidad == 0) {
                cola->primero = nuevo;
            } else {
                cola->ultimo->sig = nuevo;
            }
            cola->ultimo = nuevo;
            cola->cantidad++;
        }

    //int Desencolar(struct cola *cola) O(1)

        int Desencolar(struct cola *cola){
            int dato = cola->primero->dato;
            struct nodo *aux = cola->primero;
            cola->primero = cola->primero->sig;
            free(aux);
            cola->cantidad--;
            return dato;
        }

//para pensar en casa
    //Como implementar una cola con 2 pilas
    //se implementa con 2 pilas, una para encolar y otra para desencolar


    //Como implementar una pila con 2 colas



    //Problema del Carnaval
    //tengo todas mis carrozas que van llegando en desorden pero yo tengo que lograr que las carrozas desfilen en orden
    //lo unico que tengo es una calle cortada 
    //resolver el problema con pilas y colas

    //Solucion

    //Pila de espera

    //Cola de desfile
