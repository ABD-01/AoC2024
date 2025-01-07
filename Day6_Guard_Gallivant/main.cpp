#include <iostream>
#include <fstream>
#include "guard.h"

using std::cout;
using std::cerr;
using std::endl;

void get_map(std::vector<std::string>& map, int& gx, int& gy, const std::string& filename);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }

    int gx(-1), gy(-1);
    std::vector<std::string> map;
    get_map(map, gx, gy, argv[1]);

    //for(auto& i:map) cout << i << endl;
    //cout << "Guard Position: (" << gx << ", " << gy << ")" << endl;

    Guard g(Point(gx, gy), Guard::dirUp);
    while (g.canMove())
    {
        g.move(map);
    }
    //for(auto& i:map) cout << i << endl;
    
    cout << "Part 1: " << g.getNumDistinctPositions() << endl;

    int numObstacles = 0;

    // Obstacle to create loop could only be added in path the guard travels
    PointMap guardVisitedPositions = std::move(g.getVisited());
    for(const std::pair<Point, Point>& v: guardVisitedPositions)
    {

        int i(v.first.x), j(v.first.y);

        if(i==gx && j==gy) continue;

        map[i][j] = 'O';
        g = Guard(Point(gx, gy), Guard::dirUp);

        while (g.canMove())
        {
            g.move(map);
        }
        if(g.stuckInLoop())
        {
            ++numObstacles;
        }
        map[i][j] = 'X';
    }
    // Answer: 1995
    cout << "Part 2: " << numObstacles << endl;

    return 0;
}

void get_map(std::vector<std::string>& map, int& gx, int& gy, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    std::string line;

    for (int row = 0; std::getline(file, line); ++row) {
        auto col = line.find("^");
        if (col != std::string::npos) {
            gx = row;
            gy = col;
        }
        map.push_back(line);
    }

    /** Initial Impl
    bool guard_found = false;
    
    while (std::getline(file, line))
    {
        if (!guard_found)
        {
            ++gx;
            auto col = line.find("^");
            if (col != std::string::npos)
            {
                guard_found = true;
                gy = col;
            }
        }
        map.push_back(std::move(line));
    }
    */
}
