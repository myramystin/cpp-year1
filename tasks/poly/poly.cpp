#include "poly.h"
#include <ostream>

int64_t FastPower(int64_t number, size_t power) {
    if (power == 0) {
        return 1;
    }
    if (power % 2 == 0) {
        return FastPower(number * number, static_cast<size_t>(power / 2));
    }
    return number * FastPower(number, power - 1U);
}

Poly::Poly(const std::vector<int64_t>& coefs) {
    for (size_t i = 0; i < coefs.size(); ++i) {
        if (coefs[i] != 0) {
            coefs_.insert({i, coefs[i]});
        }
    }
}

Poly::Poly(const std::vector<std::pair<size_t, int64_t>>& coefs) {
    for (const auto& [pos, val] : coefs) {
        if (val != 0) {
            coefs_.insert({pos, val});
        }
    }
}

int64_t Poly::operator()(const int64_t x) const {
    int64_t result = 0;
    for (const auto& [pos, val] : coefs_) {
        result += FastPower(x, pos) * val;
    }
    return result;
}

bool Poly::operator==(const Poly& other) const {
    if (other.coefs_.size() != coefs_.size()) {
        return false;
    }
    for (const auto& [pos, val] : other.coefs_) {
        auto iter = coefs_.find(pos);
        if (iter == coefs_.end() || iter->second != val) {

            return false;
        }
    }
    return true;
}

bool Poly::operator!=(const Poly& other) const {
    return !(*this == other);
}

Poly Poly::operator-(const Poly& other) const {
    auto first = *this;
    first -= other;
    return first;
}

Poly Poly::operator+(const Poly& other) const {
    auto first = *this;
    first += other;
    return first;
}

Poly& Poly::operator-=(const Poly& other) {
    for (const auto& [pos, val] : other.coefs_) {
        if (!coefs_.contains(pos)) {
            coefs_.insert({pos, -val});
        } else {
            if (coefs_.at(pos) != val) {
                coefs_[pos] -= val;
            } else {
                coefs_.erase(pos);
            }
        }
    }
    return *this;
}

Poly& Poly::operator+=(const Poly& other) {
    for (const auto& [pos, val] : other.coefs_) {
        if (!coefs_.contains(pos)) {
            coefs_.insert({pos, val});
        } else {
            if (coefs_.at(pos) != -val) {
                coefs_[pos] += val;
            } else {
                coefs_.erase(pos);
            }
        }
    }
    return *this;
}

void MergeCoefs(std::unordered_map<size_t, int64_t>& base, const std::unordered_map<size_t, int64_t>& to_add) {

    for (const auto& [pos, val] : to_add) {
        base[pos] += val;
    }
}

std::unordered_map<size_t, int64_t> MulCoefs(const std::unordered_map<size_t, int64_t>& coefs, size_t degree,
                                             int64_t coef) {

    std::unordered_map<size_t, int64_t> result;
    for (const auto& [pos, val] : coefs) {
        result.insert({pos + degree, val * coef});
    }
    return result;
}

Poly Poly::operator*(const Poly& other) const {
    auto first = *this;
    first *= other;
    return first;
}

Poly Poly::operator-() const {
    std::vector<std::pair<size_t, int64_t>> result_coefs;
    for (const auto& [pos, val] : coefs_) {
        result_coefs.push_back({pos, -val});
    }
    return Poly(result_coefs);
}

bool ComparePairs(const std::pair<size_t, int64_t>& lhs, const std::pair<size_t, int64_t>& rhs) {
    return lhs.first > rhs.first;
}

std::ostream& operator<<(std::ostream& stream, const Poly& poly) {
    if (poly.coefs_.empty()) {
        stream << "y = 0";
        return stream;
    }
    std::vector<std::pair<size_t, int64_t>> sorted_coefs(poly.coefs_.begin(), poly.coefs_.end());
    std::sort(sorted_coefs.begin(), sorted_coefs.end(), ComparePairs);
    size_t iter = 0;
    stream << "y = ";
    for (const auto& [pos, val] : sorted_coefs) {
        ++iter;
        stream << std::to_string(val);
        if (pos != 0) {
            stream << "x";
            if (pos != 1) {
                stream << "^";
                stream << std::to_string(pos);
            }
        }
        if (iter != sorted_coefs.size()) {
            stream << " + ";
        }
    }
    return stream;
}

Poly& Poly::operator*=(const Poly& other) {
    if (other.coefs_.empty() || coefs_.empty()) {

        coefs_.clear();
        return *this;
    }
    auto coefs_copy = coefs_;
    coefs_.clear();
    for (const auto& [pos, val] : other.coefs_) {
        MergeCoefs(coefs_, MulCoefs(coefs_copy, pos, val));
    }
    return *this;
}