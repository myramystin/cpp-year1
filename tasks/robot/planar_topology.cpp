#include "planar_topology.h"
#include <algorithm>
#include <queue>

std::vector<Point> PlanarTopology::GetNeighbours(const Point& point) const {
    std::vector<Point> result;
    if (point.x > 0u) {
        result.push_back(Point{.x = point.x - 1u, .y = point.y});
    }
    if (point.y > 0u) {
        result.push_back(Point{.x = point.x, .y = point.y - 1u});
    }
    if (point.x < Topology::GetFieldXBorder() - 1u) {
        result.push_back(Point{.x = point.x + 1u, .y = point.y});
    }
    if (point.y < Topology::GetFieldYBorder() - 1u) {
        result.push_back(Point{.x = point.x, .y = point.y + 1u});
    }
    return result;
}

PlanarTopology::PlanarTopology(const Map& map) : Topology(map) {
}
