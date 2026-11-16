#ifdef __CLING__
#pragma cling optimize(0)
#endif
void outputFile()
{
//=========Macro generated from canvas: cCL/cCL
//=========  (Wed Nov 11 15:16:46 2026) by ROOT version 6.36.07
   TCanvas *cCL = new TCanvas("cCL", "cCL", 0, 0, 600, 450);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(1);
   cCL->SetHighLightColor(2);
   TColor::SetPalette(57, nullptr);
   cCL->Range(0,0,1,1);
   cCL->SetFillColor(0);
   cCL->SetBorderMode(0);
   cCL->SetBorderSize(2);
   cCL->SetTickx(1);
   cCL->SetTicky(1);
   cCL->SetFrameBorderMode(0);
   
// ------------>Primitives in pad: plotPad
   TPad *plotPad__0 = new TPad("plotPad", "plotPad", 0, 0, 1, 1);
   plotPad__0->Draw();
   plotPad__0->cd();
   plotPad__0->Range(310.8281,-5.808881,1590.627,-1.576791);
   plotPad__0->SetFillColor(0);
   plotPad__0->SetBorderMode(0);
   plotPad__0->SetBorderSize(2);
   plotPad__0->SetLogy();
   plotPad__0->SetTickx(1);
   plotPad__0->SetTicky(1);
   plotPad__0->SetLeftMargin(0.14);
   plotPad__0->SetRightMargin(0.063);
   plotPad__0->SetBottomMargin(0.12);
   plotPad__0->SetFrameBorderMode(0);
   plotPad__0->SetFrameBorderMode(0);
   
   TH1F *DummyGraph = new TH1F("DummyGraph", "", 100, 500, 1500);
   DummyGraph->SetMaximum(0.01);
   DummyGraph->SetStats(0);
   DummyGraph->SetLineColor(TColor::GetColor("#000099"));
   DummyGraph->GetXaxis()->SetTitle("Mass [GeV]");
   DummyGraph->GetXaxis()->SetRange(0, 101);
   DummyGraph->GetXaxis()->SetLabelFont(42);
   DummyGraph->GetXaxis()->SetLabelSize(0.05499999970197678);
   DummyGraph->GetXaxis()->SetTitleSize(0.05499999970197678);
   DummyGraph->GetXaxis()->SetTitleOffset(1.049999952316284);
   DummyGraph->GetXaxis()->SetTitleFont(42);
   DummyGraph->GetYaxis()->SetTitle("#sigma(pp#rightarrow H^{++}H^{--}) [pb]");
   DummyGraph->GetYaxis()->SetLabelFont(42);
   DummyGraph->GetYaxis()->SetLabelSize(0.05499999970197678);
   DummyGraph->GetYaxis()->SetTitleSize(0.05499999970197678);
   DummyGraph->GetYaxis()->SetTitleOffset(1.299999952316284);
   DummyGraph->GetYaxis()->SetTitleFont(42);
   DummyGraph->GetZaxis()->SetLabelFont(42);
   DummyGraph->GetZaxis()->SetTitleOffset(1);
   DummyGraph->GetZaxis()->SetTitleFont(42);
   DummyGraph->Draw();
   
   std::vector<Double_t> graph_x_vect0{
      500, 500, 600, 600, 700, 700, 800, 800, 900, 900,
      1000, 1000, 1100, 1100, 1200, 1200, 1300, 1300, 1400, 1400,
      1500, 1500, 1500, 1500, 1400, 1400, 1300, 1300, 1200, 1200,
      1100, 1100, 1000, 1000, 900, 900, 800, 800, 700, 700,
      600, 600, 500, 500
   };
   std::vector<Double_t> graph_y_vect1{
      0.000107115, 0.000107115, 6.241600000000001e-05, 6.241600000000001e-05, 3.66375e-05, 3.66375e-05, 2.232e-05, 2.232e-05, 1.52405e-05, 1.52405e-05,
      1.145586e-05, 1.145586e-05, 9.417379999999999e-06, 9.417379999999999e-06, 8.0325e-06, 8.0325e-06, 7.171503e-06, 7.171503e-06, 6.474816999999999e-06, 6.474816999999999e-06,
      5.811959999999999e-06, 5.811959999999999e-06, 5.783568e-05, 5.783568e-05, 6.4431966e-05, 6.4431966e-05, 6.957894599999999e-05, 6.957894599999999e-05, 7.6029e-05, 7.6029e-05,
      8.598119999999999e-05, 8.598119999999999e-05, 9.641892000000001e-05, 9.641892000000001e-05, 0.00011164835, 0.00011164835, 0.000136098, 0.000136098, 0.0001868625, 0.0001868625,
      0.000279876, 0.000279876, 0.00043232, 0.00043232
   };
   TGraph *graph = new TGraph(44, graph_x_vect0.data(), graph_y_vect1.data());
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(TColor::GetColor("#ffcc00"));
   graph->SetFillStyle(1000);
   
   TH1F *Graph_histogram1 = new TH1F("Graph_histogram1", "Graph", 100, 400, 1600);
   Graph_histogram1->SetMinimum(5.230764e-06);
   Graph_histogram1->SetMaximum(0.0004749708040000001);
   Graph_histogram1->SetDirectory(nullptr);
   Graph_histogram1->SetStats(0);
   Graph_histogram1->SetLineColor(TColor::GetColor("#000099"));
   Graph_histogram1->GetXaxis()->SetLabelFont(42);
   Graph_histogram1->GetXaxis()->SetTitleOffset(1);
   Graph_histogram1->GetXaxis()->SetTitleFont(42);
   Graph_histogram1->GetYaxis()->SetLabelFont(42);
   Graph_histogram1->GetYaxis()->SetTitleFont(42);
   Graph_histogram1->GetZaxis()->SetLabelFont(42);
   Graph_histogram1->GetZaxis()->SetTitleOffset(1);
   Graph_histogram1->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_histogram1);
   
   graph->Draw("f");
   
   std::vector<Double_t> grae_fx_vect2{
      500, 500, 600, 600, 700, 700, 800, 800, 900, 900,
      1000, 1000, 1100, 1100, 1200, 1200, 1300, 1300, 1400, 1400,
      1500, 1500, 1500, 1500, 1400, 1400, 1300, 1300, 1200, 1200,
      1100, 1100, 1000, 1000, 900, 900, 800, 800, 700, 700,
      600, 600, 500, 500
   };
   std::vector<Double_t> grae_fy_vect3{
      0.000146101, 0.000146101, 8.7316e-05, 8.7316e-05, 5.20125e-05, 5.20125e-05, 3.3084e-05, 3.3084e-05, 2.368355e-05, 2.368355e-05,
      1.843114e-05, 1.843114e-05, 1.563444e-05, 1.563444e-05, 1.3626e-05, 1.3626e-05, 1.22256e-05, 1.22256e-05, 1.1208057e-05, 1.1208057e-05,
      1.006012e-05, 1.006012e-05, 3.641064e-05, 3.641064e-05, 4.0395061e-05, 4.0395061e-05, 4.3257399e-05, 4.3257399e-05, 4.68705e-05, 4.68705e-05,
      5.210658e-05, 5.210658e-05, 5.765968e-05, 5.765968e-05, 6.869444999999999e-05, 6.869444999999999e-05, 8.708400000000001e-05, 8.708400000000001e-05, 0.0001247625, 0.0001247625,
      0.000194137, 0.000194137, 0.000308607, 0.000308607
   };
   std::vector<Double_t> grae_fexl_vect4{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0
   };
   std::vector<Double_t> grae_fexh_vect5{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0
   };
   std::vector<Double_t> grae_feyl_vect6{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0
   };
   std::vector<Double_t> grae_feyh_vect7{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0
   };
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(44, grae_fx_vect2.data(), grae_fy_vect3.data(), grae_fexl_vect4.data(), grae_fexh_vect5.data(), grae_feyl_vect6.data(), grae_feyh_vect7.data());
   grae->SetName("Graph");
   grae->SetTitle("Graph");
   grae->SetFillColor(TColor::GetColor("#00cc00"));
   grae->SetFillStyle(1000);
   
   TH1F *Graph_histogram2 = new TH1F("Graph_histogram2", "Graph", 100, 400, 1600);
   Graph_histogram2->SetMinimum(9.054108e-06);
   Graph_histogram2->SetMaximum(0.000338461688);
   Graph_histogram2->SetDirectory(nullptr);
   Graph_histogram2->SetStats(0);
   Graph_histogram2->SetLineColor(TColor::GetColor("#000099"));
   Graph_histogram2->GetXaxis()->SetLabelFont(42);
   Graph_histogram2->GetXaxis()->SetTitleOffset(1);
   Graph_histogram2->GetXaxis()->SetTitleFont(42);
   Graph_histogram2->GetYaxis()->SetLabelFont(42);
   Graph_histogram2->GetYaxis()->SetTitleFont(42);
   Graph_histogram2->GetZaxis()->SetLabelFont(42);
   Graph_histogram2->GetZaxis()->SetTitleOffset(1);
   Graph_histogram2->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_histogram2);
   
   grae->Draw("f");
   
   std::vector<Double_t> graph_x_vect8{
      500, 500, 600, 600, 700, 700, 800, 800, 900, 900,
      1000, 1000, 1100, 1100, 1200, 1200, 1300, 1300, 1400, 1400,
      1500, 1500
   };
   std::vector<Double_t> graph_y_vect9{
      0.000211142, 0.000211142, 0.000128899, 0.000128899, 7.94625e-05, 7.94625e-05, 5.2902e-05, 5.2902e-05, 3.98123e-05, 3.98123e-05,
      3.222362e-05, 3.222362e-05, 2.836447999999999e-05, 2.836447999999999e-05, 2.50785e-05, 2.50785e-05, 2.2949319e-05, 2.2949319e-05, 2.1250971e-05, 2.1250971e-05,
      1.9075e-05, 1.9075e-05
   };
   graph = new TGraph(22, graph_x_vect8.data(), graph_y_vect9.data());
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillStyle(1000);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);
   
   TH1F *Graph_histogram3 = new TH1F("Graph_histogram3", "Graph", 100, 400, 1600);
   Graph_histogram3->SetMinimum(1.71675e-05);
   Graph_histogram3->SetMaximum(0.0002303487);
   Graph_histogram3->SetDirectory(nullptr);
   Graph_histogram3->SetStats(0);
   Graph_histogram3->SetLineColor(TColor::GetColor("#000099"));
   Graph_histogram3->GetXaxis()->SetLabelFont(42);
   Graph_histogram3->GetXaxis()->SetTitleOffset(1);
   Graph_histogram3->GetXaxis()->SetTitleFont(42);
   Graph_histogram3->GetYaxis()->SetLabelFont(42);
   Graph_histogram3->GetYaxis()->SetTitleFont(42);
   Graph_histogram3->GetZaxis()->SetLabelFont(42);
   Graph_histogram3->GetZaxis()->SetTitleOffset(1);
   Graph_histogram3->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_histogram3);
   
   graph->Draw("lp");
   
   std::vector<Double_t> graph_x_vect10{
      200, 250, 300, 350, 400, 450, 500, 550, 600, 650,
      700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150,
      1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650,
      1700
   };
   std::vector<Double_t> graph_y_vect11{
      0.08160000000000001, 0.0355, 0.0173, 0.009480000000000001, 0.00531, 0.00317, 0.00193, 0.00127, 0.00083, 0.00055,
      0.000375, 0.000266, 0.00018, 0.000132, 9.35e-05, 6.81e-05, 4.94e-05, 3.66e-05, 2.74e-05, 2.02e-05,
      1.5e-05, 1.12e-05, 8.49e-06, 6.3e-06, 4.91e-06, 3.8e-06, 2.8e-06, 2.1e-06, 1.69e-06, 1.29e-06,
      1e-06
   };
   graph = new TGraph(31, graph_x_vect10.data(), graph_y_vect11.data());
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillStyle(1000);
   graph->SetLineColor(2);
   graph->SetLineWidth(3);
   
   TH1F *Graph_histogram4 = new TH1F("Graph_histogram4", "Graph", 100, 50, 1850);
   Graph_histogram4->SetMinimum(9e-07);
   Graph_histogram4->SetMaximum(0.0897599);
   Graph_histogram4->SetDirectory(nullptr);
   Graph_histogram4->SetStats(0);
   Graph_histogram4->SetLineColor(TColor::GetColor("#000099"));
   Graph_histogram4->GetXaxis()->SetLabelFont(42);
   Graph_histogram4->GetXaxis()->SetTitleOffset(1);
   Graph_histogram4->GetXaxis()->SetTitleFont(42);
   Graph_histogram4->GetYaxis()->SetLabelFont(42);
   Graph_histogram4->GetYaxis()->SetTitleFont(42);
   Graph_histogram4->GetZaxis()->SetLabelFont(42);
   Graph_histogram4->GetZaxis()->SetTitleOffset(1);
   Graph_histogram4->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_histogram4);
   
   graph->Draw("l");
   
   TPaveLabel *pavelabel = new TPaveLabel(0.16, 0.89125, 0.27, 1.0125, "CMS", "NBNDC");
   pavelabel->SetBorderSize(0);
   pavelabel->SetFillColor(0);
   pavelabel->SetFillStyle(0);
   pavelabel->SetTextAlign(12);
   pavelabel->SetTextSize(0.99);
   pavelabel->Draw("NBNDC");
   
   pavelabel = new TPaveLabel(0.25, 0.915, 0.3, 0.98, "Preliminary", "NBNDC");
   pavelabel->SetBorderSize(0);
   pavelabel->SetFillColor(0);
   pavelabel->SetFillStyle(0);
   pavelabel->SetTextAlign(12);
   pavelabel->SetTextFont(52);
   pavelabel->SetTextSize(0.6);
   pavelabel->Draw("NBNDC");
   
   TLegend *leg = new TLegend(0.5, 0.58, 0.834885, 0.878644, nullptr, "brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *legentry = leg->AddEntry("Graph","Exp. 95% CL limit, median","l");
   legentry->SetLineStyle(2);
   legentry->SetLineWidth(2);
   legentry->SetTextFont(42);
   legentry = leg->AddEntry("Graph","Exp. (68%)","F");
   legentry->SetFillColor(TColor::GetColor("#00cc00"));
   legentry->SetFillStyle(1000);
   legentry->SetTextFont(42);
   legentry = leg->AddEntry("Graph","Exp. (95%)","F");
   legentry->SetFillColor(TColor::GetColor("#ffcc00"));
   legentry->SetFillStyle(1000);
   legentry->SetTextFont(42);
   legentry = leg->AddEntry("Graph","#sigma(pp#rightarrow H^{++}_{L}H^{--}_{L})","lpf");
   legentry->SetFillStyle(1000);
   legentry->SetLineColor(2);
   legentry->SetLineWidth(3);
   legentry->SetMarkerStyle(1);
   legentry->SetTextFont(42);
   leg->Draw("hist");
   
   leg = new TLegend(0.7, 0.4, 0.9, 0.55, nullptr, "brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.05);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->Draw("hist");
   
   pavelabel = new TPaveLabel(0.65, 0.905, 0.9, 0.99, "139 fb^{-1} (13 TeV)", "NBNDC");
   pavelabel->SetBorderSize(0);
   pavelabel->SetFillColor(0);
   pavelabel->SetTextFont(42);
   pavelabel->SetTextSize(0.5);
   pavelabel->Draw("NBNDC");
   
   TH1F *DummyGraph_copy = new TH1F("DummyGraph_copy", "", 100, 500, 1500);
   DummyGraph_copy->SetMaximum(0.01);
   DummyGraph_copy->SetDirectory(nullptr);
   DummyGraph_copy->SetStats(0);
   DummyGraph_copy->SetLineColor(TColor::GetColor("#000099"));
   DummyGraph_copy->GetXaxis()->SetTitle("Mass [GeV]");
   DummyGraph_copy->GetXaxis()->SetRange(0, 101);
   DummyGraph_copy->GetXaxis()->SetLabelFont(42);
   DummyGraph_copy->GetXaxis()->SetLabelSize(0.05499999970197678);
   DummyGraph_copy->GetXaxis()->SetTitleSize(0.05499999970197678);
   DummyGraph_copy->GetXaxis()->SetTitleOffset(1.049999952316284);
   DummyGraph_copy->GetXaxis()->SetTitleFont(42);
   DummyGraph_copy->GetYaxis()->SetTitle("#sigma(pp#rightarrow H^{++}H^{--}) [pb]");
   DummyGraph_copy->GetYaxis()->SetLabelFont(42);
   DummyGraph_copy->GetYaxis()->SetLabelSize(0.05499999970197678);
   DummyGraph_copy->GetYaxis()->SetTitleSize(0.05499999970197678);
   DummyGraph_copy->GetYaxis()->SetTitleOffset(1.299999952316284);
   DummyGraph_copy->GetYaxis()->SetTitleFont(42);
   DummyGraph_copy->GetZaxis()->SetLabelFont(42);
   DummyGraph_copy->GetZaxis()->SetTitleOffset(1);
   DummyGraph_copy->GetZaxis()->SetTitleFont(42);
   DummyGraph_copy->Draw("sameaxis");
   plotPad__0->Modified();
   cCL->cd();
   cCL->Modified();
   cCL->SetSelected(cCL);
}
