#include <iostream>
#include <vector>

using namespace std;

vector<int> findMinMax(const vector<int>& arr, int start, int end) {
    vector<int> result(2, 0);
    if (start == end) {
        result[0] = arr[start]; // 数组中只有一个元素
        result[1] = arr[start];
        return result;
    }
    int mid = (start + end) / 2;
    vector<int> leftResult = findMinMax(arr, start, mid);
    vector<int> rightResult = findMinMax(arr, mid + 1, end);
    result[0] = min(leftResult[0], rightResult[0]);
    result[1] = max(leftResult[1], rightResult[1]);
    return result;
}

int main() {
    vector<int> arr = {11,222,3333,4,55,666,777,888,-9, 999};
    // 计算最大值和最小值
    vector<int> result = findMinMax(arr, 0, arr.size() - 1);
    cout << "Maximum value : " << result[1] << endl;
    cout << "Minimum value : " << result[0] << endl;
    return 0;
}
