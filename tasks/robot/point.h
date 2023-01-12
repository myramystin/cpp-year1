#pragma once

#include <cstddef>
#include <unordered_map>

struct Point {
    size_t x = 0;
    size_t y = 0;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

template <>
struct std::hash<Point> {
    std::size_t operator()(const Point& point) const;
};
