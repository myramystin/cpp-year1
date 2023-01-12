#pragma once

#include "topology.h"
#include <unordered_set>
#include <queue>

using BoolPoint = std::pair<bool, Point>;

class CheckersTopology : public Topology {
public:
    CheckersTopology(const Map& map);

    virtual std::vector<Point> GetNeighbours(const Point& point) const override;

private:
    BoolPoint FindExactNeighbor(bool x_up, bool y_up, const Point& starter) const;
    BoolPoint FindInOneNeighbor(bool x_up, bool y_up, const Point& starter) const;
};
