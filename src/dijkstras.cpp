#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#include <algorithm>
#include "dijkstras.h"

using namespace std;

vector<int> dijkstra_shortest_path(const Graph & G, int source, vector<int> & previous) {
    int n = G.numVertices;

    vector<int> dist(n, INF); // initialize infinity for all vectices
    previous.assign(n, -1);
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue; // skips rest of current iteration

        for (const Edge & e: G[u]) { // for each edge of current vertex u
            int v = e.dst; // for neighboring vertex
            int weight = e.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                previous[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;  

    int at = destination;
    if (distances[destination] == INF) {
        return path;  
    }
    
    while (at != -1) {
        path.push_back(at);
        at = previous[at];
    }
    
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& v, int total) {
    for (int vertex : v) {
        cout << vertex << " ";
    }
    cout << "\nTotal cost is " << total << endl;
}