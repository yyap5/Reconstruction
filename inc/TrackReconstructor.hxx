#ifndef TRACKRECONSTRUCTOR_HXX
#define TRACKRECONSTRUCTOR_HXX

#include <vector>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <iostream>

#include "DataTypes.hxx"

class TrackReconstructor{

  public:

    TrackReconstructor();

    std::unordered_map<int, std::vector<std::pair<int, two_vector>>> Reconstruct(
        const std::vector<std::vector<std::pair<int, two_vector>>>& layers
        );


  private:

    Float_t fMaxSlopeChange;
    Float_t fMaxSlopeChangeAdjacent;
    Float_t fMaxLength;

    Float_t Length(const two_vector& a, const two_vector& b);

    Float_t Slope(const two_vector& a, const two_vector& b);

    std::pair<int, int> MinMap(std::unordered_map<int, std::unordered_map<int, Float_t>> map);

    std::unordered_map<int, std::vector<std::pair<int,two_vector>>> ReconstructLayer(
         std::unordered_map<int, std::vector<std::pair<int, two_vector>>> recon_track, const std::vector<std::pair<int, two_vector>>& new_layer
        );

};

#endif
