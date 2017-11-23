import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import sys

# read graph
lines_ = [line.rstrip('\n') for line in open('graph.txt')]

nodes = [] 
for i in range(int(lines_[0])):
	nodes.append(i)

edges = []
for i in range(len(lines_)-1):
	one_edge_string = lines_[i+1].split(',')
	_edge1 = []
	_edge1.append(int(one_edge_string[1]))
	_edge1.append(int(one_edge_string[0]))
	_edge2 = []
	_edge2.append(int(one_edge_string[0]))
	_edge2.append(int(one_edge_string[1]))
	edges.append(_edge1)
	edges.append(_edge2)
"""
 one_edge = [start_node,end_node]
 start_node = int(one_edge_string[1])
 end_node = int(one_edge_string[0])
"""

# Create Directed Graph
G=nx.DiGraph()

# Add a list of nodes:
G.add_nodes_from(nodes)

# Add a list of edges:
G.add_edges_from(edges)

#Return a list of cycles described as a list o nodes
cycles  = list(nx.simple_cycles(G))

delete = []
for i in range(len(cycles)):
	if len(cycles[i]) <= 2:
		delete.append(i)

for j in range(len(delete),0,-1):
	cycles.pop(delete[j-1])

print(cycles)

file = open('cycle.txt','w')

for i in range(len(cycles)):
	str1 = ' '.join(str(e) for e in cycles[i])
	file.write(str1+'\n')


file.close()