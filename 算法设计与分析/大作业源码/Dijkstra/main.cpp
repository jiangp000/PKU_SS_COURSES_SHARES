#include <iostream>
#include <vector>
using namespace std;
const int INF = INT_MAX; // 代表无穷大
void dijkstra(const vector<vector<int>>& graph, int startNode, int numNodes) {
    vector<int> distance(numNodes, INF);
    vector<bool> visited(numNodes, false);
    distance[startNode] = 0;
    for (int i = 0; i < numNodes - 1; ++i) {
        int minDistance = INF;
        int minIndex = -1;
        for (int j = 0; j < numNodes; ++j) {
            if (!visited[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                minIndex = j;
            }
        }
        visited[minIndex] = true;
        // 更新通过当前节点的路径，更新到达其它节点的最短距离
        for (int j = 0; j < numNodes; ++j) {
            if (!visited[j] && graph[minIndex][j] != INF && distance[minIndex] != INF &&
                distance[minIndex] + graph[minIndex][j] < distance[j]) {
                distance[j] = distance[minIndex] + graph[minIndex][j];
            }
        }
    }
    cout << "Shortest distances from node " << startNode << ":\n";
    for (int i = 0; i < numNodes; ++i) {
        cout << "To node " << i << ": " << distance[i] << endl;
    }
}

int main() {
    // 代表无连接的节点使用INF
    vector<vector<int>> graph = {
            {0, 2, INF, 1, INF},
            {2, 0, 4, INF, 5},
            {INF, 4, 0, 3, INF},
            {1, INF, 3, 0, 6},
            {INF, 5, INF, 6, 0}
    };

    int startNode = 2; // 起始节点
    int numNodes = graph.size();
    dijkstra(graph, startNode, numNodes);
    return 0;
}

//Shortest distances from node 2:
//To node 0: 4
//To node 1: 4
//To node 2: 0
//To node 3: 3
//To node 4: 9