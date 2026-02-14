import subprocess
import re

lowerBound = 600
upperBound = 1500
step = 100
n = int((upperBound - lowerBound) / step)
massArray = []
for  i in range(n + 1):
	massArray.append(lowerBound + i * step)

# Use this for no systematics (make sure to put systematics here as final)
#file_name = "Signal_datacards/datacard_Higgs_Combined.root"
# Use this for no systematics testing with only a single channel
#file_name = "Signal_datacards/datacard_eeee_X_copy.root"

# Use this for systematics testing
#file_name = "Signal_datacards/datacard_Higgs_Combined_copy.root"
#file_name = "Signal_datacards/datacard_Higgs_Combined_copy2.root"
file_name = "Signal_datacards/datacard_Higgs_Combined.root"
# Use this for systematics testing for only a single channel
#file_name = "Signal_datacards/datacard_eeee_X_copyForSystematicsTesting.root"

terminal_outputs = []
counter = 0
for mass in massArray:
	
	result = subprocess.run(
	[
		"combine",
		file_name,
		"-M", "MarkovChainMC",
		"--freezeParameters", "realHiggsMass,b_ee,b_eu",
		"--setParameters", f"realHiggsMass={mass},b_ee=1,b_eu=1",
		"-m", "1000",
		"-L", "RooPDF_DSCB_test_cxx.so",
		"-L", "RooPDF_BKG_cxx.so",
		"-L", "RooPDF_DBLGAUSS_cxx.so",
		 #"--tries", "200",
		"-i", "20000",
		"-b", "20",
		"-t", "1",
		"--toysNoSystematics",
		"-v", "3"
	],
	capture_output = True,
	text = True)
	
	terminal_outputs.append(result.stdout)
	# terminal_outputs.append(result.stderr)
	print(mass)
	# print(terminal_outputs)

median_limit_array = []
band_68_array = []
band_95_array = []

for terminal_output in terminal_outputs:
	median_match= re.search(r"median expected limit: r < ([\d\.]+)", terminal_output)
	band_68_match = re.search(r"68% expected band : ([\d\.]+) < r < ([\d\.]+)", terminal_output)
	band_95_match = re.search(r"95% expected band : ([\d\.]+) < r < ([\d\.]+)", terminal_output)

	band_68 = (float(band_68_match.group(1)), float(band_68_match.group(2))) if band_68_match else (None,None)
	band_95 = (float(band_95_match.group(1)), float(band_95_match.group(2))) if band_95_match else (None,None)
	median_limit = float(median_match.group(1)) if median_match else None

	median_limit_array.append(median_limit)
	band_68_array.append(band_68)
	band_95_array.append(band_95)


with open('realHiggsMass,medianLimit_fromCombineCommands.txt', 'w') as file:
	file.write("mass".ljust(8) + "median Limit".ljust(16) + "1_Sigma Lower".ljust(16) + "1_Sigma Upper".ljust(16) + "2_Sigma Lower".ljust(16) + "2_Sigma Upper".ljust(16) + "\n")
	for i in range(n+1):
		file.write(f"{str(massArray[i]).ljust(7)} {str(median_limit_array[i]).ljust(15)} {str(band_68_array[i][0]).ljust(15)} {str(band_68_array[i][1]).ljust(15)} {str(band_95_array[i][0]).ljust(15)} {str(band_95_array[i][1]).ljust(15)} \n")
	

