from HiggsAnalysis.CombinedLimit.PhysicsModel import *
import fnmatch 

class new_stagex(PhysicsModel):
    "Allow different signal strength fits for the stage-x model"
    def __init__(self):
	self.POIs = ""
	self.actth=False 
	self.acggh=False 
	self.accor=False 
	self.useint=False 
	self.eft=False 
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
	    if not self.DC.isSignal[process]: 
		    return 1
	    else:
		muname=""
		if self.stage0 :
			if process.startswith("ggH"):
				if self.acggh: 
				    if "ALT" in process:
					  	if self.eft:
					  		muname= "c_ts"
						else:
					  		muname= "r_ggH_times_x"
				    elif "Mix" in process:
					  	if self.eft:
							if self.useint:
						  		muname= "c_int"
							else:
								return 0
						else:
							if self.useint:
					  			muname= "r_ggH_times_int"
							else:
								return 0
				    else:
					  	if self.eft:
					  		muname= "c_s"
						else:
					  		muname= "r_ggH_times_notx"
				else:
				    if not "ALT" in process:
				 		muname= "r_ggH"
				    else:
					    return 0
		  	elif "VHori" in process :
				muname= "r_VH"
		  	elif "VBFori" in process :
				muname= "r_VBF"
		  	elif process.startswith("VBF") :
				muname= "r_VBF"
			elif process.startswith("WH"): 
			  	muname= "r_VH"
			elif process.startswith("ZH"):
			  	muname= "r_VH"
			elif process.startswith("TTH"):
				if self.actth:
				    if "ALT" in process:
					  	if self.eft:
					  		muname= "kappa_ts"
						else:
					  		muname= "r_TTH_times_x"
				    else:
					  	if self.eft:
					  		muname= "kappa_s"
						else:
					  		muname= "r_TTH_times_notx"
				else:
				    if not "ALT" in process:
					muname = "r_TTH"
				    else:
					    return 0
			elif process.startswith("TH") or process.startswith("tqH"):
			  	muname= "r_TTH"
			elif process.startswith("BBH"):
			  	muname= "r_ggH"
			elif process.startswith("VH"):
			  	muname= "r_VH"
			if self.splitHad:	
				if ("VHori" in process):
				 	muname= "r_VH_Had"
			        elif (process.startswith("VH") and (not "VBFori" in process)):
			  	        muname= "r_VH_Had"
				if "Lep" in process:
				 	muname= "r_VH_Lep"
		elif self.singlemu:
			muname = "r"
		elif self.rvrf:
		  	if ("VH" in process or "VBF" in process):
				muname= "rv"
		  	elif ("ggH" in process or "BBH" in process or "TH" in process):
				muname= "rf"
		else:
			muname = "r_%s"%process
			if process.startswith("BBH"):
			  	muname= "r_ggH_0j_10_200"
			if process.startswith("TH"):
			  	muname= "r_TTH"
			muname = muname.replace("_VHori","") 
			muname = muname.replace("_VBFori","") 
		if self.actth or self.acggh:
			if self.eft:
				self.modelBuilder.doVar("kappa[1,0,2.0]" )
				self.modelBuilder.doVar("kappa_t[0,-2.0,2.0]" )
				self.modelBuilder.doVar("cgg[1,0,2]" )
				self.modelBuilder.doVar("cgg_t[0,-2,2]" )
				self.modelBuilder.factory_("expr::kappa_ts(\"@0*@0/2.56\", kappa_t)");
				self.modelBuilder.factory_("expr::kappa_s(\"@0*@0\", kappa)");
				if not self.accor:
					if self.useint:
						self.modelBuilder.factory_("expr::c_ts(\"@0*@0+@0*@1\", cgg_t,cgg)");
						self.modelBuilder.factory_("expr::c_s(\"@0*@0+@0*@1\", cgg,cgg_t)");
					else:
						self.modelBuilder.factory_("expr::c_ts(\"@0*@0\", cgg_t)");
						self.modelBuilder.factory_("expr::c_s(\"@0*@0\", cgg)");
					self.modelBuilder.factory_("expr::c_int(\"-2*@0*@1\", cgg, cgg_t)");##
				else:
					if self.useint:
						self.modelBuilder.factory_("expr::c_ts(\"@0*@0*2.25+1.5*@0*@1\", kappa_t,kappa)");
						self.modelBuilder.factory_("expr::c_s(\"@0*@0+1.5*@0*@1\", kappa,kappa_t)");
					else:
						self.modelBuilder.factory_("expr::c_ts(\"@0*@0*2.25\", kappa_t)");
						self.modelBuilder.factory_("expr::c_s(\"@0*@0\", kappa)");
					self.modelBuilder.factory_("expr::c_int(\"-1.5*2*@0*@1\", kappa, kappa_t)");
				if self.actth:
					self.pois.append("kappa,kappa_t")
				else:
					self.pois.append("cgg,cgg_t")

			else:
				self.modelBuilder.doVar("r_TTH[1,0,10.0]" )
				self.modelBuilder.doVar("r_ggH[1,0,10.0]" )
				self.modelBuilder.doVar("x[0,-1,1]" )
				self.modelBuilder.factory_("expr::r_TTH_times_x(\"@0*abs(@1)/2.56\", r_TTH, x)");
				self.modelBuilder.factory_("expr::r_TTH_times_notx(\"@0*(1-abs(@1))\", r_TTH, x)");
				if self.accor:
					if self.useint:
						self.modelBuilder.factory_("expr::r_ggH_times_x(\"2.25*@0*abs(@1)-(@1>0?-1.5:1.5)*@0*sqrt(1-abs(@1))*sqrt(abs(@1))\", r_TTH, x)");
						self.modelBuilder.factory_("expr::r_ggH_times_notx(\"@0*(1-abs(@1))-(@1>0?-1.5:1.5)*@0*sqrt(1-abs(@1))*sqrt(abs(@1))\", r_TTH, x)");
					else:
						self.modelBuilder.factory_("expr::r_ggH_times_x(\"2.25*@0*abs(@1)\", r_TTH, x)");
						self.modelBuilder.factory_("expr::r_ggH_times_notx(\"@0*(1-abs(@1)))\", r_TTH, x)");
					self.modelBuilder.factory_("expr::r_ggH_times_int(\"(@1>0?-1.5:1.5)*2*@0*sqrt(1-abs(@1))*sqrt(abs(@1))\", r_TTH, x)");
					self.pois.append("x,r_TTH")
				else:
					if self.useint:
						self.modelBuilder.factory_("expr::r_ggH_times_x(\"2.25*@0*abs(@1)-(@1>0?-1.5:1.5)*@0*sqrt(1-abs(@1))*sqrt(abs(@1))\", r_ggH, x)");
						self.modelBuilder.factory_("expr::r_ggH_times_notx(\"@0*(1-abs(@1))-(@1>0?-1.5:1.5)*@0*sqrt(1-abs(@1))*sqrt(abs(@1))\", r_ggH, x)");
					else:
						self.modelBuilder.factory_("expr::r_ggH_times_x(\"2.25*@0*abs(@1)\", r_ggH, x)");
						self.modelBuilder.factory_("expr::r_ggH_times_notx(\"@0*(1-abs(@1))\", r_ggH, x)");
					self.modelBuilder.factory_("expr::r_ggH_times_int(\"(@1>0?-1.5:1.5)*2*@0*sqrt(1-abs(@1))*sqrt(abs(@1))\", r_ggH, x)");
					self.pois.append("x,r_ggH,r_TTH")
		if self.modelBuilder.out.var(muname):
			print "reclying %s" %muname
		else:
			if muname.startswith("r_") and not "times" in muname:
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
		    if 'doactth' in po: 
			    self.actth= True
	                    print "doing tth AC ttH"
		    if 'doaccor' in po: 
			    self.accor= True
	                    print "doing tth ggh correlation"
		    if 'doacggh' in po: 
			    self.acggh= True
	                    print "doing tth AC ggH"
		    if 'useint' in po: 
			    self.useint= True
	                    print "use interference"
		    if 'doeft' in po: 
			    self.eft= True
	                    print "doing EFT framework"
		    if 'singlemu' in po: 
			    self.singlemu= True
	                    print "doing single mu"
		    if 'rvrf' in po: 
			    self.rvrf= True
	                    print "doing rvrf"
		    if 'splitHad' in po: 
			    self.splitHad= True
	                    print "Splitting had and lep VH"

    def doParametersOfInterest(self):
	    self.POIs=",".join(self.pois)
	    print "Default parameters of interest: ", self.POIs
	    self.modelBuilder.doSet("POI",self.POIs)

new_stagex = new_stagex()
