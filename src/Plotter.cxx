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

void Plotter::SetVertexZY(const three_vector& vertex){
  g_vertexZY->SetPoint(0, vertex.z, vertex.y);
  g_vertexZY->SetMarkerStyle(24);
  g_vertexZY->SetMarkerSize(1.0);
  g_vertexZY->SetMarkerColor(kBlack);
}

void Plotter::SetVertexZX(const three_vector& vertex){
  g_vertexZX->SetPoint(0, vertex.z, vertex.x);
  g_vertexZX->SetMarkerStyle(24);
  g_vertexZX->SetMarkerSize(1.0);
  g_vertexZX->SetMarkerColor(kBlack);
}

void Plotter::SetPIDHitsZY(
    const std::vector<two_vector>& proton, const std::vector<two_vector>& electron, const std::vector<two_vector>& muon, const std::vector<two_vector>& pion){
  SetHits(proton, g_protonZY);
  SetHits(electron, g_electronZY);
  SetHits(muon, g_muonZY);
  SetHits(pion, g_pionZY);
}

void Plotter::SetPIDHitsZX(
    const std::vector<two_vector>& proton, const std::vector<two_vector>& electron, const std::vector<two_vector>& muon, const std::vector<two_vector>& pion){
  SetHits(proton, g_protonZX);
  SetHits(electron, g_electronZX);
  SetHits(muon, g_muonZX);
  SetHits(pion, g_pionZX);
}

void Plotter::SetTracks(
    const std::unordered_map<int, std::vector<std::pair<int, two_vector>>>& recon_track, std::vector<TGraph*>& g_vector){

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

void Plotter::Set3DReconTrack(
    const std::vector<std::pair<std::vector<two_vector>, std::vector<two_vector>>>& recon3Dhits){

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
  for(const auto& x_y_hits : recon3Dhits){
    TGraph *g_zy = new TGraph();
    TGraph *g_zx = new TGraph();
    int ip=0;
    for(const auto& hit : x_y_hits.first){
      g_zy->SetPoint(ip, hit.x, hit.y);
      ip++;
    }
    ip=0;
    for(const auto& hit : x_y_hits.second){
      g_zx->SetPoint(ip, hit.x, hit.y);
      ip++;
    }
    g_zy->SetLineColor(colors[icolor]);
    g_zx->SetLineColor(colors[icolor]);
    g_zy->SetLineWidth(3);
    g_zx->SetLineWidth(3);
    gs_3DreconZY.push_back(g_zy);
    gs_3DreconZX.push_back(g_zx);

    icolor++;
    if(icolor >= 8) icolor=0;

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

void Plotter::PlotTrueAndRecon(TGraph *g_primary, TGraph *g_notprimary, std::vector<TGraph*>& g_vector, const char* yaxis){
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
  g_notprimary->SetMarkerColor(kCyan+1);

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
void Plotter::PlotPID(
    TGraph *g_proton, TGraph *g_electron, TGraph *g_muon, TGraph *g_pion, TGraph *g_vertex, const char* yaxis){

  TCanvas *c = new TCanvas("c", "PID track", 800, 600);

  g_proton->SetMarkerColor(kRed);
  g_electron->SetMarkerColor(kCyan+1);
  g_muon->SetMarkerColor(kBlue);
  g_pion->SetMarkerColor(kGreen+2);

  TMultiGraph *mg = new TMultiGraph();
  mg->Add(g_proton, "P");
  mg->Add(g_electron, "P");
  mg->Add(g_muon, "P");
  mg->Add(g_pion, "P");

  //vertex
  mg->Add(g_vertex, "P");

  mg->SetTitle(Form("True Hits; Z [mm];%s [mm]", yaxis));
  mg->Draw("A");

  TLegend *leg = new TLegend(0.7, 0.8, 0.9, 0.89);
  leg->AddEntry(g_proton, "proton", "p");
  leg->AddEntry(g_electron, "electron", "p");
  leg->AddEntry(g_muon, "muon", "p");
  leg->AddEntry(g_pion, "pion", "p");
  leg->SetBorderSize(0);
  leg->Draw();

  c->Update();

}

void Plotter::Plot2DTrue(){
  TCanvas *c = new TCanvas("c", "recon track", 800, 1000);
  c->Divide(1, 2);


  //set true hits Black
  g_primaryZY->SetMarkerColor(kRed);
  g_primaryZX->SetMarkerColor(kRed);
  g_notprimaryZY->SetMarkerColor(kCyan+1);
  g_notprimaryZX->SetMarkerColor(kCyan+1);

  TMultiGraph *mg_ZY = new TMultiGraph();
  TMultiGraph *mg_ZX = new TMultiGraph();

  //true hits
  mg_ZX->Add(g_primaryZX, "P");
  mg_ZX->Add(g_notprimaryZX, "P");
  mg_ZY->Add(g_primaryZY, "P");
  mg_ZY->Add(g_notprimaryZY, "P");

  mg_ZX->SetTitle("True Hits;Z [mm];X [mm]");
  mg_ZY->SetTitle("True Hits;Z [mm];Y [mm]");

  c->cd(1);
  mg_ZX->GetXaxis()->SetLimits(-900, 350);
  mg_ZX->Draw("A");
  c->cd(2);
  mg_ZY->GetXaxis()->SetLimits(-900, 350);
  mg_ZY->Draw("A");

  c->Update();
}

void Plotter::Plot2DRecon(){
  TCanvas *c = new TCanvas("c", "recon track", 800, 1000);
  c->Divide(1, 2);


  //set true hits Black
  g_primaryZY->SetMarkerColor(kBlack);
  g_primaryZX->SetMarkerColor(kBlack);
  g_notprimaryZY->SetMarkerColor(kBlack);
  g_notprimaryZX->SetMarkerColor(kBlack);


  TMultiGraph *mg_ZY = new TMultiGraph();
  TMultiGraph *mg_ZX = new TMultiGraph();

  //recon track
  for(auto* g : gs_reconZX){
    mg_ZX->Add(g, "L");
  }
  for(auto* g : gs_reconZY){
    mg_ZY->Add(g, "L");
  }

  //true hits
  mg_ZX->Add(g_primaryZX, "P");
  mg_ZX->Add(g_notprimaryZX, "P");
  mg_ZY->Add(g_primaryZY, "P");
  mg_ZY->Add(g_notprimaryZY, "P");

  mg_ZX->SetTitle("Reconstructed tracks;Z [mm];X [mm]");
  mg_ZY->SetTitle("Reconstructed tracks;Z [mm];Y [mm]");

  c->cd(1);
  mg_ZX->GetXaxis()->SetLimits(-900, 350);
  mg_ZX->Draw("A");
  c->cd(2);
  mg_ZY->GetXaxis()->SetLimits(-900, 350);
  mg_ZY->Draw("A");

  c->Update();


}

void Plotter::Plot3DRecon(){
  TCanvas *c = new TCanvas("c", "recon track", 800, 1000);
  c->Divide(1, 2);


  //set true hits Black
  g_primaryZY->SetMarkerColor(kBlack);
  g_primaryZX->SetMarkerColor(kBlack);
  g_notprimaryZY->SetMarkerColor(kBlack);
  g_notprimaryZX->SetMarkerColor(kBlack);


  TMultiGraph *mg_ZY = new TMultiGraph();
  TMultiGraph *mg_ZX = new TMultiGraph();

  //recon track
  for(auto* g : gs_3DreconZX){
    mg_ZX->Add(g, "L");
  }
  for(auto* g : gs_3DreconZY){
    mg_ZY->Add(g, "L");
  }

  //true hits
  mg_ZX->Add(g_primaryZX, "P");
  mg_ZX->Add(g_notprimaryZX, "P");
  mg_ZY->Add(g_primaryZY, "P");
  mg_ZY->Add(g_notprimaryZY, "P");

  mg_ZX->SetTitle("Reconstructed tracks;Z [mm];X [mm]");
  mg_ZY->SetTitle("Reconstructed tracks;Z [mm];Y [mm]");

  c->cd(1);
  mg_ZX->GetXaxis()->SetLimits(-900, 350);
  mg_ZX->Draw("A");
  c->cd(2);
  mg_ZY->GetXaxis()->SetLimits(-900, 350);
  mg_ZY->Draw("A");

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

void Plotter::Plot3DReconZY(){
  PlotTrueAndRecon(g_primaryZY, g_notprimaryZY, gs_3DreconZY, "Y");
}

void Plotter::Plot3DReconZX(){
  PlotTrueAndRecon(g_primaryZX, g_notprimaryZX, gs_3DreconZX, "X");
}

void Plotter::PlotPIDZY(){
  PlotPID(g_protonZY, g_electronZY, g_muonZY, g_pionZY, g_vertexZY, "Y");
}

void Plotter::PlotPIDZX(){
  PlotPID(g_protonZX, g_electronZX, g_muonZX, g_pionZX, g_vertexZX, "X");
}



