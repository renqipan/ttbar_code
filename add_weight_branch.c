void add_weight_branch(){
	//add weight(kappa=2) to generator level tree
	
    TH2F* hist[8];
    TString dir="/home/renqi/Documents/ttbar_cp/correction_roots/";
    TString files[8]={"correction_kappa1","correction_kappa2","correction_kappa3",
                  "correction_tilde1","correction_tilde2","correction_kappa1tilde1",
                  "correction_kappa2tilde2","correction_kappa0"};
    for(Int_t i=0;i<8;i++){
      TFile* fhist=TFile::Open(dir+files[i]+".root");
      hist[i]=(TH2F*)fhist->Get("h2");

      }             
  

	TFile *file=new TFile("/home/renqi/Documents/ttbar_cp/lhereader/tree_pp_ttbar_decay.root","update");
	TTree *tree1=(TTree*)file->Get("mytree");
	Float_t M_tt,delta_rapidity;
	tree1->SetBranchAddress("M_tt_gen",&M_tt);
	tree1->SetBranchAddress("delta_rapidity_gen",&delta_rapidity);
	Float_t weight[8];
	TTree* mytree=new TTree("newtree","tree with branch");

   	mytree->Branch("weight_kappa1",&weight[0],"weight_kappa1/F");
    mytree->Branch("weight_kappa2",&weight[1],"weight_kappa2/F");
    mytree->Branch("weight_kappa3",&weight[2],"weight_kappa3/F");
    mytree->Branch("weight_tilde1",&weight[3],"weight_tilde1/F");
    mytree->Branch("weight_tilde2",&weight[4],"weight_tilde2/F");
    mytree->Branch("weight_kappa1tilde1",&weight[5],"weight_kappa1tilde1/F");
    mytree->Branch("weight_kappa2tilde2",&weight[6],"weight_kappa2tilde2/F");
    mytree->Branch("weight_kappa0",&weight[7],"weight_kappa0/F");

    mytree->Branch("M_tt_gen",&M_tt,"M_tt_gen/F");
    mytree->Branch("delta_rapidity_gen",&delta_rapidity,"delta_rapidity_gen/F");	
    Int_t entries=tree1->GetEntries();
	for(Int_t i=0;i<entries;i++){
		tree1->GetEntry(i);
		//cout<<"M_tt: "<<M_tt<<endl;
		//cout<<"delta_rapidity: "<<delta_rapidity<<endl;
		for(Int_t i=0;i<8;i++){
            Int_t nbin=hist[i]->FindBin(M_tt,delta_rapidity);
            weight[i]=1.0+hist[i]->GetBinContent(nbin);
           //cout<<"weight[i]: "<<weight[i]<<endl;
           }
           mytree->Fill();

	}
	mytree->Write("",TObject::kOverwrite);
	
    cout<<"number of entries: "<<mytree->GetEntries()<<endl;
    cout<<"branch weight_kaap2 has been added."<<endl;
    cout<<"tree_pp_ttbar_decay.root has been updated"<<endl;
	file->Close();
	//fhist->Close();

}