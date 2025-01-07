#ifndef GUARD_H
#define GUARD_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "point.h"

using PointSet = std::unordered_set<Point, PointHash>;
using PointPairSet = std::unordered_set<std::pair<Point, Point>, PairHash>;
using PointMap = std::unordered_map<Point, Point, PointHash>;

class Guard
{
public:
    Guard(Point position, Point direction): position(position), direction(direction) {
        //visited.emplace(position);
        visited.emplace(position, direction);
    }
    static const Point dirUp, dirDown, dirLeft, dirRight;
    void move(std::vector<std::string>& map);
    void turnRight();
    bool canMove() const { return can_move; }
    bool stuckInLoop() const { return stuck_in_loop; }
    std::size_t getNumDistinctPositions() const { return visited.size(); }
    const Point& getPosition() const { return position; }
    const Point& getDirection() const { return direction; }
    //const PointSet& getVisited() const { return visited; }
    const PointMap& getVisited() const { return visited; }

    void reset(Point startPosition, Point startDirection);

private:
    Point position, direction;
    //std::vector<Point> visited; // for part 1
    //PointSet visited;
    PointMap visited; // part 2
    bool can_move = true;
    bool stuck_in_loop = false;

    bool is_looping(Point p, Point d) const;
};


#endif // GUARD_H
