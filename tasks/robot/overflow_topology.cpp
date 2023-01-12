#include "overflow_topology.h"
#include <unordered_set>

OverflowTopology::OverflowTopology(const Map& map) : Topology(map) {
}

std::vector<Point> OverflowTopology::GetNeighbours(const Point& point) const {
    std::unordered_set<Point> result_unique;
    std::vector<Point> result;
    if (point.x > 0u) {
        result_unique.insert(Point{.x = point.x - 1u, .y = point.y});
    } else {
        result_unique.insert(Point{.x = Topology::GetFieldXBorder() - 1u, .y = point.y});
    }
    if (point.y > 0u) {
        result_unique.insert(Point{.x = point.x, .y = point.y - 1u});
    } else {
        result_unique.insert(Point{
            .x = point.x,
            .y = Topology::GetFieldYBorder() - 1u,
        });
    }
    if (point.x < Topology::GetFieldXBorder() - 1u) {
        result_unique.insert(Point{.x = point.x + 1u, .y = point.y});
    } else {
        result_unique.insert(Point{.x = 0u, .y = point.y});
    }
    if (point.y < Topology::GetFieldYBorder() - 1u) {
        result_unique.insert(Point{.x = point.x, .y = point.y + 1u});
    } else {
        result_unique.insert(Point{.x = point.x, .y = 0u});
    }
    for (const auto& pt : result_unique) {
        result.push_back(pt);
    }
    return result;
}
