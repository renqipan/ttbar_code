 #contour2D(TString datafile,TString xvar, int xbins, float xmin, float xmax, TString yvar, int ybins, float ymin, 
 #float ymax, float smx=1.0, float smy=1.0, TString name="contour2D",TString xtitle="#mu_{ggH,bbH,ttH}", 
 #TString ytitle="#mu_{VBF,VH}",TFile *fOut=0 )

root -q -b contour2D.cxx"(\"higgsCombinettbar_expect.MultiDimFit.mH125.root\",\"kappa\",20,0.0,2.0,\"kappa_t\",20,-2.,2.,1.,0.,\"kappa_tilde\",\"#kappa\",\"#tilde{#kappa}\")"
