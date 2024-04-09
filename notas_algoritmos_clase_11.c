//un diccionario tiene un key y un value y se pueden armar con:
//Arreglos/Vectores -> alg. sort, alg. Busqueda
//Listas ->Skip List
//Arboles -> BST

//hay arboles binarios(Grado 2) -> Heaps(min/max)(completo izq + nodo < hijos) o BST
//BST -> nodo izq < nodo < nodo der
//AVL - Tree (1962 - Adelson-Velsky y Landis) -> BST balanceado - diferencia de sub-arbol derecho e izquierdo es <= h = 1
//balancea utilizando funciones LeftRotate y RightRotate
//Instert(k,v) -> O(log n)
//Delete(k) -> O(log n)
//Search(k) -> O(log n)

//red-black tree -> BST balanceado - nodo

//B-Tree -> se define con un grado t , todos los nodos excepto la raiz tienen entre t-1 y 2t-1 claves
//la raiz puede tener una clave
//todos los nodos tienen hasta claves+1 hijos
//todos los nodos hoja estan al mismo nivel