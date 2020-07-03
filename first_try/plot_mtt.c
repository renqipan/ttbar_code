//plot differential cross section of ttbar with respect to y_tt

void plot_mtt(){
//gStyle->SetOptStat(0);
TCanvas *c=new TCanvas("c","",8, 30, 800, 800);
c->SetFillColor(0);
c->SetBorderMode(0);
c->SetBorderSize(2);
c->SetTickx(1);
c->SetTicky(1);
c->SetLeftMargin(0.16);
c->SetRightMargin(0.08);
c->SetTopMargin(0.07);
c->SetBottomMargin(0.13);
c->SetFrameFillStyle(0);
c->SetFrameBorderMode(0);
c->SetFrameFillStyle(0);
c->SetFrameBorderMode(0);
TMultiGraph *mg = new TMultiGraph();
TTree *t = new TTree("t", "tree from mtt_yt.csv");
t->ReadFile("mtt_yt.csv", "yt0_X/D:yt0_Y:yt1_X:yt1_Y:yt2_X:yt2_Y");
t->Draw("yt0_Y:yt0_X");
TGraph *gr0=(TGraph*) gROOT->FindObject("Graph")->Clone();
t->Draw("yt1_Y:yt1_X");
TGraph *gr1=(TGraph*) gROOT->FindObject("Graph")->Clone();
t->Draw("yt2_Y:yt2_X");
TGraph *gr2=(TGraph*) gROOT->FindObject("Graph")->Clone();
TTree *t3=new TTree("t3","tree from mtt_yt3.csv");
t3->ReadFile("mtt_yt3.csv","yt3_X/D:yt3_Y");
t3->Draw("yt3_Y:yt3_X");
TGraph *gr3=(TGraph*) gROOT->FindObject("Graph")->Clone();
gr0->SetMarkerStyle(20);
gr0->SetMarkerColor(kBlack);
gr0->SetLineColor(kBlack);
gr1->SetMarkerColor(kBlue);
gr1->SetMarkerStyle(21);
gr1->SetLineColor(kBlue);
gr2->SetMarkerStyle(22);
gr2->SetMarkerColor(kRed);
gr2->SetLineColor(kRed);
gr3->SetMarkerStyle(23);
gr3->SetMarkerColor(kGreen);
gr3->SetLineColor(kGreen);
gr0->Sort();
gr1->Sort();
gr2->Sort();
gr3->Sort();
mg->Add(gr0);
mg->Add(gr1);
mg->Add(gr2);
mg->Add(gr3);
TH1 *frame = c->DrawFrame(350,-0.08,2000,0.18); //Draw an empty pad frame 
//DrawFrame (Double_t xmin, Double_t ymin, Double_t xmax, Double_t ymax, const char *title="")
frame->GetXaxis()->CenterTitle();
frame->GetYaxis()->CenterTitle();
frame->GetXaxis()->SetTitleOffset(1.2);
//frame->GetXaxis()->SetRangeUser(-3.36,3.36);
//frame->GetYaxis()->SetRangeUser(-0.045,0.038);
frame->GetXaxis()->SetTitle("M_{t#bar{t}}^{gen}[GeV]");
frame->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/dM_{t#bar{t}})/(d#sigma_{LO}/dM_{t#bar{t}})");
mg->Draw("PC");

auto leg=new TLegend(.7,.7,.89,.9);
TLegendEntry* leg_entry0=leg->AddEntry(gr0,"Y_{t}=0");
TLegendEntry* leg_entry1=leg->AddEntry(gr1,"Y_{t}=1");
TLegendEntry* leg_entry2=leg->AddEntry(gr2,"Y_{t}=2");
TLegendEntry* leg_entry3=leg->AddEntry(gr3,"Y_{t}=3");
leg_entry0->SetTextColor(kBlack);
leg_entry1->SetTextColor(kBlue);
leg_entry2->SetTextColor(kRed);
leg_entry3->SetTextColor(kGreen);

leg->Draw("same");
c->Print("mtt_yt.png");
}
