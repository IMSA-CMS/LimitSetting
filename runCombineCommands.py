#!/usr/bin/env python3

import subprocess
import re
import random
import os
import time
import argparse
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed

parser = argparse.ArgumentParser(description="Run Combine over the mass points.")
parser.add_argument("-j", "--cpus", type=int, default=1,
	help="Number of mass points to run simultaneously (default: 1)")
args = parser.parse_args()
if args.cpus < 1:
	parser.error("--cpus must be at least 1")

lowerBound = 500
upperBound = 1500
step = 100
n = int((upperBound - lowerBound) / step)
massArray = []
for  i in range(n + 1):
	massArray.append(lowerBound + i * step)

#file_name = "eeee-uuuuChannelsOnly.root"
#file_name = "eeeeChannelOnly.root"
#file_name = "Signal_datacards/datacard_Higgs_Combined_No_eeuu.root" 
# Use this for no systematics (make sure to put systematics here as final)
#file_name = "Signal_datacards/datacard_Higgs_Combined.root"
# Use this for no systematics testing with only a single channel
#file_name = "Signal_datacards/datacard_eeee_X_copy.root"

# Use this for systematics testing
#file_name = "Signal_datacards/datacard_Higgs_Combined_copy.root"
#file_name = "Signal_datacards/datacard_Higgs_Combined_copy2.root"
#file_name = "Signal_datacards/datacard_Higgs_Combined.root"
#file_name = "Signal_datacards/datacard_Higgs_Combined_copy3.root"

#file_name = "datacard_Higgs_Combined.root"
# Use this for systematics testing for only a single channel
#file_name = "Signal_datacards/datacard_eeee_X_copyForSystematicsTesting.root"



# Testing for limit improvement via number of channels
#file_name = "eeeeChannelOnly.root"
#file_name = "datacard_Higgs_Combined_eeee_eeeu.root"
#file_name = "datacard_Higgs_Combined_eeee_eeeu_eeuu.root"
#file_name = "datacard_Higgs_Combined_eeee_eeeu_eeuu_eueu.root"
#file_name = "datacard_Higgs_Combined_eeee_eeeu_eeuu_eueu_euuu.root"
#file_name = "datacard_Higgs_Combined_eeee_eeeu_eeuu_eueu_euuu.root"
#file_name = "Signal_datacards/datacard_Higgs_Combined.root"

#Datacard tests
#file_name = "eeeeChannelOnly_copy.root"
file_name = "datacard.root"
file_name = os.path.abspath(file_name)


print(os.listdir())

terminal_outputs = [None] * len(massArray)

# Note: When using AsymptoticLimits, comment out the --tries, -i, and "t" settings
def run_mass(mass):
	start_time = time.monotonic()
	thread = threading.current_thread().name
	print(f"mass point {mass} dispatched to thread {thread}", flush=True)
	# Combine's fixed logger filename requires a separate working directory per mass.
	work_directory = f"combine_mass{mass}"
	os.makedirs(work_directory, exist_ok=True)
	
	result = subprocess.run(
	[
		# "nohup",
		"combine",
		file_name,
		#"-M", "AsymptoticLimits",
		"-M", "MarkovChainMC",
		"--freezeParameters", "realHiggsMass,b_ee,b_eu",
		"--setParameters", f"realHiggsMass={mass},b_ee=1,b_eu=1",
		"-m", "1000",
		 #"--tries", "200",
		 "-n", f"HiggsCombine{mass}",
		"-i", "50000",
		"-b", "200",
		"-t", "1",
		"--rMax", "100",
		"-s", str(random.randint(1,999999)),
		"--toysNoSystematics",
		"-v", "2"
	],
	cwd = work_directory,
	capture_output = True,
	text = True)
	
	with open(f"combine_mass{mass}.stdout.log", "w") as file:
		file.write(result.stdout)
	with open(f"combine_mass{mass}.stderr.log", "w") as file:
		file.write(result.stderr)
	logger_path = os.path.join(work_directory, "combine_logger.out")
	if os.path.exists(logger_path):
		os.replace(logger_path, f"combine_mass{mass}.logger.out")
	for name in os.listdir(work_directory):
		if name.startswith("higgsCombine") and name.endswith(".root"):
			os.replace(os.path.join(work_directory, name), name)
	
	elapsed_time = time.monotonic() - start_time
	print(f"mass point {mass} returned from thread {thread}", flush=True)
	return result, elapsed_time

with ThreadPoolExecutor(max_workers=min(args.cpus, len(massArray)), thread_name_prefix="worker") as executor:
	futures = {executor.submit(run_mass, mass): (index, mass) for index, mass in enumerate(massArray)}
	for completed, future in enumerate(as_completed(futures), start=1):
		index, mass = futures[future]
		result, elapsed_time = future.result()
		terminal_outputs[index] = result.stdout
		print(f"{completed}/{len(massArray)} finished (mass {mass}, took {elapsed_time:.2f} seconds, exit code {result.returncode})", flush=True)

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



with open('higgsLimits.txt', 'w') as file:
	file.write("mass".ljust(8) + "median Limit".ljust(16) + "1_Sigma Lower".ljust(16) + "1_Sigma Upper".ljust(16) + "2_Sigma Lower".ljust(16) + "2_Sigma Upper".ljust(16) + "\n")
	for i in range(n+1):
		file.write(f"{str(massArray[i]).ljust(7)} {str(median_limit_array[i]).ljust(15)} {str(band_68_array[i][0]).ljust(15)} {str(band_68_array[i][1]).ljust(15)} {str(band_95_array[i][0]).ljust(15)} {str(band_95_array[i][1]).ljust(15)} \n")
	
#print(result)

with open('result.txt', 'w') as file:
	for terminal_output in terminal_outputs:
		#file.write("Writing new mass point")
		file.write(str(terminal_output))
