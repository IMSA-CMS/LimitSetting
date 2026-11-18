#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <typeinfo>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>

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
#include "RooArgList.h"
#include "RooGenericPdf.h"
#include "RooNumIntConfig.h"


#include "HiggsAnalysis/CombinedLimit/interface/FitFunctionPDF.h"
#include "HiggsAnalysis/CombinedLimit/interface/FitFunctionPDF2D.h"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionParameterization.hh"

struct Process
{
	std::shared_ptr<FitFunction> function;
	RooAbsPdf* pdf = nullptr;
	RooAbsReal* norm = nullptr;
};

struct Channel
{
	std::string name;

	Channel(std::string channelName) : name(channelName) {}
};


std::vector<double> split(const std::string& line);
std::string replaceAll(std::string unmodifiedString, const std::string from, const std::string to);


void makeCombinedDatacard(std::string filename, std::vector<Channel> channels, std::set<std::string> shapeNames,
                          bool includeSystematics)
{
    std::ostringstream out;
    int nBins = channels.size();

    // header
    out << "imax " << nBins << "\n";
    out << "jmax " << (3 * nBins - 1) << "\n";
    out << "kmax *\n";
    out << "---------------------------------------------\n";

    // shapes: one 2D (X, Y) pdf per process in each channel
    for (size_t i = 0; i < channels.size(); i++) {
        std::string ch = channels[i].name;
        std::string bin = "ch" + std::to_string(i+1);
        out << "shapes " << ch << "  " << bin
            << "  higgsworkspace.root  higgsworkspace:" << ch << "_signal\n";
        out << "shapes ZZ_" << ch << "  " << bin
            << "  higgsworkspace.root  higgsworkspace:" << ch << "_ZZ\n";
        out << "shapes ttbarMultiboson_" << ch << "  " << bin
            << "  higgsworkspace.root  higgsworkspace:" << ch << "_ttbarMultiboson\n";
        out << "shapes data_obs  " << bin
            << "  higgsworkspace.root  higgsworkspace:Events900\n";
    }
    out << "---------------------------------------------\n";

    out << "bin  ";                                          
    for (size_t i = 0; i < channels.size(); i++)
        out << "ch" << i+1 << "  ";
    out << "\nobservation  ";                              
    for (int i = 0; i < nBins; i++) out << "-1  ";       
    out << "\n---------------------------------------------\n";

    out << "bin  ";                                        
    for (size_t i = 0; i < channels.size(); i++) {
        std::string bin = "ch" + std::to_string(i+1);
        out << bin << "  " << bin << "  " << bin << "  ";
    }
    out << "\nprocess  ";                                   
    for (auto& ch : channels)
        out << ch.name << "  ZZ_" << ch.name << "  ttbarMultiboson_" << ch.name << "  ";
    out << "\nprocess  ";                                   
    for (int i = 0; i < nBins; i++)
        out << "0  1  2  ";                                 
    out << "\nrate  ";                                       
    for (int i = 0; i < 3 * nBins; i++) out << "1  ";       
    out << "\n---------------------------------------------\n";

       std::ofstream(filename + ".txt") << out.str();

    std::ostringstream syst;
    if (includeSystematics)
    {
        for (const auto& n : shapeNames)
            syst << n << " param 0 1\n";

        syst << "lumi lnN";
        for (int i = 0; i < 3 * nBins; i++)
            syst << " 1.025"; // PLACEHOLDER
        syst << "\n";

        std::map<std::string, std::pair<double, double>> xsec;
        std::ifstream in("crossSections.txt");
        std::string line;
        while (std::getline(in, line))
        {
            std::istringstream ss(line);
            std::string name;
            double value = 0, error = 0;
            if (ss >> name >> value)
            {
                ss >> error;
                xsec[name] = {value, error};
            }
        }

        auto frac = [&](std::vector<std::string> samples) {
            double sumOfSquares = 0;
            for (const auto& s : samples)
            {
                auto [v, e] = xsec[s];
                if (v > 0 && e > 0)
                {
                    double fr = e / v; // fractional uncertainty (2nd / 1st)
                    sumOfSquares += std::pow(fr, 2);
                }
            }
            return std::sqrt(sumOfSquares);
        };

        const std::string procNames[3] = {"signal", "ZZ", "ttbarMultiboson"};
        const double fracs[3] = {
            0,
            frac({"ZZTo4L", "ZZTo2mu2tau", "ZZTo2e2mu", "ZZTo2e2tau", "ZZTo4mu", "ZZTo4tau", "ZZTo4e"}),
            frac({"WWTo2L2Nu", "WZTo3LNu", "WWW", "WWZ", "WZZ", "ZZZ", "TTW", "TTZ", "TTTo2L2Nu"})};
        for (int p = 1; p <= 2; p++)
        {
            syst << "xsec_" << procNames[p] << " lnN";
            for (int i = 0; i < 3 * nBins; i++)
                syst << (i % 3 == p ? " " + std::to_string(1 + fracs[p]) : " -");
            syst << "\n";
        }
    }

    std::ofstream(filename + "_syst.txt") << out.str() << syst.str();
}

void prepareForLimit(bool includeSystematics = true)
{
	gROOT->SetBatch(true); // I can't remember what this does exactly, but it needs to be here
	// need to keep the normalization derivatives accurate cause raw signal has v small yield
	RooNumIntConfig signalIntegration(*RooAbsReal::defaultIntegratorConfig());
	signalIntegration.setEpsAbs(1e-12);
	signalIntegration.setEpsRel(1e-12);

	// Get the Signal Events from Monte Carlo
	// We will be fitting our model to these events
	// The X and Y refer to the sign (+ or -) of the leptons, which can be plotted on the X and Y axises

	TFile file("Usefulstuff/output900.root","READ");
	auto hist_X = file.Get<TTree>("Signal");

	// These are the invariant masses of the two projections and are the independent variables of the 2D Signal and background PDFs
	RooRealVar massX("massX", "massX", 900, 50, 2000);
	RooRealVar massY("massY", "massY", 900, 50, 2000);

	// Build a 2D RooDataSet by pairing entries of the X and Y trees (which are the same tree for now).
	// Both trees must have the same number of entries and the "mass" branch.
	auto hist_Y = hist_X; // When we get both data sets, change to the Y tree
	double treeMassX = 0, treeMassY = 0;
	hist_X->SetBranchAddress("mass", &treeMassX);
	hist_Y->SetBranchAddress("mass", &treeMassY);
	RooDataSet mc("Events900", "Events900", RooArgSet(massX, massY));
	for (Long64_t entry = 0; entry < hist_X->GetEntries(); ++entry)
	{
		hist_X->GetEntry(entry);
		const double xValue = treeMassX;
		hist_Y->GetEntry(entry);
		if (xValue < massX.getMin() || xValue > massX.getMax() || treeMassY < massY.getMin() || treeMassY > massY.getMax())
			continue;
		massX.setVal(xValue);
		massY.setVal(treeMassY);
		mc.add(RooArgSet(massX, massY));
	}

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
	// Need to keep the signal nusiances alive until all pdfs are imported
	std::map<std::string, std::unique_ptr<RooRealVar>> shapeNuisances;
	
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


	w_sig.import(mc);


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
	std::vector<std::string> channelsToCheck = {"eeee", "uuuu"};

	// Get signal and background parameters from files - Note: Update file paths later

	std::string signalParamsFileName = "/uscms/home/ssahoo1/heze/fits/9_28_26/kansas/H++SignalParameterFunctions.txt";
	std::string backgroundParamsFileName = "/uscms/home/ssahoo1/heze/fits/9_28_26/kansas/H++BackgroundFunctions.txt";

	const std::map<std::string, std::string> backgroundProcessNames = {
		{"Drell-Yan Background", "DY"},
		{"ZZ Background", "ZZ"},
		{"t#bar{t}, Multiboson Background", "ttbarMultiboson"}
	};

	std::vector<FitFunctionParameterization> signalModels = FitFunctionParameterization::loadFunctions(signalParamsFileName);
	FitFunctionCollection backgroundCollection = FitFunctionCollection::loadFunctions(backgroundParamsFileName);

	std::set<std::string> channelNames;
	//takes first from different gensims, eventually we will need to do something with them
	for (const auto& signalModel : signalModels)
	{
		std::cout << "Signal Model Name: " << signalModel.getName() << "\n";
		channelNames.insert(FitFunction::decodeName(signalModel.getName()).at("Channel"));
		std::cout << "Channel Name: " << FitFunction::decodeName(signalModel.getName()).at("Channel") << "\n";
	}

	std::vector<Channel> channels;
	for (const auto& channelName : channelNames)
	{
		std::cout << "Channel name: " << channelName << '\n';
		channels.push_back(Channel(channelName));
	}

	for (auto& channel : channels)
	{
		std::cout << "Processing " << channel.name << " channels\n";

		const std::string fullChannelName = channel.name;
		std::cout << "Processing " << fullChannelName << "\n";

		// Use the first generated signal model for this channel and projection.
		auto findSignalModel = [&](const std::string& projection) {
			const auto signalModel = std::find_if(signalModels.begin(), signalModels.end(), [&](const auto& candidate) {
				const auto metadata = FitFunction::decodeName(candidate.getName());
				return metadata.at("Channel") == channel.name && metadata.at("Projection") == projection;
			});
			if (signalModel == signalModels.end())
				throw std::runtime_error("No signal functions for " + channel.name + "_" + projection);
			return std::make_shared<FitFunctionParameterization>(*signalModel);
		};
		const auto modelX = findSignalModel("X");
		const auto modelY = findSignalModel("Y");

		// Returns the cleaned nuisance names' deltas for a model, creating nuisance variables as needed
		auto buildDeltas = [&](const std::vector<std::string>& shapeNames) {
			RooArgList shapeDeltas;
			for (const auto& name : shapeNames)
			{
				// RooFit needs whitespace trimmed
				const auto first = name.find_first_not_of(" \t\r\n");
				const std::string nuisanceName = first == std::string::npos ? "" :
					name.substr(first, name.find_last_not_of(" \t\r\n") - first + 1);
				if (nuisanceName.empty() || nuisanceName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") != std::string::npos)
					throw std::invalid_argument("Invalid shape-systematic name: " + name);
				if (nuisanceName == massX.GetName() || nuisanceName == massY.GetName() || nuisanceName == realHiggsMass.GetName() ||
				    nuisanceName == Bee.GetName() || nuisanceName == Beu.GetName() ||
				    nuisanceName == norm_Systematic.GetName() || nuisanceName == shape_Systematic.GetName())
					throw std::invalid_argument("Shape-systematic name collides with a model variable: " + name);
				// construct delta
				auto& delta = shapeNuisances[nuisanceName];
				if (!delta)
					delta = std::make_unique<RooRealVar>(nuisanceName.c_str(), nuisanceName.c_str(), 0.0, -5.0, 5.0);
				shapeDeltas.add(*delta);
			}
			return shapeDeltas;
		};
		const auto shapeNamesX = includeSystematics ? modelX->listSystematics() : std::vector<std::string>{};
		const auto shapeNamesY = includeSystematics ? modelY->listSystematics() : std::vector<std::string>{};
		const RooArgList shapeDeltasX = buildDeltas(shapeNamesX);
		const RooArgList shapeDeltasY = buildDeltas(shapeNamesY);

		auto signal_pdf = std::make_unique<FitFunctionPDF2D>(
			(channel.name + "_signal").c_str(), (channel.name + "_signal").c_str(),
			massX, massY, realHiggsMass, Bee, Beu, norm_Systematic, shape_Systematic,
			modelX, modelY, shapeNamesX, shapeDeltasX, shapeNamesY, shapeDeltasY);
		signal_pdf->setIntegratorConfig(signalIntegration);

		auto signal_norm = signal_pdf->signal_norm(signal_pdf->GetName());

		// Import signal
		std::cout << "Importing Signal PDF " << signal_pdf->GetName() << "\n";
		w_sig.import(*signal_pdf);
		std::cout << "Importing Signal Normalization " << signal_norm.GetName() << "\n";
		w_sig.import(signal_norm);

		auto backgroundFunctionsX = backgroundCollection.getFunctions("Channel", channel.name).getFunctions("Projection", "X").getFunctionsMap();
		auto backgroundFunctionsY = backgroundCollection.getFunctions("Channel", channel.name).getFunctions("Projection", "Y").getFunctionsMap();

		// Match the X and Y background functions of each process
		auto processShortName = [&](const std::shared_ptr<FitFunction>& function) {
			const auto process = FitFunction::decodeName(function->getName()).at("Process");
			const auto shortName = backgroundProcessNames.find(process);
			if (shortName == backgroundProcessNames.end())
				throw std::runtime_error("No short name for background process " + process);
			return shortName->second;
		};
		for (auto& [keyX, backgroundFunctionX] : backgroundFunctionsX)
		{
			const std::string shortName = processShortName(backgroundFunctionX);
			std::shared_ptr<FitFunction> backgroundFunctionY;
			for (auto& [keyY, candidate] : backgroundFunctionsY)
				if (processShortName(candidate) == shortName)
					backgroundFunctionY = candidate;
			if (!backgroundFunctionY)
				throw std::runtime_error("No Y background function for " + channel.name + "_" + shortName);
			const std::string pdfName = channel.name + "_" + shortName;

			auto* bkg_pdf = new FitFunctionPDF2D(
				pdfName.c_str(), pdfName.c_str(), massX, massY, realHiggsMass, Bee, Beu, norm_Systematic, shape_Systematic,
				backgroundFunctionX, backgroundFunctionY);

			RooRealVar bkg_norm((std::string(bkg_pdf->GetName()) + "_norm").c_str(), (std::string(bkg_pdf->GetName()) + "_norm").c_str(),
				std::stod(backgroundFunctionX->getNormExpression("")));
			bkg_norm.setConstant(true);

			// Import background
			std::cout << "Importing Background PDF " << bkg_pdf->GetName() << "\n";
			w_sig.import(*bkg_pdf);
			std::cout << "Importing Background Normalization " << backgroundFunctionX->getNormExpression("") << "\n";
			w_sig.import(bkg_norm);
		}
	}

	std::set<std::string> nuisanceNames;                  
	for (const auto& entry : shapeNuisances)                
		nuisanceNames.insert(entry.first);          
	makeCombinedDatacard("datacard", channels, nuisanceNames, includeSystematics);

	w_sig.Print("v");
	std::cout << "\n";
	std::cout << "Writing to workspace \n";
	w_sig.Write();
	f_out.Close();

	// Add constraints once to datacard with kmax *
	// this works for some reason?
	std::ofstream shapeConstraints("signal_shape_systematics.txt");
	if (!shapeConstraints)
		throw std::runtime_error("Cannot write signal_shape_systematics.txt");
	for (const auto& entry : shapeNuisances)
		shapeConstraints << entry.first << " param 0 1\n";
}
