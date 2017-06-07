import sys
import math
import random


class City(object):
    def __init__(self, cityID, x, y):
        self.cityID = cityID
        self.x = x
        self.y = y
        self.adj = []

def dist(a, b):
    return int(round(math.hypot(b.x - a.x, b.y - a.y)))


def get_input(inFileName):
    with open(inFileName, "r") as in_f:
        inputData = [line.rstrip('\n') for line in in_f]
        cities = []
        for city in inputData:
            cityID, x, y = city.split(' ')
            cities.append(City(int(cityID), int(x), int(y)))
    return cities


def make_tour(inputCities, inFileName):
    # if (inFileName == "tsp_example_1.txt"):
    #     length, tour = nearNeighbor(inputCities)
    # if we need to run different algos on different problems...
    # if(inFileName == "tsp_example_2.txt" or inFileName ==
    # "competition_5.txt"):
    length, tour = christofides(inputCities)
    # elif(inFileName == "tsp_example_3.txt" || "competition_2.txt"): # length, tour = genetic(inputCities)
    # etc... #else print "Error: The input file name DOES NOT match expected
    # filenames."
    return length, tour


def descreseKey(G, k):
    for x in G:
        G[x].adj.remove(k)


def extractMin(Q, A):
    m = 9999
    i = 0
    while (i < len(Q)):
        k = []
        for j in Q:
            if j != i:
                k.append(A[Q[i]][j])
        # get min from adj list
        print("K:", k)
        n = min(k)
        if (n < m):
            print("k:::::::", k, n)
            m = n
        i+=1
    i-=1
    # remove from vertex list
    Q.remove(Q[i])
    return i


def minimum_spanning_tree(G, A, V):
    """ Finds minimum spanning tree using Prims

    """
    print("in minimum_spanning_tree")
    k = [9999]*len(G)
    c = [None]*len(G)
    p = [None]*len(G)
    k[0] = 0
    Q = [_ for _ in V]
    while len(Q):
        u = extractMin(Q, A)
        print("EXTRACTED NUM: ", u)
        print("Q:", Q)
        for v in V:
            if (c[u] == None and dist(G[u], G[v]) < k[v]):
                k[v] = dist(G[u], G[v])
                # descreseKey(Q, k[v])
                p[v] = u
            c[u] = 1
    print(p)


def find_odd_vertices(M):
    print("finding odd verticies")
    pass


def find_prefect_matching(G):
    print("finding perfect macthing")
    pass


def create_multigraph(M, T):
    print("creating multigraph")
    return


def create_euler_circuit(G):
    print("finding euler tour")
    tour = []
    current_vertex = G[0][0]
    tour.append(current_vertex)

    while len(graph) > 0:
        print(G, current_vertex)
        for edge in G:
            if current_vertex in edge:
                if edge[0] == current_vertex:
                    current_vertex = edge[1]
                else:
                    current_vertex = edge[0]

                G.remove(edge)
                tour.append(current_vertex)
                break
    return tour


def shortcut_euler_circuit(G):
    pass


def build_lists(G):
    k=0
    A = []
    V = []
    for i in G:
        A.append([])
        V.append(k)
        for j in G:
            A[k].append(dist(i,j))
        k+=1
    return A, V


def christofides(G):
    A, V = build_lists(G)
    print("A:::", A)
    print("V:::", V)
    # 1) Create a minimum spanning tree T of G.
    T = minimum_spanning_tree(G, A, V)
    # # 2) Let O be the set of vertices with odd degree in T. By the handshaking lemma, O has an even number of vertices.
    O = find_odd_vertices(T)
    # # 3) Find a minimum-weight perfect matching M in the induced subgraph given by the vertices from O.
    # M = find_prefect_matching(O)
    # # 4) Combine the edges of M and T to form a connected multigraph H in which each vertex has even degree.
    # H = create_multigraph(M, T)
    # # 5) Form an Eulerian circuit in H.
    # E = create_euler_circuit(H)
    # # 6) Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
    # C = shortcut_euler_circuit(E)
    return [], []


# def main():
#     cities = Graph()
#     # get input file name
#     if len(sys.argv) > 1:
#         fn = sys.argv[1]
#     else:
#         fn = raw_input("File name:")
#     # create graph based off file entered
#     with open(fn, "r") as f:
#         for line in f:
#             city, x, y = line.split()
#             cities.add_vertex(city, x, y)
#             print("adding: ", city,x, y)
#     # connect all vertex
#     for t in cities:
#         for f in cities:
#             print("addingedge: ", t.get_id(), f.get_id())
#             cities.add_edge(f, t)
#
#     christofides(cities)


def main(inFileName):
    print inFileName
    inputCities = get_input(inFileName)
    # for city in inputCities:
    #   print city.cityID, city.x, city.y
    length, tour = make_tour(inputCities, inFileName)
    # make_output(length, tour, inFileName)

if __name__ == '__main__':
    main(sys.argv[1])
