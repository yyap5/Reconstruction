#ifndef RECON3D_HXX
#define RECON3D_HXX

#include <vector>
#include <utility>
#include <unordered_map>
#include <set>
#include <cmath>
#include <iostream>


#include "DataTypes.hxx"

class Recon3D{
  public:

    Recon3D(){
      fMatchingLength = (2*fMatchingLayerNumber - 1) * 10 + 5;
      fMatchingOverlapThreshold = 3;
    }

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Match(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
        );

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Match_w_start(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
        );

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Recon(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
        );



  private:

    //Specify the maximum number of layers that can be spaced during track matching.
    Int_t fMatchingLayerNumber = 1;

    //track starting/end layers are closer than this length, these tracks are matched
    //dont tune this number directly, the number is setted automatically
    Float_t fMatchingLength;

    //for "public Recon" function, this value decide the threshold number of overlap hits.
    //this should be more than 3 because "2" would be achieved when there are one Xlayer hit and 
    //two Y layer hits.
    Int_t fMatchingOverlapThreshold;

    std::pair<Float_t, Float_t> GetStartEnd(const std::vector<two_vector>& track);
    std::vector<two_vector> pair_to_vec(const std::vector<std::pair<int, two_vector>>& pv);

    //this is almost same of the public function "Match" but recreate recon_** after matching
    //for the whole 3D recon (for public function "Recon")
    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> MatchForRecon(
        std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
        );

    std::unordered_map<int, std::unordered_map<int, Int_t>> CalMatchingScoreMap(
        const std::unordered_map<int, std::vector<two_vector>>& ZX, std::unordered_map<int, std::vector<two_vector>>& ZY
        );

    Int_t CalOverlapNum(
        const std::vector<two_vector>& a, const std::vector<two_vector>& b
        );

    //return "true" when the max score is more than fMatchingOverlapThreshold
    bool CalMaxScore(
        const std::unordered_map<int, std::unordered_map<int, Int_t>>& score_map, std::pair<int, int>& received_track
        );

    //make track indicated by matchingID, delete used hits and make new remaining tracks
    std::pair<std::vector<two_vector>, std::vector<two_vector>> MakeTrack(
        const std::pair<int, int>& matchingID, std::unordered_map<int, std::vector<two_vector>>& ZX, std::unordered_map<int, std::vector<two_vector>>& ZY
        );

    //extract ramaining tracks(ramain_first, remain_second), using reference reconstructed track
    void ExtractRemainTrack(
        const std::vector<two_vector>& ref_track, const std::vector<two_vector>& old_remain,
        std::vector<two_vector>& remain_first, std::vector<two_vector>& remain_second
        );

    //get unique key of the score map, this is use for pushing new remaining tracks
    int GetUniqueKey(
        const std::unordered_map<int, std::vector<two_vector>>& map
        );


};


#endif
