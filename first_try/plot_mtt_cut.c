//graph times hist
TGraph* create_graph(TGraph* graph, TH1F* hist){
	Int_t Nbins=graph->GetN();
	Double_t *x=graph->GetX();
	Double_t *y=graph->GetY();
	Double_t yy[Nbins];
	Double_t scale=1.0/hist->Integral();
	hist->Scale(scale);
	TGraph* graph_new;
	for(Int_t i=0;i<Nbins;i++){
		yy[i]=(y[i]+1)*(hist->GetBinContent(i+1));
	//	cout<<y[i]<<" "<<yy[i]<<endl;
		}
		
	graph_new=new TGraph(Nbins,x,yy);
	return graph_new;

}
void format_sytle(TCanvas* canvas, TGraph* graph){
	canvas->SetFillColor(0);
	canvas->SetBorderMode(0);
	canvas->SetBorderSize(2);
	canvas->SetTickx(1);
	canvas->SetTicky(1);
	canvas->SetLeftMargin(0.16);
	canvas->SetRightMargin(0.08);
	canvas->SetTopMargin(0.07);
	canvas->SetBottomMargin(0.13);
	canvas->SetFrameFillStyle(0);
	canvas->SetFrameBorderMode(0);
	canvas->SetFrameFillStyle(0);
	canvas->SetFrameBorderMode(0);
	canvas->SetGrid();
	graph->GetXaxis()->SetRangeUser(350,2000);
	graph->GetXaxis()->SetTitle("M_{t#bar{t}}");
	graph->GetYaxis()->SetTitle("d#sigma/dM_{t#bar{t}}/#sigma");
	graph->GetXaxis()->CenterTitle();
	graph->GetYaxis()->CenterTitle();
	graph->GetXaxis()->SetTitleOffset(1.2);


}
  
void format_sytle(TCanvas* canvas, TMultiGraph* mulgra){
	canvas->SetFillColor(0);
	canvas->SetBorderMode(0);
	canvas->SetBorderSize(2);
	canvas->SetTickx(1);
	canvas->SetTicky(1);
	canvas->SetLeftMargin(0.16);
	canvas->SetRightMargin(0.08);
	canvas->SetTopMargin(0.07);
	canvas->SetBottomMargin(0.13);
	canvas->SetFrameFillStyle(0);
	canvas->SetFrameBorderMode(0);
	canvas->SetFrameFillStyle(0);
	canvas->SetFrameBorderMode(0);
	canvas->SetGrid();
	mulgra->GetXaxis()->SetRangeUser(340,2000);
	mulgra->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mulgra->GetYaxis()->SetTitle("d#sigma/dM_{t#bar{t}}/#sigma");
	mulgra->GetXaxis()->CenterTitle();
	mulgra->GetYaxis()->CenterTitle();
	mulgra->GetXaxis()->SetTitleOffset(1.2);
}
//divide two graphs with same points
//   graph2/graph1
TGraph* Divided_gra(TGraph* graph1,TGraph* graph2){
	Int_t nbin1=graph1->GetN();
	Int_t nbin2=graph2->GetN();
	TGraph* divided_graph;
	if(nbin1==nbin2){
		Double_t *x1=graph1->GetX();
		Double_t *y1=graph1->GetY();
		Double_t *y2=graph2->GetY();
				
		Double_t xx[nbin1],yy[nbin1];
		Int_t j=0;
		for(Int_t i=0;i<nbin1;i++){
			if(y1[i]>0){
				xx[j]=x1[i];
				yy[j]=y2[i]/y1[i];
				j=j+1;
			}
		}
		divided_graph=new TGraph(j,xx,yy);

	}
	return divided_graph;

}
//divide two graphs using interpolate method
//   graph1/graph2
TGraph* graph_divide(TGraph* graph1, TGraph* graph2,TString title,TString line1,TString line2){
	//divide two graphs using Interpolate method
	Double_t xef[100],yef[100],yg1[100],yg2[100];
	Int_t n_point=80;
	for(int i=0;i<n_point;i++){
		if(line2=="|#Deltay_{t#bar{t}}|>1.2")
			{xef[i]=420+i*20;
			n_point=80-3;}
		else 
			xef[i]=340+i*20;

		yg1[i]=graph1->Eval(xef[i]);
		yg2[i]=graph2->Eval(xef[i]);
		yef[i]=yg1[i]/yg2[i];
	}
	TGraph *graph1_new=new TGraph(n_point,xef,yg1);
	TGraph *graph2_new=new TGraph(n_point,xef,yg2);
	TGraph *efficiency=new TGraph(n_point,xef,yef);	
	efficiency->SetLineColor(2);
	efficiency->SetLineWidth(2);
	graph1_new->SetLineColor(9);
	graph1_new->SetLineWidth(2);
	graph2_new->SetLineColor(7);
	graph2_new->SetLineWidth(2);
	TCanvas* ceffi=new TCanvas();
	gStyle->SetOptStat(kFALSE);
	gPad->SetGrid();
	graph1_new->GetYaxis()->SetRangeUser(0.92,1.12);
	graph1_new->GetXaxis()->SetRangeUser(340,2100);
	graph1_new->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	graph1_new->GetXaxis()->CenterTitle();
	graph1_new->SetTitle("");
	auto legend=new TLegend(.65,.70,.9,.9);
	TLegendEntry* leg_ent1=legend->AddEntry(graph1_new,"r^{gen}");
	TLegendEntry* leg_ent2=legend->AddEntry(graph2_new,"r^{dec}");
	TLegendEntry* leg_ent3=legend->AddEntry(efficiency,"r^{gen}/r^{dec}");
	leg_ent1->SetTextColor(9);
	leg_ent2->SetTextColor(7);
	leg_ent3->SetTextColor(2);
	//TLatex latex;
	//latex.DrawLatex(.3,.7,"#splitline{Signal Peak over}{background}");
	graph1_new->Draw("AC");
	graph2_new->Draw("Csame");
	efficiency->Draw("Csame");
	legend->Draw("same");
	TString text="#splitline{"+line1+"}{"+line2+"}";
	TLatex latex(800,1.1,text);
	latex.DrawClone("same");

	ceffi->Print(title+".png");

	return efficiency;
}
//do graph integal and make a new graph
Double_t graph_integral(TGraph* graph, Double_t xlow,Double_t xup){
	int xnum=167;
	Double_t xvalue[xnum], yvalue[xnum];
	for(int i=0;i<xnum;i++){
		xvalue[i]=340+i*10;
		yvalue[i]=graph->Eval(xvalue[i]);
	}
	TGraph* graph_temp=new TGraph(xnum,xvalue,yvalue);
	//build a TF1 using a lambda function to make integral
	//TF1 (const char *name, Double_t(*fcn)(Double_t *, Double_t *), Double_t xmin=0, Double_t xmax=1, Int_t npar=0)
	TF1 *f1=new TF1("f1",[&](Double_t *x, Double_t*){return graph_temp->Eval(x[0]);},340,2000,0);
	Double_t integral=f1->Integral(xlow,xup);
	return integral;
}
//integral graphs and calculate the ratios
TGraph* gen_ratio(TGraph *graph1,TGraph *graph2,TGraph *graph3,TGraph *graph4,int xlow,int xup){
	Double_t value1=graph_integral(graph1,xlow,xup);
	Double_t value2=graph_integral(graph2,xlow,xup);
	Double_t value3=graph_integral(graph3,xlow,xup);
	Double_t value4=graph_integral(graph4,xlow,xup);
	Double_t xvalue[4]={0.0, 1.0, 2.0, 3.0};
	Double_t yvalue[4];
	yvalue[0]=value1/value2;
	yvalue[1]=1;
	yvalue[2]=value3/value2;
	yvalue[3]=value4/value2;
	cout<<"a new graph built"<<endl;
	cout<<"value1: "<<value1<<"value2: "<<value2<<endl;
	for(int k=0;k<4;k++){
		cout<<yvalue[k]<<endl;
	}

	TGraph *ratios=new TGraph(4,xvalue,yvalue);
	ratios->SetLineWidth(2);
	ratios->SetMarkerStyle(22);
	return ratios;
}
//calculate the ratio of generator leval and detector leval and plot.
TGraph* plot_effi_yt(TGraph* graph1,TGraph*graph2,TString fileName,TString line1,TString line2){
	int num=graph2->GetN();
	Double_t *xvalue,*y1value,*y2value;
	Double_t yvalue[num];
	xvalue=graph2->GetX();
	y1value=graph1->GetY();
	y2value=graph2->GetY();

	for(int i=0;i<num;i++){
		yvalue[i]=y2value[i]/y1value[i];
	}
	TGraph* ratios_gen=new TGraph(num,xvalue,yvalue);
	
	//TCanvas *c_ratios=new TCanvas("c_ratios","c_ratios",10,30,800,800);
	TCanvas *c_ratios=new TCanvas();

	TMultiGraph *mul_ratios=new TMultiGraph();
	mul_ratios->Add(graph1);
	mul_ratios->Add(graph2);
	mul_ratios->Add(ratios_gen);
	graph1->SetLineColor(4);
	graph1->SetMarkerColor(4);
	graph2->SetLineColor(3);
	graph2->SetMarkerColor(3);
	graph1->SetMarkerStyle(21);
	ratios_gen->SetMarkerStyle(20);
	ratios_gen->SetLineColor(2);
	ratios_gen->SetMarkerColor(2);
	c_ratios->SetFillColor(0);
	c_ratios->SetBorderMode(0);
	c_ratios->SetBorderSize(2);
	c_ratios->SetTickx(1);
	c_ratios->SetTicky(1);
	c_ratios->SetLeftMargin(0.12);
	c_ratios->SetRightMargin(0.08);
	c_ratios->SetTopMargin(0.07);
	c_ratios->SetBottomMargin(0.13);
	c_ratios->SetFrameFillStyle(0);
	c_ratios->SetFrameBorderMode(0);
	c_ratios->SetFrameFillStyle(0);
	c_ratios->SetFrameBorderMode(0);
	c_ratios->SetGrid();
	mul_ratios->GetXaxis()->SetRangeUser(0,3.0);
	mul_ratios->GetXaxis()->SetTitle("Y_{t}");
	mul_ratios->GetXaxis()->CenterTitle();
	mul_ratios->GetYaxis()->SetRangeUser(0.90,1.20);
	mul_ratios->Draw("ACP");
	auto legend=new TLegend(.17,.65,.39,.9);
	TLegendEntry* leg_ent1=legend->AddEntry(graph1,"R^{bin}_{dec}");
	TLegendEntry* leg_ent2=legend->AddEntry(graph2,"R^{bin}_{gen}");
	TLegendEntry* leg_ent3=legend->AddEntry(ratios_gen,"R^{bin}_{gen}/R^{bin}_{dec}");
	leg_ent1->SetTextColor(4);
	leg_ent2->SetTextColor(3);
	leg_ent3->SetTextColor(2);
	legend->Draw("same");
	TString text="#splitline{"+line1+"}{"+line2+"}";
	TLatex latex(1.5,1.11,text);
	latex.DrawClone("same");
	TLatex latex2(1.5,1.15,"3 jets");
	latex2.DrawClone("same");
	c_ratios->Print(fileName+".png");
	return ratios_gen;

}

void plot_mtt_cut(){
	//draw graphs 
	gStyle->SetOptStat(kFALSE);
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

	gr0->Sort();
	gr1->Sort();
	gr2->Sort();
	gr3->Sort();
//	gr1->Draw("ACP");
	

	//draw histograms
	int nbin=gr1->GetN();
	int nbin3=gr3->GetN();
	TFile* file=new TFile("./lhereader/tree_pp_ttbar.root");
	TTree *mytree;
	file->GetObject("mytree",mytree);//I have a tree named "mytree" inside the root file.
	TH1F *h1=new TH1F("ht1","",nbin,320,2000); //define a one-dimension histogram
	TH1F *h2=new TH1F("ht2","hist of mtt with cut",nbin,320,2000);
	TH1F *h3=new TH1F("ht3","hist of mtt with cut2",nbin,320,2000);
	mytree->Draw("M_tt>>ht1","abs(delta_rapidity)<0.6"); 
	mytree->Draw("M_tt>>ht2","abs(delta_rapidity)>0.6&&abs(delta_rapidity)<1.2");
	mytree->Draw("M_tt>>ht3","abs(delta_rapidity)>1.2");

	//build new graphs, including EW corrections.
	TGraph *gra1,*gra2,*gra3,*gra4,*gra5,*gra6;
	gra1=create_graph(gr1,h1);	
	gra2=create_graph(gr1,h2);
	gra3=create_graph(gr1,h3);
	gra4=create_graph(gr2,h1);
	gra5=create_graph(gr2,h2);
	gra6=create_graph(gr2,h3);
	gra1->SetLineColor(2);
	gra2->SetLineColor(3);
	gra3->SetLineColor(4);
	gra4->SetLineColor(6);
	gra5->SetLineColor(7);
	gra6->SetLineColor(9);

	gra1->SetLineWidth(2);
	gra2->SetLineWidth(2);
	gra3->SetLineWidth(2);
	gra4->SetLineWidth(2);
	gra5->SetLineWidth(2);
	gra6->SetLineWidth(2);

	TCanvas *c1=new TCanvas("c","",10,30,800,800);

	TMultiGraph *mg = new TMultiGraph();
	mg->Add(gra1);
	mg->Add(gra2);
	mg->Add(gra3);
	mg->Add(gra4);
	mg->Add(gra5);
	mg->Add(gra6);	
	format_sytle(c1,mg);
	mg->GetYaxis()->SetRangeUser(0,0.4);
	mg->Draw("ACP");

	auto leg=new TLegend(.55,.60,.89,.9);
	TLegendEntry* leg_entry1=leg->AddEntry(gra1,"Y_{t}=1, |#Deltay_{t#bar{t}}|<0.6");
	TLegendEntry* leg_entry2=leg->AddEntry(gra2,"Y_{t}=1, 0.6<|#Deltay_{t#bar{t}}|<1.2");
	TLegendEntry* leg_entry3=leg->AddEntry(gra3,"Y_{t}=1, |#Deltay_{t#bar{t}}|>1.2");

	TLegendEntry* leg_entry4=leg->AddEntry(gra4,"Y_{t}=2, |#Deltay_{t#bar{t}}|<0.6");
	TLegendEntry* leg_entry5=leg->AddEntry(gra5,"Y_{t}=2, 0.6<|#Deltay_{t#bar{t}}|<1.2");
	TLegendEntry* leg_entry6=leg->AddEntry(gra6,"Y_{t}=2, |#Deltay_{t#bar{t}}|>1.2");	
	leg_entry1->SetTextColor(2);
	leg_entry2->SetTextColor(3);
	leg_entry3->SetTextColor(4);
	leg_entry4->SetTextColor(6);
	leg_entry5->SetTextColor(7);
	leg_entry6->SetTextColor(9);
	leg->Draw("same");
	gPad->Print("mtt_yt_cut.png");
 
	//plot the ratio of Y_t=2 and Y_t=1
	TCanvas *c2=new TCanvas("c2","c2",10,30,800,800);
	TMultiGraph* mulg=new TMultiGraph();
//	gra1->GetXaxis()->SetRangeUser(340,950);
//	gra4->GetXaxis()->SetRangeUser(340,950);

	mulg->Add(gra1);
	mulg->Add(gra4);
	mulg->GetYaxis()->SetTitle("d#sigma/dM_{t#bar{t}}/#sigma");
	format_sytle(c2,mulg);

	c2->SetLeftMargin(0.12);
	c2->Divide(1,2,0.0,0.0);
	c2->cd(1);
	mulg->Draw("APC");
	mulg->GetYaxis()->SetTitleSize(0.06);	
	mulg->GetYaxis()->SetTitleOffset(0.6);
	mulg->GetXaxis()->SetRangeUser(340,950);
	mulg->GetYaxis()->SetRangeUser(0.0,0.42);

	c2->GetPad(1)->SetGrid();
	auto legend=new TLegend(.55,.60,.90,.9);
	TLegendEntry* leg_ent1=legend->AddEntry(gra1,"Y_{t}=1, |#Deltay_{t#bar{t}}|<0.6");
	TLegendEntry* leg_ent4=legend->AddEntry(gra4,"Y_{t}=2, |#Deltay_{t#bar{t}}|<0.6"); 
	leg_ent1->SetTextColor(2);
	leg_ent4->SetTextColor(6);
	legend->Draw("same");
	
	c2->cd(2);
	TGraph* divided_graph=Divided_gra(gra1,gra4); ////R(Y_t=2)/R(Y_t=1) with y_tt in first interval in generator level
	//divided_graph->SetPoint(0,340,1.04);
	divided_graph->SetTitle(";M_{t#bar{t}}[GeV];R^{gen}(Y_{t}=2)/R^{gen}(Y_{t}=1)");
	divided_graph->GetYaxis()->CenterTitle();
	c2->GetPad(2)->SetGrid();
	divided_graph->SetLineWidth(2);
	divided_graph->SetLineColor(9);
	divided_graph->GetYaxis()->SetTitleSize(0.06);
	divided_graph->GetXaxis()->SetTitleSize(0.06);
	divided_graph->GetYaxis()->SetTitleOffset(0.6); 
	divided_graph->GetXaxis()->SetRangeUser(340,950);
	divided_graph->Draw("AC");
	c2->Print("mtt_ratio12.png");

//plot ratios  with cut1, cut2 and cut3.
	auto divided_graph2=Divided_gra(gra2,gra5);//R(Y_t=2)/R(Y_t=1) with y_tt in the second interval
	auto divided_graph3=Divided_gra(gra3,gra6);////R(Y_t=2)/R(Y_t=1) with y_tt in the third interval
	divided_graph2->SetLineColor(9);
	divided_graph2->SetLineWidth(2);
	divided_graph3->SetLineColor(9);
	divided_graph3->SetLineWidth(2);

//read detector level data and build graphs.
	TCanvas *ctempt=new TCanvas();

	TTree *tdec = new TTree("tdec", "tree from mtt_dec.csv");
	tdec->ReadFile("mtt_dec.csv", "bin31_x/D:bin31_y:bin32_x:bin32_y:bin33_x:bin33_y:bin41_x:bin41_y:bin42_x:bin42_y:bin43_x:bin43_y:bin51_x:bin51_y:bin52_x:bin52_y:bin53_x:bin53_y");
	tdec->Draw("bin31_y:bin31_x");
	TGraph *gh31=(TGraph*) gROOT->FindObject("Graph")->Clone();	
	tdec->Draw("bin32_y:bin32_x");
	TGraph *gh32=(TGraph*) gROOT->FindObject("Graph")->Clone();	
	tdec->Draw("bin33_y:bin33_x");
	TGraph *gh33=(TGraph*) gROOT->FindObject("Graph")->Clone();
	tdec->Draw("bin41_y:bin41_x");
	TGraph *gh41=(TGraph*) gROOT->FindObject("Graph")->Clone();	
	tdec->Draw("bin42_y:bin42_x"); 
	TGraph *gh42=(TGraph*) gROOT->FindObject("Graph")->Clone();
	tdec->Draw("bin43_y:bin43_x");
	TGraph *gh43=(TGraph*) gROOT->FindObject("Graph")->Clone();
	tdec->Draw("bin51_y:bin51_x");
	TGraph *gh51=(TGraph*) gROOT->FindObject("Graph")->Clone();
	tdec->Draw("bin52_y:bin52_x");
	TGraph *gh52=(TGraph*) gROOT->FindObject("Graph")->Clone();		
	tdec->Draw("bin53_y:bin53_x");
	TGraph *gh53=(TGraph*) gROOT->FindObject("Graph")->Clone();	

	gh31->SetLineWidth(2);
	gh32->SetLineWidth(2);
	gh33->SetLineWidth(2);
	gh41->SetLineWidth(2);
	gh42->SetLineWidth(2);
	gh43->SetLineWidth(2);
	gh51->SetLineWidth(2);
	gh52->SetLineWidth(2);
	gh53->SetLineWidth(2);
	gh31->SetLineColor(7);
	gh32->SetLineColor(7);
	gh33->SetLineColor(7);
	gh41->SetLineColor(7);
	gh42->SetLineColor(7);
	gh43->SetLineColor(7);
	gh51->SetLineColor(7);
	gh52->SetLineColor(7);
	gh53->SetLineColor(7);
	gh31->Sort();
	gh32->Sort();
	gh33->Sort();

	TGraph* efficiency31=graph_divide(divided_graph,gh31,"effi31","3 jets","|#Deltay_{t#bar{t}}|<0.6");
	// effficiency of 3jets with y_tt in the first interval
	TGraph* efficiency32=graph_divide(divided_graph2,gh32,"effi32","3 jets","0.6<|#Deltay_{t#bar{t}}|<1.2");
	//efficiency of 3jets with y_tt in the second interval
	TGraph* efficiency33=graph_divide(divided_graph3,gh33,"effi33","3 jets","|#Deltay_{t#bar{t}}|>1.2");
	//efficiency of 3jets with y_tt in the second interval
	
	TGraph* efficiency41=graph_divide(divided_graph,gh41,"effi41","4 jets","|#Deltay_{t#bar{t}}|<0.6");
	TGraph* efficiency42=graph_divide(divided_graph2,gh42,"effi42","4 jets","0.6<|#Deltay_{t#bar{t}}|<1.2");
	TGraph* efficiency43=graph_divide(divided_graph3,gh43,"effi43","4 jets","|#Deltay_{t#bar{t}}|>1.2");
	TGraph* efficiency51=graph_divide(divided_graph,gh51,"effi51","5 jets","|#Deltay_{t#bar{t}}|<0.6");
	TGraph* efficiency52=graph_divide(divided_graph2,gh52,"effi52","5 jets","0.6<|#Deltay_{t#bar{t}}|<1.2");
	TGraph* efficiency53=graph_divide(divided_graph3,gh53,"effi53","5 jets","|#Deltay_{t#bar{t}}|>1.2");


//plot efficiency, ratio of generator level and detector level.
//plot the results of 3jets
	TCanvas * c3=new TCanvas("c3","c3",10,30,1000,600);
	c3->Divide(3,1,0.0,0.0);
	c3->cd(1);
	TMultiGraph *mg31=new TMultiGraph();
	mg31->Add(divided_graph);
	mg31->Add(gh31);
	mg31->Add(efficiency31);
	mg31->GetYaxis()->SetRangeUser(0.95,1.12);
	mg31->GetXaxis()->SetRangeUser(340,2100);
	mg31->Draw("ACP");
	mg31->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mg31->GetXaxis()->CenterTitle();
	c3->GetPad(1)->SetGrid();
	TLatex latex1(800,1.10,"#splitline{3 jets}{|#Deltay_{t#bar{t}}|<0.6}");
	latex1.DrawClone("same");

	c3->cd(2);
	TMultiGraph *mg32=new TMultiGraph();
	mg32->Add(divided_graph2);
	mg32->Add(gh32);
	mg32->Add(efficiency32);
	mg32->GetYaxis()->SetRangeUser(0.95,1.12);
	mg32->GetXaxis()->SetRangeUser(340,2100);
	mg32->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mg32->GetXaxis()->CenterTitle();
	c3->GetPad(2)->SetGrid();
	mg32->Draw("ACP");
	TLatex latex2(800,1.10,"#splitline{3 jets}{0.6<|#Deltay_{t#bar{t}}|<1.2}");
	latex2.DrawClone("same");

	c3->cd(3);
	TMultiGraph *mg33=new TMultiGraph();
	mg33->Add(divided_graph3);
	mg33->Add(gh33);
	mg33->Add(efficiency33);
	mg33->GetYaxis()->SetRangeUser(0.95,1.12);
	mg33->GetXaxis()->SetRangeUser(340,2100);
	mg33->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mg33->GetXaxis()->CenterTitle();
	mg33->Draw("ACP");
	c3->GetPad(3)->SetGrid();
	TLatex latex3(800,1.10,"#splitline{3 jets}{|#Deltay_{t#bar{t}}|>1.2}");
	latex3.DrawClone("same");

	c3->Print("3jets_mtt.png");
	
// plot ratio (efficiency) according to jet number.
//seperate 3jets 4jets 5jets to show wheter there are distinguish.
	TCanvas * c4=new TCanvas("c3","c3",10,30,1000,600);
	c4->SetFillColor(0);
	c4->SetBorderMode(0);
	c4->SetBorderSize(2);
	c4->SetTickx(1);
	c4->SetTicky(1);
	c4->SetLeftMargin(0.18);
	c4->SetRightMargin(0.08);
	c4->SetTopMargin(0.07);
	c4->SetBottomMargin(0.13);
	c4->SetFrameFillStyle(0);
	c4->SetFrameBorderMode(0);
	c4->SetFrameFillStyle(0);
	c4->SetFrameBorderMode(0);
	c4->Divide(3,1,0.0,0.0);
	c4->cd(1);
	TMultiGraph *mg_bin1=new TMultiGraph();
	efficiency31->SetLineColor(6);
	efficiency41->SetLineColor(7);
	efficiency51->SetLineColor(9);
	//format_sytle(c4,mg_bin1);
	mg_bin1->Add(efficiency31);
	mg_bin1->Add(efficiency41);
	mg_bin1->Add(efficiency51);
	mg_bin1->GetYaxis()->SetRangeUser(0.92,1.11);
	mg_bin1->GetXaxis()->SetRangeUser(340,2100);
	mg_bin1->Draw("ACP");
	mg_bin1->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mg_bin1->GetYaxis()->SetTitle("r^{gen}/r^{dec}");
	mg_bin1->GetYaxis()->SetTitleSize(0.06);	
	mg_bin1->GetXaxis()->SetTitleSize(0.06);	

	mg_bin1->GetYaxis()->CenterTitle();
	mg_bin1->GetXaxis()->CenterTitle();
	c4->GetPad(1)->SetGrid();
	auto legend_bin1=new TLegend(.69,.75,.94,.95);
	TLegendEntry* leg_3jets1=legend_bin1->AddEntry(efficiency31,"3 jets");
	TLegendEntry* leg_4jets1=legend_bin1->AddEntry(efficiency41,"4 jets");
	TLegendEntry* leg_5jets1=legend_bin1->AddEntry(efficiency51,"5 jets");
	leg_3jets1->SetTextColor(6);
	leg_4jets1->SetTextColor(7);
	leg_5jets1->SetTextColor(9);
	legend_bin1->Draw("same");
	TLatex latex_bin1(800,1.05,"|#Deltay_{t#bar{t}}|<0.6");
	latex_bin1.DrawClone("same");

	c4->cd(2);
	TMultiGraph *mg_bin2=new TMultiGraph();
	efficiency32->SetLineColor(6);
	efficiency42->SetLineColor(7);
	efficiency52->SetLineColor(9);
	mg_bin2->Add(efficiency32);
	mg_bin2->Add(efficiency42);
	mg_bin2->Add(efficiency52);
	mg_bin2->GetYaxis()->SetRangeUser(0.92,1.11);
	mg_bin2->GetXaxis()->SetRangeUser(340,2100);
	mg_bin2->Draw("ACP");
	mg_bin2->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mg_bin2->GetXaxis()->CenterTitle();
	mg_bin2->GetXaxis()->SetTitleSize(0.06);	

	c4->GetPad(2)->SetGrid();
	auto legend_bin2=new TLegend(.69,.75,.94,.95);
	TLegendEntry* leg_3jets2=legend_bin2->AddEntry(efficiency32,"3 jets");
	TLegendEntry* leg_4jets2=legend_bin2->AddEntry(efficiency42,"4 jets");
	TLegendEntry* leg_5jets2=legend_bin2->AddEntry(efficiency52,"5 jets");
	legend_bin2->Draw("same");

	leg_3jets2->SetTextColor(6);
	leg_4jets2->SetTextColor(7);
	leg_5jets2->SetTextColor(9);
	TLatex latex_bin2(800,1.05,"0.6<|#Deltay_{t#bar{t}}|<1.2");
	latex_bin2.DrawClone("same");


	c4->cd(3);
	TMultiGraph *mg_bin3=new TMultiGraph();
	efficiency33->SetLineColor(6);
	efficiency43->SetLineColor(7);
	efficiency53->SetLineColor(9);
	mg_bin3->Add(efficiency33);
	mg_bin3->Add(efficiency43);
	mg_bin3->Add(efficiency53);
	mg_bin3->GetYaxis()->SetRangeUser(0.92,1.11);
	mg_bin3->GetXaxis()->SetRangeUser(340,2100);
	mg_bin3->Draw("ACP");
	mg_bin3->GetXaxis()->SetTitle("M_{t#bar{t}}[GeV]");
	mg_bin3->GetXaxis()->CenterTitle();
	mg_bin3->GetXaxis()->SetTitleSize(0.06);	

	c4->GetPad(3)->SetGrid();
	auto legend_bin3=new TLegend(.69,.75,.94,.95);
	TLegendEntry* leg_3jets3=legend_bin3->AddEntry(efficiency31,"3 jets");
	TLegendEntry* leg_4jets3=legend_bin3->AddEntry(efficiency41,"4 jets");
	TLegendEntry* leg_5jets3=legend_bin3->AddEntry(efficiency51,"5 jets");
	legend_bin3->Draw("same");

	leg_3jets3->SetTextColor(6);
	leg_4jets3->SetTextColor(7);
	leg_5jets3->SetTextColor(9);
	TLatex latex_bin3(800,1.05,"|#Deltay_{t#bar{t}}|>1.2");
	latex_bin3.DrawClone("same");

	c4->Print("jets_mtt.png");

//plot differential cross section with respect m_tt at abs(Deltay_{tt})<0.6 
// take y_tt as 0,1,2,3,4.
	int points=gr0->GetN();
	Double_t xgr3_new[points],ygr3_new[points];
	Double_t *xgr3,*ygr3;
	xgr3=gr3->GetX();
	ygr3=gr3->GetY();
	for(int i=0;i<points;i++){
		if(i<26){
			xgr3_new[i]=xgr3[i];
			ygr3_new[i]=ygr3[i];
		}
		else{
			xgr3_new[i]=340+i*40;
			ygr3_new[i]=gr3->Eval(xgr3_new[i]);
		}
	//cout<<xgr3_new[i]<<" "<<ygr3_new[i]<<endl;

	}
	TGraph *gr3_new=new TGraph(points,xgr3_new,ygr3_new);

	TGraph *gra01,*gra11,*gra21,*gra31;
	gra01=create_graph(gr0,h1);
	gra11=create_graph(gr1,h1);
	gra21=create_graph(gr2,h1);
	gra31=create_graph(gr3_new,h1);

	gra01->SetLineColor(2);
	gra11->SetLineColor(3);
	gra21->SetLineColor(4);
	gra31->SetLineColor(6);
	gra01->SetLineWidth(2);
	gra11->SetLineWidth(2);
	gra21->SetLineWidth(2);
	gra31->SetLineWidth(2);

	TCanvas *c5=new TCanvas("c5","c5",10,30,800,800);
	TMultiGraph *mg_yt = new TMultiGraph();
	mg_yt->Add(gra01);
	mg_yt->Add(gra11);
	mg_yt->Add(gra21);
	mg_yt->Add(gra31);
	format_sytle(c5,mg_yt);
	mg_yt->GetYaxis()->SetRangeUser(0,0.4);
	mg_yt->Draw("ACP");

	auto leg_yt=new TLegend(.55,.60,.89,.9);
	TLegendEntry* leg_yt0=leg_yt->AddEntry(gra01,"Y_{t}=0");
	TLegendEntry* leg_yt1=leg_yt->AddEntry(gra11,"Y_{t}=1");
	TLegendEntry* leg_yt2=leg_yt->AddEntry(gra21,"Y_{t}=2");
	TLegendEntry* leg_yt3=leg_yt->AddEntry(gra31,"Y_{t}=3");
	leg_yt0->SetTextColor(2);
	leg_yt1->SetTextColor(3);
	leg_yt2->SetTextColor(4);
	leg_yt3->SetTextColor(6);
	leg_yt->Draw("same");
	c5->Print("mtt_yt_ytt1.png");

//plot R^{bin } in detector level
    TCanvas* ct=new TCanvas();
	TTree *t3jets = new TTree("t", "tree from 3jets_bins.csv");
	t3jets->ReadFile("3jets_bins.csv", "x/D:bin1:bin2:bin3:bin4:bin5:bin6:bin7:bin8");
	t3jets->Draw("bin4:x");
	TGraph *jets3_bin4=(TGraph*) gROOT->FindObject("Graph")->Clone();
	t3jets->Draw("bin5:x");
	TGraph *jets3_bin5=(TGraph*) gROOT->FindObject("Graph")->Clone();
	t3jets->Draw("bin6:x");
	TGraph *jets3_bin6=(TGraph*) gROOT->FindObject("Graph")->Clone();
	t3jets->Draw("bin7:x");
	TGraph *jets3_bin7=(TGraph*) gROOT->FindObject("Graph")->Clone();
	t3jets->Draw("bin8:x");
	TGraph *jets3_bin8=(TGraph*) gROOT->FindObject("Graph")->Clone();

//calculate R^{bin} in generator level
	TGraph* ratio_bin1=gen_ratio(gra01,gra11,gra21,gra31,340,360);
	TGraph* ratio_bin2=gen_ratio(gra01,gra11,gra21,gra31,360,400);
	TGraph* ratio_bin3=gen_ratio(gra01,gra11,gra21,gra31,400,440);
	TGraph* ratio_bin4=gen_ratio(gra01,gra11,gra21,gra31,440,500);
	TGraph* ratio_bin5=gen_ratio(gra01,gra11,gra21,gra31,500,2000);
//plot efficiency at y_tt fixed at the first area.
	plot_effi_yt(jets3_bin4,ratio_bin1,"ratio_yt4","M_{t#bar{t}}=340-360","|#Deltay_{t#bar{t}}|=0-0.6");
	plot_effi_yt(jets3_bin5,ratio_bin2,"ratio_yt5","M_{t#bar{t}}=360-400","|#Deltay_{t#bar{t}}|=0-0.6");
	plot_effi_yt(jets3_bin6,ratio_bin3,"ratio_yt6","M_{t#bar{t}}=400-440","|#Deltay_{t#bar{t}}|=0-0.6");
	plot_effi_yt(jets3_bin7,ratio_bin4,"ratio_yt7","M_{t#bar{t}}=440-500","|#Deltay_{t#bar{t}}|=0-0.6");
	plot_effi_yt(jets3_bin8,ratio_bin5,"ratio_yt8","M_{t#bar{t}}=500-2000","|#Deltay_{t#bar{t}}|=0-0.6");


}