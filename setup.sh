#!/bin/bash
set -e

cp FitFunctionPDF.h ../interface/
cp FitFunctionPDF.cxx ../src/

grep -Fxq '#include "HiggsAnalysis/CombinedLimit/interface/FitFunctionPDF.h"' ../src/classes.h ||
  printf '%s\n' '#include "HiggsAnalysis/CombinedLimit/interface/FitFunctionPDF.h"' >> ../src/classes.h #appends

grep -Fq '<class name="FitFunctionPDF"' ../src/classes_def.xml ||
  sed -i '\#</lcgdict>#i\  <class name="FitFunctionPDF" />' ../src/classes_def.xml #insert before </lcgdict>

if [[ "${1:-}" == "--build" ]]; then
  cd ..
  scram b -j
fi
