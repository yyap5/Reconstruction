#include "Evaluation.hxx"

Float_t Evaluation::CalEff( //<efficiency, purity>
    const int& target_event,
    TTree* G4Track, const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack){

  //Read TrueTrack
  std::unordered_map<int, std::vector<three_vector>> TrueTrack;
  ReadG4Track(G4Track, target_event, TrueTrack);

  //find used Z from matched recon track
  std::unordered_set<Float_t> FiberPosZ = GetFiberPosZ();

  //TrueTrack to Fiber sheet position hits
  std::unordered_map<int, std::vector<three_vector>> TrueFiberHits = 
    GetTrueFiberHits(TrueTrack, FiberPosZ);

  return GetEfficiency(TrueFiberHits, ReconTrack);
}

//for kalmanfilter
Float_t Evaluation::CalEff( //<efficiency, purity>
    const int& target_event,
    TTree* G4Track, const std::unordered_map<int, std::vector<three_vector>>& ReconTrack,
    Float_t shift_x, Float_t shift_y, Float_t shift_z){

  //Read TrueTrack
  std::unordered_map<int, std::vector<three_vector>> TrueTrack;
  ReadG4Track(G4Track, target_event, TrueTrack);

  //Shift TrueTrack for kalman filter coordinate
  std::unordered_map<int, std::vector<three_vector>> ShiftedTrueTrack = ShiftTrack(TrueTrack, shift_x, shift_y, shift_z);

  //debug
  /*
  std::cout << "True Track" << std::endl;
  for(const auto& [key,track] : ShiftedTrueTrack){
    std::cout << "----------key : " << key << std::endl;
    for(const auto& hit : track){
      std::cout << "(" << hit.x << ", " << hit.y << ", " << hit.z << ")" << std::endl;
    }
  }
  for(const auto& pair_track : ReconTrack){
    std::cout << "zy track---------------" << std::endl;
    for(const auto& zy_track : pair_track.first){
      std::cout << "(" << zy_track.x << ", " << zy_track.y << ")" << std::endl; }
    std::cout << "zx track---------------" << std::endl;
    for(const auto& zx_track : pair_track.second){
      std::cout << "(" << zx_track.x << ", " << zx_track.y << ")" << std::endl;
    }
  }
  */
  //debug

  //find used Z from matched recon track
  std::unordered_set<Float_t> FiberPosZ = GetFiberPosZ_kalman(shift_z);

  //TrueTrack to Fiber sheet position hits
  std::unordered_map<int, std::vector<three_vector>> TrueFiberHits = 
    GetTrueFiberHits(ShiftedTrueTrack, FiberPosZ);

  return GetEfficiency_3D(TrueFiberHits, ReconTrack);
}

void Evaluation::ReadG4Track( //Read True G4 Track
    TTree* G4Track,
    const Int_t& target_event,
    std::unordered_map<int, std::vector<three_vector>>& TrueTrack //<trackID, track points vector>
    ){

  Int_t file;
  Int_t event;
  Int_t trackID;
  Float_t P;
  three_vector pos;

  G4Track->SetBranchAddress("file", &file);
  G4Track->SetBranchAddress("event", &event);
  G4Track->SetBranchAddress("trackID", &trackID);
  G4Track->SetBranchAddress("P", &P);
  G4Track->SetBranchAddress("pos", &pos);

  for(int i=0; i<G4Track->GetEntries(); i++){
    G4Track->GetEntry(i);
    if(event != target_event){continue;}
    TrueTrack[trackID].push_back(pos);
  }

}

std::unordered_set<Float_t> Evaluation::GetFiberPosZ(
    const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack){

  std::unordered_set<Float_t> used_z;

  for(const auto& track : ReconTrack){
    for(const auto& v : track.first){used_z.insert(v.x);}
    for(const auto& v : track.second){used_z.insert(v.x);}
  }

  return used_z;
}

//for normal coordinate
std::unordered_set<Float_t> Evaluation::GetFiberPosZ(){
  std::unordered_set<Float_t> FiberLayer;
  for(Float_t fiber_z=-900.75; fiber_z<1000; fiber_z+=10){
    FiberLayer.insert(fiber_z);
  }
  return FiberLayer;
}

//for kalman coordinate
std::unordered_set<Float_t> Evaluation::GetFiberPosZ_kalman(Float_t shift_z){
  std::unordered_set<Float_t> FiberLayer;
  for(Float_t fiber_z=-900.75; fiber_z<1000; fiber_z+=10){
    FiberLayer.insert(ShiftZ(fiber_z, shift_z));
  }
  return FiberLayer;
}


std::unordered_map<int, std::vector<three_vector>> Evaluation::GetTrueFiberHits(
    std::unordered_map<int, std::vector<three_vector>>& TrueTrack, std::unordered_set<Float_t>& FiberPosZ){

  std::unordered_map<int, std::vector<three_vector>> TrueFiberHits;

  for(const auto& [key, truetrack] : TrueTrack){
    for(const auto& fiberhit : FiberPosZ){
      if(fiberhit < GetMin_threevZ(truetrack) or GetMax_threevZ(truetrack) < fiberhit){continue;}
      TrueFiberHits[key].push_back(ConvertToFiberHit(fiberhit, truetrack));
    }
  }
  return TrueFiberHits;
}

three_vector Evaluation::ConvertToFiberHit(
    const Float_t& fiberhit, const std::vector<three_vector>& truetrack){

  three_vector min_hit;
  three_vector max_hit;
  //min
  Float_t Value = 100000;
  for(const auto& v : truetrack){
    if(v.z > fiberhit){continue;}
    if(std::fabs(v.z - fiberhit) < Value){
      min_hit = v;
      Value = std::fabs(v.z - fiberhit);
    }
  }
  //max
  Value = 100000;
  for(const auto& v : truetrack){
    if(v.z < fiberhit){continue;}
    if(std::fabs(v.z - fiberhit) < Value){
      max_hit = v;
      Value = std::fabs(v.z - fiberhit);
    }
  }
  Float_t x = min_hit.x + (max_hit.x - min_hit.x) * (fiberhit - min_hit.z) / (max_hit.z - min_hit.z);
  Float_t y = min_hit.y + (max_hit.y - min_hit.y) * (fiberhit - min_hit.z) / (max_hit.z - min_hit.z);
  Float_t z = fiberhit;

  return three_vector{x, y, z};
}

Float_t Evaluation::GetEfficiency(
    const std::unordered_map<int, std::vector<three_vector>>& TrueFiberHits,
    const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack){

  int true_track_num = 0;
  int recon_track_num = 0;

  for(const auto& [key, true_vec] : TrueFiberHits){
    if(true_vec.size() < fThresholdHitNum){continue;}//if #hit < 4, they are not regarded as ture track
    true_track_num++;
    //one true track hits num
    int true_num = static_cast<int>(true_vec.size());
    //cal the number of the reconstructed hits
    for(const auto& recon_pair_track : ReconTrack){
      int recon_hit_num = 0;
      for(const auto& true_hit : true_vec){
        for(const auto& zy_hit : recon_pair_track.first){
          if(GetLen_twov(two_vector{true_hit.z, true_hit.y}, zy_hit) < 1.){recon_hit_num++;}
        }
        for(const auto& zx_hit : recon_pair_track.second){
          if(GetLen_twov(two_vector{true_hit.z, true_hit.x}, zx_hit) < 1.){recon_hit_num++;}
        }
      }
      if((static_cast<Float_t>(recon_hit_num) / true_num) > fReconThresholdRatio){recon_track_num++;break;}
    }
  }

  return static_cast<Float_t>(recon_track_num) / true_track_num;
}

Float_t Evaluation::GetEfficiency_3D(
    const std::unordered_map<int, std::vector<three_vector>>& TrueFiberHits,
    const std::unordered_map<int, std::vector<three_vector>>& ReconTrack){

  //parameter
  Float_t judge_len = 10.;

  int true_track_num = 0;
  int recon_track_num = 0;

  for(const auto& [key, true_vec] : TrueFiberHits){
    if(true_vec.size() < fThresholdHitNum){continue;}//if #hit < 4, they are not regarded as true track
    true_track_num++;
    //one true track hits num
    int true_num = static_cast<int>(true_vec.size());
    for(const auto& [key,recon_track] : ReconTrack){
      int recon_hit_num = 0;
      for(const auto& true_hit : true_vec){
        for(const auto& recon_hit : recon_track){
          if(GetLen_threev(true_hit, recon_hit) < judge_len){recon_hit_num++;}
        }
      }
      if((static_cast<Float_t>(recon_hit_num) / true_num) > fReconThresholdRatio){recon_track_num++;break;}
    }
  }
  return static_cast<Float_t>(recon_track_num) / true_track_num;
}

Float_t Evaluation::GetLen_twov(const two_vector& one, const two_vector& two){
  return std::sqrt((one.x - two.x)*(one.x - two.x) + (one.y - two.y)*(one.y - two.y));
}

Float_t Evaluation::GetLen_threev(const three_vector& one, const three_vector& two){
  return std::sqrt(
      (one.x - two.x) * (one.x - two.x) +
      (one.y - two.y) * (one.y - two.y) +
      (one.z - two.z) * (one.z - two.z));
}

Float_t Evaluation::GetMin_threevZ(const std::vector<three_vector>& v){
  Float_t Min = 1000000;
  for(const auto& hit : v){
    if(hit.z < Min){Min = hit.z;}
  }
  return Min;
}

Float_t Evaluation::GetMax_threevZ(const std::vector<three_vector>& v){
  Float_t Max = -1000000;
  for(const auto& hit : v){
    if(hit.z > Max){Max = hit.z;}
  }
  return Max;
}

void Evaluation::SetReconThresholdRatio(Float_t value){
  fReconThresholdRatio = value;
}

void Evaluation::SetThresholdHitNum(Float_t value){
  fThresholdHitNum = value;
}

std::unordered_map<int, std::vector<three_vector>> Evaluation::ShiftTrack(
    const std::unordered_map<int, std::vector<three_vector>>& TrueTrack,
    Float_t shift_x, Float_t shift_y, Float_t shift_z){

  std::unordered_map<int, std::vector<three_vector>> ShiftedTrack;
  for(const auto& [key, track] : TrueTrack){
    for(const auto& hit : track){
      ShiftedTrack[key].push_back(three_vector{
          ShiftX(hit.x, shift_x), 
          ShiftY(hit.y, shift_y), 
          ShiftZ(hit.z, shift_z)});
    }
  }
  return ShiftedTrack;
}

Float_t Evaluation::ShiftX(Float_t x, Float_t shift){
  return x -150*shift;
}

Float_t Evaluation::ShiftY(Float_t y, Float_t shift){
  return y -150*shift;
}

Float_t Evaluation::ShiftZ(Float_t z, Float_t shift){
  return z + 0.5 - 5.*shift;
}

std::vector<two_vector> Evaluation::GetG4ZY(const int& target_event, TTree* G4Track,
    Float_t ShiftX, Float_t ShiftY, Float_t ShiftZ){
  std::vector<two_vector> G4ZY;
  //Read TrueTrack
  std::unordered_map<int, std::vector<three_vector>> TrueTrack;
  ReadG4Track(G4Track, target_event, TrueTrack);
  std::unordered_map<int, std::vector<three_vector>> ShiftedTrueTrack = ShiftTrack(TrueTrack, ShiftX, ShiftY, ShiftZ);

  for(const auto& [key,track] : ShiftedTrueTrack){
    for(const auto& hit : track){
      G4ZY.push_back(two_vector{hit.z, hit.y});
    }
  }
  return G4ZY;
}

std::vector<two_vector> Evaluation::GetG4ZX(const int& target_event, TTree* G4Track,
    Float_t ShiftX, Float_t ShiftY, Float_t ShiftZ){
  std::vector<two_vector> G4ZX;
  //Read TrueTrack
  std::unordered_map<int, std::vector<three_vector>> TrueTrack;
  ReadG4Track(G4Track, target_event, TrueTrack);
  std::unordered_map<int, std::vector<three_vector>> ShiftedTrueTrack = ShiftTrack(TrueTrack, ShiftX, ShiftY, ShiftZ);

  for(const auto& [key,track] : ShiftedTrueTrack){
    for(const auto& hit : track){
      G4ZX.push_back(two_vector{hit.z, hit.x});
    }
  }
  return G4ZX;
}

std::unordered_map<int, std::vector<three_vector>> Evaluation::GetG4(const int& target_event,
    TTree* G4Track, Float_t shift_x, Float_t shift_y, Float_t shift_z){

  std::unordered_map<int, std::vector<three_vector>> TrueTrack;
  ReadG4Track(G4Track, target_event, TrueTrack);
  std::unordered_map<int, std::vector<three_vector>> ShiftedTrueTrack = ShiftTrack(TrueTrack, shift_x, shift_y, shift_z);

  return ShiftedTrueTrack;
}



