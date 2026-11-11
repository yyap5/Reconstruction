#ifndef VERTEXRECON_HXX
#define VERTEXRECON_HXX

#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "DataTypes.hxx"

class VertexRecon{
  public:

    VertexRecon(){
      fDeltaThresholdDistance = 50./*mm*/;
    }

    //there are several vertex reconstruction algorithm
    //minz : find the longest track, this regards track direction as forward.
    three_vector GetVertex_minz(
        const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track);

    //uDeltaRay : find the longest track, and use delta ray to decide the direction of the track
    three_vector GetVertex_uDeltaRay(
        const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track);

  private:

    Float_t CalZlen(const std::vector<two_vector>& track);

    Float_t CalXYlen(const std::vector<two_vector>& track);

    two_vector GetMinZhit(const std::vector<two_vector>& track);

    two_vector GetMaxZhit(const std::vector<two_vector>& track);

    Float_t GetDistance2D(const two_vector& one, const two_vector& two);

    //Get Deltaray trackID of the matched_track.at(trackID) track
    int GetDeltaRayScore(
        const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track, 
        const int& trackID);

    //if the length between these tracks is smaller than the value,
    //they are regarded as delta ray
    Float_t fDeltaThresholdDistance;

    template <class T>
    int GetSign(const T& a);

};

#endif
