#include <catch.hpp>
#include <word2vec.h>

#include <vector>

TEST_CASE("FindClosestWords") {
    const std::vector<std::string> words = {"sweden", "queen", "norway", "king"};

    const std::vector<std::vector<int>> vectors = {{4, 6, 3}, {4, 7, 5}, {4, 7, 5}, {-1, 3, 2}};

    const std::vector<std::string> expected = {"queen", "norway"};

    REQUIRE(expected == FindClosestWords(words, vectors));
}
