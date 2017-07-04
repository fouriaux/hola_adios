#!/bin/bash
#
#SBATCH --job-name="bench adios"
#SBATCH --time=01:00:00
#SBATCH --mail-user=jeremy.fouriaux@epfl.ch
#SBATCH --mail-type=ALL
#SBATCH --partition=prod
#SBATCH --nodes=512
#SBATCH --ntasks-per-node=16
#SBATCH --output=benchmarkingADIOS.%j.o
#SBATCH --error=benchmarkingADIOS.%j.e
#SBATCH --account=proj16
#SBATCH --exclusive

#bug fix: for partitions >1024 mpi ranks, this will cause an issue on the output unless this flag is past:
export  BGLOCKLESSMPIO_F_TYPE=0x47504653
 
#======START=====
module purge
module load intel/parallel_studio_xe-2017u3 intel/impi-4.1.0.024
module load nix/bgq/python/2.7-full
export CC=mpiicc
export CXX=mpiicpc
export DARSHAN_DIR=/gpfs/bbp.cscs.ch/home/kumbhar/workarena/systems/lugviz/softwares/install/profilers/darshan-3.1.4-intel
export PATH=$DARSHAN_DIR/bin/:$PATH
export SLURM_ACCOUNT=proj16
export ADIOS_ROOT=/gpfs/bbp.cscs.ch/home/fouriaux/Devel/adios_dev/ADIOS/install_intel
mkdir -p build_blue/variations
mkdir -p build_blue/variations/test1
./simple_blue.py

