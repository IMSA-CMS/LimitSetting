#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <sstream>
#include <unordered_map>

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
#include "RooRealConstant.h"
#include "RooConstVar.h"
#include "RooClassFactory.h"
#include "RooPDF_DSCB_test.h"
#include "RooPDF_DBLGAUSS.h"
#include "RooAbsPdf.h"
#include "RooPDF_BKG.h"
#include "RooGenericPdf.h"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"

struct Channel
{
	std::string name;
	std::map<std::string, FitFunction> fitFunctions;
	Channel(std::string channelName, std::vector<FitFunction> channelFunctions);
	std::vector<std::vector<double>> extractParameters();

};

void construct_models_Higgs_6();

std::vector<double> split(const std::string& line);
std::vector<std::string> splitLine(const std::string& str);
std::vector<std::vector<double>> getParameters(std::vector<std::string> channelParameters);
TGraph makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);

RooPDF_BKG create_bkg_pdf(std::string channel_name, std::vector<std::string> parameterSet, RooRealVar& mass);
RooPDF_DSCB_test create_signal_pdf(std::string channel_name, std::vector<std::string> parameterSet, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2, RooRealVar& norm_Systematic, RooRealVar& shape_Systematic);
RooFormulaVar get_signal_norm(std::string channel_name, std::vector<std::string> parameterSet, RooRealVar& realHiggsMass);
std::map<std::string, std::vector<double>> getParameterValuesFromMap(std::string fileName, std::vector<string>* channelNames);
bool containsSubstring(std::string mainString, std::string subString);

std::map<std::string, std::vector<FitFunction>> getFunctionsSortedByChannel(std::string filename);

std::map<std::string, std::vector<FitFunction>> getFunctionsSortedByChannel(std::string fileName)
{
    std::map<std::string, std::vector<FitFunction>> sortedFunctions = {};
    std::vector<std::string> channelNames = {};

    std::map<std::string, std::string> channelNameModifiers = {};
    channelNameModifiers["mll1"] = "_X";
    channelNameModifiers["mll2"] = "_Y";


    std::vector<std::string> existingChannelNameModifiers = {};
    for (auto [modifier, replacement] : channelNameModifiers)
    {
        existingChannelNameModifiers.push_back(modifier);
    }


    FitFunctionCollection functionCollection;
    try {
        functionCollection = FitFunctionCollection::loadFunctions(fileName);
        std::cout << "Successfully read " << fileName << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load functions: " << e.what() << std::endl;
        throw;
    }


    // Sorts functions by channel
    auto parameterFunctions = functionCollection.getFunctions();
    for (auto [functionName, function] : parameterFunctions)
    {
        std::string channel = function.getChannel();
        //std::string channel = replaceAll(unformattedChannel, "m", "u");
        std::cout << "DEBUG: channel name for function: " << channel << "\n";
        // Check whether channel name needs to be modified for internal processes
        for (auto modifier : existingChannelNameModifiers)
        {
            if (function.getParameterName().find(modifier) != std::string::npos)
            {
                channel += channelNameModifiers[modifier];
            }
        }

        sortedFunctions[channel].push_back(function);

        // Record channels
        // if (std::find(channelNames.begin(), channelNames.end(), channel) == channelNames.end())
        // {
        //     channelNames.push_back(channel);
        // }
    }

    // Alphabetize functions for each channel
    // for (std::string channelName : channelNames)
    // {
    //     std::vector<FitFunction> alphabetizedFunctions = alphabetizeParameters(sortedFunctions[channelName]);
    //     sortedFunctions[channelName] = alphabetizedFunctions;
    // }

    return sortedFunctions;
}


Channel::Channel(std::string channelName, std::vector<FitFunction> channelFunctions)
{
	name = channelName;
	for (auto channelFunction : channelFunctions)
	{
		std::string functionName = channelFunction.getParameterName();
		fitFunctions[functionName] = channelFunction;
	}
}

std::vector<std::vector<double>> Channel::extractParameters()
{
	std::vector<std::vector<double>> extractedParameters = {};

	// Record names of functions and alphabetize them
	std::vector<std::string> functionNames = {};
	for (auto [functionName, function] : fitFunctions)
	{
		functionNames.push_back(functionName);
	}
	std::sort(functionNames.begin(), functionNames.end());

	// Goes through each function (in alphabetical order) and extracts their parameters 
	for (auto functionName : functionNames)
	{
		auto function = fitFunctions[functionName].getFunction();
		
		std::vector<double> functionParameters = {};

		for (int i = 0; i < function->GetNpar(); i++)
		{
			functionParameters.push_back(function->GetParameter(i));
		}
		for (int i = 0; i < function->GetNpar(); i++)
		{
			functionParameters.push_back(function->GetParError(i));
		}

		extractedParameters.push_back(functionParameters);
	}

	return extractedParameters;
}



// Splits parameter lines by spaces, skipping the first entry (which I used as a name and not a value I actually need)
std::vector<double> split(const std::string& line)
{
	std::istringstream iss(line); 
	std::vector<double> tokens; 
	std::string token; 
	bool skip = true;
	while (iss >> token) 
	{ 
		if (skip)
		{
			skip = false;
		}
		else
		{
			tokens.push_back(std::stod(token)); 
		}
	} 
	return tokens;
}

std::vector<std::string> splitLine(const std::string& line)
{
	std::istringstream iss(line); 
	std::vector<std::string> tokens; 
	std::string token; 
	while (iss >> token) 
	{ 
		tokens.push_back(token); 
		
	} 
	// int i = 0;
	// for (std::string token : tokens)
	// {
	// 	std::cout << i << '	' << token << '\n';
	// 	i++;
	// }
	return tokens;
}

// Helper function for finding substring 
bool containsSubstring(std::string mainString, std::string subString)
{
	if (mainString.find(subString) != std::string::npos)
	{
		return true;
    } 
	else 
	{
		return false;
    }

}

// Helper function for removing substrings
std::string removeSubstring(const std::string& original, const std::string& toRemove) 
{
    if (toRemove.empty()) return original;  // Nothing to remove

    std::string result = original;
    size_t pos;

    // Find and erase all occurrences
    while ((pos = result.find(toRemove)) != std::string::npos) {
        result.erase(pos, toRemove.length());
    }

    return result;
}


// Helper function for splitting strings
// std::vector<std::string> split(const std::string& str, char delimiter) {
//     std::vector<std::string> tokens;
//     std::stringstream ss(str);
//     std::string token;
    
//     while (std::getline(ss, token, delimiter)) {
//         tokens.push_back(token);
//     }
    
// 	int i = 0;
// 	for (std::string token : tokens)
// 	{
// 		std::cout << i << '	' << token << '\n';
// 		i++;
// 	}
//     return tokens;
// }

// Gets the signal/background parameters from lines of a file; skips the first few lines, which are used as explanation for organization
std::vector<std::vector<double>> getParameters(std::vector<std::string> channelParameters)
{
	std::vector<std::vector<double>> parameterArrays;

	// std::cout << channelParameters[0] << '\n';
	// std::cout << channelParameters[1] << '\n';
	// Starts at i = 3 to skip first few lines
	for (int i = 3; i < channelParameters.size(); i++)
	{
		//std::cout << channelParameters[i] << '\n';
		parameterArrays.push_back(split(channelParameters[i]));
	}
	return parameterArrays;
}

RooAbsPdf::Ref create_signal_pdf(
    const std::string &channel,
    Channel &signalChannel,
    RooAbsReal &mass,
    RooAbsReal &realHiggsMass,
    RooAbsReal &Bee,
    RooAbsReal &Beu,
    bool multiplyBy2,
    const std::string &pdfType  // "DSCB" or "DBLGauss"
) {
    const std::vector<std::vector<double>> &params = signalChannel.extractParameters();

    if (pdfType == "DSCB") {
        RooPDF_DSCB_test pdf((channel + "_signal").c_str(), (channel + "_signal").c_str(), mass, realHiggsMass, Bee, Beu, params, multiplyBy2);
        return pdf;  // Return by value
    } else if (pdfType == "DBLGauss") {
        RooPDF_DBLGAUSS pdf((channel + "_signal").c_str(), (channel + "_signal").c_str(), mass, realHiggsMass, Bee, Beu, params, multiplyBy2);
        return pdf;  // Return by value
    } else {
        throw std::runtime_error("Unknown PDF type in create_signal_pdf");
    } 
}

// Creates the Signal Normalization Object for a channel - The Normalization object must vary with mass when it is added to workspace, which is why it is a FormulaVar and not just be the integral over the signal pdf object 
RooFormulaVar get_signal_norm(std::string channel_name, std::vector<std::string> parameterSet, RooRealVar& realHiggsMass)
{
	std::vector<std::vector<double>> parameters = getParameters(parameterSet);

	//std::cout << "get_signal_norm: " << channel_name << '\n';
	// for (std::vector<double> parameterLine : parameters)
	// {
	// 	for (double parameterValue : parameterLine)
	// 	{
	// 		std::cout << parameterValue << ", ";
	// 	}
	// 	std::cout << '\n';
	// }

	std::string norm_string = std::to_string(parameters[6][0]) +  "* (@0 - " + std::to_string(parameters[6][1]) + ")^" + std::to_string(parameters[6][2]) + " + " + std::to_string(parameters[6][3]);
	
	// (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
	RooFormulaVar norm((channel_name + "_signal_norm").c_str(), (channel_name + "_signal_norm").c_str(), norm_string.c_str(), RooArgList(realHiggsMass)); 
	return norm;
}

// Creates a RooPDF_BKG Object 
RooPDF_BKG create_bkg_pdf(std::string channel_name, Channel backgroundChannel, RooRealVar& mass)
{
	std::vector<std::vector<double>> bkg_types_params = backgroundChannel.extractParameters();
	// (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
	RooPDF_BKG pdf((channel_name + "_bkg").c_str(), (channel_name + "_bkg").c_str(), mass, bkg_types_params, channel_name); 
	return pdf;
}

// std::map<std::string, std::vector<std::vector<double>>> getParametersFromMap(std::map<std::string, std::vector<FitFunction>> sortedFunctions, std::vector<string>* channelNames)
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



void construct_models_Higgs_6()
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
	RooDataSet mc_X("Events900_X","Events900", hist_X, RooArgSet(mass), "");
	RooDataSet mc_Y("Events900_Y","Events900", hist_X, RooArgSet(mass), ""); // When we get both data sets, change to hist_Y


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

	//--------------------------------------------------------------------------------------------------
	// Building the Signal Models
	// Each signal decay channel is defined by a Double-Sided Crystal Ball Function using the above parameters.
	// RooPDF_DSCB_test is custom ; it was made using RooClassFactory::makePdf("RooPDF_DSCB_test"....)
	// We Currently have 6 Channels per sign, this may increase a lot later, so a new way of listing may be more efficient

	// Note on PDFs: RooAbsPdf is not explicitly normalized. To get normalized results, parameters to normalize over must be provided
	// For example, eeee_signal.getVal() will return the base value of the pdf (not normalized). eeee_signal.getVal(RooArgList(mass)) normalizes over the mass variable.	

	// Not entirely sure what this does, since I would get issues if i didn't run these files before this one anyway, but its here...
	// gROOT -> ProcessLineSync(".x RooPDF_DSCB_test.cxx+");
	// gROOT -> ProcessLineSync(".x RooPDF_BKG.cxx+");

	// We will want to change how these lists are handled..

	// std::vector<std::string> channel_names {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu"};
	std::vector<std::string> channel_names;
	std::vector<std::string> signs {"X", "Y"};
	std::vector<std::string> channelsToCheck = {"eeee", "uuuu"};
	std::vector<std::string> reconstructedEventTypesToCheck = {"eeee", "uuuu"};


	// Get signal and background parameters from files - Note: Update file paths later
	std::string signalParamsFileName = "/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/H++SignalParameterFunctions.txt";
	std::string backgroundParamsFileName = "/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/OtherBackgroundFunctions.txt";
	std::map<std::string, std::vector<FitFunction>> signalParameters = FitFunctionCollection::getFunctionsSortedByChannel(signalParamsFileName);
	std::map<std::string, std::vector<FitFunction>> backgroundParameters = FitFunctionCollection::getFunctionsSortedByChannel(backgroundParamsFileName);  


	std::vector<Channel> signalChannels = {};
	std::vector<Channel> backgroundChannels = {};
	
	for (auto [channel, fitFunctions] : signalParameters)
	{
		Channel signalChannel(channel, fitFunctions);
		signalChannels.push_back(signalChannel);
	}

	for (auto [channel, fitFunctions] : backgroundParameters)
	{
		Channel backgroundChannel(channel, fitFunctions);
		backgroundChannels.push_back(backgroundChannel);
	}

	for (auto channel : signalChannels)
	{
		std::vector<std::vector<double>> parameters = channel.extractParameters();
		std::cout << "Channel: " << channel.name << "\n" << "Extracted Signal Parameters size: " << parameters.size() << "\n";
		for (auto doubleVector : parameters)
		{
			for (auto value : doubleVector)
			{
				std::cout << value << "\n";
			}
		}

		std::cout << "\n \n \n";
	}

	for (auto channel : backgroundChannels)
	{
		std::vector<std::vector<double>> parameters = channel.extractParameters();
		std::cout << "Channel: " << channel.name << "\n" << "Extracted Background Parameters size: " << parameters.size() << "\n";
		for (auto doubleVector : parameters)
		{
			for (auto value : doubleVector)
			{
				std::cout << value << "\n";
			}
		}

		std::cout << "\n \n \n";
	}


	//std::map<std::string, std::vector<std::vector<double>>> signalParams = getParameterValuesFromMap(signalParameters, &channel_names);
	//std::map<std::string, std::vector<std::vector<double>>> backgroundParams = getParameterValuesFromMap(backgroundParameters, &channel_names);

	RooArgList signal_pdfs = new RooArgList();
	// std::vector<std::vector<RooAbsPdf>> signal_pdfs;
	std::vector<std::vector<std::string>> signal_pdfsNames;

	RooArgList bkg_pdfs = new RooArgList();
	// std::vector<std::vector<RooPDF_BKG>> bkg_pdfs;
	std::vector<std::vector<std::string>> bkg_pdfsNames;

	RooArgList signal_norms = new RooArgList();
	// std::vector<std::vector<RooFormulaVar>> signal_Normalizations;
	std::vector<std::vector<std::string>> signal_NormalizationsNames;

	RooArgList signal_norms = new RooArgList(); 
	// std::vector<std::vector<RooRealVar>> bkg_Normalizations;
	std::vector<std::vector<std::string>> bkg_NormalizationsNames;

	std::set<std::string> reconstructed_events_types;
	for (auto entire_channel :channel_names) {
		reconstructed_events_types.insert(entire_channel.substr(0,4));  // first 4 letters
	}


	std::vector<std::string> completedChannels;

	std::vector<std::string> completedReconstructedEventTypes;

	for (auto reconstructed_event_type : reconstructed_events_types)
	{
		std::cout << "Now processing all channels with reconstructed event type: " << reconstructed_event_type;
		
		if (std::find(reconstructedEventTypesToCheck.begin(), reconstructedEventTypesToCheck.end(), reconstructed_event_type) == reconstructedEventTypesToCheck.end() || 
			std::find(completedReconstructedEventTypes.begin(), completedReconstructedEventTypes.end(), reconstructed_event_type) != completedReconstructedEventTypes.end())
		{
			std::cout << reconstructed_event_type << " has already been processed or is not a reconstructed event type to be processed" << "\n";
			continue;
		}

		std::vector<RooAbsPdf> signal_X_and_Y;
		std::vector<std::string> signal_X_and_YNames;

		std::vector<RooPDF_BKG> bkg_X_and_Y;
		std::vector<std::string> bkg_X_and_YNames;
		
		std::vector<RooFormulaVar> signal_X_and_Y_Normalizations;
		std::vector<std::string> signal_X_and_Y_NormalizationsNames;

		std::vector<RooRealVar> bkg_X_and_Y_Normalizations;
		std::vector<std::string> bkg_X_and_Y_NormalizationsNames;
		
		for(std::string X_or_Y : signs)
		{
			RooArgList signal_pdfs_for_reconstructed_event_type_X_or_Y = new RooArgList();
			// std::vector<RooAbsPdf> signal_pdfs_for_reconstructed_event_type_X_or_Y;
			std::vector<std::string> signal_pdfsNames_for_reconstructed_event_type_X_or_Y;

			RooArgList bkg_pdfs_for_reconstructed_event_type_X_or_Y = new RooArgList();
			// std::vector<RooPDF_BKG> bkg_pdfs_for_reconstructed_event_type_X_or_Y;
			std::vector<std::string> bkg_pdfsNames_for_reconstructed_event_type_X_or_Y;
			
			
			RooArgList signal_Normalizations_for_reconstructed_event_type_X_or_Y = newRooArgList();
			// std::vector<RooFormulaVar> signal_Normalizations_for_reconstructed_event_type_X_or_Y;
			std::vector<std::string> signal_NormalizationsNames_for_reconstructed_event_type_X_or_Y;

			RooArgList bkg_Normalizations_for_reconstructed_event_type_X_or_Y = newRooArgList();
			// std::vector<RooRealVar> bkg_Normalizations_for_reconstructed_event_type_X_or_Y;
			std::vector<std::string> bkg_NormalizationsNames_for_reconstructed_event_type_X_or_Y;

			for (std::string channelXOrY : channel_names)
			{
				// Remove any X or Y modifiers to the channel name
				std::string channel = channelXOrY;
				std::cout << "Current channel with modifiers: " << channelXOrY << "\n";
				for (auto sign : signs)
				{
					channel = removeSubstring(channel, "_" + sign);
				}
				std::cout << "Channel without modifiers: " << channel << "\n";

				if (channel.substr(0,4) != reconstructed_event_type)
				{
					std::cout << channel << " does not have the following reconstructed event type: " << reconstructed_event_type;
					continue;
				}

				// setup channels with signs to process
				std::string channelX_or_Y;
				channelX_or_Y = channel + "_" + X_or_Y;
				Channel* currentSignalChannel = nullptr;
				Channel* currentBackgroundChannel = nullptr;
				for (auto& channel : signalChannels)
				{
					std::cout << channel << std::endl;
					if (channel.name == channelX_or_Y)
					{
						currentSignalChannel = &channel;
					}
				}
				for (auto& channel : backgroundChannels)
				{
					if (channel.name == channelX_or_Y)
					{
						currentBackgroundChannel = &channel;
					}
				}

				// make PDF for a given channel and push back
				RooAbsPdf signal_pdf = create_signal_pdf(channelX_or_Y, *currentSignalChannel, mass, realHiggsMass, Bee, Beu, false, currentSignalChannel->pdfType);

				// Rename for workspace
				signal_pdf.SetName((channel + "_signal_" + X_or_Y).c_str());
				signal_pdf.SetTitle((channel + "_signal_" + X_or_Y).c_str());

				std::cout << "signal_pdf successfully created for " << channelX_or_Y << "\n";
				signal_pdfs_for_reconstructed_event_type_X_or_Y.add(signal_pdf);
				signal_pdfsNames_for_reconstructed_event_type_X_or_Y.push_back(channel + "_signal_" + X_or_Y);
				// RooFormulaVar signal_norm(get_signal_norm(channel, realHiggsMass), (channel+"_signal_" + X_or_Y + "_norm").c_str());
				// signal_X_and_Y_Normalizations.push_back(signal_norm);
				signal_Normalizations_for_reconstructed_event_type_X_or_Y.add(signal_pdf.signal_norm(channel + "_signal_" + X_or_Y));
				signal_NormalizationsNames_for_reconstructed_event_type_X_or_Y.push_back(channel + "_signal_" + X_or_Y);

				RooPDF_BKG bkg_pdf(create_bkg_pdf(channel, *currentBackgroundChannel, mass), (channel + "_bkg_" + X_or_Y).c_str());
				std::cout << "background_pdf successfully created for " << channelX_or_Y << "\n";
				bkg_pdfs_for_reconstructed_event_type_X_or_Y.add(bkg_pdf);
				bkg_pdfsNames_for_reconstructed_event_type_X_or_Y.push_back(channel + "_bkg_" + X_or_Y);

				RooRealVar bkg_norm((channel + "_bkg_" + X_or_Y + "_norm").c_str(), (channel + "_bkg_" + X_or_Y +"_norm").c_str(), bkg_pdf.getNorm(mass));
				bkg_norm.setConstant(true);
				bkg_Normalizations_for_reconstructed_event_type_X_or_Y.add(bkg_norm);
				bkg_NormalizationsNames_for_reconstructed_event_type_X_or_Y.push_back(channel + "_bkg_" + X_or_Y + "_norm");
				std::cout << "Signal and background functions created for " << channel << "_" << X_or_Y << '\n';
			}
			
			std::string signal_pdf_name = reconstructed_event_type + "_signal_" + X_or_Y;
			std::string bkg_pdf_name = reconstructed_event_type + "_bkg_" + X_or_Y;
			RooAddPdf signal_pdf(signal_pdf_name.c_str(), signal_pdf_name.c_str(), signal_pdfs_for_reconstructed_event_type_X_or_Y, signal_Normalizations_for_reconstructed_event_type_X_or_Y);
			RooAddPdf bkg_pdf(bkg_pdf_name.c_str(), bkg_pdf_name.c_str(), bkg_pdfs_for_reconstructed_event_type_X_or_Y, bkg_NormalizationsNames_for_reconstructed_event_type_X_or_Y);

			signal_X_and_Y.push_back(signal_pdf);
			signal_X_and_YNames.push_back(signal_pdf_name);

			bkg_X_and_Y.push_back(bkg_pdf);
			bkg_X_and_YNames.push_back(bkg_pdf_name);

			// Placeholder pushing back normalizations
			signal_X_and_Y_Normalizations.push_back(signal_Normalizations_for_reconstructed_event_type_X_or_Y[0]);
			signal_X_and_Y_NormalizationsNames.push_back(reconstructed_event_type + "_signal_" + X_or_Y + "norm");

			bkg_X_and_Y_Normalizations.push_back(bkg_Normalizations_for_reconstructed_event_type_X_or_Y[0]);
			bkg_X_and_Y_NormalizationsNames.push_back(reconstructed_event_type + "_bkg_" + X_or_Y + "norm");
			
			std::cout << "Signal and background functions created for " << reconstructed_event_type << "_" << X_or_Y << '\n';
		} 	
	
		signal_pdfs.push_back(signal_X_and_Y);
		signal_pdfsNames.push_back(signal_X_and_YNames);

		bkg_pdfs.push_back(bkg_X_and_Y);
		bkg_pdfsNames.push_back(bkg_X_and_YNames);

		
		signal_Normalizations.push_back(signal_X_and_Y_Normalizations);
		signal_NormalizationsNames.push_back(signal_X_and_Y_NormalizationsNames);


		bkg_Normalizations.push_back(bkg_X_and_Y_Normalizations);
		bkg_NormalizationsNames.push_back(bkg_X_and_Y_NormalizationsNames);
		
		completedReconstructedEventTypes.push_back(reconstructed_event_type);

		std::cout << "All signal and background functions created for " << reconstructed_event_type << "_" << '\n';
	}

	// ----------------------------------------------------------------------------------------------------------
	// The higgsworkspace is where all of the RooFit objects are stored and manipulated.
	// Each object used by the datacard must be imported
	// Some objects, like the realHiggsMass, are implicitly imported when the PDFs or other objects that depend on them are imported.
	// The original PDFs that we copied are not imported
	TFile f_out("higgsworkspace.root", "RECREATE");
	RooWorkspace w_sig("higgsworkspace","higgsworkspace");


	w_sig.import(mc_X);
	w_sig.import(mc_Y);

	// Debug statements


	// std::vector<RooPDF_DSCB_test> signal_X_and_Y;
	// 	std::vector<std::string> signal_X_and_YNames;


	// 	std::vector<RooPDF_BKG> bkg_X_and_Y;
	// 	std::vector<std::string> bkg_X_and_YNames;
		
	// 	std::vector<RooFormulaVar> signal_X_and_Y_Normalizations;
	// 	std::vector<std::string> signal_X_and_Y_NormalizationsNames;

	// 	std::vector<RooRealVar> bkg_X_and_Y_Normalizations;
	// 	std::vector<std::string> bkg_X_and_Y_NormalizationsNames;



	// std::vector<std::vector<RooPDF_DSCB_test>> signal_pdfs;
	// std::vector<std::vector<string>> signal_pdfsNames;

	// std::vector<std::vector<RooPDF_BKG>> bkg_pdfs;
	// std::vector<std::vector<string>> bkg_pdfsNames;

	// std::vector<std::vector<RooFormulaVar>> signal_Normalizations;
	// std::vector<std::vector<string>> signal_NormalizationsNames;

	// std::vector<std::vector<RooRealVar>> bkg_Normalizations;
	// std::vector<std::vector<string>> bkg_NormalizationsNames;	
	std::cout << "signal_pdfs:" << "\n";
	for (auto variable : signal_pdfsNames)
	{
		for (auto name : variable)
		{
			std::cout << name << "\n";

		}
	}
	std::cout << "\n";

	std::cout << "bkg_pdfs:" << "\n";
	for (auto variable : bkg_pdfsNames) 
	{
		for (auto name : variable)
		{
			std::cout << name << "\n";
		}		
	}
	std::cout << "\n";

	std::cout << "signalNormalizations:" << "\n";
	for (auto variable : signal_NormalizationsNames)
	{
		for (auto name : variable)
		{
			std::cout << name << "\n";
		}

	}
	std::cout << "\n";

	std::cout << "bkgNormalizations:" << "\n";
	for (auto variable : bkg_NormalizationsNames)
	{
		for (auto name : variable)
		{
			std::cout << name << "\n";
		}

	}
	std::cout << "\n";

	std::cout << "signal_pdfs size: " << signal_pdfs.size() << "\n";
	std::cout << "signal_pdfs[0] size: " << signal_pdfs[0].size() << "\n";
	std::cout << "signal_pdfs[1] size: " << signal_pdfs[1].size() << "\n" << "\n";


	std::cout << "bkg_pdfs size: " << bkg_pdfs.size() << "\n";
	std::cout << "bkg_pdfs[0] size: " << bkg_pdfs[0].size() << "\n";
	std::cout << "bkg_pdfs[1] size: " << bkg_pdfs[1].size() << "\n" << "\n";


	std::cout << "signal_normalizations size: " << signal_Normalizations.size() << "\n"; 
	std::cout << "signal_Normalizations[0] size: " << signal_Normalizations[0].size() << "\n";
	std::cout << "signal_Normalizations[1] size: " << signal_Normalizations[1].size() << "\n" << "\n";




	std::cout << "bkg_Normalizations size: " << bkg_Normalizations.size() << "\n";
	std::cout << "bkg_Normalizations[0] size: " << bkg_Normalizations[0].size() << "\n";
	std::cout << "bkg_Normalizations[1] size: " << bkg_Normalizations[1].size() << "\n" << "\n";

	std::cout << "Channels to look for:" << "\n";
	for (auto channel : channel_names)
	{
		std::cout << channel << "\n";
	}
	std::cout << "\n \n \n";

	
	for (int i=0; i < channelsToCheck.size(); i++)
	{
		for (int j=0; j < signs.size(); j++)
		{
			std::cout << "Importing " << signal_pdfs[i][j].GetName() << " " << w_sig.arg(signal_pdfs[i][j].GetName()) << "\n";
			w_sig.import(signal_pdfs[i][j]);
			std::cout << "Import successful: " << w_sig.arg(signal_pdfs[i][j].GetName()) << "\n";

			std::cout << "Importing " << bkg_pdfs[i][j].GetName() << " " << w_sig.arg(bkg_pdfs[i][j].GetName()) << "\n";
			w_sig.import(bkg_pdfs[i][j]);
			std::cout << "Import successful: " << w_sig.arg(bkg_pdfs[i][j].GetName()) << "\n";

			std::cout << "Importing " << signal_Normalizations[i][j].GetName() << " " << w_sig.arg(signal_Normalizations[i][j].GetName()) << "\n";
			w_sig.import(signal_Normalizations[i][j]);
			std::cout << "Import successful: " << w_sig.arg(signal_Normalizations[i][j].GetName()) << "\n";

			// w_sig.import(signal_pdfs[i][j].signal_norm());
			std::cout << "Importing " << bkg_Normalizations[i][j].GetName() << " " << w_sig.arg(bkg_Normalizations[i][j].GetName()) << "\n";
			w_sig.import(bkg_Normalizations[i][j]);
			std::cout << "Import successful: " << w_sig.arg(bkg_Normalizations[i][j].GetName()) << "\n";
		}
		// w_sig.Print("v");
		// std::cout << "\n";
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