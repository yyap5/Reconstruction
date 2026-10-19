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

    std::vector<two_vector> GetPrimaryHitsZY(){return fPrimaryHitsZY;}
    std::vector<two_vector> GetNotPrimaryHitsZY(){return fNotPrimaryHitsZY;}
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> GetAllHitsZY(){return fAllHitsZY;}

    std::vector<two_vector> GetPrimaryHitsZX(){return fPrimaryHitsZX;}
    std::vector<two_vector> GetNotPrimaryHitsZX(){return fNotPrimaryHitsZX;}
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> GetAllHitsZX(){return fAllHitsZX;}


  private:
    TTree* tree;
    int target_event;

    std::vector<two_vector> fPrimaryHitsZY;
    std::vector<two_vector> fNotPrimaryHitsZY;
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> fAllHitsZY;

    std::vector<two_vector> fPrimaryHitsZX;
    std::vector<two_vector> fNotPrimaryHitsZX;
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> fAllHitsZX;

    void Initialize();
};
#endif
