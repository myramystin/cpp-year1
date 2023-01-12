#pragma once

#include "point.h"

#include <vector>
#include <unordered_map>

class PriorityNode {
public:
    Point point;
    size_t priority;
    bool operator<(const PriorityNode& other) const;
};

class Topology {
public:
    using Map = std::vector<std::vector<bool>>;
    using Distance = ssize_t;

    Topology(const Map& map);

    std::vector<Point> GetValidNeighbors(const Point& from) const;
    Distance MeasureDistance(const Point& from, const Point& to) const;
    bool GetAccessibility(const Point& point) const;
    size_t GetFieldXBorder() const;
    size_t GetFieldYBorder() const;

    virtual std::vector<Point> GetNeighbours(const Point& point) const = 0;

    static const Distance UNREACHABLE = -1;

    class Field {
    public:
        Field(const Map& map = {});

        size_t GetXBorder() const;
        size_t GetYBorder() const;
        bool GetAccessibility(const Point& point) const;

    private:
        Map map_;
        size_t x_border_;
        size_t y_border_;
    };

private:
    Field field_;
};
