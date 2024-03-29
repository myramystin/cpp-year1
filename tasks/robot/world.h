#pragma once

#include "point.h"
#include "topology.h"

#include <exception>
#include <unordered_map>

class World {
public:
    class IllegalMoveException : public std::exception {};

    World(const Topology& topology, Point start, Point end);

    std::unordered_map<Point, Topology::Distance> Lookup() const;

    Topology::Distance GetDistance(const Point& from, const Point& to);

    std::vector<Point> GetValidNeighbors(const Point& from);

    const Point& GetStart() const;
    const Point& GetEnd() const;
    const Point& GetCurrentPosition() const;

    void Move(const Point& to);

private:
    const Topology& topology_;
    Point start_;
    Point end_;
    Point current_;
};
