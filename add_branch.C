/*
this file should be ran inside the Delphes file
root -l -q examples/add_branch.C'("pp_ttbar_nlo.root")'
*/
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

void add_branch(TString inputFile){
	gSystem->Load("libDelphes");

	TFile *file=TFile::Open("/home/renqi/Documents/ttbar_cp/lhereader/tree_pp_ttbar_decay.root");
	TTree *tree1=(TTree*)file->Get("mytree");
	Float_t M_tt,delta_rapidity;
	tree1->SetBranchAddress("M_tt_gen",&M_tt);
	tree1->SetBranchAddress("delta_rapidity_gen",&delta_rapidity);
	TFile *file2=new TFile(inputFile,"update");
	
	TTree *mytree=(TTree*) file2->Get("Delphes");
	TBranch* mtt=mytree->Branch("M_tt_gen",&M_tt,"M_tt_gen/F");
	TBranch* ytt=mytree->Branch("delta_rapidity_gen",&delta_rapidity,"delta_rapidity_gen/F");
	Int_t num_entries=(Int_t) tree1->GetEntries();
	for(Int_t i=0;i< num_entries;i++){
		tree1->GetEntry(i);
		mtt->Fill();
		ytt->Fill();
	}

	mytree->Write("",TObject::kOverwrite);
	file->Close();
	file2->Close();
	cout<<"num_entries: "<<num_entries<<endl;




}