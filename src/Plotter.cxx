#include "Plotter.hxx"

//Setting Points
void Plotter::SetHits(const std::vector<two_vector>& hits, TGraph *g){
  int ip = 0;
  for(const auto& h : hits){
    g->SetPoint(ip, h.x, h.y);
    ip++;
  }
  g->SetMarkerSize(0.4);
  g->SetMarkerStyle(20);
}

void Plotter::SetPrimaryHitsZY(const std::vector<two_vector>& hits){
  SetHits(hits, g_primaryZY);
}

void Plotter::SetNotPrimaryHitsZY(const std::vector<two_vector>& hits){
  SetHits(hits, g_notprimaryZY);
}

void Plotter::SetPrimaryHitsZX(const std::vector<two_vector>& hits){
  SetHits(hits, g_primaryZX);
}

void Plotter::SetNotPrimaryHitsZX(const std::vector<two_vector>& hits){
  SetHits(hits, g_notprimaryZX);
}

void SetTracks(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_track, std::vector<TGraph*> g_vector){

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
      g->SetPoint(ip, hit.second.x, hit.second.y);
      ip++;
    }
    g->SetLineColor(colors[icolor]);
    g->SetLineWidth(3);
    g_vector.push_back(g);

    icolor++;
    if(icolor >=8) icolor=0;
  }


}

void Plotter::SetReconTrackZY(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_track){
  SetTracks(recon_track, gs_reconZY);
}

void Plotter::SetReconTrackZX(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_track){
  SetTracks(recon_track, gs_reconZX);
}

void Plotter::PlotTrueAndRecon(TGraph *g_primary, TGraph *g_notprimary, std::vector<TGraph*> g_vector, const char* yaxis){
  TCanvas *c = new TCanvas("c", "recon track", 800, 600);

  //set true hits Black
  g_primary->SetMarkerColor(kBlack);
  g_notprimary->SetMarkerColor(kBlack);

  TMultiGraph *mg = new TMultiGraph();

  //recon track
  for(auto* g : g_vector){
    mg->Add(g, "L");
  }

  //true hits
  mg->Add(g_primary, "P");
  mg->Add(g_notprimary, "P");

  mg->SetTitle(Form("Reconstructed tracks;Z [mm];%s [mm]", yaxis));
  mg->Draw("A");

  c->Update();
}

void Plotter::PlotTrue(TGraph *g_primary, TGraph *g_notprimary, const char* yaxis){
  TCanvas *c = new TCanvas("c", "recon track", 800, 600);
  //set primary->red , not->black
  g_primary->SetMarkerColor(kRed);
  g_notprimary->SetMarkerColor(kCyan);

  TMultiGraph *mg = new TMultiGraph();
  mg->Add(g_primary, "P");
  mg->Add(g_notprimary, "P");

  mg->SetTitle(Form("True Hits; Z [mm];%s [mm]", yaxis));
  mg->Draw("A");

  TLegend *leg = new TLegend(0.7, 0.8, 0.9, 0.89);
  leg->AddEntry(g_primary, "primary hit", "p");
  leg->AddEntry(g_notprimary, "not primary hit", "p");
  leg->SetBorderSize(0);
  leg->Draw();

  c->Update();
}

void Plotter::PlotTrueAndReconZY(){
  PlotTrueAndRecon(g_primaryZY, g_notprimaryZY, gs_reconZY, "Y");
}

void Plotter::PlotTrueAndReconZX(){
  PlotTrueAndRecon(g_primaryZX, g_notprimaryZX, gs_reconZX, "X");
}

void Plotter::PlotTrueZY(){
  PlotTrue(g_primaryZY, g_notprimaryZY, "Y");
}

void Plotter::PlotTrueZX(){
  PlotTrue(g_primaryZX, g_notprimaryZX, "X");
}



