#ifndef VERTEXRECON_HXX
#define VERTEXRECON_HXX

#include <vector>
#include <utility>
#include <cmath>
#include <iostream>

#include "DataTypes.hxx"

class VertexRecon{
  public:

    VertexRecon() = default;

    //there are several vertex reconstruction algorithm
    //minz : find the longest track, this regards track direction as forward.
    three_vector GetVertex_minz(
        const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& matched_track);

  private:

    Float_t CalZlen(const std::vector<two_vector>& track);

    Float_t CalXYlen(const std::vector<two_vector>& track);

    two_vector GetMinZhit(const std::vector<two_vector>& track);

};

#endif
