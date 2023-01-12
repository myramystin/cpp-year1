#include "unixpath.h"

std::vector<std::string_view> GetDir(std::string_view& path) {
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

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string_view> current_dir;
    if (!path.empty() && path[0] != '/') {
        current_dir = GetDir(current_working_dir);
    }
    std::vector<std::string_view> commands = GetDir(path);
    for (auto& command : commands) {
        if (command == "..") {
            if (!current_dir.empty()) {
                current_dir.pop_back();
            }
        } else if (command != "." && command != "/") {
            current_dir.push_back(command);
        }
    }
    std::string result;
    for (auto c : current_dir) {
        result += "/";
        result += static_cast<std::string>(c);
    }
    while (!result.empty() && result[result.size() - 1] == '/') {
        result.pop_back();
    }
    if (result.empty()) {
        result = "/";
    }
    return result;
}