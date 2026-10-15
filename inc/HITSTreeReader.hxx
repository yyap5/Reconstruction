#ifndef HITSTREEREADER_HXX
#define HITSTREEREADER_HXX

#include <vector>
#include <unordered_map>
#include <utility>
#include <TTree.h>
#include "DataTypes.hxx"

class HITSTreeReader{
  public:
    HITSTreeReader(TTree* hits) : tree(hits) {Initialize();}

    std::vector<three_vector> GetPrimaryHits(){return fPrimaryHits;}
    std::vector<three_vector> GetNotPrimaryHits(){return fNotPrimaryHits;}
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_three_vector>>> GetAllHits(){return fAllHits;}


  private:
    TTree* tree;

    std::vector<three_vector> fPrimaryHits;
    std::vector<three_vector> fNotPrimaryHits;
    std::unordered_map<int,std::vector<std::pair<Int_t,charge_three_vector>>> fAllHits;

    void Initialize();
};
#endif
