#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
using namespace std;
// 表示线性规划问题的结构体
struct LinearProgram {
    vector<vector<int>> coefficients;  // 系数矩阵
    vector<double> constants;          // 右侧常数（可以是小数）
    vector<int> objectiveFunction;     // 目标函数系数
};
// 表示节点的结构体
struct Node {
    vector<int> solution;  // 当前节点的解
    int value;             // 目标函数值
    int level;             // 节点的层级
};

// 用于比较节点的优先级队列
struct CompareNode {
    bool operator()(const Node& n1, const Node& n2) {
        return n1.value < n2.value;  // 按目标函数值升序排列
    }
};

// 检查当前解是否满足整数要求
bool isIntegerSolution(const vector<int>& solution) {
    for (int value : solution) {  // 使用 int 类型
        if (fabs(value - round(value)) > numeric_limits<double>::epsilon()) {
            return false;  // 不是整数解
        }
    }
    return true;  // 是整数解
}

// 分支限界算法
int branchAndBound(const LinearProgram& lp) {
    int n = lp.coefficients[0].size();  // 变量个数
    priority_queue<Node, vector<Node>, CompareNode> pq;  // 优先级队列
    Node rootNode;
    rootNode.solution.resize(n, 0);
    rootNode.value = 0;
    rootNode.level = 0;
    pq.push(rootNode);
    int round = 1;
    while (!pq.empty()) {
        cout << "Round " << round << ":\n";
        Node currentNode = pq.top();
        pq.pop();
        cout << "Current- Level: " << currentNode.level << ", Solution: [";
        for (int i = 0; i < n; ++i) {
            cout << currentNode.solution[i];
            if (i < n - 1) {
                cout << ", ";
            }
        }
        cout << "], Value: " << currentNode.value << "\n";
        if (currentNode.level == n) {
            cout << "Found a BEST solution: [";
            for (int i = 0; i < n; ++i) {
                cout << currentNode.solution[i];
                if (i < n - 1) {
                    cout << ", ";
                }
            }
            cout << "], Value: " << currentNode.value << "\n";
            return currentNode.value;  // 返回当前节点的目标函数值
        } else {
            for (int i = 0; i <= 1; ++i) {  // 分支，每个变量可以取整数值或者不取整数值
                Node childNode = currentNode;
                childNode.solution[currentNode.level] = i;
                childNode.level++;
                // 计算子节点的目标函数值
                childNode.value = 0;
                for (int j = 0; j < n; ++j) {
                    childNode.value += childNode.solution[j] * lp.objectiveFunction[j];
                }
                // 计算约束条件是否满足
                bool constraintsSatisfied = true;
                for (int j = 0; j < lp.coefficients.size(); ++j) {
                    double constraintValue = 0;
                    for (int k = 0; k < n; ++k) {
                        constraintValue += childNode.solution[k] * lp.coefficients[j][k];
                    }
                    if (constraintValue > lp.constants[j]) {
                        constraintsSatisfied = false;
                        break;
                    }
                }
                if (constraintsSatisfied) {
                    cout << "Look Child - LP: " << childNode.level << ", Solution: [";
                    for (int i = 0; i < n; ++i) {
                        cout << childNode.solution[i];
                        if (i < n - 1) {
                            cout << ", ";
                        }
                    }
                    cout << "], Value: " << childNode.value << "\n";
                    // 如果是整数解，更新最优解
                    if (isIntegerSolution(childNode.solution) && childNode.value < pq.top().value) {
                        pq.push(childNode);
                    } else if (!isIntegerSolution(childNode.solution)) {
                        pq.push(childNode);
                    }
                }
            }
        }
        round++;
    }
    return -1;  // 未找到最优解
}

int main() {
    LinearProgram lp;
    lp.coefficients = {
            {-1, 1},   // 系数矩阵
            {2, 3}
    };
    lp.constants = {1, 11};
    lp.objectiveFunction = {-3, -5};
    int result = branchAndBound(lp);
    cout << "Optimal value: " << result << endl;
    return 0;
}
