//min-Heap implementation in C
//es un arbol binario completo

//para el min-heap muestro el orden de las funciones
//Encolar/Insert O(log n)
//GetMIn O(1)
//RemoveMin O(log n)
//Build O(n)
//Meld O( n log n)
//DecreaseNode O(log n)

//Tournament Tree es un arbol binario completo

//tiene 2^k hojas = n y n-1 nodos internos
//respeta la propiedad de heap
//cada nodo interno tiene el minimo de sus hijos
//altura log n
// limita a usar solo 2^k elementos - > puedo tener un conjunto de arboles 2^k,2^k-1,2^k-2 ... ,2^0

//min Tournament Heap
// es un conjunto de arboles de torneo
//para el tournament heap muestro el orden de las funciones
//Encolar/Insert O(log n)
//GetMIn O(log n)
//RemoveMin O(log n)
//Build O(n)
//Meld O(log n1 + log n2)
//IdentMIn O(log n)
//Fracturar O(log n)
//coolesce O(log n)

//Lazy Tournament Heap
// es un conjunto de arboles de torneo pero que a diferencia del tournament heap no se actualiza en cada operacion
//para el lazy tournament heap muestro el orden de las funciones

//Encolar/Insert O(1)
//GetMIn O(n) costo amortizado O(log n)
//RemoveMin O(n) costo amortizado O(log n)
//Build O(n)
//Meld O(1)

//abdication Heap
// es un conjunto de arboles de torneo pero que a diferencia del tournament heap no se actualiza en cada operacion
// para el abdication heap muestro el orden de las funciones

//Encolar/Insert O(1)
//RemoveMin O(n)
//Meld O(1)
//Decreasenode O(1)