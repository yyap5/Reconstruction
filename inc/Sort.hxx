#ifndef SORT_HXX
#define SORT_HXX

#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include "DataTypes.hxx"

class Sort{
  public:

    Sort() = default;

    std::vector<std::vector<std::pair<int, two_vector>>> ForwardSort(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits);

    std::vector<std::vector<std::pair<int, two_vector>>> BackwardSort(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits);

};
#endif
