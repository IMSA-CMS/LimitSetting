void rootlogon()
{
	gROOT -> LoadMacro("FitFunction.cc+");
	gROOT -> LoadMacro("FitFunctionCollection.cc+");
	gROOT -> LoadMacro("RooPDF_DSCB.cxx+");
	gROOT -> LoadMacro("RooPDF_DBLGAUSS.cxx+");
	gROOT -> LoadMacro("RooPDF_DSCB_test.cxx+");
	// gROOT -> LoadMacro("RooPDF_BKG.cxx+");
	gROOT -> LoadMacro("RooPDF_HiggsAnalysis_Base.cxx+");
	gROOT -> LoadMacro("RooPDF_HiggsAnalysis_DSCB.cxx+");
	gROOT -> LoadMacro("RooPDF_HiggsAnalysis_BKG.cxx+");
}