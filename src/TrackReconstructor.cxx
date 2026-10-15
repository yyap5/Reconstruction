#include "TrackReconstructor.hxx"


TrackReconstructor::TrackReconstructor(){
  fMaxSlopeChange = 15. * M_PI / 180.;
  fMaxSlopeChangeAdjacent = 5. * M_PI / 180.;
  fMaxLength = 25.;
}

std::unordered_map<int, std::vector<std::pair<int, three_vector>>> TrackReconstructor::Reconstruct(
    const std::vector<std::vector<std::pair<int, three_vector>>>& layers
    ){
  std::unordered_map<int, std::vector<std::pair<int, three_vector>>> recon_track;

  if(layers.empty()){return recon_track;}

  int track_num = 0;

  // setting first layer to recon_track
  for(const auto& hit : layers.at(0)){
    recon_track[track_num].push_back(hit);
    track_num++;
  }

  //for 2~ layers
  for(size_t i=1; i<layers.size(); i++){
    recon_track = ReconstructLayer(recon_track, layers.at(i));
  }
  return recon_track;
}

Float_t TrackReconstructor::Length(const three_vector& a, const three_vector& b){
  Float_t length = (a.z-b.z)*(a.z-b.z) + (a.y-b.y)*(a.y-b.y);
  return std::sqrt(length);
}

Float_t TrackReconstructor::Slope(const three_vector& a, const three_vector& b){
  return (a.y - b.y) / (a.z - b.z);
}

std::pair<int, int> TrackReconstructor::MinMap(std::unordered_map<int, std::unordered_map<int, Float_t>> map){
  Float_t min_value = 10000000000;
  std::pair<int, int> min_key;
  for(const auto& [nkey, n_map] : map){
    for(const auto& [mkey, m_map] : n_map){
      if(m_map < min_value){
        min_value = m_map;
        min_key = std::make_pair(nkey,mkey);
      }
    }
  }
  return min_key;
}

std::unordered_map<int, std::vector<std::pair<int,three_vector>>> TrackReconstructor::ReconstructLayer(
      std::unordered_map<int, std::vector<std::pair<int, three_vector>>> recon_track, const std::vector<std::pair<int, three_vector>>& new_layer
    ){
  std::unordered_map<int, Float_t> ref_value;
  for(const auto& [key, hit] : recon_track){
    if(hit.size()==1){ref_value[key] = -1;} // this track is not line but point
    else{
      ref_value[key] = atan(Slope(hit.back().second, hit.at(hit.size()-2).second));
    }
  }
  std::unordered_map<int, int> comp_sign; // 1 : complete, 0 : yet
  for(const auto& [key, hit] : recon_track){
    if(abs(new_layer.at(0).second.z - hit.back().second.z) < 30.){comp_sign[key] = 0;}
    else{comp_sign[key] = 1;}
  }
  std::unordered_map<int, std::unordered_map<int, Float_t>> p_evaluation_value; //<new hit, <recon track, value>>
  std::unordered_map<int, std::unordered_map<int, Float_t>> l_evaluation_value; //<new hit, <recon track, value>>
  std::unordered_map<int, std::unordered_map<int, Float_t>> ad_evaluation_value; //<new hit, <recon track, value>>
  for(size_t i = 0; i<new_layer.size(); i++){
    p_evaluation_value[i][-1] = 10000000;
    l_evaluation_value[i][-1] = 10000000;
    ad_evaluation_value[i][-1] = 10000000;
  }
  for(const auto& [key, hit] : recon_track){
    if(comp_sign[key] == 1){continue;}
    Float_t minimum_value = 10000000;
    for(size_t i = 0; i<new_layer.size(); i++){
      three_vector new_hit = new_layer.at(i).second;
      //point
      if(ref_value[key] == -1){
        Float_t length = Length(hit.back().second, new_hit);
        if(length < fMaxLength){
          p_evaluation_value[i][key] = length;
        }
        if(hit.back().first < 2){continue;}
        Float_t sl = atan(Slope(hit.back().second, new_hit));
        Float_t p_sl = atan(hit.back().first-1);
        Float_t m_sl = atan(-1*hit.back().first+1);
        Float_t p_sl_change = abs(p_sl - sl);
        Float_t m_sl_change = abs(m_sl - sl);
        Float_t slope_change = m_sl_change;
        if(p_sl_change < m_sl_change){slope_change = p_sl_change;}
        if(slope_change > fMaxSlopeChangeAdjacent){continue;}
        ad_evaluation_value[i][key] = slope_change;
      }
      //line
      else{
        Float_t sl = atan(Slope(hit.back().second, new_hit));
        Float_t slope_change = abs(sl - ref_value[key]);
        if(slope_change > fMaxSlopeChange){continue;}
        l_evaluation_value[i][key] = slope_change;
      }

    }
  }
  //evaluation print
  /*
  for(const auto& [nkey, n_map] : l_evaluation_value){
    for(const auto& [mkey, m_map] : n_map){
      std::cout << "(" << nkey << ", " << mkey << ") " << m_map << std::endl;
    }
  }
  std::cout << "-----------------------------" << std::endl;
  */
  //line
  while(1){
    // checking if there are any hits to be connected
    int bf = 1;
    for(const auto& [nkey, n_map] : l_evaluation_value){
      for(const auto& [mkey, m_map] : n_map){if(mkey!=-1){bf=0;}}
    }
    if(bf==1){break;}

    std::pair<int, int> connected_key = MinMap(l_evaluation_value);
    recon_track[connected_key.second].push_back(std::make_pair(new_layer.at(connected_key.first).first, new_layer.at(connected_key.first).second));
    l_evaluation_value.erase(connected_key.first);
    for(auto& [nkey, n_map] : l_evaluation_value){
      n_map.erase(connected_key.second);
    }
    p_evaluation_value.erase(connected_key.first);
    ad_evaluation_value.erase(connected_key.first);

  }
  //point
  while(1){
    // checking if there are any hits to be connected
    int bf = 1;
    for(const auto& [nkey, n_map] : p_evaluation_value){
      for(const auto& [mkey, m_map] : n_map){if(mkey!=-1){bf=0;}}
    }
    if(bf==1){break;}

    std::pair<int, int> connected_key = MinMap(p_evaluation_value);
    recon_track[connected_key.second].push_back(std::make_pair(new_layer.at(connected_key.first).first, new_layer.at(connected_key.first).second));
    p_evaluation_value.erase(connected_key.first);
    for(auto& [nkey, n_map] : p_evaluation_value){
      n_map.erase(connected_key.second);
    }
    l_evaluation_value.erase(connected_key.first);
    ad_evaluation_value.erase(connected_key.first);                                                   
  }
  //merged point use the slope (~45 deg)
  while(1){
    // checking if there are any hits to be connected
    int bf = 1;
    for(const auto& [nkey, n_map] : ad_evaluation_value){
      for(const auto& [mkey, m_map] : n_map){if(mkey!=-1){bf=0;}}
    }
    if(bf==1){break;}

    std::pair<int, int> connected_key = MinMap(ad_evaluation_value);
    recon_track[connected_key.second].push_back(std::make_pair(new_layer.at(connected_key.first).first, new_layer.at(connected_key.first).second));
    ad_evaluation_value.erase(connected_key.first);
    for(auto& [nkey, n_map] : ad_evaluation_value){
      n_map.erase(connected_key.second);
    }
    l_evaluation_value.erase(connected_key.first);
    p_evaluation_value.erase(connected_key.first);

  }
  //generate new track
  if(p_evaluation_value.empty()==false){
    for(const auto& [key, val] : p_evaluation_value){
      int new_key = 0;
      while(recon_track.find(new_key) != recon_track.end()){new_key++;}
      //recon_track[new_key].push_back(new_layer.at(key));
      recon_track[new_key] = {std::make_pair(new_layer.at(key).first, new_layer.at(key).second)};
    }
  }

  return recon_track;
}
