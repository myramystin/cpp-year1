#include <catch.hpp>
#include <palindrome.h>

TEST_CASE("IsPalindrome") {
    REQUIRE(IsPalindrome("abba"));
    REQUIRE(IsPalindrome("ababa"));
    REQUIRE(IsPalindrome("argentina manit negra"));
    REQUIRE(!IsPalindrome("notpal"));
    REQUIRE(!IsPalindrome("qwerty"));
    REQUIRE(IsPalindrome("   "));
}
