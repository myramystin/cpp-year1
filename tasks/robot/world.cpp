#include "world.h"

const Point& World::GetStart() const {
    return start_;
}

const Point& World::GetEnd() const {
    return end_;
}

const Point& World::GetCurrentPosition() const {
    return current_;
}

World::World(const Topology& topology, Point start, Point end)
    : topology_(topology), start_(start), end_(end), current_(start) {
}

std::unordered_map<Point, Topology::Distance> World::Lookup() const {
    std::unordered_map<Point, Topology::Distance> result;
    const auto& neighbors = topology_.GetNeighbours(current_);
    for (const auto& point : neighbors) {
        if (topology_.MeasureDistance(current_, point) == 1) {
            result[point] = topology_.MeasureDistance(point, end_);
        }
    }
    return result;
}

void World::Move(const Point& to) {
    if (topology_.MeasureDistance(current_, to) != 1) {
        throw IllegalMoveException();
    } else {
        current_ = to;
    }
}

Topology::Distance World::GetDistance(const Point& from, const Point& to) {
    return topology_.MeasureDistance(from, to);
}

std::vector<Point> World::GetValidNeighbors(const Point& from) {
    return topology_.GetValidNeighbors(from);
}
