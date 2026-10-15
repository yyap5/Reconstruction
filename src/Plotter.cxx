#include "Plotter.hxx"

void Plotter::SetPrimaryHitsZY(const std::vector<three_vector>& hits){
  int ip = 0;

  for(const auto& h : hits){
    g_primaryZY->SetPoint(ip, h.z, h.y);
    ip++;
  }
  g_primaryZY->SetMarkerSize(0.4);
  g_primaryZY->SetMarkerStyle(20);

}

void Plotter::SetNotPrimaryHitsZY(const std::vector<three_vector>& hits){
  int ip = 0;

  for(const auto& h : hits){
    g_notprimaryZY->SetPoint(ip, h.z, h.y);
    ip++;
  }
  g_notprimaryZY->SetMarkerSize(0.4);
  g_notprimaryZY->SetMarkerStyle(20);

}

void Plotter::SetReconTrack(
    const std::unordered_map<int, std::vector<std::pair<int, three_vector>>>& recon_track){

  //recon track color table
  int colors[] = {
    kRed,
    kBlue,
    kGreen+2,
    kMagenta,
    kCyan+1,
    kOrange+1,
    kViolet,
    kAzure+1
  };

  int icolor = 0;
  for(const auto& [key, hits] : recon_track){
    TGraph *g = new TGraph();
    int ip=0;
    for(const auto& hit : hits){
      g->SetPoint(ip, hit.second.z, hit.second.y);
      ip++;
    }
    g->SetLineColor(colors[icolor]);
    g->SetLineWidth(3);
    gs_reconZY.push_back(g);

    icolor++;
    if(icolor >=8) icolor=0;
  }


}

void Plotter::PlotTrueAndReconZY(){
  TCanvas *c = new TCanvas("c", "recon track", 800, 600);

  //set true hits Black
  g_primaryZY->SetMarkerColor(kBlack);
  g_notprimaryZY->SetMarkerColor(kBlack);

  TMultiGraph *mg = new TMultiGraph();
  mg->Add(g_primaryZY, "P");
  mg->Add(g_notprimaryZY, "P");

  for(auto* g : gs_reconZY){
    mg->Add(g, "L");
  }
  mg->SetTitle("Reconstructed tracks;Z [mm];Y [mm]");
  mg->Draw("A");

  c->Update();
}

void Plotter::PlotTrueZY(){
  TCanvas *c = new TCanvas("c", "recon track", 800, 600);
  //set primary->red , not->black
  g_primaryZY->SetMarkerColor(kRed);
  g_notprimaryZY->SetMarkerColor(kBlack);

  TMultiGraph *mg = new TMultiGraph();
  mg->Add(g_primaryZY, "P");
  mg->Add(g_notprimaryZY, "P");

  mg->SetTitle("True Hits; Z [mm];Y [mm]");
  mg->Draw("A");

  c->Update();
}



