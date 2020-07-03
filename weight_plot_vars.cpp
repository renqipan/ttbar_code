void format_canvas(TCanvas* c){
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
	}

void weight_plot_vars(){
	gStyle->SetOptStat(kFALSE);
//	gStyle->SetOptTitle(0);
	TChain *chain1= new TChain("mytree");
	chain1->Add("/home/renqi/Downloads/MG5_aMC_v2_7_2/Delphes/new_pp_ttbar_nlo.root");
	Int_t entry_num1;
	entry_num1=chain1->GetEntries();
	cout<<"number of events: "<<entry_num1<<endl;
	Int_t num_vars=2;
	TString vars[]={"rapidity_tt",
		"mass_tt","rapidity_tt","mass_tt"};
	float xlow[]={-5,0,-5,0};
	float xup[]={5,1600,5,1600};
	TString xtitle[]={"#Deltay_{t#bar{t}}","M_{t#bar{t}}[GeV]","#Deltay_{t#bar{t}}","M_{t#bar{t}}[GeV]"};
	TString ytitle[]={"d#sigma/#sigmad#Deltay_{t#bar{t}}","d#sigma/#sigmadM_{t#bar{t}}",
    					"Events/0.25","Events/40GeV"};
    TString title[]={"Deltay_ttbar_ka","M_ttbar_ka","Deltay_ttbar_kaweight","M_ttbar_kaweight"};
	Int_t bins=20;
	for(Int_t i=0; i<num_vars+2;i++){
		auto c2=new TCanvas();
		TH1F *h1=new TH1F("h1","",bins,xlow[i],xup[i]);
		TString weight="832000*137.1/100000*weight_kappa2";
        if(i<num_vars)
			chain1->Draw(vars[i]+">>h1","weight_kappa2");
		else 
			chain1->Draw(vars[i]+">>h1",weight);
		h1->SetLineColor(kRed);
		h1->SetStats(kFALSE);
		h1->GetXaxis()->SetTitle(xtitle[i]);
		h1->GetYaxis()->SetTitle(ytitle[i]);
		h1->GetXaxis()->CenterTitle();
		h1->GetYaxis()->CenterTitle();
		h1->GetXaxis()->SetTitleSize(0.05);
		h1->GetYaxis()->SetTitleSize(0.05);
		h1->GetYaxis()->SetRangeUser(0,h1->GetMaximum()*1.2);
		if(i<num_vars)
			h1->DrawNormalized("hist");
		else
			h1->Draw("hist");

		c2->SetFillColor(0);
		c2->SetBorderMode(0);
		c2->SetBorderSize(2);
		c2->SetTickx(1);
		c2->SetTicky(1);
		c2->SetLeftMargin(0.15);
		c2->SetRightMargin(0.05);
		c2->SetTopMargin(0.07);
		c2->SetBottomMargin(0.13);
		c2->SetFrameFillStyle(0);
		c2->SetFrameBorderMode(0);
		c2->SetFrameFillStyle(0);
		c2->SetFrameBorderMode(0);
		gPad->SetGrid();
		c2->Print(title[i]+".png");
		
	}
	/////////////////////////////////////////////
	//calculate (EW-LO)/LO at generator level
	TH1F* hmtt_gen=new TH1F("hmtt_gen","hmtt_gen",bins,350,1200);
	TH1F* hmtt_gen_ka=new TH1F("hmtt_gen_ka","hmtt_gen_ka",bins,350,1200);
	TH1F* hytt_gen=new TH1F("hytt_gen","hytt_gen",bins,-3,3);
	TH1F* hytt_gen_ka=new TH1F("hytt_gen_ka","hytt_gen_ka",bins,-3,3);
	TCanvas *ctem=new TCanvas();
	chain1->Draw("M_tt_gen>>hmtt_gen");
	chain1->Draw("M_tt_gen>>hmtt_gen_ka","weight_kappa2");
	chain1->Draw("delta_rapidity_gen>>hytt_gen");
	chain1->Draw("delta_rapidity_gen>>hytt_gen_ka","weight_kappa2");

	hytt_gen_ka->Add(hytt_gen,-1.0);
	hytt_gen_ka->Divide(hytt_gen);
	
	TCanvas* c3=new TCanvas("c3","",10,30,600,600);
	hytt_gen_ka->GetXaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hytt_gen_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/d#Deltay_{t#bar{t}})/(d#sigma_{LO}/d#Deltay_{t#bar{t}})");
	format_canvas(c3);
	hytt_gen_ka->GetXaxis()->CenterTitle();
	hytt_gen_ka->GetYaxis()->CenterTitle();
	hytt_gen_ka->SetTitle("");
	hytt_gen_ka->GetYaxis()->SetRangeUser(1.2*hytt_gen_ka->GetMinimum(),(hytt_gen_ka->GetMaximum())*1.2);
	hytt_gen_ka->Draw("hist");
	//c3->SetGrid();
	c3->Print("ytt_gen_ratio.png");

	TCanvas *c4=new TCanvas("c4","",10,30,600,600);
	hmtt_gen_ka->Add(hmtt_gen,-1.0);
	hmtt_gen_ka->Divide(hmtt_gen);
	hmtt_gen_ka->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hmtt_gen_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/dM_{t#bar{t}})/(d#sigma_{LO}/dM_{t#bar{t}})");
	format_canvas(c4);
	hmtt_gen_ka->GetXaxis()->CenterTitle();
	hmtt_gen_ka->GetYaxis()->CenterTitle();
	hmtt_gen_ka->SetTitle("");
	hmtt_gen_ka->GetYaxis()->SetRangeUser(1.2*hmtt_gen_ka->GetMinimum(),hmtt_gen_ka->GetMaximum()*1.2);
	hmtt_gen_ka->Draw("hist");
	c4->Print("mtt_gen_ratio.png");
	/////////////////////////////////////////////
	//calculate (EW-LO)/LO at detector level
	TH1F* hmtt_dec=new TH1F("hmtt_dec","hmtt_dec",bins,350,1200);
	TH1F* hmtt_dec_ka=new TH1F("hmtt_dec_ka","hmtt_dec_ka",bins,350,1200);
	TH1F* hytt_dec=new TH1F("hytt_dec","hytt_dec",bins,-3,3);
	TH1F* hytt_dec_ka=new TH1F("hytt_dec_ka","hytt_dec_ka",bins,-3,3);
	TCanvas *ctempl=new TCanvas();
	chain1->Draw("mass_tt>>hmtt_dec");
	chain1->Draw("mass_tt>>hmtt_dec_ka","weight_kappa2");
	chain1->Draw("rapidity_tt>>hytt_dec");
	chain1->Draw("rapidity_tt>>hytt_dec_ka","weight_kappa2");

	hytt_dec_ka->Add(hytt_dec,-1.0);
	hytt_dec_ka->Divide(hytt_dec);
	
	TCanvas* c6=new TCanvas("c6","",10,30,600,600);
	hytt_dec_ka->GetXaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hytt_dec_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/d#Deltay_{t#bar{t}})/(d#sigma_{LO}/d#Deltay_{t#bar{t}})");
	format_canvas(c6);
	hytt_dec_ka->GetXaxis()->CenterTitle();
	hytt_dec_ka->GetYaxis()->CenterTitle();
	hytt_dec_ka->SetTitle("");
	hytt_dec_ka->GetYaxis()->SetRangeUser(1.2*hytt_dec_ka->GetMinimum(),(hytt_dec_ka->GetMaximum())*1.2);
	hytt_dec_ka->Draw("hist");
	//c3->SetGrid();
	c6->Print("ytt_dec_ratio.png");

	TCanvas *c7=new TCanvas("c7","",10,30,600,600);
	hmtt_dec_ka->Add(hmtt_dec,-1.0);
	hmtt_dec_ka->Divide(hmtt_dec);
	hmtt_dec_ka->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hmtt_dec_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/dM_{t#bar{t}})/(d#sigma_{LO}/dM_{t#bar{t}})");
	format_canvas(c7);
	hmtt_dec_ka->GetXaxis()->CenterTitle();
	hmtt_dec_ka->GetYaxis()->CenterTitle();
	hmtt_dec_ka->SetTitle("");
	hmtt_dec_ka->GetYaxis()->SetRangeUser(1.2*hmtt_dec_ka->GetMinimum(),hmtt_dec_ka->GetMaximum()*1.2);
	hmtt_dec_ka->Draw("hist");
	c7->Print("mtt_dec_ratio.png");
////////////////////////////////////////////////
	//events distribution in 2D
	TCanvas *c5=new TCanvas("c5","",10,30,600,600);
	//format_canvas(c5);
	TH2F *hist=new TH2F("hist","hist",40,0,1600,40,-5,5);
	chain1->Draw("delta_rapidity_gen:M_tt_gen>>hist");
	hist->Draw("prof colz");
	hist->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hist->GetYaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hist->GetXaxis()->CenterTitle();
	hist->GetYaxis()->CenterTitle();
	hist->SetTitle("");
	c5->Print("distri2D.png");

//calculate (EW-LO)/LO at generator level before selection
    TChain *chain2= new TChain("newtree");
	chain2->Add("/home/renqi/Documents/ttbar_cp/lhereader/tree_pp_ttbar_decay.root");
	cout<<"number of entries: "<<chain2->GetEntries()<<" at generator level"<<endl;
	TH1F* hmtt_mc=new TH1F("hmtt_mc","hmtt_mc",bins,350,1200);
	TH1F* hmtt_mc_ka=new TH1F("hmtt_mc_ka","hmtt_mc_ka",bins,350,1200);
	TH1F* hytt_mc=new TH1F("hytt_mc","hytt_mc",bins,-3,3);
	TH1F* hytt_mc_ka=new TH1F("hytt_mc_ka","hytt_mc_ka",bins,-3,3);
	TCanvas *cmc=new TCanvas();
	chain2->Draw("M_tt_gen>>hmtt_mc");
	chain2->Draw("M_tt_gen>>hmtt_mc_ka","weight_kappa2");
	chain2->Draw("delta_rapidity_gen>>hytt_mc");
	chain2->Draw("delta_rapidity_gen>>hytt_mc_ka","weight_kappa2");

	hytt_mc_ka->Add(hytt_mc,-1.0);
	hytt_mc_ka->Divide(hytt_mc);
	
	TCanvas* c8=new TCanvas("c8","",10,30,600,600);
	hytt_mc_ka->GetXaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hytt_mc_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/d#Deltay_{t#bar{t}})/(d#sigma_{LO}/d#Deltay_{t#bar{t}})");
	format_canvas(c8);
	hytt_mc_ka->GetXaxis()->CenterTitle();
	hytt_mc_ka->GetYaxis()->CenterTitle();
	hytt_mc_ka->SetTitle("");
	hytt_mc_ka->GetYaxis()->SetRangeUser(1.2*hytt_mc_ka->GetMinimum(),(hytt_mc_ka->GetMaximum())*1.2);
	hytt_mc_ka->Draw("hist");
	//c3->SetGrid();
	c8->Print("ytt_mc_ratio.png");

	TCanvas *c9=new TCanvas("c9","",10,30,600,600);
	hmtt_mc_ka->Add(hmtt_mc,-1.0);
	hmtt_mc_ka->Divide(hmtt_mc);
	hmtt_mc_ka->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hmtt_mc_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/dM_{t#bar{t}})/(d#sigma_{LO}/dM_{t#bar{t}})");
	format_canvas(c9);
	hmtt_mc_ka->GetXaxis()->CenterTitle();
	hmtt_mc_ka->GetYaxis()->CenterTitle();
	hmtt_mc_ka->SetTitle("");
	hmtt_mc_ka->GetYaxis()->SetRangeUser(1.2*hmtt_mc_ka->GetMinimum(),hmtt_mc_ka->GetMaximum()*1.2);
	hmtt_mc_ka->Draw("hist");
	c9->Print("mtt_mc_ratio.png");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//compare weight_kappa with predict 
//	TChain *chain3= new TChain("newtree");
//	chain3->Add("/home/renqi/Documents/ttbar_cp/lhereader/tree_pp_ttbar.root");
//	cout<<"number of entries: "<<chain3->GetEntries()<<" at generator level at LO"<<endl;
	TH1F* hmtt_kappa3=new TH1F("hmtt_kappa3","hmtt_kappa3",bins,350,1200);
	TH1F* hmtt_pred=new TH1F("hmtt_pred","hmtt_pred",bins,350,1200);
	TH1F* hytt_kappa3=new TH1F("hytt_kappa3","hytt_kappa3",bins,-3,3);
	TH1F* hytt_pred=new TH1F("hytt_pred","hytt_pred",bins,-3,3);
	TH1F* hmtt_no=new TH1F("hmtt_no","hmtt_no",bins,350,1200);
	TH1F* hytt_no=new TH1F("hytt_no","hytt_no",bins,-3,3);
	
	TString weight_pred="weight_kappa1-0.75*weight_kappa0-0.25*weight_kappa2+weight_tilde1-0.25*weight_tilde2+0.25*weight_kappa2tilde2";
	//TString weight_pred="-3*weight_kappa1+1+3*weight_kappa2";
	//TString weight_pred="-4*weight_kappa1+3+weight_kappa2-4*weight_tilde1+weight_tilde2+4*weight_kappa1tilde1";
	chain2->Draw("M_tt_gen>>hmtt_kappa3","weight_kappa1tilde1");
	chain2->Draw("M_tt_gen>>hmtt_pred",weight_pred);
	chain2->Draw("delta_rapidity_gen>>hytt_kappa3","weight_kappa1tilde1");
	chain2->Draw("delta_rapidity_gen>>hytt_pred",weight_pred);
	chain2->Draw("M_tt_gen>>hmtt_no");
	chain2->Draw("delta_rapidity_gen>>hytt_no");

    hmtt_pred->Add(hmtt_no,-1.0);
	hmtt_pred->Divide(hmtt_no);
	hmtt_kappa3->Add(hmtt_no,-1.0);
	hmtt_kappa3->Divide(hmtt_no);

	hytt_pred->Add(hytt_no,-1.0);
	hytt_pred->Divide(hytt_no);
	hytt_kappa3->Add(hytt_no,-1.0);
	hytt_kappa3->Divide(hytt_no);
	
	TCanvas *c10=new TCanvas("c10","",10,30,600,600);
	format_canvas(c10);

	hmtt_kappa3->Draw("hist");
	hmtt_pred->Draw("histsame");
	hmtt_kappa3->SetTitle("");
	hmtt_kappa3->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hmtt_kappa3->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/dM_{t#bar{t}})/(d#sigma_{LO}/dM_{t#bar{t}})");
	hmtt_kappa3->GetXaxis()->CenterTitle();
	hmtt_kappa3->GetYaxis()->CenterTitle();
	hmtt_kappa3->GetYaxis()->SetRangeUser(-0.05,0.0); 
	//hmtt_kappa3->GetYaxis()->SetRangeUser(1.5*hmtt_kappa3->GetMinimum(),1.5*hmtt_kappa3->GetMaximum());
	hmtt_kappa3->SetLineColor(2);
	hmtt_pred->SetLineColor(4);
	c10->Print("hmtt_kappa1tilde1.png");
	
	TCanvas *c11=new TCanvas("c11","",10,30,600,600);
  	format_canvas(c11);
	hytt_kappa3->Draw("hist");
	hytt_pred->Draw("histsame");
	hytt_kappa3->SetTitle("");	
    hytt_kappa3->GetXaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hytt_kappa3->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/d#Deltay_{t#bar{t}})/(d#sigma_{LO}/d#Deltay_{t#bar{t}})");
	hytt_kappa3->GetXaxis()->CenterTitle();
	hytt_kappa3->GetYaxis()->CenterTitle();
	hytt_kappa3->GetYaxis()->SetRangeUser(-0.04,-0.01);
	//hytt_kappa3->GetYaxis()->SetRangeUser(1.5*hytt_kappa3->GetMinimum(),1.5*hytt_kappa3->GetMaximum());
	hytt_kappa3->SetLineColor(2);
	hytt_pred->SetLineColor(4); 
	c11->Print("hytt_kappa1tilde1.png");  

//calculate (Ew- lo)/lo under weight kappa2tilde2 after selection

	TH1F* hmtt_kappa_ka=new TH1F("hmtt_kappa_ka","hmtt_kappa_ka",bins,350,1200);
	TH1F* hytt_kappa_ka=new TH1F("hytt_kappa_ka","hytt_kappa_ka",bins,-3,3);
	TCanvas *ckappa=new TCanvas();
	chain1->Draw("M_tt_gen>>hmtt_kappa_ka","weight_kappa2tilde2");
	chain1->Draw("delta_rapidity_gen>>hytt_kappa_ka","weight_kappa2tilde2");
	TCanvas* c12=new TCanvas("c12","",15,30,600,600);
	hytt_kappa_ka->Add(hytt_gen,-1.0);
	hytt_kappa_ka->Divide(hytt_gen);
	hytt_kappa_ka->GetXaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hytt_kappa_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/d#Deltay_{t#bar{t}})/(d#sigma_{LO}/d#Deltay_{t#bar{t}})");
	format_canvas(c12);
	hytt_kappa_ka->GetXaxis()->CenterTitle();
	hytt_kappa_ka->GetYaxis()->CenterTitle();
	hytt_kappa_ka->SetTitle("");
	//hytt_kappa_ka->GetYaxis()->SetRangeUser(1.2*hytt_kappa_ka->GetMinimum(),0);
	hytt_kappa_ka->Draw("hist");
	//c3->SetGrid();
	c12->Print("ytt_kappa2tilde2_ratio.png");

	TCanvas *c13=new TCanvas("c13","",10,30,600,600);
	hmtt_kappa_ka->Add(hmtt_gen,-1.0);
	hmtt_kappa_ka->Divide(hmtt_gen);
	hmtt_kappa_ka->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hmtt_kappa_ka->GetYaxis()->SetTitle("(d#delta#sigma_{weak}/dM_{t#bar{t}})/(d#sigma_{LO}/dM_{t#bar{t}})");
	format_canvas(c13);
	hmtt_kappa_ka->GetXaxis()->CenterTitle();
	hmtt_kappa_ka->GetYaxis()->CenterTitle();
	hmtt_kappa_ka->SetTitle("");
	//hmtt_kappa_ka->GetYaxis()->SetRangeUser(1.2*hmtt_kappa_ka->GetMinimum(),0);
	hmtt_kappa_ka->Draw("hist");
	c13->Print("mtt_kappa2tilde2_ratio.png");


}