#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#include <algorithm>
#include "dijkstras.h"

int main() {
    Graph G;
    G.numVertices = 4;
    G.resize(G.numVertices);
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 5));
    G[2].push_back(Edge(2, 3, 1));

    int source = 0;
    int destination = 3;
    vector<int> previous;

    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    
    vector<int> path = extract_shortest_path(distances, previous, destination);

    print_path(path, distances[destination]);

    return 0;
}