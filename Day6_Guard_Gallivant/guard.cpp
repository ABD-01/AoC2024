#include "guard.h"

const Point Guard::dirUp(-1, 0);
const Point Guard::dirDown(1, 0);
const Point Guard::dirLeft(0, -1);
const Point Guard::dirRight(0, 1);

void Guard::move(std::vector<std::string>& map)
{
    map[position.x][position.y] = 'X';
    Point temp = position+direction;

    if (temp.x < 0 || temp.y < 0 || temp.x >= map.size() || temp.y >= map[0].size())
    {
        // Guard left the map
        can_move = false;
        //cout << "Guard left the map" << endl;
        return;
    }

    if (map[temp.x][temp.y] == '#' || map[temp.x][temp.y] == 'O')
    {
        // Guard hit an obstacle, turn right
        turnRight();
        move(map);
        return;
    }

    if(is_looping(temp, direction))
    {
        can_move = false;
        stuck_in_loop = true;
        //cout << "Guard stuck in a Loop" << endl;
        return;
    }

    position = temp;

    //map[position.x][position.y] = (direction == dirUp) ? '^' : 
        //(direction == dirDown) ? 'v' :
        //(direction == dirLeft) ? '<' : '>';
    
    //visited.emplace(position);
    visited.emplace(position, direction);
}

void Guard::turnRight()
{
    //-1,0  -> 0,1
    // 0,1  -> 1,0
    // 1,0  -> 0,-1
    // 0,-1 -> -1,0

    direction = Point(direction.y, -direction.x);
}

bool Guard::is_looping(Point p, Point d) const
{
    PointMap::const_iterator it = visited.find(p);
    if (it != visited.end() && it->second == d)
    {
        return true;
    }
    return false;
}

void Guard::reset(Point startPosition, Point startDirection)
{
    position = startPosition;
    direction = startDirection;
    visited.clear();  // Clear previous visit data
    visited.emplace(position, direction);  // Re-add initial position and direction
    can_move = true;
    stuck_in_loop = false;
}

