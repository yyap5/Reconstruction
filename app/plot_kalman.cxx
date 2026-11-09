#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TApplication.h>

#include "DataTypes.hxx"
#include "TrackReconstructor.hxx"
#include "HitMerger.hxx"
#include "HITSTreeReader.hxx"
#include "Plotter.hxx"
#include "Sort.hxx"
#include "Recon3D.hxx"
#include "ParameterMessenger.hxx"
#include "KalmanFitReader.hxx"


int main(int argc, char* argv[]){
  TApplication app("app", &argc, argv);
  if(argc != 3 and argc != 4){
    std::cout << "# arguments is wrong." << std::endl;
    return 1;
  }
  int target_file = std::stoi(argv[1]);
  int target_event = std::stoi(argv[2]);

  ///////////////////////////
  //Open Kalman Fitted File//
  ///////////////////////////
  TFile* _file0 = TFile::Open(Form("/home/daiki/yasushi/ML/kalmanfilter/Pgun/monoEmonoDir/e-500-pgun/fitroot/fitpos_%04d.root", target_file));
  TTree* tree = (TTree*)_file0->Get("fitpos");

  ////////////////
  //Open swdhits//
  ////////////////
  //this is for true hits
  TFile* _swdfile0 = TFile::Open(Form("/home/daiki/yasushi/nd280/mc/Pgun/monoEmonoDir/e-500-pgun/gun_e-_500MeV_%04d_tree.root", target_file));
  TTree* swdhits = (TTree*)_swdfile0->Get("swdhits");
  HITSTreeReader SWDhits(swdhits, target_event);

  ////////////////////////
  //Open Graph True File//
  ////////////////////////
  TFile* _graphfile0 = TFile::Open(Form("/home/daiki/yasushi/nd280/mc/Pgun/monoEmonoDir/e-500-pgun/Graph/TrueHit/TrueRoot/truepos_%04d.root", target_file));
  TTree* graph_true = (TTree*)_graphfile0->Get("truepos");


  Plotter plotevent;

  //////////////////////
  //Read hits and save//
  //////////////////////
  KalmanFitReader Reader(tree, target_event);
  Reader.GraphTrue(graph_true);
  HITSTreeReader TrueReader(swdhits, target_event);


  ////////
  //Plot//
  ////////
  plotevent.SetPrimaryHitsZY(SWDhits.GetPrimaryHitsZY());
  plotevent.SetPrimaryHitsZX(SWDhits.GetPrimaryHitsZX());
  plotevent.SetNotPrimaryHitsZY(SWDhits.GetNotPrimaryHitsZY());
  plotevent.SetNotPrimaryHitsZX(SWDhits.GetNotPrimaryHitsZX());
  plotevent.Set3DReconTrack(Reader.GetFit());
  plotevent.KalmanTrans(Reader.GetShiftX(), Reader.GetShiftY(), Reader.GetShiftZ());

  //plotevent.PlotTrueZY();
  //plotevent.PlotTrueZX();
  //plotevent.PlotTrueAndReconZY();
  //plotevent.PlotTrueAndReconZX();
  plotevent.Plot3DRecon();

  app.Run();
  return 0;

}
