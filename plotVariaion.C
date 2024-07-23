{
  TFile f("data/PRISMState_FHC_RHC_EVisReco_FluxXSecDet_9Sep22.root");
  auto nom = (TH2F*)f.Get("FDUnSelected_ETrue_numu_nu/pred_nom/extrap/numu_surv/hist");
  TH2F* h2[10];
  for (int i=0;i<10;i++){
    h2[i] = (TH2F*)f.Get(Form("FDUnSelected_ETrue_numu_nu/pred_flux_Nov17_%d_+1/extrap/numu_surv/hist",i));
  }
  new TCanvas();
  h2[0]->Draw("colz");

  TH1D* h1[10];
  for (int i =0;i<10;i++)
    h1[i] = h2[i]->ProjectionX();
  auto hnom = nom->ProjectionX();
  h1[0]->SetLineColor(2);
  hnom->SetLineColor(4);
  new TCanvas();
  h1[0]->Draw("");
  hnom->Draw("same");

  double binedge[101] = {};
  for (int i=0;i<h1[0]->GetNbinsX(); i++){
    binedge[i] = h1[0]->GetBinLowEdge(i+1);
  }
  
  binedge[h1[0]->GetNbinsX()] = h1[0]->GetBinLowEdge(h1[0]->GetNbinsX()+1) + h1[0]->GetBinWidth(h1[0]->GetNbinsX()+1);

  TH1D* hdiff[10];
  for (int i=0;i<10;i++){
    hdiff[i] = new TH1D("","",h1[0]->GetNbinsX(), binedge);
  }
  
  for (int j=0;j<10;j++){
    for(int i=0;i<h1[0]->GetNbinsX(); i ++){
      hdiff[j] -> SetBinContent(i+1, (h1[j]->GetBinContent(i+1) - hnom->GetBinContent(i+1))/ hnom->GetBinContent(i+1));
    }
  }
  TGraph* s1[10];
  for (int i=0;i<10;i++) s1[i] = new TGraph(hdiff[i]);
  TCanvas* c1 = new TCanvas();
  for (int i=0;i<10;i++) s1[i] -> SetLineColor(i);
  c1->Divide(3,4);
  c1->cd(1);
  s1[0]->Draw("ALP");
  for (int i=1;i<10;i++) {
    c1->cd(i+1); 
    s1[i] -> Draw("ALP");
  }
  f.Close();
  TFile* outfile = TFile::Open("flux_uncertainty.root","recreate");
  for (int i=0;i<10;i++){
    s1[i]->Write(Form("flux_variation_%d",i));
  }
  outfile->Write();
  outfile->Close();

  
}
