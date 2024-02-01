#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// 定义一个结构体表示单纯形表
struct SimplexTable {
    std::vector<std::vector<double>> table;
    std::vector<int> basis; // 存储基变量的索引
};

// 打印单纯形表
void printSimplexTable(const SimplexTable& simplexTable) {
    int width = 6;
    for (const auto& row : simplexTable.table) {
        for (double value : row) {
            std::cout << std::setw(width) << value;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 在单纯形表上应用主元素法
void pivot(SimplexTable& simplexTable, int row, int col) {
    int numRows = simplexTable.table.size();
    int numCols = simplexTable.table[0].size();
    // 将主元素所在列变为单位向量
    for (int i = 0; i < numRows; ++i) {
        if (i != row) {
            double ratio = simplexTable.table[i][col] / simplexTable.table[row][col];
            for (int j = 0; j < numCols; ++j) {
                simplexTable.table[i][j] -= ratio * simplexTable.table[row][j];
            }
        }
    }
    // 将主元素所在行变为单位向量
    double pivotElement = simplexTable.table[row][col];
    for (int j = 0; j < numCols; ++j) {
        simplexTable.table[row][j] /= pivotElement;
    }
    // 更新基变量
    simplexTable.basis[row] = col;
}

// 执行单纯形法
bool simplex(std::vector<std::vector<double>>& coefficients, std::vector<double>& objectiveFunction, std::vector<double>& constraints) {
    int numRows = coefficients.size();
    int numCols = coefficients[0].size();
    SimplexTable simplexTable;
    simplexTable.table.resize(numRows + 1, std::vector<double>(numCols + numRows + 1, 0));
    simplexTable.basis.resize(numRows);
    // 初始化单纯形表
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            simplexTable.table[i][j] = coefficients[i][j];
        }
        simplexTable.basis[i] = numCols + i;
        simplexTable.table[i][numCols + i] = 1;
        simplexTable.table[i][numCols + numRows] = constraints[i];
    }
    for (int j = 0; j < numCols; ++j) {
        simplexTable.table[numRows][j] = objectiveFunction[j];
    }
    printSimplexTable(simplexTable);
    // 迭代直到目标函数系数非负
    while (true) {
        int enteringCol = -1;
        for (int j = 0; j < numCols; ++j) {
            if (simplexTable.table[numRows][j] < 0) {
                enteringCol = j;
                break;
            }
        }
        if (enteringCol == -1) {
            // 所有目标函数系数非负，达到最优解
            break;
        }
        int leavingRow = -1;
        double minRatio = std::numeric_limits<double>::infinity();

        for (int i = 0; i < numRows; ++i) {
            if (simplexTable.table[i][enteringCol] > 0) {
                double ratio = simplexTable.table[i][numCols + numRows] / simplexTable.table[i][enteringCol];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    leavingRow = i;
                }
            }
        }
        if (leavingRow == -1) {
            return false;
        }

        pivot(simplexTable, leavingRow, enteringCol);

        std::cout << "CHOOSE AND CHANGE:\n";
        printSimplexTable(simplexTable);
    }

    // 输出最优解和最优值
    std::cout << "Optimal Solution:\n";
    for (int i = 0; i < numCols; ++i) {
        int basisIndex = -1;
        for (int j = 0; j < numRows; ++j) {
            if (simplexTable.basis[j] == i) {
                basisIndex = j;
                break;
            }
        }
        if (basisIndex != -1) {
            std::cout << "x" << i + 1 << " = " << simplexTable.table[basisIndex][numCols + numRows] << std::endl;
        } else {
            std::cout << "x" << i + 1 << " = 0" << std::endl;
        }
    }
    return true;
}

int main() {
    // 利用单纯性求解下列的标准型。
    // 小于等于的标准形式
    // 例如这里是课后6.10题目的第一小题
//    min -2x1 + x2 - x3
//    2x1 + x2 <= 10
//    -4x1 -2x2 + 3x3 <= 10
//    x1 - 2x2 + x3 <= 14
//    x1 x2 x3 >= 0
    std::vector<std::vector<double>> coefficients = {{2, 1, 0, 1, 0, 0}, {-4, -2, 3, 0, 1, 0},{1, -2, 1, 0, 0, 1}};
    std::vector<double> objectiveFunction = {-2, 1, -1, 0, 0, 0};
    std::vector<double> constraints = {10, 10, 14};

    if (simplex(coefficients, objectiveFunction, constraints)) {
        std::cout << "Optimization successful.\n";
    } else {
        std::cout << "Optimization failed (unbounded problem).\n";
    }

    return 0;
}
