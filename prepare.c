//build 2-D RooHistPdf and datacard file
using namespace std;
using namespace RooFit;

void writeline(vector<TString> arr , ofstream &card){
	for (int i=0;i<arr.size();i++)
		card<< arr[i]<<"\t";
	card<<endl;
}

void writeline(vector<int> arr , ofstream &card){
	for (int i=0;i<arr.size();i++)
		card<< arr[i]<<"\t";
	card<<endl;
}
void writeline(vector<float> arr , ofstream &card){
	//	gStyle->SetPaintTextFormat("2.2f");
	card<< std::fixed;
	card<< std::setprecision(3);
	for (int i=0;i<arr.size();i++)
		card<< arr[i]<<"\t";
	card<<endl;
}
void Floor(TH2F* histo){
	for (int i=0;i<histo->GetNbinsX();i++){
		for (int j=0;j<histo->GetNbinsY();j++){
			if(histo->GetBinContent(i+1,j+1)==0)
				histo->SetBinContent(i+1,j+1,1.E-9);
		}
	}
}

void Floor(TH1F* histo){
	//	histo->Smooth();
	for (int i=0;i<histo->GetNbinsX();i++){
		if(histo->GetBinContent(i+1)==0)
			histo->SetBinContent(i+1,1.E-10);
	}
}

void prepare(){
	int pro_num=6;
	TString processNames[]={"ttbar_Aterm","ttbar_Bterm","ttbar_Cterm","ttbar_Dterm","ttbar_Eterm","ttbar_Fterm"};
	vector<TString>  process_names(processNames,processNames+pro_num); 
	int processID[]={-5,-4,-3,-2,-1,0};
	vector<int> process_array(processID,processID+pro_num);
	vector<TString> bin_arr; // store category names

	TString weights[]={"weight_kappa1","weight_kappa0","weight_kappa2","weight_tilde1","weight_tilde2","weight_kappa2tilde2"};

	TChain *chain1= new TChain("mytree");
	chain1->Add("new_pp_ttbar_nlo.root");
	cout<<"number of entries: "<<chain1->GetEntries()<<" in ttbar production"<<endl;
	//define two variables
	RooRealVar* mtt=new RooRealVar("mass_tt","mass_tt",0,2000);
	RooRealVar* ytt=new RooRealVar("rapidity_tt","rapidity_tt",-4,4);
	mtt->setBins(7);
	ytt->setBins(5);
	TString outputDir="datacard";
	TFile *f=new TFile(outputDir+"/"+"ttbar.root","recreate");
	RooWorkspace w("w");
	vector<float> yield_array;  //store rate(event yeild)
	vector<TString> bkg_norm;  //store background  normlization uncertainty
	vector<TString> sig_norm;   //store signal norlization uncertainty
	vector<TString> weight_error; // store weight(EW correction) uncertainty
	vector<TString> others_un;   //store other's uncertainty

	//build RooHistPdf of signal(ttbar)
	//Double_t edges[8]={0,360,440,520,620,720,950,2000};
	//Double_t rap_edges[8]={-4,-1.8,-1,-0.2,0.6,1.2,2,4};
	Double_t edges[8]={0,420,540,710,850,990,1200,2000};
	Double_t rap_edges[6]={-4,-1.1,-0.45,0.45,1.1,4};			
	for(Int_t i=0;i<pro_num;i++){
		TH2F* h2dist=new TH2F(process_names[i]+"_hist",process_names[i]+"_hist",7,edges,5,rap_edges);
		float yield=0;
		chain1->Draw("rapidity_tt:mass_tt>>"+process_names[i]+"_hist",weights[i]+"*832000*137.1/200000");
		//Floor(h2dist);
		h2dist->Draw("colz");
		gPad->Print(outputDir+"/"+process_names[i]+"_hist.png");
		yield=h2dist->GetSumOfWeights();
		yield_array.push_back(yield);
		bin_arr.push_back("ttbar");
		sig_norm.push_back("1.08");
		bkg_norm.push_back("-");
		weight_error.push_back("1.03");
		others_un.push_back("1.15");
		cout<<"there are "<<yield<<" events in process "<<processNames[i]<<endl;
		RooDataHist* datahist=new RooDataHist(process_names[i]+"datahist",process_names[i]+"datahist",RooArgSet(*mtt,*ytt),h2dist);
		RooHistPdf* hist_pdf=new RooHistPdf(process_names[i],process_names[i],RooArgSet(*mtt,*ytt),*datahist);
		w.import(*hist_pdf,RecycleConflictNodes());
	}
	//build RooHistPdf of background(single top)
	TChain *chain2= new TChain("mytree");
	chain2->Add("new_pp_top.root");
	Int_t num_top=chain2->GetEntries();
	//TString weight_top="45365/"+TString(num_top);
	cout<<"number of entries: "<<chain2->GetEntries()<<" in single top"<<endl;
	TH2F* h2dist_top=new TH2F("single_top_hist","single_top_hist",7,edges,5,rap_edges);
	float yield_top;
	chain2->Draw("rapidity_tt:mass_tt>>single_top_hist","259000*137.1/200000*1.71*0.105");
	yield_top=h2dist_top->GetSumOfWeights();
	cout<<"there are "<<yield_top<<" events in process single top;"<<endl;
	//Floor(h2dist_top);
	h2dist_top->Draw("colz");
	gPad->Print(outputDir+"/single_top_hist.png");
	RooDataHist *tophist=new RooDataHist("single_top_datahist","single_top_datahist",RooArgSet(*mtt,*ytt),h2dist_top);
	RooHistPdf *toppdf=new RooHistPdf("single_top","single_top",RooArgSet(*mtt,*ytt),*tophist);
	w.import(*toppdf,RecycleConflictNodes());
	yield_array.push_back(yield_top);
	process_names.push_back("single_top");
	process_array.push_back(1);
	bin_arr.push_back("ttbar");
	sig_norm.push_back("-");
	bkg_norm.push_back("1.3");
	weight_error.push_back("-");
	others_un.push_back("1.15");

	//write corresponding datcard (.txt) file
	ofstream card;
	card.open (outputDir+"/"+"ttbar.txt");
	card <<"Datacard for event category: "<< "ttbar"<<endl;
	card<< "imax 1 number of channels"<<endl;
	card<< "jmax 6 number of processes minus 1"<<endl;
	card<< "kmax * number of nuisance parameters"<<endl;
	card<<"---------------------------------"<<endl;
	card<<endl;
	card<< "shapes * "<< "ttbar" << " "<< "ttbar.root w:$PROCESS w:$PROCESS_$SYSTEMATIC" <<endl;
	card<<"---------------------------------"<<endl;
	card<< "bin           "<< "ttbar" <<endl;
	card<< "observation   "<< "-1"<<endl;
	card<<"---------------------------------"<<endl;
	card<<endl;
	card<<"bin \t";
	writeline(bin_arr,card);
	card<<"process \t";
	writeline(process_names,card);
	card<<"process \t";
	writeline(process_array,card);
	card<<"rate \t";
	writeline(yield_array,card);
	card<<"sig_norm"<<"\t lnN \t";
	writeline(sig_norm,card);
	card<<"bkg_norm"<<"\t lnN \t";
	writeline(bkg_norm,card);
	//card<<"weight_error"<<"\t lnN \t";
	//writeline(weight_error,card);
	card<<"others_un"<<"\t lnN \t";
	writeline(others_un,card);

	//build dataset, but for expected results dataset is not needed.
    RooDataSet *data;
    data=new RooDataSet("data_obs","",chain1,RooArgList(*mtt,*ytt));
    w.import(*data);
    w.Print();
    cout<<"---------------------------------------"<<endl;
    cout<<"expected data: "<<yield_array[0]+yield_array[6]<<endl;
	cout<<"events of ttbar in SM: "<<yield_array[0]<<endl;
	cout<<"events of background: "<<yield_array[6]<<endl;
	cout<<"------------------------------------------"<<endl;
	
	cout<<"events[kappa=0]:events[kappa=1]: "<<yield_array[1]/yield_array[0]<<endl;
	cout<<"events[kappa=2]:events[kappa=1]: "<<yield_array[2]/yield_array[0]<<endl;
	cout<<"events[kappa_tilde=1]:events[kappa=1]: "<<yield_array[3]/yield_array[0]<<endl;
	cout<<"events[kappa_tilde=2]:events[kappa=1]: "<<yield_array[4]/yield_array[0]<<endl;
	cout<<"events[kappa=tilde=2]:events[kappa=1]: "<<yield_array[5]/yield_array[0]<<endl;


	card.close();
	f->cd();
	w.Write();
	f->Close();

}