#include <iostream>
#include <vector>
using namespace std;
int maxSumIncreasingSubsequence(const vector<int>& values) {
    int n = values.size();
    vector<int> dp(n, 0);
    int result = values[0];
    for (int i = 0; i < n; ++i) {
        dp[i] = values[i];
        for (int j = 0; j < i; ++j) {
            if (values[i] > values[j]) {
                dp[i] = max(dp[i], dp[j] + values[i]);
            }
        }
        result = max(result, dp[i]);
    }
    return result;
}

int main() {
    vector<int> values = {6, 7, 8, 1, 2, 3, 9};
    int result = maxSumIncreasingSubsequence(values);
    cout << "Maximum Sum Increasing Subsequence: " << result << endl;
    return 0;
}
