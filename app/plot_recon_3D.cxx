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
  std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> hits_by_module_ZX = SWDhits.GetAllHitsZX();

  //////////////
  //Merge Hits//
  //////////////
  HitMerger hit_merger;
  std::unordered_map<int, std::vector<std::pair<int, two_vector>>> merged_hits_by_module_ZY;
  std::unordered_map<int, std::vector<std::pair<int, two_vector>>> merged_hits_by_module_ZX;
  for(const auto& [key, cp] : hits_by_module_ZY){
    merged_hits_by_module_ZY[key] = hit_merger.Merge(cp);
  }
  for(const auto& [key, cp] : hits_by_module_ZX){
    merged_hits_by_module_ZX[key] = hit_merger.Merge(cp);
  }

  /////////////
  //Sort hits//
  /////////////
  Sort sort_hits;
  std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits_ZY = sort_hits.ForwardSort(merged_hits_by_module_ZY);
  std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits_ZX = sort_hits.ForwardSort(merged_hits_by_module_ZX);

  //////////////////
  //Reconstruction//
  //////////////////
  TrackReconstructor track_reconstructor;
  std::unordered_map<int, std::vector<std::pair<int, two_vector>>> recon_track_ZY = track_reconstructor.Reconstruct(sorted_hits_ZY);
  std::unordered_map<int, std::vector<std::pair<int, two_vector>>> recon_track_ZX = track_reconstructor.Reconstruct(sorted_hits_ZX);

  /////////////////////
  //3D Reconstruction//
  /////////////////////
  Recon3D track_reconstructor3D;
  std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> matched_track = track_reconstructor3D.Match(recon_track_ZX, recon_track_ZY);


  ////////
  //Plot//
  ////////
  Plotter plotevent;
  plotevent.SetPrimaryHitsZY(SWDhits.GetPrimaryHitsZY());
  plotevent.SetPrimaryHitsZX(SWDhits.GetPrimaryHitsZX());
  plotevent.SetNotPrimaryHitsZY(SWDhits.GetNotPrimaryHitsZY());
  plotevent.SetNotPrimaryHitsZX(SWDhits.GetNotPrimaryHitsZX());
  plotevent.SetReconTrackZY(recon_track_ZY);
  plotevent.SetReconTrackZX(recon_track_ZX);

  //plotevent.PlotTrueZY();
  //plotevent.PlotTrueZX();
  //plotevent.PlotTrueAndReconZY();
  plotevent.PlotTrueAndReconZX();

  app.Run();
  return 0;

}
