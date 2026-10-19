#ifndef PLOTTER_HXX
#define PLOTTER_HXX

#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TAxis.h>

#include "DataTypes.hxx"

class Plotter{
  public:

    Plotter() = default;

    //Setting ZY Points
    void SetPrimaryHitsZY(const std::vector<two_vector>& hits);
    void SetNotPrimaryHitsZY(const std::vector<two_vector>& hits);
    void SetReconTrackZY(const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits);

    //Setting ZX Points
    void SetPrimaryHitsZX(const std::vector<two_vector>& hits);
    void SetNotPrimaryHitsZX(const std::vector<two_vector>& hits);
    void SetReconTrackZX(const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits);

    //Setting 3D recon track
    void Set3DReconTrack(const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& hits);

    //Plotting ZY plane
    void PlotTrueAndReconZY();
    void PlotTrueZY();

    //Plotting ZX plane
    void PlotTrueAndReconZX();
    void PlotTrueZX();

    //Plotting Both
    void Plot2DTrue();
    void Plot2DRecon();

    //Plotting 3D recon
    void Plot3DReconZY();
    void Plot3DReconZX();
    void Plot3DRecon();

  private:

    void SetHits(const std::vector<two_vector>& hits, TGraph *g);
    void SetTracks(const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits, std::vector<TGraph*>& g_vector);

    void PlotTrueAndRecon(TGraph *g_primary, TGraph *g_notprimary, std::vector<TGraph*>& g_vector, const char* yaxis);
    void PlotTrue(TGraph *g_primary, TGraph *g_notprimary, const char* yaxis);


    //for ZY plane
    TGraph *g_primaryZY = new TGraph;
    TGraph *g_notprimaryZY = new TGraph;
    std::vector<TGraph*> gs_reconZY;

    //for ZX plane
    TGraph *g_primaryZX = new TGraph;
    TGraph *g_notprimaryZX = new TGraph;
    std::vector<TGraph*> gs_reconZX;

    //for 3D recon track
    std::vector<TGraph*> gs_3DreconZY;
    std::vector<TGraph*> gs_3DreconZX;
};

#endif
