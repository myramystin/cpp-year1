#include "topology.h"
#include <algorithm>
#include <queue>

const Topology::Distance Topology::UNREACHABLE;

Topology::Field::Field(const Map& map) : map_(map) {
    if (map.empty()) {
        x_border_ = 0u;
        y_border_ = 0u;
    } else {
        x_border_ = map[0].size();
        y_border_ = map.size();
    };
}

size_t Topology::Field::GetXBorder() const {
    return x_border_;
}
size_t Topology::Field::GetYBorder() const {
    return y_border_;
}

bool Topology::Field::GetAccessibility(const Point& point) const {
    if (point.x >= x_border_ || point.y >= y_border_) {
        return false;
    }
    return !map_[point.y][point.x];
}

Topology::Distance Topology::MeasureDistance(const Point& from, const Point& to) const {
    if (!field_.GetAccessibility(to) || !field_.GetAccessibility(from)) {
        return Topology::UNREACHABLE;
    }

    if (from == to) {
        return 1u;
    }

    std::priority_queue<PriorityNode, std::vector<PriorityNode>> frontier;
    frontier.push({from, 0});
    std::unordered_map<Point, Topology::Distance> dist_so_far;
    dist_so_far[from] = 0u;
    bool success = false;
    Topology::Distance result = 1u;
    bool result_found = false;
    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();
        if (current.point == to) {
            success = true;
            break;
        }
        const auto neighbors = GetValidNeighbors(current.point);
        for (const auto& next : neighbors) {
            if (next == to) {
                success = true;
            }
            Topology::Distance new_dist = dist_so_far[current.point] + 1u;
            if (!dist_so_far.contains(next) || new_dist < dist_so_far[next]) {
                dist_so_far[next] = new_dist;
                size_t priority = new_dist;
                frontier.push({next, priority});
                if (next == to) {
                    if (result_found) {
                        result = std::min(result, new_dist);
                    } else {
                        result_found = true;
                        success = true;
                        result = new_dist;
                    }
                }
            }
        }
    }
    if (!success) {
        return Topology::UNREACHABLE;
    }
    return result;
}

Topology::Topology(const Map& map) : field_(map) {
}

bool Topology::GetAccessibility(const Point& point) const {
    return field_.GetAccessibility(point);
}

size_t Topology::GetFieldXBorder() const {
    return field_.GetXBorder();
}

size_t Topology::GetFieldYBorder() const {
    return field_.GetYBorder();
}

std::vector<Point> Topology::GetValidNeighbors(const Point& from) const {
    if (!GetAccessibility(from)) {
        return {};
    }
    std::vector<Point> result;
    const auto& all_neighbors = GetNeighbours(from);
    for (const auto& point : all_neighbors) {
        if (GetAccessibility(point)) {
            result.push_back(point);
        }
    }
    return result;
}

bool PriorityNode::operator<(const PriorityNode& other) const {
    return priority > other.priority;
}
