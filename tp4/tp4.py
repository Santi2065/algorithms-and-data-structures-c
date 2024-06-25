from graph import Graph
import time
import random
from collections import deque
import matplotlib.pyplot as plt

page_graph = Graph()

with open('web-Google.txt', 'r') as file:
    for l in file:
        if "# FromNodeId	ToNodeId" in l:
            break
    for l in file:
        if not l:
            break
        edge = tuple(int(v.replace("\n", "").replace("\t", "")) for v in l.split("\t"))
        for v in edge:
            if not page_graph.vertex_exists(v):
                page_graph.add_vertex(str(v))
        page_graph.add_edge(str(edge[0]), str(edge[1]))


#1) cantidad de nodos de la parte conexa mas grande y cantidad de componentes conexas

print("1)")

#transforma grafo dirigido en no dirigido
def make_undirected(graph):
    new_graph = Graph()
    for v in graph._graph.keys():
        for n in graph.get_neighbors(v):
            new_graph.add_vertex(v)
            new_graph.add_vertex(n)
            new_graph.add_edge(v, n)
            new_graph.add_edge(n, v)
    return new_graph

def dfs(graph, start, visited=set()):
    stack = [start]
    size = 0
    while stack:
        v = stack.pop()
        if v not in visited:
            visited.add(v)
            size += 1
            for n in graph.get_neighbors(v):
                stack.append(n)
    return size

def connected_components(graph):
    visited = set()
    components = []
    max_component = 0
    for v in graph._graph.keys():
        if v not in visited:
            size = dfs(graph, v, visited)
            components.append(size)
            if size > max_component:
                max_component = size
    return components, max_component

grafo = make_undirected(page_graph)
components, max_component = connected_components(grafo)
print(f"La cantidad de nodos de la componente conexa mas grande es {max_component} y la cantidad de componentes conexas es {len(components)}")

#2) Calcular el camino mínimo de todos con todos. ¿En cuanto tiempo lo puede hacer? ¿Qué orden tiene el algoritmo? En caso de no alcanzarle el tiempo, estime cuanto tiempo le llevaría.
print("2)")

def bfs(graph, start):
    visited = set()
    visited.add(start)
    queue = deque([(start, 0)])
    distances = {start: 0}
    while queue:
        vertex, distance = queue.popleft()
        for neighbor in graph.get_neighbors(vertex):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append((neighbor, distance + 1))
                distances[neighbor] = distance + 1
    return distances

def bfs_timer():
    start_time = time.time()
    bfs(page_graph, '1')
    end_time = time.time()
    elapsed_time = end_time - start_time
    return elapsed_time

segundos = bfs_timer()*len(page_graph._graph.keys())
minutos = segundos/60
horas = minutos/60

print(f"El camino minimo tomaria {horas} horas")

#3) Calcular la cantidad de triángulos que tiene el grafo.

print("3)")

def triangles(grafo):
    triangulos = 0
    for node in grafo._graph.keys():
        for vecino in grafo.get_neighbors(node):
            for vecino_de_vecino in grafo.get_neighbors(vecino):
                if node in grafo.get_neighbors(vecino_de_vecino):
                    triangulos += 1
    return triangulos

triangulos = triangles(page_graph)
print(f"El grafo tiene {triangulos} triangulos")

#4)Utilice el punto 2 para calcular el diámetro del grafo.

print("4)")

def estimate_diameter(graph, num_samples=100, num_repeats=6):
    diametros = []
    
    for _ in range(num_repeats):
        vertices = random.sample(list(graph._graph.keys()), num_samples)
        diametro = 0
        
        for inicio in vertices:
            visitados = set()
            cola = deque([(inicio, 0)]) 
            visitados.add(inicio)
            profundidad_maxima = 0 

            while cola:
                vertice_actual, profundidad = cola.popleft() 

                profundidad_maxima = max(profundidad_maxima, profundidad)

                vecinos = graph.get_neighbors(vertice_actual)

                for vecino in vecinos:
                    if vecino not in visitados:
                        visitados.add(vecino)
                        cola.append((vecino, profundidad + 1))

            diametro = max(diametro, profundidad_maxima)

        diametros.append(diametro)

    return max(diametros)

diametro = estimate_diameter(grafo)
print(f"El diametro del grafo es {diametro}")

#5) Calcule el PageRank de los vértices del grafo

print("5)")

def pagerank(graph, random_walks):
    pr = {v: 0 for v in graph._graph.keys()}
    for _ in range(random_walks):
        v = random.choice(list(graph._graph.keys()))
        for _ in range(100):
            pr[v] += 1
            if random.random() < 0.15:
                break
            v = random.choice(list(graph._graph.keys()))
    return pr

def pr_top_n(pr, n):
    return sorted(pr.items(), key=lambda x: x[1], reverse=True)[:n]

pr = pagerank(page_graph, 100)
top_n = pr_top_n(pr, 10)
print(f"el top 10 de vertices son:{top_n}")

#6) Calcular el tamaño del ciclo más largo en el grafo.

print("6)")

def encontrar_ciclo_mas_largo(grafo, num_muestras=100):
    num_muestras = min(num_muestras, len(grafo._graph))
    vertices_muestra = random.sample(list(grafo._graph), num_muestras)
    longitud_maxima_ciclo = 0

    for inicio in vertices_muestra:
        cola, visitados = deque([(inicio, 1)]), {inicio}
        while cola:
            vertice_actual, profundidad = cola.pop()
            for vecino in grafo.get_neighbors(vertice_actual):
                if vecino == inicio:
                    longitud_maxima_ciclo = max(longitud_maxima_ciclo, profundidad + 1)
                elif vecino not in visitados:
                    visitados.add(vecino)
                    cola.append((vecino, profundidad + 1))

    return longitud_maxima_ciclo

ciclo = encontrar_ciclo_mas_largo(page_graph)
print(f"El tamaño del ciclo mas largo es {ciclo}")


#puntos extra

#1) funcion recursiva para encontrar poligonos de n lados

def k_polygons(grafo, n):
    poligons = [0]
    for node in grafo._graph.keys():
        k_polygons_rec(grafo, grafo.get_neighbors(node), node, n - 1, poligons)
    return poligons[0]

def k_polygons_rec(grafo, vecinos,nodo, k, poligons):
    if k > 0:
        for vecino in vecinos:
            if vecino == nodo: continue
            k_polygons_rec(grafo, grafo.get_neighbors(vecino), nodo, k - 1, poligons)
    else:
        if nodo in vecinos:
            poligons[0] += 1
    return

def estimate_k_polygons(grafo, n, num_samples=0):
    poligons = [0]
    vertices = random.sample(list(grafo._graph.keys()), num_samples)
    for nodo in vertices:
        k_polygons_rec(grafo, grafo.get_neighbors(nodo), nodo, n - 1, poligons)
    return poligons[0]

def plot_polygons(grafo):
    poligonos = []
    for i in range(3, 7):
        if i < 4:
            poligonos.append(k_polygons(grafo, i))
        else:
            poligonos.append(estimate_k_polygons(grafo, i, 4000))
    plot_graph(poligonos)
    return poligonos

def plot_graph(poligonos):
    plt.plot(range(3, 7), poligonos)
    plt.xlabel('Cantidad de lados')
    plt.ylabel('Cantidad de poligonos')
    plt.show()

print(f"hay {plot_polygons(page_graph)} poligonos en el grafo")

#2) Calcule el coeficiente de clustering del grafo

def contar_aristas_entre_vecinos(grafo, nodo):
    vecinos = grafo.get_neighbors(nodo)
    aristas = 0
    for vecino in vecinos:
        pointing = grafo.get_neighbors(vecino)
        aristas += len(set(vecinos) & set(pointing))
    return aristas

def coeficiente_clustering_local(grafo, nodo):
    vecinos = grafo.get_neighbors(nodo)
    k = len(vecinos)
    if k < 2:
        return 0.0
    aristas = contar_aristas_entre_vecinos(grafo, nodo)
    coeficiente = aristas / (k * (k - 1))
    return coeficiente

def coeficiente_clustering_global(grafo):
    suma_clustering_local = 0.0
    nodos = list(grafo._graph.keys())
    for nodo in nodos:
        suma_clustering_local += coeficiente_clustering_local(grafo, nodo)
    coeficiente_global = suma_clustering_local / len(nodos)
    return coeficiente_global


print("Coeficiente de clustering global:", coeficiente_clustering_global(page_graph))


#3) ¿cuál es el vértice con más betweenness centrality?

def estimate_betweenness_centrality_random_sampling(grafo, num_samples= 4000):
    betweenness = {}
    vertices = random.sample(list(grafo._graph.keys()), num_samples)
    for nodo in vertices:
        distancias = bfs(grafo, nodo)
        for v in distancias:
            if v != nodo:
                if v not in betweenness:
                    betweenness[v] = 0
                betweenness[v] += 1
    max_node = max(betweenness, key=betweenness.get)
    return (max_node, betweenness[max_node])

max_node,max_value = estimate_betweenness_centrality_random_sampling(page_graph)
print("Vértice con más betweenness centrality:", max_node, "con valor:", max_value)