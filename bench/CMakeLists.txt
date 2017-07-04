cmake_minimum_required(VERSION 2.8)
project (bench_adios)
include (BlueGenePortability.cmake)
### configure packages variables
set(ADIOS_ROOT "" CACHE PATH "extra path to find modules")
list(APPEND CMAKE_MODULE_PATH ${ADIOS_ROOT}/etc)
find_package(ADIOS REQUIRED)
find_package(MPI REQUIRED)

### Compilation options
set(ADIOS_PROGS adios_raw_perfs adios_raw_perfs2 adios_raw_perfs3)
foreach(PROG ${ADIOS_PROGS})
  include_directories(${MPI_INCLUDE_PATH} ${ADIOS_INCLUDE_DIRS})
  add_executable(${PROG} ${PROG}.cpp)
  set_target_properties(${PROG} PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS}")
  target_link_libraries(${PROG} ${MPI_CXX_LIBRARIES} ${ADIOS_LIBRARIES})
endforeach()
