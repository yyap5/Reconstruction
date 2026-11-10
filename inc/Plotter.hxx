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

    void SetVertexZY(const three_vector& vertex);
    void SetVertexZX(const three_vector& vertex);

    void SetReconVertexZY(const three_vector& vertex);
    void SetReconVertexZX(const three_vector& vertex);

    //Setting ZY Points
    void SetPrimaryHitsZY(const std::vector<two_vector>& hits);
    void SetNotPrimaryHitsZY(const std::vector<two_vector>& hits);
    void SetReconTrackZY(const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits);
    void SetPIDHitsZY(const std::vector<two_vector>& proton, const std::vector<two_vector>& electron, const std::vector<two_vector>& muon, const std::vector<two_vector>& pion);


    //Setting ZX Points
    void SetPrimaryHitsZX(const std::vector<two_vector>& hits);
    void SetNotPrimaryHitsZX(const std::vector<two_vector>& hits);
    void SetReconTrackZX(const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits);
    void SetPIDHitsZX(const std::vector<two_vector>& proton, const std::vector<two_vector>& electron, const std::vector<two_vector>& muon, const std::vector<two_vector>& pion);

    //Setting 3D recon track
    void Set3DReconTrack(const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& hits);

    //Setting G4 True Track
    void SetG4Track(const std::unordered_map<int, std::vector<three_vector>>& G4Track);

    //Plotting ZY plane
    void PlotTrueAndReconZY();
    void PlotTrueZY();
    void PlotPIDZY();

    //Plotting ZX plane
    void PlotTrueAndReconZX();
    void PlotTrueZX();
    void PlotPIDZX();

    //Plotting Both
    void Plot2DTrue();
    void Plot2DRecon();

    //Plotting 3D recon
    void Plot3DReconZY();
    void Plot3DReconZX();
    void Plot3DRecon();

    //original coordinate -> Kalman coordinate
    void KalmanTrans(Float_t mean_x, Float_t mean_y, Float_t mean_z);

    //Use Geant4 True Track
    void Plot3DReconG4();

  private:

    void SetHits(const std::vector<two_vector>& hits, TGraph *g);
    void SetTracks(const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& hits, std::vector<TGraph*>& g_vector);

    void PlotTrueAndRecon(TGraph *g_primary, TGraph *g_notprimary, std::vector<TGraph*>& g_vector, const char* yaxis);
    void PlotTrue(TGraph *g_primary, TGraph *g_notprimary, const char* yaxis);
    void PlotPID(TGraph *g_proton, TGraph *g_electron, TGraph *g_muon, TGraph *g_pion, TGraph *g_vertex, const char* yaxis);

    //vertex
    TGraph *g_vertexZY = new TGraph;
    TGraph *g_vertexZX = new TGraph;

    //recon vertex
    TGraph *g_reconvertexZY = new TGraph;
    TGraph *g_reconvertexZX = new TGraph;


    //for ZY plane
    TGraph *g_primaryZY = new TGraph;
    TGraph *g_notprimaryZY = new TGraph;
    TGraph *g_protonZY = new TGraph;
    TGraph *g_electronZY = new TGraph;
    TGraph *g_muonZY = new TGraph;
    TGraph *g_pionZY = new TGraph;
    std::vector<TGraph*> gs_reconZY;

    //for ZX plane
    TGraph *g_primaryZX = new TGraph;
    TGraph *g_notprimaryZX = new TGraph;
    TGraph *g_protonZX = new TGraph;
    TGraph *g_electronZX = new TGraph;
    TGraph *g_muonZX = new TGraph;
    TGraph *g_pionZX = new TGraph;
    std::vector<TGraph*> gs_reconZX;

    //for 3D recon track
    std::vector<TGraph*> gs_3DreconZY;
    std::vector<TGraph*> gs_3DreconZX;

    //shift each coordinate
    Float_t KalmanTransX(Float_t x, Float_t shift);
    Float_t KalmanTransY(Float_t y, Float_t shift);
    Float_t KalmanTransZ(Float_t z, Float_t shift);

    //G4 True Track
    std::vector<TGraph*> gs_G4ZY;
    std::vector<TGraph*> gs_G4ZX;
};

#endif
