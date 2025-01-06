#include <iostream>
#include <fstream>
//#include <string>
#include <iterator>
#include <vector>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;

std::vector<std::string> get_diagonals(const std::vector<std::string>& contents, bool antidiagonals = false);
void part1(const std::string& filename);
void part2(const std::string& filename);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }
    part1(argv[1]);
    part2(argv[1]);
    return 0;
}

bool checkMAS(const std::vector<std::string>& grid, int x, int y, int dir)
{
    char c1 = grid[x-1][y-dir];
    char c2 = grid[x+1][y+dir];

    if(c1 == 'M' && c2 == 'S') return true;
    if(c2 == 'M' && c1 == 'S') return true;
    return false;
}

void part2(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Failed to open file" << endl;
        return;
    }
    std::vector<std::string> grid = {std::istream_iterator<std::string>(file), std::istream_iterator<std::string>()};

    int rows{(int)grid.size()}, cols{(int)grid[0].size()};
    int result = 0;

    for (int i = 1; i < rows - 1; ++i)
    {
        for(int j = 1; j < cols - 1; ++j)
        {
            if(grid[i][j] != 'A') continue;
            result += checkMAS(grid, i, j, 1) & checkMAS(grid, i, j, -1);
        }
    }
    cout << "Part 2: " << result << endl;
}


void part1(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Failed to open file" << endl;
        return;
    }
    std::vector<std::string> contents = {std::istream_iterator<std::string>(file), std::istream_iterator<std::string>()};
    
    int rows{(int)contents.size()}, cols{(int)contents[0].size()};

    std::stringstream ss{};
    for (auto j = 0; j < cols; ++j)
    {
        for(auto i = 0; i < rows; ++i)
            ss << contents[i][j];
        ss << endl;
    }
    std::vector<std::string> rotated_contents = {std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>()};

    std::vector<std::string> diag_contents = get_diagonals(contents);
    std::vector<std::string> antidiag_contents = get_diagonals(contents, true);

    int result(0);
    std::string xmas("XMAS");
    std::string samx("SAMX");

    //cout << "Contents:" << endl;
    for(const auto& line : contents)
    {
        for(auto i = 0; i <= line.size() - xmas.size(); ++i)
        {
            if(line.substr(i, xmas.size()) == xmas || line.substr(i, samx.size()) == samx)
                ++result;
        }
        //cout << line << endl;
    }
    //cout << "Rotated Contents:" << endl;
    for(const auto& line : rotated_contents)
    {
        for(auto i = 0; i <= line.size() - xmas.size(); ++i)
        {
            if(line.substr(i, xmas.size()) == xmas || line.substr(i, samx.size()) == samx)
                ++result;
        }
        //cout << line << endl;
    }
    //cout << "Diag Contents:" << endl;
    for(const auto& line : diag_contents)
    {
        if (line.size() < xmas.size()) continue;
        for(auto i = 0; i <= line.size() - xmas.size(); ++i)
        {
            if(line.substr(i, xmas.size()) == xmas || line.substr(i, samx.size()) == samx)
                ++result;
        }
        //cout << line << endl;
    }
    //cout << "AntiDiag Contents:" << endl;
    for(const auto& line : antidiag_contents)
    {
        if (line.size() < xmas.size()) continue;
        for(auto i = 0; i <= line.size() - xmas.size(); ++i)
        {
            if(line.substr(i, xmas.size()) == xmas || line.substr(i, samx.size()) == samx)
                ++result;
        }
        //cout << line << endl;
    }

    cout << "Part 1: " << result << endl;
}

std::vector<std::string> get_diagonals(const std::vector<std::string>& contents, bool antidiagonals)
{
    std::vector<std::string> diagonals{};
    int maxRow{(int)contents.size()}, maxCol{(int)contents[0].size()};
    diagonals.reserve(maxRow + maxCol - 1); // Reserve memory upfront to avoid reallocations

    int row_start = 0;
    int col_start = antidiagonals ? 0 : maxCol - 1;

    for (int i = 0; i < maxCol+maxRow-1; ++i) { 
        std::string diagonal{""};

        if (antidiagonals)
            row_start += (col_start > maxCol-1);
        else
            row_start += (col_start < 0);
        
        //int j = antidiagonals
                //? std::min(maxCol-1, col_start) 
                //: std::max(0, col_start);
        //int k = row_start;
        int j = antidiagonals  ? std::min(maxCol - 1, i) : std::max(0, maxCol - 1 - i);
        int k = antidiagonals ? std::max(0, i - maxCol + 1) : std::max(0, i - maxRow + 1);
        while(j < maxCol && j > -1 && k < maxRow)
        {
            diagonal += contents[k][j];
            ++k;
            j += (antidiagonals ? -1 : 1);
        }
        col_start += (antidiagonals ? 1 : -1);
        diagonals.push_back(diagonal);
    }

    return diagonals;
}
