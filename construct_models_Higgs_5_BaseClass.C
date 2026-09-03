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


#include "FitFunctionPDF.h"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"



struct Channel
{
	std::string name;

	Process signal;
	std::vector<Process> backgrounds;
	Channel(std::string channelName) : name(channelName) {}

};

void construct_models_Higgs_5();

std::vector<double> split(const std::string& line);
std::string replaceAll(std::string unmodifiedString, const std::string from, const std::string to);

std::vector<std::string> splitLine(const std::string& str);
std::vector<std::vector<double>> getParameters(std::vector<std::string> channelParameters);
TGraph makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);



void makeCombinedDatacard(std::string filename, std::vector<Channel> channels)
{
    std::vector<std::string> variants = {"X", "Y"};
    std::ofstream out(filename + ".txt");
    int nBins = channels.size() * variants.size();
    // header
    out << "imax " << nBins << "\n";
    out << "jmax " << (2 * nBins - 1) << "\n";
    out << "kmax *\n";
    out << "---------------------------------------------\n";
    // shapes block
    for (size_t i = 0; i < channels.size(); i++) {
        for (size_t j = 0; j < variants.size(); j++) {
            std::string ch = channels[i].name;
            std::string var = variants[j];
            std::string bin = "ch" + std::to_string(i+1) + "_ch" + std::to_string(j+1);
            out << "shapes " << ch << "_" << var << "  " << bin
                << "  higgsworkspace.root  higgsworkspace:" << ch << "_signal_" << var << "\n";
            out << "shapes bkg_" << ch << "_" << var << "  " << bin
                << "  higgsworkspace.root  higgsworkspace:" << ch << "_bkg_" << var << "\n";
            out << "shapes data_obs  " << bin
                << "  higgsworkspace.root  higgsworkspace:Events900_" << var << "\n";
        }
    }
    out << "---------------------------------------------\n";
    // bin / observation
    out << "bin  ";
    for (size_t i = 0; i < channels.size(); i++)
        for (size_t j = 0; j < variants.size(); j++)
            out << "ch" << i+1 << "_ch" << j+1 << "  ";
    out << "\nobservation  ";
    for (int i = 0; i < nBins; i++) out << "-1  ";
    out << "\n---------------------------------------------\n";
    // process rows
    out << "bin  ";
    for (size_t i = 0; i < channels.size(); i++)
        for (size_t j = 0; j < variants.size(); j++) {
            std::string bin = "ch" + std::to_string(i+1) + "_ch" + std::to_string(j+1);
            out << bin << "  " << bin << "  ";
        }
    out << "\nprocess  ";
    for (auto& ch : channels)
        for (auto& var : variants)
            out << ch.name << "_" << var << "  bkg_" << ch.name << "_" << var << "  ";
    out << "\nprocess  ";
    for (int i = 0; i < nBins; i++)
        out << "0  1  ";
    out << "\nrate  ";
    for (int i = 0; i < 2 * nBins; i++) out << "1  ";
    out << "\n---------------------------------------------\n";
}

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
// 	std::map<std::string, std::vector<std::vector<double>>> parameters;
 
// 	for (const auto [channel, parameters] : sortedFunctions)
// 	{
// 		// Record channels
// 		if (std::find(channelNames->begin(), channelNames->end(), channel) == channelNames->end())
// 		{
// 			channelNames->push_back(channel);
// 		}
		
// 		// Retrieve parameters from a channel
// 		for (auto fitFunctionParameter : parameters)
// 		{
// 			auto parameter = fitFunctionParameter.getFunction();
// 			std::vector<double> parameterValues;
// 			for (int i = 0; i < parameter->GetNpar(); ++i)
// 			{
// 				parameterValues.push_back(parameter->GetParameter(i));
// 			}

// 			for (int i = 0; i < parameter->GetNpar(); ++i)
// 			{
// 				parameterValues.push_back(parameter->GetParError(i));
// 			}

// 			parameters[channel].push_back(parameterValues);
// 		}

// 	}
// 	return parameters;
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

	// std::vector<std::string> channel_names {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu"};
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


			// std::vector<std::vector<double>> parameters = signalChannel.extractParameters();
			auto* signal_pdf = new FitFunctionPDF((channel.name + "_signal_" + X_or_Y).c_str(), (channel.name + "_signal").c_str(), mass, realHiggsMass, Bee, Beu, norm_Systematic, shape_Systematic, channel.signal.function); //is this right???

			auto signal_norm = signal_pdf->signal_norm(fullChannelName + "_signal");

			// Import signal
			std::cout << "Importing Signal PDF " << channel.signal.pdf->GetName() << "\n";
			w_sig.import(*signal_pdf);
			std::cout << "Importing Signal Normalization " << channel.signal.norm->GetName() << "\n";
			w_sig.import(*signal_norm);

			for (auto& backgroundProcess : channel.backgrounds)
			{

			// std::vector<std::vector<double>> bkg_types_params = backgroundChannel.extractParameters();
			auto* bkg_pdf = new FitFunctionPDF(
				(channel.name + "_bkg_" + X_or_Y).c_str(), (channel.name + "_bkg").c_str(), mass, realHiggsMass, Bee, Beu, norm_Systematic, shape_Systematic, backgroundProcess.function); //search for the right bkg fitfunction, should be in this file, use my searching function to find which one??
					
				RooRealVar bkg_norm((fullChannelName + "_bkg_norm").c_str(), (fullChannelName + "_bkg_norm").c_str(),
					bkg_pdf->getNorm(mass));
				bkg_norm.setConstant(true);
				backgroundProcess.norm = &bkg_norm;

				// Import background
				std::cout << "Importing Background PDF " << bkg.pdf->GetName() << "\n";
				w_sig.import(*bkg_pdf);
				std::cout << "Importing Background Normalization " << bkg_norm.GetName() << "\n";
				w_sig.import(bkg_norm);
			}
		}
	}

	w_sig.Print("v");
	std::cout << "\n";
	std::cout << "Writing to workspace \n";
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