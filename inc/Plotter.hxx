#ifndef PLOTTER_HXX
#define PLOTTER_HXX

#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>

#include "DataTypes.hxx"

class Plotter{
  public:

    Plotter() = default;

    void SetPrimaryHitsZY(const std::vector<three_vector>& hits);
    void SetNotPrimaryHitsZY(const std::vector<three_vector>& hits);
    void SetReconTrack(const std::unordered_map<int, std::vector<std::pair<int, three_vector>>>& hits);

    void PlotTrueAndReconZY();
    void PlotTrueZY();

  private:

    TGraph *g_primaryZY;
    TGraph *g_notprimaryZY;
    std::vector<TGraph*> gs_reconZY;
};

#endif
