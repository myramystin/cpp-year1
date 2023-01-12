#include "checkers_topology.h"

CheckersTopology::CheckersTopology(const Map& map) : Topology(map) {
}

std::vector<Point> CheckersTopology::GetNeighbours(const Point& point) const {
    std::vector<Point> result;
    std::queue<Point> buffer;
    std::unordered_set<Point> result_unique;
    // first 4:
    for (auto i = 0; i < 2; ++i) {
        for (auto j = 0; j < 2; ++j) {
            const auto& bool_neighbor = FindExactNeighbor(i, j, point);
            if (bool_neighbor.first) {
                if (!result_unique.contains(bool_neighbor.second)) {
                    result_unique.insert(bool_neighbor.second);
                }
            } else {
                const auto& bool_neighbor2 = FindInOneNeighbor(i, j, point);
                if (bool_neighbor2.first) {
                    if (!result_unique.contains(bool_neighbor2.second)) {
                        buffer.push(bool_neighbor2.second);
                        result_unique.insert(bool_neighbor2.second);
                    }
                }
            }
        }
    }
    while (!buffer.empty()) {
        auto& current = buffer.back();
        buffer.pop();
        for (auto i = 0; i < 2; ++i) {
            for (auto j = 0; j < 2; ++j) {
                const auto& bool_neighbor = FindInOneNeighbor(i, j, current);
                if (bool_neighbor.first) {
                    if (!result_unique.contains(bool_neighbor.second)) {
                        buffer.push(bool_neighbor.second);
                        result_unique.insert(bool_neighbor.second);
                    }
                }
            }
        }
    }
    for (const auto& pt : result_unique) {
        result.push_back(pt);
    }
    return result;
}

BoolPoint CheckersTopology::FindExactNeighbor(bool x_up, bool y_up, const Point& starter) const {
    if (x_up && starter.x < Topology::GetFieldXBorder() - 1u) {
        if (y_up && starter.y < Topology::GetFieldYBorder() - 1u) {
            if (Topology::GetAccessibility({starter.x + 1u, starter.y + 1u})) {
                return {true, {starter.x + 1u, starter.y + 1u}};
            }
        } else if (!y_up && starter.y > 0u) {
            if (Topology::GetAccessibility({starter.x + 1u, starter.y - 1u})) {
                return {true, {starter.x + 1u, starter.y - 1u}};
            }
        }
    } else if (!x_up && starter.x > 0u) {
        if (y_up && starter.y < Topology::GetFieldYBorder() - 1u) {
            if (Topology::GetAccessibility({starter.x - 1u, starter.y + 1u})) {
                return {true, {starter.x - 1u, starter.y + 1u}};
            }
        } else if (!y_up && starter.y > 0u) {
            if (Topology::GetAccessibility({starter.x - 1u, starter.y - 1u})) {
                return {true, {starter.x - 1u, starter.y - 1u}};
            }
        }
    }
    return {false, {0, 0}};
}

BoolPoint CheckersTopology::FindInOneNeighbor(bool x_up, bool y_up, const Point& starter) const {
    if (x_up && starter.x < Topology::GetFieldXBorder() - 2u) {
        if (y_up && starter.y < Topology::GetFieldYBorder() - 2u) {
            if (!Topology::GetAccessibility({starter.x + 1u, starter.y + 1u}) &&
                Topology::GetAccessibility({starter.x + 2u, starter.y + 2u})) {
                return {true, {starter.x + 2u, starter.y + 2u}};
            }
        } else if (!y_up && starter.y > 1u) {
            if (!Topology::GetAccessibility({starter.x + 1u, starter.y - 1u}) &&
                Topology::GetAccessibility({starter.x + 2u, starter.y - 2u})) {
                return {true, {starter.x + 2u, starter.y - 2u}};
            }
        }
    } else if (!x_up && starter.x > 1u) {
        if (y_up && starter.y < Topology::GetFieldYBorder() - 2u) {
            if (!Topology::GetAccessibility({starter.x - 1u, starter.y + 1u}) &&
                Topology::GetAccessibility({starter.x - 2u, starter.y + 2u})) {
                return {true, {starter.x - 2u, starter.y + 2u}};
            }
        } else if (!y_up && starter.y > 1u) {
            if (!Topology::GetAccessibility({starter.x + 1u, starter.y - 1u}) &&
                Topology::GetAccessibility({starter.x + 2u, starter.y - 2u})) {
                return {true, {starter.x - 2u, starter.y - 2u}};
            }
        }
    }
    return {false, {0, 0}};
}
