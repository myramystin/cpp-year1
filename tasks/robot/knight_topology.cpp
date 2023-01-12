#include "knight_topology.h"

KnightTopology::KnightTopology(const Map& map) : Topology(map) {
}

std::vector<Point> KnightTopology::GetNeighbours(const Point& point) const {
    std::vector<Point> result;
    if (point.x > 1u) {
        if (point.y > 0u) {
            result.push_back(Point{.x = point.x - 2u, .y = point.y - 1u});
        }
        if (point.y < Topology::GetFieldYBorder() - 1u) {
            result.push_back(Point{.x = point.x - 2u, .y = point.y + 1u});
        }
    }
    if (point.x < Topology::GetFieldXBorder() - 2u) {
        if (point.y > 0u) {
            result.push_back(Point{.x = point.x + 2u, .y = point.y - 1u});
        }
        if (point.y < Topology::GetFieldYBorder() - 1u) {
            result.push_back(Point{.x = point.x + 2u, .y = point.y + 1u});
        }
    }
    if (point.y > 1u) {
        if (point.x > 0u) {
            result.push_back(Point{.x = point.x - 1u, .y = point.y - 2u});
        }
        if (point.x < Topology::GetFieldXBorder() - 1u) {
            result.push_back(Point{.x = point.x + 1u, .y = point.y - 2u});
        }
    }
    if (point.y < GetFieldYBorder() - 2u) {
        if (point.x > 0u) {
            result.push_back(Point{.x = point.x - 1u, .y = point.y + 2u});
        }
        if (point.x < Topology::GetFieldXBorder() - 1u) {
            result.push_back(Point{.x = point.x + 1u, .y = point.y + 2u});
        }
    }
    return result;
}
