void outputFile()
{
//=========Macro generated from canvas: cCL/cCL
//=========  (Thu May 29 15:23:34 2025) by ROOT version 6.22/09
   TCanvas *cCL = new TCanvas("cCL", "cCL",0,0,600,450);
   gStyle->SetOptStat(0);
   cCL->SetHighLightColor(2);
   cCL->Range(0,0,1,1);
   cCL->SetFillColor(0);
   cCL->SetBorderMode(0);
   cCL->SetBorderSize(2);
   cCL->SetTickx(1);
   cCL->SetTicky(1);
   cCL->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: plotPad
   TPad *plotPad = new TPad("plotPad", "plotPad",0,0,1,1);
   plotPad->Draw();
   plotPad->cd();
   plotPad->Range(310.8281,-5.808881,1590.627,-1.576791);
   plotPad->SetFillColor(0);
   plotPad->SetBorderMode(0);
   plotPad->SetBorderSize(2);
   plotPad->SetLogy();
   plotPad->SetTickx(1);
   plotPad->SetTicky(1);
   plotPad->SetLeftMargin(0.14);
   plotPad->SetRightMargin(0.063);
   plotPad->SetBottomMargin(0.12);
   plotPad->SetFrameBorderMode(0);
   plotPad->SetFrameBorderMode(0);
   
   TH1F *DummyGraph = new TH1F("DummyGraph","",100,500,1500);
   DummyGraph->SetMaximum(0.01);
   DummyGraph->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   DummyGraph->SetLineColor(ci);
   DummyGraph->GetXaxis()->SetTitle("Mass [GeV]");
   DummyGraph->GetXaxis()->SetRange(0,101);
   DummyGraph->GetXaxis()->SetLabelFont(42);
   DummyGraph->GetXaxis()->SetLabelSize(0.055);
   DummyGraph->GetXaxis()->SetTitleSize(0.055);
   DummyGraph->GetXaxis()->SetTitleOffset(1.05);
   DummyGraph->GetXaxis()->SetTitleFont(42);
   DummyGraph->GetYaxis()->SetTitle("#sigma(pp#rightarrow H^{++}H^{--}) [pb]");
   DummyGraph->GetYaxis()->SetLabelFont(42);
   DummyGraph->GetYaxis()->SetLabelSize(0.055);
   DummyGraph->GetYaxis()->SetTitleSize(0.055);
   DummyGraph->GetYaxis()->SetTitleOffset(1.3);
   DummyGraph->GetYaxis()->SetTitleFont(42);
   DummyGraph->GetZaxis()->SetLabelFont(42);
   DummyGraph->GetZaxis()->SetTitleOffset(1);
   DummyGraph->GetZaxis()->SetTitleFont(42);
   DummyGraph->Draw("");
   
   Double_t Graph0_fx1[20] = {
   600,
   700,
   800,
   900,
   1000,
   1100,
   1200,
   1300,
   1400,
   1500,
   1500,
   1400,
   1300,
   1200,
   1100,
   1000,
   900,
   800,
   700,
   600};
   Double_t Graph0_fy1[20] = {
   1.284558e-05,
   1.334914e-05,
   1.190561e-05,
   1.349289e-05,
   1.323688e-05,
   1.330067e-05,
   1.281186e-05,
   1.241162e-05,
   1.205214e-05,
   1.137517e-05,
   1.537315e-05,
   1.793608e-05,
   1.987441e-05,
   1.92105e-05,
   2.866807e-05,
   1.845969e-05,
   2.240297e-05,
   2.074536e-05,
   1.851146e-05,
   3.102673e-05};
   TGraph *graph = new TGraph(20,Graph0_fx1,Graph0_fy1);
   graph->SetName("Graph0");
   graph->SetTitle("Graph");

   ci = TColor::GetColor("#ffcc00");
   graph->SetFillColor(ci);
   graph->SetFillStyle(1000);
   
   TH1F *Graph_Graph01 = new TH1F("Graph_Graph01","Graph",100,510,1590);
   Graph_Graph01->SetMinimum(9.410012e-06);
   Graph_Graph01->SetMaximum(3.299188e-05);
   Graph_Graph01->SetDirectory(0);
   Graph_Graph01->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph01->SetLineColor(ci);
   Graph_Graph01->GetXaxis()->SetLabelFont(42);
   Graph_Graph01->GetXaxis()->SetTitleOffset(1);
   Graph_Graph01->GetXaxis()->SetTitleFont(42);
   Graph_Graph01->GetYaxis()->SetLabelFont(42);
   Graph_Graph01->GetYaxis()->SetTitleFont(42);
   Graph_Graph01->GetZaxis()->SetLabelFont(42);
   Graph_Graph01->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph01);
   
   graph->Draw("f");
   
   Double_t Graph1_fx3001[20] = {
   600,
   700,
   800,
   900,
   1000,
   1100,
   1200,
   1300,
   1400,
   1500,
   1500,
   1400,
   1300,
   1200,
   1100,
   1000,
   900,
   800,
   700,
   600};
   Double_t Graph1_fy3001[20] = {
   1.351207e-05,
   1.411223e-05,
   1.253781e-05,
   1.351617e-05,
   1.335292e-05,
   1.332829e-05,
   1.294892e-05,
   1.27423e-05,
   1.208307e-05,
   1.152122e-05,
   1.537315e-05,
   1.793608e-05,
   1.987441e-05,
   1.92105e-05,
   2.866807e-05,
   1.845969e-05,
   2.240297e-05,
   2.074536e-05,
   1.851146e-05,
   3.102673e-05};
   Double_t Graph1_felx3001[20] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph1_fely3001[20] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph1_fehx3001[20] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph1_fehy3001[20] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20,Graph1_fx3001,Graph1_fy3001,Graph1_felx3001,Graph1_fehx3001,Graph1_fely3001,Graph1_fehy3001);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00cc00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);
   
   TH1F *Graph_Graph13001 = new TH1F("Graph_Graph13001","Graph",100,510,1590);
   Graph_Graph13001->SetMinimum(9.570665e-06);
   Graph_Graph13001->SetMaximum(3.297728e-05);
   Graph_Graph13001->SetDirectory(0);
   Graph_Graph13001->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph13001->SetLineColor(ci);
   Graph_Graph13001->GetXaxis()->SetLabelFont(42);
   Graph_Graph13001->GetXaxis()->SetTitleOffset(1);
   Graph_Graph13001->GetXaxis()->SetTitleFont(42);
   Graph_Graph13001->GetYaxis()->SetLabelFont(42);
   Graph_Graph13001->GetYaxis()->SetTitleFont(42);
   Graph_Graph13001->GetZaxis()->SetLabelFont(42);
   Graph_Graph13001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph13001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph13001);
   
   grae->Draw("f");
   
   Double_t Graph2_fx2[10] = {
   600,
   700,
   800,
   900,
   1000,
   1100,
   1200,
   1300,
   1400,
   1500};
   Double_t Graph2_fy2[10] = {
   2.074909e-05,
   1.562291e-05,
   1.36172e-05,
   1.620364e-05,
   1.400381e-05,
   1.680404e-05,
   1.360217e-05,
   1.501737e-05,
   1.233854e-05,
   1.196516e-05};
   graph = new TGraph(10,Graph2_fx2,Graph2_fy2);
   graph->SetName("Graph2");
   graph->SetTitle("Graph");
   graph->SetFillStyle(1000);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);
   
   TH1F *Graph_Graph22 = new TH1F("Graph_Graph22","Graph",100,510,1590);
   Graph_Graph22->SetMinimum(1.108676e-05);
   Graph_Graph22->SetMaximum(2.162748e-05);
   Graph_Graph22->SetDirectory(0);
   Graph_Graph22->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph22->SetLineColor(ci);
   Graph_Graph22->GetXaxis()->SetLabelFont(42);
   Graph_Graph22->GetXaxis()->SetTitleOffset(1);
   Graph_Graph22->GetXaxis()->SetTitleFont(42);
   Graph_Graph22->GetYaxis()->SetLabelFont(42);
   Graph_Graph22->GetYaxis()->SetTitleFont(42);
   Graph_Graph22->GetZaxis()->SetLabelFont(42);
   Graph_Graph22->GetZaxis()->SetTitleOffset(1);
   Graph_Graph22->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph22);
   
   graph->Draw("lp");
   
   Double_t Graph3_fx3[31] = {
   200,
   250,
   300,
   350,
   400,
   450,
   500,
   550,
   600,
   650,
   700,
   750,
   800,
   850,
   900,
   950,
   1000,
   1050,
   1100,
   1150,
   1200,
   1250,
   1300,
   1350,
   1400,
   1450,
   1500,
   1550,
   1600,
   1650,
   1700};
   Double_t Graph3_fy3[31] = {
   0.0816,
   0.0355,
   0.0173,
   0.00948,
   0.00531,
   0.00317,
   0.00193,
   0.00127,
   0.00083,
   0.00055,
   0.000375,
   0.000266,
   0.00018,
   0.000132,
   9.35e-05,
   6.81e-05,
   4.94e-05,
   3.66e-05,
   2.74e-05,
   2.02e-05,
   1.5e-05,
   1.12e-05,
   8.49e-06,
   6.3e-06,
   4.91e-06,
   3.8e-06,
   2.8e-06,
   2.1e-06,
   1.69e-06,
   1.29e-06,
   1e-06};
   graph = new TGraph(31,Graph3_fx3,Graph3_fy3);
   graph->SetName("Graph3");
   graph->SetTitle("Graph");
   graph->SetFillStyle(1000);
   graph->SetLineColor(2);
   graph->SetLineWidth(3);
   
   TH1F *Graph_Graph33 = new TH1F("Graph_Graph33","Graph",100,50,1850);
   Graph_Graph33->SetMinimum(9e-07);
   Graph_Graph33->SetMaximum(0.0897599);
   Graph_Graph33->SetDirectory(0);
   Graph_Graph33->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph33->SetLineColor(ci);
   Graph_Graph33->GetXaxis()->SetLabelFont(42);
   Graph_Graph33->GetXaxis()->SetTitleOffset(1);
   Graph_Graph33->GetXaxis()->SetTitleFont(42);
   Graph_Graph33->GetYaxis()->SetLabelFont(42);
   Graph_Graph33->GetYaxis()->SetTitleFont(42);
   Graph_Graph33->GetZaxis()->SetLabelFont(42);
   Graph_Graph33->GetZaxis()->SetTitleOffset(1);
   Graph_Graph33->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph33);
   
   graph->Draw("l");
   
   TPaveLabel *pl = new TPaveLabel(0.16,0.89125,0.27,1.0125,"CMS","NBNDC");
   pl->SetBorderSize(0);
   pl->SetFillColor(0);
   pl->SetFillStyle(0);
   pl->SetTextAlign(12);
   pl->SetTextSize(0.99);
   pl->Draw();
   
   pl = new TPaveLabel(0.25,0.915,0.3,0.98,"Preliminary","NBNDC");
   pl->SetBorderSize(0);
   pl->SetFillColor(0);
   pl->SetFillStyle(0);
   pl->SetTextAlign(12);
   pl->SetTextFont(52);
   pl->SetTextSize(0.6);
   pl->Draw();
   
   TLegend *leg = new TLegend(0.5,0.58,0.834885,0.878644,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("Graph2","Exp. 95% CL limit, median","l");
   entry->SetLineColor(1);
   entry->SetLineStyle(2);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph1","Exp. (68%)","F");

   ci = TColor::GetColor("#00cc00");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1000);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph0","Exp. (95%)","F");

   ci = TColor::GetColor("#ffcc00");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1000);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph3","#sigma(pp#rightarrow H^{++}_{L}H^{--}_{L})","lpf");
   entry->SetFillStyle(1000);
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   leg = new TLegend(0.7,0.4,0.9,0.55,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.05);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->Draw();
   
   pl = new TPaveLabel(0.65,0.905,0.9,0.99,"139 fb^{-1} (13 TeV)","NBNDC");
   pl->SetBorderSize(0);
   pl->SetFillColor(0);
   pl->SetTextFont(42);
   pl->SetTextSize(0.5);
   pl->Draw();
   
   TH1F *DummyGraph_copy = new TH1F("DummyGraph_copy","",100,500,1500);
   DummyGraph_copy->SetMaximum(0.01);
   DummyGraph_copy->SetDirectory(0);
   DummyGraph_copy->SetStats(0);

   ci = TColor::GetColor("#000099");
   DummyGraph_copy->SetLineColor(ci);
   DummyGraph_copy->GetXaxis()->SetTitle("Mass [GeV]");
   DummyGraph_copy->GetXaxis()->SetRange(0,101);
   DummyGraph_copy->GetXaxis()->SetLabelFont(42);
   DummyGraph_copy->GetXaxis()->SetLabelSize(0.055);
   DummyGraph_copy->GetXaxis()->SetTitleSize(0.055);
   DummyGraph_copy->GetXaxis()->SetTitleOffset(1.05);
   DummyGraph_copy->GetXaxis()->SetTitleFont(42);
   DummyGraph_copy->GetYaxis()->SetTitle("#sigma(pp#rightarrow H^{++}H^{--}) [pb]");
   DummyGraph_copy->GetYaxis()->SetLabelFont(42);
   DummyGraph_copy->GetYaxis()->SetLabelSize(0.055);
   DummyGraph_copy->GetYaxis()->SetTitleSize(0.055);
   DummyGraph_copy->GetYaxis()->SetTitleOffset(1.3);
   DummyGraph_copy->GetYaxis()->SetTitleFont(42);
   DummyGraph_copy->GetZaxis()->SetLabelFont(42);
   DummyGraph_copy->GetZaxis()->SetTitleOffset(1);
   DummyGraph_copy->GetZaxis()->SetTitleFont(42);
   DummyGraph_copy->Draw("sameaxis");
   plotPad->Modified();
   cCL->cd();
   cCL->Modified();
   cCL->cd();
   cCL->SetSelected(cCL);
}
