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
#include "RooPDF_BKG.h"
#include "RooGenericPdf.h"


void construct_models_Higgs_3();

std::vector<double> split(const std::string& line);
std::vector<std::vector<double>> getParameters(std::string fileName);
TGraph makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);

RooPDF_BKG create_bkg_pdf(std::string channel_name, RooRealVar& mass);
RooPDF_DSCB_test create_signal_pdf(std::string channel_name, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2, RooRealVar& norm_Systematic, RooRealVar& shape_Systematic);
RooFormulaVar get_signal_norm(std::string channel_name, RooRealVar& realHiggsMass);



// Splits lines by spaces, skipping the first entry (which I used as a name and not a value I actually need)
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

// Gets the signal/background parameters from file; skips the first few lines, which are used as explanation for organization
std::vector<std::vector<double>> getParameters(std::string fileName)
{
	std::ifstream file(fileName);
	std::string line;
	int i = 0;

	std::vector<std::vector<double>> parameterArrays;
	while (std::getline(file, line))
	{
		if (i < 3) 
		{
			i++;
		} 
		else 
		{
			parameterArrays.push_back(split(line));
		}
	}
	return parameterArrays;
}

// Creates a RooPDF_DSCB_test object for a channel (The naming stuff is for consistency, I don't think it matters since I am copying it anyway)
RooPDF_DSCB_test create_signal_pdf(std::string channel_name, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2, RooRealVar& norm_Systematic, RooRealVar& shape_Systematic)
{
	std::vector<std::vector<double>> parameters = getParameters("Parameter_Values/Signal_Parameters/signal_parameters_" + channel_name + ".txt");
	
	// (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
	RooPDF_DSCB_test pdf((channel_name + "_signal").c_str(), (channel_name + "_signal").c_str(), mass, realHiggsMass, branch_1, branch_2, norm_Systematic, shape_Systematic, parameters, false); 
	return pdf;
}

// Creates the Signal Normalization Object for a channel - The Normalization object must vary with mass when it is added to workspace, which is why it is a FormulaVar and not just be the integral over the signal pdf object 
RooFormulaVar get_signal_norm(std::string channel_name, RooRealVar& realHiggsMass)
{
	std::vector<std::vector<double>> parameters = getParameters("Parameter_Values/Signal_Parameters/signal_parameters_" + channel_name + ".txt");
	std::string norm_string = std::to_string(parameters[6][0]) +  "* (@0 - " + std::to_string(parameters[6][1]) + ")^" + std::to_string(parameters[6][2]) + " + " + std::to_string(parameters[6][3]);
	
	// (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
	RooFormulaVar norm((channel_name + "_signal_norm").c_str(), (channel_name + "_signal_norm").c_str(), norm_string.c_str(), RooArgList(realHiggsMass)); 
	return norm;
}

// Creates a RooPDF_BKG Object 
RooPDF_BKG create_bkg_pdf(std::string channel_name,RooRealVar& mass)
{
	std::vector<std::vector<double>> bkg_types_params= getParameters("Parameter_Values/Background_Parameters/background_parameters_" + channel_name + ".txt");
	
	// (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
	RooPDF_BKG pdf((channel_name + "_bkg").c_str(), (channel_name + "_bkg").c_str(), mass, bkg_types_params, channel_name); 
	return pdf;
}

void construct_models_Higgs_3()
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
	RooRealVar norm_Systematic("normalization_systematic", "normalization_systematic", 1, 0, 1);
	RooRealVar shape_Systematic("shape_systematic", "shape_systeamtic", 1, 0, 1); 
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
	gROOT -> ProcessLineSync(".x RooPDF_DSCB_test.cxx+");
	gROOT -> ProcessLineSync(".x RooPDF_BKG.cxx+");


	// We will want to change how these lists are handled..

	std::vector<std::string> channel_names {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu"};
	std::vector<std::string> signs {"X", "Y"};

	std::vector<std::vector<RooPDF_DSCB_test>> signal_pdfs;
	std::vector<std::vector<RooPDF_BKG>> bkg_pdfs;

	std::vector<std::vector<RooFormulaVar>> signal_Normalizations;
	std::vector<std::vector<RooRealVar>> bkg_Normalizations;

	for (std::string channel : channel_names)
	{
		std::vector<RooPDF_DSCB_test> signal_X_and_Y;
		std::vector<RooPDF_BKG> bkg_X_and_Y;
		
		std::vector<RooFormulaVar> signal_X_and_Y_Normalizations;
		std::vector<RooRealVar> bkg_X_and_Y_Normalizations;

		for(std::string X_or_Y : signs)
		{
			RooPDF_DSCB_test signal_pdf(create_signal_pdf(channel, mass, realHiggsMass, Bee, Beu, norm_Systematic, shape_Systematic), (channel + "_signal_" + X_or_Y).c_str());
			signal_X_and_Y.push_back(signal_pdf);
			
			// RooFormulaVar signal_norm(get_signal_norm(channel, realHiggsMass), (channel+"_signal_" + X_or_Y + "_norm").c_str());
			// signal_X_and_Y_Normalizations.push_back(signal_norm);
			signal_X_and_Y_Normalizations.push_back(signal_pdf.signal_norm(channel + "_signal_" + X_or_Y));

			RooPDF_BKG bkg_pdf(create_bkg_pdf(channel, mass), (channel + "_bkg_" + X_or_Y).c_str());
			bkg_X_and_Y.push_back(bkg_pdf);

			RooRealVar bkg_norm((channel + "_bkg_" + X_or_Y + "_norm").c_str(), (channel + "_bkg_" + X_or_Y +"_norm").c_str(), bkg_pdf.getNorm(mass));
			bkg_norm.setConstant(true);
			bkg_X_and_Y_Normalizations.push_back(bkg_norm);
		}

		signal_pdfs.push_back(signal_X_and_Y);
		bkg_pdfs.push_back(bkg_X_and_Y);
		signal_Normalizations.push_back(signal_X_and_Y_Normalizations);
		bkg_Normalizations.push_back(bkg_X_and_Y_Normalizations);
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


	for (int i=0; i < channel_names.size(); i++)
	{
		for (int j=0; j < signs.size(); j++)
		{
			w_sig.import(signal_pdfs[i][j]);
			w_sig.import(bkg_pdfs[i][j]);
			w_sig.import(signal_Normalizations[i][j]);
			// w_sig.import(signal_pdfs[i][j].signal_norm());
			w_sig.import(bkg_Normalizations[i][j]);
		}
	}

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