
//ej 2
// si tengo un vector de enteros casi todo ordenado excepto por un elemento, me conviene usar
// insertion sort, ya que es O(n) en el mejor caso, y O(n^2) en el peor caso, pero como casi todo
//bien

//un vector de enteros de 16bits completamente desordenados que contiene 4 millones de numeros
//me conviene usar quick sort, ya que es O(n*log(n)) en el mejor caso, y O(n^2) en el peor caso
//mal -> count sort, ya que es O(n) en el mejor caso, y O(n) en el peor caso

//un vector de 1000 strings de 128 caracteres cada uno
//sin considerar radix sort
//me conviene usar quick sort, ya que es O(n*log(n)) en el mejor caso, y O(n^2) en el peor caso
//bien


//ej 3

//voy haciendo operaciones a un arbol vacio, ir haciendo el dibujo de como queda paso a paso
//insert(29)
//(dibujo)
//(29)

//insert(16)
//(dibujo)
//(29)
// /
//(16)

//insert(10)
//(dibujo)  
//(29)
// /
//(16)
//  /
//(10)

//ej 4

//Heapdown (mejor caso O(1) depende) (caso amortizado o(log(n)))
//insert sort (mejor caso O(n))
//remove max en tournament heap (mejor caso O(log(n)))
//search en arbol binario de busqueda (mejor caso O(log(n)))

//ej 5

//ej 11

//push(elem) agrega el numero x en la pila pero solo si es mas grande que el tope actual, si no hace pops hasta que el top sea mas chico que x

//el orden de pop() o(1)
//el orden amortizado de push(elem) o()