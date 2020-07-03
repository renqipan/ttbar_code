//plot RooHistPdf from workspace.
//firstly, set up CMS enviroment: cmsenv
void Format_Canvas(TCanvas* c){
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
void mtt_roo_plot(){

	gSystem->Load("libHiggsAnalysisCombinedLimit.so");//cmsenv needed!
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	TString dir="datacard/";
	TFile *file=TFile::Open(dir+"workspace_ttbar.root");
	RooWorkspace *w=(RooWorkspace*) file->Get("w");
	RooSimultaneous *sbpdf = (RooSimultaneous*)w->pdf("model_s");
	RooCategory *chan = w->cat("CMS_channel");
	RooRealVar *mass_tt= w->var("mass_tt");
	RooRealVar *rapidity= w->var("rapidity_tt");
	RooRealVar *kappa=w->var("kappa");
	RooRealVar *kappa_t=w->var("kappa_t");
	kappa_t->setVal(0);
	kappa_t->Print();
	for (int i=0; i<1; ++i) {
	chan->setIndex(i);
	RooAbsPdf *sbcatpdf = sbpdf->getPdf(chan->getLabel());
	
	kappa->setVal(0);
	kappa->Print();
	double events_kappa0 = sbcatpdf->expectedEvents(*mass_tt);
	RooAbsPdf *kappa0_pdf_mtt=sbcatpdf->createProjection(*rapidity);
	TH1F* mtt_kappa0=(TH1F*)kappa0_pdf_mtt->createHistogram("mass_tt");
	mtt_kappa0->Scale(events_kappa0/mtt_kappa0->Integral());
	mtt_kappa0->SetLineColor(2);
	TCanvas *canvas=new TCanvas("canvas","canvas",10,30,600,600);
	Format_Canvas(canvas);
	mtt_kappa0->GetYaxis()->SetRangeUser(0,mtt_kappa0->GetMaximum()*1.3);
	mtt_kappa0->GetYaxis()->SetTitle("Events/bin");
	mtt_kappa0->GetYaxis()->CenterTitle();
	mtt_kappa0->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mtt_kappa0->GetXaxis()->CenterTitle();
	mtt_kappa0->GetXaxis()->SetTitleSize(0.05);
	mtt_kappa0->GetYaxis()->SetTitleSize(0.055);
	mtt_kappa0->SetTitle("");
	mtt_kappa0->Draw("hist");


	kappa->setVal(1);
	kappa->Print();
	double events_kappa1=sbcatpdf->expectedEvents(*mass_tt);
	RooAbsPdf *kappa1_pdf_mtt=sbcatpdf->createProjection(*rapidity);
	TH1F* mtt_kappa1=(TH1F*)kappa1_pdf_mtt->createHistogram("mass_tt");	
	mtt_kappa1->Scale(events_kappa1/mtt_kappa1->Integral());
	mtt_kappa1->SetLineColor(1);
	mtt_kappa1->Draw("histsame");

	kappa->setVal(2);
	kappa->Print();
	double events_kappa2=sbcatpdf->expectedEvents(*mass_tt);
	RooAbsPdf *kappa2_pdf_mtt=sbcatpdf->createProjection(*rapidity);
	TH1F* mtt_kappa2=(TH1F*)kappa2_pdf_mtt->createHistogram("mass_tt");
	mtt_kappa2->Scale(events_kappa2/mtt_kappa2->Integral());
	mtt_kappa2->SetLineColor(4);
	mtt_kappa2->Draw("histsame");

	kappa->setVal(0);
	kappa_t->setVal(2);
	kappa->Print();
	kappa_t->Print();
	double events_tilde2=sbcatpdf->expectedEvents(*mass_tt);
	RooAbsPdf *tilde2_pdf_mtt=sbcatpdf->createProjection(*rapidity);
	TH1F* mtt_tilde2=(TH1F*)tilde2_pdf_mtt->createHistogram("mass_tt");
	mtt_tilde2->Scale(events_tilde2/mtt_tilde2->Integral());
	mtt_tilde2->SetLineColor(3);
	mtt_tilde2->Draw("histsame");

	kappa->setVal(2);
	kappa_t->setVal(2);
	kappa->Print();
	kappa_t->Print();
	double events_kappa2tilde2=sbcatpdf->expectedEvents(*mass_tt);
	RooAbsPdf *kappa2tilde2_pdf_mtt=sbcatpdf->createProjection(*rapidity);
	TH1F* mtt_kappa2tilde2=(TH1F*)kappa2tilde2_pdf_mtt->createHistogram("mass_tt");
	mtt_kappa2tilde2->Scale(events_kappa2tilde2/mtt_kappa2tilde2->Integral());
	mtt_kappa2tilde2->SetLineColor(6);
	mtt_kappa2tilde2->Draw("histsame");

	kappa->setVal(1);
	kappa_t->setVal(2);
	kappa->Print();
	kappa_t->Print();
	double events_kappa1tilde2=sbcatpdf->expectedEvents(*mass_tt);
	RooAbsPdf *kappa1tilde2_pdf_mtt=sbcatpdf->createProjection(*rapidity);
	TH1F* mtt_kappa1tilde2=(TH1F*)kappa1tilde2_pdf_mtt->createHistogram("mass_tt");
	mtt_kappa1tilde2->Scale(events_kappa1tilde2/mtt_kappa1tilde2->Integral());
	mtt_kappa1tilde2->SetLineColor(7);
	mtt_kappa1tilde2->Draw("histsame");

	auto leg=new TLegend(.65,.6,.89,.9);
	TLegendEntry* leg_entry0=leg->AddEntry(mtt_kappa0,"#kappa=0, #tilde{#kappa}=0");
	TLegendEntry* leg_entry1=leg->AddEntry(mtt_kappa1,"#kappa=1, #tilde{#kappa}=0");
	TLegendEntry* leg_entry2=leg->AddEntry(mtt_kappa2,"#kappa=2, #tilde{#kappa}=0");
	TLegendEntry* leg_entry3=leg->AddEntry(mtt_tilde2,"#kappa=0, #tilde{#kappa}=2");
	TLegendEntry* leg_entry4=leg->AddEntry(mtt_kappa2tilde2,"#kappa=2, #tilde{#kappa}=2");
	TLegendEntry* leg_entry5=leg->AddEntry(mtt_kappa1tilde2,"#kappa=1, #tilde{#kappa}=2");


	leg_entry0->SetTextColor(2);
	leg_entry1->SetTextColor(1);
	leg_entry2->SetTextColor(4);
	leg_entry3->SetTextColor(3);
	leg_entry4->SetTextColor(6);
	leg_entry5->SetTextColor(7);
	leg->Draw("same");

	cout<<"events_kappa0: "<<events_kappa0<<endl;
	cout<<"events_kappa1: "<<events_kappa1<<endl;
	cout<<"events_kappa2: "<<events_kappa2<<endl;
	cout<<"events_tilde2: "<<events_tilde2<<endl;
	cout<<"events_kappa2tilde2: "<<events_kappa2tilde2<<endl;
	cout<"events_kappa1tilde2: "<<events_kappa1tilde2<<endl;

	canvas->Print(dir+"pdf_mtt.png");

	}

}