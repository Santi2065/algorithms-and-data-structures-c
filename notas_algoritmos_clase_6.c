//Cola con Prioridad estilo lista

struct elemento{
    int dato;
    int prioridad;
    struct elemento *sig;
};

struct colaPrio{
    struct elemento *primero;
};

//create(struct colaPrio * c);



//destroy(struct colaPrio * c);
//encolar(struct colaPrio * c, elem *e, int p);
//elemento * removeMax(struct colaPrio * c);
//elemento *getMax(struct colaPrio * c);
//build(struct *colaPrio, elem e[], int prio[], int cantidad);

//Heap
    //definiciones previas
        //arbol: grafo conexo y aciclico
        //arbol binario: arbol en el que cada nodo tiene a lo sumo dos hijos
            struct nodo{
                int dato;
                struct nodo *izq;
                struct nodo *der;
            };
            //h+1 <= nodos <= 2^(h+1) - 1
            // arbol completo: todos los niveles estan completos, excepto el ultimo
                //2^h <= nodos <= 2^(h+1) - 1
    //un heap es un arbol binario completo