#include <iostream>
#include <sstream>
#include <fstream> // contains ifstream and ofstream
#include <iterator>
#include <vector>
#include <stack>
#include <unordered_set>
#include <set>
#include <chrono>


#define DEBUG_PRINT 0

#if DEBUG_PRINT
#define DEBUG(x) std::cout << x
#else
#define DEBUG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;

class Timer {
public:
    Timer() { start = std::chrono::high_resolution_clock::now(); }
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Elapsed time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
                  << " us" << std::endl;
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

std::vector<std::pair<int, int>> get_map(std::vector<std::vector<int>>& map, const std::string& filename);

template<typename VisitedType>
void DFS(const std::vector<std::vector<int>>& map, const std::pair<int, int>& idx, VisitedType& visited, int& result);

template <typename VisitedType>
bool isVisitedAndMark(VisitedType& visited, const std::pair<int, int>& idx, int width);

static bool g_part2 = false;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }
    std::vector<std::vector<int>> map;
    auto toVisit = get_map(map, argv[1]);

#if DEBUG_PRINT
    for(std::vector<int>& row: map)
    {
        for(int& value: row)
            cout << value << " ";
        cout << endl;
    }
#endif

    {
        Timer t;
        int r = 0;
        for(std::pair<int,int>& trailHead: toVisit)
        {
            int result = 0;
            std::vector<bool> visited(map.size() * map[0].size(), false);
            DFS(map, trailHead, visited, result);
            DEBUG("trailHead (" << trailHead.first << ", " << trailHead.second << "): " << result << endl);
            r += result;
        }
        cout << "Part 1"  <<  ": " << r << endl;
    }

    {
        g_part2 = true;
        Timer t;
        int r = 0;
        for(std::pair<int,int>& trailHead: toVisit)
        {
            int result = 0;
            std::unordered_set<int> visited = {};
            DFS(map, trailHead, visited, result);
            DEBUG("trailHead (" << trailHead.first << ", " << trailHead.second << "): " << result << endl);
            r += result;
        }
        cout << "Part 2"  <<  ": " << r << endl;
    }

    return 0;
}

template<typename VisitedType>
void DFS(const std::vector<std::vector<int>>& map, const std::pair<int, int>& idx, VisitedType& visited, int& result)
{
    static std::pair<int, int> dirs[4] = {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}
    };

    int curr = map[idx.first][idx.second];

    if(g_part2)
    {
        if (isVisitedAndMark(visited, idx, map[0].size())) return;
    }

    if(curr == 9)
    {
        // cout << "Found 9-height position" << endl;
        ++result;
        return;
    }

    for(const auto& dir: dirs)
    {
        int i = idx.first+dir.first, j = idx.second+dir.second;
        if(i<0||i>map.size()-1||j<0||j>map[0].size()-1)
        {
            continue;
        }
        int temp = map[i][j];
        if(temp-curr != 1)
        {
            continue;
        }
        DFS(map, {i,j}, visited, result);
    }
}

// Default  for std::vector<T>
template <typename VisitedType>
bool isVisitedAndMark(VisitedType &visited, const std::pair<int, int> &idx, int width)
{
    int pos = idx.first * width + idx.second;
   if (visited[pos]) return true;
    visited[pos] = true;
    return false;
}

// Specialization for std::unordered_set<int>
template<>
bool isVisitedAndMark<std::unordered_set<int>>(std::unordered_set<int>& visited, const std::pair<int, int>& idx, int width) {
    int pos = idx.first * width + idx.second;
    if (visited.find(pos) != visited.end()) return true;
    visited.insert(pos);
    return false;
}

// Specialization for std::set<int>
template<>
bool isVisitedAndMark<std::set<int>>(std::set<int>& visited, const std::pair<int, int>& idx, int width) {
    int pos = idx.first * width + idx.second;
    if (visited.find(pos) != visited.end()) return true;
    visited.insert(pos);
    return false;
}

// Specialization for std::vector<std::vector<bool>>
template<>
bool isVisitedAndMark(std::vector<std::vector<bool>>& visited, const std::pair<int, int>& idx, int) {
    if (visited[idx.first][idx.second]) return true;
    visited[idx.first][idx.second] = true;
    return false;
}

std::vector<std::pair<int, int>> get_map(std::vector<std::vector<int>>& map, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return {};
    }
    std::vector<std::pair<int, int>> trailHeads = {};
    
    std::string line;
    for(int i = 0; std::getline(file, line); ++i)
    {
        std::vector<int> row;
        row.reserve(line.size());

        for(int j = 0; j < line.size(); ++j)
        {
            row.push_back(line[j]-'0');
            if(line[j]=='0')
            {
                trailHeads.emplace_back(i,j);
            }
        }
        map.push_back(row);
    }
    return trailHeads;
}

// [Unused] for educational purposes
void DFS_iterative(const std::vector<std::vector<int>>& map, const std::pair<int, int>& start, std::vector<bool>& visited, int& result)
{
    static std::pair<int, int> dirs[4] = {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}
    };

    std::stack<std::pair<int, int>> stk;
    stk.push(start);

    while (!stk.empty()) {
        int x(stk.top().first), y(stk.top().second);
        // auto [x, y] = stk.top();
        stk.pop();

        if(g_part2) {
            if (visited[x * map[0].size() + y]) return;
            visited[x * map[0].size() + y] = true;
        }

        int curr = map[x][y];

        if (curr == 9) {
            ++result;
            continue;
        }

        for (const auto& dir : dirs) {
            int i = x + dir.first, j = y + dir.second;
            if (i < 0 || i >= map.size() || j < 0 || j >= map[0].size()) {
                continue;
            }

            int temp = map[i][j];
            if (temp - curr != 1) {
                continue;
            }

            stk.push({i, j});  // Push valid neighbor onto stack
        }
    }
}