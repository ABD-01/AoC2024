#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point
{
    int x,y;
    Point(int x, int y);
    Point();

    Point operator+(const Point& p) const;
    bool operator==(const Point& p) const;
};

std::ostream& operator<<(std::ostream& os, const Point& p);

struct PointHash {
	std::size_t operator()(const Point& p) const;
};

struct PairHash {
    std::size_t operator()(const std::pair<Point, Point>& pair) const;
};

// Unused
struct PairEqual {
    bool operator()(const std::pair<Point, Point>& a, const std::pair<Point, Point>& b) const;
};
#endif // POINT_H
