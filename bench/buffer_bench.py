#!/bin/env python
import subprocess
import os
import sys

nb_procs          = [16, 32, 64]
# writing 1 Go per proc
#sizes     = [1000000, 10000000, 100000000,  1000000000]
#sizes     = [500000, 1000000, 2000000, 100000000]

# size of batch of 65536 * 16 Proc ends in 1 M limit

sizes             = [65536/16, 65536/2, 65536, 65536*2]
methods           = ["MPI", "MPI_Aggregate"]
buffer_sizes      = [1]
env               = dict(os.environ)
exec_name         = "/gpfs/bbp.cscs.ch/home/fouriaux/Devel/adios_dev/hola_adios/bench/build/adios_raw_perfs2"
env["LD_PRELOAD"] = "/gpfs/bbp.cscs.ch/home/kumbhar/workarena/systems/lugviz/softwares/install/profilers/darshan-3.1.4-intel/lib/libdarshan.so"
for p in nb_procs:
  for s in sizes:
    for m in methods:
      for buf in buffer_sizes:
        for i in range(10):
          command = "mpirun -n {p} {exec_name} {size} {buffer} {method}".format(p=p, exec_name=exec_name, size=s, buffer=buf, method=m)
          print (command)
          env["DARSHAN_LOGFILE"]  = "/gpfs/bbp.cscs.ch/home/fouriaux/Devel/adios_dev/hola_adios/bench/build/variations/test2/{p}_{s}_{m}_{sample}.darshan".format(p=p, s=s, m=m, sample=i)
          output = subprocess.check_output(command.split(), env=env)
