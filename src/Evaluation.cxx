#include "Evaluation.hxx"

Float_t Evaluation::CalEff( //<efficiency, purity>
    const int& target_event,
    TTree* G4Track, const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack){

  //Read TrueTrack
  std::unordered_map<int, std::vector<three_vector>> TrueTrack;
  ReadG4Track(G4Track, target_event, TrueTrack);

  //find used Z from matched recon track
  std::unordered_set<Float_t> FiberPosZ = GetFiberPosZ(ReconTrack);

  //TrueTrack to Fiber sheet position hits
  std::unordered_map<int, std::vector<three_vector>> TrueFiberHits = 
    GetTrueFiberHits(TrueTrack, FiberPosZ);
  //debug
  for(const auto& [key, vec] : TrueFiberHits){
    std::cout << "key : " << key << std::endl;
    for(const auto& v : vec){
      std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }
  }

  return GetEfficiency(TrueFiberHits, ReconTrack);
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
  //debug
  std::cout << recon_track_num << " / " << true_track_num << std::endl;

  return static_cast<Float_t>(recon_track_num) / true_track_num;
}

Float_t Evaluation::GetLen_twov(const two_vector& one, const two_vector& two){
  return std::sqrt((one.x - two.x)*(one.x - two.x) + (one.y - two.y)*(one.y - two.y));
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





