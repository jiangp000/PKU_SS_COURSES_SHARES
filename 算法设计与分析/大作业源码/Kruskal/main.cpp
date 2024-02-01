#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge {
    int src, dest, weight;
    Edge(int src, int dest, int weight) : src(src), dest(dest), weight(weight) {}
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};
class DisjointSet {
public:
    DisjointSet(int size) : parent(size), rank(size, 0) {
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }
    void Union(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootX] = rootY;
                rank[rootY]++;
            }
        }
    }
private:
    vector<int> parent;
    vector<int> rank;
};

void kruskal(const vector<Edge>& edges, int numNodes) {
    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end());
    DisjointSet ds(numNodes);
    cout << "Edges of the Minimum Spanning Tree:\n";
    for (const Edge& edge : sortedEdges) {
        int rootSrc = ds.find(edge.src);
        int rootDest = ds.find(edge.dest);
        if (rootSrc != rootDest) {
            cout << "Edge: " << edge.src << " - " << edge.dest << " Weight: " << edge.weight << endl;
            ds.Union(rootSrc, rootDest);
        }
    }
}

int main() {
    vector<Edge> edges = {
            {0, 1, 1},
            {0, 3, 2},
            {1, 2, 3},
            {1, 4, 4},
            {2, 4, 4},
            {2, 5, 4},
            {3, 5, 5},
            {4, 5, 9}
    };
    int numNodes = 6; // 节点数
    kruskal(edges, numNodes);
    return 0;
}
//Edges of the Minimum Spanning Tree:
//Edge: 0 - 1 Weight: 2
//Edge: 1 - 2 Weight: 3
//Edge: 4 - 5 Weight: 4
//Edge: 1 - 4 Weight: 5
//Edge: 0 - 3 Weight: 6