#include <fstream>
#include <iostream>
#include <sstream>  // For ostringstream
#include <vector>
#include <iterator>
#include <chrono>
#include <cctype>
#include <unordered_map>
#include <unordered_set>

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

template <typename T>
struct Vec2
{
    T x, y;
    Vec2(T x_val, T y_val) : x(x_val), y(y_val) {}
};

using V2 = Vec2<int>;

template <typename T> Vec2<T> constexpr operator+(Vec2<T> a, Vec2<T> b) { return {a.x + b.x, a.y + b.y}; }
template <typename T> Vec2<T> constexpr operator-(Vec2<T> a, Vec2<T> b) { return {a.x - b.x, a.y - b.y}; }
template <typename T> Vec2<T> constexpr operator-(Vec2<T> a) { return {-a.x, -a.y}; }
template <typename T> Vec2<T> constexpr operator*(Vec2<T> a, Vec2<T> b) { return {a.x * b.x, a.y * b.y}; }
template <typename T> Vec2<T> constexpr operator*(Vec2<T> a, T b) { return {a.x * b, a.y * b}; }
template <typename T> Vec2<T> constexpr &operator+=(Vec2<T> &a, Vec2<T> b) { a = a + b; return a; }
template <typename T> Vec2<T> constexpr &operator-=(Vec2<T> &a, Vec2<T> b) { a = a - b; return a; }
template <typename T> bool constexpr operator==(Vec2<T> a, Vec2<T> b) { return a.x == b.x && a.y == b.y; }
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
{
    os << "(" << v.x << "," << v.y << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (auto i = v.begin(); i != v.end(); ++i) {
        if (i != v.begin()) os << ", ";
        os << *i;
    }
    os << "]";
    return os;
}

using mapV2 = std::unordered_map<char, std::vector<Vec2<int>>>;

void part1(const mapV2& data, int maxX, int maxY);
void part2(const mapV2& data, int maxX, int maxY);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << argv[1] << endl;
        return 1;
    }

    mapV2 antennas;
    std::string line;

    int row = 0;
    int maxRow(0), maxCol(0);
    for (; std::getline(file, line); ++row)
    {
        maxCol = (int)line.size();
        for(int col = 0; col < maxCol; ++col)
        {
            if(std::isalnum(line[col]))
            {
                antennas[line[col]].emplace_back(row, col);
            }
        }
    }
    maxRow = row;
    DEBUG("(maxRow, maxCol) = (" << maxRow << ", " << maxCol << ")" << endl); 

#if DEBUG_PRINT
     for(auto& [k, v]: antennas)
     {
         cout << k << ": " << v << endl;
     }
#endif

    {
        Timer t;
        part1(antennas, maxRow, maxCol);
    }
    {
        Timer t;
        part2(antennas, maxRow, maxCol);
    }
}

void part1(const mapV2& data, int maxX, int maxY)
{
    std::unordered_set<int> antinodes;
    for(const std::pair<const char, std::vector<V2>>& antenas: data)
    {
        DEBUG(antenas.first << ": " << antenas.second << endl);
        const std::vector<V2>& a = antenas.second;
        for(int i = 0; i < static_cast<int>(a.size()); ++i)
        {
            for (int j = i+1; j < static_cast<int>(a.size()); ++j)
            {
                DEBUG("[" << a[i] << "," << a[j] << "]" << ": ");
                // check for antinode
                V2 dist = a[i] - a[j];
                V2 antinode1(a[i] + dist), antinode2(a[j] -dist);
                // check boundary condition
                if(antinode1.x < maxX && antinode1.y < maxY && antinode1.x >= 0 && antinode1.y >= 0)
                {
                    antinodes.insert(antinode1.x*maxY + antinode1.y);
                    DEBUG(antinode1 << " hash(" << antinode1.x*maxY + antinode1.y << ") ");
                }
                if(antinode2.x < maxX && antinode2.y < maxY && antinode2.x >= 0 && antinode2.y >= 0)
                {
                    antinodes.insert(antinode2.x*maxY + antinode2.y);
                    DEBUG(antinode2 << " hash(" << antinode2.x*maxY + antinode2.y << ") ");
                }
                DEBUG(endl);
            } 
        }
    }
    cout << "Part 1: " << antinodes.size() << endl;
}
void part2(const mapV2& data, int maxX, int maxY)
{
    std::unordered_set<int> antinodes;
    for(const auto& antenas: data)
    {
        DEBUG(antenas.first << ": " << antenas.second << endl);
        const std::vector<V2>& a = antenas.second;
        for(int i = 0; i < static_cast<int>(a.size()); ++i)
        {
            for (int j = i+1; j < static_cast<int>(a.size()); ++j)
            {
                DEBUG("[" << a[i] << "," << a[j] << "]" << ": ");
                // check for antinode
                antinodes.insert(a[i].x*maxY + a[i].y);
                antinodes.insert(a[j].x*maxY + a[j].y);
                V2 dist = a[i] - a[j];
                V2 antinode1(a[i] + dist), antinode2(a[j] -dist);
                // check boundary condition
                while(antinode1.x < maxX && antinode1.y < maxY && antinode1.x >= 0 && antinode1.y >= 0)
                {
                    antinodes.insert(antinode1.x*maxY + antinode1.y);
                    DEBUG(antinode1 << " hash(" << antinode1.x*maxY + antinode1.y << ") ");
                    antinode1 += dist;
                }
                while(antinode2.x < maxX && antinode2.y < maxY && antinode2.x >= 0 && antinode2.y >= 0)
                {
                    antinodes.insert(antinode2.x*maxY + antinode2.y);
                    DEBUG(antinode2 << " hash(" << antinode2.x*maxY + antinode2.y << ") ");
                    antinode2 -= dist;
                }
                DEBUG(endl);
            } 
        }
    }
    cout << "Part 2: " << antinodes.size() << endl;
}
