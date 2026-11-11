#include "VertexRecon.hxx"

three_vector VertexRecon::GetVertex_minz(
    const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track){

  three_vector vertex;
  //find the longest track
  Float_t max_track_len = 0;
  int trackID = -1; //the longest track ID
  for(size_t i=0; i < matched_track.size(); i++){
    Float_t y_len = CalXYlen(matched_track.at(i).first);
    Float_t x_len = CalXYlen(matched_track.at(i).second);
    Float_t z_len = CalZlen(matched_track.at(i).first);
    if(z_len < CalZlen(matched_track.at(i).second)){
      z_len = CalZlen(matched_track.at(i).second);
    }
    Float_t track_len = std::sqrt(y_len*y_len + x_len*x_len + z_len*z_len);
    if(max_track_len < track_len){
      max_track_len = track_len;
      trackID = i;
    }
  }
  if(trackID == -1){
    std::cout << "there are no reconstructed track" << std::endl;
  }
  //cal vertex z from selected track
  two_vector minhitZY = GetMinZhit(matched_track.at(trackID).first);
  two_vector minhitZX = GetMinZhit(matched_track.at(trackID).second);

  vertex.z = std::min(minhitZY.x, minhitZX.x);
  vertex.x = minhitZX.y;
  vertex.y = minhitZY.y;

  return vertex;
}

three_vector VertexRecon::GetVertex_uDeltaRay(
    const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track){

  three_vector vertex;
  //find the longest track
  Float_t max_track_len = 0;
  int trackID = -1; //the longest track ID
  for(size_t i=0; i < matched_track.size(); i++){
    Float_t y_len = CalXYlen(matched_track.at(i).first);
    Float_t x_len = CalXYlen(matched_track.at(i).second);
    Float_t z_len = CalZlen(matched_track.at(i).first);
    if(z_len < CalZlen(matched_track.at(i).second)){//choose longer z len
      z_len = CalZlen(matched_track.at(i).second);
    }
    Float_t track_len = std::sqrt(y_len*y_len + x_len*x_len + z_len*z_len);
    if(max_track_len < track_len){
      max_track_len = track_len;
      trackID = i;
    }
    if(trackID == -1){
      std::cout << "there are no reconstructed track" << std::endl;
    }
  }

  int DeltaRayScore = GetDeltaRayScore(matched_track, trackID);
  //debug
  std::cout << "DeltaRayScore : " << DeltaRayScore << std::endl;
  if(DeltaRayScore>=0){
    two_vector minhitZY = GetMinZhit(matched_track.at(trackID).first);
    two_vector minhitZX = GetMinZhit(matched_track.at(trackID).second);
    vertex.z = std::min(minhitZY.x, minhitZX.x);
    vertex.x = minhitZX.y;
    vertex.y = minhitZY.y;
  }
  if(DeltaRayScore<0){
    two_vector maxhitZY = GetMaxZhit(matched_track.at(trackID).first);
    two_vector maxhitZX = GetMaxZhit(matched_track.at(trackID).second);
    vertex.z = std::max(maxhitZY.x, maxhitZX.x);
    vertex.x = maxhitZX.y;
    vertex.y = maxhitZY.y;
  }

  return vertex;
}

int VertexRecon::GetDeltaRayScore(
    const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track,
    const int& trackID){

  int DeltaRayScore = 0;

  //search for delta ray candidate
  for(size_t i=0; i<matched_track.size(); i++){
    if(i==trackID){continue;}
    std::vector<Float_t> zy_len_vec;
    std::vector<Float_t> zx_len_vec;
    Float_t front_score = 10000;
    Float_t back_score = 10000;
    //for front point of the candidate
    for(const auto& hit :  matched_track.at(trackID).first){
      Float_t len_zy = GetDistance2D(hit, matched_track.at(i).first.front());
      zy_len_vec.push_back(len_zy);
    }
    for(const auto& hit : matched_track.at(trackID).second){
      Float_t len_zx = GetDistance2D(hit, matched_track.at(i).second.front());
      zx_len_vec.push_back(len_zx);
    }
    auto min_zy_len = std::min_element(zy_len_vec.begin(), zy_len_vec.end());
    auto min_zx_len = std::min_element(zx_len_vec.begin(), zx_len_vec.end());
    if(*min_zy_len < fDeltaThresholdDistance and *min_zx_len < fDeltaThresholdDistance){
      front_score = std::min(*min_zy_len, *min_zx_len);
    }

    //for back point of the candidate
    zy_len_vec.clear();
    zx_len_vec.clear();
    for(const auto& hit :  matched_track.at(trackID).first){
      Float_t len_zy = GetDistance2D(hit, matched_track.at(i).first.back());
      zy_len_vec.push_back(len_zy);
    }
    for(const auto& hit : matched_track.at(trackID).second){
      Float_t len_zx = GetDistance2D(hit, matched_track.at(i).second.back());
      zx_len_vec.push_back(len_zx);
    }
    min_zy_len = std::min_element(zy_len_vec.begin(), zy_len_vec.end());
    min_zx_len = std::min_element(zx_len_vec.begin(), zx_len_vec.end());
    if(*min_zy_len < fDeltaThresholdDistance and *min_zx_len < fDeltaThresholdDistance){
      back_score = std::min(*min_zy_len, *min_zx_len);
    }
    if(front_score == 10000 and back_score == 10000){continue;}
    if(front_score < back_score){
      DeltaRayScore += GetSign(matched_track.at(i).first.back().x - matched_track.at(i).first.front().x);
    }
    if(back_score < front_score){
      DeltaRayScore += GetSign(matched_track.at(i).first.front().x - matched_track.at(i).first.back().x);
    }
  }
  return DeltaRayScore;
}


Float_t VertexRecon::CalZlen(const std::vector<two_vector>& track){
  Float_t min_z = 100000;
  Float_t max_z = -100000;
  for(const auto& v: track){
    if(v.x < min_z){
      min_z = v.x;
    }
    if(v.x > max_z){
      max_z = v.x;
    }
  }
  return max_z - min_z;
}

Float_t VertexRecon::CalXYlen(const std::vector<two_vector>& track){
  Float_t min = 100000;
  Float_t max = -100000;
  for(const auto& v : track){
    if(v.y < min){
      min = v.y;
    }
    if(v.y > max){
      max = v.y;
    }
  }
  return max - min;
}

two_vector VertexRecon::GetMinZhit(const std::vector<two_vector>& track){
  Float_t min_z = 100000;
  two_vector MinHit;
  for(const auto& v : track){
    if(v.x < min_z){
      min_z = v.x;
      MinHit = v;
    }
  }
  return MinHit;
}

two_vector VertexRecon::GetMaxZhit(const std::vector<two_vector>& track){
  Float_t max_z = -100000;
  two_vector MaxHit;
  for(const auto& v : track){
    if(max_z < v.x){
      max_z = v.x;
      MaxHit = v;
    }
  }
  return MaxHit;
}

Float_t VertexRecon::GetDistance2D(const two_vector& one, const two_vector& two){
  return std::sqrt(
      (one.x-two.x)*(one.x-two.x)+
      (one.y-two.y)*(one.y-two.y));
}

template <class T>
int VertexRecon::GetSign(const T& a){
  int sign;
  if(a==0){sign=0;}
  if(a>0){sign=1;}
  if(a<0){sign=-1;}
  return sign;
}
