#include <iostream>
#include <vector>
#include <climits>
using namespace std;
const int INF = INT_MAX;
int findMinKey(const vector<int>& key, const vector<bool>& mstSet) {
    int minKey = INF, minIndex = -1;
    for (int i = 0; i < key.size(); ++i) {
        if (!mstSet[i] && key[i] < minKey) {
            minKey = key[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void prim(const vector<vector<int>>& graph, int numNodes) {
    vector<int> parent(numNodes, -1);
    vector<int> key(numNodes, INF);
    vector<bool> mstSet(numNodes, false);
    key[0] = 0; // 选择第一个节点作为起始节点
    for (int count = 0; count < numNodes - 1; ++count) {
        int u = findMinKey(key, mstSet);
        mstSet[u] = true;
        for (int v = 0; v < numNodes; ++v) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }
    // 打印最小生成树的边
    cout << "Edges of the Minimum Spanning Tree:\n";
    for (int i = 1; i < numNodes; ++i) {
        cout << "Edge: " << parent[i] << " - " << i << " Weight: " << graph[i][parent[i]] << endl;
    }
}

int main() {
    vector<vector<int>> graph = {
            {0, 2, INF, 6, INF},
            {2, 0, 3, 8, 5},
            {INF, 3, 0, INF, 7},
            {6, 8, INF, 0, 9},
            {INF, 5, 7, 9, 0}
    };
    int numNodes = graph.size();
    prim(graph, numNodes);
    return 0;
}
//Edges of the Minimum Spanning Tree:
//Edge: 0 - 1 Weight: 2
//Edge: 1 - 2 Weight: 3
//Edge: 0 - 3 Weight: 6
//Edge: 1 - 4 Weight: 5
