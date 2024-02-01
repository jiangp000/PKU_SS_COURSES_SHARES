#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
class FordFulkerson {
public:
    FordFulkerson(int vertices) : V(vertices), residualGraph(vertices, vector<int>(vertices, 0)), parent(vertices, -1) {}
    void addEdge(int u, int v, int capacity) {
        residualGraph[u][v] = capacity;
    }
    int maxFlow(int source, int sink) {
        int maxFlow = 0;
        while (augmentPath(source, sink)) {
            int pathFlow = INT_MAX;
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, residualGraph[u][v]);
            }
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= pathFlow;
                residualGraph[v][u] += pathFlow;
            }
            maxFlow += pathFlow;
        }
        return maxFlow;
    }

private:
    int V; // Number of vertices
    vector<vector<int>> residualGraph;
    vector<int> parent;
    bool augmentPath(int source, int sink) {
        fill(parent.begin(), parent.end(), -1);
        vector<bool> visited(V, false);
        visited[source] = true;
        parent[source] = source;
        vector<int> stack;
        stack.push_back(source);
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            for (int v = 0; v < V; ++v) {
                if (!visited[v] && residualGraph[u][v] > 0) {
                    parent[v] = u;
                    visited[v] = true;
                    stack.push_back(v);
                    if (v == sink) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
};

int main() {
    int vertices = 6;
    FordFulkerson fordFulkerson(vertices);
    fordFulkerson.addEdge(0, 1, 16);
    fordFulkerson.addEdge(0, 2, 13);
    fordFulkerson.addEdge(1, 2, 10);
    fordFulkerson.addEdge(1, 3, 12);
    fordFulkerson.addEdge(2, 1, 4);
    fordFulkerson.addEdge(2, 4, 14);
    fordFulkerson.addEdge(3, 2, 9);
    fordFulkerson.addEdge(3, 5, 20);
    fordFulkerson.addEdge(4, 3, 7);
    fordFulkerson.addEdge(4, 5, 4);
    int source = 0;
    int sink = 5;
    int maxFlow = fordFulkerson.maxFlow(source, sink);
    cout << "Maximum Flow: " << maxFlow << endl;
    return 0;
}
