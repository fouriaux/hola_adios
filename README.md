      _ _           _            _       _ _           _ 
     (_) |__   ___ | | __ _     / \   __| (_) ___  ___| |
     | | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |
     | | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|
     \_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_)

===============

HELLO WORLD LIKE EXAMPLES
---

### 0_hola_adios.cpp

  Simple Write of ADIOS bp file using MPI
  Each Process writes his own block.
  results can be read using the following ADIOS utility:
  > $ADIOS_ROOT/bin/bpls --string -d data/0.bp /hola_msg
   byte     /hola_msg   {3}
     (0)    "Ho"

### 1_hola_adios.cpp

  Simple Read of different blocks for each Process and agregate results


### 2_hola_adios.cpp

  Usage of Global Array.
NOTES:
  in XML coordination-communicator attribute is mandatory must be specified on group that use global arrays (error message not clear...)

Note the difference when now dumping file:
  > $ADIOS_ROOT/bin/bpls --string --dump data/2.bp /hola_msg 
  byte     /hola_msg    {12}
    ( 0)    "!Hola Adios!"

### 3_hola_adios.cpp

  Writing a Structure, HDF5 or ADIOS format


### 4_hola_adios.cpp

  Multi-threaded example, crash ADIOS

### 5_hola_hdf5.cpp

  Writing HDF5 

### 6_hola
  Illegally writing two times the same variable using XML API.


MORE ADVANCED SYNTHETIC EXAMPLES
---

### 7_report.cpp

This sample demonstrates that we can use ADIOS to write different sizes per rank

running sample : ./srun -n 4 ./7_report 100 0.1
Checking results
```
> bpls --dump no_xml
  integer  global_size  scalar
100 

  integer  batch_size   scalar
23 

  integer  offset       scalar
0 

  real     data         {100}
    ( 0)    0 0 0 0 0 0
    ( 6)    0 0 0 0 0 0
    (12)    0 0 0 0 0 0
    (18)    0 0 0 0 0 3
    (24)    3 3 3 3 3 3
    (30)    3 3 3 3 3 3
    (36)    3 3 3 3 3 3
    (42)    3 3 3 3 3 3
    (48)    3 3 2 2 2 2
    (54)    2 2 2 2 2 2
    (60)    2 2 2 2 2 2
    (66)    2 2 2 2 2 2
    (72)    2 2 2 2 2 1
    (78)    1 1 1 1 1 1
    (84)    1 1 1 1 1 1
    (90)    1 1 1 1 1 1
    (96)    1 1 1 1 
```

### 8_report.cpp

This sample demonstrates that the API works correctly with automatic C scalar variables

### 9_report.cpp

This sample demonstrates the usage of the no XML API, from previous example


### 10_report.cpp
This sample demonstrate the usage of writing sub-blocks using the no XML interface.
expected output:
 The trick here is to declare as many times as needed the array name using the no_xml api before using it
```
bpls --dump no_xml_several_chunk
  integer  global_size  scalar
96 

  integer  batch_size   scalar
12 

  integer  offset       scalar
48 

  real     data         {96}
    ( 0)    2 2 2 2 2 2
    ( 6)    2 2 2 2 2 2
    (12)    1 1 1 1 1 1
    (18)    1 1 1 1 1 1
    (24)    3 3 3 3 3 3
    (30)    3 3 3 3 3 3
    (36)    3 3 3 3 3 3
    (42)    3 3 3 3 3 3
    (48)    0 0 0 0 0 0
    (54)    0 0 0 0 0 0
    (60)    2 2 2 2 2 2
    (66)    2 2 2 2 2 2
    (72)    1 1 1 1 1 1
    (78)    1 1 1 1 1 1
    (84)    0 0 0 0 0 0
    (90)    0 0 0 0 0 0
```


