#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    if (str.empty()) {
        return true;
    }
    size_t left = 0;
    size_t right = str.size() - 1;
    while (right > left) {
        while (std::isspace(str[left]) && left < str.size() - 1) {
            ++left;
        }
        while (std::isspace(str[right]) && right > 0) {
            --right;
        }
        if (str[right] != str[left]) {
            return false;
        }
        if (right == 0) {
            break;
        }
        ++left;
        --right;
    }
    return true;
}
