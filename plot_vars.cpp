void plot_vars(){
	TChain *chain1= new TChain("mytree");
	chain1->Add("/home/renqi/Downloads/MG5_aMC_v2_7_2/Delphes/new_pp_ttbar_nlo.root");
	Int_t entry_num1;
	entry_num1=chain1->GetEntries();
	cout<<"number of events: "<<entry_num1<<endl;
	Int_t num_vars=7;
	TString vars[]={"top_pt","mass_tlep","mass_thad","rapidity_tt",
		"mass_tt","mass_wlep","mass_whad","rapidity_tt","mass_tt"};
	float xlow[]={0,0,0, -5,0,0,0,-5,0};
	float xup[]={500,700,700, 5,1600,350,350,5,1600};
	TString xtitle[]={"p_{T}[GeV]","M_{tlep}[GeV]","M_{thad}[GeV]","#Deltay_{t#bar{t}}","M_{t#bar{t}}[GeV]","M_{Wlep}[GeV]",
						"M_{Whad}[GeV]","#Deltay_{t#bar{t}}","M_{t#bar{t}}[GeV]"};
	TString ytitle[]={"d#sigma/#sigmadp_{T}","d#sigma/#sigmadM_{tlep}","d#sigma/#sigmadM_{thad}",
    					"d#sigma/#sigmad#Deltay_{t#bar{t}}","d#sigma/#sigmadM_{t#bar{t}}",
    					"d#sigma/#sigmadM_{Wlep}","d#sigma/#sigmadM_{Whad}","Events/0.25","Events/40GeV"};
    TString title[]={"p_T","M_tlep","M_thad","Deltay_ttbar","M_ttbar","M_Wlep",
						"M_Whad","Deltay_ttbar","M_ttbar"};
	Int_t bins=40;
	for(Int_t i=0; i<num_vars+2;i++){
		auto c2=new TCanvas();
		TH1F *h1=new TH1F("h1","",bins,xlow[i],xup[i]);
		TString weight="805856*137.1/200000";
        if(i<num_vars)
			chain1->Draw(vars[i]+">>h1");
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
		if(i<num_vars)
			c2->Print(title[i]+".png");
		else
			c2->Print(title[i]+"_weight.png");
	}
}