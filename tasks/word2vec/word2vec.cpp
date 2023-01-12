#include "word2vec.h"

int64_t CountScalar(const std::vector<int>& first, const std::vector<int>& second) {
    assert(first.size() == second.size());
    int64_t result = 0;
    for (size_t i = 0; i < first.size(); ++i) {
        result += static_cast<int64_t>(first[i]) * static_cast<int64_t>(second[i]);
    }
    return result;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    if (words.empty() || words.size() == 1) {
        return {};
    }
    std::vector<int64_t> scalars;
    scalars.reserve(words.size() - 1U);
    for (size_t i = 1U; i < words.size(); ++i) {
        scalars.push_back(CountScalar(vectors.front(), vectors[i]));
    }
    size_t max_scalar_index = max_element(scalars.begin(), scalars.end()) - scalars.begin();
    std::vector<std::string> result;
    for (size_t i = 0U; i < scalars.size(); ++i) {
        if (scalars[max_scalar_index] == scalars[i]) {
            result.push_back(words[i + 1U]);
        }
    }

    return result;
}
