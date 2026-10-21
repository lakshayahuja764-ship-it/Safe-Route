// Graph.cpp
#include "Graph.h"
#include "Validation.h"
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

Graph::Graph()
{
}

bool Graph::addLocation(const string& name)
{
    string key = toLowerCase(name);
    if (indexOf.find(key) != indexOf.end()) return false;   // already present

    indexOf[key] = names.size();
    names.push_back(name);
    adj.push_back(vector<pair<int, int> >());               // empty neighbour list
    return true;
}

// Road is two-way, so we add the edge in both lists. Distance must be positive.
bool Graph::addRoad(const string& a, const string& b, int distance)
{
    int u = findLocation(a);
    int v = findLocation(b);
    if (u == -1 || v == -1 || distance <= 0) return false;

    adj[u].push_back(make_pair(v, distance));
    adj[v].push_back(make_pair(u, distance));
    return true;
}

int Graph::findLocation(const string& name) const
{
    unordered_map<string, int>::const_iterator it = indexOf.find(toLowerCase(name));
    if (it == indexOf.end()) return -1;
    return it->second;
}

string Graph::getName(int index) const
{
    return names[index];
}

int Graph::getLocationCount() const
{
    return names.size();
}

void Graph::displayLocations() const
{
    cout << "Locations: ";
    for (size_t i = 0; i < names.size(); i++)
    {
        cout << names[i];
        if (i + 1 < names.size()) cout << ", ";
    }
    cout << endl;
}

void Graph::displayGraph() const
{
    cout << "Location -> Neighbour (distance in km)\n";
    cout << "---------------------------------------\n";
    for (size_t u = 0; u < names.size(); u++)
    {
        cout << names[u] << " -> ";
        for (size_t j = 0; j < adj[u].size(); j++)
        {
            cout << names[adj[u][j].first] << "(" << adj[u][j].second << ")";
            if (j + 1 < adj[u].size()) cout << ", ";
        }
        cout << endl;
    }
}

// BFS = Breadth First Search.
// Uses a QUEUE (first in, first out), so all locations 1 road away are visited first,
// then 2 roads away, and so on (level by level). Time: O(V + E).
void Graph::bfs(int start, vector<int>& order, vector<int>& level) const
{
    int n = names.size();
    vector<bool> visited(n, false);
    level.assign(n, -1);
    order.clear();

    queue<int> q;
    visited[start] = true;
    level[start] = 0;
    q.push(start);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (size_t j = 0; j < adj[u].size(); j++)
        {
            int v = adj[u][j].first;
            if (!visited[v])
            {
                visited[v] = true;
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
}

// Dijkstra's algorithm = shortest path when all distances are NON-NEGATIVE.
// Idea:
//   1. dist[start] = 0, every other distance = infinity.
//   2. Always take the unvisited location with the smallest known distance (min-heap).
//   3. "Relax" its roads: if going through it gives a shorter way to a neighbour, update it.
//   4. parent[] remembers where we came from, so the path can be rebuilt at the end.
// Time: O((V + E) log V) with a priority queue.
bool Graph::dijkstra(int source, int destination, int& totalDistance, vector<int>& path) const
{
    const int INF = 1000000000;
    int n = names.size();
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);

    // min-heap of (distance, location): smallest distance comes out first
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    dist[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty())
    {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;   // old, longer entry - ignore

        for (size_t j = 0; j < adj[u].size(); j++)
        {
            int v = adj[u][j].first;
            int w = adj[u][j].second;
            if (dist[u] + w < dist[v])       // found a shorter way to v
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    if (dist[destination] == INF) return false;   // not connected

    // Rebuild path by walking backwards using parent[], then reverse it.
    path.clear();
    for (int at = destination; at != -1; at = parent[at])
        path.push_back(at);
    reverse(path.begin(), path.end());

    totalDistance = dist[destination];
    return true;
}
