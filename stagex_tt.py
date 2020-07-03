from HiggsAnalysis.CombinedLimit.PhysicsModel import *
import fnmatch 

class stagex_tt(PhysicsModel):
    "Allow different signal strength fits for the stage-x model"
    def __init__(self):
	self.POIs = ""
	self.acttbar=False 
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
				if self.acttbar:
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
			self.modelBuilder.doVar("kappa[1,0,5]" )
			self.modelBuilder.doVar("kappa_t[0,-5,5]" )
			
			self.modelBuilder.factory_("expr::r_tt_times_Aterm(\"-@0*@0+2*@0\", kappa)")
			self.modelBuilder.factory_("expr::r_tt_times_Bterm(\"1+0.5*@0*@0-1.5*@0+0.5*@1*@1-1.5*@1+0.25*@0*@1\", kappa,kappa_t)")
			self.modelBuilder.factory_("expr::r_tt_times_Cterm(\"0.5*@0*@0-0.5*@0-0.25*@0*@1\", kappa,kappa_t)")
			self.modelBuilder.factory_("expr::r_tt_times_Dterm(\"-@0*@0+2*@0\", kappa_t)")
			self.modelBuilder.factory_("expr::r_tt_times_Eterm(\"0.5*@0*@0-0.5*@0-0.25*@0*@1\", kappa_t,kappa)")
			self.modelBuilder.factory_("expr::r_tt_times_Fterm(\"0.25*@0*@1\", kappa,kappa_t)")
			
			self.pois.append("kappa,kappa_t")
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

stagex_tt = stagex_tt()
