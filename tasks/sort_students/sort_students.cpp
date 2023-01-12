#include "sort_students.h"

int CompareDates(Date first, Date second) {
    if (first.year < second.year) {
        return 1;
    } else if (first.year == second.year) {
        if (first.month < second.month) {
            return 1;
        } else if(first.month == second.month) {
            if (first.day < second.day) {
                return 1;
            } else if (first.day == second.day) {
                return 0;
            } else {
                return -1;
            }
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int CompareNames(Student first, Student second) {
    if (first.last_name < second.last_name) {
        return 1;
    } else if (first.last_name == second.last_name) {
        if (first.name < second.name) {
            return 1;
        } else if (first.name == second.name) {
            return 0;
        } else {
            return  -1;
        }
    } else {
        return -1;
    }
}
bool DateSortKindComparison(Student first, Student second) {
    int dates_compared = CompareDates(first.birth_date, second.birth_date);
    if (dates_compared == 1) {
        return true;
    } else if (dates_compared == 0) {
        int names_compared = CompareNames(first, second);
        if (names_compared == 1) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool NameSortKindComparison(Student first, Student second) {
    int names_compared = CompareNames(first, second);
    if (names_compared == 1) {
        return true;
    } else if (names_compared == 0) {
        int dates_compared = CompareDates(first.birth_date, second.birth_date);
        if (dates_compared == 1){
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Date) {
        std::sort(students.begin(), students.end(), DateSortKindComparison);
    } else {
        std::sort(students.begin(), students.end(), NameSortKindComparison);
    }
}
