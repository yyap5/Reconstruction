#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "DataTypes.hxx"
#include "TrackReconstructor.hxx"
#include "HitMerger.hxx"
#include "HITSTreeReader.hxx"

int main(int argc, char* argv[]){
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
  HITSTreeReader SWDhits(tree);

  std::unordered_map<int,std::vector<std::pair<Int_t,charge_three_vector>>> hits_by_module = SWDhits.ReadHits(target_event);

  //////////////
  //Merge Hits//
  //////////////
  HitMerger hit_merger;
  std::unordered_map<int, std::vector<std::pair<int, three_vector>>> merged_hits_by_module;
  for(const auto& [key, cp] : hits_by_module){
    merged_hits_by_module[key] = hit_merger.Merge(cp);
  }

  for(const auto& [lay, hitv] : hits_by_module){
    std::cout << "layer" << lay << std::endl;
    for(const auto& hits : hitv){
      std::cout << "charge:" << hits.first << ", " << "pos.z " << hits.second.z << std::endl;
    }
  }
  return 0;

}
