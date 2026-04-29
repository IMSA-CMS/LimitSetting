#!/bin/bash
set -e

if [[ -z "${1-}" ]]; then
  echo "Usage: $0 <commit-message>" >&2
  exit 1
fi

commit_message="$1"

headers=(../interface/RooPDF_HiggsAnalysis_*.h)
sources=(../src/RooPDF_HiggsAnalysis_*.cxx)

if (( ${#headers[@]} == 0 )); then
  echo "No RooPDF_HiggsAnalysis headers found in ../interface" >&2
  exit 1
fi

if (( ${#sources[@]} == 0 )); then
  echo "No RooPDF_HiggsAnalysis sources found in ../src" >&2
  exit 1
fi

cp "${headers[@]}" .
cp "${sources[@]}" .

git add RooPDF_HiggsAnalysis_*.h RooPDF_HiggsAnalysis_*.cxx setup.sh commit.sh
git commit -m "$commit_message"
