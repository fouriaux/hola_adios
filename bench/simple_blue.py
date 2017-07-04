#!/bin/env python
import subprocess
import os
import sys

nb_procs  = [8192]
# writing 1 Go per proc
#sizes     = [1000000, 10000000, 100000000,  1000000000]
#sizes     = [500000, 1000000, 2000000, 100000000]

# 1 M, 4M, 8M, 16M
sizes      = [1048576, 4194304, 8388608, 16777216]
trnasports = ["MPI", "MPI_Aggregate", "BGQ"]
env        = dict(os.environ)
exec_name  = "/gpfs/bbp.cscs.ch/home/fouriaux/Devel/adios_dev/hola_adios/bench/build_blue/adios_raw_perfs"
#env["LD_PRELOAD"] = "/gpfs/bbp.cscs.ch/home/kumbhar/workarena/systems/lugviz/softwares/install/profilers/darshan-3.1.4-intel/lib/libdarshan.so"
for p in nb_procs:
  for s in sizes:
    for t in transports:
      for i in range (10)
        command = "mpirun -n {p} {exec_name} {size}".format(p=p, exec_name=exec_name, size=s)
        env["DARSHAN_LOGFILE"]  = "/gpfs/bbp.cscs.ch/home/fouriaux/Devel/adios_dev/hola_adios/bench/build_blue/variations/test1/{p}_{s}_{t}_{sample}.darshan".format(p=p, s=s, sample=i, t=t)
        output = subprocess.check_output(command.split(), env=env)
