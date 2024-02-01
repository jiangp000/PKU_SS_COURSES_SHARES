#include <iostream>
#include <vector>
using namespace std;
int maxVal = 0;

void backtrack(int W, vector<int>& weights, vector<int>& values, int index, int currentWeight, int currentValue) {
    if (index == weights.size() || currentWeight > W) {
        maxVal = max(maxVal, currentValue);
        return;
    }
    backtrack(W, weights, values, index + 1, currentWeight, currentValue);
    if (currentWeight + weights[index] <= W) {
        backtrack(W, weights, values, index + 1, currentWeight + weights[index], currentValue + values[index]);
    }
}

int compute(int W, vector<int>& weights, vector<int>& values) {
    maxVal = 0;
    backtrack(W, weights, values, 0, 0, 0);
    return maxVal;
}

int main() {
    vector<int> weights = {2, 1, 3, 2};
    vector<int> values = {12, 10, 20, 15};
    int W = 5;
    int result = compute(W, weights, values);
    cout << "Maximum value: " << result << endl;

    return 0;
}
