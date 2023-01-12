#include "scorer.h"

void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    events_by_students_[student_name][task_name].is_checked = false;
    auto task_it = events_by_students_[student_name].find(task_name);
    if (task_it != events_by_students_[student_name].end() && !task_it->second.request) {
        events_by_students_.erase(task_name);
        if (events_by_students_[student_name].empty()) {
            events_by_students_.erase(student_name);
        }
    }
}
void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    events_by_students_[student_name][task_name].is_checked = true;
};
void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    events_by_students_[student_name][task_name].request = true;
};
void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    if (events_by_students_.contains(student_name)) {
        auto task_it = events_by_students_[student_name].find(task_name);
        if (task_it != events_by_students_[student_name].end() && !task_it->second.is_checked) {
            events_by_students_.erase(task_name);
            if (events_by_students_[student_name].empty()) {
                events_by_students_.erase(student_name);
            }
        } else {
            events_by_students_[student_name][task_name].request = false;
        }
    } else {
        events_by_students_[student_name][task_name].request = false;
    }
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable score_table;
    for (const auto &[student_name, tasks] : events_by_students_) {
        for (const auto &[task_name, task_status] : tasks) {
            if (task_status.request && task_status.is_checked) {
                score_table[student_name].insert(task_name);
            }
        }
    }

    return score_table;
}

void Scorer::Reset() {
    events_by_students_.clear();
}
