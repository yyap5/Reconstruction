#include "HITSTreeReader.hxx"

void HITSTreeReader::Initialize(){

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


  int nentries = tree->GetEntries();
  for(int i=0; i<nentries; i++){
    tree->GetEntry(i);
    if(event != target_event){continue;}
    if(trackId==-1){continue;}
    //for ZY plane
    if(SWDFiberType == 0 && SWDFiberDirection == 0){
      fAllHitsZY[SWDFiberModule].push_back(std::make_pair(SWDFiberNumber, charge_two_vector{charge, pos.z, pos.y}));

      if(trackId == primaryTrackId){fPrimaryHitsZY.push_back(two_vector{pos.z, pos.y});}
      if(trackId != primaryTrackId){fNotPrimaryHitsZY.push_back(two_vector{pos.z, pos.y});}
      if(PDG == 2212){fProtonHitsZY.push_back(two_vector{pos.z, pos.y});}
      if(std::abs(PDG) == 11 or PDG == 22){fElectronHitsZY.push_back(two_vector{pos.z, pos.y});}
      if(std::abs(PDG) == 13){fMuonHitsZY.push_back(two_vector{pos.z, pos.y});}
      if(std::abs(PDG) == 211){fPionHitsZY.push_back(two_vector{pos.z, pos.y});}
    }

    //for ZX plane
    if(SWDFiberType == 0 && SWDFiberDirection == 1){
      fAllHitsZX[SWDFiberModule].push_back(std::make_pair(SWDFiberNumber, charge_two_vector{charge, pos.z, pos.x}));

      if(trackId == primaryTrackId){fPrimaryHitsZX.push_back(two_vector{pos.z, pos.x});}
      if(trackId != primaryTrackId){fNotPrimaryHitsZX.push_back(two_vector{pos.z, pos.x});}
      if(PDG == 2212){fProtonHitsZX.push_back(two_vector{pos.z, pos.x});}
      if(std::abs(PDG) == 11 or PDG == 22){fElectronHitsZX.push_back(two_vector{pos.z, pos.x});}
      if(std::abs(PDG) == 13){fMuonHitsZX.push_back(two_vector{pos.z, pos.x});}
      if(std::abs(PDG) == 211){fPionHitsZX.push_back(two_vector{pos.z, pos.x});}
    }

  }
}
