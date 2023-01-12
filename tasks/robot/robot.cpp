#include "robot.h"
#include <queue>

robot::Path robot::FindPath(World& world) {
    // https://habr.com/ru/post/331192/ -- algorithm taken from here
    const auto& start = world.GetCurrentPosition();
    const auto& end = world.GetEnd();

    if (start == end) {
        return {start};
    }

    std::priority_queue<PriorityNode, std::vector<PriorityNode>> frontier;
    std::unordered_map<Point, Point> came_from;
    std::unordered_map<Point, size_t> dist_so_far;

    frontier.push({start, 0u});
    came_from[start] = {0, 0};
    dist_so_far[start] = 0u;
    bool success = false;

    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();
        if (current.point == end) {
            success = true;
            break;
        }
        const auto& neighbors = world.GetValidNeighbors(current.point);
        for (const auto& next : neighbors) {
            auto new_dist = dist_so_far[current.point] + 1u;
            if (!dist_so_far.contains(next) || new_dist < dist_so_far[next]) {
                dist_so_far[next] = new_dist;
                auto priority = new_dist + world.GetDistance(end, next);
                frontier.push({next, priority});
                came_from[next] = current.point;
            }
        }
    }
    if (!success) {
        return {};
    }
    robot::Path result;
    auto current = end;
    while (current != start) {
        result.push_back(current);
        current = came_from[current];
    }
    result.push_back(current);
    std::reverse(result.begin(), result.end());
    return result;
}
