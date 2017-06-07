import sys
import math
from scipy.sparse import csr_matrix
from scipy.sparse.csgraph import minimum_spanning_tree


class City(object):
	def __init__(self, cityID, x, y):
		self.cityID = int(cityID)
		self.x = int(x)
		self.y = int(y)


def dist(a, b):
	return int(math.hypot(b.x-a.x, b.y-a.y))


def find_odd_vertices(M):
    """ Finds verticies with odd degree
        @param M        - distance matrix in csr_matrix format
    """
    M = M.toarray().astype(int)
    odd_verticies = [0 for _ in range(M.shape[0])]
    # look through all rows
    for i in xrange(len(M)):
        # look at all cols
        for j in xrange(len(M[i])):
            # found edge, increment both verticies attched to it
            if (M[i][j]):
                odd_verticies[i] += 1
                odd_verticies[j] += 1
    # return array with odd verticies = True
    return [x % 2 == 1 for x in odd_verticies]


def build_subgraph(G, S):
    """ Builds a subgraph based off verticies list
        @param G        - Parent Graph
        @param S        - Sub Graph
    """
    _G = [x for x in G if S[x.cityID]]
    return build_distance(_G), build_distance_matrix(_G)


def build_distance(G):
    """ Returns distance list
        @param G        - List/graph of city/vertex objects
    """
    return {v.cityID: {u.cityID: dist(u,v) for u in G} for v in G}


def build_distance_matrix(G):
    """ Returns distance matrix in compressed sparse representaiton format
        (https://docs.scipy.org/doc/scipy-0.19.0/reference/generated/scipy.sparse.csr_matrix.html)
        @param G        - List/graph of city/vertex objects
    """
    return csr_matrix([[dist(u,v) for u in G] for v in G])


def christofides(G):
    """ Computes travelling salesman problem.
        (https://en.wikipedia.org/wiki/Christofides_algorithm)
        @param G        - List/graph of city/vertex objects
    """
    distance = build_distance(G)
    print("distance:", distance)
    distance_matrix = build_distance_matrix(G)
    print("distance_matrix:", distance_matrix)
    # 1) Create a minimum spanning tree T of G.
    # (https://docs.scipy.org/doc/scipy-0.14.0/reference/generated/scipy.sparse.csgraph.minimum_spanning_tree.html)
    T = minimum_spanning_tree(distance_matrix)
    # # 2) Let O be the set of vertices with odd degree in T. By the handshaking lemma, O has an even number of vertices.
    O = find_odd_vertices(T)
    # # 3) Find a minimum-weight perfect matching M in the induced subgraph given by the vertices from O.
    sub_distance, sub_distance_matrix = build_subgraph(G, O)
    print("odd verticies", O)
    print('sub_distance', sub_distance)
    print('sub_distance_matrix', sub_distance_matrix)
    # M = find_prefect_matching(O) #TODO: Construct a minimum-weight perfect matching M in this subgraph
    # # 4) Combine the edges of M and T to form a connected multigraph H in which each vertex has even degree.
    # H = create_multigraph(M, T) #TODO: Unite matching and spanning tree T union M to form an Eulerian multigraph
    # # 5) Form an Eulerian circuit in H.
    # E = create_euler_circuit(H) #TODO: Calculate Euler tour
    # # 6) Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
    # C = shortcut_euler_circuit(E) #TODO: Remove repeated vertices, giving the algorithm's output


def main():
    cities = []
    # get input file name
    fn = sys.argv[1] if len(sys.argv) > 1 else raw_input("File name:")
    # create graph based off file entered
    with open(fn, "r") as f:
        for line in f:
            cities.append(City(*line.split()))

    christofides(cities)


if __name__ == '__main__':
    main()
