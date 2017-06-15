### configure packages variables
set(ADIOS_ROOT "" CACHE PATH "extra path to find modules")
list(APPEND CMAKE_MODULE_PATH ${ADIOS_ROOT}/etc)
find_package(ADIOS REQUIRED)
find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

### Compilation options
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
set(ADIOS_PROGS 0_hola_adios 1_hola_adios 2_hola_adios 3_hola_adios 6_hola_adios 7_report 8_report 9_report 10_report)
foreach(PROG ${ADIOS_PROGS})
  include_directories(${MPI_INCLUDE_PATH} ${ADIOS_INCLUDE_DIRS})
  add_executable(${PROG} ${PROG}.cpp)
  set_target_properties(${PROG} PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS}")
  target_link_libraries(${PROG} ${MPI_CXX_LIBRARIES} ${ADIOS_LIBRARIES})
endforeach()

add_executable (4_hola_adios 4_hola_adios.cpp)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set_target_properties(${PROG} PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS} ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS}")
target_link_libraries(4_hola_adios ${MPI_CXX_LIBRARIES} ${ADIOS_LIBRARIES})

