#ifndef EVALUATION_HXX
#define EVALUATION_HXX

#include <TTree.h>
#include <utility>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <set>

#include "DataTypes.hxx"

class Evaluation{

  public:

    Evaluation() = default;

    std::pair<Float_t, Float_t> CalEffPur( //<efficiency, purity>
        const int& target_event, 
        TTree* G4Track, const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack);

  private:

    void ReadG4Track( //Read True G4 Track
        TTree* G4Track, 
        const Int_t& target_event,
        std::unordered_map<int, std::vector<three_vector>>& TrueTrack //<trackID, track points vector>
        );

    std::unordered_set<Float_t> GetFiberPosZ(
        const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack);

    //True Track --> True Fiber Hits
    std::unordered_map<int, std::vector<three_vector>> GetTrueFiberHits(
        std::unordered_map<int, std::vector<three_vector>>& TrueTrack, std::unordered_set<Float_t>& FiberPosZ);

    //get both side of near hit from fiber hit
    three_vector ConvertToFiberHit(
        const Float_t& fiberhit, const std::vector<three_vector>& truetrack);

    Float_t GetMin_threevZ(const std::vector<three_vector>& v);
    Float_t GetMax_threevZ(const std::vector<three_vector>& v);

};

#endif
