#include "password.h"

bool ValidatePassword(const std::string& password) {
    const int MIN_PASSWORD_SIZE = 8;
    const int MAX_PASSWORD_SIZE = 14;
    bool upper = false;
    bool lower = false;
    bool digits = false;
    bool other = false;

    if (password.size() > MAX_PASSWORD_SIZE || password.size() < MIN_PASSWORD_SIZE) {
        return false;
    }
    for (int i = 0; i < password.size(); i++) {
        if (static_cast<int>(password[i]) < 33 || static_cast<int>(password[i]) > 126) {
            return false;
        }
        if (std::isupper(static_cast<int>(password[i]))) {
            upper = true;
        } else if (std::islower(static_cast<int>(password[i]))) {
            lower = true;
        } else if (std::isalnum(static_cast<int>(password[i]))) {
            digits = true;
        } else {
            other = true;
        }
    }
    return upper + lower + other + digits >= 3;
}
