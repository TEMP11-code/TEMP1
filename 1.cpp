#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <stack>

using namespace std;

class Graph {
public:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    Graph(int V) : V(V), adj(V) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v); // Since the graph is undirected
    }

    // Parallel BFS using OpenMP
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);
        cout << start << " ";

        while (!q.empty()) {
            int level_size = q.size();
            vector<int> next_level;

            #pragma omp parallel for shared(q, visited, adj, next_level)
            for (int i = 0; i < level_size; ++i) {
                int v;
                #pragma omp critical
                {
                    if (!q.empty()) {
                        v = q.front();
                        q.pop();
                    } else {
                        continue; // Queue might become empty due to other threads
                    }
                }

                for (int neighbor : adj[v]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        #pragma omp critical
                        next_level.push_back(neighbor);
                    }
                }
            }

            #pragma omp critical
            {
                for (int node : next_level) {
                    cout << node << " ";
                    q.push(node);
                }
            }
        }
        cout << endl;
    }

    // Parallel DFS using OpenMP (Conceptual - True Parallel DFS is Complex)
    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;

        #pragma omp parallel
        {
            #pragma omp single
            {
                s.push(start);
                visited[start] = true;
                cout << start << " ";
            }

            while (!s.empty()) {
                int v = -1;
                #pragma omp critical
                {
                    if (!s.empty()) {
                        v = s.top();
                        s.pop();
                    }
                }

                if (v != -1) {
                    #pragma omp parallel for shared(visited, adj, s)
                    for (int i = 0; i < adj[v].size(); ++i) {
                        int u = adj[v][i];
                        if (!visited[u]) {
                            visited[u] = true;
                            #pragma omp critical
                            {
                                s.push(u);
                                cout << u << " ";
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    cout << "Parallel BFS starting from vertex 0: " << endl;
    g.parallelBFS(0);

    cout << "\nParallel DFS starting from vertex 0 (Conceptual): " << endl;
    g.parallelDFS(0);

    return 0;
}