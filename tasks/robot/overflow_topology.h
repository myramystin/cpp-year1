#pragma once

#include "topology.h"

class OverflowTopology : public Topology {
public:
    OverflowTopology(const Map& map);

    virtual std::vector<Point> GetNeighbours(const Point& point) const override;
};
