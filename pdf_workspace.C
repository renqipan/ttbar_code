void Format_Canvas(TCanvas* c){
		c->SetFillColor(0);
		c->SetBorderMode(0);
		c->SetBorderSize(2);
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.12);
		c->SetRightMargin(0.16);
		c->SetTopMargin(0.07);
		c->SetBottomMargin(0.13);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);

	}

void Format_palette(TH2F *hist){
	hist->GetZaxis()->SetTitle("");
	hist->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hist->GetYaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hist->GetXaxis()->CenterTitle();
	hist->GetYaxis()->CenterTitle();
   TPaletteAxis *palette = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
   palette->SetX1NDC(0.85);
   palette->SetX2NDC(0.90);
   palette->SetY1NDC(0.13);
   palette->SetY2NDC(0.93);

}	

void pdf_workspace(){
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
		TH2F* pdf_kappa0=(TH2F*) sbcatpdf->createHistogram("mass_tt,rapidity_tt");
		pdf_kappa0->Scale(events_kappa0/pdf_kappa0->Integral());
		TCanvas *canvas=new TCanvas("canvas","canvas",10,30,600,600);
		Format_Canvas(canvas);
		pdf_kappa0->Draw("colz");
		canvas->Modified();
   		canvas->Update();
		Format_palette(pdf_kappa0);
		canvas->Print(dir+"pdf_kappa0.png");

		kappa->setVal(1);
		kappa->Print();
		double events_kappa1 = sbcatpdf->expectedEvents(*mass_tt);
		TH2F* pdf_kappa1=(TH2F*) sbcatpdf->createHistogram("mass_tt,rapidity_tt");
		pdf_kappa1->Scale(events_kappa1/pdf_kappa1->Integral());
		TCanvas *canvas2=new TCanvas("canvas2","canvas2",10,30,600,600);
		Format_Canvas(canvas2);
		pdf_kappa1->Draw("colz");
		canvas2->Modified();
   		canvas2->Update();
		Format_palette(pdf_kappa1);
		canvas2->Print(dir+"pdf_kappa1.png");

		kappa->setVal(2);
		kappa->Print();
		double events_kappa2 = sbcatpdf->expectedEvents(*mass_tt);
		TH2F* pdf_kappa2=(TH2F*) sbcatpdf->createHistogram("mass_tt,rapidity_tt");
		pdf_kappa2->Scale(events_kappa2/pdf_kappa2->Integral());
		TCanvas *canvas3=new TCanvas("canvas3","canvas3",10,30,600,600);
		Format_Canvas(canvas3);
		pdf_kappa2->Draw("colz");
		canvas3->Modified();
   		canvas3->Update();
		Format_palette(pdf_kappa2);
		canvas->Print(dir+"pdf_kappa2.png");


		TCanvas *canvas4=new TCanvas("canvas4","canvas4",10,30,600,600);
		Format_Canvas(canvas4);
		pdf_kappa0->Add(pdf_kappa1,-1);
		pdf_kappa0->Divide(pdf_kappa1);
		pdf_kappa0->Draw("colz");
		canvas4->Modified();
   		canvas4->Update();
		Format_palette(pdf_kappa0);
		canvas4->Print(dir+"pdf_ratio01.png");

		TCanvas *canvas5=new TCanvas("canvas5","canvas5",10,30,600,600);
		Format_Canvas(canvas5);
		pdf_kappa2->Add(pdf_kappa1,-1);
		pdf_kappa2->Divide(pdf_kappa1);
		pdf_kappa2->Draw("colz");
		canvas5->Modified();
   		canvas5->Update();
		Format_palette(pdf_kappa2);
		canvas5->Print(dir+"pdf_ratio21.png");

		cout<<"events_kappa0: "<<events_kappa0<<endl;
		cout<<"events_kappa1: "<<events_kappa1<<endl;
		cout<<"events_kappa2: "<<events_kappa2<<endl;


	}

}