      _ _           _            _       _ _           _ 
     (_) |__   ___ | | __ _     / \   __| (_) ___  ___| |
     | | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |
     | | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|
     \_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_)

===============



0_hola_adios.cpp
---
  Simple Write of ADIOS bp file using MPI
  Each Process writes his own block.
  results can be read using the following ADIOS utility:
  > $ADIOS_ROOT/bin/bpls --string -d data/0.bp /hola_msg
   byte     /hola_msg   {3}
     (0)    "Ho"

1_hola_adios.cpp
---
  Simple Read of different blocks for each Process and agregate results


2_hola_adios.cpp
---
  Usage of Global Array.
NOTES:
  in XML coordination-communicator attribute must be specified on group that use global arrays (error message not clear...)

Note the difference when now dumping file:
  > $ADIOS_ROOT/bin/bpls --string --dump data/2.bp /hola_msg 
  byte     /hola_msg    {12}
    ( 0)    "!Hola Adios!"

3_hola_adios.cpp
---
  Writing a Structure, HDF5 or ADIOS format


4_hola_adios.cpp
---
  Multi-threaded only

4_hola_hdf5.cpp
---
  Reading a Structure in HDF5


5_realistic_structure.cpp
---
  Save Pre-Synaptic structures 


6_hola_hdf5.py
---
Example of writing an HDF5 using python API

