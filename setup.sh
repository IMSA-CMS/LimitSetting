#!/bin/bash
set -e

cp FitFunctionPDF.h ../interface/
cp FitFunctionPDF.cxx ../src/

grep -Fxq '#include "HiggsAnalysis/CombinedLimit/interface/FitFunctionPDF.h"' ../src/classes.h ||
  printf '%s\n' '#include "HiggsAnalysis/CombinedLimit/interface/FitFunctionPDF.h"' >> ../src/classes.h #appends

grep -Fq '<class name="FitFunctionPDF"' ../src/classes_def.xml ||
  sed -i '\#</lcgdict>#i\  <class name="FitFunctionPDF" />' ../src/classes_def.xml #insert before </lcgdict>
sed -i 's/<class name="FitFunctionPDF"[^>]*\/>/<class name="FitFunctionPDF" noStreamer="true" \/>/' ../src/classes_def.xml

grep -Eq '<use[[:space:]]+name="CMSAnalysis/Analysis"' ../BuildFile.xml ||
  sed -i '1i<use name="CMSAnalysis/Analysis"/>' ../BuildFile.xml

if [[ "${1:-}" == "--build" ]]; then
  cd ..
  scram b -j
fi
