// Graph.h - locations (vertices) and roads (edges with distance as weight)
// Stored as an ADJACENCY LIST: adj[i] = list of (neighbour, distance) for location i.
#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Graph
{
private:
    vector<string> names;                       // index -> location name
    unordered_map<string, int> indexOf;         // lowercase name -> index (hash table, O(1) lookup)
    vector<vector<pair<int, int> > > adj;       // adj[u] holds pairs (v, distance)

public:
    Graph();

    bool addLocation(const string& name);                        // false if it already exists
    bool addRoad(const string& a, const string& b, int distance);// two-way road

    int findLocation(const string& name) const;                  // -1 if not found
    string getName(int index) const;
    int getLocationCount() const;

    void displayLocations() const;
    void displayGraph() const;

    // BFS: fills 'order' (visit order) and 'level' (level[node] = number of roads from start)
    void bfs(int start, vector<int>& order, vector<int>& level) const;

    // Dijkstra: returns false if no path. Otherwise fills totalDistance and path (list of nodes).
    bool dijkstra(int source, int destination, int& totalDistance, vector<int>& path) const;
};

#endif
