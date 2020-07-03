void drawstack(){
	gStyle->SetOptStat(0);

	TChain *ttbar=new TChain("mytree");
	TChain *top=new TChain("mytree");
	ttbar->Add("/home/renqi/Downloads/MG5_aMC_v2_7_2/Delphes/new_pp_ttbar_nlo.root");
	top->Add("/home/renqi/Downloads/MG5_aMC_v2_7_2/Delphes/new_pp_top.root");
	Int_t nbin=10;
	Int_t num_vars=3;
	TString vars[]={"top_pt","mass_tt","rapidity_tt"};
	TString xtitle[]={"p_{T}[GeV]","M_{t#bar{t}}[GeV]","#Deltay_{t#bar{t}}"};
	TString output[]={"pt","mass_tt","deltay_tt"};
	Float_t low[]={0,0,-4};
	Float_t high[]={500,2000,4};
	TString weight_ttbar="832000*137.1/200000*weight_kappa1";// normalize ttbar to QCD NNLO
	Int_t num_top=top->GetEntries();
	TString weight_top="259000*137.1/200000*1.71*0.105";//normalize single top to all background
	//TString weight_top="45365/"+TString(num_top);
	for(Int_t i=0;i<num_vars;i++){
		TCanvas *c=new TCanvas("c","",10, 30, 600, 600);
		c->SetFillColor(0);
		c->SetBorderMode(0);
		c->SetBorderSize(2);
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.17);
		c->SetRightMargin(0.05);
		c->SetTopMargin(0.07);
		c->SetBottomMargin(0.13);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
		TH1F *httbar, *htop;
		if(vars[i].Contains("mass")){
			//Float_t edges[12]={0,360,400,440,480,540,620,750,880,1100,1500,2000};
			Double_t edges[8]={0,410,540,710,850,990,1200,2000};
			httbar=new TH1F("httbar","",7,edges);
			htop=new TH1F("htop","",7,edges);
			}
		else if(vars[i].Contains("rapidity_tt")){
			Double_t rap_edges[6]={-4,-1.2,-0.45,0.45,1.2,4};			
			httbar=new TH1F("httbar","",5,rap_edges);
			htop=new TH1F("htop","",5,rap_edges);

		}	
		else{
			httbar=new TH1F("httbar","",nbin,low[i],high[i]);
			htop=new TH1F("htop","",nbin,low[i],high[i]);
			}
		ttbar->Draw(vars[i]+">>httbar",weight_ttbar);
		top->Draw(vars[i]+">>htop",weight_top);
		httbar->SetFillColor(4);
		htop->SetFillColor(2);

		THStack *hstack=new THStack("hstack","");
		hstack->Add(htop);
		hstack->Add(httbar);
		hstack->Draw("hist");

		hstack->GetXaxis()->SetNdivisions(505);;
		hstack->GetXaxis()->SetLabelFont(42);
		hstack->GetXaxis()->SetLabelOffset(0.007);
		hstack->GetXaxis()->SetLabelSize(0.04);
		hstack->GetXaxis()->SetTitleSize(0.05);
		hstack->GetXaxis()->SetTitleOffset(0.9);
		hstack->GetXaxis()->SetTitleFont(42);

		hstack->GetYaxis()->SetNdivisions(505);
		hstack->GetYaxis()->SetLabelFont(42);
		hstack->GetYaxis()->SetLabelOffset(0.007);
		hstack->GetYaxis()->SetLabelSize(0.04);
		hstack->GetYaxis()->SetTitleSize(0.05);
		hstack->GetYaxis()->SetTitleOffset(1.1);
		hstack->GetYaxis()->SetTitleFont(42);

		hstack->GetXaxis()->CenterTitle();
		hstack->GetYaxis()->CenterTitle();
		hstack->GetXaxis()->SetTitle(xtitle[i]);
		hstack->GetYaxis()->SetTitle("Events / bin");
		hstack->SetMaximum(hstack->GetMaximum()*1.3);
		
		TLegend *leg = new TLegend(0.68, .8, .88, .9);
	    //leg->SetNColumns(2);
		//leg->SetColumnSeparation (0);
		leg->SetBorderSize(0);
		leg->SetTextFont(42);
		leg->SetTextSize(0.04);
		leg->SetLineColor(1);
		leg->SetLineStyle(1);
		leg->SetLineWidth(1);
		leg->SetFillColor(0);
		leg->SetFillStyle(0);
		leg->AddEntry(httbar,"t#bar{t}","f");
		leg->AddEntry(htop," Background","f");
		
		leg->Draw();

		gPad->Print(output[i]+"_stack.png");
		cout<<"number of ttbar events: "<<httbar->GetSumOfWeights()<<endl;
		cout<<"number of background events: "<<htop->GetSumOfWeights()<<endl;
		
		
	}
	
	


}
