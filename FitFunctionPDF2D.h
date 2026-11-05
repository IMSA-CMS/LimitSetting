#ifndef FitFunctionPDF2D_h
#define FitFunctionPDF2D_h

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooArgList.h"
#include "RooListProxy.h"
#include "RooAbsReal.h"
#include "RooFormulaVar.h"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"

#include <memory>
#include <string>
#include <vector>

// 2D PDF in (x, y) whose value is the product of two FitFunctions, one evaluated at x and one at y.
class FitFunctionPDF2D : public RooAbsPdf
{
public:
   FitFunctionPDF2D() {}
   FitFunctionPDF2D(const char *name, const char *title,
        RooAbsReal& _x,
        RooAbsReal& _y,
        RooAbsReal& _realHiggsMass,
        RooAbsReal& _branch_ratio_1,
        RooAbsReal& _branch_ratio_2,
        RooAbsReal& _norm_Systematic,
        RooAbsReal& _shape_Systematic,
        std::shared_ptr<FitFunction> functionX,
        std::shared_ptr<FitFunction> functionY);
   FitFunctionPDF2D(const char *name, const char *title,
        RooAbsReal& _x,
        RooAbsReal& _y,
        RooAbsReal& _realHiggsMass,
        RooAbsReal& _branch_ratio_1,
        RooAbsReal& _branch_ratio_2,
        RooAbsReal& _norm_Systematic,
        RooAbsReal& _shape_Systematic,
        std::shared_ptr<FitFunction> functionX,
        std::shared_ptr<FitFunction> functionY,
        const std::vector<std::string>& systematicNamesX,
        const RooArgList& deltasX,
        const std::vector<std::string>& systematicNamesY,
        const RooArgList& deltasY);
   FitFunctionPDF2D(FitFunctionPDF2D const &other, const char *name=nullptr);
   TObject *clone(const char *newName) const override { return new FitFunctionPDF2D(*this, newName); }

   double evaluate() const override;
   // The yield is the same in both projections, so the normalization comes from the X function.
   RooFormulaVar signal_norm(std::string channel_name) const;

protected:
   RooRealProxy x ;
   RooRealProxy y ;
   RooRealProxy realHiggsMass ;
   RooRealProxy branch_ratio_1 ;
   RooRealProxy branch_ratio_2 ;
   RooRealProxy norm_Systematic ;
   RooRealProxy shape_Systematic ;
   // Unique deltas shared by both functions; each function refers to them by index
   RooListProxy shape_Systematics{"shape_Systematics", "shape_Systematics", this};

private:
   void addSystematics(const FitFunction& function, const std::vector<std::string>& systematicNames,
                       const RooArgList& deltas, std::vector<std::string>& names, std::vector<int>& indices);
   FitFunction::NuisanceValues getNuisances(const std::vector<std::string>& names, const std::vector<int>& indices) const;

   std::shared_ptr<FitFunction> functionX; //! custom streamer, shared_ptr io doesn't work
   std::shared_ptr<FitFunction> functionY; //!
   std::vector<std::string> shapeSystematicNamesX;
   std::vector<int> shapeSystematicIndicesX;
   std::vector<std::string> shapeSystematicNamesY;
   std::vector<int> shapeSystematicIndicesY;

   ClassDefOverride(FitFunctionPDF2D, 1)
};

#endif // FitFunctionPDF2D_h
