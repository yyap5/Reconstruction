#include "KalmanFitReader.hxx"

void KalmanFitReader::Initialize(){

  Int_t event;
  Int_t track;
  Double_t x;
  Double_t y;
  Double_t z;

  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("track", &track);
  tree->SetBranchAddress("x", &x);
  tree->SetBranchAddress("y", &y);
  tree->SetBranchAddress("z", &z);

  std::unordered_map<int,std::vector<three_vector>> FittedTrack;

  int nentries = tree->GetEntries();
  //fill FittedTrack map
  for(int i=0; i<nentries; i++){
    tree->GetEntry(i);
    if(event != target_event){continue;}
    FittedTrack[track].push_back(three_vector{static_cast<Float_t>(x),static_cast<Float_t>(y),static_cast<Float_t>(z)});
  }

  for(const auto& [key, v] : FittedTrack){
    std::vector<two_vector> zy_track;
    std::vector<two_vector> zx_track;
    for(const auto& hit : v){
      zy_track.push_back(two_vector{hit.z,hit.y});
      zx_track.push_back(two_vector{hit.z,hit.x});
    }
    fFit.push_back(make_pair(zy_track, zx_track));
  }
}
