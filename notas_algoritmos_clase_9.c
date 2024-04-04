//metadatos
struct node{
    int Key;
    void *Value;
    Node *Left;
    Node *Right;
}

struct Tree{
    Node *Root;
    size_t Size;
}

//Primitivas
//tree *CreateTree(); o(1)
//Node *CreateNode(int Key, void *Value);
Node *CreateNode(int Key, void *Value){
    Node *N = (Node*)malloc(sizeof(Node));
    if(N == NULL){
        return NULL;
    }
    N->Key = Key;
    N->Value = Value;
    N->Left = NULL;
    N->Right = NULL;
    return N;
}
//bool Insert(tree *T, int Key, void *Value);o(log(n))
//bool InsertNode(Node *N, int Key, void *Value);
bool InsertNode(Node *N, int Key, void *Value){
    if(N->Key == Key){
        return false;
    }
    if(N->Key > Key){
        if(N->Left == NULL){
            N->Left = CreateNode(Key, Value);
            return true;
        }
        return InsertNode(N->Left, Key, Value);
    }
    if(N->Right == NULL){
        N->Right = CreateNode(Key, Value);
        return true;
    }
    return InsertNode(N->Right, Key, Value);
}
//bool Delete
//bool DeleteNode(Node *N, int Key);
bool DeleteNode(Node *N, int Key){
    if(N == NULL){
        return false;
    }
    if(N->Key == Key){
        if(N->Left == NULL && N->Right == NULL){
            free(N);
            return true;
        }
        if(N->Left == NULL){
            Node *Temp = N->Right;
            free(N);
            return true;
        }
        if(N->Right == NULL){
            Node *Temp = N->Left;
            free(N);
            return true;
        }
        Node *Temp = N->Right;
        while(Temp->Left != NULL){
            Temp = Temp->Left;
        }
        N->Key = Temp->Key;
        N->Value = Temp->Value;
        return DeleteNode(N->Right, Temp->Key);
    }
    if(N->Key > Key){
        return DeleteNode(N->Left, Key);
    }
    return DeleteNode(N->Right, Key);
}
//void *searchKey
//void Pre_orden(Node *N, void (*FuncVisit)(Node* Node)));
void pre_orden(Node *N, void (*FuncVisit)(Node* Node)){
    if(N == NULL){
        return;
    }
    FuncVisit(N);
    pre_orden(N->Left, FuncVisit);
    pre_orden(N->Right, FuncVisit);
}

//void in_order(Node *N, void (*FuncVisit)(Node* Node));
void in_order(Node *N, void (*FuncVisit)(Node* Node)){
    if(N == NULL){
        return;
    }
    in_order(N->Left, FuncVisit);
    FuncVisit(N);
    in_order(N->Right, FuncVisit);
}
// Node *searchNode(Node *N, int Key);
Node *searchNode(Node *N, int Key){
    if(N == NULL){
        return NULL;
    }
    if(N->Key == Key){
        return N;
    }
    if(N->Key > Key){
        return searchNode(N->Left, Key);
    }
    return searchNode(N->Right, Key);
}
//costo amortizado o(log(n))
//peor caso O(n) cuando el arbol degenero en una lista