#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

class Dinic {
public:
    Dinic(int vertices) : V(vertices), level(vertices, -1), adjMatrix(vertices, vector<int>(vertices, 0)) {}
    void addEdge(int u, int v, int capacity) {
        adjMatrix[u][v] = capacity;
    }

    int maxFlow(int source, int sink) {
        int totalFlow = 0;
        while (bfs(source, sink)) {
            vector<int> start(V, 0);
            int pathFlow;
            while ((pathFlow = dfs(source, sink, INT_MAX, start)) != 0) {
                totalFlow += pathFlow;
            }
        }
        return totalFlow;
    }

private:
    int V;
    vector<int> level;
    vector<vector<int>> adjMatrix;
    bool bfs(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        level[source] = 0;
        queue<int> q;
        q.push(source);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 0; v < V; ++v) {
                if (level[v] < 0 && adjMatrix[u][v] > 0) {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
        return level[sink] >= 0;
    }

    // Depth-first search for augmenting paths
    int dfs(int u, int sink, int flow, vector<int>& start) {
        if (u == sink) {
            return flow;
        }
        for (int v = start[u]; v < V; ++v) {
            if (level[v] == level[u] + 1 && adjMatrix[u][v] > 0) {
                int pathFlow = dfs(v, sink, min(flow, adjMatrix[u][v]), start);
                if (pathFlow > 0) {
                    adjMatrix[u][v] -= pathFlow;
                    adjMatrix[v][u] += pathFlow;
                    return pathFlow;
                }
            }
            ++start[u];
        }
        return 0;
    }
};

int main() {
    int vertices = 6;
    Dinic dinic(vertices);
    dinic.addEdge(0, 1, 16);
    dinic.addEdge(0, 2, 13);
    dinic.addEdge(1, 2, 10);
    dinic.addEdge(1, 3, 12);
    dinic.addEdge(2, 1, 4);
    dinic.addEdge(2, 4, 14);
    dinic.addEdge(3, 2, 9);
    dinic.addEdge(3, 5, 20);
    dinic.addEdge(4, 3, 7);
    dinic.addEdge(4, 5, 4);
    int source = 0;
    int sink = 5;
    int maxFlow = dinic.maxFlow(source, sink);
    cout << "Maximum Flow: " << maxFlow << endl;
    return 0;
}
Maximum Flow: 23