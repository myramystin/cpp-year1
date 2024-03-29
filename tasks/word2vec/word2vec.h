#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <cassert>

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors);
