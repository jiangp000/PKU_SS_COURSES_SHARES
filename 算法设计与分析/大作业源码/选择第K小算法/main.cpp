#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

int quickSelect(vector<int>& arr, int low, int high, int k) {
    if (k > 0 && k <= high - low + 1) {
        int pi = partition(arr, low, high);
        if (pi - low == k - 1) {
            return arr[pi];
        }
        if (pi - low > k - 1) {
            return quickSelect(arr, low, pi - 1, k);
        }
        return quickSelect(arr, pi + 1, high, k - pi + low - 1);
    }
    return -1;
}

int kthSmallest(vector<int>& arr, int k) {
    int n = arr.size();
    if (k > 0 && k <= n) {
        return quickSelect(arr, 0, n - 1, k);
    }
    return -1;
}

int main() {
    vector<int> arr = {11,222,3333,4,55,666,777,888,-9, 999};
    int k = 4;
    int result = kthSmallest(arr, k);
    if (result != -1) {
        cout << "The " << k << "th smallest element: " << result << endl;
    } else {
        cout << "ERROR" << endl;
    }
    return 0;
}
