#pragma once

#include "topology.h"

class PlanarTopology : public Topology {
public:
    PlanarTopology(const Map& map);

    virtual std::vector<Point> GetNeighbours(const Point& point) const override;
};
