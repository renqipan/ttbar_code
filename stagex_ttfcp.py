from HiggsAnalysis.CombinedLimit.PhysicsModel import *
import fnmatch 
from math import sqrt

class stagex_ttfcp(PhysicsModel):
    "Allow different signal strength fits for the stage-x model"
    def __init__(self):
	self.POIs = ""
	self.acttbar=False 
	self.acttbar_ng=False
	self.acggh=False 
	self.accor=False 
	self.options= ""
	self.stage0= False 
	self.rvrf= False 
	self.singlemu= False 
	self.splitHad= False 
	self.muNames =[]
	self.pois = []
	self.count=0
    def setModelBuilder(self, modelBuilder):
	PhysicsModel.setModelBuilder(self, modelBuilder)
	self.modelBuilder.doModelBOnly = False
    def getYieldScale(self,bin,process):
        if "top" in process: # return 1 for single top
		    return 1
        else:
        	print "ttbar process"
		muname=""
		if self.stage0:
			if "ttbar" in process:
				if self.acttbar or self.acttbar_ng:
					if "Aterm" in process:
						muname="r_tt_times_Aterm"
					elif "Bterm" in process:
						muname="r_tt_times_Bterm"
					elif "Cterm" in process:
						muname="r_tt_times_Cterm"
					elif "Dterm" in process:
						muname="r_tt_times_Dterm"
					elif "Eterm" in process:
						muname="r_tt_times_Eterm"
					elif "Fterm" in process:
						muname="r_tt_times_Fterm"
				else:
					muname="r_tt"
				print "muname: %s" %muname
	
        if self.acttbar:
			self.modelBuilder.doVar("mu[1,0,8]" )
			self.modelBuilder.doVar("x[0,0,1]" )
			
			self.modelBuilder.factory_("expr::r_tt_times_Aterm(\"-(1-@0)*@1+2*sqrt((1-@0)*@1)\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Bterm(\"1+0.5*(1-@0)*@1-1.5*sqrt((1-@0)*@1)+0.5*@0*@1\
-1.5*sqrt(@0*@1)+0.25*sqrt(@0*(1-@0))*@1\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Cterm(\"0.5*(1-@0)*@1-0.5*sqrt((1-@0)*@1)-0.25*sqrt((1-@0)*@0)*@1\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Dterm(\"-@0*@1+2*sqrt(@0*@1)\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Eterm(\"0.5*@0*@1-0.5*sqrt(@0*@1)-0.25*sqrt(@0*(1-@0))*@1\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Fterm(\"0.25*sqrt(@0*(1-@0))*@1\", x,mu)")
			
			self.pois.append("x,mu")
			self.POIs=",".join(self.pois)
			self.modelBuilder.doSet("POI",self.POIs)
			print "parameters of interest in ttbar analysis: ", self.POIs

        if self.acttbar_ng:
			self.modelBuilder.doVar("mu[1,0,8]" )
			self.modelBuilder.doVar("x[0,0,1]" )
			
			self.modelBuilder.factory_("expr::r_tt_times_Aterm(\"-(1-@0)*@1+2*sqrt((1-@0)*@1)\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Bterm(\"1+0.5*(1-@0)*@1-1.5*sqrt((1-@0)*@1)+0.5*@0*@1\
+1.5*sqrt(@0*@1)-0.25*sqrt(@0*(1-@0))*@1\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Cterm(\"0.5*(1-@0)*@1-0.5*sqrt((1-@0)*@1)+0.25*sqrt((1-@0)*@0)*@1\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Dterm(\"-@0*@1-2*sqrt(@0*@1)\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Eterm(\"0.5*@0*@1+0.5*sqrt(@0*@1)+0.25*sqrt(@0*(1-@0))*@1\", x,mu)")
			self.modelBuilder.factory_("expr::r_tt_times_Fterm(\"-0.25*sqrt(@0*(1-@0))*@1\", x,mu)")
			
			self.pois.append("x,mu")
			self.POIs=",".join(self.pois)
			self.modelBuilder.doSet("POI",self.POIs)
			print "parameters of interest in ttbar analysis: ", self.POIs


        	
        if self.modelBuilder.out.var(muname):
			print "reclying %s" %muname
        else:                              
			if not "times" in muname:
				self.modelBuilder.doVar("%s[1,0,10]" % muname)
				print "scale process %s with %s" %(process,muname)
				self.pois.append(muname)
				self.POIs=",".join(self.pois)
				self.modelBuilder.doSet("POI",self.POIs)
				print "Default parameters of interest: ", self.POIs
				
        return muname 
    def setPhysicsOptions(self,physOptions):
	    for po in physOptions:
		    if 'doStage0' in po: 
			    self.stage0= True
			    print "doing stage0"
		    if 'doacttbar' in po: 
			    self.acttbar= True
			    print "doing tth AC ttbar"
            if 'doacttbar_ng' in po:
				self.acttbar_ng=True
				print "doing ttbar kappa_tilde negative"
            if 'doacggh' in po: 
			    self.acggh= True
			    print "doing tth AC ggH"
            if 'singlemu' in po: 
			    self.singlemu= True
			    print "doing single mu"

    def doParametersOfInterest(self):
	    self.POIs=",".join(self.pois)
	    print "the parameters of interest: ", self.POIs
	    self.modelBuilder.doSet("POI",self.POIs)

stagex_ttfcp = stagex_ttfcp()
