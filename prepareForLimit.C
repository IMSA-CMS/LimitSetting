#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <memory>
#include <set>
#include <stdexcept>
#include "TFile.h"
#include "TH2.h"
#include "TROOT.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooBinning.h"

struct Channel
{
	std::string name;

	Channel(std::string channelName) : name(channelName) {}
};


void makeCombinedDatacard(std::string filename, std::vector<Channel> channels, std::set<std::string> shapeNames,
                          bool includeSystematics)
{
    if (includeSystematics)
        throw std::runtime_error("Joint systematic templates and validated rate definitions are required");
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
            << "  higgsworkspace.root  higgsworkspace:" << ch << "_data_obs\n";
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


}

void prepareForLimit(bool includeSystematics = false, const char* pairedInputs = "paired-input.root",
                     int modelMass = 500, const char* channelName = "0tau")
{
    if (includeSystematics)
        throw std::runtime_error("Joint systematic templates and validated rate definitions are required");
    gROOT->SetBatch(true);
    TFile input(pairedInputs, "READ");
    if (input.IsZombie())
        throw std::runtime_error("Cannot open paired 2D input file");
    const std::string channel(channelName);
    const std::vector<std::string> names{channel + "_signal", channel + "_ZZ", channel + "_ttbarMultiboson"};
    std::vector<TH2*> histograms;
    for (const auto& name : names)
    {
        auto* histogram = input.Get<TH2>(name.c_str());
        if (!histogram)
            throw std::runtime_error("Missing paired TH2: " + name);
        if (std::string(histogram->GetXaxis()->GetTitle()) != "m++" ||
            std::string(histogram->GetYaxis()->GetTitle()) != "m--")
            throw std::runtime_error("Paired TH2 axes must declare m++ and m--");
        for (int ix = 0; ix <= histogram->GetNbinsX() + 1; ++ix)
            for (int iy = 0; iy <= histogram->GetNbinsY() + 1; ++iy)
            {
                const double value = histogram->GetBinContent(ix, iy);
                const double error = histogram->GetBinError(ix, iy);
                if (!std::isfinite(value) || value < 0 || !std::isfinite(error))
                    throw std::runtime_error("Nonphysical template bin in " + name);
                if ((ix == 0 || iy == 0 || ix == histogram->GetNbinsX() + 1 || iy == histogram->GetNbinsY() + 1) && (value != 0 || error != 0))
                    throw std::runtime_error("Explicit range treatment required for template flow bins in " + name);
            }
        if (!std::isfinite(histogram->Integral()) || histogram->Integral() <= 0)
            throw std::runtime_error("Template yield must be finite and positive: " + name);
        if (!histograms.empty())
        {
            const auto* reference = histograms.front();
            if (histogram->GetNbinsX() != reference->GetNbinsX() || histogram->GetNbinsY() != reference->GetNbinsY())
                throw std::runtime_error("Inconsistent 2D template binning");
            for (int i = 1; i <= histogram->GetNbinsX() + 1; ++i)
                if (histogram->GetXaxis()->GetBinLowEdge(i) != reference->GetXaxis()->GetBinLowEdge(i))
                    throw std::runtime_error("Inconsistent X bin edges");
            for (int i = 1; i <= histogram->GetNbinsY() + 1; ++i)
                if (histogram->GetYaxis()->GetBinLowEdge(i) != reference->GetYaxis()->GetBinLowEdge(i))
                    throw std::runtime_error("Inconsistent Y bin edges");
        }
        histograms.push_back(histogram);
    }
    const auto* reference = histograms.front();
    RooRealVar massX("massX", "m++", reference->GetXaxis()->GetXmin(), reference->GetXaxis()->GetXmax());
    RooRealVar massY("massY", "m--", reference->GetYaxis()->GetXmin(), reference->GetYaxis()->GetXmax());
    std::vector<double> xEdges, yEdges;
    for (int i = 1; i <= reference->GetNbinsX() + 1; ++i)
        xEdges.push_back(reference->GetXaxis()->GetBinLowEdge(i));
    for (int i = 1; i <= reference->GetNbinsY() + 1; ++i)
        yEdges.push_back(reference->GetYaxis()->GetBinLowEdge(i));
    massX.setBinning(RooBinning(reference->GetNbinsX(), xEdges.data()));
    massY.setBinning(RooBinning(reference->GetNbinsY(), yEdges.data()));
    RooWorkspace workspace("higgsworkspace");
    RooRealVar realHiggsMass("realHiggsMass", "realHiggsMass", modelMass);
    workspace.import(realHiggsMass);
    for (std::size_t i = 0; i < histograms.size(); ++i)
    {
        RooDataHist data((names[i] + "_template").c_str(), names[i].c_str(), RooArgList(massX, massY), histograms[i]);
        RooHistPdf pdf(names[i].c_str(), names[i].c_str(), RooArgSet(massX, massY), data, 0);
        RooRealVar norm((names[i] + "_norm").c_str(), names[i].c_str(), histograms[i]->Integral());
        norm.setConstant(true);
        if (workspace.import(pdf) || workspace.import(norm))
            throw std::runtime_error("Cannot import paired PDF or yield");
        std::cout << names[i] << " expected yield " << norm.getVal() << '\n';
    }
    std::unique_ptr<TH2> observation(static_cast<TH2*>(histograms[1]->Clone("asimov_background")));
    observation->SetDirectory(nullptr);
    observation->Add(histograms[2]);
    RooDataHist data((channel + "_data_obs").c_str(), "Background-only simulation expectation",
                     RooArgList(massX, massY), observation.get());
    if (workspace.import(data))
        throw std::runtime_error("Cannot import background-only observation");
    workspace.writeToFile("higgsworkspace.root");
    makeCombinedDatacard("datacard", {Channel(channel)}, {}, false);
}
