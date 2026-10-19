#ifndef HITMERGER_HXX
#define HITMERGER_HXX

#include <vector>
#include <utility>
#include <cstdlib>

#include "DataTypes.hxx"

class HitMerger{
  public:

    HitMerger() = default;

    std::vector<std::pair<int, two_vector>> 
      Merge(
          const std::vector<std::pair<Int_t, charge_two_vector>>& hits
          );

  private:

    Float_t ChargeMean(
        const std::vector<std::pair<Float_t, Float_t>>& hits
        );
};

#endif
