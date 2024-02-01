#include <iostream>
#include <vector>

using namespace std;

int knapsack1D(int capacity, const vector<int>& weights, const vector<int>& values) {
    int n = weights.size();
    vector<int> dp(capacity + 1, 0);

    for (int i = 0; i < n; ++i) {
        for (int w = capacity; w >= weights[i]; --w) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }

    return dp[capacity];
}

int main() {
    vector<int> weights = {2, 3, 4, 5};
    vector<int> values = {3, 4, 5, 6};
    int capacity = 10;

    int result = knapsack1D(capacity, weights, values);
    cout << "Maximum value in 1D knapsack: " << result << endl;
    return 0;

//    13
}
