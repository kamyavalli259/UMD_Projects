from __future__ import annotations
from typing import List

# The Graph object is a bit of a hybrid object.
# It is initialized with only an adjacency matrix self.adjmat but it also contains:
# self.edgelist: A list of edges of the form [x,y] with x<y, sorted by weight with ties broken by x and then by y.
# self.parent: A list which represents the disjoint set data structure corresponding to the edges of the graph.
# self.size: A list such that:
#     if i is the root of a component tree then self.size[i] = the size of the tree
#     else self.size[i] = 1

class Graph():
    def __init__(self,adjmat):
        # The next lines do the following:
        # - Assign self.adjmat from the parameter passed.
        # - Assign self.parent to represent a DSDS containing one component for each edge.
        # - Assign self.size to represent the size of each component.
        self.adjmat = adjmat
        self.parent = list(range(len(adjmat)))
        self.size = [1] * len(adjmat)
        # Fill in edgelist so it is a list of edges of the form [x,y] with x<y,
        # sorted by weight with ties broken by x and then by y.
        self.edgelist = []
        # Add the code here to fill in edgelist.
        templist = []
        #going through the rows
        for i in range(len(self.adjmat)):
            for j in range(0,i):
                if self.adjmat[j][i]!=0:
                    templist.append([j, i, self.adjmat[j][i]])
        templist.sort(key = lambda x: (x[2],x[0], x[1]))
        self.edgelist = [[x[0], x[1]] for x in templist] 

    # Dump various things from the graph.
    # DO NOT MODIFY!
    def dump_adjmat(self):
        for row in self.adjmat:
            print(row)
    def dump_edgelist(self):
        for row in self.edgelist:
            print(row)

    # Perform Kruskal's Algorithm.
    # Print the list of included edges in the order they are included.
    def kruskal(self):
        includededges = []
        # Iterate through self.edgelist.
        # For each edge determine if adding it would create a cycle.
        # If not, append the edge to includededges.
        # Update the disjoint set data structure accordingly.
        # Once we've obtained the correct number, bail.
        for edge in self.edgelist:
            if self.findrep(edge[0])!=self.findrep(edge[1]):
                includededges.append(edge)
                self.union(edge[0], edge[1])
            if len(includededges) == len(self.adjmat)-1:
                break
        # Print the included edges.
        for row in includededges:
            print(row)

    # Perform Kruskal's Algorithm.
    # Print the list of included edges in the order they are included.
    def unkruskal(self):
        excludededges = []
        # Iterate through self.edgelist.
        # For each edge determine if adding it would create a cycle.
        # If so, append the edge to includededges.
        # Update the disjoint set data structure accordingly.
        # Once we've obtained the correct number, bail.
        for edge in self.edgelist:
            if self.findrep(edge[0])==self.findrep(edge[1]):
                excludededges.append(edge)
            else:
                self.union(edge[0], edge[1])
        # Print the excluded edges.
        for row in excludededges:
            print(row)

    # Find the representative for the edge with index i.
    # Use path compression.
    def findrep(self,i) -> int:
        if self.parent[i] == i:
            return i
        else:
            self.parent[i] = self.findrep(self.parent[i])
            return self.parent[i]
        
       

    # Take the weighted union of the sets containing the edges with indices i and j.
    # Use the above findrep.
    def union(self,i,j):
        if self.findrep(i)!= self.findrep(j):
            if self.size[self.findrep(i)] < self.size[self.findrep(j)]:
                self.parent[self.findrep(i)] = self.findrep(j)
                self.size[self.findrep(j)]+= self.size[self.findrep(i)]
            else:
                self.parent[self.findrep(j)] = self.findrep(i)
                self.size[self.findrep(i)]+= self.size[self.findrep(j)]