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

void KalmanFitReader::GraphTrue(TTree* graph_tree){
  Int_t event;
  Float_t x;
  Float_t y;
  Float_t z;
  Float_t mean_x;
  Float_t mean_y;
  Float_t mean_z;
  Float_t m_x;
  Float_t m_y;
  Float_t m_z;

  graph_tree->SetBranchAddress("event", &event);
  graph_tree->SetBranchAddress("x", &x);
  graph_tree->SetBranchAddress("y", &y);
  graph_tree->SetBranchAddress("z", &z);
  graph_tree->SetBranchAddress("mean_x", &mean_x);
  graph_tree->SetBranchAddress("mean_y", &mean_y);
  graph_tree->SetBranchAddress("mean_z", &mean_z);

  std::vector<two_vector> TrueZY;
  std::vector<two_vector> TrueZX;


  int nentries = graph_tree->GetEntries();
  for(int i=0; i<nentries; i++){
    graph_tree->GetEntry(i);
    if(event != target_event){continue;}
    TrueZY.push_back(two_vector{z,y});
    TrueZX.push_back(two_vector{z,x});
    m_x = mean_x;
    m_y = mean_y;
    m_z = mean_z;
  }

  fGraphTrueZY = TrueZY;
  fGraphTrueZX = TrueZX;

  fMeanX = m_x;
  fMeanY = m_y;
  fMeanZ = m_z;
}

