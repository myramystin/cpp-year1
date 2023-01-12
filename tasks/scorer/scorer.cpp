#include "scorer.h"

bool CompareEvents(const Event* first, const Event* second) {
    if (first == nullptr || second == nullptr) {
        return false;
    }
    return std::tie(first->time, first->event_type) < std::tie(second->time, second->event_type);
}

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::map<StudentName, std::vector<const Event*>> events_by_students;
    for (const auto& event : events) {
        if (event.time <= score_time) {
            events_by_students[event.student_name].push_back(&event);
        }
    }
    for (auto& [student, events_vec] : events_by_students) {
        std::sort(events_vec.begin(), events_vec.end(), CompareEvents);
    }
    ScoreTable result_table;
    for (auto& [student, events_vec] : events_by_students) {
        std::map<TaskName, std::map<std::string, bool>> tasks_status;
        for (auto& event : events_vec) {
            if (event->event_type == EventType::CheckSuccess || event->event_type == EventType::CheckFailed) {
                tasks_status[event->task_name]["CI"] = (event->event_type == EventType::CheckSuccess);
            } else {
                tasks_status[event->task_name]["Merge"] = (event->event_type == EventType::MergeRequestClosed);
            }
        }
        for (auto [task, status] : tasks_status) {
            if (status.find("CI") != status.end() && status["CI"] &&
                (status.find("Merge") == status.end() || status["Merge"])) {
                (void)result_table[student].insert(task);
            }
        }
    }
    return result_table;
}
