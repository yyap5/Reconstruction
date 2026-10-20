#ifndef RECON3D_HXX
#define RECON3D_HXX

#include <vector>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <iostream>


#include "DataTypes.hxx"

class Recon3D{
  public:

    Recon3D(){
      fMatchingLength = (2*fMatchingLayerNumber - 1) * 10 + 5;
    }

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Match(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
        );

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> Match_w_start(
        const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZX, const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_ZY
        );


  private:

    //Specify the maximum number of layers that can be spaced during track matching.
    Int_t fMatchingLayerNumber = 1;

    //track starting/end layers are closer than this length, these tracks are matched
    //dont tune this number directly, the number is setted automatically
    Float_t fMatchingLength;

    std::pair<Float_t, Float_t> GetStartEnd(const std::vector<two_vector>& track);
    std::vector<two_vector> pair_to_vec(const std::vector<std::pair<int, two_vector>>& pv);

};


#endif
