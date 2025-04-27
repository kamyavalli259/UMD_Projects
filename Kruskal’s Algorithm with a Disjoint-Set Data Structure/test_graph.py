# This is provided to you so that you can test your bst.py file with a particular tracefile.

import argparse
import graph

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('-tf', '--tracefile')
    args = parser.parse_args()
    tracefile = args.tracefile

    A = []
    with open(tracefile) as file:
        lines = [line.rstrip() for line in file]
    n = int(lines[0])
    for r in range(0,n):
        row = lines[r+1].split(',')
        row = [int(x) for x in row]
        A.append(row)
    t = graph.Graph(A)
    for action in lines[n+1:]:
        if action == 'kruskal':
            t.kruskal()
        if action == 'unkruskal':
            t.unkruskal()
        if action == 'dump_adjmat':
            t.dump_adjmat()
        if action == 'dump_edgelist':
            t.dump_edgelist()