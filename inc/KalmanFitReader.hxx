#ifndef KALMANFITREADER_HXX
#define KALMANFITREADER_HXX

#include <vector>
#include <TTree.h>
#include <unordered_map>

#include "DataTypes.hxx"

class KalmanFitReader{
  public:
    KalmanFitReader(TTree* hits, int event_num) : 
      tree(hits), target_event(event_num) {Initialize();}

    std::vector<two_vector> GetFitZY(){return fFitZY;}
    std::vector<two_vector> GetFitZX(){return fFitZX;}
    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> GetFit(){return fFit;}
    std::unordered_map<int, std::vector<three_vector>> GetFit3D(){return fFit3D;}

    void GraphTrue(TTree* graph_tree);

    std::vector<two_vector> GetTrueZY(){return fGraphTrueZY;}
    std::vector<two_vector> GetTrueZX(){return fGraphTrueZX;}
    Float_t GetShiftX(){return fMeanX;}
    Float_t GetShiftY(){return fMeanY;}
    Float_t GetShiftZ(){return fMeanZ;}

  private:
    TTree* tree;
    int target_event;

    std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> fFit;
    std::unordered_map<int, std::vector<three_vector>> fFit3D;
    std::vector<two_vector> fFitZY;
    std::vector<two_vector> fFitZX;

    //graph true hit vector
    std::vector<two_vector> fGraphTrueZY;
    std::vector<two_vector> fGraphTrueZX;

    //shift value
    Float_t fMeanX;
    Float_t fMeanY;
    Float_t fMeanZ;

    void Initialize();
};

#endif

