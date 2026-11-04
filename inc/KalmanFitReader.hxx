#ifndef KALMANFITREADER_HXX
#define KALMANFITREADER_HXX

#include <vector>
#include <TTree.h>

#include "DataTypes.hxx"

class KalmanFitReader{
  public:
    KalmanFitReader(TTree* hits, int event_num) : 
      tree(hits), target_event(event_num) {Initialize();}

    std::vector<two_vector> GetFitZY(){return fFitZY;}
    std::vector<two_vector> GetFitZX(){return fFitZX;}
    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> GetFit(){return fFit;}

  private:
    TTree* tree;
    int target_event;

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> fFit;
    std::vector<two_vector> fFitZY;
    std::vector<two_vector> fFitZX;

    void Initialize();
};

#endif

