#include "point.h"

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

std::size_t std::hash<Point>::operator()(const Point& point) const {
    std::size_t h1 = std::hash<std::size_t>{}(point.x);
    std::size_t h2 = std::hash<std::size_t>{}(point.y);
    return h1 ^ (h2 << 1);  // or use boost::hash_combine
}
