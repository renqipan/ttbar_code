void plot_branch(){
	gStyle->SetOptStat(kFALSE);
	TFile* file=new TFile("./lhereader/tree_pp_ttbar.root");
	TTree *mytree;
	file->GetObject("mytree",mytree);//I have a tree named "mytree" inside the root file.
	TCanvas *c=new TCanvas("c","",10,30,800,800);
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
	TH1F *h1=new TH1F("ht1","",100,200,1600); //define a one-dimension histogram
	TH1F *h2=new TH1F("ht2","hist of mtt with cut",100,200,1600);
	TH1F *h3=new TH1F("ht3","hist of mtt with cut2",100,200,1600);

	mytree->Draw("M_tt>>ht1","abs(delta_rapidity)<0.6"); //the second argument can put cut and weight.
	//repoint the histogram drawn to a histogram named "hmtt" rather than draw directly
 	// since here we wanna plot two histograms in one canvas.
	mytree->Draw("M_tt>>ht2","abs(delta_rapidity)>0.6&&abs(delta_rapidity)<1.2");
	mytree->Draw("M_tt>>ht3","abs(delta_rapidity)>1.2");
	h1->SetLineColor(2);
	h2->SetLineColor(3);
	h3->SetLineColor(4);
	h1->SetLineWidth(2);
	h2->SetLineWidth(2);
	h3->SetLineWidth(2);
	h1->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	h1->GetYaxis()->SetTitle("d#sigma/dM_{t#bar{t}}/#sigma");
	h1->GetXaxis()->CenterTitle();
	h1->GetYaxis()->CenterTitle();
	h1->GetXaxis()->SetTitleOffset(1.1);
	h1->GetYaxis()->SetRangeUser(0,(h1->GetMaximum())*1.2);
	TFile* file_nlo=new TFile("./lhereader/tree_pp_ttbar_nlo.root");
	TTree *mytree_nlo;
	file_nlo->GetObject("mytree",mytree_nlo);//I have a tree named "mytree" inside the root file.
	TH1F *h4=new TH1F("ht4","hist of mtt",100,200,1600); //define a one-dimension histogram
	TH1F *h5=new TH1F("ht5","hist of mtt with cut",100,200,1600);
	TH1F *h6=new TH1F("ht6","hist of mtt with cut2",100,200,1600);

	mytree_nlo->Draw("M_tt>>ht4","abs(delta_rapidity)<0.6"); //the second argument can put cut and weight.
	//repoint the histogram drawn to a histogram named "hmtt" rather than draw directly
 	// since here we wanna plot two histograms in one canvas.
	mytree_nlo->Draw("M_tt>>ht5","abs(delta_rapidity)>0.6&&abs(delta_rapidity)<1.2");
	mytree_nlo->Draw("M_tt>>ht6","abs(delta_rapidity)>1.2");
	h4->SetLineColor(6);
	h5->SetLineColor(7);
	h6->SetLineColor(9);
	h4->SetLineWidth(2);
	h5->SetLineWidth(2);
	h6->SetLineWidth(2);
	h1->DrawNormalized("hist");
	h2->DrawNormalized("histsame");//use parameter"same" to make them appear in same diagram.
	h3->DrawNormalized("histsame");
	h4->DrawNormalized("histsame");
	h5->DrawNormalized("histsame");
	h6->DrawNormalized("histsame");
	auto leg=new TLegend(.55,.60,.89,.9);
	TLegendEntry* leg_entry1=leg->AddEntry(h1,"LO, |#Deltay_{t#bar{t}}|<0.6");
	TLegendEntry* leg_entry2=leg->AddEntry(h2,"LO, 0.6<|#Deltay_{t#bar{t}}|<1.2");
	TLegendEntry* leg_entry3=leg->AddEntry(h3,"LO, |#Deltay_{t#bar{t}}|>1.2");

	TLegendEntry* leg_entry4=leg->AddEntry(h4,"NLO, |#Deltay_{t#bar{t}}|<0.6");
	TLegendEntry* leg_entry5=leg->AddEntry(h5,"NLO, 0.6<|#Deltay_{t#bar{t}}|<1.2");
	TLegendEntry* leg_entry6=leg->AddEntry(h6,"NLO, |#Deltay_{t#bar{t}}|>1.2");	
	leg_entry1->SetTextColor(2);
	leg_entry2->SetTextColor(3);
	leg_entry3->SetTextColor(4);
	leg_entry4->SetTextColor(6);
	leg_entry5->SetTextColor(7);
	leg_entry6->SetTextColor(9);
	leg->Draw();
	gPad->SetGrid();
	gPad->Print("dist_cuts.png");

}
