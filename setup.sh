#!/bin/bash
set -e #stop on error

cp RooPDF_HiggsAnalysis_*.h ../interface/
cp RooPDF_HiggsAnalysis_*.cxx ../src/

for header in RooPDF_HiggsAnalysis_*.h; do
  header_name="$(basename "${header}")"
  class_name="$(grep -m 1 -E '^[[:space:]]*class[[:space:]]+RooPDF_HiggsAnalysis_' "${header}" | awk '{print $2}' | sed 's/[:{].*//')"
  #class RooPDF_HiggsAnalysis_DBLGAUSS : public RooPDF_HiggsAnalysis_Base { -> RooPDF_HiggsAnalysis_DBLGAUSS
  include_line="#include \"HiggsAnalysis/CombinedLimit/interface/${header_name}\""

  grep -Fxq "${include_line}" ../src/classes.h || printf '%s\n' "${include_line}" >> ../src/classes.h
  grep -Fq "<class name=\"${class_name}\"" ../src/classes_def.xml || sed -i "\#</lcgdict>#i\\  <class name=\"${class_name}\" />" ../src/classes_def.xml
done

if [[ "${1:-}" != "--nobuild" ]]; then
  cd ..
  scram b -j
  cd ./LimitSetting
fi

