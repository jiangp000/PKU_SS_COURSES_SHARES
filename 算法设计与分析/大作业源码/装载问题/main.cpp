#include <iostream>
#include <vector>
using namespace std;
void Backtracking(vector<int>& items, int capacity, vector<int>& currentBin) {
    static int binCount = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (items[i] <= capacity) {
            currentBin.push_back(items[i]);
            int removedItem = items[i];
            items.erase(items.begin() + i);
            Backtracking(items, capacity - removedItem, currentBin);
            currentBin.pop_back();
            items.insert(items.begin() + i, removedItem);
        }
    }

    if (currentBin.empty()) {
        cout << "Bin " << ++binCount << ": ";
        for (int item : items) {
            cout << item << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<int> items = {4, 8, 1, 4, 2, 5, 3};
    int binCapacity = 10;
    vector<int> currentBin;
    Backtracking(items, binCapacity, currentBin);
    return 0;
}
//Bin 1: 4 8 1 4 2 5 3
