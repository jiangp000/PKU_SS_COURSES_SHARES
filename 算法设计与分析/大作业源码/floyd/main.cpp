#include <iostream>
#include <vector>
using namespace std;
const int INF = INT_MAX;
void floyd(vector<vector<int>>& graph, int numNodes) {
    vector<vector<int>> distance = graph;
    for (int k = 0; k < numNodes; ++k) {
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                if (distance[i][k] != INF && distance[k][j] != INF &&
                    distance[i][k] + distance[k][j] < distance[i][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                }
            }
        }
    }
    cout << "Shortest distances between all pairs of nodes:\n";
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (distance[i][j] == INF) {
                cout << "INF ";
            } else {
                cout << distance[i][j] << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> graph = {
        {0, 2, INF, 1, INF},
        {2, 0, 4, INF, 5},
        {INF, 4, 0, 3, INF},
        {1, INF, 3, 0, 6},
        {INF, 5, INF, 6, 0}
    };
    int numNodes = graph.size();
    floyd(graph, numNodes);
    return 0;
}
Shortest distances between all pairs of nodes:
0 2 4 1 7
2 0 4 3 5
4 4 0 3 9
1 3 3 0 6
7 5 9 6 0