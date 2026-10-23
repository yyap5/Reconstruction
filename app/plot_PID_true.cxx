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


int main(int argc, char* argv[]){
  TApplication app("app", &argc, argv);
  if(argc != 3){
    std::cout << "# arguments is wrong." << std::endl;
    return 1;
  }
  int target_file = std::stoi(argv[1]);
  int target_event = std::stoi(argv[2]);

  //////////////////
  //Open ROOT File//
  //////////////////
  TFile* _file0 = TFile::Open(Form("/home/daiki/yasushi/nd280/mc/20260116-neuth10-nd320kAflux/14.36mc/Tree/FHCnue/FHCnue_%04d_tree.root", target_file));
  TTree* tree = (TTree*)_file0->Get("swdhits");

  //////////////////////
  //Read hits and save//
  //////////////////////
  HITSTreeReader SWDhits(tree, target_event);

  std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> hits_by_module_ZY = SWDhits.GetAllHitsZY();
  std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> hits_by_module_ZX = SWDhits.GetAllHitsZX();


  ////////
  //Plot//
  ////////
  Plotter plotevent;
  plotevent.SetPrimaryHitsZY(SWDhits.GetPrimaryHitsZY());
  plotevent.SetPrimaryHitsZX(SWDhits.GetPrimaryHitsZX());
  plotevent.SetNotPrimaryHitsZY(SWDhits.GetNotPrimaryHitsZY());
  plotevent.SetNotPrimaryHitsZX(SWDhits.GetNotPrimaryHitsZX());
  plotevent.SetPIDHitsZY(SWDhits.GetProtonHitsZY(), SWDhits.GetElectronHitsZY(), 
      SWDhits.GetMuonHitsZY(), SWDhits.GetPionHitsZY());
  plotevent.SetPIDHitsZX(SWDhits.GetProtonHitsZX(), SWDhits.GetElectronHitsZX(), 
      SWDhits.GetMuonHitsZX(), SWDhits.GetPionHitsZX());

  //plotevent.Plot2DTrue();
  plotevent.PlotPIDZY();

  app.Run();
  return 0;

}
