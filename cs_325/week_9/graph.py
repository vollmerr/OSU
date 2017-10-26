import math
# modified version of http://www.bogotobogo.com/python/python_graph_data_structures.php
class Vertex:
    def __init__(self, node, x, y):
        self.id = node
        self.x = int(x)
        self.y = int(y)
        self.adjacent = {}

    def __str__(self):
        return str(self.id) + ' adjacent: ' + str([x.id for x in self.adjacent])

    def add_neighbor(self, neighbor, weight=0):
        self.adjacent[neighbor] = weight

    def get_connections(self):
        return self.adjacent.keys()

    def get_id(self):
        return self.id

    def get_weight(self, neighbor):
        return self.adjacent[neighbor]

class Graph:
    def __init__(self):
        self.vert_dict = {}
        self.num_vertices = 0

    def __iter__(self):
        return iter(self.vert_dict.values())

    def add_vertex(self, node, x, y):
        self.num_vertices = self.num_vertices + 1
        new_vertex = Vertex(node, x, y)
        self.vert_dict[node] = new_vertex
        return new_vertex

    def get_vertex(self, n):
        if n in self.vert_dict:
            return self.vert_dict[n]
        else:
            return None

    def add_edge(self, frm, to, x=0, y=0):
        if frm not in self.vert_dict:
            self.add_vertex(frm, x, y)
        if to not in self.vert_dict:
            self.add_vertex(to, x, y)

        _x = (frm.x, to.x)
        _y = (frm.y, to.y)
        cost = math.sqrt(sum([(a - b) ** 2 for a, b in zip(_x, _y)]))

        self.vert_dict[frm].add_neighbor(self.vert_dict[to], cost)
        self.vert_dict[to].add_neighbor(self.vert_dict[frm], cost)

    def get_vertices(self):
        return self.vert_dict.keys()