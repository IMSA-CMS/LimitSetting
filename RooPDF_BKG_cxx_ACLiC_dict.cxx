// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIuscmsdIhomedIkponxdICMSSW_11_3_4dIsrcdIHiggsAnalysisdICombinedLimitdIdatadItutorialsdIparametric_exercisedIcombinetutorialmI2023mIparametricdIRooPDF_BKG_cxx_ACLiC_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/uscms/home/kponx/CMSSW_11_3_4/src/HiggsAnalysis/CombinedLimit/data/tutorials/parametric_exercise/combinetutorial-2023-parametric/./RooPDF_BKG.cxx"

// Header files passed via #pragma extra_include

   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *ROOT_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("ROOT", 0 /*version*/, "Rtypes.h", 103,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &ROOT_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *ROOT_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }

namespace ROOT {
   static void *new_RooPDF_BKG(void *p = 0);
   static void *newArray_RooPDF_BKG(Long_t size, void *p);
   static void delete_RooPDF_BKG(void *p);
   static void deleteArray_RooPDF_BKG(void *p);
   static void destruct_RooPDF_BKG(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooPDF_BKG*)
   {
      ::RooPDF_BKG *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooPDF_BKG >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooPDF_BKG", ::RooPDF_BKG::Class_Version(), "RooPDF_BKG.h", 18,
                  typeid(::RooPDF_BKG), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooPDF_BKG::Dictionary, isa_proxy, 4,
                  sizeof(::RooPDF_BKG) );
      instance.SetNew(&new_RooPDF_BKG);
      instance.SetNewArray(&newArray_RooPDF_BKG);
      instance.SetDelete(&delete_RooPDF_BKG);
      instance.SetDeleteArray(&deleteArray_RooPDF_BKG);
      instance.SetDestructor(&destruct_RooPDF_BKG);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooPDF_BKG*)
   {
      return GenerateInitInstanceLocal((::RooPDF_BKG*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooPDF_BKG*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RooPDF_BKG::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooPDF_BKG::Class_Name()
{
   return "RooPDF_BKG";
}

//______________________________________________________________________________
const char *RooPDF_BKG::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooPDF_BKG*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooPDF_BKG::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooPDF_BKG*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooPDF_BKG::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooPDF_BKG*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooPDF_BKG::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooPDF_BKG*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RooPDF_BKG::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooPDF_BKG.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooPDF_BKG::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooPDF_BKG::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooPDF_BKG(void *p) {
      return  p ? new(p) ::RooPDF_BKG : new ::RooPDF_BKG;
   }
   static void *newArray_RooPDF_BKG(Long_t nElements, void *p) {
      return p ? new(p) ::RooPDF_BKG[nElements] : new ::RooPDF_BKG[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooPDF_BKG(void *p) {
      delete ((::RooPDF_BKG*)p);
   }
   static void deleteArray_RooPDF_BKG(void *p) {
      delete [] ((::RooPDF_BKG*)p);
   }
   static void destruct_RooPDF_BKG(void *p) {
      typedef ::RooPDF_BKG current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooPDF_BKG

namespace ROOT {
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary();
   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p);
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<double> >*)
   {
      vector<vector<double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<double> >", -2, "vector", 386,
                  typeid(vector<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<double> >) );
      instance.SetNew(&new_vectorlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<double> > >()));

      ::ROOT::AddClassAlternate("vector<vector<double> >","std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<double> >*)0x0)->GetClass();
      vectorlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> > : new vector<vector<double> >;
   }
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> >[nElements] : new vector<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete ((vector<vector<double> >*)p);
   }
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((vector<vector<double> >*)p);
   }
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p) {
      typedef vector<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<double> >

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 386,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace {
  void TriggerDictionaryInitialization_RooPDF_BKG_cxx_ACLiC_dict_Impl() {
    static const char* headers[] = {
"./RooPDF_BKG.cxx",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/lcg/root/6.22.08-ljfedo/include",
"/uscms/home/kponx/CMSSW_11_3_4/src",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/cms/cmssw/CMSSW_11_3_4/src",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/cms/coral/CORAL_2_3_21-ljfedo7/include/LCG",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/mctester/1.25.0a-ljfedo5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/dd4hep/v01-16-01-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/herwig7/7.2.2-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/tauolapp/1.1.8-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/charybdis/1.003-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/sherpa/2.2.11-ljfedo3/include/SHERPA-MC",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/thepeg/2.2.2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/pythia8/303-ljfedo3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/herwig/6.521-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/lwtnn/2.10-ljfedo4/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/lhapdf/6.2.3-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/cgal/4.2-ljfedo4/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/starlight/r193-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/rivet/3.1.3-ljfedo5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/highfive/2.0-ljfedo4/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/geant4/10.7.1-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/classlib/3.1.3-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/tkonlinesw/4.2.0-1_gcc7-ljfedo5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libungif/4.1.4-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libtiff/4.0.10-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libpng/1.6.37-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/frontier_client/2.9.1-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/pcre/8.43-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/zstd/1.4.5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/xrootd/4.12.3/include/xrootd/private",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/cms/vdt/0.4.0-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/valgrind/3.15.0-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/utm/utm_0.8.1-ljfedo4/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/toprex/4.23-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/tauola/27.121.5-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/sigcpp/2.6.2-bcolbf2/include/sigc++-2.0",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/sqlite/3.22.0-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/pacparser/1.3.5-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/oracle/12.1.0.2.0-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/py2-numpy/1.16.6-ljfedo2/c-api/core/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/meschach/1.2.pCMS1-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/lz4/1.9.2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libuuid/2.34-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libunwind/1.4.0/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/ktjet/1.06-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/jimmy/4.2-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/jemalloc/5.2.1-ghbfee3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/hls/2019.08-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/madgraph5amcatnlo/2.7.3",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/heppdt/3.04.01-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/hector/1.3.4_patch1-ljfedo5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/gsl/2.6-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/protobuf/3.15.1-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libjpeg-turbo/2.0.2-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/giflib/5.2.0-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/gdbm/1.10-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/freetype/2.10.0-bcolbf3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fftw3/3.3.8-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fftjet/1.5.0-bcolbf3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet/3.4.0-beta/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/hepmc/2.06.10-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/dpm/1.8.0.1-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/expat/2.1.0-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/vecgeom/v1.1.8-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/vecgeom/v1.1.8-ljfedo2/include/VecGeom",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/xerces-c/3.1.3-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/xz/5.2.4-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/lcg/root/6.22.08-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/dcap/2.47.12-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/libxml2/2.9.10/include/libxml2",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/openssl/1.0.2d-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/curl/7.70.0/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/cppunit/1.15.x-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/clhep/2.4.4.0-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/pythia6/426-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/photos/215.5-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/zlib/1.2.11-bcolbf/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/cascade/2.2.04-ljfedo2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/bz2lib/1.0.6-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/python/2.7.15-bcolbf2/include/python2.7",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/tbb/2020_U3-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/cuda/11.2.2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/boost/1.75.0-ljfedo/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/xgboost/1.3.3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/tinyxml2/6.2.0-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/ittnotify/16.06.18-ghbfee/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/gosamcontrib/2.0-20150803-bcolbf2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fmt/7.0.1/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/eigen/011e0db31d1bed8b7f73662be6d57d9f30fa457a/include/eigen3",
"/usr/local/include",
"/usr/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/lcg/root/6.22.08-ljfedo/etc/",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/lcg/root/6.22.08-ljfedo/etc//cling",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/lcg/root/6.22.08-ljfedo/include/",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc900/lcg/root/6.22.08-ljfedo/include/",
"/uscms/home/kponx/CMSSW_11_3_4/src/HiggsAnalysis/CombinedLimit/data/tutorials/parametric_exercise/combinetutorial-2023-parametric/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "RooPDF_BKG_cxx_ACLiC_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(file_name@@@/uscms/home/kponx/CMSSW_11_3_4/src/HiggsAnalysis/CombinedLimit/data/tutorials/parametric_exercise/combinetutorial-2023-parametric/./RooPDF_BKG.h)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Your description goes here...)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooPDF_BKG.h")))  __attribute__((annotate("$clingAutoload$./RooPDF_BKG.cxx")))  RooPDF_BKG;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "RooPDF_BKG_cxx_ACLiC_dict dictionary payload"

#ifndef __ACLIC__
  #define __ACLIC__ 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "./RooPDF_BKG.cxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"", payloadCode, "@",
"ROOT::GenerateInitInstance", payloadCode, "@",
"RooPDF_BKG", payloadCode, "@",
"RooPDF_BKG::fgIsA", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("RooPDF_BKG_cxx_ACLiC_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_RooPDF_BKG_cxx_ACLiC_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_RooPDF_BKG_cxx_ACLiC_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_RooPDF_BKG_cxx_ACLiC_dict() {
  TriggerDictionaryInitialization_RooPDF_BKG_cxx_ACLiC_dict_Impl();
}
