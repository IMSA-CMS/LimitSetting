#include "../interface/FitFunctionPDF2D.h"
#include "TBufferFile.h"
#include "TClass.h"

#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <utility>

ClassImp(FitFunctionPDF2D);

FitFunctionPDF2D::FitFunctionPDF2D(const char *name, const char *title,
                        RooAbsReal& _x, RooAbsReal& _y,
                        RooAbsReal& _realHiggsMass,
                        RooAbsReal& _branch_ratio_1,
                        RooAbsReal& _branch_ratio_2,
                        RooAbsReal& _norm_Systematic,
                        RooAbsReal& _shape_Systematic,
                        std::shared_ptr<FitFunction> modelX,
                        std::shared_ptr<FitFunction> modelY)
   : RooAbsPdf(name,title),
   x("x","x",this,_x),
   y("y","y",this,_y),
   realHiggsMass("realHiggsMass","realHiggsMass",this,_realHiggsMass),
   branch_ratio_1("branch_ratio_1","branch_ratio_1",this,_branch_ratio_1),
   branch_ratio_2("branch_ratio_2","branch_ratio_2",this,_branch_ratio_2),
   norm_Systematic("norm_Systematic","norm_Systematic",this,_norm_Systematic),
   shape_Systematic("shape_Systematic","shape_Systematic",this,_shape_Systematic),
   functionX(std::move(modelX)),
   functionY(std::move(modelY))
{
   if (!functionX || !functionY)
      throw std::invalid_argument("FitFunctionPDF2D requires an X and a Y model");
}

FitFunctionPDF2D::FitFunctionPDF2D(const char *name, const char *title,
                        RooAbsReal& _x, RooAbsReal& _y,
                        RooAbsReal& _realHiggsMass,
                        RooAbsReal& _branch_ratio_1,
                        RooAbsReal& _branch_ratio_2,
                        RooAbsReal& _norm_Systematic,
                        RooAbsReal& _shape_Systematic,
                        std::shared_ptr<FitFunction> modelX,
                        std::shared_ptr<FitFunction> modelY,
                        const std::vector<std::string>& systematicNamesX,
                        const RooArgList& deltasX,
                        const std::vector<std::string>& systematicNamesY,
                        const RooArgList& deltasY)
   : FitFunctionPDF2D(name, title, _x, _y, _realHiggsMass, _branch_ratio_1, _branch_ratio_2,
                      _norm_Systematic, _shape_Systematic, std::move(modelX), std::move(modelY))
{
   addSystematics(*functionX, systematicNamesX, deltasX, shapeSystematicNamesX, shapeSystematicIndicesX);
   addSystematics(*functionY, systematicNamesY, deltasY, shapeSystematicNamesY, shapeSystematicIndicesY);
}

FitFunctionPDF2D::FitFunctionPDF2D(FitFunctionPDF2D const &other, const char *name)
   : RooAbsPdf(other,name),
   x("x",this,other.x),
   y("y",this,other.y),
   realHiggsMass("realHiggsMass",this,other.realHiggsMass),
   branch_ratio_1("branch_ratio_1",this,other.branch_ratio_1),
   branch_ratio_2("branch_ratio_2",this,other.branch_ratio_2),
   norm_Systematic("norm_Systematic",this,other.norm_Systematic),
   shape_Systematic("shape_Systematic",this,other.shape_Systematic),
   shape_Systematics("shape_Systematics",this,other.shape_Systematics),
   functionX(other.functionX),
   functionY(other.functionY),
   shapeSystematicNamesX(other.shapeSystematicNamesX),
   shapeSystematicIndicesX(other.shapeSystematicIndicesX),
   shapeSystematicNamesY(other.shapeSystematicNamesY),
   shapeSystematicIndicesY(other.shapeSystematicIndicesY)
{
}

void FitFunctionPDF2D::addSystematics(const FitFunction& function, const std::vector<std::string>& systematicNames,
                                      const RooArgList& deltas, std::vector<std::string>& names, std::vector<int>& indices)
{
   if (systematicNames.size() != static_cast<std::size_t>(deltas.getSize()))
      throw std::invalid_argument("Systematic names must match the delta list");

   std::set<std::string> seen;
   const auto availableSystematics = function.listSystematics();
   for (std::size_t i = 0; i < systematicNames.size(); ++i)
   {
      const auto& systematic = systematicNames[i];
      const auto* delta = dynamic_cast<const RooAbsReal*>(deltas.at(static_cast<int>(i)));
      if (!delta)
         throw std::invalid_argument("Shape deltas must be RooAbsReal objects");
      if (!seen.insert(systematic).second)
         throw std::invalid_argument("Duplicate shape systematic: " + systematic);
      if (std::find(availableSystematics.begin(), availableSystematics.end(), systematic) == availableSystematics.end())
         throw std::invalid_argument("Unknown or incomplete shape systematic: " + systematic);
      // The same nuisance may be used by both functions, so only add it to the proxy list once
      if (!shape_Systematics.find(delta->GetName()))
         shape_Systematics.add(*delta);
      names.push_back(systematic);
      indices.push_back(shape_Systematics.index(delta->GetName()));
   }
}

FitFunction::NuisanceValues FitFunctionPDF2D::getNuisances(const std::vector<std::string>& names, const std::vector<int>& indices) const
{
   FitFunction::NuisanceValues nuisances;
   for (std::size_t i = 0; i < names.size(); ++i)
   {
      // read through the proxy so clones/imported pdfs use redirected nuisances
      const double delta = static_cast<const RooAbsReal*>(shape_Systematics.at(indices[i]))->getVal();
      if (!std::isfinite(delta))
         throw std::invalid_argument("Shape-systematic deltas must be finite");
      nuisances.emplace(names[i], delta);
   }
   return nuisances;
}

double FitFunctionPDF2D::evaluate() const
{
   const double mass = realHiggsMass.arg().getVal();
   return functionX->evaluate(x.arg().getVal(), mass, getNuisances(shapeSystematicNamesX, shapeSystematicIndicesX)) *
          functionY->evaluate(y.arg().getVal(), mass, getNuisances(shapeSystematicNamesY, shapeSystematicIndicesY));
}

RooFormulaVar FitFunctionPDF2D::signal_norm(std::string channel_name) const
{
   std::string normExpression = "@1 * (" + functionX->getNormExpression("@0") + ")";
   std::string normName = channel_name + "_norm";

   return RooFormulaVar(normName.c_str(), normName.c_str(), normExpression.c_str(),
                        RooArgList(*realHiggsMass.absArg(), *norm_Systematic.absArg()));
}

// Custom streamer because ROOT can't do shared_ptr io; see FitFunctionPDF::Streamer
void FitFunctionPDF2D::Streamer(TBuffer &buffer)
{
   const auto *modelClass = TClass::GetClass(typeid(FitFunction));
   if (buffer.IsReading())
   {
      UInt_t start, count;
      const auto version = buffer.ReadVersion(&start, &count);
      buffer.ReadClassBuffer(FitFunctionPDF2D::Class(), this, version, start, count);
      for (auto* function : {&functionX, &functionY})
      {
         std::string data;
         buffer.ReadStdString(&data);
         TBufferFile modelBuffer(TBuffer::kRead, data.size(), data.data(), false);
         function->reset(static_cast<FitFunction*>(modelBuffer.ReadObjectAny(modelClass)));
         if (!*function)
            throw std::runtime_error("Can't read the FitFunctionPDF2D model");
      }
   }
   else
   {
      if (!functionX || !functionY)
         throw std::runtime_error("Can't write a FitFunctionPDF2D without both models");
      buffer.WriteClassBuffer(FitFunctionPDF2D::Class(), this);
      for (const auto& function : {functionX, functionY})
      {
         TBufferFile modelBuffer(TBuffer::kWrite);
         modelBuffer.WriteObjectAny(function.get(), modelClass);
         const std::string data(modelBuffer.Buffer(), modelBuffer.Length());
         buffer.WriteStdString(&data);
      }
   }
}
