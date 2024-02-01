#include <iostream>
#include <vector>

using namespace std;

int knapsack2D(int capacity1, int capacity2, const vector<int>& weights1, const vector<int>& weights2, const vector<int>& values) {
    int n = weights1.size(); // 物品的数量

    // 创建一个三维数组dp，其中dp[i][j][k]表示在容量1为i、容量2为j时，前k个物品的最大价值
    vector<vector<vector<int>>> dp(capacity1 + 1, vector<vector<int>>(capacity2 + 1, vector<int>(n + 1, 0)));

    // 遍历每个物品
    for (int k = 1; k <= n; ++k) {
        // 对容量1进行递减遍历
        for (int w1 = 0; w1 <= capacity1; ++w1) {
            // 对容量2进行递减遍历
            for (int w2 = 0; w2 <= capacity2; ++w2) {
                // 如果当前物品可以放入背包
                if (w1 >= weights1[k - 1] && w2 >= weights2[k - 1]) {
                    // 更新最大价值，选择放入当前物品或不放入当前物品的最优解
                    dp[w1][w2][k] = max(dp[w1][w2][k - 1], dp[w1 - weights1[k - 1]][w2 - weights2[k - 1]][k - 1] + values[k - 1]);
                } else {
                    // 如果当前物品无法放入背包，则继承上一个物品的最优解
                    dp[w1][w2][k] = dp[w1][w2][k - 1];
                }
            }
        }
    }

    // 最终的最大价值存储在dp[capacity1][capacity2][n]中
    return dp[capacity1][capacity2][n];
}

int main() {
    // 输入数据
    vector<int> weights1 = {2, 3, 4, 5};
    vector<int> weights2 = {1, 2, 3, 4};
    vector<int> values = {3, 4, 5, 6};
    int capacity1 = 10;
    int capacity2 = 10;

    // 调用二维背包问题的解决函数
    int result = knapsack2D(capacity1, capacity2, weights1, weights2, values);

    // 输出结果
    cout << "Maximum value: " << result << endl;

    return 0;
}
