#include <iostream>
#include <vector>

using namespace std;

bool isSafe(const vector<int>& board, int row, int col) {
    for (int i = 0; i < row; ++i) {
        if (board[i] == col || abs(board[i] - col) == abs(i - row)) {
            return false;
        }
    }
    return true;
}
void printBoard(const vector<int>& board) {
    int N = board.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i] == j) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void solveNQueensUtil(vector<int>& board, int row) {
    int N = board.size();
    if (row == N) {
        // 放置好了所有的皇后，打印整个棋盘。
        printBoard(board);
        return;
    }

    for (int col = 0; col < N; ++col) {
        if (isSafe(board, row, col)) {
            // 放置皇后
            board[row] = col;
            solveNQueensUtil(board, row + 1);
            // 递归
            board[row] = -1;
        }
    }
}

void solveNQueens(int N) {
    vector<int> board(N, -1);
    solveNQueensUtil(board, 0);
}

int main() {
    int N;
    cout << "Enter one value: ";
    cin >> N ;
    solveNQueens(N);
    return 0;
}
