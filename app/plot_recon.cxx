#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TApplication.h>

#include "DataTypes.hxx"
#include "TrackReconstructor.hxx"
#include "HitMerger.hxx"
#include "HITSTreeReader.hxx"
#include "Plotter.hxx"


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
  TFile* _file0 = TFile::Open(Form("/home/daiki/yasushi/nd280/mc/Pgun/monoEmonoDir/e-500-pgun/gun_e-_500MeV_%04d_tree.root", target_file));
  TTree* tree = (TTree*)_file0->Get("swdhits");

  //////////////////////
  //Read hits and save//
  //////////////////////
  HITSTreeReader SWDhits(tree, target_event);

  std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> hits_by_module_ZY = SWDhits.GetAllHitsZY();

  //////////////
  //Merge Hits//
  //////////////
  HitMerger hit_merger;
  std::unordered_map<int, std::vector<std::pair<int, two_vector>>> merged_hits_by_module_ZY;
  for(const auto& [key, cp] : hits_by_module_ZY){
    merged_hits_by_module_ZY[key] = hit_merger.Merge(cp);
  }

  /////////////
  //Sort hits//
  /////////////
  std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits_ZY;
  std::vector<int> keys;
  for(const auto& [key, hits] : merged_hits_by_module_ZY){
    keys.push_back(key);
  }
  std::sort(keys.begin(), keys.end(), std::greater<int>());
  for(const auto& key : keys){
    sorted_hits_ZY.push_back(merged_hits_by_module_ZY.at(key));
  }

  //////////////////
  //Reconstruction//
  //////////////////
  TrackReconstructor track_reconstructor;
  std::unordered_map<int, std::vector<std::pair<int, two_vector>>> recon_track_ZY = track_reconstructor.Reconstruct(sorted_hits_ZY);


  ////////
  //Plot//
  ////////
  Plotter plotevent;
  plotevent.SetPrimaryHitsZY(SWDhits.GetPrimaryHitsZY());
  plotevent.SetPrimaryHitsZX(SWDhits.GetPrimaryHitsZX());
  plotevent.SetNotPrimaryHitsZY(SWDhits.GetNotPrimaryHitsZY());
  plotevent.SetNotPrimaryHitsZX(SWDhits.GetNotPrimaryHitsZX());
  plotevent.SetReconTrackZY(recon_track_ZY);

  //plotevent.PlotTrueZY();
  plotevent.PlotTrueZX();
  //plotevent.PlotTrueAndReconZY();

  app.Run();
  return 0;

}
