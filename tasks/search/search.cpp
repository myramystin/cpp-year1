#include "search.h"

std::string_view::iterator NextSplit(std::string_view::iterator pos, std::string_view::iterator end) {
    auto it = pos;
    while (it < end && isalpha(*it)) {
        ++it;
    }
    return it;
}

bool AreLettersEqual(const char a, const char b) {
    if (a == b || tolower(a) == b || toupper(a) == b) {
        return true;
    }
    return false;
}

bool AreWordsEqual(const std::string_view& first, const std::string_view& second) {
    if (first.size() == second.size()) {
        for (size_t i = 0; i < first.size(); ++i) {
            if (!AreLettersEqual(first[i], second[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::vector<std::string_view> GetWords(const std::string_view& line, bool unique) {
    std::vector<std::string_view> result;
    auto it = line.begin();
    while (it < line.end()) {
        if (isalpha(*it)) {
            auto substr_end = NextSplit(it, line.end());
            std::string_view substr;
            if (it != substr_end) {
                substr = line.substr(it - line.begin(), substr_end - it);
            } else {
                substr = line.substr(it - line.begin(), line.end() - it);
            }
            if (unique) {
                bool to_add = true;
                for (const auto& word : result) {
                    if (AreWordsEqual(word, substr)) {
                        to_add = false;
                        break;
                    }
                }
                if (to_add) {
                    result.push_back(substr);
                }
            } else {
                result.push_back(substr);
            }
            it = substr_end + 1U;
        } else {
            ++it;
        }
    }
    return result;
}

bool CompareLines(const Line& first, const Line& second) {
    return (std::tie(second.tf_idf, first.position) < std::tie(first.tf_idf, second.position));
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

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    if (text.empty()) {
        return {};
    }
    size_t lines_in_text = std::count(text.begin(), text.end(), '\n') + 1;

    std::vector<Line> lines;
    lines.reserve(lines_in_text);
    std::unordered_map<std::string_view, size_t> words_from_query_in_text;
    auto begin_iter = text.begin();
    for (size_t i = 0; i < lines_in_text; ++i) {
        Line current_token;
        current_token.position = i;
        std::string_view::iterator end_iter;
        if (i == lines_in_text - 1) {
            end_iter = text.end();
        } else {
            end_iter = text.find('\n', begin_iter - text.begin()) + text.begin();
        }
        current_token.line = text.substr(begin_iter - text.begin(), end_iter - begin_iter);
        current_token.words = GetWords(current_token.line, false);
        current_token.words_count = current_token.words.size();
        if (current_token.words_count > 0) {
            lines.push_back(current_token);
        }
        begin_iter = end_iter + 1U;
    }
    auto words_from_query = GetWords(query, true);
    for (const auto& query_word : words_from_query) {
        size_t in_text_count = 0;
        for (auto& line : lines) {
            size_t in_line_count = 0;
            for (const auto& word : line.words) {
                in_line_count += AreWordsEqual(word, query_word);
            }
            if (in_line_count > 0) {
                ++in_text_count;
            }
            line.query_words[query_word] = in_line_count;
        }
        words_from_query_in_text[query_word] = in_text_count;
    }

    for (auto& line : lines) {
        for (const auto& query_word : words_from_query) {
            auto tf = GetTf(static_cast<long double>(line.query_words[query_word]),
                            static_cast<long double>(line.words_count));
            auto idf = GetIdf(static_cast<long double>(lines.size()),
                              static_cast<long double>(words_from_query_in_text[query_word]));
            line.tf_idf += tf * idf;
        }
    }
    std::sort(lines.begin(), lines.end(), CompareLines);
    size_t res_sz = std::min(results_count, lines.size());
    std::vector<std::string_view> result;
    for (size_t i = 0; i < res_sz; ++i) {
        if (lines[i].tf_idf == 0) {
            break;
        }
        result.push_back(lines[i].line);
    }
    return result;
}
