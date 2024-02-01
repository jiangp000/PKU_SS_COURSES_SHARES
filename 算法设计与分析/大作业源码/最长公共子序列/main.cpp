#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int longestCommonSubsequence(const string& str1, const string& str2) {
    int m = str1.length();
    int n = str2.length();
    // dp[i][j]表示str1前i个字符和str2前j个字符的LCS长度
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

int main() {
    string str1 = "stay hungry";
    string str2 = "stay foolish";
    int result = longestCommonSubsequence(str1, str2);
    cout << "Length of Longest Common Subsequence: " << result << endl;
    return 0;
}
