#include "point.h"

Point::Point(int x, int y): x(x), y(y) {}
Point::Point() : x(0), y(0) {}

Point Point::operator+(const Point& p) const
{
    return Point(x + p.x, y + p.y);
}
bool Point::operator==(const Point& p) const
{
    return x == p.x && y == p.y;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

std::size_t PointHash::operator()(const Point& p) const {
    return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
}

std::size_t PairHash::operator()(const std::pair<Point, Point>& pair) const {
    PointHash pointHash;
    return pointHash(pair.first) ^ (pointHash(pair.second) << 1);
}

// Unused
bool PairEqual::operator()(const std::pair<Point, Point>& a, const std::pair<Point, Point>& b) const {
    return a.first == b.first && a.second == b.second;
}
