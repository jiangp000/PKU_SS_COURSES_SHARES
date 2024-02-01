#include <iostream>
#include <vector>

using namespace std;

int knapsack3D(int capacity1, int capacity2, int capacity3, const vector<int>& weights1, const vector<int>& weights2, const vector<int>& weights3, const vector<int>& values) {
    int n = weights1.size();
    // dp[i][j][k][l]表示在容量1为i、容量2为j、容量3为k时，前l个物品的最大价值
    vector<vector<vector<vector<int>>>> dp(capacity1 + 1, vector<vector<vector<int>>>(capacity2 + 1, vector<vector<int>>(capacity3 + 1, vector<int>(n + 1, 0))));
    // 遍历每个物品
    for (int l = 1; l <= n; ++l) {
        for (int w1 = 0; w1 <= capacity1; ++w1) {
            for (int w2 = 0; w2 <= capacity2; ++w2) {
                for (int w3 = 0; w3 <= capacity3; ++w3) {
                    if (w1 >= weights1[l - 1] && w2 >= weights2[l - 1] && w3 >= weights3[l - 1]) {
                        dp[w1][w2][w3][l] = max(dp[w1][w2][w3][l - 1], dp[w1 - weights1[l - 1]][w2 - weights2[l - 1]][w3 - weights3[l - 1]][l - 1] + values[l - 1]);
                    } else {
                        dp[w1][w2][w3][l] = dp[w1][w2][w3][l - 1];
                    }
                }
            }
        }
    }
    return dp[capacity1][capacity2][capacity3][n];
}

int main() {
    // 输入数据
    vector<int> weights1 = {2, 3, 4, 5};
    vector<int> weights2 = {1, 2, 3, 4};
    vector<int> weights3 = {1, 2, 3, 4};
    vector<int> values = {3, 4, 5, 6};
    int capacity1 = 10;
    int capacity2 = 10;
    int capacity3 = 10;
    int result = knapsack3D(capacity1, capacity2, capacity3, weights1, weights2, weights3, values);
    cout << "Maximum value in 3D knapsack: " << result << endl;

    return 0;
}
