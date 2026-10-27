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
    if(trackID == -1){
      std::cout << "there are no reconstructed track" << std::endl;
    }
  }
  //cal vertex z from selected track
  two_vector minhitZY = GetMinZhit(matched_track.at(trackID).first);
  two_vector minhitZX = GetMinZhit(matched_track.at(trackID).second);

  vertex.z = std::min(minhitZY.x, minhitZX.x);
  vertex.x = minhitZX.y;
  vertex.y = minhitZY.y;

  return vertex;
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
