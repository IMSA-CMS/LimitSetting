void rootlogon()
{
	gROOT -> LoadMacro("FitFunction.cc+");
	gROOT -> LoadMacro("FitFunctionCollection.cc+");
	gROOT -> LoadMacro("RooPDF_DSCB_test.cxx+");
	gROOT -> LoadMacro("RooPDF_BKG.cxx+");
<<<<<<< HEAD
	gROOT -> LoadMacro("RooPDF_HiggsAnalysis_Base.cxx+");
	gROOT -> LoadMacro("RooPDF_HiggsAnalysis_DSCB.cxx+");
=======
>>>>>>> 26faa2700ab3e558e75938fa85e04194e47df49f
}