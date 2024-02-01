#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Graph {
public:
    Graph(int vertices);
    void addEdge(int u, int v);
    void graphColoring(int m);

private:
    int vertices;
    vector<vector<int>> adjacencyMatrix;

    bool isSafe(int v, vector<int>& color, int c);
};

Graph::Graph(int vertices) : vertices(vertices) {
    adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
}

void Graph::addEdge(int u, int v) {
    // 添加边
    adjacencyMatrix[u][v] = 1;
    adjacencyMatrix[v][u] = 1;
}

bool Graph::isSafe(int v, vector<int>& color, int c) {
    for (int i = 0; i < vertices; ++i) {
        if (adjacencyMatrix[v][i] && c == color[i]) {
            return false;
        }
    }
    return true;
}

void Graph::graphColoring(int m) {
    vector<int> color(vertices, 0);
    color[0] = 1;
    // 尝试着色其余节点
    for (int v = 1; v < vertices; ++v) {
        for (int c = 1; c <= m; ++c) {
            if (isSafe(v, color, c)) {
                color[v] = c;
                break;
            }
        }
    }

    // 输出结果
    cout << "Minimum number of colors required: " << *max_element(color.begin(), color.end()) << endl;
    cout << "Node colors: ";
    for (int i = 0; i < vertices; ++i) {
        cout << color[i] << " ";
    }
    cout << endl;
}

int main() {
    Graph graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(0,5);
    graph.addEdge(1,5);
    graph.addEdge(2,4);
    int m = 4; // 颜色的数量
    graph.graphColoring(m);
    return 0;
}
//Minimum number of colors required: 3
//Node colors: 1 2 3 1 1 3