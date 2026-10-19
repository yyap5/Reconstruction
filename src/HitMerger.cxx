#include "HitMerger.hxx"

Float_t HitMerger::ChargeMean(
    const std::vector<std::pair<Float_t, Float_t>>& layer_hits
    ){
  //<charge, pos.y>
  Float_t sum_charge = 0;
  Float_t charge_hit = 0;
  for(const auto& v : layer_hits){
    sum_charge += v.first;
    charge_hit += v.first * v.second;
  }
  return charge_hit / sum_charge;
}

std::vector<std::pair<int, two_vector>> HitMerger::Merge(
    const std::vector<std::pair<Int_t, charge_two_vector>>& v
    ){
  std::vector<std::pair<int, two_vector>> m_hits;
  std::vector<bool> used(v.size(), false);

  for(size_t i = 0; i < v.size(); i++){

    if(used[i]) continue;

    std::vector<std::pair<Float_t, Float_t>> layer_hits;
    std::vector<Int_t> merged_fiber;

    // initial fiber
    merged_fiber.push_back(v.at(i).first);
    used[i] = true;

    layer_hits.push_back(
        std::make_pair(
          v.at(i).second.cha,
          v.at(i).second.y
          )
        );

    //look for near fibers
    size_t current = 0;

    while(current < merged_fiber.size()){

      Int_t fiber = merged_fiber.at(current);

      for(size_t k = 0; k < v.size(); k++){

        if(used[k]) continue;

        if(std::abs(fiber - v.at(k).first) <= 1){

          merged_fiber.push_back(v.at(k).first);
          used[k] = true;

          layer_hits.push_back(
              std::make_pair(
                v.at(k).second.cha,
                v.at(k).second.y
                )
              );
        }
      }
      current++;
    }

    m_hits.push_back(std::make_pair(layer_hits.size(), two_vector{v.at(i).second.x, ChargeMean(layer_hits)}));
  }

  return m_hits;

}
