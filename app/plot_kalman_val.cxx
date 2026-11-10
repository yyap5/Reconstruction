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
#include "KalmanFitReader.hxx"


int main(int argc, char* argv[]){
  ////////////////////////////
  //Prepare output root file//
  ////////////////////////////
  TFile *outfile = new TFile("~/yasushi/reconperformance_kalman.root", "recreate");
  TTree *outtree = new TTree("ReconEfficiency", "reconstruction efficiency");
  Float_t _efficiency;
  outtree->Branch("efficiency", &_efficiency);
  for(int target_file=0; target_file<50; target_file++){
    for(int target_event=0; target_event<99; target_event++){



      ///////////////////////////
      //Open Kalman Fitted File//
      ///////////////////////////
      TFile* _file0 = TFile::Open(Form("/home/daiki/yasushi/ML/kalmanfilter/Pgun/monoEmonoDir/e-500-pgun/fitroot/fitpos_%04d.root", target_file));
      TTree* tree = (TTree*)_file0->Get("fitpos");

      //////////////////////
      //Read hits and save//
      /////////////////////
      KalmanFitReader Reader(tree, target_event);

      HitMerger hit_merger;
      Sort sort_hits;
      TrackReconstructor track_reconstructor;
      Recon3D track_reconstructor3D;
      Plotter plotevent;
      Evaluation evaluation;
      ParameterMessenger messenger;
      //Read Macro File
      if(argc == 2){
        std::string MacroFileName = Form("../Settings/%s", argv[1]);
        messenger.LoadFile(MacroFileName, track_reconstructor, track_reconstructor3D, evaluation);
      }


      //////////////
      //Evaluation//
      //////////////
      TFile* _G4file = TFile::Open(Form("/home/daiki/yasushi/DetSim/EventDisplay/20260116NI/Pgun/monoEmonoDir/e-500-pgun/TrueG4Track/TrueG4Track_%04d.root", target_file));
      TTree* TrueTrackTree = (TTree*)_G4file->Get("track");
      _efficiency = evaluation.CalEff(target_event, TrueTrackTree, Reader.GetFit3D(),
          Reader.GetShiftX(), Reader.GetShiftY(), Reader.GetShiftZ());
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
