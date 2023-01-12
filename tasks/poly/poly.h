#pragma once
#include <vector>
#include <unordered_map>
#include <string>

class Poly {
public:
    Poly() = default;
    Poly(const std::vector<int64_t>& coefs);
    Poly(const std::vector<std::pair<size_t, int64_t>>& coefs);

    int64_t operator()(const int64_t x) const;

    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;

    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;
    Poly operator*(const Poly& other) const;

    Poly operator-() const;

    Poly& operator+=(const Poly& other);
    Poly& operator-=(const Poly& other);
    Poly& operator*=(const Poly& other);

    friend std::ostream& operator<<(std::ostream& stream, const Poly& poly);

private:
    std::unordered_map<size_t, int64_t> coefs_;
};
