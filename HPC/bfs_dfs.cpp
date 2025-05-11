#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

vector<vector<int>> graph;
int nodes;

// Parallel BFS
void parallelBFS(int start) {
    vector<bool> visited(nodes, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "\nParallel BFS Traversal:\n";

    while (!q.empty()) {
        int q_size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < q_size; i++) {
            int current;

            // Ensure thread-safe access to queue
            #pragma omp critical
            {
                if (!q.empty()) {
                    current = q.front();
                    q.pop();
                    cout << "Visited: " << current << endl;
                }
            }

            // Push unvisited neighbors
            #pragma omp critical
            for (int neighbor : graph[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }
}

// Parallel DFS (fixed version)
void parallelDFS(int node, vector<bool> &visited) {
    bool alreadyVisited;

    // Check and mark visited in critical section
    #pragma omp critical
    {
        alreadyVisited = visited[node];
        if (!alreadyVisited) {
            visited[node] = true;
            cout << "Visited: " << node << endl;
        }
    }

    if (alreadyVisited) return;

    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); i++) {
        int neighbor = graph[node][i];
        if (!visited[neighbor]) {
            parallelDFS(neighbor, visited);
        }
    }
}

int main() {
    int edges, u, v, start;

    cout << "Enter number of nodes: ";
    cin >> nodes;
    graph.resize(nodes);

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "Enter " << edges << " edges (u v):\n";
    for (int i = 0; i < edges; i++) {
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected
    }

    cout << "Enter starting node: ";
    cin >> start;

    parallelBFS(start);

    cout << "\nParallel DFS Traversal:\n";
    vector<bool> visited(nodes, false);
    parallelDFS(start, visited);

    return 0;
}
