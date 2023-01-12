#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    for (size_t x = 0; x < table.size(); ++x) {
        for (size_t y = 0; y < table.size(); ++y) {
            for (size_t z = 0; z < table.size(); ++z) {
                if (table[x][y] >= table.size() || table[y][z] >= table.size()) {
                    return false;
                }
                if (table[table[x][y]][z] != table[x][table[y][z]]) {
                    return false;
                }
            }
        }
    }
    return true;
}
