#include "search.h"
#include <cmath>
#include <string>
#include <unordered_set>

const size_t HASHER_X = 29U;
const size_t HASHER_M = 2147483647U;

std::string_view::iterator NextSplit(std::string_view::iterator pos, std::string_view::iterator end) {
    auto it = pos;
    while (it < end && isalpha(*it)) {
        ++it;
    }
    return it;
}

size_t Hasher(const std::string_view& word) {
    size_t result = 0;
    for (auto it = word.begin(); it < word.end(); ++it) {
        result = (result * HASHER_X + tolower(*it)) % HASHER_M;
    }
    return result % HASHER_M;
}

std::size_t CaseInsentiveHash::operator()(const std::string_view& s) const {
    return Hasher(s);
}

std::size_t CaseInsentiveEqual::operator()(const std::string_view& lhs, const std::string_view& rhs) const {
    if (rhs.size() != lhs.size()) {
        return false;
    }
    for (size_t i = 0U; i < lhs.size(); ++i) {
        if (tolower(lhs[i]) != tolower(rhs[i])) {
            return false;
        }
    }
    return true;
}

SVtoSizetMap GetWords(const std::string_view& line) {
    SVtoSizetMap result;
    auto substr_begin = line.begin();
    while (substr_begin < line.end()) {
        if (isalpha(*substr_begin)) {
            auto substr_end = NextSplit(substr_begin, line.end());
            std::string_view substr;
            if (substr_begin != substr_end) {
                substr = line.substr(substr_begin - line.begin(), substr_end - substr_begin);
            } else {
                substr = line.substr(substr_begin - line.begin(), line.end() - substr_begin);
            }
            auto find = result.find(substr);
            if (find != result.end()) {
                ++result[substr];
            } else {
                result.insert({substr, 1});
            }

            substr_begin = substr_end + 1U;
        } else {
            ++substr_begin;
        }
    }
    return result;
}

long double GetTf(long double occurencies_in_document, long double words_in_document) {
    if (words_in_document != 0) {
        return occurencies_in_document / words_in_document;
    }
    return 0;
}

long double GetIdf(long double docs_number, long double docs_with_word) {
    if (docs_number > 0 && docs_with_word > 0) {
        return log(docs_number / docs_with_word);
    }
    return 0;
}

void SearchEngine::BuildIndex(std::string_view text) {
    documents_.clear();
    size_t lines_in_text = std::count(text.begin(), text.end(), '\n') + 1;
    auto begin_iter = text.begin();

    for (size_t i = 0; i < lines_in_text; ++i) {
        std::string_view::iterator end_iter;
        Document current_doc;
        if (i == lines_in_text - 1) {
            end_iter = text.end();
        } else {
            end_iter = text.find('\n', begin_iter - text.begin()) + text.begin();
        }
        current_doc.body = text.substr(begin_iter - text.begin(), end_iter - begin_iter);
        current_doc.position = i;
        current_doc.words_to_occurrencies = GetWords(current_doc.body);
        for (const auto& [word, occurs] : current_doc.words_to_occurrencies) {
            words_to_docs_[word].insert(i);
            current_doc.size += occurs;
        }
        if (!current_doc.words_to_occurrencies.empty()) {
            documents_.push_back(current_doc);
        }

        begin_iter = end_iter + 1U;
    }
    docs_amount_ = documents_.size();
}

bool CompareDocs(std::pair<long double, const Document*> first, std::pair<long double, const Document*> second) {
    return (std::tie(second.first, first.second->position) < std::tie(first.first, second.second->position));
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    if (docs_amount_ == 1) {
        return {documents_[0].body};
    }
    auto query_words = GetWords(query);
    std::unordered_set<size_t> relatable_docs_nums;
    for (const auto& [word, occurs] : query_words) {
        auto in_which_docs = words_to_docs_.find(word);
        if (in_which_docs != words_to_docs_.end()) {
            for (const auto& pos : in_which_docs->second) {
                relatable_docs_nums.insert(pos);
            }
        }
    }
    std::vector<std::pair<long double, const Document*>> tf_idf_to_docs;
    for (const auto& doc_num : relatable_docs_nums) {
        const auto& document = documents_[doc_num];
        long double tf_idf = 0;
        for (const auto& [word, occurs] : query_words) {
            size_t word_occurs = 0U;
            auto find_occurs = document.words_to_occurrencies.find(word);
            if (find_occurs != document.words_to_occurrencies.end()) {
                word_occurs = find_occurs->second;
            }
            auto tf = GetTf(word_occurs, document.size);
            size_t word_occurs_in_text = 0U;
            auto find_occurs_in_text = words_to_docs_.find(word);
            if (find_occurs_in_text != words_to_docs_.end()) {
                word_occurs_in_text = find_occurs_in_text->second.size();
            }
            auto idf = GetIdf(docs_amount_, word_occurs_in_text);
            tf_idf += tf * idf;
        }
        if (tf_idf != 0) {
            tf_idf_to_docs.push_back({tf_idf, &documents_[doc_num]});
        }
    }
    auto ans_size = std::min(results_count, tf_idf_to_docs.size());
    std::nth_element(tf_idf_to_docs.begin(), tf_idf_to_docs.begin() + ans_size, tf_idf_to_docs.end(), CompareDocs);
    std::vector<std::string_view> result;
    result.reserve(ans_size);
    for (size_t i = 0; i < ans_size; ++i) {
        result.push_back(tf_idf_to_docs[i].second->body);
    }
    return result;
}
