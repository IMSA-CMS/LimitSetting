# NEED TO FIX INDENTATION!!!!!!
# #/usr/bin/env python
import os
import sys
sys.path.append('cfgs/')
from copy import deepcopy
import numpy
import math
import ROOT
from ROOT import TCanvas,TGraphAsymmErrors,TFile,TH1D,TH1F,TGraph,TGraphErrors,gStyle,TLegend,TLine,TGraphSmooth,TPaveText,TGraphAsymmErrors,TPaveLabel,gROOT
printStats = 1
ROOT.gROOT.SetBatch(True)

colors = {"ssm":ROOT.kGreen+3,"psi":ROOT.kBlue,"N":ROOT.kRed+2,"S":ROOT.kOrange,"I":ROOT.kMagenta+3,"kai":ROOT.kGreen+3,"eta":ROOT.kBlack,"RS_kMpl01":ROOT.kOrange+3,"RS_kMpl005":ROOT.kRed,"RS_kMpl001":ROOT.kViolet,"RS_kMpl0001":ROOT.kRed}
#labels = {"ssm":"Z'_{SSM}","psi":"Z'_{#psi}","S":"Z'_{S}","I":"Z'_{I}","N":"Z'_{N}","eta":"Z'_{#eta}","kai":"Z'_{#chi}","RS_kMpl01":"G_{KK} k/#bar{M}_{pl} = 0.1 (LO x 1.6)" ,"RS_kMpl005":"G_{KK} k/#bar{M}_{pl} = 0.05 (LO x 1.6)" ,"RS_kMpl001":"G_{KK} k/#bar{M}_{pl} = 0.01 (LO x 1.6)" ,"RS_kMpl0001":"G_{KK} k/#bar{M}_{pl} = 0.001 (LO x 1.6)" }
labels = {"ssm":"Z'_{SSM}","psi":"Z'_{#psi}","S":"Z'_{S}","I":"Z'_{I}","N":"Z'_{N}","eta":"Z'_{#eta}","kai":"Z'_{#chi}","RS_kMpl01":"k/#bar{M}_{pl} = 0.1" ,"RS_kMpl005":"k/#bar{M}_{pl} = 0.05" ,"RS_kMpl001":"k/#bar{M}_{pl} = 0.01" ,"RS_kMpl0001":"k/#bar{M}_{pl} = 0.001" }
kFacs  = {"ssm":1.0,"psi":1.0,"eta":1.0,"S":1.0,"N":1.0,"I":1.0,"kai":1.0,"RS_kMpl01":1.6,"RS_kMpl005":1.6,"RS_kMpl001":1.6,"RS_kMpl0001":1.6}

def printPlots(canvas,name):
	canvas.Print('plots/'+name+".png","png")
	canvas.Print('plots/'+name+".pdf","pdf")
	canvas.SaveSource('plots/'+name+".C","cxx")
	canvas.Print('plots/'+name+".root","root")
	canvas.Print('plots/'+name+".eps","eps")


def getXSecCurve(name,kFac,massDependent=False):
	smoother=TGraphSmooth("normal")
	X=[]
	Y=[]
	# file=open('xsec_%s.txt'%name,'r')
	# for entries in file:
	# 	entry=entries.split()
	# 	X.append(float(entry[0]))
	# 	Y.append(float(entry[1]))#1928*Kfac
	# print("Haha funny start here")

	file = open("cross_sections_HiggsAnalysis.txt")
	skip = True
	for line in file:
		if skip:
			skip = False
		else:
			entries = line.split()
			X.append(float(entries[0]))
			Y.append(float(entries[1]))

	#print(X)
	#print(Y)
	aX=numpy.array(X)
	aY=numpy.array(Y)
	#print(aY)

	Graph=TGraph(len(X),aX,aY)
	GraphSmooth= Graph

	GraphSmooth.SetLineWidth(3)
	if GUT:
		GraphSmooth.SetLineWidth(2)
		GraphSmooth.SetLineColor(colors[name])

	if SPIN2:
		Graph.SetLineColor(colors[name])
		Graph.SetLineWidth(3)
		return deepcopy(Graph)
	else:
		if massDependent:
			GraphSmooth.SetLineStyle(ROOT.kDashed)	
		return deepcopy(GraphSmooth)



def makeLimitPlot(output,exp,printStats=False,ratioLabel=""):#chan, -4th karedit  obs, -2nd    obs2="", -5th
	#fileForHEPData = TFile("plots/"+output+"_forHEPData.root","RECREATE")
	#KarEdit    	fileObs=open(obs,'r')
	#fileExp=open(exp,'r')

	observedx=[]
	observedy=[]
	obsLimits={}

	# crossSections = {}
	# file=open('xsec_ssm.txt','r')
	# #file=open('tools/xsec_%s.txt'%name,'r')
	# for entries in file:
	# 	entry=entries.split()
	# 	crossSections[float(entry[0])] = float(entry[1])

	# print("Type 1 for Higgs, type 2 for lepton jets")
	# WhichStudy = input()
	# DatacardResultsFile = open("RawResultFile.txt", "r")
	# DatacardResults = DatacardResultsFile.read()
	# ResultArray = eval(DatacardResults)
	# print(ResultArray)
	# limits = {}
	# if WhichStudy == 1:
	# 	allMasses = [300,500,700,900,1100,1300,1500,1700]
	# if WhichStudy ==2:
	# 	allMasses = [5000,9000,25000]

	# if WhichStudy == 1:
	# 	for i in range (0, len(ResultArray)):
	# 		Cross = crossSections[allMasses[i]]
	# 		limits[allMasses[i]] = [float(ResultArray[i][2][0])*Cross,float(ResultArray[i][1][0])*Cross,float(ResultArray[i][0])*Cross,float(ResultArray[i][1][1])*Cross,float(ResultArray[i][2][1])*Cross]


	# else:    # Lepton Jets but goes to infinity on the top part
	# 	for i in range (0, len(ResultArray)):
	# 	#Cross = crossSections[allMasses[i]]
	# 		limits[allMasses[i]] = [float(ResultArray[i][2][0]),float(ResultArray[i][1][0]),float(ResultArray[i][0]),999999, 9999999]#float(ResultArray[i][1][1]),float(ResultArray[i][2][1])]
	
	
	
	file = open("realHiggsMass,medianLimit_fromCombineCommands.txt")
	allMasses = []
	median_limits =[]
	_1Sigma_lower = []
	_1Sigma_upper = []
	_2Sigma_lower = []
	_2Sigma_upper = []
	skip = True
	for line in file:
		if skip:
			skip = False
		else:
			entries = line.split()
			if 'None' in entries:
				continue
			allMasses.append(float(entries[0]))
			median_limits.append(float(entries[1]))
			_1Sigma_lower.append(float(entries[2]))
			_1Sigma_upper.append(float(entries[3]))
			_2Sigma_lower.append(float(entries[4]))
			_2Sigma_upper.append(float(entries[5]))
			if len(entries) >= 6:
				try:
					allMasses.append(float(entries[0]))
					median_limits.append(float(entries[1]))
					_1Sigma_lower.append(float(entries[2]))
					_1Sigma_upper.append(float(entries[3]))
					_2Sigma_lower.append(float(entries[4]))
					_2Sigma_upper.append(float(entries[5]))
				except ValueError as e:
					print(f"Error converting values to float in line: {line.strip()}")
					print(f"Error: {e}")
					continue

	
	file = open("cross_sections_HiggsAnalysis.txt")
	cross_sections = {}
	skip = True
	for line in file:
		if skip:
			skip = False
		else:
			entries = line.split()
			cross_sections[float(entries[0])] = float(entries[1])
			# cross_sections[float(entries[0])] = 1


	limits = {}
	for i in range(0,len(allMasses)):
		cross = cross_sections[allMasses[i]]
		#limits[allMasses[i]] = [float(median_limits[i]) * cross,float(_1Sigma_lower[i]) * cross,float(_1Sigma_upper[i]) * cross,float(_2Sigma_lower[i]) * cross,float(_2Sigma_upper[i]) * cross]
		limits[allMasses[i]] = [float(median_limits[i]),float(_1Sigma_lower[i]),float(_1Sigma_upper[i]),float(_2Sigma_lower[i]),float(_2Sigma_upper[i])]
		for j in range(len(limits[allMasses[i]])):
			limits[allMasses[i]][j] /= (137000 * 0.4)




#Use SetPoint to add points but include the length.

	expectedx=[]
	expectedy=[]
	expected1SigLow=[]
	expected1SigHigh=[]
	expected2SigLow=[]
	expected2SigHigh=[]
   
	
	medianNr = 2
	lower2Sig = 0
	lower1Sig = 1
	upper1Sig = 3
	upper2Sig = 4
	
	# for i in range(0,len(allMasses)):
	# 	expectedy.append(median_limits[i])
	# 	expected1SigLow.append(_1Sigma_lower[i])
	# 	expected1SigHigh.append(_1Sigma_upper[i])
	# 	expected2SigLow.append(_2Sigma_lower[i])
	# 	expected2SigHigh.append(_2Sigma_upper[i])
	# 	expectedx.append(allMasses[i])
	for massPoint in sorted(allMasses):
		expectedy.append(limits[massPoint][0])
		expected1SigLow.append(limits[massPoint][1])
		expected1SigHigh.append(limits[massPoint][2])
		expected2SigLow.append(limits[massPoint][3])
		expected2SigHigh.append(limits[massPoint][4])
		expectedx.append(massPoint)

	print("------------------ Expected X")
	print(expectedx)
	print("-------------- Expected Y")
	print(expectedy)
	print("----------------------")
	expX=numpy.array(expectedx)
	expY=numpy.array(expectedy)

	values2=[]
	xPointsForValues2=[]
	values=[]
	xPointsForValues=[]
	xPointsForErrors=[]

	for x in range (0,len(expectedx)):
		values2.append(expected2SigLow[x])
		xPointsForValues2.append(expectedx[x])
		xPointsForErrors.append(100)#karedit
	for x in range (len(expectedx)-1,0-1,-1):
		values2.append(expected2SigHigh[x])
		xPointsForValues2.append(expectedx[x])
	if printStats: print ("length of values2: ", len(values2))

	for x in range (0,len(expectedx)):
		values.append(expected1SigLow[x])
		xPointsForValues.append(expectedx[x])
	for x in range (len(expectedx)-1,0-1,-1):
		values.append(expected1SigHigh[x])
		xPointsForValues.append(expectedx[x])
	if printStats: print ("length of values: ", len(values))
	exp2Sig=numpy.array(values2)
	xPoints2=numpy.array(xPointsForValues2)
	exp1Sig=numpy.array(values)
	xPoints=numpy.array(xPointsForValues)
	xPointsErrors=numpy.array(xPointsForErrors)



	#GraphErr2SigForHEPData=TGraphAsymmErrors(len(expX),expX,expY,numpy.array(xPointsErrors),numpy.array(xPointsErrors),numpy.array(expected2SigLow),numpy.array(expected2SigHigh))

	#GraphErr1SigForHEPData=TGraphAsymmErrors(len(expX),expX,expY,numpy.array(xPointsErrors),numpy.array(xPointsErrors),numpy.array(expected1SigLow),numpy.array(expected1SigHigh))
	
	GraphErr2Sig=TGraph(len(xPoints))
	for i in range(len(xPoints)):
		GraphErr2Sig.SetPoint(i,float(xPoints2[i]),float(exp2Sig[i]))
	

	GraphErr2Sig.SetFillColor(ROOT.kOrange)
	GraphErr1Sig=TGraphAsymmErrors(len(xPoints))
	for i in range(len(xPoints)):
		GraphErr1Sig.SetPoint(i,float(xPoints[i]),float(exp1Sig[i]))

	GraphErr1Sig.SetFillColor(ROOT.kGreen+1)
	cCL=TCanvas("cCL", "cCL",0,0,600,450)
	gStyle.SetOptStat(0)
	gStyle.SetPadRightMargin(0.063)
	gStyle.SetPadLeftMargin(0.14)
	gStyle.SetPadBottomMargin(0.12)
	
	plotPad = ROOT.TPad("plotPad","plotPad",0,0,1,1)
	plotPad.Draw()	
	plotPad.cd()

	
	expX=numpy.array(expectedx)
	expY=numpy.array(expectedy)

	GraphExp=TGraph(len(expX))
	for i in range(len(expX)):
		GraphExp.SetPoint(i,float(expX[i]),float(expY[i]))


	GraphExp.SetLineWidth(2)
	GraphExp.SetLineStyle(2)
	GraphExp.SetLineColor(ROOT.kBlack)
	obsX=numpy.array(observedx)
	obsY=numpy.array(observedy)
	if printStats: print ("obsX: ",obsX)
	if printStats: print ("obsY: ",obsY)
	
	if not "hi" == "":
		ratio = []
		ratiox = []
		for index,val in enumerate(observedy):
			mass = observedx[index]
			foundIndex = -1
			for index2, mass2 in enumerate(observedx2):
				if mass == mass2:
					foundIndex = index2
		
			if foundIndex > 0:
				ratio.append(observedy2[foundIndex]/val)
				ratiox.append(mass)
		ratioA = numpy.array(ratio)
		ratioX = numpy.array(ratiox)
    	
	if SPIN2:
		signals = ["RS_kMpl01","RS_kMpl005","RS_kMpl001"]	
	elif GUT:
		signals = ["ssm","psi","kai","eta","I","S","N"]
	else:
		signals = ["ssm"]#psi
	xSecCurves = []
	for signal in signals:
		xSecCurves.append(getXSecCurve(signal,1))# karEdit ask DrDong     kFacs[signal]


	plotPad.SetLogy()
	DummyGraph=TH1F("DummyGraph","",100,500,1500)
	DummyGraph.GetXaxis().SetTitle("Mass [GeV]")
	DummyGraph.GetYaxis().SetTitle("#sigma(pp#rightarrow H^{++}H^{--}) [pb]")

	# DummyGraph.SetMinimum(0)#5e-9
	DummyGraph.SetMaximum(1e-2)#1e-1
	# DummyGraph.SetMaximum(7)#1e-1

	# if SPIN2:
	# 	DummyGraph.GetYaxis().SetTitle("[#sigma#upoint#font[12]{B}] G_{KK} / [#sigma#upoint#font[12]{B}] Z")
	# else:
	# 	DummyGraph.GetYaxis().SetTitle("#sigma(pp#rightarrow H^{++}H^{--}) [pb]")#"[#sigma#upoint#font[12]{B}] H++ [pb]"
	gStyle.SetOptStat(0)

	DummyGraph.GetXaxis().SetRangeUser(0,25000)#DIMENSIONS
	DummyGraph.GetXaxis().SetLabelSize(0.055)
	DummyGraph.GetXaxis().SetTitleSize(0.055)
	DummyGraph.GetXaxis().SetTitleOffset(1.05)
	DummyGraph.GetYaxis().SetLabelSize(0.055)
	DummyGraph.GetYaxis().SetTitleSize(0.055)
	DummyGraph.GetYaxis().SetTitleOffset(1.3)
	DummyGraph.Draw()
	print("GraphErr1SigGetN",GraphErr1Sig.GetMean())

	GraphErr2Sig.Draw("Fsame")
	GraphErr1Sig.Draw("Fsame")
	GraphExp.Draw("lpsame")
    
	# if WhichStudy == 1: #Draw the cross section only if it is the Higgs Study
	for curve in xSecCurves:
		curve.SetLineColor(2)
		curve.Draw("lsame")
		


	plCMS=TPaveLabel(.16,1.0125,.27,.89125,"CMS","NBNDC")
	plCMS.SetTextAlign(12)
	plCMS.SetTextFont(62)
	plCMS.SetFillColor(0)
	plCMS.SetFillStyle(0)
	plCMS.SetBorderSize(0)

	plCMS.Draw()

	plPrelim=TPaveLabel(.3,0.915,.25,.98,"Preliminary","NBNDC")#.65,.905,.9,.99, corrdinates of 
	plPrelim.SetTextSize(0.6)
	plPrelim.SetTextAlign(12)
	plPrelim.SetTextFont(52)
	plPrelim.SetFillColor(0)
	plPrelim.SetFillStyle(0)
	plPrelim.SetBorderSize(0)
	plPrelim.Draw()


	cCL.SetTickx(1)
	cCL.SetTicky(1)
	cCL.RedrawAxis()
	cCL.Update()
    
 
	leg=TLegend(0.5,0.58,0.834885,0.878644,"","brNDC")   
	if SPIN2:
		leg=TLegend(0.5,0.58,0.834885,0.878644,"","brNDC")   
		leg.SetTextSize(0.0425)
	if not "hi" == "":
		if ratioLabel == "":
			ratioLabel = "Variant/Default"
		ratioLabels = ratioLabel.split("/")
		print (ratioLabels)	
	
    	
	else:
		if not EXPONLY:
			leg.AddEntry(GraphObs,"Obs. 95% CL limit","l")
			leg.AddEntry(GraphExp,"Exp. 95% CL limit, median","l")
			if (FULL):
				leg.AddEntry(GraphErr1Sig,"Exp. (68%)","f")
				leg.AddEntry(GraphErr2Sig,"Exp. (95%)","f")



	#Legend----------------
	leg.AddEntry(GraphExp,"Exp. 95% CL limit, median","l")#karedit
	leg.AddEntry(GraphErr1Sig,"Exp. (68%)","F")
	leg.AddEntry(GraphErr2Sig,"Exp. (95%)","F")
	leg.AddEntry(xSecCurves[0],"#sigma(pp#rightarrow H^{++}_{L}H^{--}_{L})")#"H^{++} NLO","l"


	leg1=TLegend(0.7,0.4,0.9,0.55,"","brNDC")
	leg1.SetTextSize(0.05)
	if GUT: 
		leg1=TLegend(0.6,0.35,0.75,0.623051,"","brNDC")
	if SPIN2:
		leg1=TLegend(0.7,0.35,0.9,0.58,"G_{KK} (LO x 1.6)","brNDC")
		leg1.SetTextSize(0.045)
      
	leg1.SetBorderSize(0)

	leg.SetLineWidth(0)
	leg.SetLineStyle(0)
	leg.SetFillStyle(0)
	leg.SetLineColor(0)
	leg.Draw("hist")

	leg1.SetLineWidth(0)
	leg1.SetLineStyle(0)
	leg1.SetFillStyle(0)
	leg1.SetLineColor(0)
	leg1.Draw("hist")

	   
	plLumi=TPaveLabel(.65,.905,.9,.99,"139 fb^{-1} (13 TeV)","NBNDC")
    	
	plLumi.SetTextSize(0.5)
	plLumi.SetTextFont(42)
	plLumi.SetFillColor(0)
	plLumi.SetBorderSize(0)
	plLumi.Draw()
    
	plotPad.SetTicks(1,1)
	plotPad.RedrawAxis()
		
	cCL.Update()
	printPlots(cCL,output)
    

#### ========= MAIN =======================
SMOOTH=False
FULL=False
SPIN2=False
KFAC=False
GUT=False
EXPONLY = False
TWOENERGY=False
if __name__ == "__main__":
	import argparse
	parser = argparse.ArgumentParser(usage="makeLimitPlot.py [options] -o OUTPUTFILE --obs CARD1 --exp CARD2",description="Check if all the ascii files have been produced with the right number of iterations",formatter_class=argparse.ArgumentDefaultsHelpFormatter)

	parser.add_argument("--obs",dest="obs", default='', help='Observed datacard')
	parser.add_argument("--obs2",dest="obs2", default='', help='2nd Observed datacard')
	parser.add_argument("--exp",dest="exp", default='', help='Expected datacard')
	parser.add_argument("--stats",dest="stats", action="store_true", default=False, help='Print stats')
	parser.add_argument("--smooth",dest="smooth",action="store_true",default=False, help="Smooth observed values")
	parser.add_argument("--full",dest="full",action="store_true",default=False, help="Draw 2sigma bands")
	parser.add_argument("--spin2",dest="spin2",action="store_true",default=False, help="Make Spin2 limits")
	parser.add_argument("--kFac",dest="kFac",action="store_true",default=False, help="use mass dependent k factor")
	parser.add_argument("--expOnly",dest="expOnly",action="store_true",default=False, help="plot only expected")
	parser.add_argument("--gut",dest="gut",action="store_true",default=False, help="Make GUT limits")
	parser.add_argument("-c","--config",dest="config",default='', help="config name")
	parser.add_argument("-t","--tag",dest="tag",default='', help="limit tag")
	parser.add_argument("--ratioLabel",dest="ratioLabel",default='', help="label for ratio")
	args = parser.parse_args()
	SMOOTH=args.smooth
	FULL=args.full
	SPIN2=args.spin2
	KFAC=args.kFac
	GUT = args.gut
	EXPONLY = args.expOnly

	
	outputfile = "outputlimitPlot_%s"%args.config
	if not args.tag == "":
		outputfile += "_"+args.tag        
	if GUT:
		outputfile += "_GUT"
	if SPIN2:
		outputfile += "_RS"
	if KFAC:
		outputfile += "_KFAC"
	
	exp = "LimitCardTest" #exp = "limitCard_%s_Exp"%args.config 
	if not args.tag == "":
		exp += "_" + args.tag
	exp += ".txt"
	if not args.exp == "":
		exp = args.exp

		print ("Saving histograms in %s" %(outputfile))
		print (" - Exp file: %s" %(exp))
		if (SMOOTH):
			print ("                  ")
			print ("Smoothing observed lines...")
		print ("\n")


	
	makeLimitPlot("outputFile",exp,args.stats,args.ratioLabel) # config.leptons,-4th  obs,-2nd ,args.obs2 -5th