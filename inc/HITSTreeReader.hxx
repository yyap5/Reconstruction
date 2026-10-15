#ifndef HITSTREEREADER_HXX
#define HITSTREEREADER_HXX

#include <vector>
#include <unordered_map>
#include <utility>
#include <TTree.h>
#include "DataTypes.hxx"

class HITSTreeReader{
  public:
    HITSTreeReader(TTree* hits) : tree(hits) {}

    std::unordered_map<int,std::vector<std::pair<Int_t,charge_three_vector>>> ReadHits(int event_num);

  private:
      TTree* tree;
};
#endif
