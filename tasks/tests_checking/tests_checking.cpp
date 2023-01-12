#include "tests_checking.h"

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<const std::string*> works;
    for (auto& action : student_actions) {
        if (action.side == Side::Top) {
            works.push_front(&action.name);
        } else {
            works.push_back(&action.name);
        }
    }
    std::vector<std::string> result;
    result.reserve(queries.size());
    for (auto& query : queries) {
        result.push_back(*works[query - 1]);
    }
    return result;
}
