typedef struct { Float_t px; Float_t py; Float_t pz; Float_t E; } four_momentum;
typedef struct { Float_t x; Float_t y; Float_t z; } three_vector;
typedef struct { Float_t cha; Float_t x; Float_t y; Float_t z; } charge_three_vector;

Float_t charge_mean(vector<pair<Float_t, Float_t>> layer_hits){
  //<charge, pos.y>
  Float_t sum_charge = 0;
  Float_t charge_hit = 0;
  for(const auto& v : layer_hits){
    sum_charge += v.first;
    charge_hit += v.first * v.second;
  }
  return charge_hit / sum_charge;
}

Float_t len2p(three_vector a, three_vector b){
  Float_t length = (a.z-b.z)*(a.z-b.z) + (a.y-b.y)*(a.y-b.y);
  return sqrt(length);
}

Float_t slope(three_vector a, three_vector b){
  return (a.y - b.y) / (a.z - b.z);
}

pair<int, int> min_map(unordered_map<int, unordered_map<int, Float_t>> map){
  Float_t min_value = 10000000000;
  pair<int, int> min_key;
  for(const auto& [nkey, n_map] : map){
    for(const auto& [mkey, m_map] : n_map){
      if(m_map < min_value){
        min_value = m_map;
        min_key = make_pair(nkey,mkey);
      }
    }
  }
  return min_key;
}

unordered_map<int, vector<pair<int,three_vector>>> recon1layer(unordered_map<int, vector<pair<int, three_vector>>> recon_track, vector<pair<int, three_vector>> new_layer){
  Float_t max_slope_change = 15. *M_PI / 180.; //degree
  Float_t max_slope_change_ad = 5. *M_PI / 180.; //degree
  Float_t max_len = 25.; //mm

  unordered_map<int, Float_t> ref_value;
  for(const auto& [key, hit] : recon_track){
    if(hit.size()==1){ref_value[key] = -1;} // this track is not line but point
    else{
      ref_value[key] = atan(slope(hit.back().second, hit.at(hit.size()-2).second));
    }
  }
  unordered_map<int, int> comp_sign; // 1 : complete, 0 : yet
  for(const auto& [key, hit] : recon_track){
    if(abs(new_layer.at(0).second.z - hit.back().second.z) < 30.){comp_sign[key] = 0;}
    else{comp_sign[key] = 1;}
  }
  unordered_map<int, unordered_map<int, Float_t>> p_evaluation_value; //<new hit, <recon track, value>>
  unordered_map<int, unordered_map<int, Float_t>> l_evaluation_value; //<new hit, <recon track, value>>
  unordered_map<int, unordered_map<int, Float_t>> ad_evaluation_value; //<new hit, <recon track, value>>
  for(size_t i = 0; i<new_layer.size(); i++){
    p_evaluation_value[i][-1] = 10000000;
    l_evaluation_value[i][-1] = 10000000;
    ad_evaluation_value[i][-1] = 10000000;
  }
  for(const auto& [key, hit] : recon_track){
    if(comp_sign[key] == 1){continue;}
    Float_t minimum_value = 10000000;
    for(size_t i = 0; i<new_layer.size(); i++){
      three_vector new_hit = new_layer.at(i).second;
      //point
      if(ref_value[key] == -1){
        Float_t length = len2p(hit.back().second, new_hit);
        if(length < max_len){
          p_evaluation_value[i][key] = length;
        }
        if(hit.back().first < 2){continue;}
        Float_t sl = atan(slope(hit.back().second, new_hit));
        Float_t p_sl = atan(hit.back().first-1);
        Float_t m_sl = atan(-1*hit.back().first+1);
        Float_t p_sl_change = abs(p_sl - sl);
        Float_t m_sl_change = abs(m_sl - sl);
        Float_t slope_change = m_sl_change;
        if(p_sl_change < m_sl_change){slope_change = p_sl_change;}
        if(slope_change > max_slope_change_ad){continue;}
        ad_evaluation_value[i][key] = slope_change;
      }
      //line
      else{
        Float_t sl = atan(slope(hit.back().second, new_hit));
        Float_t slope_change = abs(sl - ref_value[key]);
        if(slope_change > max_slope_change){continue;}
        l_evaluation_value[i][key] = slope_change;
      }

    }
  }
  //evaluation print
  for(const auto& [nkey, n_map] : l_evaluation_value){
    for(const auto& [mkey, m_map] : n_map){
      cout << "(" << nkey << ", " << mkey << ") " << m_map << endl;
    }
  }
  cout << "-----------------------------" << endl;
  //line
  while(1){
    // checking if there are any hits to be connected
    int bf = 1;
    for(const auto& [nkey, n_map] : l_evaluation_value){
      for(const auto& [mkey, m_map] : n_map){if(mkey!=-1){bf=0;}}
    }
    if(bf==1){break;}

    pair<int, int> connected_key = min_map(l_evaluation_value);
    recon_track[connected_key.second].push_back(make_pair(new_layer.at(connected_key.first).first, new_layer.at(connected_key.first).second));
    l_evaluation_value.erase(connected_key.first);
    for(auto& [nkey, n_map] : l_evaluation_value){
      n_map.erase(connected_key.second);
    }
    p_evaluation_value.erase(connected_key.first);
    ad_evaluation_value.erase(connected_key.first);

  }
  //point
  while(1){
    // checking if there are any hits to be connected
    int bf = 1;
    for(const auto& [nkey, n_map] : p_evaluation_value){
      for(const auto& [mkey, m_map] : n_map){if(mkey!=-1){bf=0;}}
    }
    if(bf==1){break;}

    pair<int, int> connected_key = min_map(p_evaluation_value);
    recon_track[connected_key.second].push_back(make_pair(new_layer.at(connected_key.first).first, new_layer.at(connected_key.first).second));
    p_evaluation_value.erase(connected_key.first);
    for(auto& [nkey, n_map] : p_evaluation_value){
      n_map.erase(connected_key.second);
    }
    l_evaluation_value.erase(connected_key.first);
    ad_evaluation_value.erase(connected_key.first);

  }
  //merged point use the slope (~45 deg)
  while(1){
    // checking if there are any hits to be connected
    int bf = 1;
    for(const auto& [nkey, n_map] : ad_evaluation_value){
      for(const auto& [mkey, m_map] : n_map){if(mkey!=-1){bf=0;}}
    }
    if(bf==1){break;}

    pair<int, int> connected_key = min_map(ad_evaluation_value);
    recon_track[connected_key.second].push_back(make_pair(new_layer.at(connected_key.first).first, new_layer.at(connected_key.first).second));
    ad_evaluation_value.erase(connected_key.first);
    for(auto& [nkey, n_map] : ad_evaluation_value){
      n_map.erase(connected_key.second);
    }
    l_evaluation_value.erase(connected_key.first);
    p_evaluation_value.erase(connected_key.first);

  }
  //generate new track
  if(p_evaluation_value.empty()==false){

    for(const auto& [key, val] : p_evaluation_value){
      int new_key = 0;
      while(recon_track.find(new_key) != recon_track.end()){new_key++;}
      //recon_track[new_key].push_back(new_layer.at(key));
      recon_track[new_key] = {make_pair(new_layer.at(key).first, new_layer.at(key).second)};
    }
  }

  return recon_track;


}

vector<pair<int, three_vector>> merge_hits(
    vector<pair<Int_t, charge_three_vector>> v
    ){
  vector<pair<int, three_vector>> m_hits;
  vector<bool> used(v.size(), false);

  for(size_t i = 0; i < v.size(); i++){

    if(used[i]) continue;

    vector<pair<Float_t, Float_t>> layer_hits;
    vector<Int_t> merged_fiber;

    // initial fiber
    merged_fiber.push_back(v.at(i).first);
    used[i] = true;

    layer_hits.push_back(
        make_pair(
          v.at(i).second.cha,
          v.at(i).second.y
          )
        );

    //look for near fibers
    size_t current = 0;

    while(current < merged_fiber.size()){

      Int_t fiber = merged_fiber.at(current);

      for(size_t k = 0; k < v.size(); k++){

        if(used[k]) continue;

        if(abs(fiber - v.at(k).first) <= 1){

          merged_fiber.push_back(v.at(k).first);
          used[k] = true;

          layer_hits.push_back(
              make_pair(
                v.at(k).second.cha,
                v.at(k).second.y
                )
              );
        }
      }

      current++;
    }

    /*
    //make merged hit
    m_hits.push_back({
    v.at(i).second.x,
    charge_mean(layer_hits),
    v.at(i).second.z
    });
     */
    m_hits.push_back(make_pair(layer_hits.size(), three_vector{v.at(i).second.x, charge_mean(layer_hits), v.at(i).second.z}));
  }

  return m_hits;
}

void plot_recon(int target_file=0, int target_event=0, int plot_mode=0){
  TFile *f_temp = TFile::Open("/home/daiki/yasushi/nd280/mc/20260116-neuth10-nd320kAflux/14.36mc/Tree/FHCnue/FHCnue_0000_tree.root");
  TTree *t_temp = (TTree*)f_temp->Get("swdhits");

  int max_filenum = 49;
  int max_eventnum = t_temp->GetMaximum("event");
  f_temp->Close();

  for(int f=0; f<=max_filenum; f++){
    if(f!=target_file){continue;}
    TFile *_file0 = TFile::Open(Form("/home/daiki/yasushi/nd280/mc/20260116-neuth10-nd320kAflux/14.36mc/Tree/FHCnue/FHCnue_%04d_tree.root", f));
    TTree *swdhits = (TTree*)_file0->Get("swdhits");
    int nentries = swdhits->GetEntries();

    Int_t event;
    Int_t trackId;
    Int_t PDG;
    Int_t primaryTrackId;
    Int_t SWDFiberDirection;
    Int_t SWDFiberType;
    Int_t SWDFiberModule;
    Int_t SWDFiberNumber;
    Float_t charge;
    three_vector pos;
    three_vector primaryVertex;
    three_vector truePos;
    four_momentum initialMomentum;
    four_momentum primaryInitialMomentum;

    swdhits->SetBranchAddress("event", &event);
    swdhits->SetBranchAddress("trackId", &trackId);
    swdhits->SetBranchAddress("PDG", &PDG);
    swdhits->SetBranchAddress("primaryTrackId", &primaryTrackId);
    swdhits->SetBranchAddress("SWDFiberDirection", &SWDFiberDirection);
    swdhits->SetBranchAddress("SWDFiberType", &SWDFiberType);
    swdhits->SetBranchAddress("SWDFiberModule", &SWDFiberModule);
    swdhits->SetBranchAddress("SWDFiberNumber", &SWDFiberNumber);
    swdhits->SetBranchAddress("initialMomentum", &initialMomentum);
    swdhits->SetBranchAddress("pos", &pos);
    swdhits->SetBranchAddress("truePos", &truePos);
    swdhits->SetBranchAddress("charge", &charge);
    swdhits->SetBranchAddress("primaryVertex", &primaryVertex);
    swdhits->SetBranchAddress("primaryInitialMomentum", &primaryInitialMomentum);


    unordered_map<int,vector<pair<Int_t,charge_three_vector>>> hits_by_module;
    unordered_map<int,vector<pair<int, three_vector>>> merged_hits_by_module;
    vector<three_vector> primary_hits;
    vector<three_vector> not_primary_hits;

    for(int eve=0; eve<=max_eventnum; eve++){
      if(eve!=target_event){continue;}
      hits_by_module.clear();
      merged_hits_by_module.clear();
      for(int i=0; i<nentries; i++){
        swdhits->GetEntry(i);
        if(event!=eve){continue;}
        if(trackId==-1){continue;}
        if(!(SWDFiberType == 0 && SWDFiberDirection == 0)){continue;}
        hits_by_module[SWDFiberModule].push_back(make_pair(SWDFiberNumber, charge_three_vector{charge, pos.x, pos.y, pos.z}));

        //Fill hits, deviding primary or not
        if(trackId == primaryTrackId){
          primary_hits.push_back(three_vector{pos.x, pos.y, pos.z});
        }
        if(trackId != primaryTrackId){
          not_primary_hits.push_back(three_vector{pos.x, pos.y, pos.z});
        }

      }
      /*
         for(const auto& [key, cp] : hits_by_module){
         cout << "----------key:" << key << endl;
         for(const auto& vv : cp){
         cout << vv.second.y << endl;
         }
         }
       */
      for(const auto& [key, cp] : hits_by_module){
        merged_hits_by_module[key] = merge_hits(cp);
      }
      /*
         cout << "************* Merged!! **************" << endl;
         for(const auto& [key, cp] : merged_hits_by_module){
         cout << "----------key:" << key << endl;
         for(const auto& vv : cp){
         cout << vv.y << ", " << vv.z << endl;
         }
         }
       */

      //sort by z layer
      vector<vector<pair<int, three_vector>>> sorted_hits;

      vector<int> keys;
      for(const auto& [key, hits] : merged_hits_by_module){
        keys.push_back(key);
      }

      sort(keys.begin(), keys.end(), greater<int>());

      for(const auto& key : keys){
        sorted_hits.push_back(merged_hits_by_module.at(key));
      }
      //yasushi
      for (const auto& [key, cp] : hits_by_module){
        for(const auto& v : cp){
          cout << "(" << v.second.z << ", " << v.second.y << endl;
        }

      }
      for(const auto& [key, cp] : hits_by_module){
        merged_hits_by_module[key] = merge_hits(cp);
      }
      for(const auto& [key, cp] : merged_hits_by_module){
        cout << "----key---- " << key << endl;
        for(const auto& v : cp){
          cout << v.first << " (" << v.second.z << ", " << v.second.y << ")" << endl;
        }
      }
      //yasushi end

      //plot merged hits
      int ip=0;


      TGraphErrors *g_original = new TGraphErrors;
      if(plot_mode==0){
        for(const auto& hit_lay : sorted_hits){
          for(const auto& hit : hit_lay){
            g_original->SetPoint(ip, hit.second.z, hit.second.y);
            ip++;
          }
        }
        g_original->SetMarkerSize(0.4);
        g_original->SetMarkerStyle(20);
        //g_original->Draw("AP");
      }

      //reconstruction
      int track_num = 0;
      unordered_map<int, vector<pair<int, three_vector>>> recon_track;
      for(const auto& hit : sorted_hits.at(0)){
        recon_track[track_num].push_back(hit);
        track_num++;
      }
      //cout << "first hit : " << sorted_hits.at(0).at(0).z << endl;
      sorted_hits.erase(sorted_hits.begin());
      for(const auto& hit_lay : sorted_hits){
        recon_track = recon1layer(recon_track, hit_lay);
      }
      for(const auto& [key, hit_lay] : recon_track){
        cout << "key : " << key << endl;
        for(const auto& hit : hit_lay){
          cout << "(" << hit.second.z << ", " << hit.second.y << ")" << endl;
        }
      }
      TCanvas *c = new TCanvas("c", "recon track", 800, 600);

      TMultiGraph *mg = new TMultiGraph();

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

        int ip = 0;
        for(const auto& hit : hits){
          g->SetPoint(ip, hit.second.z, hit.second.y);
          ip++;
        }

        cout << "key : " << key
          << ", N = " << g->GetN() << endl;

        g->SetLineColor(colors[icolor]);
        g->SetLineWidth(3);

        mg->Add(g, "L");

        icolor++;
        if(icolor >= 8)
          icolor = 0;
      }
      //mg->Add(ghits_primary, "AP");
      //mg->Add(ghits_not_primary, "AP");
      mg->Add(g_original, "AP");

      mg->SetTitle("Reconstructed tracks;Z [mm];Y [mm]");
      mg->Draw("A");

      c->Update();



      //reconstruction
      /*
         int recon_num = 0;
         unordered_map<int, vector<three_vector>> temp_recon_track;
         unordered_map<int, vector<three_vector>> complete_recon_track;
         for(const auto& hit : sorted_hits.at(0)){
         temp_recon_track[recon_num].push_back(hit);
         recon_num++;
         }
         sorted_hits.erase(sorted_hits.begin());
         unordered_map<int, Float_t> min_len_map;
         for(const auto& [key, first_hit] : temp_recon_track){
         Float_t min_len = 100000000;
         for(const auto& hit : sorted_hits.at(0)){
         Float_t len = len2p(temp_recon_track.at(0), hit);
         if(len < min_len){
         min_len = len;
         if(min_len > 50.){continue;}
         temp_recon_track[key].at(1) = hit;
         min_len_map[key] = min_len;
         }
         }
         }
       */



      /*
      //reconstruct track by selecting (likely) hit
      vector<three_vector> selected_hits;

      cout << sorted_hits.size() << endl;
      //reconstruct first and second hit
      for(size_t i=0; i<2; i++){
      Float_t min_len = 10000;
      three_vector temp_v;
      for(const auto& v : sorted_hits.at(i)){
      Float_t len = abs(v.y);
      if (min_len > len){
      temp_v = three_vector{v.x, v.y, v.z};
      min_len = len;
      }
      }
      selected_hits.push_back(temp_v); 
      }

      //reconstruct hits
      Float_t reject_theta = 15.;
      Float_t slope_weight = 0.7;
      Float_t ref_slope_sum = (selected_hits.at(1).y - selected_hits.at(0).y) / (selected_hits.at(1).z - selected_hits.at(0).z);
      Float_t ref_num = 1;
      for(size_t i=2; i<sorted_hits.size(); i++){
      three_vector first_hit = selected_hits.at(i-2);
      three_vector second_hit = selected_hits.at(i-1);
      Float_t ref_a = (second_hit.y - first_hit.y) / (second_hit.z - first_hit.z);

      //cal reference slope : previous slopes * weight + one before slope;
      ref_num += 1 * slope_weight;
      ref_slope_sum = slope_weight * ref_slope_sum + ref_a;
      Float_t ref_slope = ref_slope_sum / ref_num;

      Float_t ref_theta = atan(ref_slope)*180./M_PI;
      Float_t min_diff_a = 10000000;

      three_vector temp_hit;
      int hit_flag = 0;
      for(const auto& hit : sorted_hits.at(i)){
      Float_t a = (hit.y - second_hit.y) / (hit.z - second_hit.z);
      Float_t theta = atan(a)*180./M_PI;
      if(abs(ref_theta - theta) > reject_theta){continue;}
      if(abs(ref_slope - a) < min_diff_a){
      min_diff_a = abs(ref_slope - a);
      temp_hit = hit;
      hit_flag = 1;
      }
      }
      if((temp_hit.z - selected_hits.back().z) > 30.){break;}
      if(hit_flag == 0){break;}
      selected_hits.push_back(temp_hit);
      }
       */




    }
  }
}
