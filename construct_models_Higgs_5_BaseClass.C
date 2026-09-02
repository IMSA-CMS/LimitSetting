#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <typeinfo>

#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooProduct.h"
#include "RooDataHist.h"
#include "TTree.h"

#include "RooAbsReal.h"
#include "RooRealSumPdf.h"
#include "RooAddPdf.h"
#include "RooRealConstant.h"
#include "RooConstVar.h"
#include "RooClassFactory.h"
#include "RooPDF_HiggsAnalysis_Base.h"
#include "RooPDF_HiggsAnalysis_DSCB.h"
#include "RooPDF_HiggsAnalysis_BKG.h"
#include "RooArgList.h"
#include "RooGenericPdf.h"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"


struct Process
{
	FitFunction function;
	RooAbsPdf* pdf = nullptr;
	RooAbsReal* norm = nullptr;
};

struct Channel
{
	std::string name;
	Process signal;
	std::vector<Process> backgrounds;
	Channel(std::string channelName) : name(channelName) {}

};

void construct_models_Higgs_5();

std::string replaceAll(std::string unmodifiedString, const std::string from, const std::string to);

TGraph makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);

// RooFormulaVar get_signal_norm(std::string channel_name, std::vector<std::string> parameterSet, RooRealVar& realHiggsMass);

std::string replaceAll(std::string unmodifiedString, const std::string from, const std::string to)
{
    if (from.empty())
        return unmodifiedString; // avoid infinite loop if 'from' is empty

    std::string str = unmodifiedString;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // move past the replaced part
    }
    
    return str;

}

// Creates the Signal Normalization Object for a channel - The Normalization object must vary with mass when it is added to workspace, which is why it is a FormulaVar and not just be the integral over the signal pdf object 
// RooFormulaVar get_signal_norm(std::string channel_name, std::vector<std::string> parameterSet, RooRealVar& realHiggsMass)
// {
// 	std::cout << "signal_norm_error0" << "\n";
// 	std::vector<std::vector<double>> parameters = getParameters(parameterSet);

// 	std::cout << "signal_norm_error1" << "\n";

// 	std::string norm_string = std::to_string(parameters[6][0]) +  "* (@0 - " + std::to_string(parameters[6][1]) + ")^" + std::to_string(parameters[6][2]) + " + " + std::to_string(parameters[6][3]);
	
// 	std::cout << "signal_norm_error2" << "\n";

// 	// (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
// 	RooFormulaVar norm((channel_name + "_signal_norm").c_str(), (channel_name + "_signal_norm").c_str(), norm_string.c_str(), RooArgList(realHiggsMass)); 

// 	std::cout << "signal_norm_error3" << "\n";
// 	return norm;
// }


void construct_models_Higgs_5_BaseClass()
{
	gROOT->SetBatch(true); // I can't remember what this does exactly, but it needs to be here

	// Get the Signal Events from Monte Carlo
	// We will be fitting our model to these events
	// The X and Y refer to the sign (+ or -) of the leptons, which can be plotted on the X and Y axises

	TFile file("Usefulstuff/output900.root","READ");
	auto hist_X = file.Get<TTree>("Signal");
	// auto hist_Y = file.Get<TTree>("Signal");      <------ When We Get both data sets, uncomment this and fix mc_Y below

	RooRealVar mass("mass", "mass", 900, 50, 2000); // This is the invariant mass (energy) of the event and is the independent variable for the background and Signal PDFs

	// This converts the TTree to a RooDataSet correlated to / dependent on the mass RooRealVar.
	RooDataSet mc_X("Events900_X","Events900", RooArgSet(mass), RooFit::Import(*hist_X));
	RooDataSet mc_Y("Events900_Y","Events900", RooArgSet(mass), RooFit::Import(*hist_X)); // When we get both data sets, change to hist_Y

	// Define the RooRealVars we are fitting and Scanning over.
	// realHiggsMass represents mass of the Higgs we are looking for. The others are the branching ratio of the H++ decay channels.
	// Currently, we only scan over mass. Later, we will look for most likely Branching ratio values for a given mass through fits and other scans
	RooRealVar realHiggsMass("realHiggsMass", "realHiggsMass",1500, 200,2000);
	RooRealVar Bee("b_ee", "b_ee", 1, 0,1);
	RooRealVar Beu("b_eu", "b_eu",1, 0,1);
	//RooRealVar norm_Systematic("normalization_systematic", "normalization_systematic", 0, -5, 5);
	RooRealVar norm_Systematic("normalization_systematic", "normalization_systematic", 1, 0, 1);
	//RooRealVar shape_Systematic("shape_systematic", "shape_systeamtic", 1, 0, 1); 
	RooRealVar shape_Systematic("shape_systematic", "shape_systematic", 0, -5, 5);
	norm_Systematic.setConstant(true);
	shape_Systematic.setConstant(true);
	
	// For now, we don't need this variable to be able to change
	Bee.setConstant(true);
	Beu.setConstant(true);

	// ----------------------------------------------------------------------------------------------------------
	// The higgsworkspace is where all of the RooFit objects are stored and manipulated.
	// Each object used by the datacard must be imported
	// Some objects, like the realHiggsMass, are implicitly imported when the PDFs or other objects that depend on them are imported.
	// The original PDFs that we copied are not imported
	TFile f_out("higgsworkspace.root", "RECREATE");
	RooWorkspace w_sig("higgsworkspace","higgsworkspace");


	w_sig.import(mc_X);
	w_sig.import(mc_Y);


	//--------------------------------------------------------------------------------------------------
	// Building the Signal Models
	// Each signal decay channel is defined by a Double-Sided Crystal Ball Function using the above parameters.
	// RooPDF_DSCB_test is custom ; it was made using RooClassFactory::makePdf("RooPDF_DSCB_test"....)
	// We Currently have 6 Channels per sign, this may increase a lot later, so a new way of listing may be more efficient

	// Note on PDFs: RooAbsPdf is not explicitly normalized. To get normalized results, parameters to normalize over must be provided
	// For example, eeee_signal.getVal() will return the base value of the pdf (not normalized). eeee_signal.getVal(RooArgList(mass)) normalizes over the mass variable.	

	// Not entirely sure what this does, since I would get issues if i didn't run these files before this one anyway, but its here...

	


	// We will want to change how these lists are handled..
	std::vector<std::string> signs {"X", "Y"};
	std::vector<std::string> channelsToCheck = {"eeee", "uuuu"};

	// Get signal and background parameters from files - Note: Update file paths later
	std::string signalParamsFileName = "/uscms/home/kprasad/cmsReleaseArea/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/H++SignalParameterFunctions.txt";
	std::string backgroundParamsFileName = "/uscms/home/hchen2/analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/H++BackgroundFunctions930.txt";

	FitFunctionCollection signalCollection = FitFunctionCollection::loadFunctions(signalParamsFileName);
	FitFunctionCollection backgroundCollection = FitFunctionCollection::loadFunctions(backgroundParamsFileName);

	std::set<std::string> channelNames = signalCollection.findUniqueNames("Channel");

	std::vector<Channel> channels;
	for (const auto& channelName : channelNames)
	{
		channels.push_back(Channel(channelName));
	}

	TFile f_out("higgsworkspace.root", "RECREATE");
	RooWorkspace w_sig("higgsworkspace","higgsworkspace");

	w_sig.import(mc_X);
	w_sig.import(mc_Y);

	for (auto& channel : channels)
	{
		std::cout << "Processing " << channel.name << " channels\n";

		for (const auto& X_or_Y : signs)
		{
			const std::string fullChannelName = channel.name + "_" + X_or_Y;
			std::cout << "Processing " << fullChannelName << "\n";

			FitFunctionCollection signalFunctions = signalCollection.getFunctions("channel", fullChannelName).getFunctions("projection", X_or_Y);
			FitFunctionCollection backgroundFunctions = backgroundCollection.getFunctions("channel", fullChannelName).getFunctions("projection", X_or_Y);

			for (auto& [key, sig] : signalFunctions.getFunctions())
			{
				channel.signal.function = sig;
			}

			for (auto& [key, bg] : backgroundFunctions.getFunctions())
			{
				Process backgroundProcess;
				backgroundProcess.function = bg;
				channel.backgrounds.push_back(backgroundProcess);
			}


			std::vector<std::vector<double>> parameters = signalChannel.extractParameters();
			auto* signal_pdf = new RooPDF_HiggsAnalysis_DSCB((channel.name + "_signal_" + X_or_Y).c_str(), (channel.name + "_signal").c_str(), mass, realHiggsMass, Bee, Beu, norm_Systematic, shape_Systematic, parameters, false); 

			auto signal_norm = signal_pdf->signal_norm(fullChannelName + "_signal");

			// Import signal
			std::cout << "Importing Signal PDF " << channel.signal.pdf->GetName() << "\n";
			w_sig.import(*signal_pdf);
			std::cout << "Importing Signal Normalization " << channel.signal.norm->GetName() << "\n";
			w_sig.import(*signal_norm);

			for (auto& backgroundProcess : channel.backgrounds)
			{

			std::vector<std::vector<double>> bkg_types_params = backgroundChannel.extractParameters();

			auto* bkg_pdf = new RooPDF_HiggsAnalysis_BKG(
				(channel.name + "_bkg_" + X_or_Y).c_str(),
				(channel.name + "_bkg").c_str(),
				mass, 
				realHiggsMass, 
				Bee, 
				Beu, 
				norm_Systematic, 
				shape_Systematic,
				bkg_types_params,
				channel.name
			);
					
				RooRealVar bkg_norm((fullChannelName + "_bkg_norm").c_str(), (fullChannelName + "_bkg_norm").c_str(),
					bkg_pdf->getNorm(mass));
				bkg_norm.setConstant(true);
				backgroundProcess.norm = &bkg_norm;

				// Import background
				std::cout << "Importing Background PDF " << bkg_pdf_for_norm->GetName() << "\n";
				w_sig.import(*bkg_pdf);
				std::cout << "Importing Background Normalization " << bkg_norm.GetName() << "\n";
				w_sig.import(bkg_norm);
			}
		}
	}

	w_sig.Print("v");
	std::cout << "\n";
	std::cout << "Writing to workspace\n";
	w_sig.Write();
	f_out.Close();
}



// make a graph using vectors since ROOT needs arrays
TGraph makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords)
{
	//Transfer the vector input to an array (used in TGraph)
	int arraySizeX = xCoords.size();
	int arraySizeY = yCoords.size();
	double xArray[arraySizeX], yArray[arraySizeY];
	for (int i=0; i < arraySizeX; i++)
	{
		xArray[i] = xCoords[i];
	}

	for (int i = 0; i < arraySizeY; i++)
	{
		yArray[i] = yCoords[i];
	}

	TGraph graph(numCoords, xArray, yArray);
	return graph;
}