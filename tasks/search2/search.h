#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct CaseInsentiveHash {
    std::size_t operator()(const std::string_view& s) const;
};

struct CaseInsentiveEqual {
    std::size_t operator()(const std::string_view& lhs, const std::string_view& rhs) const;
};

using SVtoSizetMap = std::unordered_map<std::string_view, size_t, CaseInsentiveHash, CaseInsentiveEqual>;
using SVtoSetMap =
    std::unordered_map<std::string_view, std::unordered_set<size_t>, CaseInsentiveHash, CaseInsentiveEqual>;

struct Document {
    std::string_view body;
    size_t position;
    SVtoSizetMap words_to_occurrencies;
    size_t words_amount;
    size_t size = 0;
};

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;

private:
    std::vector<Document> documents_;
    SVtoSetMap words_to_docs_;
    size_t docs_amount_;
};
