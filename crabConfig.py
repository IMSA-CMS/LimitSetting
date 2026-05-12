import CRABClient
from CRABClient.UserUtilities import config
# config = config()

# config.General.requestName   = 'test1'

# config.JobType.pluginName = 'PrivateMC'
# # Name of the CMSSW configuration file
# config.JobType.psetName = 'pset.py'

# # This string determines the primary dataset of the newly-produced outputs.
# # For instance, this dataset will be named /CrabTestSingleMu/something/USER
# config.Data.outputPrimaryDataset = 'CrabTestSingleMu'
# config.Data.splitting = 'EventBased'
# config.Data.unitsPerJob = 100
# config.Data.totalUnits = 1000
# config.Data.publication = True

# # This string is used to construct the output dataset name
# config.Data.outputDatasetTag = 'CRAB3_MC_generation_test1'

# # Where the output files will be transmitted to
# config.Site.storageSite = 'T3_US_FNALLPC'

config = config()
config.General.requestName = 'CombineLimits_NoSystematics_eeee_uuuuChannelsOnly'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.instance = 'prod'
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'
#config.JobType.inputFiles = ['eeee-uuuuChannelsOnly.root', 'RooPDF_DSCB_test_cxx.so', 'RooPDF_BKG_cxx.so', 'Signal_datacards/datacard_Higgs_Combined_copy3.root', 'realHiggsMass,medianLimit_fromCombineCommands.txt', 'FrameworkJobReport.xml', 'input/']
config.JobType.inputFiles = ['datacard_Higgs_Combined.root']
config.JobType.scriptExe = 'runCombineCommands.py'
config.JobType.outputFiles = ['testCombine_NoSystematics_eeee_uuuuChannelsOnly.root']
config.Data.inputDataset = '/Tau/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
config.Site.storageSite = 'T3_US_FNALLPC'