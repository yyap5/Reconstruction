#include "HITSTreeReader.hxx"

std::unordered_map<int,std::vector<std::pair<Int_t,charge_three_vector>>> HITSTreeReader::ReadHits(int target_event){

  Int_t event;
  Int_t trackId;
  Int_t PDG;
  Int_t primaryTrackId;
  Int_t SWDFiberDirection;
  Int_t SWDFiberType;
  Int_t SWDFiberModule;
  Int_t SWDFiberNumber;
  Float_t charge;
  three_vector pos;
  three_vector primaryVertex;
  three_vector truePos;
  four_momentum initialMomentum;
  four_momentum primaryInitialMomentum;

  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("trackId", &trackId);
  tree->SetBranchAddress("PDG", &PDG);
  tree->SetBranchAddress("primaryTrackId", &primaryTrackId);
  tree->SetBranchAddress("SWDFiberDirection", &SWDFiberDirection);
  tree->SetBranchAddress("SWDFiberType", &SWDFiberType);
  tree->SetBranchAddress("SWDFiberModule", &SWDFiberModule);
  tree->SetBranchAddress("SWDFiberNumber", &SWDFiberNumber);
  tree->SetBranchAddress("initialMomentum", &initialMomentum);
  tree->SetBranchAddress("pos", &pos);
  tree->SetBranchAddress("truePos", &truePos);
  tree->SetBranchAddress("charge", &charge);
  tree->SetBranchAddress("primaryVertex", &primaryVertex);
  tree->SetBranchAddress("primaryInitialMomentum", &primaryInitialMomentum);

  std::unordered_map<int,std::vector<std::pair<Int_t,charge_three_vector>>> hits_by_module;

  int nentries = tree->GetEntries();
  for(int i=0; i<nentries; i++){
    tree->GetEntry(i);
    if(event != target_event){continue;}
    if(trackId==-1){continue;}
    if(!(SWDFiberType == 0 && SWDFiberDirection == 0)){continue;}
    hits_by_module[SWDFiberModule].push_back(std::make_pair(SWDFiberNumber, charge_three_vector{charge, pos.x, pos.y, pos.z}));
  }

  return hits_by_module;

}
