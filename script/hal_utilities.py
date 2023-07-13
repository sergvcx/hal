#!/usr/bin/python
import subprocess
import re

def get_addr(absname, varname):
    p = subprocess.Popen(["nmc-readelf", "-s", absname], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)     
    for line in p.stdout.readlines():
        match = re.search(varname, line.decode('utf-8'))
        if match:
            s_line = line.split()
            res = s_line[1]
            return res
    return 0

def check_abs(a, b):
    addr_a = get_addr(a, "halSyncro")
    addr_b = get_addr(b, "halSyncro")
    if (addr_a == 0) or (addr_b == 0):
        return False;
    return addr_a == addr_b

if __name__ == "__main__":
    print("Hal utilities for python scripts")