#include "Recon3D.hxx"


std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Recon3D::Match(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
    ){

  std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> matched_track;

  std::unordered_map<int, std::pair<Float_t, Float_t>> recon_ZX_p;
  std::unordered_map<int, std::pair<Float_t, Float_t>> recon_ZY_p;

  for(const auto& [key, hits] : recon_ZX){
    recon_ZX_p[key] = GetStartEnd(pair_to_vec(hits));
  }

  for(const auto& [key, hits] : recon_ZY){
    recon_ZY_p[key] = GetStartEnd(pair_to_vec(hits));
  }

  for(const auto& [key_ZX, v_ZX] : recon_ZX_p){
    for(const auto& [key_ZY, v_ZY] : recon_ZY_p){
      if(std::fabs(v_ZX.first - v_ZY.first) > fMatchingLength){continue;}
      if(std::fabs(v_ZX.second - v_ZY.second) > fMatchingLength){continue;}
      if(recon_ZY.at(key_ZY).size()<2){continue;}
      if(recon_ZX.at(key_ZX).size()<2){continue;}
      matched_track.push_back(std::make_pair(pair_to_vec(recon_ZY.at(key_ZY)), pair_to_vec(recon_ZX.at(key_ZX))));
    }
  }

  return matched_track;

}

std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Recon3D::Recon(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
    ){

  //return
  std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> matched_track;

  std::unordered_map<int, std::vector<two_vector>> recon_remain_ZX;
  std::unordered_map<int, std::vector<two_vector>> recon_remain_ZY;

  //setting remaining recon tracks (simple and not const)
  for(const auto& [key, hits] : recon_ZX){
    recon_remain_ZX[key] = pair_to_vec(hits);
  }
  for(const auto& [key, hits] : recon_ZY){
    recon_remain_ZY[key] = pair_to_vec(hits);
  }

  std::unordered_map<int, std::unordered_map<int, Int_t>> matching_score_map;
  std::pair<int, int> max_matching_track;
  while(true){
    matching_score_map = CalMatchingScoreMap(recon_remain_ZX, recon_remain_ZY);
    if(!CalMaxScore(matching_score_map, max_matching_track)){break;}
    matched_track.push_back(MakeTrack(max_matching_track, recon_remain_ZX, recon_remain_ZY));
  }

  return matched_track;
}

std::unordered_map<int, std::unordered_map<int, Int_t>> Recon3D::CalMatchingScoreMap(
    const std::unordered_map<int, std::vector<two_vector>>& ZX, std::unordered_map<int, std::vector<two_vector>>& ZY
    ){

  std::unordered_map<int, std::unordered_map<int, Int_t>> matching_score_map;

  for(const auto& [keyZX, hitsZX] : ZX){
    for(const auto& [keyZY, hitsZY] : ZY){
      matching_score_map[keyZX][key_ZY] = CalOverlapNum(hitsZX, hitsZY);
    }
  }

  return matching_score_map;

}

Int_t Recon3D::CalOverlapNum(
    const std::vector<two_vector>& a, const std::vector<two_vector>& b
    ){
  Int_t score = 0;
  for(const auto& hita : a){
    for(const auto& hitb : b){
      if(std::fabs(a.x - b.x) < 1){score++;}
    }
  }
  return score;
}

bool Recon3D::CalMaxScore(
    const std::unordered_map<int, std::unordered_map<int, Int_t>>& score_map, std::pair<int, int>& received_track
    ){
  int max_score = 0;
  for(const auto& [keya, map] : score_map){
    for(const auto& [keyb, score] : map){
      if(score > max_score){
        max_score = score;
        received_track = std::make_pair(keya, keyb);
      }
    }
  }
  if(max_score < fMatchingOverlapThreshold){return false;}
  else{return true;}
}

std::pair<std::vector<two_vector>, std::vector<two_vector>> Recon3D::MakeTrack(
    const std::pair<int, int>& matchingID, std::unordered_map<int, std::vector<two_vector>>& ZX, std::unordered_map<int, std::vector<two_vector>>& ZY
    ){

  std::vector<two_vector> madeZX;
  std::vector<two_vector> madeZY;


  for(const auto& hitZX : ZX.at(matchingID.first)){
    for(const auto& hitZY : ZY.at(matchingID.second)){
      if(std::fabs(hitZX.x - hitZY.x) < 1){
        madeZX.push_back(ZX.at(matchingID.first));
        madeZY.push_back(ZY.at(matchingID.second));
      }
    }
  }

  //create new remaining track
  std::vector<two_vector> new_remainTrackZXfirst;
  std::vector<two_vector> new_remainTrackZXsecond;
  std::vector<two_vector> new_remainTrackZYfirst;
  std::vector<two_vector> new_remainTrackZYsecond;
  ExtractRemainTrack(madeZX, ZX.at(matchingID.first), new_remainTrackZXfirst, new_remainTrackZXsecond);
  ExtractRemainTrack(madeZY, ZY.at(matchingID.second), new_remainTrackZYfirst, new_remainTrackZYsecond);

  //delete matched track
  madeZX.erase(matchingID.first);
  madeZY.erase(matchingID.second);
  //push new remain tracks
  if(!new_remainTrackZXfirst.empty()){ZX.emplace(GetUniqueKey(ZX), new_remainTrackZXfirst);}
  if(!new_remainTrackZXsecond.empty()){ZX.emplace(GetUniqueKey(ZX), new_remainTrackZXsecond);}
  if(!new_remainTrackZYfirst.empty()){ZY.emplace(GetUniqueKey(ZY), new_remainTrackZYfirst);}
  if(!new_remainTrackZYsecond.empty()){ZY.emplace(GetUniqueKey(ZY), new_remainTrackZYsecond);}


  return std::make_pair(madeZX, madeZY);
}

void Recon3D::ExtractRemainTrack(
    const std::vector<two_vector>& ref_track, const std::vector<two_vector>& old_remain,
    std::vector<two_vector>& remain_first, std::vector<two_vector>& remain_second
    ){

  bool for_first_track = true;

  for(const auto& old : old_remain){
    bool used_f = false;
    for(const auto& : ref : ref_track){
      if(std::fabs(old.x - ref.x) < 1){used_f = true;}
    }
    if(used_f == false){
      if(for_first_track == true){
        if(remain_first.empty()){remain_first.push_back(old);}
        else{
          if(std::fabs(remain_first.back().x - old.x) < 25){remain_first.push_back(old);}
          else{
            for_first_track = false;
            remain_second.push_back(old);
          }
        }
      }
      else{
        remain_second.push_back(old);
      }
    }
  }
}

int Recon3D::GetUniqueKey(const std::unordered_map<int, std::vector<two_vector>>& map){
  int key = 0;
  while(map.count(key)){
    ++key;
  }
  return key;
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
