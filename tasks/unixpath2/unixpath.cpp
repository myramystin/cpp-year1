#include "unixpath.h"

std::vector<std::string_view> ParseDir(std::string_view& path) {
    std::vector<std::string_view> vector_from_path;
    size_t pos = 0;
    auto substr_end = path.find('/');
    while (substr_end != path.npos) {
        if (substr_end - pos != 0) {
            vector_from_path.push_back(path.substr(pos, substr_end - pos));
        }
        pos = substr_end + 1U;
        substr_end = path.find('/', pos);
    }
    vector_from_path.push_back(path.substr(pos, substr_end - pos));
    return vector_from_path;
}

std::string FormPath(const std::vector<std::string>& dir) {
    std::string result;
    for (const auto& c : dir) {
        result += "/";
        result += c;
    }
    while (!result.empty() && result[result.size() - 1] == '/') {
        result.pop_back();
    }
    if (result.empty()) {
        result = "/";
    }
    return result;
}

void NormalizePathVec(std::string_view dir, std::vector<std::string>& result) {
    auto parsed_dir = ParseDir(dir);
    for (const auto& command : parsed_dir) {
        if (command == "..") {
            if (!result.empty()) {
                result.pop_back();
            }
        } else if (command != "." && command != "/") {
            result.push_back(static_cast<std::string>(command));
        }
    }
}

UnixPath::UnixPath(std::string_view initial_dir) {
    NormalizePathVec(initial_dir, initial_);
    current_dir_ = initial_;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    if (!path.empty() && path[0] == '/') {
        current_dir_.clear();
    }

    NormalizePathVec(path, current_dir_);
}

std::string UnixPath::GetAbsolutePath() const {
    return FormPath(current_dir_);
}

size_t UnixPath::CommonPartsCount() const {
    auto min_size = std::min(initial_.size(), current_dir_.size());
    size_t count = 0;
    std::string result;
    while (count < min_size && initial_[count] == current_dir_[count]) {
        ++count;
    }
    return count;
}

std::string UnixPath::GetRelativePath() const {
    auto min_size = std::min(initial_.size(), current_dir_.size());
    size_t current_pos = CommonPartsCount();
    std::string result;
    size_t current_pos_copy = current_pos;
    if (current_pos == initial_.size()) {
        result = ".";
    }
    for (; current_pos < initial_.size(); ++current_pos) {
        if (current_pos != min_size && current_pos != 0) {
            result += '/';
        }
        result += "..";
    }
    for (; current_pos_copy < current_dir_.size(); ++current_pos_copy) {
        result += '/';
        result += current_dir_[current_pos_copy];
    }

    return result;
}
