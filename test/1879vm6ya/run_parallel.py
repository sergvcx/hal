#!/usr/bin/python
# TODO check addr halSynchro


import subprocess
import sys
import os
from inspect import currentframe, getframeinfo

hal_path = os.getenv("HAL")
if hal_path == "":
    print("Warning: HAL not found")
else:
    sys.path.append(hal_path + "/script")
    import hal_utilities    
    if hal_utilities.check_abs(sys.argv[1], sys.argv[2]) == False:
        frameinfo = getframeinfo(currentframe())
        print("check_abs failed: ", frameinfo.filename, frameinfo.lineno)
        sys.exit(1)

proc_timeout = 5
if len(sys.argv) < 3:
	print("Wrong arguments")
	sys.exit(1)
process0 = subprocess.Popen(['mc12101run', sys.argv[1], '-a0', '-p', '-v', '-q'])
process1 = subprocess.Popen(['mc12101run', sys.argv[2], '-a1', '-p', '-v', '-q'])
process0.communicate()
process1.communicate()
result0 = -1
result1 = -1
try:
	result0 = process0.wait(timeout=proc_timeout)
except subprocess.TimeoutExpired:
	process0.terminate()
	print("Timeout program 0")
try:
	result1 = process1.wait(timeout=30)
except subprocess.TimeoutExpired:
	process1.terminate()
	print("Timeout program 1")
result = result0 | result1
if result != 0:
	sys.exit(result)
