#include "admission.h"

bool SortComparisonPeople(const Applicant* first, const Applicant* second) {
    if (first == nullptr || second == nullptr) {
        return false;
    }
    return (std::tie(second->points, first->student.name, first->student.birth_date.year,
                     first->student.birth_date.month, first->student.birth_date.day) <
            std::tie(first->points, second->student.name, second->student.birth_date.year,
                     second->student.birth_date.month, second->student.birth_date.day));
}

bool SortComparisonNames(const Student* first, const Student* second) {
    if (first == nullptr || second == nullptr) {
        return false;
    }
    return (std::tie(first->name, first->birth_date.year, first->birth_date.month, first->birth_date.day) <
            std::tie(second->name, second->birth_date.year, second->birth_date.month, second->birth_date.day));
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    AdmissionTable result_table;
    std::vector<const Applicant*> applicant_pointers(applicants.size());
    std::unordered_map<std::string, size_t> places_remnants;
    for (const auto& university : universities) {
        places_remnants[university.name] = university.max_students;
    }
    for (size_t i = 0; i < applicants.size(); ++i) {
        applicant_pointers[i] = &applicants[i];
    }
    std::sort(applicant_pointers.begin(), applicant_pointers.end(), SortComparisonPeople);
    for (auto applicant : applicant_pointers) {
        if (!applicant->wish_list.empty()) {
            for (const auto& university : applicant->wish_list) {
                auto& places_count = places_remnants[university];
                if (places_count > 0) {
                    places_count--;
                    result_table[university].push_back(&applicant->student);
                    break;
                }
            }
        }
    }
    for (auto& [university, students] : result_table) {
        std::sort(students.begin(), students.end(), SortComparisonNames);
    }
    return result_table;
}
