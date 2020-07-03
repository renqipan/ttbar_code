void format_canvas(TCanvas* c){
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
   TPaletteAxis *palette = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
   palette->SetX1NDC(0.85);
   palette->SetX2NDC(0.90);
   palette->SetY1NDC(0.13);
   palette->SetY2NDC(0.93);

}	
void plot_distri2d(){
	gStyle->SetOptStat(kFALSE);

	TString dir="/home/renqi/Documents/ttbar_cp/correction_roots/";
	TString input="correction_tilde2";
	TFile* file=new TFile(dir+input+".root");
	TH2F *hist=new TH2F("hist","hist",40,0,1600,40,-5,5);
	hist=(TH2F*)file->Get("h2");
	//hist->Smooth();
	TCanvas *c1=new TCanvas("c5","",10,30,600,600);
	format_canvas(c1);

	hist->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	hist->GetYaxis()->SetTitle("#Deltay_{t#bar{t}}");
	hist->GetXaxis()->CenterTitle();
	hist->GetYaxis()->CenterTitle();
	hist->GetYaxis()->SetRangeUser(-5,5);
	hist->GetXaxis()->SetTitleOffset(1.2);
	hist->SetTitle("");
	hist->Draw("colz");
	Format_palette(hist);


	c1->Print("dist2d.png");
}