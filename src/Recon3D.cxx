#include "Recon3D.hxx"


std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Recon3D::Match(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
    ){

  std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> matched_track;

  std::unordered_map<int, std::pair<Float_t, Float_t,>> recon_ZX_p;
  std::unordered_map<int, std::pair<Float_t, Float_t,>> recon_ZY_p;

  for(const auto& [key, hits] : recon_ZX){
    recon_ZX_p[key] = GetStartEnd(pair_to_vec(hits));
  }

  for(const auto& [key, hits] : recon_ZY){
    recon_ZY_p[key] = GetStartEnd(pair_to_vec(hits));
  }

  for(const auto& [key_ZX, v_ZX] : recon_ZX_p){
    for(const auto& [key_ZY, v_ZY] : recon_ZY_p){
      if(std::fabs(v_ZX.first - v_ZY.first) > 15.){continue;}
      if(std::fabs(v_ZX.second - v_ZY.second) > 15.){continue;}
      matched_track.push_back(std::make_pair(pair_to_vec(recon_ZY[key_ZY]), pair_to_vec(recon_ZX[key_ZX])));
    }
  }

  return matched_track;

}

std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Recon3D::Match_w_start(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
    ){
  std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> matched_track;
  return matched_track;
}

std::pair<Float_t, Float_t> Recon3D::GetStartEnd(const std::vector<two_vector>& track){
  Float_t start_point = 100000;
  Float_t end_point = -100000;
  for(const auto& v : track){
    if(start_point > v.x){start_point = v.x;}
    if(end_point < v.x){end_point = v.x;}
  }
  return std::make_pair(start_point, end_point);
}

std::vector<two_vector> Recon3D::pair_to_vec(const std::vector<std::pair<int, two_vector>>& pv){
  std::vector<two_vector> vec;
  for(const auto& v : pv){
    vec.push_back(v.second);
  }
  return vec;
}
