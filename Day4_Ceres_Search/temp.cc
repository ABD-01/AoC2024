#include <iostream>
#include <iterator>
#include <fstream>

// Claude Solution for diagonals
void printDiagonals(int maxRow, int maxCol) {
    std::cout << "Diagonals\n";
    // For main diagonals, points on same diagonal have same (row-col)
    for (int diff = -(maxCol-1); diff < maxRow; ++diff) {
        std::cout << (diff + maxCol - 1) << ": ";
        int startRow = std::max(0, diff);
        int startCol = startRow - diff;
        for (int row = startRow; row < maxRow && (row-diff) < maxCol; ++row) {
            std::cout << row << "," << (row-diff) << " ";
        }
        std::cout << '\n';
    }
    
    std::cout << "\nAnti Diagonals\n";
    // For anti-diagonals, points on same diagonal have same (row+col)
    for (int sum = 0; sum < maxRow + maxCol - 1; ++sum) {
        std::cout << sum << ": ";
        int startRow = std::max(0, sum - (maxCol - 1));
        int endRow = std::min(maxRow - 1, sum);
        for (int row = startRow; row <= endRow; ++row) {
            std::cout << row << "," << (sum-row) << " ";
        }
        std::cout << '\n';
    }
}

int __main()
{
    int maxRow = 4;
    int maxCol = 3;

    std::cout << "Diagonals" << std::endl;
    int row_start = 0;
    int col_start = maxCol - 1;
    for (int i = 0; i < maxCol+maxRow-1; ++i) { 
        std::cout << i << ": ";
        row_start += (col_start < 0);
        for (int j = std::max(0, col_start), k = row_start; j < maxCol && k < maxRow; ++j, ++k)
        {
                std::cout << k << "," << j << " ";
        }
        --col_start;
        std::cout << std::endl;
    }

    std::cout << "Anti Diagonals" << std::endl;
    row_start = 0;
    col_start = 0;
    for (int i = 0; i < maxCol+maxRow-1; ++i) { 
        std::cout << i << ": ";
        row_start += (col_start > maxCol-1);
        for (int j = std::min(maxCol-1, col_start), k = row_start; j > -1 && k < maxRow; --j, ++k)
        {
                std::cout << k << "," << j << " ";
        }
        ++col_start;
        std::cout << std::endl;
    }

    printDiagonals(maxRow, maxCol);
    return 0;
}


// ChatGPT Solution
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Define directions for searching (dx, dy pairs)
const vector<pair<int, int>> directions = {
    {0, 1},   // Right
    {0, -1},  // Left
    {1, 0},   // Down
    {-1, 0},  // Up
    {1, 1},   // Diagonal Down-Right
    {1, -1},  // Diagonal Down-Left
    {-1, 1},  // Diagonal Up-Right
    {-1, -1}  // Diagonal Up-Left
};

// Function to check if "XMAS" exists starting at (x, y) in a given direction
bool findWord(const vector<string>& grid, int x, int y, int dx, int dy, const string& word) {
    int n = grid.size();
    int m = grid[0].size();
    int len = word.length();
    
    for (int i = 0; i < len; i++) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx < 0 || ny < 0 || nx >= n || ny >= m || grid[nx][ny] != word[i]) {
            return false;
        }
    }
    return true;
}

int countXMAS(const vector<string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    string word = "XMAS";
    int count = 0;

    // Traverse the grid and check for "XMAS" in all directions
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (auto [dx, dy] : directions) {
                if (findWord(grid, i, j, dx, dy, word)) {
                    count++;
                }
            }
        }
    }
    return count;
}


int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }
    ifstream file(argv[1]);
    if(!file.is_open())
    {
        cerr << "Failed to open file" << endl;
        return 1;
    }
    vector<string> grid = {istream_iterator<string>(file), istream_iterator<string>()};

    int result = countXMAS(grid);
    cout << "Number of occurrences of XMAS: " << result << endl;
    return 0;
}

