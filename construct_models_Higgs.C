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
#include "RooPDF_DSCB.h"
#include "RooPDF_DBLGAUSS.h"
#include "RooPDF_BKG.h"
#include "RooGenericPdf.h"
#include "RooAddPdf.h"



void construct_models_Higgs();


std::vector<double> split(const std::string& line);
std::vector<std::vector<double>> getParameters(std::string fileName);
TGraph makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);


RooPDF_BKG create_bkg_pdf(std::string channel_name, RooRealVar& mass);
RooPDF_DSCB create_DSCB_signal_pdf(std::string channel_name, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2);
RooPDF_DBLGAUSS create_DBLGAUSS_signal_pdf(std::string channel_name, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2);
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


// Creates a RooPDF_DSCB object for a channel (The naming stuff is for consistency, I don't think it matters since I am copying it anyway)
RooPDF_DSCB create_DSCB_signal_pdf(std::string channel_name, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2)
{
    std::vector<std::vector<double>> parameters = getParameters("Parameter_Values/Signal_Parameters/signal_parameters_" + channel_name + ".txt");
   
    // (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
    RooPDF_DSCB pdf((channel_name + "_signal").c_str(), (channel_name + "_signal").c_str(), mass, realHiggsMass, branch_1, branch_2, parameters, false);
    return pdf;
}


// Creates a Roo_PDF_DBLGAUSS object for a channel (the naming stuff is for consistency, I don't thin kit matters since I am copying it anyways)
RooPDF_DBLGAUSS create_DBLGAUSS_signal_pdf(std::string channel_name, RooRealVar& mass, RooRealVar& realHiggsMass, RooRealVar& branch_1, RooRealVar& branch_2)
{
    std::vector<std::vector<double>> parameters = getParameters("Parameter_Values/Signal_Parameters/signal_parameters_" + channel_name + ".txt");
   
    // (The naming stuff is for consistency, I don't think it matters since I am copying the object out of the function anyway)
    RooPDF_DBLGAUSS pdf((channel_name + "_signal").c_str(), (channel_name + "_signal").c_str(), mass, realHiggsMass, branch_1, branch_2, parameters, false);
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

// old broken method
// void construct_models_Higgs()
// {
//     gROOT->SetBatch(true);

//     TFile file("Usefulstuff/output900.root","READ");
//     auto hist_X = file.Get<TTree>("Signal");

//     RooRealVar mass("mass", "mass", 900, 50, 2000);

//     RooDataSet mc_X("Events900_X","Events900", hist_X, RooArgSet(mass), "");
//     RooDataSet mc_Y("Events900_Y","Events900", hist_X, RooArgSet(mass), ""); // change to hist_Y when available

//     RooRealVar realHiggsMass("realHiggsMass", "realHiggsMass",1500, 200,2000);
//     RooRealVar Bee("b_ee", "b_ee", 1, 0,1);
//     RooRealVar Beu("b_eu", "b_eu",1, 0,1);
//     Bee.setConstant(true);
//     Beu.setConstant(true);

//     gROOT->ProcessLineSync(".x RooPDF_DSCB.cxx+");
//     gROOT->ProcessLineSync(".x RooPDF_DBLGAUSS.cxx+");
//     gROOT->ProcessLineSync(".x RooPDF_BKG.cxx+");

//     std::vector<std::string> channel_names {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eeet"};
//     std::vector<std::string> signs {"X", "Y"};

//     std::vector<std::vector<RooPDF_DSCB>> eu_only_signal_pdfs;
//     std::vector<std::vector<RooAddPdf>> leptonic_t_signal_pdfs;
//     std::vector<std::vector<RooPDF_BKG>> bkg_pdfs;
//     std::vector<std::vector<RooFormulaVar>> signal_Normalizations;
//     std::vector<std::vector<RooRealVar>> bkg_Normalizations;

//     for (auto channel : channel_names)
//     {
//         char lepton1 = channel[0];
//         char lepton2 = channel[1];
//         char lepton3 = channel[2];
//         char lepton4 = channel[3];

//         bool iseeee = (lepton1=='e' && lepton2=='e' && lepton3=='e' && lepton4=='e');
//         bool iseeet = (lepton1=='e' && lepton2=='e' && lepton3=='e' && lepton4=='t');

//         std::vector<RooPDF_DSCB> eu_only_signal_XY;
//         std::vector<RooAddPdf> leptonic_t_signal_XY;
//         std::vector<RooPDF_BKG> bkg_XY;
//         std::vector<RooFormulaVar> eu_only_signal_XY_norms;
//         std::vector<RooFormulaVar> leptonic_t_signal_XY_norms;
//         std::vector<RooRealVar> bkg_XY_norms;

//         if (iseeet)
//         {
//             for (auto X_or_Y : signs)
//             {
//                 RooPDF_DBLGAUSS signal_eeet_pdf(
//                     create_DBLGAUSS_signal_pdf(channel, mass, realHiggsMass, Bee, Beu),
//                     (channel + "_signal_" + X_or_Y + "_eeet").c_str()
//                 );

//                 RooPDF_DSCB signal_eeee_pdf(
//                     create_DSCB_signal_pdf("eeee", mass, realHiggsMass, Bee, Beu),
//                     ("eeee_signal_" + X_or_Y).c_str()
//                 );

//                 RooRealVar frac(
//                     ("frac_"+channel+"_"+X_or_Y).c_str(),
//                     "fraction of eeet",
//                     0.5, 0.0, 1.0
//                 );

//                 std::cerr << "eeee: " << signal_eeee_pdf.GetName() << " eeet: " << signal_eeet_pdf.GetName() << '\n';

//                 RooArgList pdfSignalList;
//                 pdfSignalList.add(signal_eeee_pdf);
//                 pdfSignalList.add(signal_eeet_pdf);
//                 std::cout << "Element 1: " << pdfSignalList[0].GetName()  << " Element 2: :" << pdfSignalList[1].GetName() << '\n';

//                 RooAddPdf signal_pdf(
//                     (channel + "_signal_" + X_or_Y).c_str(),
//                     ("sum of eeet+eeee for "+X_or_Y).c_str(),
//                     pdfSignalList,
//                     RooArgList(frac)
//                 );

//                 RooFormulaVar signal_norm(
//                     (channel + "_signal_" + X_or_Y + "_norm").c_str(),
//                     "@0",
//                     RooArgList(signal_pdf)
//                 );

//                 leptonic_t_signal_XY.push_back(signal_pdf);
//                 leptonic_t_signal_XY_norms.push_back(signal_norm);
//             }
//         }
//         else if (iseeee)
//         {
//             continue;
//         }
//         else
//         {
//             for (auto X_or_Y : signs)
//             {
//                 RooPDF_DSCB signal_pdf(
//                     create_DSCB_signal_pdf(channel, mass, realHiggsMass, Bee, Beu),
//                     (channel + "_signal_" + X_or_Y).c_str()
//                 );
//                 eu_only_signal_XY.push_back(signal_pdf);
//                 eu_only_signal_XY_norms.push_back(signal_pdf.signal_norm(channel + "_signal_" + X_or_Y));

//                 RooPDF_BKG bkg_pdf(
//                     create_bkg_pdf(channel, mass),
//                     (channel + "_bkg_" + X_or_Y).c_str()
//                 );
//                 bkg_XY.push_back(bkg_pdf);

//                 RooRealVar bkg_norm(
//                     (channel + "_bkg_" + X_or_Y + "_norm").c_str(),
//                     (channel + "_bkg_" + X_or_Y +"_norm").c_str(),
//                     bkg_pdf.getNorm(mass)
//                 );
//                 bkg_norm.setConstant(true);
//                 bkg_XY_norms.push_back(bkg_norm);
//             }
//         }

//         eu_only_signal_pdfs.push_back(eu_only_signal_XY);
//         std::cout << __LINE__ << '\n';
//         leptonic_t_signal_pdfs.push_back(leptonic_t_signal_XY);
//         std::cout << __LINE__ << '\n';
//         bkg_pdfs.push_back(bkg_XY);
//         signal_Normalizations.push_back(eu_only_signal_XY_norms);
//         signal_Normalizations.push_back(leptonic_t_signal_XY_norms);
//         bkg_Normalizations.push_back(bkg_XY_norms);
//         std::cout << __LINE__ << '\n';
//     }

//     TFile f_out("higgsworkspace.root", "RECREATE");
//     RooWorkspace w_sig("higgsworkspace","higgsworkspace");

//     w_sig.import(mc_X);
//     w_sig.import(mc_Y);

//     // --- Import eeee/eeet PDFs separately ---
//     for (int i=0; i<channel_names.size(); ++i)
//     {
//         std::string channel = channel_names[i];
//         bool iseeee = (channel=="eeee");
//         bool iseeet = (channel=="eeet");

//         if (iseeet)
//         {
//             for (int j=0; j<signs.size(); ++j)
//             {
//                 std::cout << __LINE__ << '\n';
//                 std::cout << "About to import to workspace\n";
//                 w_sig.import(leptonic_t_signal_pdfs[i][j]);
//                 w_sig.import(signal_Normalizations[2*i+1][j]); // index offset for eeet
//             }
//         }
//         else if (!iseeee)
//         {
//             for (int j=0; j<signs.size(); ++j)
//             {
//                 w_sig.import(eu_only_signal_pdfs[i][j]);
//                 w_sig.import(signal_Normalizations[2*i][j]); // index offset for eu-only
//                 w_sig.import(bkg_pdfs[i][j]);
//                 w_sig.import(bkg_Normalizations[i][j]);
//             }
//         }
//     }

//     w_sig.Write();
//     f_out.Close();
// }

void construct_models_Higgs() {
    gROOT->SetBatch(true);

    // Load input file
    TFile file("Usefulstuff/output900.root", "READ");
    auto hist_X = file.Get<TTree>("Signal");

    // Define observable and datasets
    RooRealVar mass("mass", "mass", 900, 50, 2000);
    RooDataSet mc_X("Events900_X", "Events900", hist_X, RooArgSet(mass), "");
    RooDataSet mc_Y("Events900_Y", "Events900", hist_X, RooArgSet(mass), "");  // placeholder

    // Physics parameters
    RooRealVar realHiggsMass("realHiggsMass", "realHiggsMass", 1500, 200, 2000);
    RooRealVar Bee("b_ee", "b_ee", 1, 0, 1);
    RooRealVar Beu("b_eu", "b_eu", 1, 0, 1);
    Bee.setConstant(true);
    Beu.setConstant(true);

    // Load custom PDFs
    gROOT->ProcessLineSync(".x RooPDF_DSCB.cxx+");
    gROOT->ProcessLineSync(".x RooPDF_DBLGAUSS.cxx+");
    gROOT->ProcessLineSync(".x RooPDF_BKG.cxx+");

    // Define channels and charge combinations
    std::vector<std::string> channel_names {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eeet"};
    std::vector<std::string> signs {"X", "Y"}; 

    // Create workspace and output file
    TFile f_out("higgsworkspace.root", "RECREATE");
    RooWorkspace w_sig("higgsworkspace", "higgsworkspace");

    // Import datasets
    w_sig.import(mc_X);
    w_sig.import(mc_Y);

    // Loop through channels
    for (auto channel : channel_names) {
        char lepton1 = channel[0];
        char lepton2 = channel[1];
        char lepton3 = channel[2];
        char lepton4 = channel[3];

        bool iseeee = (lepton1 == 'e' && lepton2 == 'e' && lepton3 == 'e' && lepton4 == 'e');
        bool iseeet = (lepton1 == 'e' && lepton2 == 'e' && lepton3 == 'e' && lepton4 == 't');

        for (auto X_or_Y : signs) {
            // Background always built the same way
            std::string bkg_pdf_name = channel + "_bkg_" + X_or_Y;
            RooPDF_BKG bkg_pdf(create_bkg_pdf(channel, mass), bkg_pdf_name.c_str());
            w_sig.import(bkg_pdf, RooFit::RecycleConflictNodes());

            if (iseeet) {
                // Mixed channel: combine eeee and eeet components
                std::string eeee_name = "eeee_signal_" + X_or_Y + "_component";
                std::string eeet_name = "eeet_signal_" + X_or_Y + "_component";

                RooPDF_DSCB eeee_pdf(
                    create_DSCB_signal_pdf("eeee", mass, realHiggsMass, Bee, Beu),
                    eeee_name.c_str()
                );
                RooPDF_DBLGAUSS eeet_pdf(
                    create_DBLGAUSS_signal_pdf("eeet", mass, realHiggsMass, Bee, Beu),
                    eeet_name.c_str()
                );

                w_sig.import(eeee_pdf, RooFit::RecycleConflictNodes());
                w_sig.import(eeet_pdf, RooFit::RecycleConflictNodes());

                // Fraction parameter
                std::string frac_name = "frac_" + channel + "_" + X_or_Y;
                RooRealVar frac(frac_name.c_str(), "fraction of eeet", 0.5, 0.0, 1.0);
                w_sig.import(frac, RooFit::RecycleConflictNodes());

                // Build RooRealSumPdf directly
                std::string combo_name = channel + "_signal_" + X_or_Y;

                RooArgList pdfs;
                pdfs.add(*w_sig.pdf(eeet_name.c_str()));   // <-- dereference pointer!
                pdfs.add(*w_sig.pdf(eeee_name.c_str()));   // <-- dereference pointer!

                RooArgList coeffs;
                coeffs.add(frac);

                RooAddPdf signal_sum_pdf(
                    combo_name.c_str(),
                    combo_name.c_str(),
                    pdfs,
                    coeffs,
                    true   // recursiveFraction = true → second gets 1 - fracA
                );

                w_sig.import(signal_sum_pdf, RooFit::RecycleConflictNodes());
            }



            else if (iseeee) {
                // Simple single component
                std::string sig_name = "eeee_signal_" + X_or_Y;
                RooPDF_DSCB sig_pdf(create_DSCB_signal_pdf("eeee", mass, realHiggsMass, Bee, Beu),
                                    sig_name.c_str());
                w_sig.import(sig_pdf, RooFit::RecycleConflictNodes());
            }

            else {
                // Other channels, single component signal
                std::string sig_name = channel + "_signal_" + X_or_Y;
                RooPDF_DSCB sig_pdf(create_DSCB_signal_pdf(channel, mass, realHiggsMass, Bee, Beu),
                                    sig_name.c_str());
                w_sig.import(sig_pdf, RooFit::RecycleConflictNodes());
            }
        }
    }

    // Save workspace
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