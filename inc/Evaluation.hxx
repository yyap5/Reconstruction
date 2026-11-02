#ifndef EVALUATION_HXX
#define EVALUATION_HXX

#include <TTree.h>
#include <utility>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <set>
#include <cmath>

#include "DataTypes.hxx"

class Evaluation{

  public:

    Evaluation(){
      fReconThresholdRatio = 0.8;
      fThresholdHitNum = 4;
    }

    void SetReconThresholdRatio(Float_t value);
    void SetThresholdHitNum(Float_t value);

    Float_t CalEff( //<efficiency, purity>
        const int& target_event, 
        TTree* G4Track, const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack);


  private:

    //if true track is reconstructed over this value, 
    //the true track is regarded as reconstructed
    Float_t fReconThresholdRatio;

    //when the true hit is more than this number, 
    //it is regarded as a track.
    //In other words, the track whose track is less than this value is not considerd.
    Int_t fThresholdHitNum; //<= #hits ----> regarded as track

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

    //cal tracking efficiency. = 
    //#correctly reconstructed track / #all true track
    //from TrueFiberHits and matched recon tracks
    Float_t GetEfficiency(
        const std::unordered_map<int, std::vector<three_vector>>& TrueFiberHits,
        const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& ReconTrack);

    Float_t GetMin_threevZ(const std::vector<three_vector>& v);
    Float_t GetMax_threevZ(const std::vector<three_vector>& v);

    Float_t GetLen_twov(const two_vector& one, const two_vector& two);

};

#endif
