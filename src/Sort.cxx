#include"Sort.hxx"

std::vector<std::vector<std::pair<int, two_vector>>> Sort::ForwardSort(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits_by_module){

  std::vector<int> keys;
  std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits;

  for(const auto& [key, hits] : hits_by_module){
    keys.push_back(key);
  }
  std::sort(keys.begin(), keys.end());
  for(const auto& key : keys){
    sorted_hits.push_back(hits_by_module.at(key));
  }

  return sorted_hits;
}

std::vector<std::vector<std::pair<int, two_vector>>> Sort::BackwardSort(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits_by_module){

  std::vector<int> keys;
  std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits;

  for(const auto& [key, hits] : hits_by_module){
    keys.push_back(key);
  }
  std::sort(keys.begin(), keys.end(), std::greater<int>());
  for(const auto& key : keys){
    sorted_hits.push_back(hits_by_module.at(key));
  }

  return sorted_hits;
}
