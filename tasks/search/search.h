#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include "math.h"
#include <string>

struct Line {
    std::unordered_map<std::string_view, size_t> query_words;
    long double tf_idf = 0;
    size_t words_count;
    size_t position;
    std::vector<std::string_view> words;
    std::string_view line;
};

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count);