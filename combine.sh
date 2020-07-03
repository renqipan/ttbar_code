#!/bin/bash
source env.sh
cd $YourCombine_CMSSW_src_Dir #/directory of higgs combine analysis
eval `scramv1 runtime -sh`
cd $YourDatacard13TeV_Dir #/ directory of datacard
cd datacard/

text2workspace.py ttbar.txt -o workspace_ttbar.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_tt:stagex_tt -m 125 --X-allow-no-background  -v 7 
text2workspace.py ttbar.txt -o workspace_ttbar_fcp.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttfcp:stagex_ttfcp -m 125 --X-allow-no-background  -v 7 
text2workspace.py ttbar.txt -o workspace_ttbar_fcpng.root --PO doStage0 --PO doacttbar_ng -P HiggsAnalysis.CombinedLimit.stagex_ttfcp:stagex_ttfcp -m 125 --X-allow-no-background  -v 7 

combine -M MultiDimFit workspace_ttbar.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=900 --alignEdges=1  -P kappa -P kappa_t --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_expect -v 3 -m 125 --setParameterRanges kappa=0.0,2.0:kappa_t=-2.0,2.0
combine -M MultiDimFit workspace_ttbar.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=40 --alignEdges=1 -P kappa --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_kappa -v 3 -m 125 --setParameterRanges kappa=0.0,2.0:kappa_t=0.0,0.0
combine -M MultiDimFit workspace_ttbar.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=40 --alignEdges=1 -P kappa_t --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_tilde -v 3 -m 125 --setParameterRanges kappa=1.0,1.0:kappa_t=-2.0,2.0

combine -M MultiDimFit workspace_ttbar_fcp.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=40 --alignEdges=1 -P x --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_fcp -v 3 -m 125 --setParameterRanges x=0.0,1.0
combine -M MultiDimFit workspace_ttbar_fcpng.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=40 --alignEdges=1 -P x --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_fcpng -v 3 -m 125 --setParameterRanges x=0.0,1.0

#  -S [ --systematics ] arg (=1) Include constrained systematic uncertainties, 
#-S 0 will ignore systematics constraint terms in the datacard.
#   -t [ --toys ] arg (=0)  Number of Toy MC extractions
# --algo arg (=none)   Algorithm to compute uncertainties
#--grid arg  Use the specified file containing a grid of SamplingDistributions for the limit
# --X-rtd arg Define some constants to be used at runtime (for debugging purposes). 
#   -v [ --verbose ] arg (=0) Verbosity level (-1 = very quiet; 0 =quiet, 1 = verbose, 2+ = debug) 
#  --alignEdges arg (=0) Align the grid points such that the endpoints of the ranges are included