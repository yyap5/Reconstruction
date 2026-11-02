#include "Evaluation.hxx"

std::pair<Float_t, Float_t> Evaluation::CalEffPur( //<efficiency, purity>
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
  for(const auto& match : ReconTrack){
    std::cout << "----zy----" << std::endl;
    for(const auto& v : match.first){
      std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
    }
    std::cout << "----zx----" << std::endl;
    for(const auto& v : match.second){
      std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
    }
  }




  std::pair<Float_t, Float_t> efpu;
  return efpu;
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





