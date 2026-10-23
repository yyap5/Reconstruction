#ifndef HITSTREEREADER_HXX
#define HITSTREEREADER_HXX

#include <vector>
#include <unordered_map>
#include <utility>
#include <TTree.h>
#include "DataTypes.hxx"

class HITSTreeReader{
  public:
    HITSTreeReader(TTree* hits, int event_num) : tree(hits), target_event(event_num) {Initialize();}

    three_vector GetVertex(){return fVertex;}

    std::vector<two_vector> GetPrimaryHitsZY(){return fPrimaryHitsZY;}
    std::vector<two_vector> GetNotPrimaryHitsZY(){return fNotPrimaryHitsZY;}
    std::vector<two_vector> GetProtonHitsZY(){return fProtonHitsZY;}
    std::vector<two_vector> GetElectronHitsZY(){return fElectronHitsZY;}
    std::vector<two_vector> GetMuonHitsZY(){return fMuonHitsZY;}
    std::vector<two_vector> GetPionHitsZY(){return fPionHitsZY;}
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> GetAllHitsZY(){return fAllHitsZY;}

    std::vector<two_vector> GetPrimaryHitsZX(){return fPrimaryHitsZX;}
    std::vector<two_vector> GetNotPrimaryHitsZX(){return fNotPrimaryHitsZX;}
    std::vector<two_vector> GetProtonHitsZX(){return fProtonHitsZX;}
    std::vector<two_vector> GetElectronHitsZX(){return fElectronHitsZX;}
    std::vector<two_vector> GetMuonHitsZX(){return fMuonHitsZX;}
    std::vector<two_vector> GetPionHitsZX(){return fPionHitsZX;}
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> GetAllHitsZX(){return fAllHitsZX;}


  private:
    TTree* tree;
    int target_event;

    three_vector fVertex;

    std::vector<two_vector> fPrimaryHitsZY;
    std::vector<two_vector> fNotPrimaryHitsZY;
    std::vector<two_vector> fProtonHitsZY;
    std::vector<two_vector> fElectronHitsZY;
    std::vector<two_vector> fMuonHitsZY;
    std::vector<two_vector> fPionHitsZY;
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> fAllHitsZY;

    std::vector<two_vector> fPrimaryHitsZX;
    std::vector<two_vector> fNotPrimaryHitsZX;
    std::vector<two_vector> fProtonHitsZX;
    std::vector<two_vector> fElectronHitsZX;
    std::vector<two_vector> fMuonHitsZX;
    std::vector<two_vector> fPionHitsZX;
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> fAllHitsZX;

    void Initialize();
};
#endif
