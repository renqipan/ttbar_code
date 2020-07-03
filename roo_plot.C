using namespace std;
using namespace RooFit;
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
	hist->GetXaxis()->SetTitleOffset(1.2);
	hist->GetYaxis()->SetTitleOffset(1.2);
   TPaletteAxis *palette = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
   palette->SetX1NDC(0.85);
   palette->SetX2NDC(0.90);
   palette->SetY1NDC(0.13);
   palette->SetY2NDC(0.93);

}	
void roo_plot(){
	//gSystem->Load("libHiggsAnalysisCombinedLimit.so");
	//plot RooAbsPdf
	TString dir="datacard/";
	TFile *file=TFile::Open(dir+"ttbar.root");
	RooWorkspace *w=(RooWorkspace*) file->Get("w");
	RooAbsPdf *aterm=w->pdf("ttbar_Aterm");
	RooAbsPdf *bterm=w->pdf("ttbar_Bterm");
	RooAbsPdf *cterm=w->pdf("ttbar_Cterm");
	RooAbsPdf *dterm=w->pdf("ttbar_Dterm");
	RooAbsPdf *eterm=w->pdf("ttbar_Eterm");
	RooAbsPdf *fterm=w->pdf("ttbar_Fterm");

	RooPlot *plotm=w->var("mass_tt")->frame();
	aterm->plotOn(plotm, RooFit::LineColor(kBlack),RooFit::LineWidth(1));
	bterm->plotOn(plotm, RooFit::LineColor(kBlue),RooFit::LineWidth(1));
	cterm->plotOn(plotm, RooFit::LineColor(kRed),RooFit::LineWidth(1));
	dterm->plotOn(plotm, RooFit::LineColor(kViolet),RooFit::LineWidth(1));
	eterm->plotOn(plotm, RooFit::LineColor(kGreen),RooFit::LineWidth(1));
	fterm->plotOn(plotm, RooFit::LineColor(kMagenta),RooFit::LineWidth(1));

	plotm->Draw();
	gPad->Print(dir+"pdf_mtt_roo.png");

	RooPlot *ploty=w->var("rapidity_tt")->frame();
	aterm->plotOn(ploty, RooFit::LineColor(kBlack),RooFit::LineWidth(1));
	bterm->plotOn(ploty, RooFit::LineColor(kBlue),RooFit::LineWidth(1));
	cterm->plotOn(ploty, RooFit::LineColor(kRed),RooFit::LineWidth(1));
	dterm->plotOn(ploty, RooFit::LineColor(kViolet),RooFit::LineWidth(1));
	eterm->plotOn(ploty, RooFit::LineColor(kGreen),RooFit::LineWidth(1));
	fterm->plotOn(ploty, RooFit::LineColor(kMagenta),RooFit::LineWidth(1));
	ploty->Draw();
	gPad->Print(dir+"pdf_ytt_roo.png");

	//////////////////////////////////////////////////////////////////////
	//plot histogram from RooAbsPdf and projection into 1D
	RooRealVar *mass_tt=w->var("mass_tt");
	RooRealVar *rapidity_tt=w->var("rapidity_tt");
	//rapidity_tt->setBins(7);
	TCanvas *canvas=new TCanvas();
	canvas->cd();

	TH2F *hist_a =(TH2F*) aterm->createHistogram("mass_tt,rapidity_tt");
	TH1F  *hist_a1d=(TH1F*) hist_a->ProjectionY();
	hist_a1d->Scale(90000/hist_a1d->Integral());
	hist_a1d->Draw("hist");

	//the second method is choosed
	Double_t edges[8]={0,360,440,520,620,720,950,2000};
	Double_t rap_edges[8]={-4,-1.8,-1,-0.2,0.6,1.2,2,4};
	RooBinning *bins=new RooBinning(7,rap_edges);
	RooAbsPdf *atem_ytt_pdf=aterm->createProjection(*mass_tt);
	TH1F* aterm_ytt_hist=(TH1F*) atem_ytt_pdf->createHistogram("rapidity_tt");
	aterm_ytt_hist->Scale(900000/aterm_ytt_hist->Integral());
	cout<<"aterm_ytt_hist->Integral(): "<<aterm_ytt_hist->Integral()<<endl;
	aterm_ytt_hist->GetYaxis()->SetRangeUser(0,aterm_ytt_hist->GetMaximum()*1.3);
	hist_a1d->SetLineColor(2);
	aterm_ytt_hist->SetLineColor(4);
	aterm_ytt_hist->Draw("hist");

		
		//double events_kappa0 = atem_ytt_pdf->expectedEvents(*mass_tt);
		TH2F* pdf_kappa0=(TH2F*) aterm->createHistogram("mass_tt,rapidity_tt");
		pdf_kappa0->Scale(90000/pdf_kappa0->Integral());
		TCanvas *canvas5=new TCanvas("canvas5","canvas5",10,30,600,600);
		Format_Canvas(canvas5);
		pdf_kappa0->Draw("colz");
		canvas5->Modified();
   		canvas5->Update();
		Format_palette(pdf_kappa0);
		canvas5->Print(dir+"pdf_aterm.png");


}