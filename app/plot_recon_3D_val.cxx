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
#include "Evaluation.hxx"


int main(){
  ////////////////////////////
  //Prepare output root file//
  ////////////////////////////
  TFile *outfile = new TFile("~/yasushi/reconperformance.root", "recreate");
  TTree *outtree = new TTree("ReconEfficiency", "reconstruction efficiency");
  Float_t _efficiency;
  outtree->Branch("efficiency", &_efficiency);
  for(int target_file=0; target_file<50; target_file++){
    for(int target_event=0; target_event<99; target_event++){



      //////////////////
      //Open ROOT File//
      //////////////////
      TFile* _file0 = TFile::Open(Form("/home/daiki/yasushi/nd280/mc/Pgun/monoEmonoDir/e-500-pgun/gun_e-_500MeV_%04d_tree.root", target_file));
      TTree* tree = (TTree*)_file0->Get("swdhits");

      HitMerger hit_merger;
      Sort sort_hits;
      TrackReconstructor track_reconstructor;
      Recon3D track_reconstructor3D;
      Plotter plotevent;
      Evaluation evaluation;
      ParameterMessenger messenger;
      //Read Macro file
      /*
         if(argc == 4){
         std::string MacroFileName = Form("../Settings/%s", argv[3]);
         messenger.LoadFile(MacroFileName, track_reconstructor, track_reconstructor3D, evaluation);
         }
       */

      //////////////////////
      //Read hits and save//
      //////////////////////
      HITSTreeReader SWDhits(tree, target_event);

      std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> hits_by_module_ZY = SWDhits.GetAllHitsZY();
      std::unordered_map<int,std::vector<std::pair<Int_t,charge_two_vector>>> hits_by_module_ZX = SWDhits.GetAllHitsZX();

      //////////////
      //Merge Hits//
      //////////////
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
      std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits_ZY = sort_hits.ForwardSort(merged_hits_by_module_ZY);
      std::vector<std::vector<std::pair<int, two_vector>>> sorted_hits_ZX = sort_hits.ForwardSort(merged_hits_by_module_ZX);

      //////////////////
      //Reconstruction//
      //////////////////
      std::unordered_map<int, std::vector<std::pair<int, two_vector>>> recon_track_ZY = track_reconstructor.Reconstruct(sorted_hits_ZY);
      std::unordered_map<int, std::vector<std::pair<int, two_vector>>> recon_track_ZX = track_reconstructor.Reconstruct(sorted_hits_ZX);

      /////////////////////
      //3D Reconstruction//
      /////////////////////
      std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>> matched_track = track_reconstructor3D.Recon(recon_track_ZX, recon_track_ZY);

      //////////////
      //Evaluation//
      //////////////
      TFile* _G4file = TFile::Open(Form("/home/daiki/yasushi/DetSim/EventDisplay/20260116NI/Pgun/monoEmonoDir/e-500-pgun/TrueG4Track/TrueG4Track_%04d.root", target_file));
      TTree* TrueTrackTree = (TTree*)_G4file->Get("track");
      _efficiency = evaluation.CalEff(target_event, TrueTrackTree, matched_track);
      outtree->Fill();

      _file0->Close();
      _G4file->Close();
    }
  }
  outfile->cd();
  outtree->Write();
  outfile->Close();



  return 0;

}
