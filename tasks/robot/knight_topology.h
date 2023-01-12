#pragma once

#include "topology.h"

class KnightTopology : public Topology {
public:
    KnightTopology(const Map& map);

    virtual std::vector<Point> GetNeighbours(const Point& point) const override;
};
